/* cparser extension for PHP */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_cparser.h"
#include "cparser_arginfo.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE()  \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

zend_class_entry *cparser_translationunit_ce = nullptr;
zend_class_entry *cparser_classdecl_ce = nullptr;
zend_class_entry *cparser_enumconstant_ce = nullptr;
zend_class_entry *cparser_methoddecl_ce = nullptr;
zend_class_entry *cparser_paramdecl_ce = nullptr;
zend_class_entry *cparser_type_ce = nullptr;
zend_class_entry *cparser_templatedecl_ce = nullptr;
zend_class_entry *cparser_templateparameter_ce = nullptr;
zend_class_entry *cparser_templateargument_ce = nullptr;
zend_class_entry *cparser_enumdecl_ce = nullptr;
zend_class_entry *cparser_diagnostic_ce = nullptr;

PHP_MINIT_FUNCTION(cparser)
{
	// TranslationUnit
	cparser_translationunit_ce = register_class_CParser_TranslationUnit();
	cparser_translationunit_ce->create_object = cparser_object_create<CXTranslationUnit>;

	// ClassDecl
	cparser_classdecl_ce = register_class_CParser_ClassDecl();
	cparser_classdecl_ce->create_object = cparser_object_create<CXCursor>;

	// EnumConstant
	cparser_enumconstant_ce = register_class_CParser_EnumConstant();
	cparser_enumconstant_ce->create_object = cparser_object_create<CXCursor>;

	// MethodDecl
	cparser_methoddecl_ce = register_class_CParser_MethodDecl();
	cparser_methoddecl_ce->create_object = cparser_object_create<CXCursor>;

	// ParamDecl
	cparser_paramdecl_ce = register_class_CParser_ParamDecl();
	cparser_paramdecl_ce->create_object = cparser_object_create<CXCursor>;

	// Type
	cparser_type_ce = register_class_CParser_Type();
	cparser_type_ce->create_object = cparser_object_create<CXType>;

	cparser_templatedecl_ce = register_class_CParser_TemplateDecl();
	cparser_templatedecl_ce->create_object = cparser_object_create<CXCursor>;

	cparser_templateparameter_ce = register_class_CParser_TemplateParameter();
	cparser_templateparameter_ce->create_object = cparser_object_create<CXCursor>;

	cparser_templateargument_ce = register_class_CParser_TemplateArgument();
	cparser_templateargument_ce->create_object = cparser_object_create<CXType>;
	// note: could also be CXCursor if I need non-type args later

	cparser_enumdecl_ce = register_class_CParser_EnumDecl();
	cparser_enumdecl_ce->create_object = cparser_object_create<CXCursor>;

	cparser_diagnostic_ce = register_class_CParser_Diagnostic();
	cparser_diagnostic_ce->create_object = cparser_object_create<CXDiagnostic>;

	return SUCCESS;
}

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(cparser)
{
#if defined(ZTS) && defined(COMPILE_DL_CPARSER)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(cparser)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "cparser support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ cparser_module_entry */
zend_module_entry cparser_module_entry = {
	STANDARD_MODULE_HEADER,
	"cparser",			 /* Extension name */
	NULL,				 /* zend_function_entry */
	PHP_MINIT(cparser),	 /* PHP_MINIT - Module initialization */
	NULL,				 /* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(cparser),	 /* PHP_RINIT - Request initialization */
	NULL,				 /* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(cparser),	 /* PHP_MINFO - Module info */
	PHP_CPARSER_VERSION, /* Version */
	STANDARD_MODULE_PROPERTIES};
/* }}} */

#ifdef COMPILE_DL_CPARSER
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(cparser)
#endif
