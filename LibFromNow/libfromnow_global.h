#pragma once

#include <QtCore/qglobal.h>

#if defined(LIBFROMNOW_LIBRARY)
#  define LIBFROMNOWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBFROMNOWSHARED_EXPORT Q_DECL_IMPORT
#endif

