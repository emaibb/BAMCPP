#ifdef _WIN32
    #ifdef BAMCPP_EXPORTS
        #define DECL_SHARE __declspec(dllexport)
    #else
        #define DECL_SHARE __declspec(dllimport)
    #endif
#else
    #define DECL_SHARE
#endif