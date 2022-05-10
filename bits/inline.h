#ifndef S_INLINE_H
#define S_INLINE_H
#ifdef __OPTIMIZE__
#define S_USING_INLINE
#else
#endif

#ifdef S_USING_INLINE
#define __INLINE_ON_OPTIMIZE inline
#else
#define __INLINE_ON_OPTIMIZE
#endif
#endif