/* cparser extension for PHP */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_cparser.h"
#include "cparser_arginfo.h"

zend_class_entry *cparser_translationunit_ce = nullptr;

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE()  \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

PHP_MINIT_FUNCTION(cparser)
{
	register_class_CParser_TranslationUnit();

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
