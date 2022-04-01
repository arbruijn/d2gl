#ifndef __WATCOMC__
#define FORCE_INLINE static inline __attribute__((always_inline)) 
#else
#define FORCE_INLINE inline
#endif
