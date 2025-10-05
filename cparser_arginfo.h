/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: c435367d13d3becc3b71ead3e466ba07b7fb45b8 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_CParser_CursorIterator___construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, tu, CParser\\TranslationUnit, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_CParser_CursorIterator_current, 0, 0, CParser\\Cursor, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_CParser_CursorIterator_key, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_CParser_CursorIterator_next, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_CursorIterator_rewind arginfo_class_CParser_CursorIterator_next

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_CParser_CursorIterator_valid, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_CParser_TranslationUnit_fromFile, 0, 1, CParser\\TranslationUnit, 0)
	ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, args, IS_ARRAY, 0, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_class_CParser_TranslationUnit_cursors, 0, 0, Traversable, MAY_BE_ARRAY)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, kind, IS_LONG, 0, "-1")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_class_CParser_TranslationUnit_classes, 0, 0, Traversable, MAY_BE_ARRAY)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_TranslationUnit_enums arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_TranslationUnit_diagnostics arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_Cursor_getKind arginfo_class_CParser_CursorIterator_key

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_CParser_Cursor_getSpelling, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_Cursor_getDisplayName arginfo_class_CParser_Cursor_getSpelling

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_CParser_Cursor_getLocation, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_CParser_Cursor_getType, 0, 0, CParser\\Type, 1)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_Cursor_isDefinition arginfo_class_CParser_CursorIterator_valid

#define arginfo_class_CParser_Cursor_getCanonical arginfo_class_CParser_CursorIterator_current

#define arginfo_class_CParser_Cursor_getParent arginfo_class_CParser_CursorIterator_current

#define arginfo_class_CParser_Cursor_getChildren arginfo_class_CParser_TranslationUnit_cursors

#define arginfo_class_CParser_ClassCursor_getBases arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_ClassCursor_getMethods arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_ClassCursor_getFields arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_ClassCursor_getInnerClasses arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_ClassCursor_getEnums arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_ClassCursor_isAbstract arginfo_class_CParser_CursorIterator_valid

#define arginfo_class_CParser_ClassCursor_isStruct arginfo_class_CParser_CursorIterator_valid

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_CParser_MethodCursor_getReturnType, 0, 0, CParser\\Type, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_MethodCursor_getParameters arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_MethodCursor_isStatic arginfo_class_CParser_CursorIterator_valid

#define arginfo_class_CParser_MethodCursor_isConst arginfo_class_CParser_CursorIterator_valid

#define arginfo_class_CParser_MethodCursor_isVirtual arginfo_class_CParser_CursorIterator_valid

#define arginfo_class_CParser_MethodCursor_isPureVirtual arginfo_class_CParser_CursorIterator_valid

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_CParser_MethodCursor_getAccessSpecifier, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_FunctionCursor_getReturnType arginfo_class_CParser_MethodCursor_getReturnType

#define arginfo_class_CParser_FunctionCursor_getParameters arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_FunctionCursor_getNumArguments arginfo_class_CParser_CursorIterator_key

#define arginfo_class_CParser_FieldCursor_getAccessSpecifier arginfo_class_CParser_MethodCursor_getAccessSpecifier

#define arginfo_class_CParser_FieldCursor_isStatic arginfo_class_CParser_CursorIterator_valid

#define arginfo_class_CParser_EnumCursor_getConstants arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_EnumCursor_getIntegerType arginfo_class_CParser_MethodCursor_getReturnType

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_CParser_EnumConstantCursor_getValue, 0, 0, MAY_BE_LONG|MAY_BE_DOUBLE|MAY_BE_STRING)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_ParameterCursor_isConstQualified arginfo_class_CParser_CursorIterator_valid

#define arginfo_class_CParser_ParameterCursor_hasDefaultValue arginfo_class_CParser_CursorIterator_valid

#define arginfo_class_CParser_TypeAliasCursor_getUnderlyingType arginfo_class_CParser_MethodCursor_getReturnType

#define arginfo_class_CParser_Type_toString arginfo_class_CParser_Cursor_getSpelling

#define arginfo_class_CParser_Type_isConstQualified arginfo_class_CParser_CursorIterator_valid

#define arginfo_class_CParser_Type_isPointer arginfo_class_CParser_CursorIterator_valid

