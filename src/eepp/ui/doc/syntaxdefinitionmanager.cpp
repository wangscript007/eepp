#include <algorithm>
#include <eepp/system/filesystem.hpp>
#include <eepp/system/luapatternmatcher.hpp>
#include <eepp/ui/doc/syntaxdefinitionmanager.hpp>
#include <eepp/ui/uiwidgetcreator.hpp>

using namespace EE::System;

namespace EE { namespace UI { namespace Doc {

SINGLETON_DECLARE_IMPLEMENTATION( SyntaxDefinitionManager )

// Syntax definitions can be directly converted from the lite (https://github.com/rxi/lite) and
// lite-plugins (https://github.com/rxi/lite-plugins) supported languages.

SyntaxDefinitionManager::SyntaxDefinitionManager() {
	// Register some languages support.

	// XML - HTML
	add( {{"%.xml$", "%.html?$", "%.svg?$"},
		  {
			  {{"<!%-%-", "%-%->"}, "comment"},
			  {{"%f[^>][^<]", "%f[<]"}, "normal"},
			  {{"\"", "\"", "\\"}, "string"},
			  {{"'", "'", "\\"}, "string"},
			  {{"0x[%da-fA-F]+"}, "number"},
			  {{"-?%d+[%d%.]*f?"}, "number"},
			  {{"-?%.?%d+f?"}, "number"},
			  {{"%f[^<]![%a_][%w_]*"}, "keyword2"},
			  {{"%f[^<][%a_][%w_]*"}, "function"},
			  {{"%f[^<]/[%a_][%w_]*"}, "function"},
			  {{"[%a_][%w_]*"}, "keyword"},
			  {{"[/<>=]"}, "operator"},
		  }} );

	// CSS
	add( {{"%.css$"},
		  {
			  {{"\\."}, "normal"},
			  {{"//.-\n"}, "comment"},
			  {{"/%*", "%*/"}, "comment"},
			  {{"\"", "\"", "\\"}, "string"},
			  {{"'", "'", "\\"}, "string"},
			  {{"[%a][%w-]*%s*%f[:]"}, "keyword"},
			  {{"#%x+"}, "string"},
			  {{"-?%d+[%d%.]*p[xt]"}, "number"},
			  {{"-?%d+[%d%.]*deg"}, "number"},
			  {{"-?%d+[%d%.]*"}, "number"},
			  {{"[%a_][%w_]*"}, "symbol"},
			  {{"#[%a][%w_-]*"}, "keyword2"},
			  {{"@[%a][%w_-]*"}, "keyword2"},
			  {{"%.[%a][%w_-]*"}, "keyword2"},
			  {{"[{}:]"}, "operator"},
		  }} )
		.addSymbols( UIWidgetCreator::getWidgetNames(), "keyword2" );

	// Markdown
	add( {{"%.md$", "%.markdown$"},
		  {
			  {{"\\."}, "normal"},
			  {{"<!%-%-", "%-%->"}, "comment"},
			  {{"```", "```"}, "string"},
			  {{"``", "``", "\\"}, "string"},
			  {{"`", "`", "\\"}, "string"},
			  {{"~~", "~~", "\\"}, "keyword2"},
			  {{"%-%-%-+"}, "comment"},
			  {{"%*%s+"}, "operator"},
			  {{"%*", "[%*\n]", "\\"}, "operator"},
			  {{"%_", "[%_\n]", "\\"}, "keyword2"},
			  {{"#.-\n"}, "keyword"},
			  {{"!?%[.-%]%(.-%)"}, "function"},
			  {{"https?://%S+"}, "function"},
		  }} );

	// C
	add( {{"%.c$", "%.h$"},
		  {
			  {{"//.-\n"}, "comment"},
			  {{"/%*", "%*/"}, "comment"},
			  {{"#", "[^\\]\n"}, "comment"},
			  {{"\"", "\"", "\\"}, "string"},
			  {{"'", "'", "\\"}, "string"},
			  {{"-?0x%x+"}, "number"},
			  {{"-?%d+[%d%.eE]*f?"}, "number"},
			  {{"-?%.?%d+f?"}, "number"},
			  {{"[%+%-=/%*%^%%<>!~|&]"}, "operator"},
			  {{"[%a_][%w_]*%f[(]"}, "function"},
			  {{"[%a_][%w_]*"}, "symbol"},
		  },
		  {
			  {"if", "keyword"},	  {"then", "keyword"},	  {"else", "keyword"},
			  {"elseif", "keyword"},  {"do", "keyword"},	  {"while", "keyword"},
			  {"for", "keyword"},	  {"break", "keyword"},	  {"continue", "keyword"},
			  {"return", "keyword"},  {"goto", "keyword"},	  {"struct", "keyword"},
			  {"union", "keyword"},	  {"typedef", "keyword"}, {"enum", "keyword"},
			  {"extern", "keyword"},  {"static", "keyword"},  {"volatile", "keyword"},
			  {"const", "keyword"},	  {"inline", "keyword"},  {"switch", "keyword"},
			  {"case", "keyword"},	  {"default", "keyword"}, {"auto", "keyword"},
			  {"const", "keyword"},	  {"void", "keyword"},	  {"int", "keyword2"},
			  {"short", "keyword2"},  {"long", "keyword2"},	  {"float", "keyword2"},
			  {"double", "keyword2"}, {"char", "keyword2"},	  {"unsigned", "keyword2"},
			  {"bool", "keyword2"},	  {"true", "literal"},	  {"false", "literal"},
			  {"NULL", "literal"},
		  },
		  "//"} );

	// Lua
	add( {{"%.lua$"},
		  {
			  {{"\"", "\"", "\\"}, "string"},
			  {{"'", "'", "\\"}, "string"},
			  {{"%[%{", "%]%]"}, "string"},
			  {{"%-%-%[%{", "%]%]"}, "comment"},
			  {{"%-%-.-\n"}, "comment"},
			  {{"-?0x%x+"}, "number"},
			  {{"-?%d+[%d%.eE]*"}, "number"},
			  {{"-?%.?%d+"}, "number"},
			  {{"%.%.%.?"}, "operator"},
			  {{"[<>~=]="}, "operator"},
			  {{"[%+%-=/%*%^%%#<>]"}, "operator"},
			  {{"[%a_][%w_]*%s*%f[(\"{]"}, "function"},
			  {{"[%a_][%w_]*"}, "symbol"},
			  {{"::[%a_][%w_]*::"}, "function"},
		  },
		  {
			  {"if", "keyword"},	   {"then", "keyword"},	  {"else", "keyword"},
			  {"elseif", "keyword"},   {"end", "keyword"},	  {"do", "keyword"},
			  {"function", "keyword"}, {"repeat", "keyword"}, {"until", "keyword"},
			  {"while", "keyword"},	   {"for", "keyword"},	  {"break", "keyword"},
			  {"return", "keyword"},   {"local", "keyword"},  {"in", "keyword"},
			  {"not", "keyword"},	   {"and", "keyword"},	  {"or", "keyword"},
			  {"goto", "keyword"},	   {"self", "keyword2"},  {"true", "literal"},
			  {"false", "literal"},	   {"nil", "literal"},
		  },
		  "--"} );

	// JavaScript
	add( {{"%.js$", "%.json$", "%.cson$"},
		  {
			  {{"//.-\n"}, "comment"},
			  {{"/%*", "%*/"}, "comment"},
			  {{"\"", "\"", "\\"}, "string"},
			  {{"'", "'", "\\"}, "string"},
			  {{"0x[%da-fA-F]+"}, "number"},
			  {{"-?%d+[%d%.eE]*"}, "number"},
			  {{"-?%.?%d+"}, "number"},
			  {{"[%+%-=/%*%^%%<>!~|&]"}, "operator"},
			  {{"[%a_][%w_]*%f[(]"}, "function"},
			  {{"[%a_][%w_]*"}, "symbol"},
		  },
		  {
			  {"arguments", "keyword2"}, {"async", "keyword"},		{"await", "keyword"},
			  {"break", "keyword"},		 {"case", "keyword"},		{"catch", "keyword"},
			  {"class", "keyword"},		 {"const", "keyword"},		{"continue", "keyword"},
			  {"debugger", "keyword"},	 {"default", "keyword"},	{"delete", "keyword"},
			  {"do", "keyword"},		 {"else", "keyword"},		{"export", "keyword"},
			  {"extends", "keyword"},	 {"false", "literal"},		{"finally", "keyword"},
			  {"for", "keyword"},		 {"function", "keyword"},	{"get", "keyword"},
			  {"if", "keyword"},		 {"import", "keyword"},		{"in", "keyword"},
			  {"Infinity", "keyword2"},	 {"instanceof", "keyword"}, {"let", "keyword"},
			  {"NaN", "keyword2"},		 {"new", "keyword"},		{"null", "literal"},
			  {"return", "keyword"},	 {"set", "keyword"},		{"super", "keyword"},
			  {"switch", "keyword"},	 {"this", "keyword2"},		{"throw", "keyword"},
			  {"true", "literal"},		 {"try", "keyword"},		{"typeof", "keyword"},
			  {"undefined", "literal"},	 {"var", "keyword"},		{"void", "keyword"},
			  {"while", "keyword"},		 {"with", "keyword"},		{"yield", "keyword"},
		  },
		  "//"} );

	// Python
	add( {{"%.py$"},
		  {
			  {{"#", "\n"}, "comment"},
			  {{"[ruU]?\"", "\"", "\\"}, "string"},
			  {{"[ruU]?'", "'", "\\"}, "string"},
			  {{"\"\"\"", "\"\"\""}, "string"},
			  {{"0x[%da-fA-F]+"}, "number"},
			  {{"-?%d+[%d%.eE]*"}, "number"},
			  {{"-?%.?%d+"}, "number"},
			  {{"[%+%-=/%*%^%%<>!~|&]"}, "operator"},
			  {{"[%a_][%w_]*%f[(]"}, "function"},
			  {{"[%a_][%w_]*"}, "symbol"},
		  },
		  {
			  {"class", "keyword"},	 {"finally", "keyword"},  {"is", "keyword"},
			  {"return", "keyword"}, {"continue", "keyword"}, {"for", "keyword"},
			  {"lambda", "keyword"}, {"try", "keyword"},	  {"def", "keyword"},
			  {"from", "keyword"},	 {"nonlocal", "keyword"}, {"while", "keyword"},
			  {"and", "keyword"},	 {"global", "keyword"},	  {"not", "keyword"},
			  {"with", "keyword"},	 {"as", "keyword"},		  {"elif", "keyword"},
			  {"if", "keyword"},	 {"or", "keyword"},		  {"else", "keyword"},
			  {"import", "keyword"}, {"pass", "keyword"},	  {"break", "keyword"},
			  {"except", "keyword"}, {"in", "keyword"},		  {"del", "keyword"},
			  {"raise", "keyword"},	 {"yield", "keyword"},	  {"assert", "keyword"},
			  {"self", "keyword2"},	 {"None", "literal"},	  {"True", "literal"},
			  {"False", "literal"},
		  },
		  "#"} );

	// sh - bash
	add( {{"%.sh$", "%.bash$"},
		  {
			  {{"#.*\n"}, "comment"},
			  {{"[[\\.]]"}, "normal"},
			  {{"\"", "\"", "\\"}, "string"},
			  {{"'", "'", "\\"}, "string"},
			  {{"`", "`", "\\"}, "string"},
			  {{"%f[%w_][%d%.]+%f[^%w_]"}, "number"},
			  {{"[!<>|&%[%]=*]"}, "operator"},
			  {{"%f[%S]%-[%w%-_]+"}, "function"},
			  {{"${.*}"}, "keyword2"},
			  {{"$[%a_@*][%w_]*"}, "keyword2"},
			  {{"[%a_][%w_]*"}, "symbol"},
		  },
		  {
			  {"case", "keyword"},	{"do", "keyword"},	 {"done", "keyword"},
			  {"elif", "keyword"},	{"else", "keyword"}, {"esac", "keyword"},
			  {"fi", "keyword"},	{"for", "keyword"},	 {"function", "keyword"},
			  {"if", "keyword"},	{"in", "keyword"},	 {"select", "keyword"},
			  {"then", "keyword"},	{"time", "keyword"}, {"until", "keyword"},
			  {"while", "keyword"}, {"echo", "keyword"}, {"true", "literal"},
			  {"false", "literal"},
		  },
		  "#"} );

	// C++
	add( {{"%.h$", "%.inl$", "%.cpp$", "%.cc$", "%.C$", "%.cxx$", "%.c++$", "%.hh$", "%.H$",
		   "%.hxx$", "%.hpp$", "%.h++$"},
		  {
			  {{"//.-\n"}, "comment"},
			  {{"/%*", "%*/"}, "comment"},
			  {{"#", "[^\\]\n"}, "keyword2"},
			  {{"\"", "\"", "\\"}, "string"},
			  {{"'", "'", "\\"}, "string"},
			  {{"-?0x%x+"}, "number"},
			  {{"-?%d+[%d%.eE]*f?"}, "number"},
			  {{"-?%.?%d+f?"}, "number"},
			  {{"[%+%-=/%*%^%%<>!~|&]"}, "operator"},
			  {{"[%a_][%w_]*%f[(]"}, "function"},
			  {{"[%a_][%w_]*"}, "symbol"},
		  },
		  {
			  {"alignof", "keyword"},
			  {"alignas", "keyword"},
			  {"and", "keyword"},
			  {"and_eq", "keyword"},
			  {"not", "keyword"},
			  {"not_eq", "keyword"},
			  {"or", "keyword"},
			  {"or_eq", "keyword"},
			  {"xor", "keyword"},
			  {"xor_eq", "keyword"},
			  {"private", "keyword"},
			  {"protected", "keyword"},
			  {"public", "keyword"},
			  {"register", "keyword"},
			  {"nullptr", "keyword"},
			  {"operator", "keyword"},
			  {"asm", "keyword"},
			  {"bitand", "keyword"},
			  {"bitor", "keyword"},
			  {"catch", "keyword"},
			  {"throw", "keyword"},
			  {"try", "keyword"},
			  {"class", "keyword"},
			  {"compl", "keyword"},
			  {"explicit", "keyword"},
			  {"export", "keyword"},
			  {"concept", "keyword"},
			  {"consteval", "keyword"},
			  {"constexpr", "keyword"},
			  {"constinit", "keyword"},
			  {"const_cast", "keyword"},
			  {"dynamic_cast", "keyword"},
			  {"reinterpret_cast", "keyword"},
			  {"static_cast", "keyword"},
			  {"static_assert", "keyword"},
			  {"template", "keyword"},
			  {"this", "keyword"},
			  {"thread_local", "keyword"},
			  {"requires", "keyword"},
			  {"co_wait", "keyword"},
			  {"co_return", "keyword"},
			  {"co_yield", "keyword"},
			  {"decltype", "keyword"},
			  {"delete", "keyword"},
			  {"export", "keyword"},
			  {"friend", "keyword"},
			  {"typeid", "keyword"},
			  {"typename", "keyword"},
			  {"mutable", "keyword"},
			  {"virtual", "keyword"},
			  {"using", "keyword"},
			  {"namespace", "keyword"},
			  {"new", "keyword"},
			  {"noexcept", "keyword"},
			  {"if", "keyword"},
			  {"then", "keyword"},
			  {"else", "keyword"},
			  {"elseif", "keyword"},
			  {"do", "keyword"},
			  {"while", "keyword"},
			  {"for", "keyword"},
			  {"break", "keyword"},
			  {"continue", "keyword"},
			  {"return", "keyword"},
			  {"goto", "keyword"},
			  {"struct", "keyword"},
			  {"union", "keyword"},
			  {"typedef", "keyword"},
			  {"enum", "keyword"},
			  {"extern", "keyword"},
			  {"static", "keyword"},
			  {"volatile", "keyword"},
			  {"const", "keyword"},
			  {"inline", "keyword"},
			  {"switch", "keyword"},
			  {"case", "keyword"},
			  {"default", "keyword"},
			  {"auto", "keyword"},
			  {"const", "keyword"},
			  {"void", "keyword"},
			  {"int", "keyword2"},
			  {"short", "keyword2"},
			  {"long", "keyword2"},
			  {"float", "keyword2"},
			  {"double", "keyword2"},
			  {"char", "keyword2"},
			  {"unsigned", "keyword2"},
			  {"bool", "keyword2"},
			  {"true", "keyword2"},
			  {"false", "keyword2"},
			  {"wchar_t", "keyword2"},
			  {"char8_t", "keyword2"},
			  {"char16_t", "keyword2"},
			  {"char32_t", "keyword2"},
			  {"size_t", "keyword2"},
			  {"int16_t", "keyword2"},
			  {"int32_t", "keyword2"},
			  {"int64_t", "keyword2"},
			  {"uint16_t", "keyword2"},
			  {"uint32_t", "keyword2"},
			  {"uint64_t", "keyword2"},
			  {"NULL", "literal"},
		  },
		  "//"} );

	// PHP
	add( {{"%.php$", "%.php3$", "%.php4$", "%.php5$", "%.phtml"},
		  {
			  {{"//.-\n"}, "comment"},
			  {{"/%*", "%*/"}, "comment"},
			  {{"\"", "\"", "\\"}, "string"},
			  {{"'", "'", "\\"}, "string"},
			  {{"%\\x[%da-fA-F]+"}, "number"},
			  {{"-?%d+[%d%.eE]*"}, "number"},
			  {{"-?%.?%d+"}, "number"},
			  {{"[%.%+%-=/%*%^%%<>!~|&]"}, "operator"},
			  {{"[%a_][%w_]*%f[(]"}, "function"},
			  {{"[%a_][%w_]*"}, "symbol"},
			  {{"%$[%a][%w_]*"}, "keyword2"},
		  },
		  {{"return", "keyword"},	   {"if", "keyword"},
		   {"else", "keyword"},		   {"elseif", "keyword"},
		   {"endif", "keyword"},	   {"declare", "keyword"},
		   {"enddeclare", "keyword"},  {"switch", "keyword"},
		   {"endswitch", "keyword"},   {"as", "keyword"},
		   {"do", "keyword"},		   {"for", "keyword"},
		   {"endfor", "keyword"},	   {"foreach", "keyword"},
		   {"endforeach", "keyword"},  {"while", "keyword"},
		   {"endwhile", "keyword"},	   {"switch", "keyword"},
		   {"case", "keyword"},		   {"continue", "keyword"},
		   {"default", "keyword"},	   {"break", "keyword"},
		   {"exit", "keyword"},		   {"goto", "keyword"},

		   {"catch", "keyword"},	   {"throw", "keyword"},
		   {"try", "keyword"},		   {"finally", "keyword"},

		   {"class", "keyword"},	   {"trait", "keyword"},
		   {"interface", "keyword"},   {"public", "keyword"},
		   {"static", "keyword"},	   {"protected", "keyword"},
		   {"private", "keyword"},	   {"abstract", "keyword"},
		   {"final", "keyword"},

		   {"function", "keyword2"},   {"global", "keyword2"},
		   {"var", "keyword2"},		   {"const", "keyword2"},
		   {"bool", "keyword2"},	   {"boolean", "keyword2"},
		   {"int", "keyword2"},		   {"integer", "keyword2"},
		   {"real", "keyword2"},	   {"double", "keyword2"},
		   {"float", "keyword2"},	   {"string", "keyword2"},
		   {"array", "keyword2"},	   {"object", "keyword2"},
		   {"callable", "keyword2"},   {"iterable", "keyword2"},

		   {"namespace", "keyword2"},  {"extends", "keyword2"},
		   {"implements", "keyword2"}, {"instanceof", "keyword2"},
		   {"require", "keyword2"},	   {"require_once", "keyword2"},
		   {"include", "keyword2"},	   {"include_once", "keyword2"},
		   {"use", "keyword2"},		   {"new", "keyword2"},
		   {"clone", "keyword2"},

		   {"true", "literal"},		   {"false", "literal"},
		   {"NULL", "literal"},		   {"parent", "literal"},
		   {"self", "literal"},		   {"echo", "function"}},
		  "//"} );

	// Add SQL
	std::vector<std::string> keywords = {
		"CREATE",	  "SELECT",		"INSERT",	 "INTO",	  "UPDATE",		  "DELETE",
		"TABLE",	  "DROP",		"VALUES",	 "NOT",		  "NULL",		  "PRIMARY",
		"KEY",		  "REFERENCES", "DEFAULT",	 "UNIQUE",	  "CONSTRAINT",	  "CHECK",
		"ON",		  "EXCLUDE",	"WITH",		 "USING",	  "WHERE",		  "GROUP",
		"BY",		  "HAVING",		"DISTINCT",	 "LIMIT",	  "OFFSET",		  "ONLY",
		"CROSS",	  "JOIN",		"INNER",	 "LEFT",	  "RIGHT",		  "FULL",
		"OUTER",	  "NATURAL",	"AND",		 "OR",		  "AS",			  "ORDER",
		"ORDINALITY", "UNNEST",		"FROM",		 "VIEW",	  "RETURNS",	  "SETOF",
		"LANGUAGE",	  "SQL",		"LIKE",		 "LATERAL",	  "INTERVAL",	  "PARTITION",
		"UNION",	  "INTERSECT",	"EXCEPT",	 "ALL",		  "ASC",		  "DESC",
		"NULLS",	  "FIRST",		"LAST",		 "IN",		  "RECURSIVE",	  "ARRAY",
		"RETURNING",  "SET",		"ALSO",		 "INSTEAD",	  "ALTER",		  "SEQUENCE",
		"OWNED",	  "AT",			"ZONE",		 "WITHOUT",	  "TO",			  "TIMEZONE",
		"TYPE",		  "ENUM",		"DOCUMENT",	 "XMLPARSE",  "XMLSERIALIZE", "CONTENT",
		"OPTION",	  "INDEX",		"ANY",		 "EXTENSION", "ISNULL",		  "NOTNULL",
		"UNKNOWN",	  "CASE",		"THEN",		 "WHEN",	  "ELSE",		  "END",
		"ROWS",		  "BETWEEN",	"UNBOUNDED", "PRECEDING", "UNBOUNDED",	  "FOLLOWING",
		"EXISTS",	  "SOME",		"COLLATION", "FOR",		  "TRIGGER",	  "BEFORE",
		"EACH",		  "ROW",		"EXECUTE",	 "PROCEDURE", "FUNCTION",	  "DECLARE",
		"BEGIN",	  "LOOP",		"RAISE",	 "NOTICE",	  "LOOP",		  "EVENT",
		"OPERATOR",	  "DOMAIN",		"VARIADIC",	 "FOREIGN"};

	std::vector<std::string> types = {
		"BIGINT",	 "INT8",	 "BIGSERIAL",	  "SERIAL8",	 "BIT",		  "VARBIT",
		"BOOLEAN",	 "BOOL",	 "BOX",			  "BYTEA",		 "CHARACTER", "CHAR",
		"VARCHAR",	 "CIDR",	 "CIRCLE",		  "DATE",		 "DOUBLE",	  "PRECISION",
		"FLOAT8",	 "INET",	 "INTEGER",		  "INT",		 "INT4",	  "INTERVAL",
		"JSON",		 "JSONB",	 "LINE",		  "LSEG",		 "MACADDR",	  "MONEY",
		"NUMERIC",	 "DECIMAL",	 "PATH",		  "POINT",		 "POLYGON",	  "REAL",
		"FLOAT4",	 "INT2",	 "SMALLINT",	  "SMALLSERIAL", "SERIAL2",	  "SERIAL",
		"SERIAL4",	 "TEXT",	 "TIME",		  "TIMEZ",		 "TIMESTAMP", "TIMESTAMPZ",
		"TSQUERY",	 "TSVECTOR", "TXID_SNAPSHOT", "UUID",		 "XML",		  "INT4RANGE",
		"INT8RANGE", "NUMRANGE", "TSRANGE",		  "TSTZRANGE",	 "DATERANGE", "PG_LSN"};

	std::vector<std::string> literals = {"FALSE",		  "TRUE",		  "CURRENT_TIMESTAMP",
										 "CURRENT_TIME",  "CURRENT_DATE", "LOCALTIME",
										 "LOCALTIMESTAMP"};

	std::unordered_map<std::string, std::string> symbols;
	for ( auto& keyword : keywords ) {
		symbols[String::toLower( keyword )] = "keyword";
		symbols[keyword] = "keyword";
	}

	for ( auto& type : types ) {
		symbols[String::toLower( type )] = "keyword2";
		symbols[type] = "keyword2";
	}

	for ( auto& literal : literals ) {
		symbols[String::toLower( literal )] = "literal";
		symbols[literal] = "literal";
	}

	add( {{"%.sql$", "%.psql$"},
		  {
			  {{"%-%-.-\n"}, "comment"},
			  {{"/%*", "%*/"}, "comment"},
			  {{"'", "'", "\\"}, "string"},
			  {{"-?%d+[%d%.eE]*f?"}, "number"},
			  {{"-?%.?%d+f?"}, "number"},
			  {{"[%+%-=/%*%%<>!~|&@%?$#]"}, "operator"},
			  {{"[%a_][%w_]*%f[(]"}, "function"},
			  {{"[%a_][%w_]*"}, "symbol"},
		  },
		  symbols,
		  "--"} );

	// GLSL
	add( {{"%.glsl$", "%.frag$", "%.vert$"},
		  {
			  {{"//.-\n"}, "comment"},
			  {{"/%*", "%*/"}, "comment"},
			  {{"#", "[^\\]\n"}, "comment"},
			  {{"\"", "\"", "\\"}, "string"},
			  {{"'", "'", "\\"}, "string"},
			  {{"-?0x%x+"}, "number"},
			  {{"-?%d+[%d%.eE]*f?"}, "number"},
			  {{"-?%.?%d+f?"}, "number"},
			  {{"[%+%-=/%*%^%%<>!~|&]"}, "operator"},
			  {{"ivec[2-4]"}, "keyword2"},
			  {{"bvec[2-4]"}, "keyword2"},
			  {{"uvec[2-4]"}, "keyword2"},
			  {{"vec[2-4]"}, "keyword2"},
			  {{"dmat[2-4]x[2-4]"}, "keyword2"},
			  {{"dmat[2-4]"}, "keyword2"},
			  {{"mat[2-4]x[2-4]"}, "keyword2"},
			  {{"mat[2-4]"}, "keyword2"},
			  {{"[%a_][%w_]*%f[(]"}, "function"},
			  {{"[%a_][%w_]*"}, "symbol"},
		  },
		  {
			  {"if", "keyword"},
			  {"else", "keyword"},
			  {"do", "keyword"},
			  {"while", "keyword"},
			  {"for", "keyword"},
			  {"break", "keyword"},
			  {"continue", "keyword"},
			  {"return", "keyword"},
			  {"const", "keyword"},
			  {"switch", "keyword"},
			  {"case", "keyword"},
			  {"default", "keyword"},
			  {"const", "keyword"},
			  {"void", "keyword"},
			  {"bool", "keyword2"},
			  {"int", "keyword2"},
			  {"uint", "keyword2"},
			  {"float", "keyword2"},
			  {"double", "keyword2"},
			  {"true", "literal"},
			  {"false", "literal"},
			  {"NULL", "literal"},
			  {"attribute", "keyword"},
			  {"varying", "keyword"},
			  {"uniform", "keyword"},
			  {"buffer", "keyword"},
			  {"shared", "keyword"},
			  {"layout", "keyword"},
			  {"centroid", "keyword"},
			  {"flat", "keyword"},
			  {"smooth", "keyword"},
			  {"noperspective", "keyword"},
			  {"patch", "keyword"},
			  {"sample", "keyword"},
			  {"in", "keyword"},
			  {"out", "keyword"},
			  {"inout", "keyword"},
			  {"invariant", "keyword"},
			  {"precise", "keyword"},
			  {"lowp", "keyword"},
			  {"mediump", "keyword"},
			  {"highp", "keyword"},
			  {"precision", "keyword"},
			  {"struct", "keyword"},
			  {"subroutine", "keyword"},
			  {"coherent", "keyword"},
			  {"volatile", "keyword"},
			  {"readonly", "keyword"},
			  {"writeonly", "keyword"},
			  {"sampler1D", "keyword2"},
			  {"sampler2D", "keyword2"},
			  {"sampler3D", "keyword2"},
			  {"samplerCube", "keyword2"},
			  {"sampler1DShadow", "keyword2"},
			  {"sampler2DShadow", "keyword2"},
			  {"samplerCubeShadow", "keyword2"},
			  {"sampler1DArray", "keyword2"},
			  {"sampler2DArray", "keyword2"},
			  {"samplerCubeArray", "keyword2"},
			  {"sampler1DArrayShadow", "keyword2"},
			  {"sampler2DArrayShadow", "keyword2"},
			  {"samplerCubeArrayShadow", "keyword2"},
			  {"isampler1D", "keyword2"},
			  {"isampler2D", "keyword2"},
			  {"isampler3D", "keyword2"},
			  {"isamplerCube", "keyword2"},
			  {"sampler2DMS", "keyword2"},
			  {"isampler2DMS", "keyword2"},
			  {"usampler2DMS", "keyword2"},
			  {"sampler2DMSArray", "keyword2"},
			  {"isampler2DMSArray", "keyword2"},
			  {"usampler2DMSArray", "keyword2"},
			  {"isampler1DArray", "keyword2"},
			  {"isampler2DArray", "keyword2"},
			  {"usampler1D", "keyword2"},
			  {"usampler2D", "keyword2"},
			  {"usampler3D", "keyword2"},
			  {"usamplerCube", "keyword2"},
			  {"usampler1DArray", "keyword2"},
			  {"usampler2DArray", "keyword2"},
			  {"sampler2DRect", "keyword2"},
			  {"sampler2DRectShadow", "keyword2"},
			  {"isampler2DRect", "keyword2"},
			  {"usampler2DRect", "keyword2"},
			  {"samplerBuffer", "keyword2"},
			  {"isamplerBuffer", "keyword2"},
			  {"usamplerBuffer", "keyword2"},
			  {"image1D", "keyword2"},
			  {"iimage1D", "keyword2"},
			  {"uimage1D", "keyword2"},
			  {"image1DArray", "keyword2"},
			  {"iimage1DArray", "keyword2"},
			  {"uimage1DArray", "keyword2"},
			  {"image2D", "keyword2"},
			  {"iimage2D", "keyword2"},
			  {"uimage2D", "keyword2"},
			  {"image2DArray", "keyword2"},
			  {"iimage2DArray", "keyword2"},
			  {"uimage2DArray", "keyword2"},
			  {"image2DRect", "keyword2"},
			  {"iimage2DRect", "keyword2"},
			  {"uimage2DRect", "keyword2"},
			  {"image2DMS", "keyword2"},
			  {"iimage2DMS", "keyword2"},
			  {"uimage2DMS", "keyword2"},
			  {"image2DMSArray", "keyword2"},
			  {"iimage2DMSArray", "keyword2"},
			  {"uimage2DMSArray", "keyword2"},
			  {"image3D", "keyword2"},
			  {"iimage3D", "keyword2"},
			  {"uimage3D", "keyword2"},
			  {"imageCube", "keyword2"},
			  {"iimageCube", "keyword2"},
			  {"uimageCube", "keyword2"},
			  {"imageCubeArray", "keyword2"},
			  {"iimageCubeArray", "keyword2"},
			  {"uimageCubeArray", "keyword2"},
			  {"imageBuffer", "keyword2"},
			  {"iimageBuffer", "keyword2"},
			  {"uimageBuffer", "keyword2"},
			  {"atomic_uint", "keyword2"},
			  {"radians", "keyword"},
			  {"degrees", "keyword"},
			  {"sin", "keyword"},
			  {"cos", "keyword"},
			  {"tan", "keyword"},
			  {"asin", "keyword"},
			  {"acos", "keyword"},
			  {"atan", "keyword"},
			  {"sinh", "keyword"},
			  {"cosh", "keyword"},
			  {"tanh", "keyword"},
			  {"asinh", "keyword"},
			  {"acosh", "keyword"},
			  {"pow", "keyword"},
			  {"exp", "keyword"},
			  {"exp2", "keyword"},
			  {"log2", "keyword"},
			  {"sqrt", "keyword"},
			  {"inversesqrt", "keyword"},
			  {"abs", "keyword"},
			  {"sign", "keyword"},
			  {"floor", "keyword"},
			  {"trunc", "keyword"},
			  {"round", "keyword"},
			  {"roundEven", "keyword"},
			  {"ceil", "keyword"},
			  {"fract", "keyword"},
			  {"mod", "keyword"},
			  {"modf", "keyword"},
			  {"min", "keyword"},
			  {"max", "keyword"},
			  {"clamp", "keyword"},
			  {"mix", "keyword"},
			  {"step", "keyword"},
			  {"smoothstep", "keyword"},
			  {"isnan", "keyword"},
			  {"isinf", "keyword"},
			  {"floatBitsToInt", "keyword"},
			  {"floatBitsToUint", "keyword"},
			  {"intBitsToFloat", "keyword"},
			  {"uintBitsToFloat", "keyword"},
			  {"fma", "keyword"},
			  {"frexp", "keyword"},
			  {"ldexp", "keyword"},
			  {"packUnorm2x16", "keyword"},
			  {"packSnorm2x16", "keyword"},
			  {"packUnorm4x8", "keyword"},
			  {"packSnorm4x8", "keyword"},
			  {"unpackUnorm2x16", "keyword"},
			  {"unpackSnorm2x16", "keyword"},
			  {"unpackUnorm4x8", "keyword"},
			  {"unpackSnorm4x8", "keyword"},
			  {"packHalf2x16", "keyword"},
			  {"unpackHalf2x16", "keyword"},
			  {"packDouble2x32", "keyword"},
			  {"unpackDouble2x32", "keyword"},
			  {"length", "keyword"},
			  {"distance", "keyword"},
			  {"dot", "keyword"},
			  {"cross", "keyword"},
			  {"normalize", "keyword"},
			  {"ftransform", "keyword"},
			  {"faceforward", "keyword"},
			  {"reflect", "keyword"},
			  {"refract", "keyword"},
			  {"matrixCompMult", "keyword"},
			  {"outerProduct", "keyword"},
			  {"transpose", "keyword"},
			  {"determinant", "keyword"},
			  {"inverse", "keyword"},
			  {"lessThan", "keyword"},
			  {"lessThanEqual", "keyword"},
			  {"greaterThan", "keyword"},
			  {"greaterThanEqual", "keyword"},
			  {"equal", "keyword"},
			  {"notEqual", "keyword"},
			  {"any", "keyword"},
			  {"all", "keyword"},
			  {"not", "keyword"},
			  {"uaddCarry", "keyword"},
			  {"usubBorrow", "keyword"},
			  {"umulExtended", "keyword"},
			  {"imulExtended", "keyword"},
			  {"bitfieldExtract", "keyword"},
			  {"bitfieldInsert", "keyword"},
			  {"bitfieldReverse", "keyword"},
			  {"bitCount", "keyword"},
			  {"findLSB", "keyword"},
			  {"findMSB", "keyword"},
			  {"textureSize", "keyword"},
			  {"textureQueryLod", "keyword"},
			  {"textureQueryLevels", "keyword"},
			  {"textureSamples", "keyword"},
			  {"texture", "keyword"},
			  {"textureProj", "keyword"},
			  {"textureLod", "keyword"},
			  {"textureOffset", "keyword"},
			  {"texelFetch", "keyword"},
			  {"texelFetchOffset", "keyword"},
			  {"textureProjOffset", "keyword"},
			  {"textureLodOffset", "keyword"},
			  {"textureProjLod", "keyword"},
			  {"textureProjLodOffset", "keyword"},
			  {"textureGrad", "keyword"},
			  {"textureGradOffset", "keyword"},
			  {"textureProjGrad", "keyword"},
			  {"textureProjGradOffset", "keyword"},
			  {"textureGather", "keyword"},
			  {"textureGatherOffset", "keyword"},
			  {"textureGatherOffsets", "keyword"},
			  {"atomicCounterIncrement", "keyword"},
			  {"atomicCounterDecrement", "keyword"},
			  {"atomicCounter", "keyword"},
			  {"atomicCounterAdd", "keyword"},
			  {"atomicCounterSubtract", "keyword"},
			  {"atomicCounterMin", "keyword"},
			  {"atomicCounterMax", "keyword"},
			  {"atomicCounterAnd", "keyword"},
			  {"atomicCounterOr", "keyword"},
			  {"atomicCounterXor", "keyword"},
			  {"atomicCounterExchange", "keyword"},
			  {"atomicCounterCompSwap", "keyword"},
			  {"atomicAdd", "keyword"},
			  {"atomicMin", "keyword"},
			  {"atomicMax", "keyword"},
			  {"atomicAnd", "keyword"},
			  {"atomicOr", "keyword"},
			  {"atomicXor", "keyword"},
			  {"atomicExchange", "keyword"},
			  {"atomicCompSwap", "keyword"},
			  {"imageSize", "keyword"},
			  {"imageSamples", "keyword"},
			  {"imageLoad", "keyword"},
			  {"imageStore", "keyword"},
			  {"imageAtomicAdd", "keyword"},
			  {"imageAtomicMin", "keyword"},
			  {"imageAtomicMax", "keyword"},
			  {"imageAtomicAnd", "keyword"},
			  {"imageAtomicOr", "keyword"},
			  {"imageAtomicXor", "keyword"},
			  {"imageAtomicExchange", "keyword"},
			  {"imageAtomicCompSwap", "keyword"},
			  {"EmitStreamVertex", "keyword"},
			  {"EndStreamPrimitive", "keyword"},
			  {"EmitVertex", "keyword"},
			  {"EndPrimitive", "keyword"},
			  {"dFdx", "keyword"},
			  {"dFdy", "keyword"},
			  {"dFdxFine", "keyword"},
			  {"dFdyFine", "keyword"},
			  {"dFdxCoarse", "keyword"},
			  {"dFdyCoarse", "keyword"},
			  {"fwidth", "keyword"},
			  {"fwidthFine", "keyword"},
			  {"fwidthCoarse", "keyword"},
			  {"interpolateAtCentroid", "keyword"},
			  {"interpolateAtSample", "keyword"},
			  {"interpolateAtOffset", "keyword"},
			  {"barrier", "keyword"},
			  {"memoryBarrier", "keyword"},
			  {"memoryBarrierAtomicCounter", "keyword"},
			  {"memoryBarrierBuffer", "keyword"},
			  {"memoryBarrierShared", "keyword"},
			  {"memoryBarrierImage", "keyword"},
			  {"groupMemoryBarrier", "keyword"},
			  {"subpassLoad", "keyword"},
			  {"anyInvocation", "keyword"},
			  {"allInvocations", "keyword"},
			  {"allInvocationsEqual", "keyword"},
			  {"texture1D", "keyword"},
			  {"texture1DArray", "keyword"},
			  {"itexture1D", "keyword"},
			  {"itexture1DArray", "keyword"},
			  {"utexture1D", "keyword"},
			  {"utexture1DArray", "keyword"},
			  {"texture2D", "keyword"},
			  {"texture2DArray", "keyword"},
			  {"itexture2D", "keyword"},
			  {"itexture2DArray", "keyword"},
			  {"utexture2D", "keyword"},
			  {"utexture2DArray", "keyword"},
			  {"texture2DRect", "keyword"},
			  {"itexture2DRect", "keyword"},
			  {"utexture2DRect", "keyword"},
			  {"texture2DMS", "keyword"},
			  {"itexture2DMS", "keyword"},
			  {"utexture2DMS", "keyword"},
			  {"texture2DMSArray", "keyword"},
			  {"itexture2DMSArray", "keyword"},
			  {"utexture2DMSArray", "keyword"},
			  {"texture3D", "keyword"},
			  {"itexture3D", "keyword"},
			  {"utexture3D", "keyword"},
			  {"textureCube", "keyword"},
			  {"itextureCube", "keyword"},
			  {"utextureCube", "keyword"},
			  {"textureCubeArray", "keyword"},
			  {"itextureCubeArray", "keyword"},
			  {"utextureCubeArray", "keyword"},
			  {"textureBuffer", "keyword"},
			  {"itextureBuffer", "keyword"},
			  {"utextureBuffer", "keyword"},
			  {"sampler", "keyword2"},
			  {"samplerShadow", "keyword2"},
			  {"subpassInput", "keyword2"},
			  {"isubpassInput", "keyword2"},
			  {"usubpassInput", "keyword2"},
			  {"subpassInputMS", "keyword2"},
			  {"isubpassInputMS", "keyword2"},
			  {"usubpassInputMS", "keyword2"},
		  },
		  "//"} );
}

SyntaxDefinition& SyntaxDefinitionManager::add( SyntaxDefinition&& syntaxStyle ) {
	mStyles.emplace_back( std::move( syntaxStyle ) );
	return mStyles.back();
}

const SyntaxDefinition& SyntaxDefinitionManager::getPlainStyle() const {
	return mEmptyDefinition;
}

SyntaxDefinition& SyntaxDefinitionManager::getStyleByExtensionRef( const std::string& filePath ) {
	return const_cast<SyntaxDefinition&>( getStyleByExtension( filePath ) );
}

const SyntaxDefinition&
SyntaxDefinitionManager::getStyleByExtension( const std::string& filePath ) const {
	std::string extension( FileSystem::fileExtension( filePath ) );
	if ( !extension.empty() ) {
		for ( auto style = mStyles.rbegin(); style != mStyles.rend(); ++style ) {
			for ( auto ext : style->getFiles() ) {
				if ( String::startsWith( ext, "%." ) || String::endsWith( ext, "$" ) ) {
					LuaPatternMatcher words( ext );
					int start, end;
					if ( words.find( filePath, 0, start, end ) ) {
						return *style;
					}
				} else if ( extension == ext ) {
					return *style;
				}
			}
		}
	}
	return mEmptyDefinition;
}

}}} // namespace EE::UI::Doc
