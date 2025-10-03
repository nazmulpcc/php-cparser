/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 0f9ebdd9f01649e6e6025c84a2ce204662edaf44 */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_CParser_TranslationUnit_fromFile, 0, 1, CParser\\TranslationUnit, 0)
	ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, args, IS_ARRAY, 0, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_class_CParser_TranslationUnit_classes, 0, 0, Traversable, MAY_BE_ARRAY)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_TranslationUnit_enums arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_TranslationUnit_diagnostics arginfo_class_CParser_TranslationUnit_classes

ZEND_METHOD(CParser_TranslationUnit, fromFile);
ZEND_METHOD(CParser_TranslationUnit, classes);
ZEND_METHOD(CParser_TranslationUnit, enums);
ZEND_METHOD(CParser_TranslationUnit, diagnostics);

static const zend_function_entry class_CParser_TranslationUnit_methods[] = {
	ZEND_ME(CParser_TranslationUnit, fromFile, arginfo_class_CParser_TranslationUnit_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(CParser_TranslationUnit, classes, arginfo_class_CParser_TranslationUnit_classes, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_TranslationUnit, enums, arginfo_class_CParser_TranslationUnit_enums, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_TranslationUnit, diagnostics, arginfo_class_CParser_TranslationUnit_diagnostics, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_CParser_TranslationUnit(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "TranslationUnit", class_CParser_TranslationUnit_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