#define arginfo_class_CParser_Type_isReference arginfo_class_CParser_CursorIterator_valid

#define arginfo_class_CParser_Type_getPointeeType arginfo_class_CParser_Cursor_getType

#define arginfo_class_CParser_Type_getCanonicalType arginfo_class_CParser_MethodCursor_getReturnType

#define arginfo_class_CParser_Type_isTemplateSpecialization arginfo_class_CParser_CursorIterator_valid

#define arginfo_class_CParser_Type_getTemplateArguments arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_TemplateDecl_getName arginfo_class_CParser_Cursor_getSpelling

#define arginfo_class_CParser_TemplateDecl_getParameters arginfo_class_CParser_TranslationUnit_classes

#define arginfo_class_CParser_TemplateParameter_getName arginfo_class_CParser_Cursor_getSpelling

#define arginfo_class_CParser_TemplateParameter_getKind arginfo_class_CParser_Cursor_getSpelling

#define arginfo_class_CParser_TemplateArgument_getKind arginfo_class_CParser_Cursor_getSpelling

#define arginfo_class_CParser_TemplateArgument_getType arginfo_class_CParser_Cursor_getType

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_CParser_TemplateArgument_getValue, 0, 0, MAY_BE_LONG|MAY_BE_STRING|MAY_BE_DOUBLE|MAY_BE_NULL)
ZEND_END_ARG_INFO()

#define arginfo_class_CParser_Diagnostic_getMessage arginfo_class_CParser_Cursor_getSpelling

#define arginfo_class_CParser_Diagnostic_getSeverity arginfo_class_CParser_CursorIterator_key

#define arginfo_class_CParser_Diagnostic_getLine arginfo_class_CParser_CursorIterator_key

#define arginfo_class_CParser_Diagnostic_getColumn arginfo_class_CParser_CursorIterator_key

ZEND_METHOD(CParser_CursorIterator, __construct);
ZEND_METHOD(CParser_CursorIterator, current);
ZEND_METHOD(CParser_CursorIterator, key);
ZEND_METHOD(CParser_CursorIterator, next);
ZEND_METHOD(CParser_CursorIterator, rewind);
ZEND_METHOD(CParser_CursorIterator, valid);
ZEND_METHOD(CParser_TranslationUnit, fromFile);
ZEND_METHOD(CParser_TranslationUnit, cursors);
ZEND_METHOD(CParser_TranslationUnit, classes);
ZEND_METHOD(CParser_TranslationUnit, enums);
ZEND_METHOD(CParser_TranslationUnit, diagnostics);
ZEND_METHOD(CParser_Cursor, getKind);
ZEND_METHOD(CParser_Cursor, getSpelling);
ZEND_METHOD(CParser_Cursor, getDisplayName);
ZEND_METHOD(CParser_Cursor, getLocation);
ZEND_METHOD(CParser_Cursor, getType);
ZEND_METHOD(CParser_Cursor, isDefinition);
ZEND_METHOD(CParser_Cursor, getCanonical);
ZEND_METHOD(CParser_Cursor, getParent);
ZEND_METHOD(CParser_Cursor, getChildren);
ZEND_METHOD(CParser_ClassCursor, getBases);
ZEND_METHOD(CParser_ClassCursor, getMethods);
ZEND_METHOD(CParser_ClassCursor, getFields);
ZEND_METHOD(CParser_ClassCursor, getInnerClasses);
ZEND_METHOD(CParser_ClassCursor, getEnums);
ZEND_METHOD(CParser_ClassCursor, isAbstract);
ZEND_METHOD(CParser_ClassCursor, isStruct);
ZEND_METHOD(CParser_MethodCursor, getReturnType);
ZEND_METHOD(CParser_MethodCursor, getParameters);
ZEND_METHOD(CParser_MethodCursor, isStatic);
ZEND_METHOD(CParser_MethodCursor, isConst);
ZEND_METHOD(CParser_MethodCursor, isVirtual);
ZEND_METHOD(CParser_MethodCursor, isPureVirtual);
ZEND_METHOD(CParser_MethodCursor, getAccessSpecifier);
ZEND_METHOD(CParser_FunctionCursor, getReturnType);
ZEND_METHOD(CParser_FunctionCursor, getParameters);
ZEND_METHOD(CParser_FunctionCursor, getNumArguments);
ZEND_METHOD(CParser_FieldCursor, getAccessSpecifier);
ZEND_METHOD(CParser_FieldCursor, isStatic);
ZEND_METHOD(CParser_EnumCursor, getConstants);
ZEND_METHOD(CParser_EnumCursor, getIntegerType);
ZEND_METHOD(CParser_EnumConstantCursor, getValue);
ZEND_METHOD(CParser_ParameterCursor, isConstQualified);
ZEND_METHOD(CParser_ParameterCursor, hasDefaultValue);
ZEND_METHOD(CParser_TypeAliasCursor, getUnderlyingType);
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
ZEND_METHOD(CParser_Diagnostic, getMessage);
ZEND_METHOD(CParser_Diagnostic, getSeverity);
ZEND_METHOD(CParser_Diagnostic, getLine);
ZEND_METHOD(CParser_Diagnostic, getColumn);

