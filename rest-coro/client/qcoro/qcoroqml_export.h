
#ifndef QCOROQML_EXPORT_H
#define QCOROQML_EXPORT_H

#ifdef QCOROQML_STATIC_DEFINE
#  define QCOROQML_EXPORT
#  define QCOROQML_NO_EXPORT
#else
#  ifndef QCOROQML_EXPORT
#    ifdef QCoro5Qml_EXPORTS
        /* We are building this library */
#      define QCOROQML_EXPORT 
#    else
        /* We are using this library */
#      define QCOROQML_EXPORT 
#    endif
#  endif

#  ifndef QCOROQML_NO_EXPORT
#    define QCOROQML_NO_EXPORT 
#  endif
#endif

#ifndef QCOROQML_DEPRECATED
#  define QCOROQML_DEPRECATED 
#endif

#ifndef QCOROQML_DEPRECATED_EXPORT
#  define QCOROQML_DEPRECATED_EXPORT QCOROQML_EXPORT QCOROQML_DEPRECATED
#endif

#ifndef QCOROQML_DEPRECATED_NO_EXPORT
#  define QCOROQML_DEPRECATED_NO_EXPORT QCOROQML_NO_EXPORT QCOROQML_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef QCOROQML_NO_DEPRECATED
#    define QCOROQML_NO_DEPRECATED
#  endif
#endif

#endif /* QCOROQML_EXPORT_H */
