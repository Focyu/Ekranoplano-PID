#ifndef PID_CONTROL__VISIBILITY_CONTROL_H_
#define PID_CONTROL__VISIBILITY_CONTROL_H_
#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define PID_CONTROL_EXPORT __attribute__ ((dllexport))
    #define PID_CONTROL_IMPORT __attribute__ ((dllimport))
  #else
    #define PID_CONTROL_EXPORT __declspec(dllexport)
    #define PID_CONTROL_IMPORT __declspec(dllimport)
  #endif
  #ifdef PID_CONTROL_BUILDING_LIBRARY
    #define PID_CONTROL_PUBLIC PID_CONTROL_EXPORT
  #else
    #define PID_CONTROL_PUBLIC PID_CONTROL_IMPORT
  #endif
  #define PID_CONTROL_PUBLIC_TYPE PID_CONTROL_PUBLIC
  #define PID_CONTROL_LOCAL
#else
  #define PID_CONTROL_EXPORT __attribute__ ((visibility("default")))
  #define PID_CONTROL_IMPORT
  #if __GNUC__ >= 4
    #define PID_CONTROL_PUBLIC __attribute__ ((visibility("default")))
    #define PID_CONTROL_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define PID_CONTROL_PUBLIC
    #define PID_CONTROL_LOCAL
  #endif
  #define PID_CONTROL_PUBLIC_TYPE
#endif
#endif  // PID_CONTROL__VISIBILITY_CONTROL_H_
// Generated 10-Oct-2026 16:43:36
 