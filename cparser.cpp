/* cparser extension for PHP */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_cparser.h"
#include "cparser_arginfo.h"
#include "zend_interfaces.h"
#include "src/NativeCXCursorIterator.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE()  \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

zend_class_entry *cparser_translationunit_ce = nullptr;

zend_class_entry *cparser_cursor_ce = nullptr;
zend_class_entry *cparser_classcursor_ce = nullptr;
zend_class_entry *cparser_methodcursor_ce = nullptr;
zend_class_entry *cparser_functioncursor_ce = nullptr;
zend_class_entry *cparser_fieldcursor_ce = nullptr;
zend_class_entry *cparser_enumcursor_ce = nullptr;
zend_class_entry *cparser_enumconstantcursor_ce = nullptr;
zend_class_entry *cparser_parametercursor_ce = nullptr;
zend_class_entry *cparser_namespacecursor_ce = nullptr;
zend_class_entry *cparser_typealiascursor_ce = nullptr;

zend_class_entry *cparser_type_ce = nullptr;
zend_class_entry *cparser_templatedecl_ce = nullptr;
zend_class_entry *cparser_templateparameter_ce = nullptr;
zend_class_entry *cparser_templateargument_ce = nullptr;
zend_class_entry *cparser_diagnostic_ce = nullptr;
zend_class_entry *cparser_cursoriterator_ce = nullptr;
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

	cparser_cursor_ce = register_class_CParser_Cursor();
	register_cparser_ce_handlers<CXCursor>(cparser_cursor_ce);
	cparser_classcursor_ce = register_class_CParser_ClassCursor(cparser_cursor_ce);
	register_cparser_ce_handlers<CXCursor>(cparser_classcursor_ce);
	cparser_methodcursor_ce = register_class_CParser_MethodCursor(cparser_cursor_ce);
	register_cparser_ce_handlers<CXCursor>(cparser_methodcursor_ce);
	cparser_functioncursor_ce = register_class_CParser_FunctionCursor(cparser_cursor_ce);
	register_cparser_ce_handlers<CXCursor>(cparser_functioncursor_ce);
	cparser_fieldcursor_ce = register_class_CParser_FieldCursor(cparser_cursor_ce);
	register_cparser_ce_handlers<CXCursor>(cparser_fieldcursor_ce);
	cparser_enumcursor_ce = register_class_CParser_EnumCursor(cparser_cursor_ce);
	register_cparser_ce_handlers<CXCursor>(cparser_enumcursor_ce);
	cparser_enumconstantcursor_ce = register_class_CParser_EnumConstantCursor(cparser_cursor_ce);
	register_cparser_ce_handlers<CXCursor>(cparser_enumconstantcursor_ce);
	cparser_parametercursor_ce = register_class_CParser_ParameterCursor(cparser_cursor_ce);
	register_cparser_ce_handlers<CXCursor>(cparser_parametercursor_ce);
	cparser_namespacecursor_ce = register_class_CParser_NamespaceCursor(cparser_cursor_ce);
	register_cparser_ce_handlers<CXCursor>(cparser_namespacecursor_ce);
	cparser_typealiascursor_ce = register_class_CParser_TypeAliasCursor(cparser_cursor_ce);
	register_cparser_ce_handlers<CXCursor>(cparser_typealiascursor_ce);

	cparser_type_ce = register_class_CParser_Type();
	register_cparser_ce_handlers<CXType>(cparser_type_ce);

	cparser_templatedecl_ce = register_class_CParser_TemplateDecl();
	register_cparser_ce_handlers<CXCursor>(cparser_templatedecl_ce);

	cparser_templateparameter_ce = register_class_CParser_TemplateParameter();
	register_cparser_ce_handlers<CXCursor>(cparser_templateparameter_ce);

	cparser_templateargument_ce = register_class_CParser_TemplateArgument();
	register_cparser_ce_handlers<CXType>(cparser_templateargument_ce);

	cparser_diagnostic_ce = register_class_CParser_Diagnostic();
	register_cparser_ce_handlers<CXDiagnostic>(cparser_diagnostic_ce);

	// CursorIterator
	cparser_cursoriterator_ce = register_class_CParser_CursorIterator(zend_ce_iterator);
	register_cparser_ce_handlers<NativeCXCursorIterator>(cparser_cursoriterator_ce);

	cparser_cursorkind_ce = register_class_CParser_CursorKind();
	register_cursorkind_constants(cparser_cursorkind_ce, CXCursor_FirstDecl, CXCursor_LastDecl);
	register_cursorkind_constants(cparser_cursorkind_ce, CXCursor_FirstRef, CXCursor_LastRef);
	register_cursorkind_constants(cparser_cursorkind_ce, CXCursor_FirstInvalid, CXCursor_LastInvalid);
	register_cursorkind_constants(cparser_cursorkind_ce, CXCursor_FirstExpr, CXCursor_LastExpr);

	register_class_CParser_Access();

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
