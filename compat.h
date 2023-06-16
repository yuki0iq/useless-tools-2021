#ifndef _compat_h
#define _compat_h

#undef UNIX
#undef WIN

#define VERBOSE_DEBUG

#if defined( __GNUC__) || defined(sparc)

#define UNIX

#  if defined(sparc)
#  define SPARC
#  else
#  define LINUX
#  endif

#  define SSFLAT

//#define DEBUG_COMPAT
//#define DEBUG_CALC
//#define DEBUG_NLR
//#   define DUMP_CONSUMER
//#define DEBUG_SSVP
//#define DEBUG_MODEL
//#define DEBUG_STORAGE
//#define DEBUG_KIPARIS
//#define DEBUG_SERVICE

//#define STEPBYSTEP_DEBUG

#ifndef BULK_LINK
#define BULK_LINK
#endif // BULK_LINK

#  define __CDECL
#  define __STDCALL

#  if defined( __GNUC__)
  /**
   *  @brief This does what you think it does.
   *  @param  a  A thing of arbitrary type.
   *  @param  b  Another thing of arbitrary type.
   *  @return   The greater of the parameters.
   *
   *  This is the simple classic generic implementation.  It will work on
   *  temporary expressions, since they are only evaluated once, unlike a
   *  preprocessor macro.
  */
/*  template<typename Tp>
    inline const Tp&
    max(const Tp& a, const Tp& b)
    {
      //return  a < b ? b : a;
      if (a < b) return b; return a;
    }*/
#  elif defined( __SUNPRO_CC)
template <class Tp>
inline const Tp& (max)(const Tp& a, const Tp& b) {  return  a < b ? b : a; }
template <class Tp>
inline const Tp& (min)(const Tp& a, const Tp& b) {  return  a < b ? a : b; }
#  endif

#elif defined( _WIN32)

#define WIN

// For MS VC6 workarounds
#if defined (_MSC_VER) && (_MSC_VER <= 1300)

#define VC6
#define _NEED_MIN_MAX_REDEFINE_

#endif

#  undef  __CDECL
#  define __CDECL   __cdecl
#  define __STDCALL __stdcall

#ifndef max
#if (_MSC_VER < 1300)
// TEMPLATE FUNCTION max
template<class Ty> inline
	const Ty& max(const Ty& X, const Ty& Y)
	{return (X < Y ? Y : X); }
template<class Ty> inline
  const Ty& min(const Ty& X, const Ty& Y)
  {return (X < Y ? X : Y); }
#endif
#endif

#else

#  error "Unknown compilation target. Must be _WIN32 or __GNUC__ (Linux)."

#endif //_WIN32

#if defined(WIN) || defined(__MINGW32__)
#  define WIN_RUNTIME 1
#else
#  undef  WIN_RUNTIME
#endif

#ifdef BULK_LINK
template <typename PF_FUNC>
class linker_dummy_template
{
public:
  static PF_FUNC ThePtr;
};
#endif

#endif // _compat_h
