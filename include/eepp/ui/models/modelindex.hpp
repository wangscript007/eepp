#ifndef EE_UI_MODEL_MODELINDEX_HPP
#define EE_UI_MODEL_MODELINDEX_HPP

#include <eepp/config.hpp>
#include <limits>

namespace EE { namespace UI { namespace Models {

class Model;

class EE_API ModelIndex {
  public:
	ModelIndex() {}

	bool isValid() const { return mRow != -1 && mColumn != -1; }

	const Int64& row() const { return mRow; }

	const Int64& column() const { return mColumn; }

	void* data() const { return mData; }

	Int64 internalId() const { return mInternalId; }

	ModelIndex parent() const;

	bool hasParent() const { return parent().isValid(); }

	bool operator<( const ModelIndex& other ) const {
		return mRow != other.mRow || mColumn != other.mColumn;
	}

	bool operator==( const ModelIndex& other ) const {
		return mModel == other.mModel && mRow == other.mRow && mColumn == other.mColumn &&
			   mData == other.mData;
	}

	bool operator!=( const ModelIndex& other ) const { return !( *this == other ); }

  protected:
	friend class Model;
	const Model* mModel{nullptr};
	Int64 mRow{-1};
	Int64 mColumn{-1};
	void* mData{nullptr};
	Int64 mInternalId{0};

	ModelIndex( const Model& model, int row, int column, void* internalData,
				const Int64& internalId = 0 ) :
		mModel( &model ),
		mRow( row ),
		mColumn( column ),
		mData( internalData ),
		mInternalId( internalId ) {}
};

}}} // namespace EE::UI::Models

#endif // EE_UI_MODEL_MODELINDEX_HPP
