#if defined(WIN32) && ! defined(__GNUC__)
#undef HAVE_LONG_LONG
#else
#define HAVE_LONG_LONG
#endif

#define SOCKETS_IMPLEMENTED

#define EXPRT
#define IMPRT

#if defined(WIN32) && ! defined(__CYGWIN__) && ! defined(STD_API)
 #ifdef __BORLAND__     
  #undef EXPRT
  #undef IMPRT
  #define EXPRT _export
  #define IMPRT _import
 #else
  #undef STD_API
  #ifdef _BUILD_STD
   #define STD_API __declspec(dllexport)
  #else
   #define STD_API __declspec(dllimport)
  #endif
  #define XML_API STD_API
 #endif
#else
#if ! defined(STD_API)
#define STD_API
#define XML_API
#endif
#endif

void *Malloc(int bytes);
void *Realloc(void *mem, int bytes);
void Free(void *mem);
