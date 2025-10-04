/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 3cf5ef64aea7b11ab9cfd84a20e733ee159489eb */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_CParser_ClassIterator___construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, tu, CParser\\TranslationUnit, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_CParser_ClassIterator_current, 0, 0, CParser\\ClassDecl, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_CParser_ClassIterator_key, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_CParser_ClassIterator_next, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_ClassIterator_rewind arginfo_class_CParser_ClassIterator_next

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_CParser_ClassIterator_valid, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_CParser_TranslationUnit_fromFile, 0, 1, CParser\\TranslationUnit, 0)
	ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, args, IS_ARRAY, 0, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_class_CParser_TranslationUnit_classes, 0, 0, Traversable, MAY_BE_ARRAY)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_TranslationUnit_enums arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_TranslationUnit_diagnostics arginfo_class_CParser_TranslationUnit_classes

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_CParser_ClassDecl_getName, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_ClassDecl_getBases arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_ClassDecl_methods arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_ClassDecl_innerClasses arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_ClassDecl_enums arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_ClassDecl_templates arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_ClassDecl_isAbstract arginfo_class_CParser_ClassIterator_valid

#define arginfo_class_CParser_MethodDecl_getName arginfo_class_CParser_ClassDecl_getName

#define arginfo_class_CParser_MethodDecl_getAccess arginfo_class_CParser_ClassIterator_key

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_CParser_MethodDecl_getReturnType, 0, 0, CParser\\Type, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_MethodDecl_getParameters arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_MethodDecl_isStatic arginfo_class_CParser_ClassIterator_valid

#define arginfo_class_CParser_MethodDecl_isConst arginfo_class_CParser_ClassIterator_valid

#define arginfo_class_CParser_MethodDecl_isVirtual arginfo_class_CParser_ClassIterator_valid

#define arginfo_class_CParser_MethodDecl_isPureVirtual arginfo_class_CParser_ClassIterator_valid

#define arginfo_class_CParser_MethodDecl_isConstructor arginfo_class_CParser_ClassIterator_valid

#define arginfo_class_CParser_MethodDecl_isDestructor arginfo_class_CParser_ClassIterator_valid

#define arginfo_class_CParser_MethodDecl_getSignature arginfo_class_CParser_ClassDecl_getName

#define arginfo_class_CParser_ParamDecl_getName arginfo_class_CParser_ClassDecl_getName

#define arginfo_class_CParser_ParamDecl_getType arginfo_class_CParser_MethodDecl_getReturnType

#define arginfo_class_CParser_Type_toString arginfo_class_CParser_ClassDecl_getName

#define arginfo_class_CParser_Type_isConstQualified arginfo_class_CParser_ClassIterator_valid

#define arginfo_class_CParser_Type_isPointer arginfo_class_CParser_ClassIterator_valid

#define arginfo_class_CParser_Type_isReference arginfo_class_CParser_ClassIterator_valid

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_CParser_Type_getPointeeType, 0, 0, CParser\\Type, 1)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_Type_getCanonicalType arginfo_class_CParser_MethodDecl_getReturnType

#define arginfo_class_CParser_Type_isTemplateSpecialization arginfo_class_CParser_ClassIterator_valid

#define arginfo_class_CParser_Type_getTemplateArguments arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_TemplateDecl_getName arginfo_class_CParser_ClassDecl_getName

#define arginfo_class_CParser_TemplateDecl_getParameters arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_TemplateParameter_getName arginfo_class_CParser_ClassDecl_getName

#define arginfo_class_CParser_TemplateParameter_getKind arginfo_class_CParser_ClassDecl_getName

#define arginfo_class_CParser_TemplateArgument_getKind arginfo_class_CParser_ClassDecl_getName

#define arginfo_class_CParser_TemplateArgument_getType arginfo_class_CParser_Type_getPointeeType

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_CParser_TemplateArgument_getValue, 0, 0, MAY_BE_LONG|MAY_BE_STRING|MAY_BE_DOUBLE|MAY_BE_NULL)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_EnumDecl_getName arginfo_class_CParser_ClassDecl_getName

