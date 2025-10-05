PHP_ARG_ENABLE([cparser],
  [whether to enable cparser support],
  [AS_HELP_STRING([--enable-cparser],
    [Enable cparser support])],
  [no])

AS_VAR_IF([PHP_CPARSER], [no],, [
  PHP_REQUIRE_CXX()
  
  AC_PATH_PROG(LLVM_CONFIG, llvm-config, no)

  if test -x "$LLVM_CONFIG"; then
    AC_MSG_CHECKING([for libclang via llvm-config])
    LIBCLANG_CFLAGS=`$LLVM_CONFIG --includedir --cflags`
    LIBCLANG_LIBS="`$LLVM_CONFIG --libs --system-libs` -lclang"
  else
    AC_MSG_CHECKING([for libclang via pkg-config])
    AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
    if test -x "$PKG_CONFIG"; then
      LIBCLANG_CFLAGS=`$PKG_CONFIG --cflags libclang`
      LIBCLANG_LIBS=`$PKG_CONFIG --libs libclang`
    else
      AC_MSG_ERROR([Neither pkg-config nor llvm-config found, cannot detect libclang])
    fi
  fi

  AC_MSG_RESULT([$LIBCLANG_CFLAGS])
  AC_MSG_RESULT([$LIBCLANG_LIBS])

  PHP_EVAL_INCLINE($LIBCLANG_CFLAGS)
  PHP_EVAL_LIBLINE($LIBCLANG_LIBS, CPARSER_SHARED_LIBADD)

  LDFLAGS="$LDFLAGS $LIBCLANG_LIBS"

  cparser_source_files="cparser.cpp \
    src/TranslationUnit.cpp \
    src/Cursor.cpp \
    src/Type.cpp \
    src/TemplateDecl.cpp \
    src/TemplateParameter.cpp \
    src/TemplateArgument.cpp \
    src/Diagnostic.cpp \
    src/CursorIterator.cpp"
  
  PHP_NEW_EXTENSION([cparser],
    $cparser_source_files,
    [$ext_shared],,
    [-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1])
])
