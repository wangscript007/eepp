#include <ctime>
#include <eepp/scene/scenemanager.hpp>
#include <eepp/system/filesystem.hpp>
#include <eepp/system/sys.hpp>
#include <eepp/ui/models/filesystemmodel.hpp>
#include <eepp/ui/uiscenenode.hpp>
#include <iomanip>
#include <iostream>

using namespace EE::Scene;

namespace EE { namespace UI { namespace Models {

FileSystemModel::Node::Node( const std::string& rootPath, const FileSystemModel& model ) :
	mInfo( FileSystem::getRealPath( rootPath ) ) {
	mInfoDirty = false;
	mName = FileSystem::fileNameFromPath( mInfo.getFilepath() );
	mMimeType = "";
	traverseIfNeeded( model );
}

FileSystemModel::Node::Node( FileInfo&& info, FileSystemModel::Node* parent ) :
	mParent( parent ), mInfo( info ) {
	mInfoDirty = false;
	mName = FileSystem::fileNameFromPath( mInfo.getFilepath() );
	if ( !mInfo.isDirectory() ) {
		mMimeType = "filetype-" + FileSystem::fileExtension( mName );
	} else {
		mMimeType = "folder";
	}
}

const std::string& FileSystemModel::Node::fullPath() const {
	return mInfo.getFilepath();
}

ModelIndex FileSystemModel::Node::index( const FileSystemModel& model, int column ) const {
	if ( !mParent )
		return {};
	for ( size_t row = 0; row < mParent->mChildren.size(); ++row ) {
		if ( &mParent->mChildren[row] == this )
			return model.createIndex( row, column, const_cast<Node*>( this ) );
	}
	eeASSERT( false );
	return {};
}

void FileSystemModel::Node::traverseIfNeeded( const FileSystemModel& model ) {
	if ( !mInfo.isDirectory() || mHasTraversed )
		return;
	mHasTraversed = true;

	auto files = FileSystem::filesInfoGetInPath(
		mInfo.getFilepath(), true, model.getDisplayConfig().sortByName,
		model.getDisplayConfig().foldersFirst, model.getDisplayConfig().ignoreHidden );

	const auto& patterns = model.getDisplayConfig().acceptedExtensions;
	bool accepted;
	for ( auto file : files ) {
		if ( ( model.getMode() == Mode::DirectoriesOnly && file.isDirectory() ) ||
			 model.getMode() == Mode::FilesAndDirectories ) {
			if ( file.isDirectory() || patterns.empty() ) {
				mChildren.emplace_back( Node( std::move( file ), this ) );
			} else {
				accepted = false;
				if ( patterns.size() ) {
					for ( size_t z = 0; z < patterns.size(); z++ ) {
						if ( patterns[z] == FileSystem::fileExtension( file.getFilepath() ) ) {
							accepted = true;
							break;
						}
					}
				} else {
					accepted = true;
				}
				if ( accepted )
					mChildren.emplace_back( Node( std::move( file ), this ) );
			}
		}
	}
}

void FileSystemModel::Node::refreshIfNeeded( const FileSystemModel& model ) {
	traverseIfNeeded( model );
	if ( mInfoDirty )
		fetchData( fullPath() );
}

bool FileSystemModel::Node::fetchData( const String& fullPath ) {
	if ( mInfoDirty ) {
		mInfo = FileInfo( fullPath, mParent == nullptr );
		mInfoDirty = false;
	}
	return true;
}

std::shared_ptr<FileSystemModel> FileSystemModel::New( const std::string& rootPath,
													   const FileSystemModel::Mode& mode,
													   const DisplayConfig displayConfig ) {
	return std::shared_ptr<FileSystemModel>( new FileSystemModel( rootPath, mode, displayConfig ) );
}

FileSystemModel::FileSystemModel( const std::string& rootPath, const FileSystemModel::Mode& mode,
								  const DisplayConfig displayConfig ) :
	mRootPath( rootPath ), mMode( mode ), mDisplayConfig( displayConfig ) {
	update();
}

std::string FileSystemModel::getRootPath() const {
	return mRootPath;
}

void FileSystemModel::setRootPath( const std::string& rootPath ) {
	mRootPath = rootPath;
	update();
}

void FileSystemModel::reload() {
	setRootPath( mRootPath );
}

void FileSystemModel::update() {
	mRoot = std::make_unique<Node>( mRootPath, *this );
	onModelUpdate();
}

const FileSystemModel::Node& FileSystemModel::node( const ModelIndex& index ) const {
	return nodeRef( index );
}

FileSystemModel::Node& FileSystemModel::nodeRef( const ModelIndex& index ) const {
	if ( !index.isValid() )
		return *mRoot;
	return *(Node*)index.data();
}

size_t FileSystemModel::rowCount( const ModelIndex& index ) const {
	Node& node = const_cast<Node&>( this->node( index ) );
	node.refreshIfNeeded( *this );
	if ( node.info().isDirectory() )
		return node.mChildren.size();
	return 0;
}

size_t FileSystemModel::columnCount( const ModelIndex& ) const {
	return Column::Count;
}

std::string FileSystemModel::columnName( const size_t& column ) const {
	switch ( column ) {
		case Column::Icon:
			return "";
		case Column::Name:
			return "Name";
		case Column::Size:
			return "Size";
		case Column::Owner:
			return "Owner";
		case Column::Group:
			return "Group";
		case Column::Permissions:
			return "Mode";
		case Column::ModificationTime:
			return "Modified";
		case Column::Inode:
			return "Inode";
		case Column::SymlinkTarget:
			return "Symlink target";
		case Column::Path:
			return "Path";
		default:
			return "";
	}
}

static std::string permissionString( const FileInfo& info ) {
	std::string builder;
	if ( info.isDirectory() )
		builder.append( "d" );
	else if ( info.isLink() )
		builder.append( "l" );
	else if ( info.isRegularFile() )
		builder.append( "-" );
	else
		builder.append( "?" );
	return builder;
}

static std::string timestampString( const Uint64& time ) {
	std::time_t t = time;
	auto tm = *std::localtime( &t );
	std::ostringstream oss;
	oss << std::put_time( &tm, "%Y-%m-%d %H:%M" );
	return oss.str();
}

Variant FileSystemModel::data( const ModelIndex& index, Model::Role role ) const {
	eeASSERT( index.isValid() );

	auto& node = this->nodeRef( index );

	if ( role == Role::Custom )
		return Variant( node.info().getFilepath().c_str() );

	if ( role == Role::Sort ) {
		switch ( index.column() ) {
			case Column::Icon:
				return node.info().isDirectory() ? 0 : 1;
			case Column::Name:
				return Variant( node.getName().c_str() );
			case Column::Size:
				return node.info().getSize();
			case Column::Owner:
				return node.info().getOwnerId();
			case Column::Group:
				return node.info().getGroupId();
			case Column::Permissions:
				return Variant( permissionString( node.info() ) );
			case Column::ModificationTime:
				return node.info().getModificationTime();
			case Column::Inode:
				return node.info().getInode();
			case Column::Path:
				return Variant( node.info().getFilepath().c_str() );
			case Column::SymlinkTarget:
				return node.info().isLink() ? Variant( node.info().linksTo() ) : Variant( "" );
			default:
				eeASSERT( false );
		}
	}

	if ( role == Role::Display ) {
		switch ( index.column() ) {
			case Column::Icon:
				return iconFor( node, index );
			case Column::Name:
				return Variant( node.getName().c_str() );
			case Column::Size:
				return Variant( FileSystem::sizeToString( node.info().getSize() ) );
			case Column::Owner:
				return Variant( String::toString( node.info().getOwnerId() ) );
			case Column::Group:
				return Variant( String::toString( node.info().getGroupId() ) );
			case Column::Permissions:
				return Variant( permissionString( node.info() ) );
			case Column::ModificationTime:
				return Variant( timestampString( node.info().getModificationTime() ) );
			case Column::Inode:
				return Variant( String::toString( node.info().getInode() ) );
			case Column::Path:
				return Variant( node.info().getFilepath().c_str() );
			case Column::SymlinkTarget:
				return node.info().isLink() ? Variant( node.info().linksTo() ) : Variant( "" );
		}
	}

	if ( role == Role::Icon )
		return iconFor( node, index );

	return {};
}

ModelIndex FileSystemModel::parentIndex( const ModelIndex& index ) const {
	if ( !index.isValid() )
		return {};
	auto& node = this->node( index );
	if ( !node.getParent() ) {
		eeASSERT( &node == mRoot.get() );
		return {};
	}
	return node.getParent()->index( *this, index.column() );
}

ModelIndex FileSystemModel::index( int row, int column, const ModelIndex& parent ) const {
	if ( row < 0 || column < 0 )
		return {};
	auto& node = this->node( parent );
	const_cast<Node&>( node ).refreshIfNeeded( *this );
	if ( static_cast<size_t>( row ) >= node.mChildren.size() )
		return {};
	return createIndex( row, column, &node.mChildren[row] );
}

UIIcon* FileSystemModel::iconFor( const Node& node, const ModelIndex& index ) const {
	if ( index.column() == (Int64)treeColumn() || Column::Icon == index.column() ) {
		auto* scene = SceneManager::instance()->getUISceneNode();
		auto* d = scene->findIcon( node.getMimeType() );
		if ( !d )
			return scene->findIcon( !node.info().isDirectory() ? "file" : "folder" );
		return d;
	}
	return nullptr;
}

void FileSystemModel::setMode( const Mode& mode ) {
	if ( mode != mMode ) {
		mMode = mode;
		reload();
	}
}

const FileSystemModel::DisplayConfig& FileSystemModel::getDisplayConfig() const {
	return mDisplayConfig;
}

void FileSystemModel::setDisplayConfig( const DisplayConfig& displayConfig ) {
	if ( mDisplayConfig != displayConfig ) {
		mDisplayConfig = displayConfig;
		reload();
	}
}

}}} // namespace EE::UI::Models