#define arginfo_class_CParser_EnumDecl_getConstants arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_EnumDecl_getIntegerType arginfo_class_CParser_MethodDecl_getReturnType

#define arginfo_class_CParser_EnumConstant_getName arginfo_class_CParser_ClassDecl_getName

#define arginfo_class_CParser_EnumConstant_getValue arginfo_class_CParser_ClassIterator_key

#define arginfo_class_CParser_Diagnostic_getMessage arginfo_class_CParser_ClassDecl_getName

#define arginfo_class_CParser_Diagnostic_getSeverity arginfo_class_CParser_ClassIterator_key

#define arginfo_class_CParser_Diagnostic_getLine arginfo_class_CParser_ClassIterator_key

#define arginfo_class_CParser_Diagnostic_getColumn arginfo_class_CParser_ClassIterator_key

ZEND_METHOD(CParser_ClassIterator, __construct);
ZEND_METHOD(CParser_ClassIterator, current);
ZEND_METHOD(CParser_ClassIterator, key);
ZEND_METHOD(CParser_ClassIterator, next);
ZEND_METHOD(CParser_ClassIterator, rewind);
ZEND_METHOD(CParser_ClassIterator, valid);
ZEND_METHOD(CParser_TranslationUnit, fromFile);
ZEND_METHOD(CParser_TranslationUnit, classes);
ZEND_METHOD(CParser_TranslationUnit, enums);
ZEND_METHOD(CParser_TranslationUnit, diagnostics);
ZEND_METHOD(CParser_ClassDecl, getName);
ZEND_METHOD(CParser_ClassDecl, getBases);
ZEND_METHOD(CParser_ClassDecl, methods);
ZEND_METHOD(CParser_ClassDecl, innerClasses);
ZEND_METHOD(CParser_ClassDecl, enums);
ZEND_METHOD(CParser_ClassDecl, templates);
ZEND_METHOD(CParser_ClassDecl, isAbstract);
ZEND_METHOD(CParser_MethodDecl, getName);
ZEND_METHOD(CParser_MethodDecl, getAccess);
ZEND_METHOD(CParser_MethodDecl, getReturnType);
ZEND_METHOD(CParser_MethodDecl, getParameters);
ZEND_METHOD(CParser_MethodDecl, isStatic);
ZEND_METHOD(CParser_MethodDecl, isConst);
ZEND_METHOD(CParser_MethodDecl, isVirtual);
ZEND_METHOD(CParser_MethodDecl, isPureVirtual);
ZEND_METHOD(CParser_MethodDecl, isConstructor);
ZEND_METHOD(CParser_MethodDecl, isDestructor);
ZEND_METHOD(CParser_MethodDecl, getSignature);
ZEND_METHOD(CParser_ParamDecl, getName);
ZEND_METHOD(CParser_ParamDecl, getType);
ZEND_METHOD(CParser_Type, toString);
ZEND_METHOD(CParser_Type, isConstQualified);
ZEND_METHOD(CParser_Type, isPointer);
ZEND_METHOD(CParser_Type, isReference);
ZEND_METHOD(CParser_Type, getPointeeType);
ZEND_METHOD(CParser_Type, getCanonicalType);
ZEND_METHOD(CParser_Type, isTemplateSpecialization);
ZEND_METHOD(CParser_Type, getTemplateArguments);
ZEND_METHOD(CParser_TemplateDecl, getName);
ZEND_METHOD(CParser_TemplateDecl, getParameters);
ZEND_METHOD(CParser_TemplateParameter, getName);
ZEND_METHOD(CParser_TemplateParameter, getKind);
ZEND_METHOD(CParser_TemplateArgument, getKind);
ZEND_METHOD(CParser_TemplateArgument, getType);
ZEND_METHOD(CParser_TemplateArgument, getValue);
ZEND_METHOD(CParser_EnumDecl, getName);
ZEND_METHOD(CParser_EnumDecl, getConstants);
ZEND_METHOD(CParser_EnumDecl, getIntegerType);
ZEND_METHOD(CParser_EnumConstant, getName);
ZEND_METHOD(CParser_EnumConstant, getValue);
ZEND_METHOD(CParser_Diagnostic, getMessage);
ZEND_METHOD(CParser_Diagnostic, getSeverity);
ZEND_METHOD(CParser_Diagnostic, getLine);
ZEND_METHOD(CParser_Diagnostic, getColumn);