static const zend_function_entry class_CParser_CursorIterator_methods[] = {
	ZEND_ME(CParser_CursorIterator, __construct, arginfo_class_CParser_CursorIterator___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_CursorIterator, current, arginfo_class_CParser_CursorIterator_current, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_CursorIterator, key, arginfo_class_CParser_CursorIterator_key, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_CursorIterator, next, arginfo_class_CParser_CursorIterator_next, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_CursorIterator, rewind, arginfo_class_CParser_CursorIterator_rewind, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_CursorIterator, valid, arginfo_class_CParser_CursorIterator_valid, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_TranslationUnit_methods[] = {
	ZEND_ME(CParser_TranslationUnit, fromFile, arginfo_class_CParser_TranslationUnit_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(CParser_TranslationUnit, cursors, arginfo_class_CParser_TranslationUnit_cursors, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_TranslationUnit, classes, arginfo_class_CParser_TranslationUnit_classes, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_TranslationUnit, enums, arginfo_class_CParser_TranslationUnit_enums, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_TranslationUnit, diagnostics, arginfo_class_CParser_TranslationUnit_diagnostics, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_Cursor_methods[] = {
	ZEND_ME(CParser_Cursor, getKind, arginfo_class_CParser_Cursor_getKind, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Cursor, getSpelling, arginfo_class_CParser_Cursor_getSpelling, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Cursor, getDisplayName, arginfo_class_CParser_Cursor_getDisplayName, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Cursor, getLocation, arginfo_class_CParser_Cursor_getLocation, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Cursor, getType, arginfo_class_CParser_Cursor_getType, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Cursor, isDefinition, arginfo_class_CParser_Cursor_isDefinition, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Cursor, getCanonical, arginfo_class_CParser_Cursor_getCanonical, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Cursor, getParent, arginfo_class_CParser_Cursor_getParent, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Cursor, getChildren, arginfo_class_CParser_Cursor_getChildren, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_ClassCursor_methods[] = {
	ZEND_ME(CParser_ClassCursor, getBases, arginfo_class_CParser_ClassCursor_getBases, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassCursor, getMethods, arginfo_class_CParser_ClassCursor_getMethods, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassCursor, getFields, arginfo_class_CParser_ClassCursor_getFields, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassCursor, getInnerClasses, arginfo_class_CParser_ClassCursor_getInnerClasses, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassCursor, getEnums, arginfo_class_CParser_ClassCursor_getEnums, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassCursor, isAbstract, arginfo_class_CParser_ClassCursor_isAbstract, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ClassCursor, isStruct, arginfo_class_CParser_ClassCursor_isStruct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_MethodCursor_methods[] = {
	ZEND_ME(CParser_MethodCursor, getReturnType, arginfo_class_CParser_MethodCursor_getReturnType, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodCursor, getParameters, arginfo_class_CParser_MethodCursor_getParameters, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodCursor, isStatic, arginfo_class_CParser_MethodCursor_isStatic, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodCursor, isConst, arginfo_class_CParser_MethodCursor_isConst, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodCursor, isVirtual, arginfo_class_CParser_MethodCursor_isVirtual, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodCursor, isPureVirtual, arginfo_class_CParser_MethodCursor_isPureVirtual, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_MethodCursor, getAccessSpecifier, arginfo_class_CParser_MethodCursor_getAccessSpecifier, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_FunctionCursor_methods[] = {
	ZEND_ME(CParser_FunctionCursor, getReturnType, arginfo_class_CParser_FunctionCursor_getReturnType, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_FunctionCursor, getParameters, arginfo_class_CParser_FunctionCursor_getParameters, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_FunctionCursor, getNumArguments, arginfo_class_CParser_FunctionCursor_getNumArguments, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_FieldCursor_methods[] = {
	ZEND_ME(CParser_FieldCursor, getAccessSpecifier, arginfo_class_CParser_FieldCursor_getAccessSpecifier, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_FieldCursor, isStatic, arginfo_class_CParser_FieldCursor_isStatic, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_EnumCursor_methods[] = {
	ZEND_ME(CParser_EnumCursor, getConstants, arginfo_class_CParser_EnumCursor_getConstants, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_EnumCursor, getIntegerType, arginfo_class_CParser_EnumCursor_getIntegerType, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_EnumConstantCursor_methods[] = {
	ZEND_ME(CParser_EnumConstantCursor, getValue, arginfo_class_CParser_EnumConstantCursor_getValue, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_ParameterCursor_methods[] = {
	ZEND_ME(CParser_ParameterCursor, isConstQualified, arginfo_class_CParser_ParameterCursor_isConstQualified, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_ParameterCursor, hasDefaultValue, arginfo_class_CParser_ParameterCursor_hasDefaultValue, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_CParser_TypeAliasCursor_methods[] = {
	ZEND_ME(CParser_TypeAliasCursor, getUnderlyingType, arginfo_class_CParser_TypeAliasCursor_getUnderlyingType, ZEND_ACC_PUBLIC)
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

static const zend_function_entry class_CParser_Diagnostic_methods[] = {
	ZEND_ME(CParser_Diagnostic, getMessage, arginfo_class_CParser_Diagnostic_getMessage, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Diagnostic, getSeverity, arginfo_class_CParser_Diagnostic_getSeverity, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Diagnostic, getLine, arginfo_class_CParser_Diagnostic_getLine, ZEND_ACC_PUBLIC)
	ZEND_ME(CParser_Diagnostic, getColumn, arginfo_class_CParser_Diagnostic_getColumn, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_CParser_CursorIterator(zend_class_entry *class_entry_Iterator)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "CursorIterator", class_CParser_CursorIterator_methods);
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

static zend_class_entry *register_class_CParser_Cursor(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "Cursor", class_CParser_Cursor_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, 0);

	return class_entry;
}

static zend_class_entry *register_class_CParser_ClassCursor(zend_class_entry *class_entry_CParser_Cursor)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "ClassCursor", class_CParser_ClassCursor_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_CParser_Cursor, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_MethodCursor(zend_class_entry *class_entry_CParser_Cursor)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "MethodCursor", class_CParser_MethodCursor_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_CParser_Cursor, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_FunctionCursor(zend_class_entry *class_entry_CParser_Cursor)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "FunctionCursor", class_CParser_FunctionCursor_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_CParser_Cursor, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_FieldCursor(zend_class_entry *class_entry_CParser_Cursor)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "FieldCursor", class_CParser_FieldCursor_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_CParser_Cursor, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_EnumCursor(zend_class_entry *class_entry_CParser_Cursor)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "EnumCursor", class_CParser_EnumCursor_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_CParser_Cursor, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_EnumConstantCursor(zend_class_entry *class_entry_CParser_Cursor)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "EnumConstantCursor", class_CParser_EnumConstantCursor_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_CParser_Cursor, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_ParameterCursor(zend_class_entry *class_entry_CParser_Cursor)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "ParameterCursor", class_CParser_ParameterCursor_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_CParser_Cursor, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_NamespaceCursor(zend_class_entry *class_entry_CParser_Cursor)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "NamespaceCursor", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_CParser_Cursor, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_CParser_TypeAliasCursor(zend_class_entry *class_entry_CParser_Cursor)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "TypeAliasCursor", class_CParser_TypeAliasCursor_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_CParser_Cursor, ZEND_ACC_FINAL);

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

static zend_class_entry *register_class_CParser_CursorKind(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "CParser", "CursorKind", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
