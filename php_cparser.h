/* cparser extension for PHP */

#ifndef PHP_CPARSER_H
# define PHP_CPARSER_H

extern zend_module_entry cparser_module_entry;
# define phpext_cparser_ptr &cparser_module_entry

# define PHP_CPARSER_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_CPARSER)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_CPARSER_H */