static const zend_function_entry class_CParser_ClassIterator_methods[] = {
	ZEND_ME(CParser_ClassIterator, __construct, arginfo_class_CParser_ClassIterator___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassIterator, current, arginfo_class_CParser_ClassIterator_current, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassIterator, key, arginfo_class_CParser_ClassIterator_key, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassIterator, next, arginfo_class_CParser_ClassIterator_next, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassIterator, rewind, arginfo_class_CParser_ClassIterator_rewind, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassIterator, valid, arginfo_class_CParser_ClassIterator_valid, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_TranslationUnit_methods[] = {
	ZEND_ME(CParser_TranslationUnit, fromFile, arginfo_class_CParser_TranslationUnit_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(CParser_TranslationUnit, classes, arginfo_class_CParser_TranslationUnit_classes, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_TranslationUnit, enums, arginfo_class_CParser_TranslationUnit_enums, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_TranslationUnit, diagnostics, arginfo_class_CParser_TranslationUnit_diagnostics, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_ClassDecl_methods[] = {
	ZEND_ME(CParser_ClassDecl, getName, arginfo_class_CParser_ClassDecl_getName, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassDecl, getBases, arginfo_class_CParser_ClassDecl_getBases, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassDecl, methods, arginfo_class_CParser_ClassDecl_methods, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassDecl, innerClasses, arginfo_class_CParser_ClassDecl_innerClasses, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassDecl, enums, arginfo_class_CParser_ClassDecl_enums, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassDecl, templates, arginfo_class_CParser_ClassDecl_templates, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassDecl, isAbstract, arginfo_class_CParser_ClassDecl_isAbstract, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_MethodDecl_methods[] = {
	ZEND_ME(CParser_MethodDecl, getName, arginfo_class_CParser_MethodDecl_getName, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodDecl, getAccess, arginfo_class_CParser_MethodDecl_getAccess, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodDecl, getReturnType, arginfo_class_CParser_MethodDecl_getReturnType, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodDecl, getParameters, arginfo_class_CParser_MethodDecl_getParameters, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodDecl, isStatic, arginfo_class_CParser_MethodDecl_isStatic, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodDecl, isConst, arginfo_class_CParser_MethodDecl_isConst, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodDecl, isVirtual, arginfo_class_CParser_MethodDecl_isVirtual, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodDecl, isPureVirtual, arginfo_class_CParser_MethodDecl_isPureVirtual, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodDecl, isConstructor, arginfo_class_CParser_MethodDecl_isConstructor, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodDecl, isDestructor, arginfo_class_CParser_MethodDecl_isDestructor, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodDecl, getSignature, arginfo_class_CParser_MethodDecl_getSignature, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_ParamDecl_methods[] = {
	ZEND_ME(CParser_ParamDecl, getName, arginfo_class_CParser_ParamDecl_getName, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ParamDecl, getType, arginfo_class_CParser_ParamDecl_getType, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_Type_methods[] = {
	ZEND_ME(CParser_Type, toString, arginfo_class_CParser_Type_toString, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Type, isConstQualified, arginfo_class_CParser_Type_isConstQualified, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Type, isPointer, arginfo_class_CParser_Type_isPointer, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Type, isReference, arginfo_class_CParser_Type_isReference, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Type, getPointeeType, arginfo_class_CParser_Type_getPointeeType, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Type, getCanonicalType, arginfo_class_CParser_Type_getCanonicalType, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Type, isTemplateSpecialization, arginfo_class_CParser_Type_isTemplateSpecialization, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Type, getTemplateArguments, arginfo_class_CParser_Type_getTemplateArguments, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_TemplateDecl_methods[] = {
	ZEND_ME(CParser_TemplateDecl, getName, arginfo_class_CParser_TemplateDecl_getName, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_TemplateDecl, getParameters, arginfo_class_CParser_TemplateDecl_getParameters, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_TemplateParameter_methods[] = {
	ZEND_ME(CParser_TemplateParameter, getName, arginfo_class_CParser_TemplateParameter_getName, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_TemplateParameter, getKind, arginfo_class_CParser_TemplateParameter_getKind, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_TemplateArgument_methods[] = {
	ZEND_ME(CParser_TemplateArgument, getKind, arginfo_class_CParser_TemplateArgument_getKind, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_TemplateArgument, getType, arginfo_class_CParser_TemplateArgument_getType, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_TemplateArgument, getValue, arginfo_class_CParser_TemplateArgument_getValue, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_EnumDecl_methods[] = {
	ZEND_ME(CParser_EnumDecl, getName, arginfo_class_CParser_EnumDecl_getName, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_EnumDecl, getConstants, arginfo_class_CParser_EnumDecl_getConstants, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_EnumDecl, getIntegerType, arginfo_class_CParser_EnumDecl_getIntegerType, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_EnumConstant_methods[] = {
	ZEND_ME(CParser_EnumConstant, getName, arginfo_class_CParser_EnumConstant_getName, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_EnumConstant, getValue, arginfo_class_CParser_EnumConstant_getValue, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_Diagnostic_methods[] = {
	ZEND_ME(CParser_Diagnostic, getMessage, arginfo_class_CParser_Diagnostic_getMessage, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Diagnostic, getSeverity, arginfo_class_CParser_Diagnostic_getSeverity, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Diagnostic, getLine, arginfo_class_CParser_Diagnostic_getLine, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Diagnostic, getColumn, arginfo_class_CParser_Diagnostic_getColumn, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_CParser_ClassIterator(zend_class_entry *class_entry_Iterator)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "ClassIterator", class_CParser_ClassIterator_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);
	zend_class_implements(class_entry, 1, class_entry_Iterator);

	return class_entry;
}

static zend_class_entry *register_class_CParser_TranslationUnit(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "TranslationUnit", class_CParser_TranslationUnit_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_ClassDecl(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "ClassDecl", class_CParser_ClassDecl_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_MethodDecl(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "MethodDecl", class_CParser_MethodDecl_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_ParamDecl(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "ParamDecl", class_CParser_ParamDecl_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_Type(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "Type", class_CParser_Type_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_TemplateDecl(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "TemplateDecl", class_CParser_TemplateDecl_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_TemplateParameter(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "TemplateParameter", class_CParser_TemplateParameter_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_TemplateArgument(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "TemplateArgument", class_CParser_TemplateArgument_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_EnumDecl(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "EnumDecl", class_CParser_EnumDecl_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_EnumConstant(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "EnumConstant", class_CParser_EnumConstant_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_Diagnostic(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "Diagnostic", class_CParser_Diagnostic_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_Access(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "Access", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	zval const_Public_value;
	ZVAL_LONG(&const_Public_value, 1);
	zend_string *const_Public_name = zend_string_init_interned("Public", sizeof("Public") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_Public_name, &const_Public_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_Public_name);

	zval const_Protected_value;
	ZVAL_LONG(&const_Protected_value, 2);
	zend_string *const_Protected_name = zend_string_init_interned("Protected", sizeof("Protected") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_Protected_name, &const_Protected_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_Protected_name);

	zval const_Private_value;
	ZVAL_LONG(&const_Private_value, 3);
	zend_string *const_Private_name = zend_string_init_interned("Private", sizeof("Private") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_Private_name, &const_Private_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_Private_name);

	return class_entry;
}
