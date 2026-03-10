PHP_ARG_ENABLE([cparser],
  [whether to enable cparser support],
  [AS_HELP_STRING([--enable-cparser],
    [Enable cparser support])],
  [no])

AS_VAR_IF([PHP_CPARSER], [no],, [
  PHP_REQUIRE_CXX()
  
  AC_PATH_PROGS([LLVM_CONFIG], [llvm-config llvm-config-19 llvm-config-18], [no])

  if test -x "$LLVM_CONFIG"; then
    AC_MSG_CHECKING([for libclang via llvm-config])
    LIBCLANG_CFLAGS=`$LLVM_CONFIG --includedir --cflags`
    LIBCLANG_LIBDIR=`$LLVM_CONFIG --libdir`
    LIBCLANG_LIBS="-L$LIBCLANG_LIBDIR `$LLVM_CONFIG --libs --system-libs` -lclang"
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
  PHP_SUBST([CPARSER_SHARED_LIBADD])

  AC_CHECK_DECLS([
    clang_CXXMethod_isDeleted,
    clang_CXXMethod_isDefaulted,
    clang_CXXMethod_isExplicit,
    clang_CXXConstructor_isCopyConstructor,
    clang_CXXConstructor_isMoveConstructor,
    clang_CXXConstructor_isDefaultConstructor,
    clang_isVirtualBase,
    clang_getInclusions
  ], [], [], [[#include <clang-c/Index.h>]])

  cparser_source_files="cparser.cpp \
    src/TranslationUnit.cpp \
    src/Cursor.cpp \
    src/Type.cpp \
    src/TemplateDecl.cpp \
    src/TemplateParameter.cpp \
    src/TemplateArgument.cpp \
    src/Diagnostic.cpp \
    src/CursorIterator.cpp \
    src/DiagnosticIterator.cpp \
    src/TemplateArgumentIterator.cpp"
  
  PHP_NEW_EXTENSION([cparser],
    $cparser_source_files,
    [$ext_shared],,
    [-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1],
    [yes])
])
