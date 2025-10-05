/* cparser extension for PHP */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_cparser.h"
#include "cparser_arginfo.h"
#include "zend_interfaces.h"

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
zend_class_entry *cparser_classiterator_ce = nullptr;
zend_class_entry *cparser_cursorkind_ce = nullptr;

static zend_object_handlers cparser_classiterator_object_handlers;

template <typename NativeType>
static void register_cparser_ce_handlers(zend_class_entry *ce)
{
	static zend_object_handlers handlers;
	memcpy(&handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	handlers.offset = XtOffsetOf(cparser_obj<NativeType>, std);
	handlers.free_obj = cparser_object_free<NativeType>;

	ce->create_object = cparser_object_create<NativeType>;
	ce->default_object_handlers = &handlers;
}

void register_cursorkind_constants(zend_class_entry *ce, int start, int end)
{
	for (int i = start; i <= end; i++)
	{
		const char *name = clang_getCString(clang_getCursorKindSpelling((enum CXCursorKind)i));
		if (name && name[0] != '\0')
		{
			// check if constant already defined to avoid redefinition warnings
			if (zend_hash_str_exists(&ce->constants_table, name, strlen(name)))
			{
				continue;
			}
			zend_declare_class_constant_long(ce, name, strlen(name), i);
		}
	}
}

PHP_MINIT_FUNCTION(cparser)
{
	cparser_translationunit_ce = register_class_CParser_TranslationUnit();
	register_cparser_ce_handlers<CXTranslationUnit>(cparser_translationunit_ce);

	cparser_classdecl_ce = register_class_CParser_ClassDecl();
	register_cparser_ce_handlers<CXCursor>(cparser_classdecl_ce);

	cparser_enumconstant_ce = register_class_CParser_EnumConstant();
	register_cparser_ce_handlers<CXCursor>(cparser_enumconstant_ce);

	cparser_methoddecl_ce = register_class_CParser_MethodDecl();
	register_cparser_ce_handlers<CXCursor>(cparser_methoddecl_ce);

	cparser_paramdecl_ce = register_class_CParser_ParamDecl();
	register_cparser_ce_handlers<CXCursor>(cparser_paramdecl_ce);

	cparser_type_ce = register_class_CParser_Type();
	register_cparser_ce_handlers<CXType>(cparser_type_ce);

	cparser_templatedecl_ce = register_class_CParser_TemplateDecl();
	register_cparser_ce_handlers<CXCursor>(cparser_templatedecl_ce);

	cparser_templateparameter_ce = register_class_CParser_TemplateParameter();
	register_cparser_ce_handlers<CXCursor>(cparser_templateparameter_ce);

	cparser_templateargument_ce = register_class_CParser_TemplateArgument();
	register_cparser_ce_handlers<CXType>(cparser_templateargument_ce);

	cparser_enumdecl_ce = register_class_CParser_EnumDecl();
	register_cparser_ce_handlers<CXCursor>(cparser_enumdecl_ce);

	cparser_diagnostic_ce = register_class_CParser_Diagnostic();
	register_cparser_ce_handlers<CXDiagnostic>(cparser_diagnostic_ce);

	// ClassIterator
	cparser_classiterator_ce = register_class_CParser_ClassIterator(zend_ce_iterator);
	cparser_classiterator_ce->create_object = ast_cursor_iterator_create;
	memcpy(&cparser_classiterator_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	cparser_classiterator_object_handlers.offset = XtOffsetOf(ast_cursor_iterator, std);
	cparser_classiterator_object_handlers.free_obj = ast_cursor_iterator_free;
	cparser_classiterator_ce->default_object_handlers = &cparser_classiterator_object_handlers;

	cparser_cursorkind_ce = register_class_CParser_CursorKind();
	register_cursorkind_constants(cparser_cursorkind_ce, CXCursor_FirstDecl, CXCursor_LastDecl);
	register_cursorkind_constants(cparser_cursorkind_ce, CXCursor_FirstRef, CXCursor_LastRef);
	register_cursorkind_constants(cparser_cursorkind_ce, CXCursor_FirstInvalid, CXCursor_LastInvalid);
	register_cursorkind_constants(cparser_cursorkind_ce, CXCursor_FirstExpr, CXCursor_LastExpr);

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
