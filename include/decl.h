#ifdef _WIN32
    #ifdef BAM_cpp_EXPORTS
        #define BAM_CPP __declspec(dllexport)
    #else
        #define BAM_CPP __declspec(dllimport)
    #endif
#else
    #define BAM_CPP
#endif