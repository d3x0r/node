/* <link sack::image::render::PRENDERER, Render> provides a
   method to display images on a screen. It is the interface
   between memory images and the window desktop or frame buffer
   the user is viewing on a monitor.
   Under windows, this is implemented as an HWND and an HBITMAP
   used to allow the application to draw. Updates are done
   directly from the drawable surface to the HWND as appropriate
   for the type of service. This is implemented with Vidlib.dll.
   Under Linux, this is mounted against SDL. SDL, however, does
   not give multiple display surfaces, so a more direct method
   should be used someday, other than SDL does a good job of
   aliasing frame buffer and X display windows to a consistant
   interface. This is implemented wit DisplayLib (as yet outside
   of the documentation). Display lib can interface either
   directly, or be mounted as a service across from a shared
   memory message service, and host multiple applications on a
   single frame buffer surface.
   TODO
   Implement displays as direct X displays, and allow managment
   there under linux.
   Displaylib was a good project, and although suffers from
   code-rot, it is probably still mostly usable. Message
   services were easily transported across a network, but then
   location services started failing.
   Example
   <code lang="c++">
   // get a render display, just a default window of some size
   // extended features are available for more precision.
   Render render = OpenDisplay(0);
   </code>
   A few methods of using this surface are available. One, you
   may register for events, and be called as required.
   <code lang="c++">
   RedrawCallback MyDraw = DrawHandler;
   MouseCallback MyMouse;
   </code>
   <code>
   KeyProc MyKey;
   CloseCallback MyClose;
   </code>
   <code lang="c++">
   // called when the surface is initially shown, or when its surface changes.
   // otherwise, the image drawn by the application is static, and does
   // not get an update event.
   SetRedrawHandler( render, MyDraw, 0 );
   // This will get an event every time a mouse event happens.
   // If no Key handler is specified, key strokes will also be mouse events.
   SetMouseHandler( render, MyMouse, 0 );
   // If the window was closed, get an event.
   SetCloseHandler( render, MyClose, 0 );
   // specify a handler to get keyboard events...
   SetKeyboardHandler( render, MyKey, 0 );
   </code>
   Or, if you don't really care about any events...
   <code lang="c++">
   // load an image
   Image image = LoadImageFile( "sample.jpg" );
   // get the image target of render
   Image display = GetDisplayImage( render );
   // copy the loaded image to the display image
   BlotImage( display, image );
   // and update the display
   UpdateDisplay( render );
   </code>
   <code lang="c++">
   void CPROC DrawHandler( uintptr_t psvUserData, 31~PRENDERER render )
   {
       Image display = GetDisplayImage( render );
       // the display image may change, because of an external resize
       // update the image to display as desired...
       // when done, the draw handler should call UpdateDisplay or...
       UpdateDisplayPortion( render, 0, 0, 100, 100 );
   }
   </code>
   Oh! And most importantly! Have to call this to put the window
   on the screen.
   <code lang="c++">
   UpdateDisplay( render );
   </code>
   Or maybe can pretend it was hidden
   <code lang="c++">
   RestoreDisplay( render );
   </code>                                                                     */
// this shouldprobably be interlocked with
//  display.h or vidlib.h(video.h)
#ifndef RENDER_INTERFACE_INCLUDED
// multiple inclusion protection symbol.
#define RENDER_INTERFACE_INCLUDED
/* Includes the system platform as required or appropriate. If
   under a linux system, include appropriate basic linux type
   headers, if under windows pull "windows.h".
   Includes the MOST stuff here ( a full windows.h parse is many
   many lines of code.)                                          */
/* A macro to build a wide character string of __FILE__ */
#define _WIDE__FILE__(n) n
#define WIDE__FILE__ _WIDE__FILE__(__FILE__)
#if _XOPEN_SOURCE < 500
#  undef _XOPEN_SOURCE
#  define _XOPEN_SOURCE 500
#endif
#ifndef STANDARD_HEADERS_INCLUDED
/* multiple inclusion protection symbol */
#define STANDARD_HEADERS_INCLUDED
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#if _MSC_VER
#  ifdef EXCLUDE_SAFEINT_H
#    define _INTSAFE_H_INCLUDED_
#  endif
 //_MSC_VER
#endif
#ifndef WINVER
#  define WINVER 0x0601
#endif
#ifndef _WIN32
#  ifndef __LINUX__
#    define __LINUX__
#  endif
#endif
#if !defined(__LINUX__)
#  ifndef STRICT
#    define STRICT
#  endif
#  define WIN32_LEAN_AND_MEAN
// #define NOGDICAPMASKS             // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
// #define NOVIRTUALKEYCODES         // VK_*
// #define NOWINMESSAGES             // WM_*, EM_*, LB_*, CB_*
// #define NOWINSTYLES               // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
// #define NOSYSMETRICS              // SM_*
// #define NOMENUS                   // MF_*
// #define NOICONS                   // IDI_*
// #define NOKEYSTATES               // MK_*
// #define NOSYSCOMMANDS             // SC_*
// #define NORASTEROPS               // Binary and Tertiary raster ops
// #define NOSHOWWINDOW              // SW_*
               // OEM Resource values
#  define OEMRESOURCE
// #define NOATOM                    // Atom Manager routines
#  ifndef _INCLUDE_CLIPBOARD
               // Clipboard routines
#    define NOCLIPBOARD
#  endif
// #define NOCOLOR                   // Screen colors
// #define NOCTLMGR                  // Control and Dialog routines
//(spv) #define NODRAWTEXT                // DrawText() and DT_*
// #define NOGDI                     // All GDI defines and routines
// #define NOKERNEL                  // All KERNEL defines and routines
// #define NOUSER                    // All USER defines and routines
#  ifndef _ARM_
#    ifndef _INCLUDE_NLS
                     // All NLS defines and routines
#      define NONLS
#    endif
#  endif
// #define NOMB                      // MB_* and MessageBox()
                  // GMEM_*, LMEM_*, GHND, LHND, associated routines
#  define NOMEMMGR
                // typedef METAFILEPICT
#  define NOMETAFILE
#  ifndef NOMINMAX
                  // Macros min(a,b) and max(a,b)
#    define NOMINMAX
#  endif
// #define NOMSG                     // typedef MSG and associated routines
// #define NOOPENFILE                // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
// #define NOSCROLL                  // SB_* and scrolling routines
                 // All Service Controller routines, SERVICE_ equates, etc.
#  define NOSERVICE
//#define NOSOUND                   // Sound driver routines
#  ifndef _INCLUDE_TEXTMETRIC
              // typedef TEXTMETRIC and associated routines
#    define NOTEXTMETRIC
#  endif
// #define NOWH                      // SetWindowsHook and WH_*
// #define NOWINOFFSETS              // GWL_*, GCL_*, associated routines
// #define NOCOMM                    // COMM driver routines
                   // Kanji support stuff.
#  define NOKANJI
                    // Help engine interface.
#  define NOHELP
                // Profiler interface.
#  define NOPROFILER
//#define NODEFERWINDOWPOS          // DeferWindowPos routines
                     // Modem Configuration Extensions
#  define NOMCX
   // no StrCat StrCmp StrCpy etc functions.  (used internally)
#  define NO_SHLWAPI_STRFCNS
  // This also has defines that override StrCmp StrCpy etc... but no override
#  define STRSAFE_NO_DEPRECATE
#  ifdef _MSC_VER
#    ifndef _WIN32_WINDOWS
// needed at least this for what - updatelayeredwindow?
#      define _WIN32_WINDOWS 0x0601
#    endif
#  endif
// INCLUDE WINDOWS.H
#  ifdef __WATCOMC__
#    undef _WINDOWS_
#  endif
#  ifdef UNDER_CE
// just in case windows.h also fails after undef WIN32
// these will be the correct order for primitives we require.
#    include <excpt.h>
#    include <windef.h>
#    include <winnt.h>
#    include <winbase.h>
#    include <wingdi.h>
#    include <wtypes.h>
#    include <winuser.h>
#    undef WIN32
#  endif
#  define _WINSOCKAPI_
#  include <windows.h>
#  undef _WINSOCKAPI_
#  if defined( WIN32 ) && defined( NEED_SHLOBJ )
#    include <shlobj.h>
#  endif
#  if _MSC_VER > 1500
#    define fileno _fileno
#    define stricmp _stricmp
#    define strdup _strdup
#  endif
//#  include <windowsx.h>
// we like timeGetTime() instead of GetTickCount()
//#  include <mmsystem.h>
#ifdef __cplusplus
extern "C"
#endif
__declspec(dllimport) DWORD WINAPI timeGetTime(void);
#  ifdef WIN32
#    if defined( NEED_SHLAPI )
#      include <shlwapi.h>
#      include <shellapi.h>
#    endif
#    ifdef NEED_V4W
#      include <vfw.h>
#    endif
#  endif
#  if defined( HAVE_ENVIRONMENT )
#    define getenv(name)       OSALOT_GetEnvironmentVariable(name)
#    define setenv(name,val)   SetEnvironmentVariable(name,val)
#  endif
#  define Relinquish()       Sleep(0)
//#pragma pragnoteonly("GetFunctionAddress is lazy and has no library cleanup - needs to be a lib func")
//#define GetFunctionAddress( lib, proc ) GetProcAddress( LoadLibrary( lib ), (proc) )
#  ifdef __cplusplus_cli
#    include <vcclr.h>
 /*lprintf( */
#    define DebugBreak() System::Console::WriteLine(gcnew System::String( WIDE__FILE__ "(" STRSYM(__LINE__) ") Would DebugBreak here..." ) );
//typedef unsigned int HANDLE;
//typedef unsigned int HMODULE;
//typedef unsigned int HWND;
//typedef unsigned int HRC;
//typedef unsigned int HMENU;
//typedef unsigned int HICON;
//typedef unsigned int HINSTANCE;
#  endif
 // ifdef unix/linux
#else
#  include <pthread.h>
#  include <sched.h>
#  include <unistd.h>
#  include <sys/time.h>
#  include <errno.h>
#  if defined( __ARM__ )
#    define DebugBreak()
#  else
/* A symbol used to cause a debugger to break at a certain
   point. Sometimes dynamicly loaded plugins can be hard to set
   the breakpoint in the debugger, so it becomes easier to
   recompile with a breakpoint in the right place.
   Example
   <code lang="c++">
   DebugBreak();
	</code>                                                      */
#    ifdef __ANDROID__
#      define DebugBreak()
#    else
#      if defined( __EMSCRIPTEN__ ) || defined( __ARM__ )
#        define DebugBreak()
#      else
#        define DebugBreak()  __asm__("int $3\n" )
#      endif
#    endif
#  endif
#  ifdef __ANDROID_OLD_PLATFORM_SUPPORT__
extern __sighandler_t bsd_signal(int, __sighandler_t);
#  endif
// moved into timers - please linnk vs timers to get Sleep...
//#define Sleep(n) (usleep((n)*1000))
#  define Relinquish() sched_yield()
#  define GetLastError() (int32_t)errno
/* return with a THREAD_ID that is a unique, universally
   identifier for the thread for inter process communication. */
#  define GetCurrentProcessId() ((uint32_t)getpid())
#  define GetCurrentThreadId() ((uint32_t)getpid())
  // end if( !__LINUX__ )
#endif
#ifndef NEED_MIN_MAX
#  ifndef NO_MIN_MAX_MACROS
#    define NO_MIN_MAX_MACROS
#  endif
#endif
#ifndef NO_MIN_MAX_MACROS
#  ifdef __cplusplus
#    ifdef __GNUC__
#      ifndef min
#        define min(a,b) ((a)<(b))?(a):(b)
#      endif
#    endif
#  endif
/* Define a min(a,b) macro when the compiler lacks it. */
#  ifndef min
#    define min(a,b) (((a)<(b))?(a):(b))
#  endif
/* Why not add the max macro, also? */
#  ifndef max
#    define max(a,b) (((a)>(b))?(a):(b))
#  endif
#endif
#ifndef SACK_PRIMITIVE_TYPES_INCLUDED
#define SACK_PRIMITIVE_TYPES_INCLUDED
/* Define most of the sack core types on which everything else is
   based. Also defines some of the primitive container
   structures. We also handle a lot of platform/compiler
   abstraction here.
   A reFactoring for stdint.h and uint32_t etc would be USEFUL!
   where types don't exist, define them as apprpritate types instead.
But WHO doesn't have stdint?  BTW is sizeof( size_t ) == sizeof( void* )
   This is automatically included with stdhdrs.h; however, when
   including sack_types.h, the minimal headers are pulled. */
#define HAS_STDINT
//#define USE_SACK_CUSTOM_MEMORY_ALLOCATION
	// this has to be a compile option (option from cmake)
   // enables debug dump mem...
#ifdef USE_SACK_CUSTOM_MEMORY_ALLOCATION
#  define USE_CUSTOM_ALLOCER 1
#else
#  define USE_CUSTOM_ALLOCER 0
#endif
#ifndef __64__
#  if defined( _WIN64 ) || defined( ENVIRONMENT64 ) || defined( __x86_64__ ) || defined( __ia64 ) || defined( __ppc64__ ) || defined( __LP64__ )
#    define __64__ 1
#  endif
#endif
#ifdef _MSC_VER
#  ifndef _WIN32_WINNT
#    define _WIN32_WINNT 0x501
#  endif
#  ifndef WIN32
#    ifdef _WIN32
#      define WIN32 _WIN32
#    endif
#  endif
// force windows on __MSVC
#  ifndef WIN32
#    define WIN32
#  endif
#endif
#if !defined( __NO_THREAD_LOCAL__ ) && ( defined( _MSC_VER ) || defined( __WATCOMC__ ) )
#  define HAS_TLS 1
#  ifdef __cplusplus
#    define DeclareThreadLocal thread_local
#    define DeclareThreadVar  thread_local
#  else
#    define DeclareThreadLocal static __declspec(thread)
#    define DeclareThreadVar __declspec(thread)
#  endif
#elif !defined( __NO_THREAD_LOCAL__ ) && ( defined( __GNUC__ ) )
#    define HAS_TLS 1
#    ifdef __cplusplus
#      define DeclareThreadLocal thread_local
#      define DeclareThreadVar thread_local
#    else
#    define DeclareThreadLocal static __thread
#    define DeclareThreadVar __thread
#  endif
#else
// if no HAS_TLS
#  define DeclareThreadLocal static
#  define DeclareThreadVar
#endif
#ifdef __cplusplus_cli
// these things define a type called 'Byte'
	// which causes confusion... so don't include vcclr for those guys.
#  ifdef SACK_BAG_EXPORTS
// maybe only do this while building sack_bag project itself...
#    if !defined( ZCONF_H )        && !defined( __FT2_BUILD_GENERIC_H__ )        && !defined( ZUTIL_H )        && !defined( SQLITE_PRIVATE )        && !defined( NETSERVICE_SOURCE )        && !defined( LIBRARY_DEF )
//using namespace System;
#    endif
#  endif
#endif
// Defined for building visual studio monolithic build.  These symbols are not relavent with cmakelists.
#ifdef SACK_BAG_EXPORTS
#  define SACK_BAG_CORE_EXPORTS
// exports don't really matter with CLI compilation.
#  ifndef BAG
//#ifndef TARGETNAME
//#  define TARGETNAME "sack_bag.dll"  //$(TargetFileName)
//#endif
#    ifndef __cplusplus_cli
// cli mode, we use this directly, and build the exports in sack_bag.dll directly
#    else
#      define LIBRARY_DEADSTART
#    endif
#define MD5_SOURCE
#define USE_SACK_FILE_IO
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define MEM_LIBRARY_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define SYSLOG_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define _TYPELIBRARY_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define HTTP_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define TIMER_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define IDLE_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define CLIENTMSG_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define FRACTION_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define NETWORK_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define CONFIGURATION_LIBRARY_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define FILESYSTEM_LIBRARY_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define SYSTEM_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define FILEMONITOR_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define VECTOR_LIBRARY_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define SHA1_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define CONSTRUCT_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define PROCREG_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define SQLPROXY_LIBRARY_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define TYPELIB_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define JSON_EMITTER_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define SERVICE_SOURCE
#  ifndef __NO_SQL__
#    ifndef __NO_OPTIONS__
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.    and not NO_SQL and not NO_OPTIONS   */
#      define SQLGETOPTION_SOURCE
#    endif
#  endif
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define PSI_SOURCE
#  ifdef _MSC_VER
#    ifndef JPEG_SOURCE
//wouldn't matter... the external things wouldn't need to define this
//#error projects were not generated with CMAKE, and JPEG_SORUCE needs to be defined
#    endif
//#define JPEG_SOURCE
//#define __PNG_LIBRARY_SOURCE__
//#define FT2_BUILD_LIBRARY   // freetype is internal
//#define FREETYPE_SOURCE		// build Dll Export
#  endif
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define MNG_BUILD_DLL
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define BAGIMAGE_EXPORTS
/* Defined when SACK_BAG_EXPORTS is defined. This was an
 individual library module once upon a time.           */
#ifndef IMAGE_LIBRARY_SOURCE
#  define IMAGE_LIBRARY_SOURCE
#endif
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define SYSTRAY_LIBRARAY
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define SOURCE_PSI2
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
#define VIDEO_LIBRARY_SOURCE
/* Defined when SACK_BAG_EXPORTS is defined. This was an
   individual library module once upon a time.           */
	/* define RENDER SOURCE when building monolithic. */
#     ifndef RENDER_LIBRARY_SOURCE
#       define RENDER_LIBRARY_SOURCE
#     endif
// define a type that is a public name struct type...
// good thing that typedef and struct were split
// during the process of port to /clr option.
//#define PUBLIC_TYPE public
#  else
//#define PUBLIC_TYPE
#    ifdef __cplusplus_CLR
//using namespace System;
#    endif
#  endif
#endif
 // wchar for X_16 definition
#include <wchar.h>
#include <sys/types.h>
#include <sys/stat.h>
#if !defined( _WIN32 ) && !defined( __MAC__ )
#  include <syscall.h>
#elif defined( __MAC__ )
#  include <sys/syscall.h>
#endif
#ifndef MY_TYPES_INCLUDED
#  define MY_TYPES_INCLUDED
// include this before anything else
// thereby allowing us to redefine exit()
 // CHAR_BIT
#  include <limits.h>
 // typelib requires this
#  include <stdarg.h>
#  ifdef _MSC_VER
#    ifndef UNDER_CE
 // memlib requires this, and it MUST be included befoer string.h if it is used.
#      include <intrin.h>
#    endif
#  endif
 // typelib requires this
#  include <string.h>
#  if !defined( WIN32 ) && !defined( _WIN32 ) && !defined( _PNACL )
#    include <dlfcn.h>
#  endif
#  if defined( _MSC_VER )
// disable pointer conversion warnings - wish I could disable this
// according to types...
//#pragma warning( disable:4312; disable:4311 )
// disable deprication warnings of snprintf, et al.
//#pragma warning( disable:4996 )
#    define EMPTY_STRUCT struct { char nothing[]; }
#  endif
#  if defined( __WATCOMC__ )
#     define EMPTY_STRUCT char
#  endif
#  ifdef __cplusplus
/* Could also consider defining 'SACK_NAMESPACE' as 'extern "C"
   ' {' and '..._END' as '}'                                    */
#    define SACK_NAMESPACE namespace sack {
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define SACK_NAMESPACE_END }
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _CONTAINER_NAMESPACE namespace containers {
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _CONTAINER_NAMESPACE_END }
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _LINKLIST_NAMESPACE namespace list {
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _LINKLIST_NAMESPACE_END }
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _DATALIST_NAMESPACE namespace data_list {
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _DATALIST_NAMESPACE_END }
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _SETS_NAMESPACE namespace sets {
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _SETS_NAMESPACE_END }
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _TEXT_NAMESPACE namespace text {
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _TEXT_NAMESPACE_END }
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define TEXT_NAMESPACE SACK_NAMESPACE _CONTAINER_NAMESPACE namespace text {
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define TEXT_NAMESPACE_END  } _CONTAINER_NAMESPACE_END SACK_NAMESPACE_END
#  else
/* Define the sack namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define SACK_NAMESPACE
/* Define the sack namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define SACK_NAMESPACE_END
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _CONTAINER_NAMESPACE
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _CONTAINER_NAMESPACE_END
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _LINKLIST_NAMESPACE
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _LINKLIST_NAMESPACE_END
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _DATALIST_NAMESPACE
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _DATALIST_NAMESPACE_END
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _SETS_NAMESPACE
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _SETS_NAMESPACE_END
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _TEXT_NAMESPACE
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define _TEXT_NAMESPACE_END
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define TEXT_NAMESPACE
/* Define the container namespace (when building with C++, the
   wrappers are namespace{} instead of extern"c"{} )           */
#    define TEXT_NAMESPACE_END
#  endif
/* declare composite SACK_CONTAINER namespace to declare sack::container in a single line */
#  define SACK_CONTAINER_NAMESPACE SACK_NAMESPACE _CONTAINER_NAMESPACE
/* declare composite SACK_CONTAINER namespace to close sack::container in a single line */
#  define SACK_CONTAINER_NAMESPACE_END _CONTAINER_NAMESPACE_END SACK_NAMESPACE_END
/* declare composite SACK_CONTAINER namespace to declare sack::container::list in a single line */
#  define SACK_CONTAINER_LINKLIST_NAMESPACE SACK_CONTAINER_NAMESPACE _LISTLIST_NAMESPACE
/* declare composite SACK_CONTAINER namespace to close sack::container::list in a single line */
#  define SACK_CONTAINER_LINKLIST_NAMESPACE_END _LISTLIST_NAMESPACE_END SACK_CONTAINER_NAMESPACE
// this symbols is defined to enforce
// the C Procedure standard - using a stack, and resulting
// in EDX:EAX etc...
#  define CPROC
#  ifdef SACK_BAG_EXPORTS
#    ifdef BUILD_GLUE
// this is used as the export method appropriate for C#?
#      define EXPORT_METHOD [DllImport(LibName)] public
#    else
#      ifdef __cplusplus_cli
#        if defined( __STATIC__ ) || defined( __LINUX__ ) || defined( __ANDROID__ )
#          define EXPORT_METHOD
#          define IMPORT_METHOD extern
#        else
#          define EXPORT_METHOD __declspec(dllexport)
#          define IMPORT_METHOD __declspec(dllimport)
#        endif
#        define LITERAL_LIB_EXPORT_METHOD __declspec(dllexport)
#        define LITERAL_LIB_IMPORT_METHOD extern
//__declspec(dllimport)
#      else
#        if defined( __STATIC__ ) || defined( __LINUX__ ) || defined( __ANDROID__ )
#          define EXPORT_METHOD
#          define IMPORT_METHOD extern
#        else
/* Method to declare functions exported from a DLL. (nothign on
   LINUX or building statically, but __declspec(dllimport) on
   windows )                                                    */
#          define EXPORT_METHOD __declspec(dllexport)
/* method to define a function which will be Imported from a
   library. Under windows, this is probably
   __declspec(dllimport). Under linux this is probably 'extern'. */
#          define IMPORT_METHOD __declspec(dllimport)
#        endif
#        define LITERAL_LIB_EXPORT_METHOD __declspec(dllexport)
#        define LITERAL_LIB_IMPORT_METHOD __declspec(dllimport)
#      endif
#    endif
#  else
#  if ( !defined( __STATIC__ ) && defined( WIN32 ) && !defined( __cplusplus_cli) )
#    define EXPORT_METHOD __declspec(dllexport)
#    define IMPORT_METHOD __declspec(dllimport)
#    define LITERAL_LIB_EXPORT_METHOD __declspec(dllexport)
#    define LITERAL_LIB_IMPORT_METHOD __declspec(dllimport)
#  else
// MRT:  This is needed.  Need to see what may be defined wrong and fix it.
#    if defined( __LINUX__ ) || defined( __STATIC__ ) || defined( __ANDROID__ )
#      define EXPORT_METHOD
#      define IMPORT_METHOD extern
#      define LITERAL_LIB_EXPORT_METHOD
#      define LITERAL_LIB_IMPORT_METHOD extern
#    else
#      define EXPORT_METHOD __declspec(dllexport)
#      define IMPORT_METHOD __declspec(dllimport)
/* Define how methods in LITERAL_LIBRARIES are exported.
   literal_libraries are libraries that are used for plugins,
   and are dynamically loaded by code. They break the rules of
   system prefix and suffix extensions. LITERAL_LIBRARIES are
   always dynamic, and never static.                           */
#      define LITERAL_LIB_EXPORT_METHOD __declspec(dllexport)
/* Define how methods in LITERAL_LIBRARIES are imported.
   literal_libraries are libraries that are used for plugins,
   and are dynamically loaded by code. They break the rules of
   system prefix and suffix extensions. LITERAL_LIBRARIES are
   always dynamic, and never static.                           */
#      define LITERAL_LIB_IMPORT_METHOD __declspec(dllimport)
#    endif
#  endif
#endif
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/emscripten.h>
// Emscripten exports just need to be not optimized out.
#  undef  EXPORT_METHOD
#  define EXPORT_METHOD                EMSCRIPTEN_KEEPALIVE
#  undef  LITERAL_LIB_EXPORT_METHOD
#  define LITERAL_LIB_EXPORT_METHOD    EMSCRIPTEN_KEEPALIVE
#endif
// used when the keword specifying a structure is packed
// needs to prefix the struct keyword.
#define PREFIX_PACKED
// private thing left as a note, and forgotten.  some compilers did not define offsetof
#define my_offsetof( ppstruc, member ) ((uintptr_t)&((*ppstruc)->member)) - ((uintptr_t)(*ppstruc))
SACK_NAMESPACE
#ifdef BCC16
#define __inline__
#define MAINPROC(type,name)     type _pascal name
// winproc is intended for use at libmain/wep/winmain...
#define WINPROC(type,name)      type _far _pascal _export name
// callbackproc is for things like timers, dlgprocs, wndprocs...
#define CALLBACKPROC(type,name) type _far _pascal _export name
#define PUBLIC(type,name)       type STDPROC _export name
 /* here would be if dwReason == process_attach */
#define LIBMAIN() WINPROC(int, LibMain)(HINSTANCE hInstance, WORD wDataSeg, WORD wHeapSize, LPSTR lpCmdLine )		 { {
 /* end if */
 /*endproc*/
#define LIBEXIT() } }	    int STDPROC WEP(int nSystemExit )  {
#define LIBMAIN_END()  }
// should use this define for all local defines...
// this will allow one place to modify ALL _pascal or not to _pascal decls.
#define STDPROC _far _pascal
#endif
#if defined( __LCC__ ) || defined( _MSC_VER ) || defined(__DMC__) || defined( __WATCOMC__ )
#ifdef __WATCOMC__
#undef CPROC
#define CPROC __cdecl
#define STDPROC __cdecl
#ifndef __WATCOMC__
// watcom windef.h headers define this
#define STDCALL _stdcall
#endif
#if __WATCOMC__ >= 1280
// watcom windef.h headers no longer define this.
#define STDCALL __stdcall
#endif
#undef PREFIX_PACKED
#define PREFIX_PACKED _Packed
#else
#undef CPROC
//#error blah
#define CPROC __cdecl
#define STDPROC
#define STDCALL _stdcall
#endif
#define far
#define huge
#define near
#define _far
#define _huge
#define _near
/* portability type for porting legacy 16 bit applications. */
/* portability macro for legacy 16 bit applications. */
#define __far
#ifndef FAR
#define FAR
#endif
//#define HUGE
//#ifndef NEAR
//#define NEAR
//#endif
#define _fastcall
#ifdef __cplusplus
#ifdef __cplusplus_cli
#define PUBLIC(type,name) extern "C"  LITERAL_LIB_EXPORT_METHOD type CPROC name
#else
//#error what the hell!?
// okay Public functions are meant to be loaded with LoadFuncion( "library" , "function name"  );
#define PUBLIC(type,name) extern "C"  LITERAL_LIB_EXPORT_METHOD type CPROC name
#endif
#else
#define PUBLIC(type,name) LITERAL_LIB_EXPORT_METHOD type CPROC name
#endif
#define MAINPROC(type,name)  type WINAPI name
#define WINPROC(type,name)   type WINAPI name
#define CALLBACKPROC(type,name) type CALLBACK name
#if defined( __WATCOMC__ )
#define LIBMAIN()   static int __LibMain( HINSTANCE ); PRELOAD( LibraryInitializer ) {	 __LibMain( GetModuleHandle(TARGETNAME) );   }	 static int __LibMain( HINSTANCE hInstance ) {
#define LIBEXIT() } static int LibExit( void ); ATEXIT( LiraryUninitializer ) { LibExit(); } int LibExit(void) {
#define LIBMAIN_END() }
#else
#ifdef TARGETNAME
#define LIBMAIN()   static int __LibMain( HINSTANCE ); PRELOAD( LibraryInitializer ) {	 __LibMain( GetModuleHandle(TARGETNAME) );   }	 static int __LibMain( HINSTANCE hInstance ) {
#else
#define LIBMAIN()   TARGETNAME_NOT_DEFINED
#endif
#define LIBEXIT() } static int LibExit( void ); ATEXIT( LiraryUninitializer ) { LibExit(); } int LibExit(void) {
#define LIBMAIN_END() }
#endif
#define PACKED
#endif
#if defined( __GNUC__ )
#  ifndef STDPROC
#    define STDPROC
#  endif
#  ifndef STDCALL
 // for IsBadCodePtr which isn't a linux function...
#    define STDCALL
#  endif
#  ifndef WINAPI
#    ifdef __LINUX__
#       define WINAPI
#    else
#       define WINAPI __stdcall
#    endif
#  endif
#  ifndef PASCAL
//#define PASCAL
#  endif
#  define WINPROC(type,name)   type WINAPI name
#  define CALLBACKPROC( type, name ) type name
#  define PUBLIC(type,name) EXPORT_METHOD type CPROC name
#  define LIBMAIN()   static int __LibMain( HINSTANCE ); PRELOAD( LibraryInitializer ) {	 __LibMain( GetModuleHandle(TARGETNAME) );   }	 static int __LibMain( HINSTANCE hInstance ) {
#  define LIBEXIT() } static int LibExit( void ); ATEXIT( LiraryUninitializer ) { LibExit(); } int LibExit(void) {
#  define LIBMAIN_END()  }
/* Portability Macro for porting legacy code forward. */
#  define FAR
#  define NEAR
//#define HUGE
#  define far
#  define near
#  define huge
#  define PACKED __attribute__((packed))
#endif
#if defined( BCC32 )
#define far
#define huge
/* define obsolete keyword for porting purposes */
/* defined for porting from 16 bit environments */
#define near
/* portability macro for legacy 16 bit applications. */
#define _far
#define _huge
#define _near
/* portability type for porting to compilers that don't inline. */
/* portability macro for legacy 16 bit applications. */
#define __inline__
#define MAINPROC(type,name)     type _pascal name
// winproc is intended for use at libmain/wep/winmain...
#define WINPROC(type,name)      EXPORT_METHOD type _pascal name
// callbackproc is for things like timers, dlgprocs, wndprocs...
#define CALLBACKPROC(type,name) EXPORT_METHOD type _stdcall name
#define STDCALL _stdcall
#define PUBLIC(type,name)        type STDPROC name
#ifdef __STATIC__
			/*Log( "Library Enter" );*/
#define LIBMAIN() static WINPROC(int, LibMain)(HINSTANCE hInstance, DWORD dwReason, void *unused )		 { if( dwReason == DLL_PROCESS_ATTACH ) {
 /* end if */
#define LIBEXIT() } if( dwReason == DLL_PROCESS_DETACH ) {
#define LIBMAIN_END()  } return 1; }
#else
			/*Log( "Library Enter" );*/
#define LIBMAIN() WINPROC(int, LibMain)(HINSTANCE hInstance, DWORD dwReason, void *unused )		 { if( dwReason == DLL_PROCESS_ATTACH ) {
 /* end if */
#define LIBEXIT() } if( dwReason == DLL_PROCESS_DETACH ) {
#define LIBMAIN_END()  } return 1; }
#endif
// should use this define for all local defines...
// this will allow one place to modify ALL _pascal or not to _pascal decls.
#define STDPROC _pascal
#define PACKED
#endif
#define TOCHR(n) #n[0]
#define TOSTR(n) #n
#define STRSYM(n) TOSTR(n)
#define _WIDE__FILE__(n) n
#define WIDE__FILE__ _WIDE__FILE__(__FILE__)
/* a constant text string that represents the current source
   filename and line... fourmated as "source.c(11) :"        */
#define FILELINE  TEXT(__FILE__) "(" TEXT(STRSYM(__LINE__))" : ")
#if defined( _MSC_VER ) || defined( __PPCCPP__ )
/* try and define a way to emit comipler messages... but like no compilers support standard ways to do this accross the board.*/
#define pragnote(msg) message( FILELINE msg )
/* try and define a way to emit comipler messages... but like no compilers support standard ways to do this accross the board.*/
#define pragnoteonly(msg) message( msg )
#else
/* try and define a way to emit comipler messages... but like no compilers support standard ways to do this accross the board.*/
#define pragnote(msg) msg
/* try and define a way to emit comipler messages... but like no compilers support standard ways to do this accross the board.*/
#define pragnoteonly(msg) msg
#endif
/* specify a consistant macro to pass current file and line information.   This are appended parameters, and common usage is to only use these with _DEBUG set. */
#define FILELINE_SRC         , __FILE__, __LINE__
/* specify a consistant macro to pass current file and line information, to functions which void param lists.   This are appended parameters, and common usage is to only use these with _DEBUG set. */
#define FILELINE_VOIDSRC     __FILE__, __LINE__
//#define FILELINE_LEADSRC     __FILE__, __LINE__,
/* specify a consistant macro to define file and line parameters, to functions with otherwise void param lists.  This are appended parameters, and common usage is to only use these with _DEBUG set. */
#define FILELINE_VOIDPASS    CTEXTSTR pFile, uint32_t nLine
//#define FILELINE_LEADPASS    CTEXTSTR pFile, uint32_t nLine,
/* specify a consistant macro to define file and line parameters.   This are appended parameters, and common usage is to only use these with _DEBUG set. */
#define FILELINE_PASS        , CTEXTSTR pFile, uint32_t nLine
/* specify a consistant macro to forward file and line parameters.   This are appended parameters, and common usage is to only use these with _DEBUG set. */
#define FILELINE_RELAY       , pFile, nLine
/* specify a consistant macro to forward file and line parameters.   This are appended parameters, and common usage is to only use these with _DEBUG set. */
#define FILELINE_NULL        , NULL, 0
/* specify a consistant macro to forward file and line parameters, to functions which have void parameter lists without this information.  This are appended parameters, and common usage is to only use these with _DEBUG set. */
#define FILELINE_VOIDRELAY   pFile, nLine
/* specify a consistant macro to format file and line information for printf formated strings. */
#define FILELINE_FILELINEFMT "%s(%" _32f "): "
#define FILELINE_FILELINEFMT_MIN "%s(%" _32f ")"
#define FILELINE_NULL        , NULL, 0
#define FILELINE_VOIDNULL    NULL, 0
/* define static parameters which are the declaration's current file and line, for stubbing in where debugging is being stripped.
  usage
    FILELINE_VARSRC: // declare pFile and nLine variables.
	*/
#define FILELINE_VARSRC       CTEXTSTR pFile = __FILE__; uint32_t nLine = __LINE__
// this is for passing FILE, LINE information to allocate
// useful during DEBUG phases only...
// drop out these debug relay paramters for managed code...
// we're going to have the full call frame managed and known...
#if !defined( _DEBUG ) && !defined( _DEBUG_INFO )
#  if defined( __LINUX__ ) && !defined( __PPCCPP__ )
//#warning "Setting DBG_PASS and DBG_FORWARD to be ignored."
#  else
//#pragma pragnoteonly("Setting DBG_PASS and DBG_FORWARD to be ignored"  )
#  endif
#define DBG_AVAILABLE   0
/* in NDEBUG mode, pass nothing */
#define DBG_SRC
/* <combine sack::DBG_PASS>
   in NDEBUG mode, pass nothing */
#define DBG_VOIDSRC
/* <combine sack::DBG_PASS>
   \#define DBG_LEADSRC in NDEBUG mode, declare (void) */
/* <combine sack::DBG_PASS>
   \ \                      */
#define DBG_VOIDPASS    void
/* <combine sack::DBG_PASS>
   in NDEBUG mode, pass nothing */
#define DBG_PASS
/* <combine sack::DBG_PASS>
   in NDEBUG mode, pass nothing */
#define DBG_RELAY
/* <combine sack::DBG_PASS>
   in _DEBUG mode, pass FILELINE_NULL */
#define DBG_NULL
/* <combine sack::DBG_PASS>
   in NDEBUG mode, pass nothing */
#define DBG_VOIDRELAY
/* <combine sack::DBG_PASS>
   in NDEBUG mode, pass nothing */
#define DBG_FILELINEFMT
/* <combine sack::DBG_PASS>
   in NDEBUG mode, pass nothing */
#define DBG_FILELINEFMT_MIN
/* <combine sack::DBG_PASS>
   in NDEBUG mode, pass nothing
   Example
   printf( DBG_FILELINEFMT ": extra message" DBG_PASS ); */
#define DBG_VARSRC
#else
	// these DBG_ formats are commented out from duplication in sharemem.h
#  if defined( __LINUX__ ) && !defined( __PPCCPP__ )
//#warning "Setting DBG_PASS and DBG_FORWARD to work."
#  else
//#pragma pragnoteonly("Setting DBG_PASS and DBG_FORWARD to work"  )
#  endif
// used to specify whether debug information is being passed - can be referenced in compiled code
#define DBG_AVAILABLE   1
/* <combine sack::DBG_PASS>
   in _DEBUG mode, pass FILELINE_SRC */
#define DBG_SRC         FILELINE_SRC
/* <combine sack::DBG_PASS>
   in _DEBUG mode, pass FILELINE_VOIDSRC */
#define DBG_VOIDSRC     FILELINE_VOIDSRC
/* <combine sack::DBG_PASS>
   in _DEBUG mode, pass FILELINE_VOIDPASS */
#define DBG_VOIDPASS    FILELINE_VOIDPASS
/* <combine sack::DBG_PASS>
   in NDEBUG mode, pass nothing */
/* Example
   This example shows forwarding debug information through a
   chain of routines.
   <code lang="c++">
   void ReportFunction( int sum DBG_PASS )
   {
       printf( "%s(%d):started this whole mess\\n" DBG_RELAY );
   }
   void TrackingFunction( int a, int b DBG_PASS )
   {
       ReportFunction( a+b, DBG_RELAY );
   }
   void CallTrack( void )
   {
       TrackingFunction( 1, 2 DBG_SRC );
   }
   </code>
   In this example, the debug information is passed to the
   logging system. This allows logging to blame the user
   application for allocations, releases, locks, etc...
   <code lang="c++">
   void MyAlloc( int size DBG_PASS )
   {
       _lprintf( DBG_RELAY )( ": alloc %d\\n", size );
   }
   void g( void )
   {
       lprintf( "Will Allocate %d\\n", 32 );
       MyAlloc( 32 DBG_SRC );
   }
   </code>
   This example uses the void argument macros
   <code>
   void SimpleFunction( DBG_VOIDPASS )
   {
       // this function usually has (void) parameters.
   }
   void f( void )
   {
       SimpleFunction( DBG_VOIDSRC );
   }
   </code>
   Description
   in NDEBUG mode, pass nothing.
   This function allows specification of DBG_RELAY or DBG_SRC
   under debug compilation. Otherwise, the simple AddLink macro
   should be used. DBG_RELAY can be used to forward file and
   line information which has been passed via DBG_PASS
   declaration in the function parameters.
   This is a part of a set of macros which allow additional
   logging information to be passed.
   These 3 are the most commonly used.
   DBG_SRC - this passes the current __FILE__, __LINE__
   \parameters.
   DBG_PASS - this is used on a function declaration, is a
   filename and line number from DBG_SRC or DBG_RELAY.
   DBG_RELAY - this passes the file and line passed to this
   function to another function with DBG_PASS defined on it.
   DBG_VOIDPASS - used when the argument list is ( void )
   without debugging information.
   DBG_VOIDSRC - used to call a function who's argument list is
   ( void ) without debugging information.
   DBG_VOIDRELAY - pass file and line information forward to
   another function, who's argument list is ( void ) without
   debugging information.
   Remarks
   The SACK library is highly instrumented with this sort of
   information. Very commonly the only difference between a
   specific function called 'MyFunctionName' and
   'MyFunctionNameEx' is the addition of debug information
   tracking.
   The following code blocks show the evolution added to add
   instrumentation...
   <code lang="c++">
   int MyFunction( int param )
   {
       // do stuff
   }
   int CallingFunction( void )
   {
       return MyFunction();
   }
   </code>
   Pretty simple code, a function that takes a parameter, and a
   function that calls it.
   The first thing is to extend the called function.
   <code>
   int MyFunctionEx( int param DBG_PASS )
   {
       // do stuff
   }
   </code>
   And provide a macro for everyone else calling the function to
   automatically pass their file and line information
   <code lang="c++">
   \#define MyFunction(param)  MyFunctionEx(param DBG_SRC)
   </code>
   Then all-together
   <code>
   \#define MyFunction(param)  MyFunctionEx(param DBG_SRC)
   int MyFunctionEx( int param DBG_PASS )
   {
       // do stuff
   }
   int CallingFunction( void )
   {
       // and this person calling doesn't matter
       // does require a recompile of source.
       return MyFunction( 3 );
   }
   </code>
   But then... what if CallingFunction decided wasn't really the
   one at fault, or responsible for the allocation, or other
   issue being tracked, then she could be extended....
   <code>
   int CallingFunctionEx( DBG_VOIDPASS )
   \#define CallingFunction() CallingFunction( DBG_VOIDSRC )
   {
       // and this person calling doesn't matter
       // does require a recompile of source.
       return MyFunction( 1 DBG_RELAY );
   }
   </code>
   Now, calling function will pass it's callers information to
   MyFunction....
   Why?
   Now, when you call CreateList, your code callng the list
   creation method is marked as the one who allocates the space.
   Or on a DeleteList, rather than some internal library code
   being blamed, the actual culprit can be tracked and
   identified, because it's surely not the fault of CreateList
   that the reference to the memory for the list wasn't managed
   correctly.
   Note
   It is important to note, every usage of these macros does not
   have a ',' before them. This allows non-debug code to
   eliminate these extra parameters cleanly. If the ',' was
   outside of the macro, then it would remain on the line, and
   an extra parameter would have be be passed that was unused.
   This is also why DBG_VOIDPASS exists, because in release mode
   this is substituted with 'void'.
   In Release mode, DBG_VOIDRELAY becomes nothing, but when in
   debug mode, DBG_RELAY has a ',' in the macro, so without a
   paramter f( DBG_RELAY ) would fail; on expansion this would
   be f( , pFile, nLine ); (note the extra comma, with no
   parameter would be a syntax error.                            */
#define DBG_PASS        FILELINE_PASS
/* <combine sack::DBG_PASS>
   in _DEBUG mode, pass FILELINE_RELAY */
#define DBG_RELAY       FILELINE_RELAY
/* <combine sack::DBG_PASS>
	  in _DEBUG mode, pass FILELINE_NULL */
#define DBG_NULL        FILELINE_NULL
/* <combine sack::DBG_PASS>
   in _DEBUG mode, pass FILELINE_VOIDRELAY */
#define DBG_VOIDRELAY   FILELINE_VOIDRELAY
/* <combine sack::DBG_PASS>
   in _DEBUG mode, pass FILELINE_FILELINEFMT */
#define DBG_FILELINEFMT FILELINE_FILELINEFMT
/* <combine sack::DBG_PASS>
   in _DEBUG mode, pass FILELINE_FILELINEFMT_MIN */
#define DBG_FILELINEFMT_MIN FILELINE_FILELINEFMT_MIN
/* <combine sack::DBG_PASS>
   in _DEBUG mode, pass FILELINE_VARSRC */
#define DBG_VARSRC      FILELINE_VARSRC
#endif
// cannot declare _0 since that overloads the
// vector library definition for origin (0,0,0,0,...)
//typedef void             _0; // totally unusable to declare 0 size things.
/* the only type other than when used in a function declaration that void is valid is as a pointer to void. no _0 type exists
	 (it does, but it's in vectlib, and is an origin vector)*/
typedef void             *P_0;
/*
 * several compilers are rather picky about the types of data
 * used for bit field declaration, therefore this type
 * should be used instead of uint32_t (DWORD)
 */
typedef unsigned int  BIT_FIELD;
/*
 * several compilers are rather picky about the types of data
 * used for bit field declaration, therefore this type
 * should be used instead of int32_t (LONG)
 */
typedef int  SBIT_FIELD;
// have to do this on a per structure basis - otherwise
// any included headers with structures to use will get
// padded as normal; this is appended to a strcture
// and is ued on GCC comiplers for __attribute__((packed))
#ifndef PACKED
#  define PACKED
#endif
/* An pointer to a volatile unsigned integer type that is 64 bits long. */
//typedef volatile uint64_t  *volatile int64_t*;
/* An pointer to a volatile pointer size type that is as long as a pointer. */
typedef volatile uintptr_t        *PVPTRSZVAL;
/* an unsigned type meant to index arrays.  (By convention, arrays are not indexed negatively.)  An index which is not valid is INVALID_INDEX, which equates to 0xFFFFFFFFUL or negative one cast as an INDEX... ((INDEX)-1). */
typedef size_t         INDEX;
/* An index which is not valid; equates to 0xFFFFFFFFUL or negative one cast as an INDEX... ((INDEX)-1). */
#define INVALID_INDEX ((INDEX)-1)
// constant text string content
typedef const char     *CTEXTSTR;
/* A non constant array of TEXTCHAR. A pointer to TEXTCHAR. A
   pointer to non-constant characters. (A non-static string
   probably)                                                  */
typedef char           *TEXTSTR;
#if defined( __LINUX__ ) && defined( __cplusplus )
// pointer to constant text string content
typedef TEXTSTR const  *PCTEXTSTR;
#else
// char const *const *
typedef CTEXTSTR const *PCTEXTSTR;
#endif
/* a text 8 bit character  */
typedef char            TEXTCHAR;
/* a character rune.  Strings should be interpreted as UTF-8 or 16 depending on UNICODE compile option.
   GetUtfChar() from strings.  */
typedef uint32_t             TEXTRUNE;
/* Used to handle returned values that are invalid runes; past end or beginning of string for instance */
#define INVALID_RUNE  0x80000000
//typedef enum { FALSE, TRUE } LOGICAL; // smallest information
#ifndef FALSE
#define FALSE 0
/* Define TRUE when not previously defined in the platform. TRUE
   is (!FALSE) so anything not 0 is true.                        */
#define TRUE (!FALSE)
#endif
/* Meant to hold boolean and only boolean values. Should be
   implemented per-platform as appropriate for the bool type the
   compiler provides.                                            */
typedef uint32_t LOGICAL;
/* This is a pointer. It is a void*. It is meant to point to a
   single thing, and cannot be used to reference arrays of bytes
   without recasting.                                            */
typedef P_0 POINTER;
/* This is a pointer to constant data. void const *. Compatible
   with things like char const *.                               */
typedef const void *CPOINTER;
SACK_NAMESPACE_END
//------------------------------------------------------
// formatting macro defintions for [vsf]printf output of the above types
#if !defined( _MSC_VER ) || ( _MSC_VER >= 1900 )
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#endif
SACK_NAMESPACE
/* 16 bit unsigned decimal output printf format specifier. This would
   otherwise be defined in \<inttypes.h\>                */
#define _16f   "u"
/* 16 bit hex output printf format specifier. This would
   otherwise be defined in \<inttypes.h\>                */
#define _16fx   "x"
/* 16 bit HEX output printf format specifier. This would
   otherwise be defined in \<inttypes.h\>                */
#define _16fX   "X"
/* 16 bit signed decimal output printf format specifier. This
   would otherwise be defined in \<inttypes.h\>               */
#define _16fs   "d"
/* 8 bit unsigned decimal output printf format specifier. This would
   otherwise be defined in \<inttypes.h\>                */
#define _8f   "u"
/* 8 bit hex output printf format specifier. This would
   otherwise be defined in \<inttypes.h\>                */
#define _8fx   "x"
/* 8 bit HEX output printf format specifier. This would
   otherwise be defined in \<inttypes.h\>                */
#define _8fX   "X"
/* 8 bit signed decimal output printf format specifier. This
   would otherwise be defined in \<inttypes.h\>               */
#define _8fs   "d"
#if defined( __STDC_FORMAT_MACROS )
#  define _32f   PRIu32
#  define _32fx    PRIx32
#  define _32fX    PRIX32
#  define _32fs    PRId32
#  define _64f    PRIu64
#  define _64fx   PRIx64
#  define _64fX   PRIX64
#  define _64fs   PRId64
#  define _64f    PRIu64
#  define _64fx   PRIx64
#  define _64fX   PRIX64
#  define _64fs   PRId64
// non-unicode strings
#  define c_32f    PRIu32
#  define c_32fx   PRIx32
#  define c_32fX   PRIX32
#  define c_32fs   PRId32
#  define c_64f    PRIu64
#  define c_64fx   PRIx64
#  define c_64fX   PRIX64
#  define c_64fs   PRId64
#else
#  define _32f   "u"
#  define _32fx   "x"
#  define _32fX   "X"
#  define _32fs   "d"
#  define c_32f   "u"
#  define c_32fx  "x"
#  define c_32fX  "X"
#  define c_32fs  "d"
#  define c_64f    "llu"
#  define c_64fx   "llx"
#  define c_64fX   "llX"
#  define c_64fs   "lld"
#endif
#  define _cstring_f "s"
#  define _string_f "s"
#  define _ustring_f "S"
#if defined( __64__ )
#  if defined( __STDC_FORMAT_MACROS )
#    if !defined( __GNUC__ ) || defined( _WIN32 )
#      define _size_f     PRIu64
#      define _size_fx    PRIx64
#      define _size_fX    PRIX64
#      define _size_fs    PRId64
#      define c_size_f    PRIu64
#      define c_size_fx   PRIx64
#      define c_size_fX   PRIX64
#      define c_size_fs   PRId64
#    else
#      define _size_f    "zu"
#      define _size_fx   "zx"
#      define _size_fX   "zX"
#      define _size_fs   "zd"
#      define c_size_f    "zu"
#      define c_size_fx   "zx"
#      define c_size_fX   "zX"
#      define c_size_fs   "zd"
#    endif
#    define _PTRSZVALfs  PRIuPTR
#    define _PTRSZVALfx  PRIxPTR
#    define cPTRSZVALfs PRIuPTR
#    define cPTRSZVALfx PRIxPTR
#  else
#    if !defined( __GNUC__ ) || defined( _WIN32 )
#      define _size_f    _64f
#      define _size_fx   _64fx
#      define _size_fX   _64fX
#      define _size_fs   _64fs
#      define c_size_f   c_64f
#      define c_size_fx  c_64fx
#      define c_size_fX  c_64fX
#      define c_size_fs  c_64fs
#    else
#      define _size_f    "zu"
#      define _size_fx   "zx"
#      define _size_fX   "zX"
#      define _size_fs   "zd"
#      define c_size_f    "zu"
#      define c_size_fx   "zx"
#      define c_size_fX   "zX"
#      define c_size_fs   "zd"
#    endif
#    define _PTRSZVALfs  PRIuPTR
#    define _PTRSZVALfx  PRIxPTR
#    define cPTRSZVALfs PRIuPTR
#    define cPTRSZVALfx PRIxPTR
#  endif
#else
#  if defined( __STDC_FORMAT_MACROS )
      // this HAS been fixed in UCRT - 2015!  but it'll take 5 years before everyone has that...
#    if !defined( __GNUC__ ) || defined( _WIN32 )
#      define _size_f     PRIu32
#      define _size_fx    PRIx32
#      define _size_fX    PRIX32
#      define _size_fs    PRId32
#      define c_size_f    PRIu32
#      define c_size_fx   PRIx32
#      define c_size_fX   PRIX32
#      define c_size_fs   PRId32
#    else
#      define _size_f    "zu"
#      define _size_fx   "zx"
#      define _size_fX   "zX"
#      define _size_fs   "zd"
#      define c_size_f    "zu"
#      define c_size_fx   "zx"
#      define c_size_fX   "zX"
#      define c_size_fs   "zd"
#    endif
#    define _PTRSZVALfs  PRIuPTR
#    define _PTRSZVALfx  PRIxPTR
#    define cPTRSZVALfs PRIuPTR
#    define cPTRSZVALfx PRIxPTR
#  else
      // this HAS been fixed in UCRT - 2015!  but it'll take 5 years before everyone has that...
#    if !defined( __GNUC__ ) || defined( _WIN32 )
#      define _size_f    _32f
#      define _size_fx   _32fx
#      define _size_fX   _32fX
#      define _size_fs   _32fs
#      define c_size_f    c_32f
#      define c_size_fx   c_32fx
#      define c_size_fX   c_32fX
#      define c_size_fs   c_32fs
#    else
#      define _size_f    "zu"
#      define _size_fx   "zx"
#      define _size_fX   "zX"
#      define _size_fs   "zd"
#      define c_size_f    "zu"
#      define c_size_fx   "zx"
#      define c_size_fX   "zX"
#      define c_size_fs   "zd"
#    endif
#    define _PTRSZVALfs  PRIuPTR
#    define _PTRSZVALfx  PRIxPTR
#    define cPTRSZVALfs PRIuPTR
#    define cPTRSZVALfx PRIxPTR
#  endif
#endif
#define PTRSZVALf "p"
#define _PTRSZVALf "p"
#if defined( _MSC_VER ) && ( _MSC_VER < 1900 )
/* 64 bit unsigned decimal output printf format specifier. This would
   otherwise be defined in \<inttypes.h\> as PRIu64              */
#define _64f    "llu"
/* 64 bit hex output printf format specifier. This would
   otherwise be defined in \<inttypes.h\> as PRIxFAST64                */
#define _64fx   "llx"
/* 64 bit HEX output printf format specifier. This would
   otherwise be defined in \<inttypes.h\> as PRIxFAST64                */
#define _64fX   "llX"
/* 64 bit signed decimal output printf format specifier. This
   would otherwise be defined in \<inttypes.h\> as PRIdFAST64               */
#define _64fs   "lld"
#endif
// This should be for several years a
// sufficiently large type to represent
// threads and processes.
typedef uint64_t THREAD_ID;
#define GetMyThreadIDNL GetMyThreadID
#if defined( _WIN32 )
#  define _GetMyThreadID()  ( (( ((uint64_t)GetCurrentProcessId()) << 32 ) | ( (uint64_t)GetCurrentThreadId() ) ) )
#  define GetMyThreadID()  (GetThisThreadID())
#else
// this is now always the case
// it's a safer solution anyhow...
#  ifdef __MAC__
#    define GetMyThreadID()  (( ((uint64_t)getpid()) << 32 ) | ( (uint64_t)( syscall(SYS_thread_selfid) ) ) )
#  else
#    ifndef GETPID_RETURNS_PPID
#      define GETPID_RETURNS_PPID
#    endif
#    ifdef GETPID_RETURNS_PPID
#      ifdef __ANDROID__
#        define GetMyThreadID()  (( ((uint64_t)getpid()) << 32 ) | ( (uint64_t)(gettid()) ) )
#      else
#        if defined( __EMSCRIPTEN__ )
#          define GetMyThreadID()  ( (uint64_t)(pthread_self()) )
#        else
#          define GetMyThreadID()  (( ((uint64_t)getpid()) << 32 ) | ( (uint64_t)(syscall(SYS_gettid)) ) )
#        endif
#      endif
#    else
#      define GetMyThreadID()  (( ((uint64_t)getppid()) << 32 ) | ( (uint64_t)(getpid()|0x40000000)) )
#    endif
#  endif
#  define _GetMyThreadID GetMyThreadID
#endif
//---------------------- Declare Link; 'single and a half'ly-linked lists -----------------------
// Thse macros are for linking and unlininking things in a linked list.
// The list is basically a singly-linked list, but also references the pointer that
// is pointing at the current node.  This simplifies insert/remove operations, because
// the specific list that the node is in, is not required.
// List heads will always be updated correctly.
//
// A few 'tricks' are available, such as
//     0) These are deemed dangerous; and uncomprehendable by anyone but the maintainer.
//        use at your own time and expense required to explain WHY these work.
//     1) when declaring a root node, include another node before it, and it's
//        simple to make this a circularly linked list.
//     2) defining DeclareLink at the start of the strcture, the 'me' pointer
//        also happens to be 'prior', so you can step through the list in both
//        directions.
//
//
//
// struct my_node {
//    DeclareLink( struct my_node );
//    // ...
// };
//
// that declares
//      struct my_node *next;  // the next node in list.
//      struct my_node **me;   // address of the pointer pointing to 'me';
//
//
//  struct my_node *root; // a root of a list of my_node.  It should be initialized to NULL.
//
//  struct my_node *newNode = (struct my_node*)malloc( sizeof( *newNode ) );
//     // does not require next or me to be initiialized.
//  LinkThing( root, newNode );
//     // now newNode is in the list.
//
//  to remove from a list
//
//  struct my_node *someNode; // this should be a pointer to some valid node.
//  UnlinkThing( someNode );
//     The new node is now not in the list.
//
//  To move one node from one list to another
//
//   struct my_node *rootAvail;  // available nodes
//   struct my_node *rootUsed;   // nodes in use
//
//   struct my_node *someNode; // some node in a list
//   someNode = rootAvail; // get first available.
//   if( !someNode ) ; // create a new one or abort
//   RelinkThing( rootUsed, someNode );
//      'someNode' is removed from its existing list, and added to the 'rootUsed' list.
//
// For Declaring the link structure members for lists
#define DeclareLink( type )  type *next; type **me
/* Link a new node into the list.
   Example
   struct mynode
   {
       DeclareLink( struct mynode );
   } *node;
	struct mynode *list;
   // node allocation not shown.
	LinkThing( list_root, node );
*/
#define LinkThing( root, node )		     ((( (node)->next = (root) )?	        (((root)->me) = &((node)->next)):0),	  (((node)->me) = &(root)),	             ((root) = (node)) )
/* Link a node to the end of a list. LinkThing() inserts the new
 node as the new head of the list.
 this has to scan the list to find the end, so it is a O(n) operation.
 All other linked list operations are O(1)
 */
#define LinkLast( root, type, node ) if( node ) do { if( !root )	 { root = node; (node)->me=&root; }	 else { type tmp;	 for( tmp = root; tmp->next; tmp = tmp->next );	 tmp->next = (node);	 (node)->me = &tmp->next;	 } } while (0)
// put 'Thing' after 'node'
// inserts 'node' after Thing
#define LinkThingAfter( node, thing )	 ( ( (thing)&&(node))	   ?(((((thing)->next = (node)->next))?((node)->next->me = &(thing)->next):0)	  ,((thing)->me = &(node)->next), ((node)->next = thing))	  :((node)=(thing)) )
//
// put 'Thing' before 'node'... so (*node->me) = thing
// similar to LinkThingAfter but puts the new 'thing'
// before the 'node' specified.
#define LinkThingBefore( node, thing )	 {  thing->next = (*node->me);	(*node->me) = thing;    thing->me = node->me;       node->me = &thing->next;     }
// move a list from one list to another.
// unlinks node from where it was, inserts at the head of another.
// this can also be use to reproiritize within the same list.
#define RelinkThing( root, node )	   ((( node->me && ( (*node->me)=node->next ) )?	  node->next->me = node->me:0),(node->next = NULL),(node->me = NULL),node),	 ((( node->next = root )?	        (root->me = &node->next):0),	  (node->me = &root),	             (root = node) )
/* Remove a node from a list. Requires only the node. */
#define UnlinkThing( node )	                      ((( (node) && (node)->me && ( (*(node)->me)=(node)->next ) )?	  (node)->next->me = (node)->me:0),((node)->next = NULL),((node)->me = NULL),(node))
// this has two expressions duplicated...
// but in being so safe in this expression,
// the self-circular link needs to be duplicated.
// GrabThing is used for nodes which are circularly bound
#define GrabThing( node )	    ((node)?(((node)->me)?(((*(node)->me)=(node)->next)?	 ((node)->next->me=(node)->me),((node)->me=&(node)->next):NULL):((node)->me=&(node)->next)):NULL)
/* Go to the next node with links declared by DeclareLink
 safe iterator macro that tests if node is valid, which returns
 the next item in the list, else returns NULL
 */
#define NextLink(node) ((node)?(node)->next:NULL)
// everything else is called a thing... should probably migrate to using this...
#define NextThing(node) ((node)?(node)->next:NULL)
//----------- FLAG SETS (single bit fields) -----------------
/* the default type to use for flag sets - flag sets are arrays of bits
 which can be set/read with/as integer values an index.
 All of the fields in a maskset are the same width */
#define FLAGSETTYPE uintmax_t
/* the number of bits a specific type is.
   Example
   int bit_size_int = FLAGTYPEBITS( int ); */
#define FLAGTYPEBITS(t) (sizeof(t)*CHAR_BIT)
/* how many bits to add to make sure we round to the next greater index if even 1 bit overflows */
#define FLAGROUND(t) (FLAGTYPEBITS(t)-1)
/* the index of the FLAGSETTYPE which contains the bit in question */
#define FLAGTYPE_INDEX(t,n)  (((n)+FLAGROUND(t))/FLAGTYPEBITS(t))
/* how big the flag set is in count of FLAGSETTYPEs required in a row ( size of the array of FLAGSETTYPE that contains n bits) */
#define FLAGSETSIZE(t,n) (FLAGTYPE_INDEX(t,n) * sizeof( FLAGSETTYPE ) )
// declare a set of flags...
#define FLAGSET(v,n)   FLAGSETTYPE (v)[((n)+FLAGROUND(FLAGSETTYPE))/FLAGTYPEBITS(FLAGSETTYPE)]
// set a single flag index
#define SETFLAG(v,n)   ( ( (v)[(n)/FLAGTYPEBITS((v)[0])] |= (FLAGSETTYPE)1 << ( (n) & FLAGROUND((v)[0]) )),1)
// clear a single flag index
#define RESETFLAG(v,n) ( ( (v)[(n)/FLAGTYPEBITS((v)[0])] &= ~( (FLAGSETTYPE)1 << ( (n) & FLAGROUND((v)[0]) ) ) ),0)
// test if a flags is set
//  result is 0 or not; the value returned is the bit shifted within the word, and not always '1'
#define TESTFLAG(v,n)  ( (v)[(n)/FLAGTYPEBITS((v)[0])] & ( (FLAGSETTYPE)1 << ( (n) & FLAGROUND((v)[0]) ) ) )
// reverse a flag from 1 to 0 and vice versa
// return value is undefined... and is a whole bunch of flags from some offset...
// if you want ot toggle and flag and test the result, use TESTGOGGLEFLAG() instead.
#define TOGGLEFLAG(v,n)   ( (v)[(n)/FLAGTYPEBITS((v)[0])] ^= (FLAGSETTYPE)1 << ( (n) & FLAGROUND((v)[0]) ))
// Toggle a bit, return the state of the bit after toggling.
#define TESTTOGGLEFLAG(v,n)  ( TOGGLEFLAG(v,n), TESTFLAG(v,n) )
//----------- MASK SETS -----------------
//  MASK Sets are arrays of bit-fields of some bit-width (5, 3, ... )
//  they are set/returned as integer values.
//  They are stored-in/accessed via a uint8_t which gives byte-offset calculations.
// they return their value as uintmax_t from the offset memory address directly;
//   Some platforms(Arm) may SIGBUS because of wide offset accesses spanning word boundaries.
//   This issue may be fixed by rounding, grabbing the word aligned values and shifting manually
// Declarataion/Instantiation of a mask set is done with MASKSET macro below
// 32 bits max for range on mask
#define MASK_MAX_LENGTH (sizeof(MASKSET_READTYPE)*CHAR_BIT)
/* gives a 32 bit mask possible from flagset..
 - updated; return max int possible; but only the low N bits will be set
 - mask sets are meant for small values, but could be used for like 21 bit fields. (another form of unicode encoding I suppose)
 */
#define MASKSET_READTYPE uintmax_t
// gives byte index...
#define MASKSETTYPE uint8_t
/* how many bits the type specified can hold
   Parameters
   t :  data type to measure (int, uint32_t, ... ) */
#define MASKTYPEBITS(t) (sizeof(t)*CHAR_BIT)
/* the maximum number of bits storable in a type */
#define MASK_MAX_TYPEBITS(t) (sizeof(t)*CHAR_BIT)
/* round up to the next count of types that fits 1 bit - used as a cieling round factor */
#define MASKROUND(t) (MASKTYPEBITS(t)-1)
/* define MAX_MAX_ROUND factor based on MASKSET_READTYPE - how to read it... */
#define MASK_MAX_ROUND() (MASK_MAX_TYPEBITS(MASKSET_READTYPE)-1)
/* byte index of the start of the mask
   Parameters
   t :  type to measure with
   n :  mask index                     */
#define MASKTYPE_INDEX(t,n)  (((n)+MASKROUND(t))/MASKTYPEBITS(t))
/* The number of bytes the set would be.
   Parameters
   t :  the given type to measure with
   n :  the count of masks to fit.       */
#define MASKSETSIZE(t,n) (MASKTYPE_INDEX(t,(n+1)))
// declare a set of flags...
#define MASK_TOP_MASK_VAL(length,val) ((val)&( ((MASKSET_READTYPE)-1) >> ((sizeof(MASKSET_READTYPE) * CHAR_BIT)-(length)) ))
/* the mask in the dword resulting from shift-right.   (gets a mask of X bits in length) */
#define MASK_TOP_MASK(length) ( ((MASKSET_READTYPE)-1) >> ((sizeof(MASKSET_READTYPE) * CHAR_BIT)-(length)) )
/* the mast in the dword shifted to the left to overlap the field in the word */
#define MASK_MASK(n,length)   (MASK_TOP_MASK(length) << (((n)*(length)) & (sizeof(MASKSET_READTYPE) - 1) ) )
// masks value with the mask size, then applies that mask back to the correct word indexing
#define MASK_MASK_VAL(n,length,val)   (MASK_TOP_MASK_VAL(length,val) << (((n)*(length))&0x7) )
/* declare a mask set.
 MASKSET( maskVariableName
        , 32 //number of items
		  , 5 // number of bits per field
		  );
   declares
	uint8_t maskVariableName[ (32*5 +(CHAR_BIT-1))/CHAR_BIT ];  //data array used for storage.
   const int askVariableName_mask_size = 5;  // used aautomatically by macros
*/
#define MASKSET(v,n,r)  MASKSETTYPE  (v)[(((n)*(r))+MASK_MAX_ROUND())/MASKTYPEBITS(MASKSETTYPE)]; const int v##_mask_size = r;
/* set a field index to a value
    SETMASK( askVariableName, 3, 13 );  // set set member 3 to the value '13'
 */
#define SETMASK(v,n,val)    (((MASKSET_READTYPE*)((v)+((n)*(v##_mask_size))/MASKTYPEBITS((v)[0])))[0] =    ( ((MASKSET_READTYPE*)((v)+((n)*(v##_mask_size))/MASKTYPEBITS(uint8_t)))[0]                                  & (~(MASK_MASK(n,v##_mask_size))) )	                                                                           | MASK_MASK_VAL(n,v##_mask_size,val) )
/* get the value of a field
     GETMASK( maskVariableName, 3 );   // returns '13' given the SETMASK() example code.
 */
#define GETMASK(v,n)  ( ( ((MASKSET_READTYPE*)((v)+((n)*(v##_mask_size))/MASKTYPEBITS((v)[0])))[0]         & MASK_MASK(n,v##_mask_size) )	                                                                           >> (((n)*(v##_mask_size))&0x7))
/* This type stores data, it has a self-contained length in
   bytes of the data stored.  Length is in characters       */
_CONTAINER_NAMESPACE
/* LIST is a slab array of pointers, each pointer may be
   assigned to point to any user data.
   Remarks
   When the list is filled to the capacity of Cnt elements, the
   list is reallocated to be larger.
   Cannot add NULL pointer to list, empty elements in the list
   are represented with NULL, and may be filled by any non-NULL
   value.                                                       */
_LINKLIST_NAMESPACE
/* <combine sack::containers::list::LinkBlock>
   \ \                                         */
typedef struct LinkBlock
{
	/* How many pointers the list can contain now. */
	INDEX     Cnt;
	/* \ \  */
	POINTER pNode[1];
} LIST, *PLIST;
_LINKLIST_NAMESPACE_END
#ifdef __cplusplus
using namespace sack::containers::list;
#endif
_DATALIST_NAMESPACE
/* a list of data structures... a slab array of N members of X size */
typedef struct DataBlock  DATALIST;
/* A typedef of a pointer to a DATALIST struct DataList. */
typedef struct DataBlock *PDATALIST;
/* Data Blocks are like LinkBlocks, and store blocks of data in
   slab format. If the count of elements exceeds available, the
   structure is grown, to always contain a continuous array of
   structures of Size size.
   Remarks
   When blocks are deleted, all subsequent blocks are shifted
   down in the array. So the free blocks are always at the end. */
struct DataBlock
{
	/* How many elements are used. */
	INDEX     Cnt;
	/* How many elements are available in his array. */
	INDEX     Avail;
	/* A simple exchange lock on the data for insert and delete. For
	   thread safety.                                                */
	//volatile uint32_t     Lock;
	/* How big each element of the array is. */
	INDEX     Size;
	/* The physical array. */
	uint8_t      data[1];
};
_DATALIST_NAMESPACE_END
/* This is a stack that contains pointers to user objects.
   Remarks
   This is a stack 'by reference'. When extended, the stack will
   occupy different memory, care must be taken to not duplicate
   pointers to this stack.                                       */
typedef struct LinkStack
{
	/* This is the index of the next pointer to be pushed or popped.
	   If top == 0, the stack is empty, until a pointer is added and
	   top is incremented.                                           */
	INDEX     Top;
	/* How many pointers the stack can contain. */
	INDEX     Cnt;
	/* thread interlock using InterlockedExchange semaphore. For
	                  thread safety.                                            */
	//volatile uint32_t     Lock;
	/*  a defined maximum capacity of stacked values... values beyond this are lost from the bottom  */
	uint32_t     Max;
	/* Reserved data portion that stores the pointers. */
	POINTER pNode[1];
} LINKSTACK, *PLINKSTACK;
/* A Stack that stores information in an array of structures of
   known size.
   Remarks
   The size of each element must be known at stack creation
   time. Structures are literally copied to and from this stack.
   This is a stack 'by value'. When extended, the stack will
   occupy different memory, care must be taken to not duplicate
   pointers to this stack.                                       */
typedef struct DataListStack
{
	volatile INDEX     Top;
 /* enable logging the program executable (probably the same for
	                all messages, unless they are network)
	                                                                             */
 // How many elements are on the stack.
	INDEX     Cnt;
	//volatile uint32_t     Lock;  /* thread interlock using InterlockedExchange semaphore. For
	//                  thread safety.                                            */
	INDEX     Size;
	INDEX     Max;
	uint8_t      data[1];
} DATASTACK, *PDATASTACK;
/* A queue which contains pointers to user objects. If the queue
   is filled to capacity and new queue is allocated, and all
   existing pointers are transferred.                            */
typedef struct LinkQueue
{
	/* This is the index of the next pointer to be added to the
	   queue. If Top==Bottom, then the queue is empty, until a
	   pointer is added to the queue, and Top is incremented.   */
	volatile INDEX     Top;
	/* This is the index of the next element to leave the queue. */
	volatile INDEX     Bottom;
	/* This is the current count of pointers that can be stored in
	   the queue.                                                  */
	INDEX     Cnt;
	/* thread interlock using InterlockedExchange semaphore. For
	   thread safety.                                            */
#if USE_CUSTOM_ALLOCER
	volatile uint32_t     Lock;
#endif
 // need two to have distinct empty/full conditions
	POINTER pNode[2];
} LINKQUEUE, *PLINKQUEUE;
/* A queue of structure elements.
   Remarks
   The size of each element must be known at stack creation
   time. Structures are literally copied to and from this stack.
   This is a stack 'by value'. When extended, the stack will
   occupy different memory, care must be taken to not duplicate
   pointers to this stack.                                       */
typedef struct DataQueue
{
	/* This is the next index to be added to. If Top==Bottom, the
	   queue is empty, until an entry is added at Top, and Top
	   increments.                                                */
	volatile INDEX     Top;
	/* The current bottom index. This is the next one to be
	   returned.                                            */
	volatile INDEX     Bottom;
	/* How many elements the queue can hold. If a queue has more
	   elements added to it than it has count, it will be expanded,
	   and a new queue returned.                                    */
	INDEX     Cnt;
	/* thread interlock using InterlockedExchange semaphore */
	//volatile uint32_t     Lock;
	/* How big each element in the queue is. */
	INDEX     Size;
	/* How many elements to expand the queue by, when its capacity
	   is reached.                                                 */
	INDEX     ExpandBy;
	/* The data area of the queue. */
	uint8_t      data[1];
} DATAQUEUE, *PDATAQUEUE;
/* A mostly obsolete function, but can return the status of
   whether all initially scheduled startups are completed. (Or
   maybe whether we are not complete, and are processing
   startups)                                                   */
_CONTAINER_NAMESPACE_END
SACK_NAMESPACE_END
/* This contains the methods to use the base container types
   defined in sack_types.h.                                  */
#ifndef LINKSTUFF
#define LINKSTUFF
	SACK_NAMESPACE
	_CONTAINER_NAMESPACE
#    define TYPELIB_CALLTYPE
#  if defined( _TYPELIBRARY_SOURCE_STEAL )
#    define TYPELIB_PROC extern
#  elif defined( NO_EXPORTS )
#    if defined( _TYPELIBRARY_SOURCE )
#      define TYPELIB_PROC
#    else
#      define TYPELIB_PROC extern
#    endif
#  elif defined( _TYPELIBRARY_SOURCE )
#    define TYPELIB_PROC EXPORT_METHOD
#  else
#    define TYPELIB_PROC IMPORT_METHOD
#  endif
_LINKLIST_NAMESPACE
//--------------------------------------------------------
TYPELIB_PROC  PLIST TYPELIB_CALLTYPE        CreateListEx   ( DBG_VOIDPASS );
/* Destroy a PLIST. */
TYPELIB_PROC  PLIST TYPELIB_CALLTYPE        DeleteListEx   ( PLIST *plist DBG_PASS );
/* See <link AddLink>.
   See <link DBG_PASS>. */
TYPELIB_PROC  PLIST TYPELIB_CALLTYPE        AddLinkEx      ( PLIST *pList, POINTER p DBG_PASS );
/* Sets the value of a link at the specified index.
   Parameters
   pList :     address of a PLIST
   idx :       index of the element to set
   p :         new link value to be set at the specified index
   DBG_PASS :  debug file and line information                 */
TYPELIB_PROC  PLIST TYPELIB_CALLTYPE        SetLinkEx      ( PLIST *pList, INDEX idx, POINTER p DBG_PASS );
/* Gets the link at the specified index.
   Parameters
   pList :  address of a PLIST pointer.
   idx :    index to get the link from.  */
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE      GetLink        ( PLIST *pList, INDEX idx );
/* Gets the address of the link node in the PLIST.
   Parameters
   pList :  address of a PLIST to get the node address
   idx :    index of the node to get the adddress of
   Example
   <code lang="c++">
   PLIST list = NULL; // don't have to use CreateList();
   POINTER *a;
   POINTER b;
   POINTER *result;
   a = &amp;b;
   AddLink( &amp;list, a );
   \result = GetLinkAddress( &amp;list, 0 );
    ( (*result) == b )
   </code>                                               */
TYPELIB_PROC  POINTER* TYPELIB_CALLTYPE     GetLinkAddress ( PLIST *pList, INDEX idx );
/* Locate a pointer in a PLIST. Return the index.
   Parameters
   pList :  address of a list pointer to locate link
   value :  link to find in the list
   Return Value List
   INVALID_INDEX :  Not found in the list
   0\-n :           Index of the first occurance of the link in the
                    list.                                           */
TYPELIB_PROC  INDEX TYPELIB_CALLTYPE        FindLink       ( PLIST *pList, POINTER value );
/* return the count of used members in a PLIST
    pList : the list to count
	Return Value
	   number of things in the list.
*/
TYPELIB_PROC  INDEX TYPELIB_CALLTYPE        GetLinkCount   ( PLIST pList );
/* Uses FindLink on the list for the value to delete, and then
   sets the index of the found link to NULL.
   Parameters
   pList :  Address of a PLIST pointer
   value :  the link to find and remove from the list.
   Example
   <code lang="c++">
   PLIST list = NULL;
	POINTER a = &#47;*some address*&#47;;
   </code>
   <code>
   AddLink( &amp;list, a );
   DeleteLink( &amp;list, a );
   </code>                                                     */
TYPELIB_PROC  LOGICAL TYPELIB_CALLTYPE      DeleteLink     ( PLIST *pList, CPOINTER value );
/* Remove all links from a PLIST. */
TYPELIB_PROC  void TYPELIB_CALLTYPE         EmptyList      ( PLIST *pList );
#ifdef __cplusplus
/* This was a basic attempt to make list into a C++ class. I
   gave up doing this sort of thing afterwards after realizing
   the methods of a library and these static methods for a class
   aren't much different.                                        */
#  if defined( INCLUDE_SAMPLE_CPLUSPLUS_WRAPPERS )
typedef class iList
{
public:
	PLIST list;
	INDEX idx;
	inline iList() { list = CreateListEx( DBG_VOIDSRC ); }
	inline ~iList() { DeleteListEx( &list DBG_SRC ); }
	inline iList &operator+=( POINTER &p ){ AddLinkEx( &list, p DBG_SRC ); return *this; }
	inline void add( POINTER p ) { AddLinkEx( &list, p DBG_SRC ); }
	inline void remove( POINTER p ) { DeleteLink( &list, p ); }
	inline POINTER first( void ) { POINTER p; for( idx = 0, p = NULL;list && (idx < list->Cnt) && (( p = GetLink( &list, idx ) )==0); )idx++; return p; }
	inline POINTER next( void ) { POINTER p; for( idx++;list && (( p = GetLink( &list, idx ) )==0) && idx < list->Cnt; )idx++; return p; }
	inline POINTER get(INDEX index) { return GetLink( &list, index ); }
} *piList;
#  endif
#endif
// address of the thing...
typedef uintptr_t (CPROC *ForProc)( uintptr_t user, INDEX idx, POINTER *item );
// if the callback function returns non 0 - then the looping is aborted,
// and the value is returned... the user value is passed to the callback.
TYPELIB_PROC  uintptr_t TYPELIB_CALLTYPE     ForAllLinks    ( PLIST *pList, ForProc func, uintptr_t user );
/* This is a iterator which can be used to check each member in
   a PLIST.
   Parameters
   list :     List to iterate through
   index :    variable to use to index the list
   type :     type of the elements stored in the list (for C++)
   pointer :  variable used to get the current member of the
              list.
   Example
   <code lang="c++">
   POINTER p;  // the pointer to receive the list member pointer (should be a user type)
   INDEX idx; // indexer
   PLIST pList; // some list.
   LIST_FORALL( pList, idx, POINTER, p )
   {
       // p will never be NULL here.
       // each link stored in the list is set to p here..
       // this is a way to remove this item from the list...
       SetLink( &amp;pList, idx, NULL );
       if( some condition )
          break;
   }
   </code>
   Another example that uses data and searches..
   <code lang="c++">
   PLIST pList = NULL;
   INDEX idx;
   CTEXTSTR string;
   AddLink( &amp;pList, (POINTER)"hello" );
   </code>
   <code>
   AddLink( &amp;pList, (POINTER)"world" );
   LITS_FORALL( pList, idx, CTEXTSTR, string )
   {
       if( strcmp( string, "hello" ) == 0 )
           break;
   }
   // here 'string' will be NULL if not found, else will be what was found
   </code>
   Remarks
   This initializes the parameters passed to the macro so that
   if the list is NULL or empty, then p will be set to NULL. If
   there are no non-nulll members in the list, p will be set to
   NULL. If you break in the loop, like in the case of searching
   the list for something, then p will be non-null at the end of
   the loop.
                                                                                         */
#define LIST_FORALL( l, i, t, v )  if(((v)=(t)NULL),(l))                                                        for( ((i)=0); ((i) < ((l)->Cnt))?                                         (((v)=(t)(uintptr_t)((l)->pNode[i])),1):(((v)=(t)NULL),0); (i)++ )  if( v )
/* This can be used to continue iterating through a list after a
   LIST_FORALL has been interrupted.
   Parameters
   list :     \Description
   index :    index variable for stepping through the list
   type :     type of the members in the list.
   pointer :  variable name to use to store the the current list
              element.
   Example
   <code lang="c++">
   PLIST pList = NULL;
   CTEXTSTR p;
   INDEX idx;
   </code>
   <code>
   AddLink( &amp;pList, "this" );
   AddLink( &amp;pList, "is" );
   AddLink( &amp;pList, "a" );
   AddLink( &amp;pList, "test" );
   LIST_FORALL( pList, idx, CTEXTSTR, p )
   {
       if( strcmp( p, "is" ) == 0 )
           break;
   }
   LIST_NEXTALL( pList, idx, CTEXTSTR, p )
   {
       printf( "remaining element : %s", p );
   }
   </code>
   <code lang="c++">
   j
   </code>                                                       */
#define LIST_NEXTALL( l, i, t, v )  if(l)                for( ++(i),((v)=(t)NULL); ((i) < ((l)->Cnt))?     (((v)=(t)(l)->pNode[i]),1):(((v)=(t)NULL),0); (i)++ )  if( v )
/* <combine sack::containers::list::CreateListEx@DBG_VOIDPASS>
   \ \                                                         */
#define CreateList()       ( CreateListEx( DBG_VOIDSRC ) )
/* <combine sack::containers::list::DeleteListEx@PLIST *plist>
   \ \                                                         */
#ifndef FIX_RELEASE_COM_COLLISION
#  define DeleteList(p)      ( DeleteListEx( (p) DBG_SRC ) )
#endif
/* Adds a pointer to a user object to a list.
   Example
   <code lang="c++">
   // the list can be initialized to NULL,
   // it does not have to be assigned the result of a CreateList().
   // this allows the list to only be allocated if it is used.
   PLIST list = NULL;
   AddLink( &amp;list, (POINTER)user_pointer );
   {
       POINTER p; // this should be USER_DATA_TYPE *p;
       INDEX idx; // just a generic counter.
       LIST_FORALL( list, idx, POINTER, p )
       {
           // for each item in the list, p will be not null.
           if( p-\>something == some_other_thing )
               break;
       }
       // p will be NULL if the list is empty
       // p will be NULL if the LIST_FORALL loop completes to termination.
       // p will be not NULL if the LIST_FORALL loop executed a 'break;'
   }
   </code>                                                                 */
#define AddLink(p,v)       ( AddLinkEx( (p),((POINTER)(v)) DBG_SRC ) )
/* <combine sack::containers::list::SetLinkEx@PLIST *@INDEX@POINTER p>
   \ \                                                                 */
#define SetLink(p,i,v)     ( SetLinkEx( (p),(i),((POINTER)(v)) DBG_SRC ) )
#ifdef __cplusplus
 //		namespace list;
	}
#endif
//--------------------------------------------------------
_DATALIST_NAMESPACE
/* Creates a data list which hold data elements of the specified
   size.
                                                                 */
TYPELIB_PROC  PDATALIST TYPELIB_CALLTYPE  CreateDataListEx ( uintptr_t nSize DBG_PASS );
/* <combine sack::containers::data_list::DeleteDataList>
   \ \                                                   */
TYPELIB_PROC  void TYPELIB_CALLTYPE       DeleteDataListEx ( PDATALIST *ppdl DBG_PASS );
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE    SetDataItemEx ( PDATALIST *ppdl, INDEX idx, POINTER data DBG_PASS );
/* Adds an item to a DataList.
   Example
   <code lang="c++">
   PDATALIST datalist = CreateDataList();
   struct my_struct {
       uint32_t my_data;
   }
   struct my_struct my_item;
   my_item.my_data = 0;
   AddDataItem( &amp;datalist, &amp;my_item );
   </code>                                     */
#define AddDataItem(list,data) (((list)&&(*(list)))?SetDataItemEx((list),(*list)->Cnt,data DBG_SRC ):NULL)
/* Sets the item at a specific nodes to the new data.
   Parameters
   ppdl :      address of a PDATALIST.
   idx :       index of element in list to set
   data :      POINTER to data to set element to
   DBG_PASS :  optional debug file/line information
   Example
   <code lang="c++">
      PDATALIST pdl;
      int oldval = 3;
      int newval = 5;
      pdl = CreateDataList( sizeof( int ) ); // store int's as data
      AddDataItem( &amp;pdl, &amp;oldval );
      SetDataItem( &amp;pdl, 0, &amp;newval );
   </code>                                                          */
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE    SetDataItemEx ( PDATALIST *ppdl, INDEX idx, POINTER data DBG_PASS );
/* \Returns a pointer to the data at a specified index.
   Parameters
   \ \
   ppdl :  address of a PDATALIST
   idx :   index of element to get                      */
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE    GetDataItem ( PDATALIST *ppdl, INDEX idx );
/* Removes a data element from the list (moves all other
   elements down over it since there is no used indicator.
   Parameters
   ppdl :  address of a PDATALIST.
   idx :   index of element to delete                      */
TYPELIB_PROC  void TYPELIB_CALLTYPE       DeleteDataItem ( PDATALIST *ppdl, INDEX idx );
/* Empties a PDATALIST of all content.
   Parameters
   ppdl :  address of a PDATALIST
   Example
   <code lang="c++">
   PDATALIST pdl;
   pdl = CreateDataList( sizeof( int ) ); // store int's as data
   EmptyDataList( &amp;pdl );
   </code>                                                       */
TYPELIB_PROC  void TYPELIB_CALLTYPE       EmptyDataList ( PDATALIST *ppdl );
/* For loop to iterate through all items in a PDATALIST.
   <code lang="c++">
   PDATALIST pdl;
   pdl = CreateDataList( sizeof( int ) );
   {
      INDEX index;
      int *value;
      DATA_FORALL( pdl, index, int, value )
      {
      }
   }
   </code>                                               */
#define DATA_FORALL( l, i, t, v )  if(((v)=(t)NULL),(l)&&((l)->Cnt != INVALID_INDEX))	   for( ((i)=0);	                         (((i) < (l)->Cnt)                                             ?(((v)=(t)((l)->data + (uintptr_t)(((l)->Size) * (i)))),1)	         :(((v)=(t)NULL),0))&&(v); (i)++ )
/* <code>
   PDATALIST pdl;
   pdl = CreateDataList( sizeof( int ) );
   {
      INDEX index;
      int *value;
      DATA_FORALL( pdl, index, int, value )
      {
          // abort loop early
      }
      DATA_NEXTALL( pdl, index, int, value )
      {
      }
   }
   </code>                                   */
#define DATA_NEXTALL( l, i, t, v )  if(((v)=(t)NULL),(l))	   for( ((i)++);	                         ((i) < (l)->Cnt)                                             ?((v)=(t)((l)->data + (((l)->Size) * (i))))	         :(((v)=(t)NULL),0); (i)++ )
/* <combine sack::containers::data_list::CreateDataListEx@uintptr_t nSize>
   Creates a DataList specifying just the size. Uses the current
   source and line for debugging parameter.                               */
#define CreateDataList(sz) ( CreateDataListEx( (sz) DBG_SRC ) )
/* Destroy a DataList.
   Example
   <code>
   PDATALIST datalist = CreateDataList( 4 );
   DeleteDataList( &amp;datalist );
   </code>
   Parameters
   ppDataList :  pointer to the PDATALIST.   */
#define DeleteDataList(p)  ( DeleteDataListEx( (p) DBG_SRC ) )
/* <combine sack::containers::data_list::SetDataItemEx@PDATALIST *@INDEX@POINTER data>
   \ \                                                                                 */
#define SetDataItem(p,i,v) ( SetDataItemEx( (p),(i),(v) DBG_SRC ) )
   _DATALIST_NAMESPACE_END
//--------------------------------------------------------
#ifdef __cplusplus
		namespace link_stack {
#endif
/* Creates a new stack for links (POINTERS).
   Parameters
   DBG_PASS :  Debug file and line information to use for the
               allocation of the stack.
   Returns
   Pointer to a new link stack.                               */
TYPELIB_PROC  PLINKSTACK TYPELIB_CALLTYPE   CreateLinkStackEx( DBG_VOIDPASS );
/* Creates a new stack for links (POINTERS).  Link stack has a limited number of entries.
    When the stack fills, the oldest item on the stack is removed automatically.
	 Parameters
	 max_entries : maximum depth of the stack.
   DBG_PASS :  Debug file and line information to use for the
               allocation of the stack.
   Returns
   Pointer to a new link stack.                               */
         // creates a link stack with maximum entries - any extra entries are pushed off the bottom into NULL
TYPELIB_PROC  PLINKSTACK TYPELIB_CALLTYPE      CreateLinkStackLimitedEx        ( int max_entries  DBG_PASS );
/* <combine sack::containers::link_stack::CreateLinkStackLimitedEx@int max_entries>
   Macro to pass default debug file and line information.                           */
#define CreateLinkStackLimited(n) CreateLinkStackLimitedEx(n DBG_SRC)
/* Destroy a link stack. Sets the pointer to the stack to NULL
   on deletion.
   Parameters
   pls :       address of a link stack pointer
   DBG_PASS :  debug file and line information                 */
TYPELIB_PROC  void TYPELIB_CALLTYPE         DeleteLinkStackEx( PLINKSTACK *pls DBG_PASS);
/* Pushes a new link on the stack.
   Parameters
   pls :       address of a link stack pointer
   p :         new pointer to push on the stack
   DBG_PASS :  debug source file and line information.
   Returns
   New link stack pointer if the stack was reallocated to have
   more space. Since the address of the pointer is passed, the
   pointer is already updated, and the return value is
   unimportant.                                                */
TYPELIB_PROC  PLINKSTACK TYPELIB_CALLTYPE   PushLinkEx       ( PLINKSTACK *pls, POINTER p DBG_PASS);
/* Reads the top value of the stack and returns it, removes top
   link on the stack.
   Parameters
   pls :  address of a link stack pointer
   Return Value List
   NULL :      Stack was empty
   not NULL :  Link that was on the top of the stack.           */
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE      PopLink          ( PLINKSTACK *pls );
/* Look at the top link on the stack.
   Parameters
   pls :  address of a link stack pointer
   Return Value List
   NULL :      Nothing on stack.
   not NULL :  link on the top of the stack. */
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE      PeekLink         ( PLINKSTACK *pls );
/* Look at links in the stack.
   Parameters
	pls :  address of a link stack pointer
	n : index of the element from the top to look at
   Return Value List
   NULL :      Nothing on stack at the position specified.
   not NULL :  link on the top of the stack. */
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE      PeekLinkEx         ( PLINKSTACK *pls, INDEX n );
// thought about adding these, but decided on creating a limited stack instead.
//TYPELIB_PROC  POINTER TYPELIB_CALLTYPE      StackLength      ( PLINKSTACK *pls );
//TYPELIB_PROC  POINTER TYPELIB_CALLTYPE      PopLinkEx        ( PLINKSTACK *pls, int position );
/* <combine sack::containers::link_stack::CreateLinkStackEx@DBG_VOIDPASS>
   Macro to pass default file and line information.                       */
#define CreateLinkStack()  CreateLinkStackEx( DBG_VOIDSRC )
/* <combine sack::containers::link_stack::DeleteLinkStackEx@PLINKSTACK *pls>
   Macro to pass default file and line information.                          */
#define DeleteLinkStack(p) DeleteLinkStackEx((p) DBG_SRC)
/* <combine sack::containers::link_stack::PushLinkEx@PLINKSTACK *@POINTER p>
   Macro to pass default debug file and line information.                    */
#define PushLink(p, v)     PushLinkEx((p),(v) DBG_SRC)
#ifdef __cplusplus
 //		namespace link_stack {
		}
#endif
//--------------------------------------------------------
#ifdef __cplusplus
		namespace data_stack {
#endif
/* Creates a data stack for data element of the specified size.
   Parameters
   size :       size of elements in the stack
   DBG_PASS :  debug file and line information.                 */
TYPELIB_PROC  PDATASTACK TYPELIB_CALLTYPE   CreateDataStackEx( size_t size DBG_PASS );
/* Creates a data stack for data element of the specified size.
   Parameters
   size :       size of items in the stack
   count :      max items in stack (oldest gets deleted)
   DBG_PASS :  debug file and line information.                 */
TYPELIB_PROC  PDATASTACK TYPELIB_CALLTYPE   CreateDataStackLimitedEx( size_t size, INDEX count DBG_PASS );
/* Destroys a data stack.
   Parameters
   pds :       address of a data stack pointer. The pointer will
               be set to NULL when the queue is destroyed.
   DBG_PASS :  Debug file and line information.                  */
TYPELIB_PROC  void TYPELIB_CALLTYPE         DeleteDataStackEx( PDATASTACK *pds DBG_PASS);
/* Push a data element onto the stack. The size of the element
   is known at the stack creation time.
   Parameters
   pds :       address of a data stack pointer
   p :         pointer to data to push on stack
   DBG_PASS :  debug file and line information                 */
TYPELIB_PROC  PDATASTACK TYPELIB_CALLTYPE   PushDataEx     ( PDATASTACK *pds, POINTER pdata DBG_PASS );
/* \Returns an allocated buffer containing the data on the
   stack. Removes item from the stack.
   Parameters
   pds :  address of a data stack to get data from         */
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE      PopData        ( PDATASTACK *pds );
/* Clear all data stored in the stack.
   Parameters
   pds :  address of a data stack pointer. */
TYPELIB_PROC  void TYPELIB_CALLTYPE         EmptyDataStack ( PDATASTACK *pds );
/* Look at top item in the stack without removing it.
   Parameters
   pds :  address of a data stack to look at          */
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE      PeekData       ( PDATASTACK *pds );
// Incrementing Item moves progressivly down the stack
// final(invalid) stack, and/or empty stack will return NULL;
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE      PeekDataEx     ( PDATASTACK *pds, INDEX Item );
 /* keeps data on stack (can be used)
                                                                                      Parameters
                                                                                      pds :   address of a data stack pointer
                                                                                      Item :  Item to peek at; 0 is the top, 1 is just below it...
                                                                                              (maybe \-1 is last and further up)
                                                                                      Returns
                                                                                      \returns the address of the data item in the data stack.     */
/* <combine sack::containers::data_stack::CreateDataStackEx@INDEX size>
   Macro to pass default file and line information.                     */
#define CreateDataStack(size) CreateDataStackEx( size DBG_SRC )
/* <combine sack::containers::data_stack::CreateDataStackEx@INDEX size>
   Macro to pass default file and line information.                     */
#define CreateDataStackLimited(size,items) CreateDataStackLimitedEx( size,items DBG_SRC )
/* <combine sack::containers::data_stack::DeleteDataStackEx@PDATASTACK *pds>
   Macro to pass default file and line information.                          */
#define DeleteDataStack(p) DeleteDataStackEx((p) DBG_SRC)
/* <combine sack::containers::data_stack::PushDataEx@PDATASTACK *@POINTER pdata>
   Macro to pass default file and line information.                              */
#define PushData(pds,p) PushDataEx(pds,p DBG_SRC )
#ifdef __cplusplus
 //		namespace data_stack {
		}
#endif
/* Queue container - can enque (at tail) deque (from head) and preque (at head). Can also browse the queue with peekqueue. */
#ifdef __cplusplus
		namespace queue {
#endif
/* Creates a <link sack::containers::PLINKQUEUE, LinkQueue>. In
   debug mode, gets passed the current source and file so it can
   blame the user for the allocation.                            */
TYPELIB_PROC  PLINKQUEUE TYPELIB_CALLTYPE   CreateLinkQueueEx( DBG_VOIDPASS );
/* Delete a link queue. Pass the address of the pointer to the
   queue to delete, this function sets the pointer to NULL if
   the queue is actually deleted.                              */
TYPELIB_PROC  void TYPELIB_CALLTYPE         DeleteLinkQueueEx( PLINKQUEUE *pplq DBG_PASS );
/* Enque a link to the queue.  */
TYPELIB_PROC  PLINKQUEUE TYPELIB_CALLTYPE   EnqueLinkEx      ( PLINKQUEUE *pplq, POINTER link DBG_PASS );
TYPELIB_PROC  void TYPELIB_CALLTYPE   EnqueLinkNLEx( PLINKQUEUE *pplq, POINTER link DBG_PASS );
/* EnqueLink adds the new item at the end of the list. PrequeueLink
   puts the new item at the head of the queue (so it's the next
   one to be retrieved).                                            */
TYPELIB_PROC  PLINKQUEUE TYPELIB_CALLTYPE   PrequeLinkEx      ( PLINKQUEUE *pplq, POINTER link DBG_PASS );
/* If the queue is not empty, returns the address of the next
   element in the queue and removes the element from the queue.
                                                                */
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE      DequeLink        ( PLINKQUEUE *pplq );
TYPELIB_PROC POINTER  TYPELIB_CALLTYPE      DequeLinkNL      ( PLINKQUEUE *pplq );
/* Return TRUE/FALSE if the queue is empty or not. */
TYPELIB_PROC  LOGICAL TYPELIB_CALLTYPE      IsQueueEmpty     ( PLINKQUEUE *pplq );
/* Gets the number of elements current in the queue. */
TYPELIB_PROC  INDEX TYPELIB_CALLTYPE        GetQueueLength   ( PLINKQUEUE plq );
// get a PLINKQUEUE element at index
//  If idx < 0 then count from the end of the queue, otherwise count from the start of the queue
// start of the queue is the next element to be dequeue, end of the queue is the last element added to the queue.
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE      PeekQueueEx    ( PLINKQUEUE plq, int idx );
/* Can be used to look at the next element in the queue without
   removing it from the queue. PeekQueueEx allows you to specify
   an index of an item in the queue to get.                      */
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE      PeekQueue    ( PLINKQUEUE plq );
/* <combinewith sack::containers::queue::CreateLinkQueueEx@DBG_VOIDPASS>
   \ \                                                                   */
#define     CreateLinkQueue()     CreateLinkQueueEx( DBG_VOIDSRC )
/* <combine sack::containers::queue::PrequeLinkEx@PLINKQUEUE *@POINTER link>
   \ \                                                                       */
#define     PrequeLink(pplq,link) PrequeLinkEx( pplq, link DBG_SRC )
/* <combine sack::containers::queue::DeleteLinkQueueEx@PLINKQUEUE *pplq>
   \ \                                                                   */
#define     DeleteLinkQueue(pplq) DeleteLinkQueueEx( pplq DBG_SRC )
/* <combine sack::containers::queue::EnqueLinkEx@PLINKQUEUE *@POINTER link>
   \ \                                                                      */
#define     EnqueLink(pplq, link) EnqueLinkEx( pplq, link DBG_SRC )
#define     EnqueLinkNL(pplq, link) EnqueLinkNLEx( pplq, link DBG_SRC )
#ifdef __cplusplus
//		namespace queue {
		}
#endif
/* Functions related to PDATAQUEUE container. DataQueue stores
   literal data elements in the list instead of just a pointer. (could
   be used for optimized vertex arrays for instance).
   int data = 3;
   int result;
   PDATAQUEUE pdq = CreateDataQueue( sizeof( int ) );
   EnqueData( &amp;pdq, &amp;data );
   DequeData( &amp;pdq, &amp;result );
   DestroyDataQueue( &amp;pdq );                                       */
#ifdef __cplusplus
		namespace data_queue {
#endif
/* Creates a PDATAQUEUE. Can pass DBG_FILELINE information to
   blame other code for the allocation.                       */
TYPELIB_PROC  PDATAQUEUE TYPELIB_CALLTYPE   CreateDataQueueEx( INDEX size DBG_PASS );
/* Creates a PDATAQUEUE that has an overridden expand-by amount
   and initial amount of entries in the queue. (expecting
   something like 1000 to start and expand by 500, instead of
   the default 0, and expand by 1.                              */
TYPELIB_PROC  PDATAQUEUE TYPELIB_CALLTYPE   CreateLargeDataQueueEx( INDEX size, INDEX entries, INDEX expand DBG_PASS );
/* Destroys a data queue. */
TYPELIB_PROC  void TYPELIB_CALLTYPE         DeleteDataQueueEx( PDATAQUEUE *pplq DBG_PASS );
/* Add a data element into the queue. */
TYPELIB_PROC  PDATAQUEUE TYPELIB_CALLTYPE   EnqueDataEx      ( PDATAQUEUE *pplq, POINTER Data DBG_PASS );
/* Enque data at the head of the queue instead of the tail. (Normally
   add at tail, take from head).                                      */
TYPELIB_PROC  PDATAQUEUE TYPELIB_CALLTYPE   PrequeDataEx      ( PDATAQUEUE *pplq, POINTER Data DBG_PASS );
/* Removes data from a queue, resulting with the data in the
   specified buffer, and result TRUE if there was an element
   else FALSE, and the buffer is not modified.               */
TYPELIB_PROC  LOGICAL TYPELIB_CALLTYPE      DequeData        ( PDATAQUEUE *pplq, POINTER Data );
/* Removes the last element in the queue. (takes from the tail). */
TYPELIB_PROC  LOGICAL TYPELIB_CALLTYPE      UnqueData        ( PDATAQUEUE *pplq, POINTER Data );
/* Checks if the queue is empty, result TRUE if nothing in it,
   else FALSE.                                                 */
TYPELIB_PROC  LOGICAL TYPELIB_CALLTYPE      IsDataQueueEmpty ( PDATAQUEUE *pplq );
/* Empty a dataqueue of all data. (Sets head=tail). */
TYPELIB_PROC  void TYPELIB_CALLTYPE         EmptyDataQueue ( PDATAQUEUE *pplq );
/*
 * get a PDATAQUEUE element at index
 * result buffer is a pointer to the type of structure expected to be
 * stored within this.  The buffer result is a copy of the data stored in the queue.
 * This enforces that data stored in the list is immutable.
 * Also on the basic DequeData function, after resulting, if the pointer to the
 * data within the queue were returned, it could become invalid immediatly after
 * returning by having another enque happen which overwrites that position in the buffer.
 * One could, in theory, set a flag in the queue that a deque was done, and not update the
 * bottom until that flag is encountered while within DequeData again...
 * the pointer to the data in the queue may also not be returned because the queue may be
 * reallocated and moved.
 */
TYPELIB_PROC  LOGICAL TYPELIB_CALLTYPE  PeekDataQueueEx    ( PDATAQUEUE *pplq, POINTER ResultBuffer, INDEX idx );
/* <combine sack::containers::data_queue::PeekDataQueueEx@PDATAQUEUE *@POINTER@INDEX>
   \ \                                                                                */
#define PeekDataQueueEx( q, type, result, idx ) PeekDataQueueEx( q, (POINTER)result, idx )
/*
 * Result buffer is filled with the last element, and the result is true, otherwise the return
 * value is FALSE, and the data was not filled in.
 */
TYPELIB_PROC  LOGICAL TYPELIB_CALLTYPE  PeekDataQueue    ( PDATAQUEUE *pplq, POINTER ResultBuffer );
/* <combine sack::containers::data_queue::PeekDataQueue@PDATAQUEUE *@POINTER>
   \ \                                                                        */
#define PeekDataQueue( q, type, result ) PeekDataQueue( q, (POINTER)result )
/* <combine sack::containers::data_queue::CreateDataQueueEx@INDEX size>
   \ \                                                                  */
#define     CreateDataQueue(size)     CreateDataQueueEx( size DBG_SRC )
/* <combine sack::containers::data_queue::CreateLargeDataQueueEx@INDEX@INDEX@INDEX expand>
   \ \                                                                                     */
#define     CreateLargeDataQueue(size,entries)     CreateLargeDataQueueEx( size,entries, 0 DBG_SRC )
/* <combine sack::containers::data_queue::DeleteDataQueueEx@PDATAQUEUE *pplq>
   \ \                                                                        */
#define     DeleteDataQueue(pplq) DeleteDataQueueEx( pplq DBG_SRC )
/* <combine sack::containers::data_queue::EnqueDataEx@PDATAQUEUE *@POINTER Data>
   \ \                                                                           */
#define     EnqueData(pplq, Data) EnqueDataEx( pplq, Data DBG_SRC )
/* <combine sack::containers::data_queue::PrequeDataEx@PDATAQUEUE *@POINTER Data>
   \ \                                                                            */
#define     PrequeData(pplq, Data) PrequeDataEx( pplq, Data DBG_SRC )
#ifdef __cplusplus
//		namespace data_queue {
		}
#endif
//---------------------------------------------------------------------------
#ifdef __cplusplus
namespace message {
#endif
/* handle to a message queue. */
typedef struct MsgDataHandle *PMSGHANDLE;
//typedef struct MsgDataQueue *PMSGQUEUE;
// messages sent - the first dword of them must be
// a message ID.
typedef void (CPROC *MsgQueueReadCallback)( uintptr_t psv, CPOINTER p, uintptr_t sz );
/* Create a named shared memory message queue.
   Parameters
   name :     name of the queue to create
   size :     size of the queue.
   Read :     read callback, called when a message is received on
              the queue.
   psvRead :  user data associated with the queue. Passed to the
              read callback.                                      */
TYPELIB_PROC  PMSGHANDLE TYPELIB_CALLTYPE  SackCreateMsgQueue ( CTEXTSTR name, size_t size
                                                      , MsgQueueReadCallback Read
                                                      , uintptr_t psvRead );
/* Open a message queue. Opens if it exists, does not create.
   Parameters
   name :     name of the queue.
   Read :     read callback called when a message is received.
   psvRead :  user data associated with this queue, and passed to
              the read callback.                                  */
TYPELIB_PROC  PMSGHANDLE TYPELIB_CALLTYPE  SackOpenMsgQueue ( CTEXTSTR name
													 , MsgQueueReadCallback Read
													 , uintptr_t psvRead );
/* Destroys a message queue.
   Parameters
   ppmh :  address of the message queue handle to close (sets
           pointer to NULL when deleted)                      */
TYPELIB_PROC  void TYPELIB_CALLTYPE  DeleteMsgQueue ( PMSGHANDLE **ppmh );
 // if enque, fail send, return immediate on fail
#define MSGQUE_NOWAIT 0x0001
                             // if deque, fail no msg ready to get...
 // read any msg BUT MsgID
#define MSGQUE_EXCEPT 0x0002
 // enque this message... it is a task ID which is waiting.
#define MSGQUE_WAIT_ID 0x0004
/* Error result if there is no message to read. (GetLastError()
   after peekmsg or readmsg returns -1)                         */
#define MSGQUE_ERROR_NOMSG 1
/* Error result if the message to read is bigger than the buffer
   passed to read the message.                                   */
#define MSGQUE_ERROR_E2BIG 2
/* Error result. Unexpected error (queue head/tail out of
   bounds)                                                */
#define MSGQUE_ERROR_EABORT 5
// result is the size of the message, or 0 if no message.
// -1 if some other error?
TYPELIB_PROC  int TYPELIB_CALLTYPE  DequeMsgEx ( PMSGHANDLE pmh, long *MsgID, POINTER buffer, size_t msgsize, uint32_t options DBG_PASS );
/* Receives a message from the message queue.
   Parameters
   Message Queue :  PMSGHANDLE to read from
   Message ID * :   a Pointer to the message ID to read. Updated
                    with the message ID from the queue.
   buffer :         buffer to read message into
   buffer length :  length of the buffer to read
   options :        extra options for the read
   Return Value List
   \-1 :  Error
   0 :    No Message to read
   \>0 :  size of message read.
   Returns
   \ \                                                           */
#define DequeMsg(q,b,s,i,o) DequeMsgEx(q,b,s,i,o DBG_SRC )
/* <combine sack::containers::message::PeekMsg>
   \ \                                          */
TYPELIB_PROC  int TYPELIB_CALLTYPE  PeekMsgEx ( PMSGHANDLE pmh, long MsgID, POINTER buffer, size_t msgsize, uint32_t options DBG_PASS );
/* Just peek at the next message.
   Parameters
   queue :        The PMSGHANDLE queue to read.
   MsgID :        what message to read. 0 is read any message.
   buffer :       where to read the message data into.
   buffer_size :  the length of the message buffer.
   options :      Options controlling the read
   Returns
   \-1 on error
   0 if no message
   length of the message read                                  */
#define PeekMsg(q,b,s,i,o) PeekMsgEx(q,b,s,i,o DBG_SRC )
/* <combine sack::containers::message::EnqueMsg>
   \ \                                          */
TYPELIB_PROC  int TYPELIB_CALLTYPE  EnqueMsgEx ( PMSGHANDLE pmh, POINTER buffer, size_t msgsize, uint32_t options DBG_PASS );
/* Add a message to the queue.
   Parameters
   Message Queue :  PMSGQUEUE to write to.
   Buffer :         pointer to the message to send. THe MSgID is
                    the first part of the message buffer.
   Buffer Length :  how long the message to send is
   Options :        Extra options for send
   Return Value List
   \-1 :  Error
   \>0 :  bytes of message sent                                  */
#define EnqueMsg(q,b,s,o) EnqueMsgEx(q,b,s,o DBG_SRC )
/* Check if the message queue is empty.
   Parameters
   pmh :  queue to check if it's empty. */
TYPELIB_PROC  int TYPELIB_CALLTYPE  IsMsgQueueEmpty ( PMSGHANDLE pmh );
#ifdef __cplusplus
 //namespace message {
}
#endif
/* Routines to deal with SLAB allocated blocks of structures.
   Each slab has multiple elements of a type in it, and the
   blocks are tracked as a linked list. Each block also has a
   bitmask of allocated elements in the set.
   \---------------------------------------------------------------------------
   Set type
   Usage:
   typedef struct name_tag { } \<name\>;
   \#define MAX\<name\>SPERSET
   DeclareSet( \<name\> );
   Should alias GetFromset, DeleteFromSet, CountUsedInSet,
   GetLinearSetArray
   etc so that the type name is reflected there
   another good place where #define defining defines is good.
   \---------------------------------------------------------------------------
                                                                                */
_SETS_NAMESPACE
//---------------------------------------------------------------------------
// Set type
//   Usage:
//      typedef struct name_tag { } <name>;
//      #define MAX<name>SPERSET
//      DeclareSet( <name> );
//    Should alias GetFromset, DeleteFromSet, CountUsedInSet, GetLinearSetArray
//       etc so that the type name is reflected there
//       another good place where #define defining defines is good.
//---------------------------------------------------------------------------
/* Hard coded 32 bit division for getting word index. (x\>\>5) */
#define UNIT_USED_IDX(n)   ((n) >> 5)
/* Hard coded 32 bit division for getting bit index. (x &amp;
   0x1f)                                                      */
#define UNIT_USED_MASK(n)  (1 << ((n) &0x1f))
/* A macro for use by internal code that marks a member of a set
   as used.
   Parameters
   set :    pointer to a genericset
   index :  item to mark used.                                   */
#define SetUsed(set,n)   ((((set)->bUsed[UNIT_USED_IDX(n)]) |= UNIT_USED_MASK(n)), (++(set)->nUsed) )
/* A macro for use by internal code that marks a member of a set
   as available.
   Parameters
   set :    pointer to a genericset
   index :  item to mark available.                              */
#define ClearUsed(set,n) ((((set)->bUsed[UNIT_USED_IDX(n)]) &= ~UNIT_USED_MASK(n)), (--(set)->nUsed) )
/* A macro for use by internal code that tests a whole set of
   bits for used. (32 bits, can check to see if any in 32 is
   free)
   Parameters
   set :    pointer to a genericset
   index :  index of an one in the set of 32 being tested.
   Returns
   0 if not all are used.
   1 if all in this block of bits are used.                   */
#define AllUsed(set,n)   (((set)->bUsed[UNIT_USED_IDX(n)]) == 0xFFFFFFFF )
/* A macro for use by internal code that tests a member of a set
   as used.
   Parameters
   set :    pointer to a genericset
   index :  item to test used.
   Returns
   not zero if is used, otherwise is free.                       */
#define IsUsed(set,n)    (((set)->bUsed[UNIT_USED_IDX(n)]) & UNIT_USED_MASK(n) )
#ifdef __cplusplus
#define CPP_(n)
/* A macro which is used to emit code in C++ mode... */
#else
#define CPP_(n)
#endif
// requires a symbol of MAX<insert name>SPERSET to declare max size...
 //ndef __cplusplus
#if 1
#define SizeOfSet(size,count)  (sizeof(POINTER)*2+sizeof(int)+sizeof( uint32_t[((count)+31)/32] ) + ((size)*(count)))
#define DeclareSet( name )  typedef struct name##set_tag {	   struct name##set_tag *next, *prior;	                      uint32_t nUsed;	                                               uint32_t nBias;	                                               uint32_t bUsed[(MAX##name##SPERSET + 31 ) / 32];	              name p[MAX##name##SPERSET];	                           CPP_(int forall(uintptr_t(CPROC*f)(void*,uintptr_t),uintptr_t psv) {if( this ) return _ForAllInSet( (struct genericset_tag*)this, sizeof(name), MAX##name##SPERSET, f, psv ); else return 0; })	 CPP_(name##set_tag() { next = NULL;prior = NULL;nUsed = 0; nBias = 0; MemSet( bUsed, 0, sizeof( bUsed ) ); MemSet( p, 0, sizeof( p ) );} )	} name##SET, *P##name##SET
#define DeclareClassSet( name ) typedef struct name##set_tag {	   struct name##set_tag *next, *prior;	                      uint32_t nUsed;	                                               uint32_t nBias;	                                               uint32_t bUsed[(MAX##name##SPERSET + 31 ) / 32];	              class name p[MAX##name##SPERSET];	                        CPP_(int forall(uintptr_t(CPROC*)(void*f,uintptr_t),uintptr_t psv) {if( this ) return _ForAllInSet( (struct genericset_tag*)this, sizeof(class name), MAX##name##SPERSET, f, psv ); else return 0; })	 } name##SET, *P##name##SET
#endif
/* This represents the basic generic set structure. Addtional
   data is allocated at the end of this strcture to fit the bit
   array that maps usage of the set, and for the set size of
   elements.
   Remarks
   \ \
   Summary
   Generic sets are good for tracking lots of tiny structures.
   They track slabs of X structures at a time. They allocate a
   slab of X structures with an array of X bits indicating
   whether a node is used or not. The structure overall has how
   many are used, so once full, a block can be quickly checked
   whether there is anything free. Then when checking a block
   that might have room, the availablility is checked 32 bits at
   a time, until a free spot is found.
   Sets of 1024 members of x,y coordinates for example are good
   for this sort of storage. the points are often static, once
   loaded they all exist until none of them do. This storage has
   gross deletion methods too, quickly evaporate all allocated
   chunks. Storing tiny chunks in a slab is more efficient
   because every allocation method has some sort of tracking
   associated with it - an overhead of having it. Plus, when
   operating on sets of data, a single solid slab of exatly the
   structures you are working with is more efficient to cache.
   Example
   <code lang="c++">
   struct treenode_tag {
       uint32_t treenode_data;  // abitrary structure data
   };
   typedef struct treenode_tag TREENODE;
   \#define MAXTREENODESPERSET 256
   DeclareSet( TREENODE );
   </code>
   The important part of the prior code is the last two lines.
   \#define MAX\<your type name\>SPERSET \<how many\>
   This defines how many of your structure are kept per set
   block.
   The DeclareSet( type ) declares a typedefed structure called
   'struct type##set_tag', 'name##SET', and '*P##name##SET'; in
   the above case, it would be 'struct TREENODEset_tag',
   'TREENODESET', and 'PTREENODESET'.
   Then to actually use the set...
   <code lang="c#">
   // declare a set pointer with one of the magic names.
   PTREENODESET nodeset = NULL;
   // get a node from the set.
   TREENODE *node = GetFromSet( TREENODE, nodeset );
   </code>
   Notice there is no CreateSet, getting a set member will
   create the set as required. Many operations may expend the
   set, except for GetUsedSetMember which will only result with
   \members that are definatly in the set. Accesses to the set
   are all prefixed by the type name the set was created with,
   'TREENODE' in this example.
   <code lang="c++">
   DeleteFromSet( TREENODE, nodeset, node );
   node = GetFromSet( TREENODE, nodeset );
   {
      int index = GetMemberIndex( TREENODE, nodeset, node );
   }
   </code>
   The accessor macros take care of expanding several parameters
   that require sizeof structure expansion.                      */
typedef struct genericset_tag {
	// wow might be nice to have some flags...
	// first flag - bSetSet - meaning that this is a set of sets of
	// the type specified...
	struct genericset_tag *next;
	/* This is the pointer that's pointing at the pointer pointing
	   to me. (did you get that?) See <link DeclareLink>.          */
	struct genericset_tag **me;
	/* number of spots in this set block that are used. */
	uint32_t nUsed;
 // hmm if I change this here? we're hozed... so.. we'll do it anyhow :) evil - recompile please
	uint32_t nBias;
 // after this p * unit must be computed
	uint32_t bUsed[1];
} GENERICSET, *PGENERICSET;
/* \ \
   Parameters
   pSet :      pointer to a generic set
   nMember :   index of the member
   setsize :   number of elements in each block
   unitsize :  set block
   maxcnt :    max elements per set block       */
TYPELIB_PROC  POINTER  TYPELIB_CALLTYPE GetFromSetEx( GENERICSET **pSet, int setsize, int unitsize, int maxcnt DBG_PASS );
/* <combine sack::containers::sets::GetFromSetEx@GENERICSET **@int@int@int maxcnt>
   \ \                                                                             */
#define GetFromSeta(ps, ss, us, max) GetFromSetPoolEx( NULL, 0, 0, 0, (ps), (ss), (us), (max) DBG_SRC )
/* <combine sack::containers::sets::GetFromSetEx@GENERICSET **@int@int@int maxcnt>
   \ \
   Parameters
   name :  name of type the set contains.
   pSet :  pointer to a set to get an element from.                                */
#define GetFromSet( name, pset ) (name*)GetFromSeta( (GENERICSET**)(pset), sizeof( name##SET ), sizeof( name ), MAX##name##SPERSET )
/* \ \
   Parameters
   pSet :      pointer to a generic set
   nMember :   index of the member
   setsize :   number of elements in each block
   unitsize :  set block
   maxcnt :    max elements per set block       */
TYPELIB_PROC  PGENERICSET  TYPELIB_CALLTYPE GetFromSetPoolEx( GENERICSET **pSetSet
													 , int setsetsize, int setunitsize, int setmaxcnt
													 , GENERICSET **pSet
													 , int setsize, int unitsize, int maxcnt DBG_PASS );
/* <combine sack::containers::sets::GetFromSetPoolEx@GENERICSET **@int@int@int@GENERICSET **@int@int@int maxcnt>
   \ \                                                                                                           */
#define GetFromSetPoola(pl, sss, sus, smax, ps, ss, us, max) GetFromSetPoolEx( (pl), (sss), (sus), (smax), (ps), (ss), (us), (max) DBG_SRC )
/* <combine sack::containers::sets::GetFromSetPoolEx@GENERICSET **@int@int@int@GENERICSET **@int@int@int maxcnt>
   \ \                                                                                                           */
#define GetFromSetPool( name, pool, pset ) (name*)GetFromSetPoola( (GENERICSET**)(pool)	    , sizeof( name##SETSET ), sizeof( name##SET ), MAX##name##SETSPERSET	, (GENERICSET**)(pset), sizeof( name##SET ), sizeof( name ), MAX##name##SPERSET )
/* \ \
   Parameters
   pSet :      pointer to a generic set
   nMember :   index of the member
   setsize :   number of elements in each block
   unitsize :  set block
   maxcnt :    max elements per set block       */
TYPELIB_PROC  POINTER  TYPELIB_CALLTYPE GetSetMemberEx( GENERICSET **pSet, INDEX nMember, int setsize, int unitsize, int maxcnt DBG_PASS );
/* <combine sack::containers::sets::GetSetMemberEx@GENERICSET **@INDEX@int@int@int maxcnt>
   \ \                                                                                     */
#define GetSetMembera(ps, member, ss, us, max) (GetSetMemberEx( (ps), (member), (ss), (us), (max) DBG_SRC ))
/* <combine sack::containers::sets::GetSetMemberEx@GENERICSET **@INDEX@int@int@int maxcnt>
   \ \                                                                                     */
#define GetSetMember( name, pset, member ) ((name*)GetSetMembera( (GENERICSET**)(pset), (member), sizeof( name##SET ), sizeof( name ), MAX##name##SPERSET ))
/* \ \
   Parameters
   pSet :      pointer to a generic set
   nMember :   index of the member
   setsize :   number of elements in each block
   unitsize :  set block
   maxcnt :    max elements per set block       */
TYPELIB_PROC  POINTER  TYPELIB_CALLTYPE GetUsedSetMemberEx( GENERICSET **pSet, INDEX nMember, int setsize, int unitsize, int maxcnt DBG_PASS );
/* <combine sack::containers::sets::GetUsedSetMemberEx@GENERICSET **@INDEX@int@int@int maxcnt>
   \ \                                                                                         */
#define GetUsedSetMembera(ps, member, ss, us, max) (GetUsedSetMemberEx( (ps), (member), (ss), (us), (max) DBG_SRC ))
/* <combine sack::containers::sets::GetUsedSetMemberEx@GENERICSET **@INDEX@int@int@int maxcnt>
   \ \                                                                                         */
#define GetUsedSetMember( name, pset, member ) ((name*)GetUsedSetMembera( (GENERICSET**)(pset), (member), sizeof( name##SET ), sizeof( name ), MAX##name##SPERSET ))
TYPELIB_PROC  INDEX TYPELIB_CALLTYPE  GetMemberIndex(GENERICSET **set, POINTER unit, int unitsize, int max );
/* Gets the index of a member passed as a pointer.
   Parameters
   set :       pointer to the set the member is in
   unit :      pointer to the member in the set to get the index
               of.
   unitsize :  size of each member in the set
   max :       count of members in each set block.
   Returns
   \Returns the index of the member passed in as a pointer.      */
#define GetMemberIndex(name,set,member) GetMemberIndex( (GENERICSET**)set, member, sizeof( name ), MAX##name##SPERSET )
/* <combine sack::containers::sets::GetMemberIndex>
   \ \                                              */
#define GetIndexFromSet( name, pset ) GetMemberIndex( name, pset, GetFromSet( name, pset ) )
/* \ \
   Parameters
   pSet :      pointer to a generic set
   nMember :   index of the member
   setsize :   number of elements in each block
   unitsize :  set block
   maxcnt :    max elements per set block       */
TYPELIB_PROC  void TYPELIB_CALLTYPE  DeleteFromSetExx( GENERICSET *set, POINTER unit, int unitsize, int max DBG_PASS );
/* <combine sack::containers::sets::DeleteFromSetExx@GENERICSET *@POINTER@int@int max>
   \ \                                                                                 */
#define DeleteFromSetEx( name, set, member, xx ) DeleteFromSetExx( (GENERICSET*)set, member, sizeof( name ), MAX##name##SPERSET DBG_SRC )
/* <combine sack::containers::sets::DeleteFromSetExx@GENERICSET *@POINTER@int@int max>
   \ \                                                                                 */
#define DeleteFromSet( name, set, member ) DeleteFromSetExx( (GENERICSET*)set, member, sizeof( name ), MAX##name##SPERSET DBG_SRC )
/* Marks a member in a set as usable.
   Parameters
   set :       pointer to a genericset pointer
   iMember :   index of member to delete
   unitsize :  (filled by macro) size of element in set
   max :       (filled by macro) size of a block of elements. */
TYPELIB_PROC  void TYPELIB_CALLTYPE  DeleteSetMemberEx( GENERICSET *set, INDEX iMember, uintptr_t unitsize, INDEX max );
/* <combine sack::containers::sets::DeleteSetMemberEx@GENERICSET *@INDEX@uintptr_t@INDEX>
   \ \                                                                                   */
#define DeleteSetMember( name, set, member ) DeleteSetMemberEx( (GENERICSET*)set, member, sizeof( name ), MAX##name##SPERSET )
/* This function can check to see if a pointer is a valid
   element from a set.
   Parameters
   set :       pointer to a set to check
   unit :      pointer to an element from the set
   unitsize :  size of element structures in the set.
   max :       count of structures per set block
   Returns
   TRUE if unit is in the set, else FALSE.                */
TYPELIB_PROC  int TYPELIB_CALLTYPE  MemberValidInSetEx( GENERICSET *set, POINTER unit, int unitsize, int max );
/* <combine sack::containers::sets::MemberValidInSetEx@GENERICSET *@POINTER@int@int>
   \ \                                                                               */
#define MemberValidInSet( name, set, member ) MemberValidInSetEx( (GENERICSET*)set, member, sizeof( name ), MAX##name##SPERSET )
TYPELIB_PROC  int TYPELIB_CALLTYPE  CountUsedInSetEx( GENERICSET *set, int max );
/* Count number of elements that are allocated in the set.
   Parameters
   set :  The set to check
   max :  max items per set (may be unused, since this is stored
          internally now)
   Returns
   The number of items in the step.                              */
#define CountUsedInSet( name, set ) CountUsedInSetEx( (GENERICSET*)set, MAX##name##SPERSET )
TYPELIB_PROC  POINTER * TYPELIB_CALLTYPE GetLinearSetArrayEx( GENERICSET *pSet, int *pCount, int unitsize, int max );
/* Converts a set into a copy of the objects in the set
   organized in a flat array.
   Parameters
   pSet :      set to convert to an array
   pCount :    address of an integer to receive the count of
               elements put in the array.
   unitsize :  size of each element in the set
   max :       count of elements per set block
   Returns
   Pointer to an array that are a copy of the objects in the
   set.                                                      */
#define GetLinearSetArray( name, set, pCount ) GetLinearSetArrayEx( (GENERICSET*)set, pCount, sizeof( name ), MAX##name##SPERSET )
/* Returned the index of an item in a linear array returned from
   a set.
   Parameters
   pArray :      pointer to an array which has been returned from
                 the set
   nArraySize :  size fo the array
   unit :        pointer to an element in the array
   Returns
   Index of the unit in the array, INVALID_INDEX if not in the
   array.                                                         */
TYPELIB_PROC  int TYPELIB_CALLTYPE  FindInArray( POINTER *pArray, int nArraySize, POINTER unit );
/* Delete all allocated slabs.
   Parameters
   ppSet :  pointer to a generic set pointer to delete. */
TYPELIB_PROC  void TYPELIB_CALLTYPE  DeleteSet( GENERICSET **ppSet );
/* <combine sack::containers::sets::DeleteSet@GENERICSET **>
   \ \                                                       */
#define DeleteSetEx( name, ppset ) { name##SET **delete_me = ppset; DeleteSet( (GENERICSET**)delete_me ); }
/* <combine sack::containers::sets::ForAllInSet>
   ForAllinSet Callback - callback fucntion used with
   ForAllInSet                                        */
typedef uintptr_t (CPROC *FAISCallback)(void*,uintptr_t);
/* \ \
   Parameters
   pSet :      poiner to a set
   unitsize :  size of elements in the array
   max :       count of elements per set block
   f :         user callback function to call for each element in
               the set
   psv :       user data passed to the user callback when it is
               invoked for a member of the set.
   Returns
   If the user callback returns 0, the loop continues. If the
   user callback returns non zero then the looping through the
   set ends, and that result is returned.                         */
TYPELIB_PROC  uintptr_t TYPELIB_CALLTYPE  _ForAllInSet( GENERICSET *pSet, int unitsize, int max, FAISCallback f, uintptr_t psv );
/* <combine sack::containers::sets::ForEachSetMember>
   ForEachSetMember Callback function - for the function '
   ForEachSetMember'                                       */
typedef uintptr_t (CPROC *FESMCallback)(INDEX,uintptr_t);
TYPELIB_PROC  uintptr_t TYPELIB_CALLTYPE  ForEachSetMember ( GENERICSET *pSet, int unitsize, int max, FESMCallback f, uintptr_t psv );
 //def __cplusplus
#if 0
#define DeclareSet(name)	                                struct name##set_tag {	               uint32_t set_size;	                             uint32_t element_size;	                         uint32_t element_cnt;	                          PGENERICSET pool;	                        name##set_tag() {	                        element_size = sizeof( name );	             element_cnt = MAX##name##SPERSET;	          set_size = (element_size * element_cnt )+ ((((element_cnt + 31 )/ 32 )- 1 ) * 4) + sizeof( GENERICSET );	 pool = NULL;	                               }	    ~name##set_tag() { DeleteSet( &pool ); }	 name* grab() { return (name*)GetFromSetEx( &pool, set_size, element_size, element_cnt DBG_SRC ); }	 name* grab(INDEX member) { return (name*)GetSetMemberEx( &pool, member, set_size, element_size, element_cnt DBG_SRC ); }	 name* get(INDEX member) { return (this)?(name*)GetUsedSetMemberEx( &pool, member, set_size, element_size, element_cnt DBG_SRC ):(NULL); }	 void drop( name* member ) { DeleteFromSetEx( pool, (POINTER)member, element_size, element_cnt ); }	 int valid( name* member ) { return MemberValidInSetEx( pool, (POINTER)member, element_size, element_cnt ); }	 uintptr_t forall( FAISCallback f, uintptr_t psv ) { if( this ) return _ForAllInSet( pool, element_size, element_cnt, f, psv ); else return 0; }	 };	       typedef struct name##set_tag *P##name##SET, name##SET;
#define ForAllInSet(name, pset,f,psv) _ForAllInSet( (GENERICSET*)(pset), sizeof( name ), MAX##name##SPERSET, (f), (psv) )
#else
/* <combine sack::containers::sets::_ForAllInSet@GENERICSET *@int@int@FAISCallback@uintptr_t>
   \ \                                                                                       */
#define ForAllInSet(name, pset,f,psv) _ForAllInSet( (GENERICSET*)(pset), sizeof( name ), MAX##name##SPERSET, (f), (psv) )
/* Performs an iteration over each allocated set member. Calls
   the user provided callback routine with each element in the
   set.
   Parameters
   pSet :      pointer to the set to iterate
   unitsize :  size of each element
   max :       max count of elements per set block
   f :         function to call ( uintptr_t (*)(INDEX,uintptr_t) )
   psv :       user data value to pass to function as uintptr_t
   Returns
   uintptr_t - this value is the return of the user function if
   the function does not return 0. A non zero return from the
   user callback stops iteration.                                */
#define ForEachSetMember(name,pset,f,psv) ForEachSetMember( (GENERICSET*)(pset),sizeof(name),MAX##name##SPERSET, (f), (psv) )
#endif
//---------------------------------------------------------------------------
_SETS_NAMESPACE_END
_TEXT_NAMESPACE
// this defines more esoteric formatting notions...
// these data blocks will be zero sized, and ahve the TF_FORMATEX
// bit set.
//#define DEFAULT_COLOR 0xF7
//#define PRIOR_COLOR 0xF6 // this does not change the color....
// these enumerated ops put in the foreground field of a format
// with a flag of TF_FORMATEX will cause the specified operation
// to be carried out on a display (not files) or generated into
// the appropriate sequence (ansi out encode)
// -- correction
//  this is encoded into its own field for the format
// size, due to machine optimization, 16 bits were free
// this was expanded and used for all information
// a segment may contain extended op, color, attributes,
// and text, everything short of a font for it...
//  - not sure how to address that issue... there's
// certainly modifications to current font... italic for
// instance..
	enum FORMAT_OPS {
      /* this segment clears to the end of the line.  Its content is then added to the output */
		FORMAT_OP_CLEAR_END_OF_LINE = 1
        ,FORMAT_OP_CLEAR_START_OF_LINE
                   ,
						  FORMAT_OP_CLEAR_LINE
						 ,
						  FORMAT_OP_CLEAR_END_OF_PAGE
                   ,
						  FORMAT_OP_CLEAR_START_OF_PAGE
						 ,
/* clear the entire vieable page (pushes all content to history)
                    set cursor home ;6*/
						  FORMAT_OP_CLEAR_PAGE
						 ,
						  FORMAT_OP_CONCEAL
                   ,
						  FORMAT_OP_DELETE_CHARS
                   ,
						  FORMAT_OP_SET_SCROLL_REGION
                   ,
						  FORMAT_OP_GET_CURSOR
						 ,
						  FORMAT_OP_SET_CURSOR
						 ,
						  FORMAT_OP_PAGE_BREAK
						 ,
/* break between paragraphs - kinda same as lines...
						  since lines are as long as possible... ;13 */
						 FORMAT_OP_PARAGRAPH_BREAK
						 ,
/* Justify line(s if wrapped) to the right
						   This attribute should be passed through to renderer;14*/
                   FORMAT_OP_JUSTIFY_RIGHT
						 ,
/* Justify line(s if wrapped) to the center
						 This attribute should be passed through to renderer;15*/
                   FORMAT_OP_JUSTIFY_CENTER
};
//typedef struct text_color_tag { uint32_t color: 8; } TEXTCOLOR;
// this was a 32 bit structure, but 8 fore, 8 back
// 8 x, 8 y failed for positioning...
// extended position, added more information
// reduced color, 16 colors is really all that there
// are... 4 bits... added bits for extended formatting
// like blink, bold, wide, high
// foreground/background  values will be
// sufficient... they retain full informaiton
//
typedef struct format_info_tag
{
   /* bit-packed flags indicating the type of format information that is applied to this segment.*/
	struct {
		// extended operation from enumeration above...
		// might shrink if more attributes are desired...
		// if many more are needed, one might consider
      // adding FONT!
     /* this segment uses the prior foreground, not its own. */
		BIT_FIELD prior_foreground : 1;
     /* this segment uses the prior background, not its own. */
		BIT_FIELD prior_background : 1;
     /* this segment uses the default foreground, not its own. */
		BIT_FIELD default_foreground : 1;
      /* this segment uses the default background, not its own. */
		BIT_FIELD default_background : 1;
      /* the foreground color of this segment (0-16 standard console text [ANSI text]) */
		BIT_FIELD foreground : 4;
      /* the background color of this segment (0-16 standard console text [ANSI text]) */
		BIT_FIELD background : 4;
      /* a bit indicating the text should blink if supported */
		BIT_FIELD blink : 1;
      /* a bit indicating the foreground and background color should be reversed */
		BIT_FIELD reverse : 1;
		// usually highly is bolder, perhaps it's
      // a highlighter effect and changes the background
		BIT_FIELD highlight : 1;
		// this is double height modifications to the font...
		BIT_FIELD tall : 1;
      // this is thicker characters...
		BIT_FIELD bold : 1;
      // draw a line under the text...
		BIT_FIELD underline : 1;
		// strike through - if able, draw a line right
		// through the middle of the text... maybe
		// it's a wiggly scribble line?  maybe that
      // could be extended again?
		BIT_FIELD strike : 1;
      // text is drawn wide (printer kinda font?)
		BIT_FIELD wide : 1;
       // this is pretty common......
		BIT_FIELD italic : 1;
		// --
		// these flags are free, but since we already have text segments
		// and I'm bringing in consoles, perhaps we should consider using
		// this to describe captions, but provide the api layer for CTEXTSTR
		// --
		// position data remains constant.
		// text is mounted at the top/left of the
		// first character... (unless center, then
		// the position specifies the middle of the text
		// draw vertical instead of horizontal
		BIT_FIELD bVertical:1;
		// draw opposite/upside down from normal
		// vertical/down, right/left upside down if not centered
		// if centered, the text pivots around position.
		BIT_FIELD bInvert:1;
		// 0 = default alignment 1 = left, 2 = center 3 = right
		// 0 is not set, the flag set in the lower 32 bit flags
		// is not needed any longer.... anything non zero
		// is that operation to apply.
		BIT_FIELD bAlign:2;
      /* format op indicates one of the enum FORMAT_OPS applies to this segment */
		BIT_FIELD format_op : 7;
	} flags;
	// if x,y are valid segment will have TF_POSFORMAT set...
	union {
		/* Coordinate information attached to a text segment. */
		/* Positioning specification of this text segment. with
		   basically 0 format options, position is used.
		   Position represents the distance from this segment to the
		   prior segment in count of tabs and spaces.
		   coords specifies an x,y coordinate location for the segment.
		   Usage of this union is dependant on <link text::format_info_tag::flags@1::format_op, format_op>. */
		struct {
         // Signed coordinate of this segment on a text display.  May be relative depending on format_op.
			int16_t x;
         // Signed coordinate of this segment on a text display.  May be relative depending on format_op.
			int16_t y;
		} coords;
		/* Defines the distance from the prior segment in count of tabs
		   and spaces (mostly count of spaces).                         */
		struct {
   // tabs preceed spaces....
			uint16_t tabs;
 // not sure what else to put with this...
			uint16_t spaces;
		} offset;
	} position;
} FORMAT, *PFORMAT;
 // special coordinate which is NO coordinate
#define IGNORE_CURSOR_POS -16384
/* test flag, format has position data */
#define TF_FORMATPOS (TF_FORMATABS|TF_FORMATREL|TF_FORMATEX)
/* these flags are used in PTEXT.flags member
 applications may use these flags to group expressions
 will affect the BuildLine but is not generated by library.
( TF_QUOTE, TF_SQUOTE, TF_BRACKET, TF_BRACE, TF_PAREN, and TF_TAG).
*/
enum TextFlags {
   // declared in program data.... do NOT release
 TF_STATIC    = 0x00000001,
   // double quoted string segment " "
 TF_QUOTE     = 0x00000002,
   // single quoted string ' '
 TF_SQUOTE    = 0x00000004,
   // bracketed expression []
 TF_BRACKET   = 0x00000008,
   // braced expression {}
 TF_BRACE     = 0x00000010,
   // parenthised expression ()
 TF_PAREN     = 0x00000020,
   // HTML tag like expression &lt;&gt;
 TF_TAG       = 0x00000040,
   // foreground is FORMAT_OP
 TF_FORMATEX  = 0x00000080,
   // x,y position used (relative)
 TF_FORMATREL = 0x00000100,
   // size field extually points at PTEXT
 TF_INDIRECT  = 0x00000200,
   // format position is x/y - else space count
 TF_FORMATABS = 0x00000800,
   // set during burst for last segment...
 TF_COMPLETE  = 0x00001000,
   // set for non-text variable
 TF_BINARY    = 0x00002000,
   // on release release indrect also...
 TF_DEEP      = 0x00004000,
   // set on first segment to send to omit lead \r\n
 TF_NORETURN  = 0x00008000,
// these values used originally for ODBC query construction....
// these values are flags stored on the indirect following a value
// label...
// Low bound of value...
  TF_LOWER     = 0x00010000,
// these values used originally for ODBC query construction....
// these values are flags stored on the indirect following a value
// label...
  // Upper bound of a value...
  TF_UPPER     = 0x00020000,
// these values used originally for ODBC query construction....
// these values are flags stored on the indirect following a value
// label...
// boundry may be ON this value...
 TF_EQUAL     = 0x00040000,
   // this segment is not a permanent part (SubstToken)
 TF_TEMP      = 0x00080000,
  // this is something special do not treat as text indirect.
 TF_APPLICATION = 0x00100000,
};
//--------------------------------------------------------------------------
// flag combinatoin which represents actual data is present even with 0 size
// extended format operations (position, ops) are also considered data.
#define IS_DATA_FLAGS (TF_QUOTE|TF_SQUOTE|TF_BRACKET|TF_BRACE|                              TF_PAREN|TF_TAG|TF_FORMATEX|TF_FORMATABS|TF_FORMATREL)
// this THis defines/initializes the data part of a PTEXT/TEXT structure.
// used with DECLTEXTSZTYPE
#define DECLDATA(name,length) struct {size_t size; TEXTCHAR data[length];} name
#define DECLTEXTSZTYPE( name, size ) struct {    uint32_t flags;    struct text_segment_tag *Next, *Prior;    FORMAT format;    DECLDATA(data, size); } name
/* A macro to declare a structure which is the same physically
   as a PTEXT, (for declaring static buffers). Has to be cast to
   (PTEXT) is used. Is defined as a size, but no string content.
   Parameters
   name :  name of the variable to create
   size :  size of the static text element. (0 content)          */
#define DECLTEXTSZ( name, size ) DECLTEXTSZTYPE( name,(size) )	 = { TF_STATIC, NULL, NULL, {{1,1  ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}} }
/* Defines an initializer block which can be used to satisfy a
   TEXT elemnt of a structure
   Parameters
   str :  string content of the TEXT
   Example
   <code lang="c++">
   TEXT something = DEFTEXT( "abc" );
   </code>                                                     */
#define DEFTEXT(str) {TF_STATIC,NULL,NULL,{{1,1}},{(sizeof(str)/sizeof(str[0]))-1,str}}
/* A macro to declare a structure which is the same physically
   as a PTEXT, (for declaring constant static strings
   basically). Has to be cast to (PTEXT) is used.
   Parameters
   name :   name of the variable to create
   value :  static string constant to initialize variable to.  */
#define DECLTEXT(name, str) static DECLTEXTSZTYPE( name, (sizeof(str)/sizeof(str[0])) ) = DEFTEXT(str)
/* Description
   A Text segment, it is based on DataBlock that has a length
   and an addtional region at the end of the structure which
   contains the text of the segment. Segments may have
   formatting attributes. Segments may be linked to other
   segments in a NEXTLINE/PRIORLINE. Segments may have indirect
   content, which may represent phrases. Sets of segments may
   represent sentence diagrams. A Pointer to a <link text::TEXT, TEXT>
   type.
   TEXT is a type I created to provide a variety of functions.
   One particular application was a common language processor,
   and I created the TEXT structure to store elements which are
   described by language. Sentences are words, and phases. A
   phrase is a set of words, but sometimes a word is a phrase.
   (sentence) = ( word ) ... (phrase ) ...
   (phrase) = (word)...
   hmm.. how to describe this.
   <code lang="c++">
   PTEXT phrase = NULL;
   SegAppend( phrase, SegCreateFromText( "Test" ) );
   </code>
   <code>
   SegAppend( phrase, SegCreateFromText( "Test" ) );
   SegAppend( phrase, SegCreateFromText( "Test" ) );
   </code>
   PTEXT segments point at other segments. A list of segments is
   a sentence. Segments can have information encoded on them
   that remove text from them. For instance, \< and \> tags
   might be removed around a phrase and stored as an attribute
   of the segment. A segment with such an attribute could be an
   indirect segment that points at a list of words which are the
   phrases in the tag.
   <code lang="c++">
   a map of two segments, and their content...
       (segment with TF_TAG) -\> (segment with TF_TAG)
             |                        |
             \+ - ("html")             + - (body) -\> (background="#000000")
   would actually expand to
      \<html\>\<body background="#000000"\>
   </code>
   See Also
   SegCreate
   burst
   TextParse
   SegAppend
   SegSubst
   SegSplit
   SegGrab
   SegDelete
   LineRelease
   BuildLine
   and also.....
   PVARTEXT                                                                  */
typedef struct text_segment_tag
{
	// then here I could overlap with pEnt .bshadow, bmacro, btext ?
   uint32_t flags;
	/* This points to the next segment in the sentence or phrase. NULL
	   if at the end of the line.                                      */
		struct text_segment_tag *Next;
	/* This points to the prior segment in the sentence or phrase. (NULL
	   if at the first segment)                                          */
		struct text_segment_tag *Prior;
	/* format is 64 bits.
      it's two 32 bit bitfields (position, expression)
	 valid if TF_FORMAT is set... */
	FORMAT format;
   /* A description of the data stored here.  It is compatible with a DATABLOCk.... */
   struct {
	   /* unsigned size; size is sometimes a pointer value...
                  this means bad thing when we change platforms... Or not, since we went to uintptr_t which is big enough for a pointer. */
		uintptr_t size;
		/* the data of the test segment
		 beginning of var data - this is created size+sizeof(TEXT) */
		   TEXTCHAR  data[1];
	} data;
} TEXT, *PTEXT;
//
// PTEXT DumpText( PTEXT somestring )
//    PTExT (single data segment with full description \r in text)
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  DumpText ( PTEXT text );
//SegCreateFromText( ".." );
// Burst, SegAppend, SegGrab
// segments are ment to be lines, the meaninful tag "TF_NORETURN" means it's part of the prior line.
//--------------------------------------------------------------------------
#define HAS_WHITESPACE(pText) ( pText && ( (pText)->format.position.offset.spaces || (pText)->format.position.offset.tabs ) )
/* A convenient macro to go from one segment in a line of text
   to the next segment.                                        */
#define NEXTLINE(line)   ((PTEXT)(((PTEXT)line)?(((PTEXT)line)->Next):(NULL)))
/* A convenient macro to go from one segment in a line of text
   to the prior segment.                                       */
#define PRIORLINE(line)  ((PTEXT)(((PTEXT)line)?(((PTEXT)line)->Prior):(NULL)))
/* Link one PTEXT segment to another. Sets one half of the links
   appropriate for usage.
   Example
   This example sets the prior pointer of 'word' to 'line'.
   <code>
   PTEXT line;
   PTEXT word;
   SETPRIORLINE( word, line );
   </code>                                                       */
#define SETPRIORLINE(line,p) ((line)?(((line)->Prior) = (PTEXT)(p)):0)
/* Link one PTEXT segment to another. Sets one half of the links
   appropriate for usage.
   Example
   This example sets the next pointer of 'line' to 'word'.
   <code lang="c#">
   PTEXT line;
   PTEXT word;
   SETNEXTLINE( line, word );
   </code>                                                       */
#define SETNEXTLINE(line,p)  ((line)?(((line)->Next ) = (PTEXT)(p)):0)
/* Sets a pointer to PTEXT to the first text segment in the
   list.                                                    */
#define SetStart(line)     for(; line && PRIORLINE(line);line=PRIORLINE(line))
/* Sets a PTEXT to the last segment that it points to.
   Parameters
   line :  segment in the line to move to the end of.
   Remarks
   Updates the variable passed to point to the last segment. */
#define SetEnd(line)      for(; line && NEXTLINE(line); line=NEXTLINE(line))
// might also check to see if pseg is an indirect - setting this size would be BAD
#define SetTextSize(pseg, sz ) ((pseg)?((pseg)->data.size = (sz )):0)
/* gets the indect segment content (if any) from a PTEXT
   segment.                                              */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  GetIndirect(PTEXT segment );
/* Get the format flags of a PTEXT.
                                    */
TYPELIB_PROC  uint32_t TYPELIB_CALLTYPE  GetTextFlags( PTEXT segment );
/* Gets the text segment length. */
TYPELIB_PROC  size_t TYPELIB_CALLTYPE  GetTextSize( PTEXT segment );
/* Gets the text of a PTEXT segment. (convert to a CTEXTSTR)
   Parameters
   segment :  segment to get the string content from         */
TYPELIB_PROC  TEXTSTR TYPELIB_CALLTYPE  GetText( PTEXT segment );
// by registering for TF_APPLICTION is set on the segment
// and flags anded with the segment flags match, the
// function is called.... the result is the actual
// segment of this - since a TF_APPLICATION is also
// TF_INDIRECT - using the size to point to some application
// defined structure instead of a PTEXT structure.
TYPELIB_PROC  void TYPELIB_CALLTYPE  RegisterTextExtension ( uint32_t flags, PTEXT(CPROC*)(uintptr_t,POINTER), uintptr_t );
// similar to GetIndirect - but results in the literal pointer
// instead of the text that the application may have registered to result with.
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE  GetApplicationPointer ( PTEXT text );
/* Used to set the content of a segment to some application
   defined value. This allows a users application to store
   chunks of data in lists of text. These external chunks are
   handled like other words.
   Parameters
   text :  this is the text segment to set application data on
   p :     this is a pointer to application data               */
TYPELIB_PROC  void TYPELIB_CALLTYPE  SetApplicationPointer ( PTEXT text, POINTER p);
/* Set segment's indirect data.
   Parameters
   segment :  pointer to a TEXT segment to set the indirect content
              of.
   data :     pointer to a PTEXT to be referenced indirectly.       */
#define SetIndirect(Seg,Where)  ( (Seg)->data.size = ((uintptr_t)(Where)-(uintptr_t)NULL) )
		/* these return 1 for more(l1&gt;l2) -1 for (l1&lt;l2) and 0 for match.
       */
TYPELIB_PROC  int TYPELIB_CALLTYPE  SameText ( PTEXT l1, PTEXT l2 );
/* A test if one PTEXT is similar to another PTEXT.
   Parameters
   l1 :  PTEXT segment one
   l2 :  PTEXT segment two
   Return Value List
   \<0 :  l1 with case insensitive comparison is less then l2
   0 :    Texts compare case insenitive match
   \>0 :  l1 with case insensitive comparison is more than l2 */
TYPELIB_PROC  int TYPELIB_CALLTYPE  LikeText ( PTEXT l1, PTEXT l2 );
/* Compares if text is like a C string. Case Sensitive.
   <b>Returns</b>
   TRUE if they are alike.
   FALSE if they are different.
   <b>Parameters</b>                                    */
TYPELIB_PROC  int TYPELIB_CALLTYPE  TextIs  ( PTEXT pText, CTEXTSTR text );
/* Compares if text is like a C string. Case insensitive (like).
   Returns
   TRUE if they are alike.
   FALSE if they are different.
   Parameters
   pText :  PTEXT segment to compare
   text :   C string buffer to compare against                   */
TYPELIB_PROC  int TYPELIB_CALLTYPE  TextLike  ( PTEXT pText, CTEXTSTR text );
/* Compares if text is like a C string. Case insensitive (like). Uses min string length for max match.
   Returns
   TRUE if they are similar (both case insensitive using shorter of the strings for maxlen).
   FALSE if they are different.
   Parameters
   pText :  PTEXT segment to compare
   text :   C string buffer to compare against                   */
TYPELIB_PROC  int TYPELIB_CALLTYPE  TextSimilar  ( PTEXT pText, CTEXTSTR text );
//#define SameText( l1, l2 )  ( strcmp( GetText(l1), GetText(l2) ) )
#define textmin(a,b) ( (((a)>0)&&((b)>0))?(((a)<(b))?(a):(b)):(((a)>0)?(a):((b)>0)?(b):0) )
#ifdef __LINUX__
#  include <strings.h>
/* windows went with stricmp() and strnicmp(), whereas linux
 went with strcasecmp() and strncasecmp()                  */
#  define strnicmp strncasecmp
/* windows went with stricmp() and strnicmp(), whereas linux
   went with strcasecmp() and strncasecmp()                  */
#  define stricmp strcasecmp
#endif
/* Copy segment formatting to another segment... */
TYPELIB_PROC  void TYPELIB_CALLTYPE  SegCopyFormat( PTEXT to_this, PTEXT copy_this );
/* Create a text segment of sepecified size; inclues one more character for NUL terminator */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegCreateEx( size_t nSize DBG_PASS );
/* Create a PTEXT with specified number of character capacity.
   Example
   <code lang="c#">
   PTEXT text = SegCreate( 10 );
   </code>                                                     */
#define SegCreate(s) SegCreateEx(s DBG_SRC)
/* \ \
   See Also
   <link DBG_PASS>
   <link SegCreateFromText> */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegCreateFromTextEx( CTEXTSTR text DBG_PASS );
/* Creates a PTEXT segment from a string.
   Example
   <code lang="c++">
   PTEXT line = SegCreateFromText( "Around the world in a day." );
   </code>                                                         */
#define SegCreateFromText(t) SegCreateFromTextEx(t DBG_SRC)
/* \ \
   See Also
   <link DBG_PASS>
   <link SegCreateFromChar> */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegCreateFromCharLenEx( const char *text, size_t len DBG_PASS );
/* \ \
   See Also
   <link DBG_PASS>
   <link SegCreateFromChar> */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegCreateFromCharEx( const char *text DBG_PASS );
/* Creates a PTEXT segment from a string.
   Example
   <code lang="c++">
   PTEXT line = SegCreateFromChar( "Around the world in a day." );
   </code>                                                         */
#define SegCreateFromChar(t) SegCreateFromCharEx(t DBG_SRC)
/* \ \
   See Also
   <link DBG_PASS>
   <link SegCreateFromChar> */
#define SegCreateFromCharLen(t,len) SegCreateFromCharLenEx((t),(len) DBG_SRC)
/* \ \
   See Also
   <link DBG_PASS>
   <link SegCreateFromWide> */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegCreateFromWideLenEx( const wchar_t *text, size_t len DBG_PASS );
/* \ \
   See Also
   <link DBG_PASS>
   <link SegCreateFromWide> */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegCreateFromWideEx( const wchar_t *text DBG_PASS );
/* Creates a PTEXT segment from a string.
   Example
   <code lang="c++">
   PTEXT line = SegCreateFromWideLen( L"Around the world in a day.", 26 );
   </code>                                                         */
#define SegCreateFromWideLen(t,len) SegCreateFromWideLenEx((t),(len) DBG_SRC)
/* \ \
   See Also
   <link DBG_PASS>
   <link SegCreateFromWide> */
#define SegCreateFromWide(t) SegCreateFromWideEx(t DBG_SRC)
/* \ \
   See Also
   <link DBG_PASS>
   <link SegCreateIndirect> */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegCreateIndirectEx( PTEXT pText DBG_PASS );
/* Creates a text segment that refers to the parameter
   indirectly. The new segment is not really a clone, but a
   reference of the original PTEXT.
   Example
   <code lang="c#">
   PTEXT phrase = SegCreateIndirect( SegAppend( SegCreateFromText( "Hello" )
                                              , SegCreateFromText( "World" ) ) );
   </code>
   The resulting phrase is a single segment with no prior or
   next, but its content is "HelloWorld" if it was passed to
   buildline... it's go the content of the two text segments
   linked together, but not in its buffer. It is actually a 0
   length buffer for a TEXT segment.
                                                                                  */
#define SegCreateIndirect(t) SegCreateIndirectEx(t DBG_SRC)
/* \ \
   See Also
   <link DBG_PASS>
   <link SegDuplicate> */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegDuplicateEx( PTEXT pText DBG_PASS);
/* This duplicates a specific segment. It duplicates the first
   segment of a string. If the segment has indirect data, then
   the first segment of the indirect data is duplicated.       */
#define SegDuplicate(pt) SegDuplicateEx( pt DBG_SRC )
/* Duplicates a linked list of segments.
   Duplicates the structure of a line. The resulting line is an
   exact duplicate of the input line. All segments linked in
   exactly the same sorts of ways.
   Parameters
   line :  list of segments to duplicate                        */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  LineDuplicateEx( PTEXT pText DBG_PASS );
/* <combine sack::containers::text::LineDuplicateEx@PTEXT pText>
   \ \                                                           */
#define LineDuplicate(pt) LineDuplicateEx(pt DBG_SRC )
/* \ \
   See Also
   <link DBG_PASS>
   <link TextDuplicate> */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  TextDuplicateEx( PTEXT pText, int bSingle DBG_PASS );
/* Duplicate the whole string of text to another string with
   exactly the same content.                                 */
#define TextDuplicate(pt,s) TextDuplicateEx(pt,s DBG_SRC )
/* \ \
   See Also
   <link DBG_PASS>
   <link SegCreateFromInt> */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegCreateFromIntEx( int value DBG_PASS );
/* Creates a text segment from a 64 bit integer.
   Example
   <code>
   PTEXT number = SegCreateFromInt( 3314 );
   </code>                                       */
#define SegCreateFromInt(v) SegCreateFromIntEx( v DBG_SRC )
/* Converts an integer to a PTEXT segment.
   Parameters
   _64bit_value :  integer value to convert to a PTEXT segment. */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegCreateFrom_64Ex( int64_t value DBG_PASS );
/* Create a text segment from a uint64_t bit value. (long long int) */
#define SegCreateFrom_64(v) SegCreateFrom_64Ex( v DBG_SRC )
/* \ \
   See Also
   <link DBG_PASS>
   <link SegCreateFromFloat> */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegCreateFromFloatEx( float value DBG_PASS );
/* Creates a text segment from a floating point value. Probably
   uses something like '%g' to format output. Fairly limited.
   Example
   <code lang="c++">
   PTEXT short_PI = SegCreateFromFloat( 3.14 );
   </code>                                                      */
#define SegCreateFromFloat(v) SegCreateFromFloatEx( v DBG_SRC )
/* Appends a list of segments to an existing list of segments. This
   assumes that the additional segment is referncing the head of
   the segment list.
   Parameters
   source :  source list to add to
   other :   additional segments to add to source.                  */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegAppend   ( PTEXT source, PTEXT other );
/* Inserts a segment before another segment.
   Parameters
   what :    what to insert into the list
   before :  insert the segments before this segment
   Returns
   The parameter 'what'.                             */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegInsert   ( PTEXT what, PTEXT before );
/* This expands a segment by a number of characters.
   Parameters
   PTEXT :  the segment to expand
   int :    count of character to expand by
   Returns
   A pointer to a new segment that is bigger, but has the same
   existing content.                                           */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegExpandEx (PTEXT source, INDEX nSize DBG_PASS );
/* <combine sack::containers::text::SegExpandEx@PTEXT@INDEX nSize>
   \ \                                                             */
#define SegExpand(s,n) SegExpandEx( s,n DBG_SRC )
/* Release a linked list of PTEXT segments.
   Parameters
   segments :  a segment in a list of segments to delete, first
               this routine goes to the start of the segment
               list, and then deletes all segments in the list.
   DBG_PASS :  debug file and line information                  */
TYPELIB_PROC  void TYPELIB_CALLTYPE   LineReleaseEx (PTEXT line DBG_PASS );
/* Release a line of text.
   A line may be a single segment.
   This is the proper way to dispose of PTEXT segments.
   Any segment in the line may be passed, the first segment is
   found, and then all segments in the line are deleted.       */
#define LineRelease(l) LineReleaseEx(l DBG_SRC )
/* \ \
   <b>See Also</b>
   <link DBG_PASS>
   <link SegRelease> */
TYPELIB_PROC  void TYPELIB_CALLTYPE  SegReleaseEx( PTEXT seg DBG_PASS );
/* Release a single segment. UNSAFE. Does not respect that it is
   in a list.
   See Also
   <link LineRelease>                                            */
#define SegRelease(l) SegReleaseEx(l DBG_SRC )
/* Adds a part of input to the segment list of output.
   Parameters
   output\ :   the segment list to append to.
   input\ :    the input buffer to append from
   offset :    starting offset in 'input' to start from
   length :    how much from 'offset' in input to append as a new
               segment to output.
   DBG_PASS :  \file and line debugging information               */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegConcatEx   (PTEXT output,PTEXT input,int32_t offset,size_t length DBG_PASS);
/* <combine sack::containers::text::SegConcatEx@PTEXT@PTEXT@int32_t@size_t length>
   looks like it takes a piece of one segment and appends it to
   another....
   Needs More research to document correctly and exemplify.                     */
#define SegConcat(out,in,ofs,len) SegConcatEx(out,in,ofs,len DBG_SRC)
/* Removes a segment from a list of segments. Links what was
   prior and what was after together. Sets both next and prior
   of the segment unlinked to NULL.
   Example
   <code lang="c++">
   SegUnlink( segment );
   </code>
   Returns
   The segment passed.                                         */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegUnlink   (PTEXT segment);
/* Breaks a list of PTEXT segments at the specified segment and
   \returns a segment that was before the specified.
   Parameters
   segment :  segment to break the chain at
   Returns
   Any existing segment before the segment to break at.
   Example
   <code lang="c++">
   {
      PTEXT segs;
      PTEXT breakat;
      PTEXT leftover;
		&#47;* ... segs gets populated with some segments ... *&#47;
      breakat = NEXTLINE( segs );
   </code>
   <code>
      breakat = NEXTLINE( segs );
      leftover = segbreak( breakat );
      // now breakat begins a new chain of segments
      // leftover is the segment that was just before breakat
      SegStart( leftover );  // leftover would be equal to segs...
   }
   </code>                                                         */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegBreak    (PTEXT segment);
/* Removes a segment from a list. It also releases the segment.
    Example
    <code lang="c#">
    SegDelete( segment );
    </code>
    the result is NULL;                                          */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegDelete   (PTEXT segment);
/* removes segment from any list it might be in, returns
   segment.                                              */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegGrab     (PTEXT segment);
/* Substitute one PTEXT segment for another in a list of PTEXT
   segments.
   Parameters
   _this :  This is the segment to remove
   that :   This is the segment to subustitute with. This may be
            a list of segments, and it is linked in from the
            first segment to the prior to '_this' and the last to
            the next after '_this'
   Returns
   \Returns the '_this' that was substituted.                     */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegSubst    ( PTEXT _this, PTEXT that );
/* \ \
   See Also
   <link DBG_PASS>
   <link SegSplit> */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  SegSplitEx( PTEXT *pLine, INDEX nPos DBG_PASS);
/* Split a PTEXT segment.
   Example
   \ \
   <code lang="c++">
   PTEXT result = SegSplit( &amp;old_string, 5 );
   </code>
   Returns
   PTEXT new_string;
   Remarks
   the old string segment is split at the position indicated. The
   pointer to the old segment is modified to point to now two
   segments linked dynamically, each part of the segment after
   the split. If the index is beyond the bounds of the segment,
   the segment remains unmodified.                                */
#define SegSplit(line,pos) SegSplitEx( line, pos DBG_SRC )
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  FlattenLine ( PTEXT pLine );
/* Create a highest precision signed integer from a PTEXT. */
TYPELIB_PROC  int64_t TYPELIB_CALLTYPE  IntCreateFromSeg( PTEXT pText );
/* Converts a text to the longest precision signed integer
   value.
     allows +/- leadin ([-*]|[+*])*
     supports 0x### (hex), 0b#### (binary), 0o#### (octal), 0### (octal)
	 decimal 1-9[0-9]*
	 buggy implementation supports +/- inline continue number and are either ignored(+)
	 or changes the overall sign of the number(-).  A Decimal definatly ends the number.
	 And octal/binary digits aren't checked for range, so 8/9 will over-flow in octal,
	 and 2-9 overflow to upper bits in octal...
	    0b901090 // would be like   0b 10100110    0b1001 +  010 + 1001<<3 + 0
   */
TYPELIB_PROC  int64_t TYPELIB_CALLTYPE  IntCreateFromText( CTEXTSTR p );
/* Converts a text to the longest precision signed integer
   value.  Does the work of IntCreateFromText.
   IntCreateFromTextRef updates the pointer passed by reference so
   the pointer ends at the first character after the returned number.
   */
TYPELIB_PROC  int64_t TYPELIB_CALLTYPE  IntCreateFromTextRef( CTEXTSTR *p_ );
/* Create a high precision floating point value from PTEXT
   segment.                                                */
TYPELIB_PROC  double TYPELIB_CALLTYPE  FloatCreateFromSeg( PTEXT pText );
/* Create a high precision floating point value from text
   string.                                                */
TYPELIB_PROC  double TYPELIB_CALLTYPE  FloatCreateFromText( CTEXTSTR p, CTEXTSTR *pp );
//
// IsSegAnyNumber returns 0 if no, 1 if is int, 2 if is float
//   if pfNumber or piNumber are available then the text pointer
//   will be updated to the next segment after what was used to resolve
//   the number.
//   bUseAllSegs is for testing pTexts which are indirect, such that
//      only all segments within the indirect segment will result valid.
//   pfNumber and piNumber may be passed as NULL, and the function can still
// be used to determine ifnumber
//   the number resulting in the values pointed to will be filled in
//    with (*pfNumber)=FltCreateFromSeg(p) (or Int as appropriate)
//
//#define IsNumber(p) IsSegAnyNumberEx( &(p), NULL, NULL, NULL, 0 )
#define IsIntNumber(p, pint) IsSegAnyNumberEx( &(p), NULL, pint, NULL, 0 )
/* Tests a PTEXT segment to see if it might be a floating point
   number.                                                      */
#define IsFltNumber(p, pflt) IsSegAnyNumberEx( &(p), pflt, NULL, NULL, 0 )
/* Tests the content of a PTEXT to see if it might be a number.
   Parameters
   ppText :       pointer to PTEXT to check
   pfNumber :     pointer to double to get result of number it's
                  a float
   piNumber :     pointer to a signed 64 bit value to get the
                  \result if it's not a float.
   pbIsInt :      point to a integer \- receives boolean result
                  if the segment was an integer is TRUE else it's
                  a double.
   bUseAllSegs :  if TRUE, use all the segments starting with the
                  first, and update the pointer to the next
                  stgment. If false, use only the first segment. if
                  uses all segments, it must also use ALL
                  segments to get the number.
   Returns
   0 if not a number or fails.
   1 if a valid conversion took place.                              */
TYPELIB_PROC  int TYPELIB_CALLTYPE  IsSegAnyNumberEx ( PTEXT *ppText, double *pfNumber, int64_t *piNumber, int *pbIsInt, int bUseAllSegs );
/* <combine sack::containers::text::IsSegAnyNumberEx@PTEXT *@double *@int64_t *@int *@int>
   \ \                                                                                  */
#define IsSegAnyNumber(pptext, pfNum, piNum, pbIsInt) IsSegAnyNumberEx( pptext, pfNum, piNum, pbIsInt, 0 )
/* \Returns the amount of space required to store this segment,
   and all indirect statements it contains.
   Parameters
   segment :   segment to measure
   position :  starting position in the segment to measure from
   nTabSize :  how big tabs are supposed to be
   tabs :      list of tab positions (for arbitrary tab
               positioning\- table column alignment?)           */
TYPELIB_PROC  INDEX TYPELIB_CALLTYPE  GetSegmentSpaceEx ( PTEXT segment, INDEX position, int nTabs, INDEX *tabs);
/* \Returns the amount of space required to store this segment,
   and all indirect statements it contains.
   Parameters
   segment :   segment to measure
   position :  starting position in the segment to measure
               from
   nTabSize :  how big tabs are supposed to be                  */
TYPELIB_PROC  INDEX TYPELIB_CALLTYPE  GetSegmentSpace ( PTEXT segment, INDEX position, int nTabSize );
/* Simlar to getsegment space... */
TYPELIB_PROC  INDEX TYPELIB_CALLTYPE  GetSegmentLengthEx ( PTEXT segment, INDEX position, int nTabs, INDEX *tabs );
/* \Returns the length of a single PTEXT segment.
   Parameters
   segment :   segment to measure
   position :  string position in the string to measure
   nTabSize :  how many characters a tab is supposed to be. */
TYPELIB_PROC  INDEX TYPELIB_CALLTYPE  GetSegmentLength ( PTEXT segment, INDEX position, int nTabSize );
/* Measure the length of a list of segments (combined length of
   all linked segments)                                         */
TYPELIB_PROC  INDEX TYPELIB_CALLTYPE  LineLengthExEx( PTEXT pt, LOGICAL bSingle, int nTabsize, PTEXT pEOL );
TYPELIB_PROC  INDEX TYPELIB_CALLTYPE  LineLengthExx( PTEXT pt, LOGICAL bSingle,PTEXT pEOL );
/* <combine sack::containers::text::LineLengthExEx@PTEXT@LOGICAL@int@PTEXT>
   \ \                                                                      */
#define LineLengthExx(pt,single,eol) LineLengthExEx( pt,single,8,eol)
/* \ \
   Parameters
   Text segment :  PTEXT line or segment to get the length of
   single :        boolean, if set then only a single segment is
                   measured, otherwise all segments from this to
                   the end are measured.                         */
#define LineLengthEx(pt,single) LineLengthExx( pt,single,NULL)
/* Computes the length of characters in a line, if all segments
   in the line are flattened into a single word.                */
#define LineLength(pt) LineLengthEx( pt, FALSE )
/* Collapses an indirect segment or a while list of segments
   into a single segment with content expanded. When passed to
   things like TextParse and Burst, segments have their
   positioning encoded to counters for tabs and spaces; the
   segment itself contains only text without whitespace. Buildline
   expands these segments into their plain text representation.
   Parameters
   pt :        pointer to a PTEXT segment.
   bSingle :   if TRUE, build only the first segment. If the
               segment is indirect, builds entire content of
               indirect.
   nTabsize :  how wide tabs are. When written into a line, tabs
               are written as spaces. (maybe if 0, tabs are
               emitted directly?)
   pEOL :      the segment to use to represent an end of line. Often
               this is a SegCreate(0) segment.                       */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  BuildLineExEx( PTEXT pt, LOGICAL bSingle, int nTabsize, PTEXT pEOL DBG_PASS );
/* Collapses an indirect segment or a while list of segments
into a single segment with content expanded. When passed to
things like TextParse and Burst, segments have their
positioning encoded to counters for tabs and spaces; the
segment itself contains only text without whitespace. Buildline
expands these segments into their plain text representation.
Parameters
pt :        pointer to a PTEXT segment.
bSingle :   if TRUE, build only the first segment. If the
segment is indirect, builds entire content of
indirect.
pEOL :      the segment to use to represent an end of line. Often
this is a SegCreate(0) segment.                       */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  BuildLineExx( PTEXT pt, LOGICAL bSingle, PTEXT pEOL DBG_PASS );
/* <combine sack::containers::text::BuildLineExEx@PTEXT@LOGICAL@int@PTEXT pEOL>
\ \                                                                          */
#define BuildLineExx(from,single,eol) BuildLineExEx( from,single,8,NULL DBG_SRC )
/* <combine sack::containers::text::BuildLineExEx@PTEXT@LOGICAL@int@PTEXT pEOL>
   \ \                                                                          */
#define BuildLineEx(from,single) BuildLineExEx( from,single,8,NULL DBG_SRC )
/* <combine sack::containers::text::BuildLineExEx@PTEXT@LOGICAL@int@PTEXT pEOL>
   \ \
    Flattens all segments in a line to a single segment result.
*/
#define BuildLine(from) BuildLineExEx( from, FALSE,8,NULL DBG_SRC )
//
// text parse - more generic flavor of burst.
//
//static CTEXTSTR normal_punctuation=WIDE("\'\"\\({[<>]}):@%/,;!?=*&$^~#`");
// filter_to_space " \t"
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  TextParse ( PTEXT input, CTEXTSTR punctuation, CTEXTSTR filter_tospace, int bTabs, int bSpaces  DBG_PASS );
/* normal_punctuation=WIDE("'"\\({[\<\>]}):@%/,;!?=*&amp;$^~#`");
   Process a line of PTEXT into another line of PTEXT, but with
   words parsed as appropriate for common language.
   Parameters
   input\ :  pointer to a list of PTEXT segments to parse.
   Remarks
   Burst is a simple method of breaking a sentence into its word
   and phrase parts. It collapses space and tabs before words
   into the word. Any space representation is space preceeding
   the word. Sentences are also broken on any punctuation.
   "({[\<\>]})'";;.,/?\\!@#$%^&amp;*=" for instances. + and - are
   treated specially if they prefix numbers, otherwise they are
   also punctuation. Also groups of '.' like '...' are kept
   together. if the '.' is in a number, it is stored as part of
   the number. Otherwise a '.' used in an abbreviation like P.S.
   will be a '.' with 0 spaces followed by a segment also with 0
   spaces. (unless it's the lsat one)
   so initials are encoded badly.
   Bugs
   There is an exploit in the parser such that . followed by a
   number will cause fail to break into seperate words. This is
   used by configuration scripts to write binary blocks, and
   read them back in, having the block parsed into a segment
   correctly.
   See Also
   <link sack::containers::text::TextParse@PTEXT@CTEXTSTR@CTEXTSTR@int@int bSpaces, TextParse> */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  burstEx( PTEXT input DBG_PASS);
/* <combine sack::containers::text::burstEx@PTEXT input>
   \ \                                                   */
#define burst( input ) burstEx( (input) DBG_SRC )
/* Compares a couple lists of text segments.
   Parameters
   pt1 :      pointer to a phrase
   single1 :  use only the first word, not the whole phrase
   pt2 :      pointer to a phrase
   single2 :  use only the first segment, not the whole phrase
   bExact :   if FALSE, match case insensitive, otherwise match
              exact case.                                       */
TYPELIB_PROC  int TYPELIB_CALLTYPE  CompareStrings( PTEXT pt1, int single1
                            , PTEXT pt2, int single2
                            , int bExact );
/* This removes indirect segments, replacing them with their
   indirect content.
   Parameters
   pLine :  pointer to a PTEXT segment list to flatten.      */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  FlattenLine ( PTEXT pLine );
/* Steps through a linked list of segments, just a convenient
   for loop wrapper.                                          */
#define FORALLTEXT(start,var)  for(var=start;var; var=NEXTLINE(var))
/* returns number of characters filled into output.  Output needs to be at maximum 6 chars */
TYPELIB_PROC int TYPELIB_CALLTYPE ConvertToUTF8( char *output, TEXTRUNE rune );
/* returns number of characters filled into output.  Output needs to be at maximum 6 chars;  if overlong is set
   characters are deliberatly padded to be overlong */
TYPELIB_PROC int TYPELIB_CALLTYPE ConvertToUTF8Ex( char *output, TEXTRUNE rune, LOGICAL overlong );
/* returns number of wchar filled into output.  Output needs to be at maximum 2 wchar. */
TYPELIB_PROC int TYPELIB_CALLTYPE ConvertToUTF16( wchar_t *output, TEXTRUNE rune );
TYPELIB_PROC TEXTRUNE TYPELIB_CALLTYPE GetUtfChar( const char **from );
TYPELIB_PROC TEXTRUNE TYPELIB_CALLTYPE GetUtfCharIndexed( const char *from, size_t *index, size_t length );
TYPELIB_PROC TEXTRUNE TYPELIB_CALLTYPE GetPriorUtfChar( const char *start, const char **from );
TYPELIB_PROC TEXTRUNE TYPELIB_CALLTYPE GetPriorUtfCharIndexed( const char *from, size_t *index );
TYPELIB_PROC TEXTRUNE TYPELIB_CALLTYPE GetUtfCharW( const wchar_t **from );
TYPELIB_PROC TEXTRUNE TYPELIB_CALLTYPE GetUtfCharIndexedW( const wchar_t *from, size_t *index );
TYPELIB_PROC TEXTRUNE TYPELIB_CALLTYPE GetPriorUtfCharW( const wchar_t *start, const wchar_t **from );
TYPELIB_PROC TEXTRUNE TYPELIB_CALLTYPE GetPriorUtfCharIndexedW( const wchar_t *from, size_t *index );
TYPELIB_PROC size_t TYPELIB_CALLTYPE GetDisplayableCharacterCount( const char *string, size_t max_bytes );
TYPELIB_PROC CTEXTSTR TYPELIB_CALLTYPE GetDisplayableCharactersAtCount( const char *string, size_t character_index );
TYPELIB_PROC size_t TYPELIB_CALLTYPE  GetDisplayableCharacterBytes( const char *string, size_t character_count );
/* You Must Deallocate the result */
TYPELIB_PROC char * TYPELIB_CALLTYPE WcharConvert_v2 ( const wchar_t *wch, size_t len, size_t *outlen DBG_PASS );
/* You Must Deallocate the result */
TYPELIB_PROC  char * TYPELIB_CALLTYPE  WcharConvertExx ( const wchar_t *wch, size_t len DBG_PASS );
/* You Must Deallocate the result */
TYPELIB_PROC  char * TYPELIB_CALLTYPE  WcharConvertEx ( const wchar_t *wch DBG_PASS );
/* <combine sack::containers::text::WcharConvertExx@wchar_t *@size_t len>
   \ \                                                                    */
#define WcharConvertLen(s,len) WcharConvertExx(s, len DBG_SRC )
/* <combine sack::containers::text::WcharConvertExx@wchar_t *@size_t len>
   \ \                                                                    */
#define WcharConvert(s) WcharConvertEx(s DBG_SRC )
/* You Must Deallocate the result */
TYPELIB_PROC wchar_t * TYPELIB_CALLTYPE CharWConvertExx ( const char *wch, size_t len DBG_PASS );
/* Convert wchar_t strings to char strings.
   Parameters
   string :    wchar_t string to convert
   DBG_PASS :  debug file and line information
   Returns
   A char * string. This string must be Release()'ed or
   Deallocate()'ed by the user.                         */
TYPELIB_PROC wchar_t * TYPELIB_CALLTYPE CharWConvertEx ( const char *wch DBG_PASS );
/* <combine sack::containers::text::CharWConvertExx@char *@size_t len>
   \ \                                                                 */
#define CharWConvertLen(s,len) CharWConvertExx(s,len DBG_SRC )
/* <combine sack::containers::text::CharWConvertExx@char *@size_t len>
   \ \                                                                 */
#define CharWConvert(s) CharWConvertEx(s DBG_SRC )
//--------------------------------------------------------------------------
/* This is a string collector type.  It has an interface to be able to vtprintf( vartext, "format string", ... ); which appends the specified string to the collected text.
  Example
   PVARTEXT pvt = VarTextCreate();
   vtprintf( pvt, "hello world!" );
   {
      PTEXT text = VarTextGet( pvt );
	  printf( "Text is : %s(%d)", GetText( text ), GetTextSize( text ) );
	  LineRelease( text );
   }
   VarTextDestroy( &pvt );
   */
typedef struct vartext_tag *PVARTEXT;
/* Creates a variable text collector. Allows specification of
   initial size and amount to expand by. SQL Command line sample
   utility uses this and allocates like 10,000 initial and sets
   expand as 40,000, because it expects to build very large
   strings, and expansion of 32 at a time is ludicrous; if the
   space required is more than the expansion factor, then it is
   expanded by the amount required plus the expansion factor.
   Parameters
   initial :   amount of initial buffer
   exand_by :  how much to expand the buffer by when more room
               is needed
   DBG_PASS :  debug file and line parameters.                   */
TYPELIB_PROC  PVARTEXT TYPELIB_CALLTYPE  VarTextCreateExEx ( uint32_t initial, uint32_t expand DBG_PASS );
/* <combine sack::containers::text::VarTextCreateExEx@uint32_t@uint32_t expand>
   \ \                                                                */
#define VarTextCreateExx(i,e) VarTextCreateExEx(i,e DBG_SRC )
/* <combine sack::containers::text::VarTextCreateExEx@uint32_t@uint32_t expand>
   Creates a variable text collector. Default initial size and
   expansion is 0 and 32.
                                                                      */
TYPELIB_PROC  PVARTEXT TYPELIB_CALLTYPE  VarTextCreateEx ( DBG_VOIDPASS );
/* The simplest, most general way to create a PVARTEXT
   collector. The most extended vartext creator allows
   specification of how long the initial buffer is, and how much
   the buffer expands by when required. This was added to
   optimize building HUGE SQL queries, working withing 100k
   buffers that expanded by 50k at a time was a lot less
   operations than expanding 32 bytes or something at a time.    */
#define VarTextCreate() VarTextCreateEx( DBG_VOIDSRC )
/* Empties and destroys all resources associated with the
   variable text collector.
   Parameters
   pvt * :     address of a PVARTEXT reference to destroy. Sets
               the pointer to NULL when it's destroyed.
   DBG_PASS :  debugging file and line parameters
   Example
   <code lang="c++">
   {
      PVARTEXT pvt = VarTextCreate();
      VarTextDestroy( &amp;pvt );
   }
   void Function( int something DBG_PASS )
   {
      pvt = VarTextCreateEx( DBG_RELAY );
      VarTextDestroyEx( &amp;pvt DBG_RELAY );
   }
   </code>
   C++ Syntax
   \ \                                                          */
TYPELIB_PROC  void TYPELIB_CALLTYPE  VarTextDestroyEx ( PVARTEXT* DBG_PASS );
/* Destroy a VarText collector. */
#define VarTextDestroy(pvt) VarTextDestroyEx( pvt DBG_SRC )
/* \Internal function - used to initialize a VARTEXT structure. */
TYPELIB_PROC  void TYPELIB_CALLTYPE  VarTextInitEx( PVARTEXT pvt DBG_PASS);
/* Probably should not be exported. Initializes a VARTEXT
   structure to prepare it for subsequent VarText operations. */
#define VarTextInit(pvt) VarTextInitEx( (pvt) DBG_SRC )
/* Empties a PVARTEXT structure.
   Parameters
   pvt :  PVARTEXT to empty.     */
TYPELIB_PROC  void TYPELIB_CALLTYPE  VarTextEmptyEx( PVARTEXT pvt DBG_PASS);
/* <combine sack::containers::text::VarTextEmptyEx@PVARTEXT pvt>
   \ \                                                           */
#define VarTextEmpty(pvt) VarTextEmptyEx( (pvt) DBG_SRC )
/* Add a single character to a vartext collector.
   Note
   \ \
   Parameters
   pvt :       PVARTEXT to add character to
   c :         character to add
   DBG_PASS :  optional debug information         */
TYPELIB_PROC  void TYPELIB_CALLTYPE  VarTextAddCharacterEx( PVARTEXT pvt, TEXTCHAR c DBG_PASS );
TYPELIB_PROC  void TYPELIB_CALLTYPE  VarTextAddRuneEx( PVARTEXT pvt, TEXTRUNE c, LOGICAL overlong DBG_PASS );
/* Adds a single character to a PVARTEXT collector.
   Example
   <code lang="c++">
   PVARTEXT pvt = VarTextCreate();
   VarTextAddCharacter( pvt, 'a' );
   </code>                                          */
#define VarTextAddCharacter(pvt,c) VarTextAddCharacterEx( (pvt),(c) DBG_SRC )
/* Adds a single rune to a PVARTEXT collector. (may be multiple characters convert to UTF8)
   Example
   <code lang="c++">
   PVARTEXT pvt = VarTextCreate();
   VarTextAddRune( pvt, 'a' );
   </code>                                          */
#define VarTextAddRune(pvt,c) VarTextAddRuneEx( (pvt),(c), FALSE DBG_SRC )
/* Adds a length of data to the vartext. This allows strings
   with nuls included to be added.
   Parameters
   pvt :       PVARTEXT to add data to
   block :     pointer to data to add
   size :      length of data block to add
	DBG_PASS :  optional file and line parameters             */
#define VARTEXT_ADD_DATA_NULTERM ((size_t)0xFF000000)
TYPELIB_PROC  void TYPELIB_CALLTYPE  VarTextAddDataEx( PVARTEXT pvt, CTEXTSTR block, size_t length DBG_PASS );
/* Adds a single character to a PVARTEXT collector.
   Example
   <code lang="c++">
   PVARTEXT pvt = VarTextCreate();
   VarTextAddData( pvt, "test one", 8 );
   </code>                                          */
#define VarTextAddData(pvt,block,length) VarTextAddDataEx( (pvt),(block),(length) DBG_SRC )
/* Commits the currently collected text to segment, and adds the
   segment to the internal line accumulator.
		 returns true if any data was added...
       move any collected text to commit... */
TYPELIB_PROC  LOGICAL TYPELIB_CALLTYPE  VarTextEndEx( PVARTEXT pvt DBG_PASS );
/* <combine sack::containers::text::VarTextEndEx@PVARTEXT pvt>
   \ \                                                         */
#define VarTextEnd(pvt) VarTextEndEx( (pvt) DBG_SRC )
/* Gets the length of the current collection in the VARTEXT.
   Parameters
   pvt :  PVARTEXT collector to get the length.              */
TYPELIB_PROC  INDEX TYPELIB_CALLTYPE  VarTextLength( PVARTEXT pvt );
/* Gets the text segment built in the VarText. The PVARTEXT is
   set to empty. Clears the collector.
   Parameters
   pvt :  PVARTEXT to get text from.                           */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  VarTextGetEx( PVARTEXT pvt DBG_PASS );
/* <combine sack::containers::text::VarTextGetEx@PVARTEXT pvt>
   \ \                                                         */
#define VarTextGet(pvt) VarTextGetEx( (pvt) DBG_SRC )
/* Used to look at the vartext collector and get the current
   collection. Does not clear the collector.
   Parameters
   pvt :       PVARTEXT collector to peek at
   DBG_PASS :  debugging file and line parameters
   Return Value List
   NULL :      No data
   not NULL :  text segment which is in the collector.       */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  VarTextPeekEx ( PVARTEXT pvt DBG_PASS );
/* \Returns the PTEXT that is currently in a PVARTEXT. It does
   not alter the contents of the PVARTEXT. Do not LineRelease
   this peeked value.                                          */
#define VarTextPeek(pvt) VarTextPeekEx( (pvt) DBG_SRC )
/* Increases the internal storage size of the variable text
   collector.
   Parameters
   pvt :       the var text collector to expand
   amount :    amount of size to expand the collector
   DBG_PASS :  debugging file and line parameters           */
TYPELIB_PROC  void TYPELIB_CALLTYPE  VarTextExpandEx( PVARTEXT pvt, INDEX size DBG_PASS );
/* Add a specified number of characters to the amount of space
   in the VARTEXT collector.                                   */
#define VarTextExpand(pvt, sz) VarTextExpandEx( (pvt), (sz) DBG_SRC )
//TYPELIB_PROC  int vtprintfEx( PVARTEXT pvt DBG_PASS TYPELIB_CALLTYPE  CTEXTSTR format, ... ;
// note - don't include format - MUST have at least one parameter passed to ...
//#define vtprintf(pvt, ...) vtprintfEx( (pvt) DBG_SRC, __VA_ARGS__ )
TYPELIB_PROC  INDEX TYPELIB_CALLTYPE  vtprintfEx( PVARTEXT pvt, CTEXTSTR format, ... );
/* <combine sack::containers::text::vtprintfEx@PVARTEXT@CTEXTSTR@...>
   Note                                                               */
#define vtprintf vtprintfEx
/* variable argument VARTEXT printf. Is passed a PVARTEXT to
   collect the formatted output using printf sort of formatting. */
TYPELIB_PROC  INDEX TYPELIB_CALLTYPE  vvtprintf( PVARTEXT pvt, CTEXTSTR format, va_list args );
/* encode binary buffer into base64 encoding.
   outsize is updated with the length of the buffer.
 */
TYPELIB_PROC  TEXTCHAR * TYPELIB_CALLTYPE  EncodeBase64Ex( const uint8_t* buf, size_t length, size_t *outsize, const char *encoding );
/* decode base64 buffer into binary buffer
   outsize is updated with the length of the buffer.
   result should be Release()'d
 */
TYPELIB_PROC  uint8_t * TYPELIB_CALLTYPE  DecodeBase64Ex( const char* buf, size_t length, size_t *outsize, const char *encoding );
/* xor a base64 encoded string over a utf8 string, keeping the utf8 characters in the same length...
   although technically this can result in invalid character encoding where upper bits get zeroed
   result should be Release()'d
*/
TYPELIB_PROC  char * TYPELIB_CALLTYPE  u8xor( const char *a, size_t alen, const char *b, size_t blen, int *ofs );
/* xor two base64 encoded strings, resulting in a base64 string
   result should be Release()'d
*/
TYPELIB_PROC  char * TYPELIB_CALLTYPE  b64xor( const char *a, const char *b );
//--------------------------------------------------------------------------
// extended command entry stuff... handles editing buffers with insert/overwrite/copy/paste/etc...
typedef struct user_input_buffer_tag {
	// -------------------- custom cmd buffer extension
  // position counter for pulling history; negative indexes are recalled commands.
	int nHistory;
  // a link queue which contains the prior lines of text entered for commands.
	PLINKQUEUE InputHistory;
 // set to TRUE when nHistory has wrapped...
	int   bRecallBegin;
   /* A exchange-lock variable for controlling access to the
      \history (so things aren't being read from it while it is
      scrolling old data out).                                  */
	uint32_t   CollectionBufferLock;
  // used to store index.. for insert type operations...
	INDEX CollectionIndex;
 // flag for whether we are inserting or overwriting
	int   CollectionInsert;
 // flag for whether we are inserting or overwriting
	int   storeCR;
 // used to store partial from GatherLine
	PTEXT CollectionBuffer;
 // called when a buffer is complete.
	void (CPROC*CollectedEvent)( uintptr_t psv, PTEXT text );
  // passed to the event callback when a line is completed
	uintptr_t psvCollectedEvent;
} USER_INPUT_BUFFER, *PUSER_INPUT_BUFFER;
/* Creates a buffer structure which behaves like the command
   line command recall queue.
                                                             */
TYPELIB_PROC  PUSER_INPUT_BUFFER TYPELIB_CALLTYPE  CreateUserInputBuffer ( void );
/* Destroy a created user input buffer. */
TYPELIB_PROC  void TYPELIB_CALLTYPE  DestroyUserInputBuffer ( PUSER_INPUT_BUFFER *pci );
// negative with SEEK_SET is SEEK_END -nPos
enum CommandPositionOps {
	// defined that the x,y position in the segment should be used for absolute positioning.
   // can also be SEEK_SET
 COMMAND_POS_SET = 0,
 // defined that the x,y position in the segment should be used for relative positioning.
 // can also be SEEK_CUR
 COMMAND_POS_CUR = 1
};
/* Updates the current input position, for things like input,
   etc. Some external process indicates where in the line to set
   the cursor position.                                          */
TYPELIB_PROC  LOGICAL TYPELIB_CALLTYPE  SetUserInputPosition ( PUSER_INPUT_BUFFER pci, int nPos, int whence );
// bInsert < 0 toggle insert.  bInsert == 0 clear isnert(set overwrite) else
// set insert (clear overwrite )
TYPELIB_PROC  void TYPELIB_CALLTYPE  SetUserInputInsert ( PUSER_INPUT_BUFFER pci, int bInsert );
TYPELIB_PROC  void TYPELIB_CALLTYPE  SetUserInputSaveCR( PUSER_INPUT_BUFFER pci, int bSaveCR );
/* Get the next command in the queue in the speicifed direction
   Parameters
   pci :  pointer to command input buffer
   bUp :  if TRUE \- get older command; else get the newer
          command.                                              */
TYPELIB_PROC  void TYPELIB_CALLTYPE  RecallUserInput ( PUSER_INPUT_BUFFER pci, int bUp );
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  GetUserInputLine( PUSER_INPUT_BUFFER pOutput );
/* Add a buffer to the history buffer.
                                       */
TYPELIB_PROC  void TYPELIB_CALLTYPE  EnqueUserInputHistory ( PUSER_INPUT_BUFFER pci, PTEXT pHistory );
/* Arbitrary PTEXT blocks are fed to the user input queue with
   this.
   Parameters
   pci :     pointer to command buffer
   stroke :  the stroke to add to the buffer (may be a whole
             String or linked list of segments). or NULL if
             getting existing input...
   Return Value List
   NULL :      There is no command available \- no text followed
               by a newline.
   not NULL :  A command line collected from the input text. There
               may be multiple commands in a single 'stroke'
               buffer.
   Example
   This may be used something like .... to add the storke to the
   \input buffer, and while there is a result, get the result
   from the buffer.
   <code lang="c++">
   {
       PUSER_INPUT_BUFFER pci = CreateUserInputBuffer();
       PTEXT result;
       for( result = GatherUserInput( pci, new_stroke ); result; result = GatherUserInput( pci, NULL ) )
       {
       }
   }
   </code>                                                                                               */
TYPELIB_PROC  PTEXT TYPELIB_CALLTYPE  GatherUserInput ( PUSER_INPUT_BUFFER pci, PTEXT stroke );
/* delete 1 character at current user input index */
TYPELIB_PROC  void TYPELIB_CALLTYPE  DeleteUserInput( PUSER_INPUT_BUFFER pci );
/* Converts ascii character set to ebcidc. */
TYPELIB_PROC TEXTSTR TYPELIB_CALLTYPE  ConvertAsciiEbdic( TEXTSTR text, INDEX length );
/* Routine to convert from ebcdic character set to ascii. */
TYPELIB_PROC TEXTSTR TYPELIB_CALLTYPE  ConvertEbcdicAscii( TEXTSTR text, INDEX length );
/* Converts ascii 85 to ascii */
TYPELIB_PROC TEXTSTR FtnATA( TEXTSTR buf );
/* Converts ascii character set to ascii 85  */
TYPELIB_PROC TEXTSTR ATFtnA( TEXTSTR buf );
/* Expand characters which are outside of standard ascii to URI
   compatible escapes.
   Parameters
   text :        Text to convert
   length :      max length of text to convert
   skip_slash :  if TRUE, keep slash characters as literal,
                 otherwise they get converted.                  */
TYPELIB_PROC TEXTSTR TYPELIB_CALLTYPE ConvertTextURI( CTEXTSTR text, INDEX length, int skip_slash );
/* Converts URI escape characters like %3B to the appropriate
   ascii characters. The resulting string must be released by
   the application.
   Parameters
   text :    TEXTCHAR * string to convert.
   length :  max length of text to convert.
   Example
   <code lang="c++">
   TEXTCHAR *sample = WIDE( "https://www.google.com/#hl=en&amp;sugexp=eqn&amp;cp=11&amp;gs_id=1a&amp;xhr=t&amp;q=%3B+%5C+%2B+:+";
   TEXTCHAR *result;
   \result = ConvertURIText( sample, StrLen( sample ) );
   \result == https://www.google.com/#hl=en&amp;sugexp=eqn&amp;cp=11&amp;gs_id=1a&amp;xhr=t&amp;q=;+\\+++:+
   </code>                                                                                                                        */
TYPELIB_PROC TEXTSTR TYPELIB_CALLTYPE ConvertURIText( CTEXTSTR text, INDEX length );
TYPELIB_PROC LOGICAL TYPELIB_CALLTYPE ParseStringVector( CTEXTSTR data, CTEXTSTR **pData, int *nData );
TYPELIB_PROC LOGICAL TYPELIB_CALLTYPE ParseIntVector( CTEXTSTR data, int **pData, int *nData );
#ifdef __cplusplus
 //namespace text {
}
#endif
//--------------------------------------------------------------------------
#ifdef __cplusplus
	namespace BinaryTree {
#endif
/* This type defines a specific node in the tree. It is entirely
   private, and is a useless definition.                         */
typedef struct treenode_tag *PTREENODE;
/* Defines a Binary Tree.
   See Also
   <link CreateBinaryTree> */
typedef struct treeroot_tag *PTREEROOT;
/* This option may be passed to extended CreateBinaryTree
   methods to disallow adding of duplicates. Otherwise
   duplicates will be added; they will be added to the side of
   the node with the same value that has less children. Trees
   are created by default without this option, allowing the
   addition of duplicates.
   Example
   <code lang="c++">
   PTREEROOT = <link CreateBinaryTreeExtended>( BT_OPT_NODUPLICATES, NULL, NULL DBG_SRC );
   </code>                                                                                 */
#define BT_OPT_NODUPLICATES 1
/* Generic Compare is the type declaration for the callback routine for user custom comparisons.
  This routine should return -1 if new is less than old, it should return 1 if new is more than old, and it
  should return 0 if new and old are the same key. */
typedef int (CPROC *GenericCompare)( uintptr_t oldnode,uintptr_t newnode );
/* Signature for the user callback passed to CreateBinaryTreeEx
   that will be called for each node removed from the binary
   list.                                                        */
typedef void (CPROC *GenericDestroy)( CPOINTER user, uintptr_t key);
/* when adding a node if Compare is NULL the default method of a
   basic unsigned integer compare on the key value is done. if
   Compare is specified the specified key value of the orginal
   node (old) and of the new node (new) is added. Result of
   compare should be ( \<0 (lesser)) ( 0 (equal)) ( \>0
   (greater))
   Example
   <code lang="c++">
   int CPROC MyGenericCompare( uintptr_t oldnode,uintptr_t newnode )
   {
   </code>
   <code>
      if(oldnode\>newnode)
          return 1;
      else if(oldnode\<newnode)
          return -1;
      else return 0;
   </code>
   <code lang="c++">
      return (oldnode\>newnode)? 1
             \:(oldnode\<newnode)? -1
             \:0;
   }
   void CPROC MyGenericDestroy(POINTER user, uintptr_t key)
   {
      // do something custom with your user data and or key value
   }
   PTREEROOT tree = CreateBinaryTreeExtended( 0 // BT_OPT_NODUPLICATES
                                            , MyGenericCompare
                                            , MyGenericDestroy
                                            <link DBG_PASS, DBG_SRC> );
   </code>
   See Also
   <link CreateBinaryTreeExx>
   <link CreateBinaryTreeEx>
   <link CreateBinaryTree>                                               */
TYPELIB_PROC  PTREEROOT TYPELIB_CALLTYPE  CreateBinaryTreeExtended( uint32_t flags
															, GenericCompare Compare
															, GenericDestroy Destroy DBG_PASS);
/* This is the simpler case of <link CreateBinaryTreeExtended>,
   which does not make you pass DBG_SRC.
   Example
   <code lang="c++">
   PTREEROOT tree = CreateBinaryTreeExx( BT_OPT_NODUPLICATES, NULL, NULL );
   </code>                                                                  */
#define CreateBinaryTreeExx(flags,compare,destroy) CreateBinaryTreeExtended(flags,compare,destroy DBG_SRC)
/* Creates a binary tree, allowing specification of comparison
   and destruction routines.
   Example
   <code lang="c++">
   PTREEROOT tree = CreateBinaryTreeEx( <link CreateBinaryTreeExtended, MyGenericCompare>, <link CreateBinaryTreeExtended, MyGenericDestroy> );
   </code>                                                                                                                                      */
#define CreateBinaryTreeEx(compare,destroy) CreateBinaryTreeExx( 0, compare, destroy )
/* This is the simplest way to create a binary tree.
   The default compare routine treats 'key' as an integer value
   that is compared against other for lesser/greater condition.
   This tree also allows duplicates to be added.
   Example
   <code lang="c++">
   PTREEROOT tree = CreateBinaryTree();
   </code>                                                      */
#define CreateBinaryTree() CreateBinaryTreeEx( NULL, NULL )
/* \ \
   Example
   <code lang="c++">
   PTREEROOT tree = CreateBinaryTree();
   DestroyBinaryTree( tree );
   tree = NULL;
   </code>                              */
TYPELIB_PROC  void TYPELIB_CALLTYPE  DestroyBinaryTree( PTREEROOT root );
/* Drops all the nodes in a tree so it becomes empty...
   \ \
   Example
   <code lang="c++">
   PTREEROOT tree = CreateBinaryTree();
   ResetBinaryTree( tree );
   tree = NULL;
   </code>                              */
TYPELIB_PROC  void TYPELIB_CALLTYPE  ResetBinaryTree( PTREEROOT root );
/* Balances a binary tree. If data is added to a binary list in
   a linear way (from least to most), the tree can become
   unbalanced, and all be on the left or right side of data. This
   routine can analyze branches and perform rotations so that
   the tree can be discretely rebalanced.
   Example
   <code lang="c++">
   <link PTREEROOT> tree;
   // <link AddBinaryNode>...
   BalanceBinaryTree( tree );
   </code>                                                        */
TYPELIB_PROC  void TYPELIB_CALLTYPE  BalanceBinaryTree( PTREEROOT root );
/* \ \
   See Also
   <link AddBinaryNode>
   <link DBG_PASS>
                        */
TYPELIB_PROC  int TYPELIB_CALLTYPE  AddBinaryNodeEx( PTREEROOT root
                                                   , CPOINTER userdata
                                                   , uintptr_t key DBG_PASS );
/* Adds a user pointer identified by key to a binary list.
   See Also
   <link BinaryTree::CreateBinaryTree, CreateBinaryTree>
   Example
   <code lang="c++">
   PTREEROOT tree = CreateBinaryTree();
   uintptr_t key = 1;
   POINTER data = NewArray( TEXTCHAR, 32 );
   AddBinaryNode( tree, data, key );
   </code>
   Parameters
   root :  PTREEROOT binary tree instance.
   data :  POINTER to some user object.
   key :   uintptr_t a integer type which can be used to identify
           the data. (used to compare in the tree).<p /><p />If
           the user has specified a custom comparison routine in
           an extended CreateBinaryTree(), then this value might
           be a pointer to some other data. Often the thing used
           to key into a binary tree is a <link CTEXTSTR>.
   Returns
   The tree may be created with <link BT_OPT_NODUPLICATES>, in
   which case this will result FALSE if the key is found
   duplicated in the list. Otherwise this returns TRUE. if the
   root parameter is NULL, the result is FALSE.                  */
#define AddBinaryNode(r,u,k) AddBinaryNodeEx((r),(u),(k) DBG_SRC )
//TYPELIB_PROC  int TYPELIB_CALLTYPE  AddBinaryNode( PTREEROOT root
//                                    , POINTER userdata
//                                    , uintptr_t key );
TYPELIB_PROC  void TYPELIB_CALLTYPE  RemoveBinaryNode( PTREEROOT root, POINTER use, uintptr_t key );
/* Search in a binary tree for the specified key.
   Returns
   user data POINTER if found, else NULL.
   Example
   <code lang="c++">
   PTREEROOT tree;
   void f( void )
   {
      CPOINTER mydata = FindInBinaryTree( tree, 5 );
      if( mydata )
      {
          // found '5' as the key in the tree
      }
   }
   </code>                                          */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  FindInBinaryTree( PTREEROOT root, uintptr_t key );
// result of fuzzy routine is 0 = match.  100 = inexact match
// 1 = no match, actual may be larger
// -1 = no match, actual may be lesser
// 100 = inexact match- checks nodes near for better match.
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  LocateInBinaryTree( PTREEROOT root, uintptr_t key
														, int (CPROC*fuzzy)( uintptr_t psv, uintptr_t node_key ) );
/* During FindInBinaryTree and LocateInBinaryTree, the last
   found result is stored. This function allows deletion of that
   node.
   Example
   <code lang="c++">
   FindInBinaryTree( tree, 5 );
   RemoveLastFoundNode( tree );
   </code>                                                       */
TYPELIB_PROC  void TYPELIB_CALLTYPE  RemoveLastFoundNode(PTREEROOT root );
/* Removes the currently browsed node from the tree.
   See Also
   <link GetChildNode>                               */
TYPELIB_PROC  void TYPELIB_CALLTYPE  RemoveCurrentNode(PTREEROOT root );
/* Basically this is meant to dump to a log, if the print
   function is passed as NULL, then the tree's contents are
   dumped to the log. It dumps a very cryptic log of how all
   nodes in the tree are arranged. But by allowing the user to
   provide a method to log his data and key, the logging is more
   meaningful based on the application. The basic code for
   managing trees and nodes works....
   Example
   <code>
   int ForEachNode( POINTER user, uintptr_t key )
   {
       // return not 1 to dump to log the internal tree structure
       return 0; // probably did own logging here, so don't log tree internal
   }
   <link PTREEROOT> tree;
   void f( void )
   {
       DumpTree( tree, ForEachNode );
   }
   </code>                                                                    */
TYPELIB_PROC  void TYPELIB_CALLTYPE  DumpTree( PTREEROOT root
                          , int (*Dump)( CPOINTER user, uintptr_t key ) );
/* See Also
   <link GetChildNode> */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetLeastNodeEx( PTREEROOT root, POINTER *cursor );
/* See Also
   <link GetChildNode> */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetLeastNode( PTREEROOT root );
/* See Also
   <link GetChildNode> */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetGreatestNodeEx( PTREEROOT root, POINTER *cursor );
/* See Also
   <link GetChildNode> */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetGreatestNode( PTREEROOT root );
/* See Also
   <link GetChildNode> */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetLesserNodeEx( PTREEROOT root, POINTER *cursor );
/* See Also
   <link GetChildNode> */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetLesserNode( PTREEROOT root );
/* See Also
   <link GetChildNode> */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetGreaterNodeEx( PTREEROOT root, POINTER *cursor );
/* See Also
   <link GetChildNode> */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetGreaterNode( PTREEROOT root );
/* \Returns the node that is set as 'current' in the tree. There
   is a cursor within the tree that can be used for browsing.
   See Also
   <link GetChildNode>                                           */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetCurrentNodeEx( PTREEROOT root, POINTER *cursor );
/* \Returns the node that is set as 'current' in the tree. There
   is a cursor within the tree that can be used for browsing.
   See Also
   <link GetChildNode>                                           */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetCurrentNode( PTREEROOT root );
/* This sets the current node cursor to the root of the node.
   See Also
   <link GetChildNode>                                        */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetRootNode( PTREEROOT root );
/* See Also
   <link GetChildNode> */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetParentNodeEx( PTREEROOT root, POINTER *cursor );
/* See Also
   <link GetChildNode> */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetParentNode( PTREEROOT root );
/* While browsing the tree after a find operation move to the
   next child node, direction 0 is lesser direction !0 is
   greater.
   Binary Trees have a 'current' cursor. These operations may be
   used to browse the tree.
   Example
   \ \
   <code>
   // this assumes you have a tree, and it's fairly populated, then this demonstrates
   // all steps of browsing.
   POINTER my_data;
   // go to the 'leftmost' least node. (as determined by the compare callback)
   my_data = GetLeastNode( tree );
   // go to the 'rightmost' greatest node. (as determined by the compare callback)
   my_data = GetGreatestNode( tree );
   // move to the node that is less than the current node.  (move to the 'left')
   my_data = GetLesserNode( tree );
   // move to the node that is greater than the current node.  (move to the 'right')
   my_data = GetGreaterNode( tree );
   // follow the tree to the left down from here
   my_data = GetChildNode( tree, 0 );
   // follow the tree to the right down from here
   my_data = GetChildNode( tree, 1 );
   // follow the tree up to the node above the current one.
   //  (the one who's lesser or greater points at this)
   my_data = GetParentNode( tree );
   // this is probably the least useful, but someone clever might find a trick for it
   // Move back to the node we were just at.
   //  (makes the current the prior, and moves to what the prior was,
   //     but then it's just back and forth between the last two; it's not a stack ).
   my_data = GetPriorNode( tree );
   </code>
   A more practical example...
   <code lang="c++">
   POINTER my_data;
   for( my_data = GetLeastNode( tree );
        my_data;
        my_data = GetGreaterNode( tree ) )
   {
        // browse the tree from least to most.
   }
   </code>                                                                            */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetChildNode( PTREEROOT root, int direction );
/* See Also
   <link GetChildNode> */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetChildNodeEx( PTREEROOT root, POINTER *cursor, int direction );
/* See Also
   <link GetChildNode> */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetPriorNodeEx( PTREEROOT root, POINTER *cursor );
/* See Also
   <link GetChildNode> */
TYPELIB_PROC  CPOINTER TYPELIB_CALLTYPE  GetPriorNode( PTREEROOT root );
/* \Returns the total number of nodes in the tree.
   Example
   <code lang="c++">
   int total_nodes = GetNodeCount(tree);
   </code>                                         */
TYPELIB_PROC  int TYPELIB_CALLTYPE  GetNodeCount ( PTREEROOT root );
 // returns a shadow of the original.
TYPELIB_PROC  PTREEROOT TYPELIB_CALLTYPE  ShadowBinaryTree( PTREEROOT root );
#ifdef __cplusplus
 //namespace BinaryTree {
	}
#endif
//--------------------------------------------------------------------------
#ifdef __cplusplus
namespace family {
#endif
/* A family tree structure, for tracking elements that have
   multiple children.
                                                            */
typedef struct familyroot_tag *PFAMILYTREE;
typedef struct familynode_tag *PFAMILYNODE;
/* <unfinished>
   Incomplete Work in progress (maybe) */
TYPELIB_PROC  PFAMILYTREE TYPELIB_CALLTYPE  CreateFamilyTree ( int (CPROC *Compare)(uintptr_t key1, uintptr_t key2)
															, void (CPROC *Destroy)(POINTER user, uintptr_t key) );
/* <unfinished>
   Incomplete, Family tree was never completed. */
TYPELIB_PROC  POINTER TYPELIB_CALLTYPE  FamilyTreeFindChild ( PFAMILYTREE root
														  , uintptr_t psvKey );
/* <unfinished>
   Incomplete, Family tree was never completed. */
TYPELIB_PROC  POINTER  TYPELIB_CALLTYPE FamilyTreeFindChildEx ( PFAMILYTREE root, PFAMILYNODE root_node
													 , uintptr_t psvKey );
/* Resets the search cursors in the tree... */
TYPELIB_PROC  void TYPELIB_CALLTYPE  FamilyTreeReset ( PFAMILYTREE *option_tree );
/* Resets the content of the tree (should call destroy methods, at this time it does not) */
TYPELIB_PROC  void TYPELIB_CALLTYPE  FamilyTreeClear ( PFAMILYTREE option_tree );
/* <unfinished>
   Incomplete Work in progress (maybe) */
TYPELIB_PROC  PFAMILYNODE TYPELIB_CALLTYPE  FamilyTreeAddChild ( PFAMILYTREE *root, PFAMILYNODE parent, POINTER userdata, uintptr_t key );
TYPELIB_PROC LOGICAL TYPELIB_CALLTYPE FamilyTreeForEachChild( PFAMILYTREE root, PFAMILYNODE node
			, LOGICAL (CPROC *ProcessNode)( uintptr_t psvForeach, uintptr_t psvNodeData )
			, uintptr_t psvUserData );
TYPELIB_PROC LOGICAL TYPELIB_CALLTYPE FamilyTreeForEach( PFAMILYTREE root, PFAMILYNODE node
			, LOGICAL (CPROC *ProcessNode)( uintptr_t psvForeach, uintptr_t psvNodeData, int level )
			, uintptr_t psvUserData );
#ifdef __cplusplus
 //namespace family {
}
#endif
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
#ifdef __cplusplus
//} // extern "c"
 // namespace containers
}
 // namespace sack
}
using namespace sack::containers::link_stack;
using namespace sack::containers::data_stack;
using namespace sack::containers::data_list;
using namespace sack::containers::data_queue;
using namespace sack::containers::queue;
using namespace sack::containers::BinaryTree;
using namespace sack::containers::text;
using namespace sack::containers::message;
using namespace sack::containers::sets;
using namespace sack::containers::family;
using namespace sack::containers;
#else
// should 'class'ify these things....
#endif
#ifndef _TYPELIBRARY_SOURCE
//#undef TYPELIB_PROC // we don't need this symbol after having built the right prototypes
#endif
#endif
// $Log: sack_typelib.h,v $
// Revision 1.99  2005/07/10 23:56:25  d3x0r
// Fix types for C++...
//
//
// Revision 1.39  2003/03/25 08:38:11  panther
// Add logging
//
SACK_NAMESPACE
#ifndef IS_DEADSTART
// this is always statically linked with libraries, so they may contact their
// core executable to know when it's done loading everyone else also...
#  ifdef __cplusplus
extern "C"
#  endif
#  if defined( WIN32 ) && !defined( __STATIC__ ) && !defined( __ANDROID__ )
#    ifdef __NO_WIN32API__
// DllImportAttribute ?
#    else
__declspec(dllimport)
#    endif
#  else
#ifndef __cplusplus
extern
#endif
#  endif
/* a function true/false which indicates whether the root
   deadstart has been invoked already. If not, one should call
   InvokeDeadstart and MarkDeadstartComplete.
   <code lang="c++">
   int main( )
   {
       if( !is_deadstart_complete() )
       {
           InvokeDeadstart();
           MarkDeadstartComplete()
       }
       ... your code here ....
       return 0;  // or some other appropriate return.
   }
   </code>
   sack::app::deadstart                                        */
LOGICAL
#  if defined( __WATCOMC__ )
__cdecl
#  endif
is_deadstart_complete( void );
#endif
/* Define a routine to call for exit().  This triggers specific code to handle shutdown event registration */
#ifndef NO_EXPORTS
#  ifdef SACK_BAG_CORE_EXPORTS
EXPORT_METHOD
#  else
IMPORT_METHOD
#  endif
#else
#  ifndef SACK_BAG_CORE_EXPORTS
	extern
#  endif
#endif
		void CPROC BAG_Exit( int code );
#ifndef NO_SACK_EXIT_OVERRIDE
#define exit(n) BAG_Exit(n)
#endif
 // namespace sack {
SACK_NAMESPACE_END
// this should become common to all libraries and programs...
//#include <construct.h> // pronounced 'kahn-struct'
/*
 *  Crafted by James Buckeyne
 *  Part of SACK github.com/d3x0r/SACK
 *
 *   (c) Freedom Collective 2000-2006++, 2016++
 *
 *   created to provide standard logging features
 *   lprintf( format, ... ); simple, basic
 *   if DEBUG, then logs to a file of the same name as the program
 *   if RELEASE most of this logging goes away at compile time.
 *
 *  standardized to never use int.
 */
#ifndef LOGGING_MACROS_DEFINED
#define LOGGING_MACROS_DEFINED
#define SYSLOG_API CPROC
#ifdef SYSLOG_SOURCE
#define SYSLOG_PROC EXPORT_METHOD
#else
#define SYSLOG_PROC IMPORT_METHOD
#endif
#ifdef __cplusplus
#define LOGGING_NAMESPACE namespace sack { namespace logging {
#define LOGGING_NAMESPACE_END } }
#else
#define LOGGING_NAMESPACE
#define LOGGING_NAMESPACE_END
#endif
#ifdef __cplusplus
	namespace sack {
/* Handles log output. Logs can be directed to UDP directed, or
   broadcast, or localhost, or to a file location, and under
   windows the debugging console log.
   lprintf
   SetSystemLog
   SystemLogTime
   there are options, when options code is enabled, which
   control logging output and format. Log file location can be
   specified generically for instance.... see Options.
	This namespace contains the logging functions. The most basic
   thing you can do to start logging is use 'lprintf'.
   <code lang="c++">
   lprintf( "My printf like format %s %d times", "string", 15 );
   </code>
   This function takes a format string and arguments compatible
   with vsnprintf. Internally strings are truncated to 4k
   length. (that is no single logging message can be more than
   4k in length).
   There are functions to control logging behavior.
   See Also
   SetSystemLog
   SystemLogTime
   SystemLogOptions
   lprintf
   _lprintf
   xlprintf
   _xlprintf
                                                                 */
		namespace logging {
#endif
/* \Parameters for SetSystemLog() to specify where the logging
   should go.                                                  */
enum syslog_types {
 // disable any log output.
SYSLOG_NONE     =   -1
,
SYSLOG_UDP      =    0
,
SYSLOG_FILE     =    1
,
 /* Set logging to output to a file. The file passed is a FILE*. This
   may be a FILE* like stdout, stderr, or some file the
   application opens.                                                */
SYSLOG_FILENAME =    2
,
 /* Set logging to go to a file, pass the string text name of the
   \file to open as the second parameter of SetSystemLog.        */
SYSLOG_SYSTEM   =    3
,
 /* Specify that logging should go to system (this actually means
   Windows system debugging channel. OutputDebugString() ).      */
SYSLOG_UDPBROADCAST= 4
// Allow user to specify a void UserCallback( char * )
// which recieves the formatted output.
,
SYSLOG_CALLBACK    = 5
,
 /* Send Logging to a specified user callback to handle. This
   lets logging go anywhere else that's not already thought of. */
SYSLOG_AUTO_FILE = SYSLOG_FILE + 100
 /* Send logging to a file. If the file is not open, open the
   \file. If no logging happens, no log file is created.     */
,
SYSLOG_SOCKET_SYSLOGD
};
#if !defined( NO_LOGGING )
#define DO_LOGGING
#endif
// this was forced, force no_logging off...
#if defined( DO_LOGGING )
#undef NO_LOGGING
#endif
#ifdef __LINUX__
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
SYSLOG_PROC  LOGICAL SYSLOG_API  IsBadReadPtr ( CPOINTER pointer, uintptr_t len );
#endif
SYSLOG_PROC  CTEXTSTR SYSLOG_API  GetPackedTime ( void );
//  returns the millisecond of the day (since UNIX Epoch) * 256 ( << 8 )
// the lowest 8 bits are the timezone / 15.
// The effect of the low [7/]8 bits being the time zone is that within the same millisecond
// UTC +0 sorts first, followed by +1, +2, ... etc until -14, -13, -12,... -1
// the low [7/]8 bits are the signed timezone
// (timezone could have been either be hr*60 + min (ISO TZ format)
// or in minutes (hr*60+mn) this would only take 7 bits
// one would think 8 bit shifts would be slightly more efficient than 7 bits.
// and sign extension for 8 bits already exists.
// - REVISION - timezone with hr*100 does not divide by 15 cleanly.
//     The timezone is ( hour*60 + min ) / 15 which is a range from -56 to 48
//     minimal representation is 7 bits (0 - 127 or -64 - 63)
//     still keeping 8 bits for shifting, so the effective range is only -56 to 48 of -128 to 127
// struct time_of_day {
//    uint64_t epoch_milliseconds : 56;
//    int64_t timezone : 8; divided by 15... hours * 60 / 15
// }
SYSLOG_PROC  int64_t SYSLOG_API GetTimeOfDay( void );
// binary little endian order; somewhat
typedef struct sack_expanded_time_tag
{
	uint16_t ms;
	uint8_t sc,mn,hr,dy,mo;
	uint16_t yr;
	int8_t zhr, zmn;
} SACK_TIME;
typedef struct sack_expanded_time_tag *PSACK_TIME;
// convert a integer time value to an expanded structure.
SYSLOG_PROC void     SYSLOG_API ConvertTickToTime( int64_t, PSACK_TIME st );
// convert a expanded time structure to a integer value.
SYSLOG_PROC int64_t SYSLOG_API ConvertTimeToTick( PSACK_TIME st );
// returns timezone as hours*100 + minutes.
// result is often negated?
SYSLOG_PROC  int SYSLOG_API GetTimeZone(void);
//
typedef void (CPROC*UserLoggingCallback)( CTEXTSTR log_string );
SYSLOG_PROC  void SYSLOG_API  SetSystemLog ( enum syslog_types type, const void *data );
SYSLOG_PROC  void SYSLOG_API  ProtectLoggedFilenames ( LOGICAL bEnable );
SYSLOG_PROC  void SYSLOG_API  SystemLogFL ( CTEXTSTR FILELINE_PASS );
SYSLOG_PROC  void SYSLOG_API  SystemLogEx ( CTEXTSTR DBG_PASS );
SYSLOG_PROC  void SYSLOG_API  SystemLog ( CTEXTSTR );
SYSLOG_PROC  void SYSLOG_API  LogBinaryFL ( const uint8_t* buffer, size_t size FILELINE_PASS );
SYSLOG_PROC  void SYSLOG_API  LogBinaryEx ( const uint8_t* buffer, size_t size DBG_PASS );
SYSLOG_PROC  void SYSLOG_API  LogBinary ( const uint8_t* buffer, size_t size );
// logging level defaults to 1000 which is log everything
SYSLOG_PROC  void SYSLOG_API  SetSystemLoggingLevel ( uint32_t nLevel );
#if defined( _DEBUG ) || defined( _DEBUG_INFO )
/* Log a binary buffer. Logs lines representing 16 bytes of data
   at a time. The hex of each byte in a buffer followed by the
   text is logged.
   Example
   <code lang="c#">
   char sample[] = "sample string";
   LogBinary( sample, sizeof( sample ) );
   </code>
   Results with the following output in the log...
   <code>
    73 61 6D 70 6C 65 20 73 74 72 69 6E 67 00 sample string.
   </code>
   The '.' at the end of 'sample string' is a non printable
   character. characters 0-31 and 127+ are printed as '.'.       */
#define LogBinary(buf,sz) LogBinaryFL((uint8_t*)(buf),sz DBG_SRC )
#define SystemLog(buf)    SystemLogFL(buf DBG_SRC )
#else
// need to include the typecast... binary logging doesn't really care what sort of pointer it gets.
#define LogBinary(buf,sz) LogBinary((uint8_t*)(buf),sz )
//#define LogBinaryEx(buf,sz,...) LogBinaryFL(buf,sz FILELINE_NULL)
//#define SystemLogEx(buf,...) SystemLogFL(buf FILELINE_NULL )
#endif
// int result is useless... but allows this to be
// within expressions, which with this method should be easy.
typedef INDEX (CPROC*RealVLogFunction)(CTEXTSTR format, va_list args )
//#if defined( __GNUC__ )
//	__attribute__ ((__format__ (__vprintf__, 1, 2)))
//#endif
	;
typedef INDEX (CPROC*RealLogFunction)(CTEXTSTR format,...)
#if defined( __GNUC__ )
	__attribute__ ((__format__ (__printf__, 1, 2)))
#endif
	;
SYSLOG_PROC  RealVLogFunction SYSLOG_API  _vxlprintf ( uint32_t level DBG_PASS );
SYSLOG_PROC  RealLogFunction SYSLOG_API  _xlprintf ( uint32_t level DBG_PASS );
// utility function to format a cpu delta into a buffer...
// end-start is always printed... therefore tick_end-0 is
// print absolute time... formats as millisecond.NNN
SYSLOG_PROC  void SYSLOG_API  PrintCPUDelta ( TEXTCHAR *buffer, size_t buflen, uint64_t tick_start, uint64_t tick_end );
// return the current CPU tick
SYSLOG_PROC  uint64_t SYSLOG_API  GetCPUTick ( void );
// result in nano seconds - thousanths of a millisecond...
SYSLOG_PROC  uint32_t SYSLOG_API  ConvertTickToMicrosecond ( uint64_t tick );
SYSLOG_PROC  uint64_t SYSLOG_API  GetCPUFrequency ( void );
SYSLOG_PROC  CTEXTSTR SYSLOG_API  GetTimeEx ( int bUseDay );
SYSLOG_PROC  void SYSLOG_API  SetSyslogOptions ( FLAGSETTYPE *options );
/* When setting options using SetSyslogOptions() these are the
   defines for the bits passed.
   SYSLOG_OPT_OPENAPPEND - the file, when opened, will be opened
   for append.
   SYSLOG_OPT_OPEN_BACKUP - the file, if it exists, will be
   renamed automatically.
   SYSLOG_OPT_LOG_PROGRAM_NAME - enable logging the program
   executable (probably the same for all messages, unless they
   are network)
   SYSLOG_OPT_LOG_THREAD_ID - enables logging the unique process
   and thread ID.
   SYSLOG_OPT_LOG_SOURCE_FILE - enable logging source file
   information. See <link DBG_PASS>
   SYSLOG_OPT_MAX - used for declaring a flagset to pass to
   setoptions.                                                   */
enum system_logging_option_list {
		/* the file, when opened, will be opened for append.
		 */
		SYSLOG_OPT_OPENAPPEND
										  ,
  /* the file, if it exists, will be renamed automatically.
										  */
										  SYSLOG_OPT_OPEN_BACKUP
                                ,
 /* enable logging the program executable (probably the same for
                                   all messages, unless they are network)
                                                                                                */
                                 SYSLOG_OPT_LOG_PROGRAM_NAME
										  ,
 /* enables logging the unique process and thread ID.
										                                                       */
                                 SYSLOG_OPT_LOG_THREAD_ID
                                ,
 /* enable logging source file information. See <link DBG_PASS>
                                                                                               */
										   SYSLOG_OPT_LOG_SOURCE_FILE
										  ,
										  SYSLOG_OPT_MAX
};
// this solution was developed to provide the same
// functionality for compilers that refuse to implement __VA_ARGS__
// this therefore means that the leader of the function is replace
// and that extra parenthesis exist after this... therefore the remaining
// expression must be ignored... thereofre when defining a NULL function
// this will result in other warnings, about ignored, or meaningless expressions
# if defined( DO_LOGGING )
#  define vlprintf      _vxlprintf(LOG_NOISE DBG_SRC)
#  define lprintf       _xlprintf(LOG_NOISE DBG_SRC)
#  define _lprintf(file_line,...)       _xlprintf(LOG_NOISE file_line,##__VA_ARGS__)
#  define xlprintf(level)       _xlprintf(level DBG_SRC)
#  define vxlprintf(level)       _vxlprintf(level DBG_SRC)
# else
#  ifdef _MSC_VER
#   define vlprintf      (1)?(0):
#   define lprintf       (1)?(0):
#   define _lprintf(DBG_VOIDRELAY)       (1)?(0):
#   define xlprintf(level)       (1)?(0):
#   define vxlprintf(level)      (1)?(0):
#  else
#   define vlprintf(f,...)
/* use printf formating to output to the log. (log printf).
   Parameters
   Format :  Just like printf, the format string to print.
   ... :     extra arguments passed as required for the format.
   Example
   <code lang="c++">
      lprintf( "Test Logging %d %d", 13, __LINE__ );
   </code>                                                      */
#   define lprintf(f,...)
#   define  _lprintf(DBG_VOIDRELAY)       lprintf
#   define xlprintf(level) lprintf
#   define vxlprintf(level) lprintf
#  endif
# endif
#undef LOG_WARNING
#undef LOG_ADVISORIES
#undef LOG_INFO
// Defined Logging Levels
enum {
	  // and you are free to use any numerical value,
	  // this is a rough guideline for wide range
	  // to provide a good scaling for levels of logging
 // unless logging is disabled, this will be logged
	LOG_ALWAYS = 1
 // logging level set to 50 or more will cause this to log
	, LOG_ERRORS = 50
	,
 /* Specify a logging level which only ERROR level logging is
	   logged.                                                   */
 // logging level set to 50 or more will cause this to log
	 LOG_ERROR = LOG_ERRORS
	,
 // .......
	 LOG_WARNINGS = 500
	,
 // .......
	 LOG_WARNING = LOG_WARNINGS
   ,
 /* Use to specify that the log message is a warning level
      message.                                               */
    LOG_ADVISORY = 625
   ,
    LOG_ADVISORIES = LOG_ADVISORY
	,
 /* A symbol to specify to log Adviseries, Warnings and Error
	   level messages only.                                      */
	 LOG_INFO = 750
	  ,
 /* A moderate logging level, which is near maximum verbosity of
	     logging.                                                     */
	   LOG_NOISE = 1000
     ,
 /* Define that the message is just noisy - though verbosly
	  informative, it's level is less critical than even INFO.
	  default iS LOG_NOISE which is 1000, an ddefault for disabling most messages
	  is to set log level to 999.  Have to increase to 2000 to see debug, and this name
     has beviously
	  */
      LOG_LEVEL_DEBUG = 2000
	,
 /* Specify the message is of DEBUG importance, which is far
	   above even NOISY. If debug logging is enabled, all logging,
	   ERROR, WARNING, ADVISORY, INFO, NOISY and DEBUG will be
	   logged.                                                     */
 // not quite a negative number, but really big
	 LOG_CUSTOM = 0x40000000
	,
 /* A bit with LOG_CUSTOM might be enabled, and the lower bits
	   under 0x40000000 (all bits 0x3FFFFFFF ) can be used to
	   indicate a logging type. Then SetLoggingLevel can be passed a
	   mask of bits to filter types of messages.                     */
 // not quite a negative number, but really big
	 LOG_CUSTOM_DISABLE = 0x20000000
	// bits may be user specified or'ed with this value
	// such that ...
	// Example 1:SetSystemLoggingLevel( LOG_CUSTOM | 1 ) will
	// enable custom logging messages which have the '1' bit on... a logical
	// and is used to test the low bits of this value.
	// example 2:SetSystemLogging( LOG_CUSTOM_DISABLE | 1 ) will disable logging
	// of messages with the 1 bit set.
  // mask of bits which may be used to enable and disable custom logging
#define LOG_CUSTOM_BITS 0xFFFFFF
};
 // this is a flag set consisting of 0 or more or'ed symbols
enum SyslogTimeSpecifications {
 // disable time logging
 SYSLOG_TIME_DISABLE = 0,
 // enable is anything not zero.
 SYSLOG_TIME_ENABLE  = 1,
 // specify to log milliseconds
 SYSLOG_TIME_HIGH    = 2,
 // log the year/month/day also
 SYSLOG_TIME_LOG_DAY = 4,
 // log the difference in time instead of the absolute time
 SYSLOG_TIME_DELTA   = 8,
 // logs cpu ticks... implied delta
 SYSLOG_TIME_CPU     =16
};
/* Specify how time is logged. */
SYSLOG_PROC void SYSLOG_API SystemLogTime( uint32_t enable );
#ifndef NO_LOGGING
#define OutputLogString(s) SystemLog(s)
/* Depricated. Logs a format string that takes 0 parameters.
   See Also
   <link sack::logging::lprintf, lprintf>                    */
#define Log(s)                                   SystemLog( s )
#else
#define OutputLogString(s)
/* Depricated. Logs a format string that takes 0 parameters.
   See Also
   <link sack::logging::lprintf, lprintf>                    */
#define Log(s)
#endif
/* Depricated. Logs a format string that takes 1 parameter.
   See Also
   <link sack::logging::lprintf, lprintf>                    */
#define Log1(s,p1)                               lprintf( s, p1 )
/* Depricated. Logs a format string that takes 2 parameters.
   See Also
   <link sack::logging::lprintf, lprintf>                    */
#define Log2(s,p1,p2)                            lprintf( s, p1, p2 )
/* Depricated. Logs a format string that takes 3 parameters.
   See Also
   <link sack::logging::lprintf, lprintf>                    */
#define Log3(s,p1,p2,p3)                         lprintf( s, p1, p2, p3 )
/* Depricated. Logs a format string that takes 4 parameters.
   See Also
   <link sack::logging::lprintf, lprintf>                    */
#define Log4(s,p1,p2,p3,p4)                      lprintf( s, p1, p2, p3,p4)
/* Depricated. Logs a format string that takes 5 parameters.
   See Also
   <link sack::logging::lprintf, lprintf>                    */
#define Log5(s,p1,p2,p3,p4,p5)                   lprintf( s, p1, p2, p3,p4,p5)
/* Depricated. Logs a format string that takes 6 parameters.
   See Also
   <link sack::logging::lprintf, lprintf>                    */
#define Log6(s,p1,p2,p3,p4,p5,p6)                lprintf( s, p1, p2, p3,p4,p5,p6)
/* Depricated. Logs a format string that takes 7 parameters.
   See Also
   <link sack::logging::lprintf, lprintf>                    */
#define Log7(s,p1,p2,p3,p4,p5,p6,p7)             lprintf( s, p1, p2, p3,p4,p5,p6,p7 )
/* Depricated. Logs a format string that takes 8 parameters.
   See Also
   <link sack::logging::lprintf, lprintf>                    */
#define Log8(s,p1,p2,p3,p4,p5,p6,p7,p8)          lprintf( s, p1, p2, p3,p4,p5,p6,p7,p8 )
/* Depricated. Logs a format string that takes 9 parameters.
   See Also
   <link sack::logging::lprintf, lprintf>                    */
#define Log9(s,p1,p2,p3,p4,p5,p6,p7,p8,p9)       lprintf( s, p1, p2, p3,p4,p5,p6,p7,p8,p9 )
/* Depricated. Logs a format string that takes 10 parameters.
   See Also
   <link sack::logging::lprintf, lprintf>                    */
#define Log10(s,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10)  lprintf( s, p1, p2, p3,p4,p5,p6,p7,p8,p9,p10 )
LOGGING_NAMESPACE_END
#ifdef __cplusplus
using namespace sack::logging;
#endif
#endif
#if defined( _MSC_VER ) || (1)
// huh, apparently all compiles are messed the hell up.
#  define COMPILER_THROWS_SIGNED_UNSIGNED_MISMATCH
#endif
#ifdef COMPILER_THROWS_SIGNED_UNSIGNED_MISMATCH
#  define SUS_GT(a,at,b,bt)   (((a)<0)?0:(((bt)a)>(b)))
#  define USS_GT(a,at,b,bt)   (((b)<0)?1:((a)>((at)b)))
#  define SUS_LT(a,at,b,bt)   (((a)<0)?1:(((bt)a)<(b)))
#  define USS_LT(a,at,b,bt)   (((b)<0)?0:((a)<((at)b)))
#  define SUS_GTE(a,at,b,bt)  (((a)<0)?0:(((bt)a)>=(b)))
#  define USS_GTE(a,at,b,bt)  (((b)<0)?1:((a)>=((at)b)))
#  define SUS_LTE(a,at,b,bt)  (((a)<0)?1:(((bt)a)<=(b)))
#  define USS_LTE(a,at,b,bt)  (((b)<0)?0:((a)<=((at)b)))
#else
#  define SUS_GT(a,at,b,bt)   ((a)>(b))
#  define USS_GT(a,at,b,bt)   ((a)>(b))
#  define SUS_LT(a,at,b,bt)   ((a)<(b))
#  define USS_LT(a,at,b,bt)   ((a)<(b))
#  define SUS_GTE(a,at,b,bt)  ((a)>=(b))
#  define USS_GTE(a,at,b,bt)  ((a)>=(b))
#  define SUS_LTE(a,at,b,bt)  ((a)<=(b))
#  define USS_LTE(a,at,b,bt)  ((a)<=(b))
#endif
#ifdef __cplusplus
using namespace sack;
using namespace sack::containers;
#endif
#endif
#endif
// incldue this first so we avoid a conflict.
// hopefully this comes from sack system?
/*
 *  Created by Jim Buckeyne
 *
 *  Purpose
 *    Generalization of system routines which began in
 *   dekware development.
 *   - Process control (load,start,stop)
 *   - Library runtime link control (load, unload)
 *
 */
#ifndef SYSTEM_LIBRARY_DEFINED
#define SYSTEM_LIBRARY_DEFINED
#ifdef SYSTEM_SOURCE
#define SYSTEM_PROC(type,name) EXPORT_METHOD type CPROC name
#else
#define SYSTEM_PROC(type,name) IMPORT_METHOD type CPROC name
#endif
#ifdef __LINUX__
// Hmm I thought that dlopen resulted in an int...
// but this doc says void * (redhat9)
//typedef void *HLIBRARY;
#else
//typedef HMODULE HLIBRARY;
#endif
#ifdef __cplusplus
#define _SYSTEM_NAMESPACE namespace system {
#define _SYSTEM_NAMESPACE_END }
#else
#define _SYSTEM_NAMESPACE
#define _SYSTEM_NAMESPACE_END
#endif
#define SACK_SYSTEM_NAMESPACE SACK_NAMESPACE _SYSTEM_NAMESPACE
#define SACK_SYSTEM_NAMESPACE_END _SYSTEM_NAMESPACE_END SACK_NAMESPACE_END
#ifndef UNDER_CE
#define HAVE_ENVIRONMENT
#endif
SACK_NAMESPACE
	_SYSTEM_NAMESPACE
typedef struct task_info_tag *PTASK_INFO;
typedef void (CPROC*TaskEnd)(uintptr_t, PTASK_INFO task_ended);
typedef void (CPROC*TaskOutput)(uintptr_t, PTASK_INFO task, CTEXTSTR buffer, size_t size );
// Run a program completely detached from the current process
// it runs independantly.  Program does not suspend until it completes.
// Use GetTaskExitCode() to get the return code of the process
#define LPP_OPTION_DO_NOT_HIDE           1
// for services to launch normal processes (never got it to work; used to work in XP/NT?)
#define LPP_OPTION_IMPERSONATE_EXPLORER  2
#define LPP_OPTION_FIRST_ARG_IS_ARG      4
#define LPP_OPTION_NEW_GROUP             8
#define LPP_OPTION_NEW_CONSOLE          16
#define LPP_OPTION_SUSPEND              32
#define LPP_OPTION_ELEVATE              64
SYSTEM_PROC( PTASK_INFO, LaunchPeerProgramExx )( CTEXTSTR program, CTEXTSTR path, PCTEXTSTR args
                                               , int flags
                                               , TaskOutput OutputHandler
                                               , TaskEnd EndNotice
                                               , uintptr_t psv
                                                DBG_PASS
                                               );
SYSTEM_PROC( PTASK_INFO, LaunchProgramEx )( CTEXTSTR program, CTEXTSTR path, PCTEXTSTR args, TaskEnd EndNotice, uintptr_t psv );
// launch a process, program name (including leading path), a optional path to start in (defaults to
// current process' current working directory.  And a array of character pointers to args
// args should be the NULL.
SYSTEM_PROC( PTASK_INFO, LaunchProgram )( CTEXTSTR program, CTEXTSTR path, PCTEXTSTR  args );
// abort task, no kill signal, sigabort basically.  Use StopProgram for a more graceful terminate.
// if (!StopProgram(task)) TerminateProgram(task) would be appropriate.
SYSTEM_PROC( uintptr_t, TerminateProgram )( PTASK_INFO task );
SYSTEM_PROC( void, ResumeProgram )( PTASK_INFO task );
// get first address of program startup code(?) Maybe first byte of program code?
SYSTEM_PROC( uintptr_t, GetProgramAddress )( PTASK_INFO task );
// before luanchProgramEx, there was no userdata...
SYSTEM_PROC( void, SetProgramUserData )( PTASK_INFO task, uintptr_t psv );
// attempt to implement a method on windows that allows a service to launch a user process
// current systems don't have such methods
SYSTEM_PROC( void, ImpersonateInteractiveUser )( void );
// after launching a process should revert to a protected state.
SYSTEM_PROC( void, EndImpersonation )( void );
// generate a Ctrl-C to the task.
// maybe also signal systray icon
// maybe also signal process.lock region
// maybe end process?
// maybe then terminate process?
SYSTEM_PROC( LOGICAL, StopProgram )( PTASK_INFO task );
// ctextstr as its own type is a pointer so a
//  PcTextStr is a pointer to strings -
//   char ** - returns a quoted string if args have spaces (and escape quotes in args?)
SYSTEM_PROC( TEXTSTR, GetArgsString )( PCTEXTSTR pArgs );
// after a task has exited, this can return its code.
// undefined if task has not exited (probably 0)
SYSTEM_PROC( uint32_t, GetTaskExitCode )( PTASK_INFO task );
// returns the name of the executable that is this process (without last . extension   .exe for instance)
SYSTEM_PROC( CTEXTSTR, GetProgramName )( void );
// returns the path of the executable that is this process
SYSTEM_PROC( CTEXTSTR, GetProgramPath )( void );
// returns the path that was the working directory when the program started
SYSTEM_PROC( CTEXTSTR, GetStartupPath )( void );
// returns the path of the current sack library.
SYSTEM_PROC( CTEXTSTR, GetLibraryPath )( void );
// on windows, queries an event that indicates the system is rebooting.
SYSTEM_PROC( LOGICAL, IsSystemShuttingDown )( void );
// HandlePeerOutput is called whenever a peer task has generated output on stdout or stderr
//   - someday evolution may require processing stdout and stderr with different event handlers
SYSTEM_PROC( PTASK_INFO, LaunchPeerProgramEx )( CTEXTSTR program, CTEXTSTR path, PCTEXTSTR args
                                              , TaskOutput HandlePeerOutput
                                              , TaskEnd EndNotice
                                              , uintptr_t psv
                                               DBG_PASS
                                              );
#define LaunchPeerProgram(prog,path,args,out,end,psv) LaunchPeerProgramEx(prog,path,args,out,end,psv DBG_SRC)
SYSTEM_PROC( PTASK_INFO, SystemEx )( CTEXTSTR command_line
                                   , TaskOutput OutputHandler
                                   , uintptr_t psv
                                   DBG_PASS
                                   );
#define System(command_line,output_handler,user_data) SystemEx( command_line, output_handler, user_data DBG_SRC )
// generate output to a task... read by peer task on standard input pipe
// if a task has been opened with an output handler, than IO is trapped, and this is a method of
// sending output to a task.
SYSTEM_PROC( int, pprintf )( PTASK_INFO task, CTEXTSTR format, ... );
// if a task has been opened with an otuput handler, than IO is trapped, and this is a method of
// sending output to a task.
SYSTEM_PROC( int, vpprintf )( PTASK_INFO task, CTEXTSTR format, va_list args );
typedef void (CPROC*generic_function)(void);
SYSTEM_PROC( generic_function, LoadFunctionExx )( CTEXTSTR library, CTEXTSTR function, LOGICAL bPrivate DBG_PASS);
SYSTEM_PROC( generic_function, LoadFunctionEx )( CTEXTSTR library, CTEXTSTR function DBG_PASS);
SYSTEM_PROC( void *, GetPrivateModuleHandle )( CTEXTSTR libname );
/*
  Add a custom loaded library; attach a name to the DLL space; this should allow
  getcustomsybmol to resolve these
  */
SYSTEM_PROC( void, AddMappedLibrary )( CTEXTSTR libname, POINTER image_memory );
SYSTEM_PROC( LOGICAL, IsMappedLibrary )( CTEXTSTR libname );
SYSTEM_PROC( void, DeAttachThreadToLibraries )( LOGICAL attach );
#define LoadFunction(l,f) LoadFunctionEx(l,f DBG_SRC )
SYSTEM_PROC( generic_function, LoadPrivateFunctionEx )( CTEXTSTR libname, CTEXTSTR funcname DBG_PASS );
#define LoadPrivateFunction(l,f) LoadPrivateFunctionEx(l,f DBG_SRC )
#define OnLibraryLoad(name)	  DefineRegistryMethod("SACK",_OnLibraryLoad,"system/library","load_event",name "_LoadEvent",void,(void), __LINE__)
// the callback passed will be called during LoadLibrary to allow an external
// handler to download or extract the library; the resulting library should also
// be loaded by the callback using the standard 'LoadFunction' methods
SYSTEM_PROC( void, SetExternalLoadLibrary )( LOGICAL (CPROC*f)(const char *) );
// please Release or Deallocate the reutrn value
// the callback should search for the file specified, if required, download or extract it
// and then return with a Release'able utf-8 char *.
SYSTEM_PROC( void, SetExternalFindProgram )( char * (CPROC*f)(const char *) );
// override the default program name.
// Certain program wrappers might use this to change log location, configuration, etc other defaults.
SYSTEM_PROC( void, SetProgramName )( CTEXTSTR filename );
// this is a pointer pointer - being that generic_fucntion is
// a pointer...
SYSTEM_PROC( int, UnloadFunctionEx )( generic_function* DBG_PASS );
#ifdef HAVE_ENVIRONMENT
SYSTEM_PROC( CTEXTSTR, OSALOT_GetEnvironmentVariable )(CTEXTSTR name);
SYSTEM_PROC( void, OSALOT_SetEnvironmentVariable )(CTEXTSTR name, CTEXTSTR value);
SYSTEM_PROC( void, OSALOT_AppendEnvironmentVariable )(CTEXTSTR name, CTEXTSTR value);
SYSTEM_PROC( void, OSALOT_PrependEnvironmentVariable )(CTEXTSTR name, CTEXTSTR value);
#endif
/* this needs to have 'GetCommandLine()' passed to it.
 * Otherwise, the command line needs to have the program name, and arguments passed in the string
 * the parameter to winmain has the program name skipped
 */
SYSTEM_PROC( void, ParseIntoArgs )( TEXTCHAR *lpCmdLine, int *pArgc, TEXTCHAR ***pArgv );
#define UnloadFunction(p) UnloadFunctionEx(p DBG_SRC )
/*
   Check if task spawning is allowed...
*/
SYSTEM_PROC( LOGICAL, sack_system_allow_spawn )( void );
/*
   Disallow task spawning.
*/
SYSTEM_PROC( void, sack_system_disallow_spawn )( void );
SACK_SYSTEM_NAMESPACE_END
#ifdef __cplusplus
using namespace sack::system;
#endif
#endif
//----------------------------------------------------------------------
// $Log: system.h,v $
// Revision 1.14  2005/07/06 00:33:55  jim
// Fixes for all sorts of mangilng with the system.h header.
//
//
// Revision 1.2  2003/10/24 14:59:21  panther
// Added Load/Unload Function for system shared library abstraction
//
// Revision 1.1  2003/10/24 13:22:06  panther
// Initial commit
//
//
#if defined( _MSC_VER )|| defined(__LCC__) || defined( __WATCOMC__ ) || defined( __GNUC__ )
/* Includes networking as appropriate for the target platform. Providing
   compatibility definitions as are lacking between platforms...
   or perhaps appropriate name aliasing to the correct types.            */
#ifndef INCLUDED_SOCKET_LIBRARY
#define INCLUDED_SOCKET_LIBRARY
#if defined( _WIN32 ) || defined( __CYGWIN__ )
//#ifndef __cplusplus_cli
#ifdef UNDER_CE
#define USE_WSA_EVENTS
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#if defined( MINGW_SUX ) && ( __GNUC__ < 5 )
/* Address information */
typedef struct addrinfoA {
    int             ai_flags;
    int             ai_family;
    int             ai_socktype;
    int             ai_protocol;
    size_t          ai_addrlen;
    char            *ai_canonname;
    struct sockaddr *ai_addr;
    struct addrinfoA *ai_next;
} ADDRINFOA;
typedef ADDRINFOA   *PADDRINFOA;
typedef struct addrinfoW {
    int                 ai_flags;
    int                 ai_family;
    int                 ai_socktype;
    int                 ai_protocol;
    size_t              ai_addrlen;
    PWSTR               ai_canonname;
    struct sockaddr     *ai_addr;
    struct addrinfoW    *ai_next;
} ADDRINFOW;
typedef ADDRINFOW   *PADDRINFOW;
typedef ADDRINFOA   ADDRINFOT;
typedef ADDRINFOA   *PADDRINFOT;
typedef ADDRINFOA   ADDRINFO;
typedef ADDRINFOA   *LPADDRINFO;
#endif
#ifdef __CYGWIN__
// just need this simple symbol
typedef int socklen_t;
#endif
//#endif
#elif defined( __LINUX__ )
#if defined( FBSD )
#endif
 // INADDR_ANY/NONE
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#if !defined( _PNACL )
#  include <net/if.h>
#endif
#define SOCKET int
#define SOCKADDR struct sockaddr
#define SOCKET_ERROR -1
//#define HWND int // unused params...
#define WSAEWOULDBLOCK EAGAIN
#define INVALID_SOCKET -1
#define WSAAsynchSelect( a,b,c,d ) (0)
#define WSAGetLastError()  (errno)
#define closesocket(s) close(s)
typedef struct hostent *PHOSTENT;
#ifndef __LINUX__
#define INADDR_ANY (-1)
#define INADDR_NONE (0)
#endif
struct win_in_addr {
	union {
		struct { uint8_t s_b1,s_b2,s_b3,s_b4; } S_un_b;
		struct { uint16_t s_w1,s_w2; } S_un_w;
		uint32_t S_addr;
	} S_un;
#ifndef __ANDROID__
#define s_addr  S_un.S_addr
/* can be used for most tcp & ip code */
#define s_host  S_un.S_un_b.s_b2
	/* host on imp */
#define s_net   S_un.S_un_b.s_b1
	/* network */
#define s_imp   S_un.S_un_w.s_w2
	/* imp */
#define s_impno S_un.S_un_b.s_b4
	/* imp # */
#define s_lh    S_un.S_un_b.s_b3
	/* logical host */
#endif
};
struct win_sockaddr_in {
#ifdef __MAC__
	uint8_t sa_len;
	uint8_t sin_family;
#else
	short   sin_family;
#endif
	uint16_t sin_port;
	struct  win_in_addr sin_addr;
	char    sin_zero[8];
};
typedef struct win_sockaddr_in SOCKADDR_IN;
#endif
#endif
// $Log: loadsock.h,v $
// Revision 1.7  2005/01/27 08:09:25  panther
// Linux cleaned.
//
// Revision 1.6  2003/06/04 11:38:01  panther
// Define PACKED
//
// Revision 1.5  2003/03/25 08:38:11  panther
// Add logging
//
#  if defined( __MAC__ )
#  else
               // _heapmin() included here
#    include <malloc.h>
#  endif
#else
//#include "loadsock.h"
#endif
//#include <stdlib.h>
#ifdef __CYGWIN__
 // provided by -lgcc
// lots of things end up including 'setjmp.h' which lacks sigset_t defined here.
// lots of things end up including 'setjmp.h' which lacks sigset_t defined here.
#  include <sys/signal.h>
#endif
// GetTickCount() and Sleep(n) Are typically considered to be defined by including stdhdrs...
/*
 *  Crafted by Jim Buckeyne
 *
 *  (c)2001-2006++ Freedom Collective
 *
 *  Provide API interface for timers, critical sections
 *  and other thread things.
 *
 */
#ifndef TIMERS_DEFINED
/* timers.h mutliple inclusion protection symbol. */
#define TIMERS_DEFINED
#if defined( _WIN32 )
// on windows, we add a function that returns HANDLE
#endif
#ifndef SHARED_MEM_DEFINED
/* Multiple inclusion protection symbol. */
#define SHARED_MEM_DEFINED
#if defined (_WIN32)
//#define USE_NATIVE_CRITICAL_SECTION
#endif
#if defined( _SHLWAPI_H ) || defined( _INC_SHLWAPI )
#undef StrChr
#undef StrCpy
#undef StrDup
#undef StrRChr
#undef StrStr
#endif
#if defined( __MAC__ )
#  define strdup(s) StrDup(s)
#  define strdup_free(s) Release(s)
#else
#  define strdup_free(s) free(s)
#endif
#ifdef __cplusplus
#define SACK_MEMORY_NAMESPACE SACK_NAMESPACE namespace memory {
#define SACK_MEMORY_NAMESPACE_END } SACK_NAMESPACE_END
#else
#define SACK_MEMORY_NAMESPACE
#define SACK_MEMORY_NAMESPACE_END
#endif
/* A declaration of the call type for memory library routines. */
#define MEM_API CPROC
#    ifdef MEM_LIBRARY_SOURCE
#      define MEM_PROC EXPORT_METHOD
#    else
/* Defines library linkage specification. */
#      define MEM_PROC IMPORT_METHOD
#    endif
#ifndef TIMER_NAMESPACE
#ifdef __cplusplus
#define _TIMER_NAMESPACE namespace timers {
/* define a timer library namespace in C++. */
#define TIMER_NAMESPACE SACK_NAMESPACE namespace timers {
/* define a timer library namespace in C++ end. */
#define TIMER_NAMESPACE_END } SACK_NAMESPACE_END
#else
#define TIMER_NAMESPACE
#define TIMER_NAMESPACE_END
#endif
#endif
	TIMER_NAMESPACE
   // enables file/line monitoring of sections and a lot of debuglogging
//#define DEBUG_CRITICAL_SECTIONS
   /* this symbol controls the logging in timers.c... (higher level interface to NoWait primatives)*/
//#define LOG_DEBUG_CRITICAL_SECTIONS
/* A custom implementation of windows CRITICAL_SECTION api.
   Provides same capability for Linux type systems. Can be
   checked as a study in how to implement safe locks.
   See Also
   InitCriticalSec
   EnterCriticalSec
   LeaveCriticalSec
   Example
   <c>For purposes of this example this is declared in global
   memory, known to initialize to all 0.</c>
   <code lang="c++">
   CRITICALSECTION cs_lock_test;
   </code>
   In some bit of code that can be executed by several
   threads...
   <code lang="c++">
   {
      EnterCriticalSec( &amp;cs_lock_test );
      // the code in here will only be run by a single thread
      LeaveCriticalSec( &amp;cs_lock_test );
   }
   </code>
   Remarks
   The __Ex versions of functions passes source file and line
   information in debug mode. This can be used if critical
   section debugging is turned on, or if critical section
   logging is turned on. (See ... ) This allows applications to
   find deadlocks by tracking who is entering critical sections
   and probably failing to leave them.                          */
struct critical_section_tag {
 // this is set when entering or leaving (updating)...
	uint32_t dwUpdating;
  // count of locks entered.  (only low 24 bits may count for 16M entries, upper bits indicate internal statuses.
	uint32_t dwLocks;
 // windows upper 16 is process ID, lower is thread ID
	THREAD_ID dwThreadID;
 // ID of thread waiting for this..
	THREAD_ID dwThreadWaiting;
#ifdef DEBUG_CRITICAL_SECTIONS
	// these are not included without a special compile flag
	// only required by low level deveopers who may be against
   // undefined behavior.
#define MAX_SECTION_LOG_QUEUE 16
	uint32_t bCollisions ;
	CTEXTSTR pFile[16];
	uint32_t  nLine[16];
	uint32_t  nLineCS[16];
 // windows upper 16 is process ID, lower is thread ID
	THREAD_ID dwThreadPrior[16];
 // windows upper 16 is process ID, lower is thread ID
	uint8_t isLock[16];
	int nPrior;
#endif
};
#if !defined( _WIN32 )
#undef USE_NATIVE_CRITICAL_SECTION
#endif
/* <combine sack::timers::critical_section_tag>
   \ \                                          */
#if defined( USE_NATIVE_CRITICAL_SECTION )
#define CRITICALSECTION CRITICAL_SECTION
#else
typedef struct critical_section_tag CRITICALSECTION;
#endif
/* <combine sack::timers::critical_section_tag>
   defines a pointer to a CRITICALSECTION type  */
#if defined( USE_NATIVE_CRITICAL_SECTION )
#define PCRITICALSECTION LPCRITICAL_SECTION
#else
#define InitializeCriticalSection InitializeCriticalSec
typedef struct critical_section_tag *PCRITICALSECTION;
#endif
/* attempts to enter the critical section, and does not block.
   Returns
   If it enters the return is 1, else the return is 0.
   Parameters
   pcs :    pointer to a critical section
   prior :  if not NULL, prior will be set to the current thread
            ID of the owning thread.                             */
#ifndef USE_NATIVE_CRITICAL_SECTION
MEM_PROC  int32_t MEM_API  EnterCriticalSecNoWaitEx ( PCRITICALSECTION pcs, THREAD_ID *prior DBG_PASS );
#define EnterCriticalSecNoWait( pcs,prior ) EnterCriticalSecNoWaitEx( pcs, prior DBG_SRC )
#else
#define EnterCriticalSecNoWait( pcs,prior ) TryEnterCriticalSection( (pcs) )
#endif
/* <combine sack::timers::EnterCriticalSecNoWaitEx@PCRITICALSECTION@THREAD_ID *prior>
   \ \                                                                                */
//#define EnterCriticalSecNoWait( pcs,prior ) EnterCriticalSecNoWaitEx( (pcs),(prior) DBG_SRC )
/* clears all members of a CRITICALSECTION.  Same as memset( pcs, 0, sizeof( CRITICALSECTION ) ); */
#ifndef USE_NATIVE_CRITICAL_SECTION
MEM_PROC  void MEM_API  InitializeCriticalSec ( PCRITICALSECTION pcs );
#else
#define InitializeCriticalSec(pcs)  InitializeCriticalSection(pcs)
#endif
/* Get a count of how many times a critical section is locked */
//MEM_PROC  uint32_t MEM_API  CriticalSecOwners ( PCRITICALSECTION pcs );
/* Namespace of all memory related functions for allocating and
   releasing memory.                                            */
#ifdef __cplusplus
 // namespace timers
}
 // namespace sack
}
using namespace sack::timers;
#endif
#ifdef __cplusplus
namespace sack {
/* Memory namespace contains functions for allocating and
   releasing memory. Also contains methods for accessing shared
   memory (if available on the target platform).
   Allocate
   Release
   Hold
   OpenSpace                                                    */
namespace memory {
#endif
typedef struct memory_block_tag* PMEM;
// what is an abstract name for the memory mapping handle...
// where is a filename for the filebacking of the shared memory
// DigSpace( "Picture Memory", "Picture.mem", 100000 );
/* <combinewith sack::memory::OpenSpaceExx@CTEXTSTR@CTEXTSTR@uintptr_t@uintptr_t *@uint32_t*>
   \ \                                                                                 */
MEM_PROC  POINTER MEM_API  OpenSpace ( CTEXTSTR pWhat, CTEXTSTR pWhere, size_t *dwSize );
/* <unfinished>
   Open a shared memory region. The region may be named with a
   text string (this does not work under linux platforms, and
   the name of the file to back the shared region is the sharing
   point). The region may be backed with a file (and must be if
   it is to be shared on linux.
   If the region exists by name, the region is opened, and a
   pointer to that region is returned.
   If the file exists, the file is opened, and mapped into
   memory, and a pointer to the file backed memory is returned.
   if the file does not exist, and the size parameter passed is
   not 0, then the file is created, and expanded to the size
   requested. The bCreate flag is set to true.
   If NULL is passed for pWhat and pWhere, then a block of
   memory is allocated in system memory, backed by pagefile.
   if dwSize is 0, then the region is specified for open only,
   and will not create.
   Parameters
   pWhat :     String to a named shared memory region. NULL is
               unnamed.
   pWhere :    Filename to back the shared memory with. The file
               name itself may also be used to share the memory.
   address :   A base address to map the memory at. If 0,
               specifies do not care.
   dwSize :    pointer to a uintptr_t that defines the size to
               create. If 0, then the region is only opened. The
               size of the region opened is set back into this
               value after it is opened.
   bCreated :  pointer to a boolean to indicate whether the space
               was created or not.
   Returns
   Pointer to region requested to be opened. NULL on failure.
   Example
   Many examples of this are appropriate.
   1) Open or create a file backed shared space.
   2) Open a file for direct memory access, the file is loaded
   into memory by system paging routines and not any API.         */
MEM_PROC  POINTER MEM_API  OpenSpaceExx ( CTEXTSTR pWhat, CTEXTSTR pWhere, uintptr_t address
	, size_t *dwSize, uint32_t* bCreated );
/* <combine sack::memory::OpenSpaceExx@CTEXTSTR@CTEXTSTR@uintptr_t@uintptr_t *@uint32_t*>
   \ \                                                                             */
#define OpenSpaceEx( what,where,address,psize) OpenSpaceExx( what,where,address,psize,NULL )
/* Closes a shared memory region. Calls CloseSpaceEx() with
   bFinal set TRUE.
   Parameters
   pMem :  pointer to a memory region opened by OpenSpace.  */
MEM_PROC  void MEM_API  CloseSpace ( POINTER pMem );
/* Closes a memory region. Release can also be used to close
   opened spaces.
   Parameters
   pMem :    pointer to a memory region opened with OpenSpace()
   bFinal :  If final is set, the file used for backing the shared
             region is deleted.                                    */
MEM_PROC  void MEM_API  CloseSpaceEx ( POINTER pMem, int bFinal );
/* This can give the size back of a memory space.
   Returns
   The size of the memory block.
   Parameters
   pMem :  pointer to a block of memory that was opened with
           OpenSpace().                                      */
MEM_PROC  uintptr_t MEM_API  GetSpaceSize ( POINTER pMem );
/* even if pMem is just a POINTER returned from OpenSpace this
   will create a valid heap pointer.
   will result TRUE if a valid heap is present will result FALSE
   if heap is not able to init (has content)
   Parameters
   pMem :    pointer to a memory space to setup as a heap.
   dwSize :  size of the memory space pointed at by pMem.        */
MEM_PROC  int MEM_API  InitHeap( PMEM pMem, size_t dwSize );
/* Dumps all blocks into the log.
   Parameters
   pHeap :     Heap to dump. If NULL or unspecified, dump the
               default heap.
   bVerbose :  Specify to dump each block's information,
               otherwise only summary information is generated. */
MEM_PROC  void MEM_API  DebugDumpHeapMemEx ( PMEM pHeap, LOGICAL bVerbose );
/* <combine sack::memory::DebugDumpHeapMemEx@PMEM@LOGICAL>
   Logs all of the blocks tracked in a specific heap.
   Parameters
   Heap :  Heap to dump the memory blocks of.              */
#define DebugDumpHeapMem(h)     DebugDumpMemEx( (h), TRUE )
/* <combine sack::memory::DebugDumpHeapMemEx@PMEM@LOGICAL>
   \ \                                                     */
MEM_PROC  void MEM_API  DebugDumpMemEx ( LOGICAL bVerbose );
/* Dumps all tracked heaps.
   Parameters
   None.                    */
#define DebugDumpMem()     DebugDumpMemEx( TRUE )
/* Dumps a heap to a specific file.
   Parameters
   pHeap :      Heap. If NULL or unspecified, dumps default heap.
   pFilename :  name of the file to write output to.              */
MEM_PROC  void MEM_API  DebugDumpHeapMemFile ( PMEM pHeap, CTEXTSTR pFilename );
/* <combine sack::memory::DebugDumpHeapMemFile@PMEM@CTEXTSTR>
   \ \                                                        */
MEM_PROC  void MEM_API  DebugDumpMemFile ( CTEXTSTR pFilename );
#ifdef __GNUC__
MEM_PROC  POINTER MEM_API  HeapAllocateAlignedEx ( PMEM pHeap, size_t dwSize, uint16_t alignment DBG_PASS ) __attribute__( (malloc) );
MEM_PROC  POINTER MEM_API  HeapAllocateEx ( PMEM pHeap, uintptr_t nSize DBG_PASS ) __attribute__((malloc));
MEM_PROC  POINTER MEM_API  AllocateEx ( uintptr_t nSize DBG_PASS ) __attribute__((malloc));
#else
/* \ \
   Parameters
   pHeap :  pointer to a heap which was initialized with
            InitHeap()
   Size :   Size of block to allocate                    */
MEM_PROC  POINTER MEM_API  HeapAllocateEx ( PMEM pHeap, uintptr_t nSize DBG_PASS );
/* \ Parameters
pHeap :  pointer to a heap which was initialized with
InitHeap()
Size :   Size of block to allocate
Alignment : count of bytes to return block on (1,2,4,8,16,32)  */
MEM_PROC  POINTER MEM_API  HeapAllocateAlignedEx( PMEM pHeap, uintptr_t nSize, uint16_t alignment DBG_PASS );
/* Allocates a block of memory of specific size. Debugging
   information if passed is recorded on the block.
   Parameters
   size :  size of the memory block to create              */
MEM_PROC  POINTER MEM_API  AllocateEx ( uintptr_t nSize DBG_PASS );
#endif
/* A simple macro to allocate a new single unit of a structure. Adds
   a typecast automatically to be (type*) so C++ compilation is
   clean. Does not burden the user with extra typecasts. This,
   being in definition use means that all other things that are
   typecast are potentially error prone. Memory is considered
   uninitialized.
   Parameters
   type :  type to allocate
   Example
   <code lang="c++">
   int *p_int = New( int );
   </code>                                                           */
#define New(type) ((type*)HeapAllocate(0,sizeof(type)))
/* Reallocates an array of type.
   Parameters
   type :  type to use for sizeof(type) * sz for resulting size.
   p :     pointer to realloc
   sz :    count of elements in the array                        */
#define Renew(type,p,sz) ((type*)HeapReallocate(0,p, sizeof(type)*sz))
/* an advantage of C, can define extra space at end of structure
   which is allowed to carry extra data, which is unknown by
   other code room for exploits rock.
   Parameters
   type :   passed to sizeof()
   extra :  Number of additional bytes to allocate beyond the
            sizeof( type )
   Example
   Create a text segment plus 18 characters of data. (This
   should not be done, use SegCreate instead)
   <code lang="c#">
   PTEXT text = NewPlus( TEXT, 18 );
   </code>                                                       */
#define NewPlus(type,extra) ((type*)HeapAllocate(0,sizeof(type)+(extra)))
/* Allocate a new array of type.
   Parameters
   type :   type to determine size of array element to allocate.
   count :  count of elements to allocate in the array.
   Returns
   A pointer to type. (this is important, since in C++ it's cast
   correctly to the destination type).                           */
#define NewArray(type,count) ((type*)HeapAllocate(0,(uintptr_t)(sizeof(type)*(count))))
/* Allocate sizeof(type). Will invoke some sort of registered
   initializer
   Parameters
   type :  type to allocate for. Passes the name of the type so
           the allocator can do a registered procedure lookup and
           invok an initializer for the type.                     */
//#define NewObject(type) ((type*)FancyAllocate(sizeof(type),#type DBG_SRC))
#ifdef __cplusplus
/* A 'safe' release macro. casts the block to the type to
   release. Makes sure the pointer being released is the type
   specified.
   Parameters
   type :   type of the variable
   thing :  the thing to actually release.                    */
#  ifdef _DEBUG
#    define Deallocate(type,thing) for(type _zzqz_tmp=thing;ReleaseEx((POINTER)(_zzqz_tmp)DBG_SRC),0;)
#  else
#    define Deallocate(type,thing) ReleaseEx((POINTER)(thing)DBG_SRC)
#  endif
#else
#  define Deallocate(type,thing) (ReleaseEx((POINTER)(thing)DBG_SRC))
#endif
/* <combine sack::memory::HeapAllocateEx@PMEM@uintptr_t nSize>
   \ \                                                        */
#define HeapAllocate(heap, n) HeapAllocateEx( (heap), (n) DBG_SRC )
   /* <combine sack::memory::HeapAllocateAlignedEx@PMEM@uintptr_t@uint32_t>
   \ \                                                        */
#define HeapAllocateAligned(heap, n, m) HeapAllocateAlignedEx( (heap), (n), m DBG_SRC )
   /* <combine sack::memory::AllocateEx@uintptr_t nSize>
   \ \                                               */
#ifdef FIX_RELEASE_COM_COLLISION
#else
#define Allocate( n ) HeapAllocateEx( (PMEM)0, (n) DBG_SRC )
#endif
//MEM_PROC  POINTER MEM_API  AllocateEx ( uintptr_t nSize DBG_PASS );
//#define Allocate(n) AllocateEx(n DBG_SRC )
MEM_PROC  POINTER MEM_API  GetFirstUsedBlock ( PMEM pHeap );
/* Releases an allocated block. Memory becomes free to allocate
   again. If debugging information is passed, the releasing
   source and line is recorded in the block. (can be used to
   find code deallocating memory it shouldn't).
   This also works with Hold(), and decrements the hold counter.
   If there are no more holds on the block, then the block is
   released.
   Parameters
   p :  pointer to allocated block to release.                   */
MEM_PROC  POINTER MEM_API  ReleaseEx ( POINTER pData DBG_PASS ) ;
/* <combine sack::memory::ReleaseEx@POINTER pData>
   \ \                                             */
#ifdef FIX_RELEASE_COM_COLLISION
#else
/* <combine sack::memory::ReleaseEx@POINTER pData>
   \ \                                             */
#define Release(p) ReleaseEx( (p) DBG_SRC )
#endif
/* Adds a usage count to a block of memory. For each count
   added, an additional release must be used. This can be used
   to keep a copy of the block, even if some other code
   automatically releases it.
   Parameters
   pointer :  pointer to a block of memory that was Allocate()'d.
   Example
   Allocate a block of memory, and release it properly. But we
   passed it to some function. That function wanted to keep a
   copy of the block, so it can apply a hold. It needs to later
   do a Release again to actually free the memory.
   <code lang="c++">
   POINTER p = Allocate( 32 );
   call_some_function( p );
   Release( p );
   void call_some_function( POINTER p )
   {
      static POINTER my_p_copy;
      my_p_copy = p;
      Hold( p );
   }
   </code>                                                        */
MEM_PROC  POINTER MEM_API  HoldEx ( POINTER pData DBG_PASS  );
/* <combine sack::memory::HoldEx@POINTER pData>
   \ \                                          */
#define Hold(p) HoldEx(p DBG_SRC )
/* This can be used to add additional space after the end of a
   memory block.
   Parameters
   pHeap :   If NULL or not specified, uses the common memory heap.
   source :  pointer to the block to pre\-allocate. If NULL, a new
             memory block will be allocated that is filled with 0.
   size :    the new size of the block.
   Returns
   A pointer to a new block of memory that is the new size.
   Remarks
   If the size specified for the new block is larger than the
   previous size of the block, the curernt data is copied to the
   beginning of the new block, and the memory after the existing
   content is cleared to 0.
   If the size specified for the new block is smaller than the
   previous size, the end of the original block is not copied to
   the new block.
   If NULL is passed as the source block, then a new block
   filled with 0 is created.                                        */
MEM_PROC  POINTER MEM_API  HeapReallocateAlignedEx ( PMEM pHeap, POINTER source, uintptr_t size, uint16_t alignment DBG_PASS );
MEM_PROC  POINTER MEM_API  HeapReallocateEx ( PMEM pHeap, POINTER source, uintptr_t size DBG_PASS );
/* <combine sack::memory::HeapReallocateEx@PMEM@POINTER@uintptr_t size>
   \ \                                                                 */
#define HeapReallocateAligned(heap,p,sz,al) HeapReallocateEx( (heap),(p),(sz),(al) DBG_SRC )
#define HeapReallocate(heap,p,sz) HeapReallocateEx( (heap),(p),(sz) DBG_SRC )
/* <combine sack::memory::HeapReallocateEx@PMEM@POINTER@uintptr_t size>
   \ \                                                                 */
MEM_PROC  POINTER MEM_API  ReallocateEx ( POINTER source, uintptr_t size DBG_PASS );
/* <combine sack::memory::ReallocateEx@POINTER@uintptr_t size>
   \ \                                                        */
#ifdef FIX_RELEASE_COM_COLLISION
#else
#define Reallocate(p,sz) ReallocateEx( (p),(sz) DBG_SRC )
#endif
/* This can be used to add additional space before the beginning
   of a memory block.
   Parameters
   pHeap :   If NULL or not specified, uses the common memory heap.
   source :  pointer to the block to pre\-allocate. If NULL, a new
             memory block will be allocated that is filled with 0.
   size :    the new size of the block.
   Returns
   A pointer to a new block of memory that is the new size.
   Remarks
   If the size specified for the new block is larger than the
   previous size of the block, the content data is copied to the
   end of the new block, and the memory leading up to the block
   is cleared to 0.
   If the size specified for the new block is smaller than the
   previous size, the end of the original block is not copied to
   the new block.
   If NULL is passed as the source block, then a new block
   filled with 0 is created.                                        */
MEM_PROC  POINTER MEM_API  HeapPreallocateEx ( PMEM pHeap, POINTER source, uintptr_t size DBG_PASS );
/* <combine sack::memory::HeapPreallocateEx@PMEM@POINTER@uintptr_t size>
   \ \                                                                  */
#define HeapPreallocate(heap,p,sz) HeapPreallocateEx( (heap),(p),(sz) DBG_SRC )
/* <combine sack::memory::HeapPreallocateEx@PMEM@POINTER@uintptr_t size>
   \ \                                                                  */
MEM_PROC  POINTER MEM_API  PreallocateAlignedEx ( POINTER source, uintptr_t size, uint16_t alignment DBG_PASS );
MEM_PROC  POINTER MEM_API  PreallocateEx ( POINTER source, uintptr_t size DBG_PASS );
/* <combine sack::memory::PreallocateEx@POINTER@uintptr_t size>
   \ \                                                         */
#define PreallocateAligned(p,sz,al) PreallocateAlignedEx( (p),(sz),(al) DBG_SRC )
#define Preallocate(p,sz) PreallocateEx( (p),(sz) DBG_SRC )
/* Moves a block of memory from one heap to another.
   Parameters
   pNewHeap :  heap target to move the block to.
   source :    source block to move \- pointer to the data in the
               block.
   Remarks
   Since each block remembers its own size, it is possible to
   move a block from one heap to another. A heap might be a
   memory mapped file at a specific address for instance.         */
MEM_PROC  POINTER MEM_API  HeapMoveEx ( PMEM pNewHeap, POINTER source DBG_PASS );
/* <combine sack::memory::HeapMoveEx@PMEM@POINTER source>
   \ \                                                    */
#define HeapMove(h,s) HeapMoveEx( (h), (s) DBG_SRC )
/* \returns the size of a memory block which was Allocate()'d.
   Parameters
   pData :  pointer to a allocated memory block.
   Returns
   The size of the block that was specified by the Allocate(). */
MEM_PROC uintptr_t MEM_API  SizeOfMemBlock ( CPOINTER pData );
/* \returns the allocation alignment of a memory block which was Allocate()'d.
Parameters
pData :  pointer to a allocated memory block.
Returns
The alignment of the block that was specified from Allocate(). */
MEM_PROC uint16_t  AlignOfMemBlock( CPOINTER pData );
/* not so much of a fragment as a consolidation. Finds a free
   spot earlier in the heap and attempts to move the block
   there. This can help alleviate heap fragmentation.
   Parameters
   ppMemory :  pointer to a pointer to memory which might move */
MEM_PROC  LOGICAL MEM_API  Defragment ( POINTER *ppMemory );
/* \ \
   Parameters
   pHeap :        pointer to a heap
   pFree :        pointer to a 32 bit value to receive the size
                  of free space
   pUsed :        pointer to a 32 bit value to receive the size
                  of used space
   pChunks :      pointer to a 32 bit value to receive the total
                  count of chunks.
   pFreeChunks :  pointer to a 32 bit value to receive the total
                  count of free chunks.
   Remarks
   It looks like DBG_PASS parameter isn't used... not sure why
   it would here, there is no allocate or delete.
   The count of allocated chunks can be gotten by subtracting
   FreeChunks from Chunks.
   Example
   <code lang="c++">
   uint32_t free;
   uint32_t used;
   uint32_t chunks;
   uint32_t free_chunks;
   GetHeapMemStatsEx( NULL, &amp;free, &amp;used, &amp;chunks, &amp;free_chunks );
   </code>                                                                         */
MEM_PROC  void MEM_API  GetHeapMemStatsEx ( PMEM pHeap, uint32_t *pFree, uint32_t *pUsed, uint32_t *pChunks, uint32_t *pFreeChunks DBG_PASS );
/* <combine sack::memory::GetHeapMemStatsEx@PMEM@uint32_t *@uint32_t *@uint32_t *@uint32_t *pFreeChunks>
   \ \                                                                               */
#define GetHeapMemStats(h,f,u,c,fc) GetHeapMemStatsEx( h,f,u,c,fc DBG_SRC )
//MEM_PROC  void MEM_API  GetHeapMemStats ( PMEM pHeap, uint32_t *pFree, uint32_t *pUsed, uint32_t *pChunks, uint32_t *pFreeChunks );
MEM_PROC  void MEM_API  GetMemStats ( uint32_t *pFree, uint32_t *pUsed, uint32_t *pChunks, uint32_t *pFreeChunks );
/* Sets whether to log allocations or not.
   \returns the prior state of logging...
   Parameters
   bTrueFalse :  if TRUE, allocation logging is turned on. Enables
                 logging when each block is Allocated, Released,
                 or Held.                                          */
MEM_PROC  int MEM_API  SetAllocateLogging ( LOGICAL bTrueFalse );
/* disables storing file/line, also disables auto GetMemStats
   checking
   Parameters
   bDisable :  set to TRUE to disable allocate debug logging. */
MEM_PROC  int MEM_API  SetAllocateDebug ( LOGICAL bDisable );
/* disables auto GemMemStats on every allocate/release/Hold
   GetMemStats will evaluate each and every block allocated in
   memory and inspect it for corruption.
   Parameters
   bDisable :  set to TRUE to disable auto mem check.          */
MEM_PROC  int MEM_API  SetManualAllocateCheck ( LOGICAL bDisable );
/* Sets whether to log critical sections or not.
   \returns the prior state of logging...
   Parameters
   bTrueFalse :  if TRUE, critical section logging is turned on. Logs
                 when each thread enters or leaves a
                 CRITICIALSECTION.                                    */
MEM_PROC  int MEM_API  SetCriticalLogging ( LOGICAL bTrueFalse );
/* Sets the minimum size to allocate. If a block size less than
   this is allocated, then this much is actually allocated.
   Parameters
   nSize :  Specify the minimum allocation size                 */
MEM_PROC  void MEM_API  SetMinAllocate ( size_t nSize );
/* Sets how much a heap is expanded by when it is out of space. Default
   is like 512k.
   Parameters
   dwSize :  the new size to expand heaps by.
   Remarks
   Probably internally, this is rounded up to the next 4k
   boundary.                                                            */
MEM_PROC  void MEM_API  SetHeapUnit ( size_t dwSize );
/* Multi-processor safe exchange operation. Returns the prior
   value at the pointer.
   Parameters
   p :    pointer to a volatile 64 bit value.
   val :  a new 64 bit value to put at (*p)
   Example
   <code lang="c#">
   uint64_t value = 13;
   uint64_t oldvalue = LockedExchange64( &amp;value, 15 );
   // old value will be 13
   // value will be 15
   </code>                                                    */
MEM_PROC  uint64_t MEM_API  LockedExchange64 ( volatile uint64_t* p, uint64_t val );
/* A multi-processor safe increment of a variable.
   Parameters
   p :  pointer to a 32 bit value to increment.    */
MEM_PROC  uint32_t MEM_API  LockedIncrement ( volatile uint32_t* p );
/* Does a multi-processor safe decrement on a variable.
   Parameters
   p :  pointer to a 32 bit value to decrement.         */
MEM_PROC  uint32_t MEM_API  LockedDecrement ( volatile uint32_t* p );
#ifdef __cplusplus
// like also __if_assembly__
//extern "C" {
#endif
#ifdef __64__
#define LockedExchangePtrSzVal(a,b) LockedExchange64((volatile uint64_t*)(a),b)
#else
#define LockedExchangePtrSzVal(a,b) LockedExchange((volatile uint32_t*)(a),b)
#endif
/* Multiprocessor safe swap of the contents of a variable with a
   new value, and result with the old variable.
   Parameters
   p :    pointer to a 32 bit value to exchange
   val :  value to set into the variable
   Returns
   The prior value in p.
   Example
   <code>
   uint32_t variable = 0;
   uint32_t oldvalue = LockedExchange( &amp;variable, 1 );
   </code>                                                       */
MEM_PROC  uint32_t MEM_API  LockedExchange ( volatile uint32_t* p, uint32_t val );
/* Sets a 32 bit value into memory. If the length to set is not
   a whole number of 32 bit words, the last bytes may contain
   the low 16 bits of the value and the low 8 bits.
   Parameters
   p :   pointer to memory to set
   n :   32 bit value to set memory with
   sz :  length to set
   Remarks
   Writes as many 32 it values as will fit in sz.
   If (sz &amp; 2), the low 16 bits of n are written at the end.
   then if ( sz &amp; 1 ) the low 8 bits of n are written at the
   end.                                                          */
MEM_PROC  void MEM_API  MemSet ( POINTER p, uintptr_t n, size_t sz );
//#define _memset_ MemSet
/* memory copy operation. not safe when buffers overlap. Performs
   platform-native memory stream operation to copy from one
   place in memory to another. (32 or 64 bit operations as
   possible).
   Parameters
   pTo :    Memory to copy to
   pFrom :  memory to copy from
   sz :     size of block of memory to copy                       */
MEM_PROC  void MEM_API  MemCpy ( POINTER pTo, CPOINTER pFrom, size_t sz );
//#define _memcpy_ MemCpy
/* Binary byte comparison of one block of memory to another. Results
   \-1 if less, 1 if more and 0 if equal.
   Parameters
   pOne :  pointer to memory one
   pTwo :  pointer to some other memory
   sz :    count of bytes to compare
   Returns
   0 if equal
   \-1 if the first different byte in pOne is less than pTwo.
   1 if the first different byte in pOne is more than pTwo.          */
MEM_PROC  int MEM_API  MemCmp ( CPOINTER pOne, CPOINTER pTwo, size_t sz );
	/* nothing.
   does nothing, returns nothing. */
//#define memnop(mem,sz,comment)
/* Compares two strings. Must match exactly.
   Parameters
   s1 :  string to compare
   s2 :  string to compare
   Returns
   0 if equal.
   1 if (s1 \>s2)
   \-1 if (s1 \< s2)
   if s1 is NULL and s2 is not NULL, return is -1.
   if s2 is NULL and s1 is not NULL, return is 1.
	if s1 and s2 are NULL return is 0.              */
#ifdef StrCmp
#undef StrCmp
 // StrCmp
#endif
MEM_PROC  int MEM_API  StrCmp ( CTEXTSTR pOne, CTEXTSTR pTwo );
/* Compares two strings, case insensitively.
   Parameters
   s1 :  string to compare
   s2 :  string to compare
   Returns
   0 if equal.
   1 if (s1 \>s2)
   \-1 if (s1 \< s2)
   if s1 is NULL and s2 is not NULL, return is -1.
   if s2 is NULL and s1 is not NULL, return is 1.
   if s1 and s2 are NULL return is 0.              */
MEM_PROC  int MEM_API  StrCaseCmp ( CTEXTSTR s1, CTEXTSTR s2 );
/* String insensitive case comparison with maximum length
   specified.
   Parameters
   s1 :      string to compare
   s2 :      string to compare
   maxlen :  maximum character required to match
   Returns
   0 if equal up to the number of characters.
   1 if (s1 \>s2)
   \-1 if (s1 \< s2)
   if s1 is NULL and s2 is not NULL, return is -1.
   if s2 is NULL and s1 is not NULL, return is 1.
   if s1 and s2 are NULL return is 0.                     */
MEM_PROC  int MEM_API  StrCaseCmpEx ( CTEXTSTR s1, CTEXTSTR s2, size_t maxlen );
/* This searches a string for the first character that matches
   some specified character.
   A custom strchr function, since microsoft is saying this is
   an unsafe function. This Compiles to compare native strings,
   if UNICODE uses unicode, otherwise uses 8 bit characters.
   Parameters
   s1 :  String to search
   c :   Character to find
   Returns
   pointer in string to search that is the first character that
   matches. NULL if no character matches.
   Note
   This flavor is the only one on C where operator overloading
   cannot switch between CTEXTSTR and TEXTSTR parameters, to
   \result with the correct type. If a CTEXTSTR is passed to
   this it should result with a CTEXTSTR, but if that's the only
   choice, then the result of this is never modifiable, even if
	it is a pointer to a non-const TEXTSTR.                       */
MEM_PROC  CTEXTSTR MEM_API  StrChr ( CTEXTSTR s1, TEXTCHAR c );
/* copy S2 to S1, with a maximum of N characters.
   The last byte of S1 will always be a 'nul'. If S2 was longer
   than S1, then it will be truncated to fit within S1. Perferred
   method over this is SaveText or StrDup.
   Parameters
   s1 :      desitnation TEXTCHAR buffer
   s2 :      source string
   length :  the maximum number of characters that S1 can hold. (this
             is not a size, but is a character count)                 */
MEM_PROC  TEXTSTR MEM_API  StrCpyEx ( TEXTSTR s1, CTEXTSTR s2, size_t n );
/* copy S2 to S1. This is 'unsafe', since neither paramter's
   size is known. Prefer StrCpyEx which passes the maximum
   length for S1.
   Parameters
   s1 :  desitnation TEXTCHAR buffer
   s2 :  source string                                       */
MEM_PROC  TEXTSTR MEM_API  StrCpy ( TEXTSTR s1, CTEXTSTR s2 );
/* \Returns the count of characters in a string.
   Parameters
   s :  string to measure
   Returns
   length of string.                             */
MEM_PROC  size_t MEM_API  StrLen ( CTEXTSTR s );
/* Get the length of a string in C chars.
   Parameters
   s :  char * to count.
   Returns
   the length of s. If s is NULL, return 0. */
MEM_PROC  size_t MEM_API  CStrLen ( char const*s );
/* Finds the last instance of a character in a string.
   Parameters
   s1 :  String to search in
   c :   character to find
   Returns
   NULL if character is not in the string.
   a pointer to the last character in s1 that matches c. */
MEM_PROC  CTEXTSTR MEM_API  StrRChr ( CTEXTSTR s1, TEXTCHAR c );
#ifdef __cplusplus
/* This searches a string for the first character that matches
   some specified character.
   A custom strchr function, since microsoft is saying this is
   an unsafe function. This Compiles to compare native strings,
   if UNICODE uses unicode, otherwise uses 8 bit characters.
   Parameters
   s1 :  String to search
   c :   Character to find
   Returns
   pointer in string to search that is the first character that
   matches. NULL if no character matches.
   Note
   This second flavor is only available on C++ where operator
   overloading will switch between CTEXTSTR and TEXTSTR
   \parameters, to result with the correct type. If a CTEXTSTR
   is passed to this it should result with a CTEXTSTR, but if
   that's the only choice, then the result of this is never
   modifiable, even if it is a pointer to a non-const TEXTSTR.  */
MEM_PROC  TEXTSTR MEM_API  StrChr ( TEXTSTR s1, TEXTCHAR c );
/* This searches a string for the last character that matches
   some specified character.
   A custom strrchr function, since microsoft is saying this is
   an unsafe function. This Compiles to compare native strings,
   if UNICODE uses unicode, otherwise uses 8 bit characters.
   Parameters
   s1 :  String to search
   c :   Character to find
   Returns
   pointer in string to search that is the first character that
   matches. NULL if no character matches.
   Note
   This second flavor is only available on C++ where operator
   overloading will switch between CTEXTSTR and TEXTSTR
   \parameters, to result with the correct type. If a CTEXTSTR
   is passed to this it should result with a CTEXTSTR, but if
   that's the only choice, then the result of this is never
   modifiable, even if it is a pointer to a non-const TEXTSTR.  */
MEM_PROC  TEXTSTR MEM_API  StrRChr ( TEXTSTR s1, TEXTCHAR c );
/* <combine sack::memory::StrCmp@CTEXTSTR@CTEXTSTR>
   \ \                                              */
MEM_PROC  int MEM_API  StrCmp ( const char * s1, CTEXTSTR s2 );
#endif
/* <combine sack::memory::StrCmp@char *@CTEXTSTR>
   \ \                                            */
MEM_PROC  int MEM_API  StrCmpEx ( CTEXTSTR s1, CTEXTSTR s2, INDEX maxlen );
/* Finds an instance of a string in another string.
   Custom implementation because strstr is declared unsafe, and
   to handle switching between unicode and char.
   Parameters
   s1 :  the string to search in
   s2 :  the string to locate
   Returns
   NULL if s2 is not in s1.
   The beginning of the string in s1 that matches s2.
   Example
   <code lang="c++">
   TEXTCHAR const *found = StrStr( "look in this string", "in" );
                                               ^returns a pointer to here.
   </code>                                                                        */
MEM_PROC  CTEXTSTR MEM_API  StrStr ( CTEXTSTR s1, CTEXTSTR s2 );
#ifdef __cplusplus
/* Finds an instance of a string in another string.
   Custom implementation because strstr is declared unsafe, and
   to handle switching between unicode and char.
   Parameters
   s1 :  the string to search in
   s2 :  the string to locate
   Returns
   NULL if s2 is not in s1.
   The beginning of the string in s1 that matches s2.
   Example
   <code>
   TEXTCHAR *writable_string = StrDup( "look in this string" );
   TEXTCHAR *found = StrStr( writable_string, "in" );
   // returns a pointer to 'in' in the writable string, which can then be modified.
   </code>                                                                          */
MEM_PROC  TEXTSTR MEM_API  StrStr ( TEXTSTR s1, CTEXTSTR s2 );
#endif
/* Searches for one string in another. Compares case
   insensitively.
   Parameters
   s1 :  string to search in
   s2 :  string to locate
   See Also
   <link sack::memory::StrStr@CTEXTSTR@CTEXTSTR, StrStr> */
MEM_PROC  CTEXTSTR MEM_API  StrCaseStr ( CTEXTSTR s1, CTEXTSTR s2 );
/* This duplicates a block of memory.
   Parameters
   p :  pointer to a block of memory that was allocated.
   Returns
   a pointer to a new block of memory that has the same content
   as the original.                                             */
MEM_PROC  POINTER MEM_API  MemDupEx ( CPOINTER thing DBG_PASS );
/* <combine sack::memory::MemDupEx@CPOINTER thing>
   \ \                                             */
#define MemDup(thing) MemDupEx(thing DBG_SRC )
/* Duplicates a string, and returns a pointer to the copy.
   Parameters
   original :  string to duplicate                         */
MEM_PROC  TEXTSTR MEM_API  StrDupEx ( CTEXTSTR original DBG_PASS );
/* Translates from a TEXTCHAR string to a char string. Probably
   only for UNICODE to non wide translation points.
   Parameters
   original :  string to duplicate                              */
MEM_PROC  char *  MEM_API  CStrDupEx ( CTEXTSTR original DBG_PASS );
/* Translates from a TEXTCHAR string to a wchar_t string. Probably
   only for UNICODE to non wide translation points.
   Parameters
   original :  string to duplicate                              */
MEM_PROC  wchar_t *  MEM_API  DupTextToWideEx( CTEXTSTR original DBG_PASS );
#define DupTextToWide(s) DupTextToWideEx( s DBG_SRC )
/* Translates from a TEXTCHAR string to a wchar_t string. Probably
   only for UNICODE to non wide translation points.
   Parameters
   original :  string to duplicate                              */
MEM_PROC  char *     MEM_API  DupTextToCharEx( CTEXTSTR original DBG_PASS );
#define DupTextToChar(s) DupTextToCharEx( s DBG_SRC )
/* Translates from a TEXTCHAR string to a wchar_t string. Probably
   only for UNICODE to non wide translation points.
   Parameters
   original :  string to duplicate                              */
MEM_PROC TEXTSTR     MEM_API  DupWideToTextEx( const wchar_t *original DBG_PASS );
#define DupWideToText(s) DupWideToTextEx( s DBG_SRC )
/* Translates from a TEXTCHAR string to a wchar_t string. Probably
   only for UNICODE to non wide translation points.
   Parameters
   original :  string to duplicate                              */
MEM_PROC TEXTSTR     MEM_API  DupCharToTextEx( const char *original DBG_PASS );
#define DupCharToText(s) DupCharToTextEx( s DBG_SRC )
/* Converts from 8 bit char to 16 bit wchar (or no-op if not
   UNICODE compiled)
   Parameters
   original :  original string of C char.
   Returns
   a pointer to a wide character string.                     */
MEM_PROC  TEXTSTR MEM_API  DupCStrEx ( const char * original DBG_PASS );
/* Converts from 8 bit char to 16 bit wchar (or no-op if not
UNICODE compiled)
Parameters
original :  original string of C char.
Returns
a pointer to a wide character string.                     */
MEM_PROC  TEXTSTR MEM_API  DupCStrLenEx( const char * original, size_t chars DBG_PASS );
/* <combine sack::memory::StrDupEx@CTEXTSTR original>
   \ \                                                */
#define StrDup(o) StrDupEx( (o) DBG_SRC )
/* <combine sack::memory::CStrDupEx@CTEXTSTR original>
   \ \                                                 */
#define CStrDup(o) CStrDupEx( (o) DBG_SRC )
/* <combine sack::memory::DupCStrEx@char * original>
   \ \                                               */
#define DupCStr(o) DupCStrEx( (o) DBG_SRC )
/* <combine sack::memory::DupCStrLenEx@char * original@size_t chars>
   \ \                                               */
#define DupCStrLen(o,l) DupCStrLenEx( (o),(l) DBG_SRC )
//------------------------------------------------------------------------
#if 0
// this code was going to provide network oriented shared memory.
#ifndef TRANSPORT_STRUCTURE_DEFINED
typedef uintptr_t PTRANSPORT_QUEUE;
struct transport_queue_tag { uint8_t private_data_here; };
#endif
MEM_PROC  struct transport_queue_tag * MEM_API  CreateQueue ( int size );
MEM_PROC  int MEM_API  EnqueMessage ( struct transport_queue_tag *queue, POINTER msg, int size );
MEM_PROC  int MEM_API  DequeMessage ( struct transport_queue_tag *queue, POINTER msg, int *size );
MEM_PROC  int MEM_API  PequeMessage ( struct transport_queue_tag *queue, POINTER *msg, int *size );
#endif
//------------------------------------------------------------------------
#ifdef __cplusplus
 // namespace memory
}
 // namespace sack
}
using namespace sack::memory;
#if defined( _DEBUG ) || defined( _DEBUG_INFO )
/*
inline void operator delete( void * p )
{ Release( p ); }
#ifdef DELETE_HANDLES_OPTIONAL_ARGS
inline void operator delete (void * p DBG_PASS )
{ ReleaseEx( p DBG_RELAY ); }
#define delete delete( DBG_VOIDSRC )
#endif
//#define deleteEx(file,line) delete(file,line)
#ifdef USE_SACK_ALLOCER
inline void * operator new( size_t size DBG_PASS )
{ return AllocateEx( (uintptr_t)size DBG_RELAY ); }
static void * operator new[]( size_t size DBG_PASS )
{ return AllocateEx( (uintptr_t)size DBG_RELAY ); }
#define new new( DBG_VOIDSRC )
#define newEx(file,line) new(file,line)
#endif
*/
// common names - sometimes in conflict when declaring
// other functions... AND - release is a common
// component of iComObject
//#undef Allocate
//#undef Release
// Hmm wonder where this conflicted....
//#undef LineDuplicate
#else
#ifdef USE_SACK_ALLOCER
inline void * operator new(size_t size)
{ return AllocateEx( size ); }
inline void operator delete (void * p)
{ ReleaseEx( p ); }
#endif
#endif
#endif
#endif
#ifdef __LINUX__
#endif
#ifndef _TIMER_NAMESPACE
#ifdef __cplusplus
#define _TIMER_NAMESPACE namespace timers {
/* define a timer library namespace in C++. */
#define TIMER_NAMESPACE SACK_NAMESPACE namespace timers {
/* define a timer library namespace in C++ end. */
#define TIMER_NAMESPACE_END } SACK_NAMESPACE_END
#else
#define _TIMER_NAMESPACE
#define TIMER_NAMESPACE
#define TIMER_NAMESPACE_END
#endif
#endif
// this is a method replacement to use PIPEs instead of SEMAPHORES
// replacement code only affects linux.
#if defined( __QNX__ ) || defined( __MAC__) || defined( __LINUX__ )
#  if defined( __ANDROID__ ) || defined( EMSCRIPTEN ) || defined( __MAC__ )
// android > 21 can use pthread_mutex_timedop
#    define USE_PIPE_SEMS
#  else
//   Default behavior is to use pthread_mutex_timedlock for wakeable sleeps.
// no semtimedop; no semctl, etc
//#    include <sys/sem.h>
//originally used semctl; but that consumes system resources that are not
//cleaned up when the process exits.
#endif
#endif
#ifdef USE_PIPE_SEMS
#  define _NO_SEMTIMEDOP_
#endif
SACK_NAMESPACE
/* This namespace contains methods for working with timers and
   threads. Since timers are implemented in an asynchronous
   thread, the thread creation and control can be exposed here
   also.
   ThreadTo
   WakeThread
   WakeableSleep [Example]
   AddTimer
   RemoveTimer
   RescheduleTimer
   EnterCriticalSec see Also
 EnterCriticalSecNoWait
   LeaveCriticalSec                                            */
_TIMER_NAMESPACE
#ifdef TIMER_SOURCE
#define TIMER_PROC(type,name) EXPORT_METHOD type CPROC name
#else
/* Defines import export and call method for timers. Looks like
   timers are native calltype by default instead of CPROC.      */
#define TIMER_PROC(type,name) IMPORT_METHOD type CPROC name
#endif
#if defined( __LINUX__ ) || defined( __ANDROID__ )
TIMER_PROC( uint32_t, timeGetTime )( void );
TIMER_PROC( uint32_t, GetTickCount )( void );
TIMER_PROC( void, Sleep )( uint32_t ms );
#endif
/* Function signature for user callbacks passed to AddTimer. */
typedef void (CPROC *TimerCallbackProc)( uintptr_t psv );
/* Adds a new periodic timer. From now, until the timer is
   removed with RemoveTimer, it will call the timer procedure at
   the specified frequency of milliseconds. The delay until the
   first time the timer fires can be specified independant of
   frequency. If it is not specified, the first time the timer
   will get invoked is at +1 frequency from now.
   Parameters
   start :      how long in milliseconds until the timer starts. Can
                be 0 and timer will fire at the next opportunity.
   frequency :  how long the delay is between event invocations,
                in milliseconds.
   callback :   user routine to call when the timer's delay
                expires.
   user :       user data to pass to the callback when it is
                invoked.
   Returns
   a 32 bit ID that identifies the timer for this application.
   Example
   First some setup valid for all timer creations...
   <code lang="c++">
   void CPROC TimerProc( uintptr_t user_data )
   {
       // user_data of the timer is the 'user' parameter passed to AddTimer(Exx)
   }
   </code>
   you might want to save this for something like
   RescheduleTimer
   <code>
   uint32_t timer_id;
   </code>
   Create a simple timer, it will fire at 250 milliseconds from
   now, and again every 250 milliseconds from the time it
   starts.
   <code lang="c++">
   timer_id = AddTimer( 250, TimerProc, 0 );
   </code>
   Create a timer that fires immediately, and 732 milliseconds
   after, passing some value 1234 as user data...
   <code lang="c++">
   timer_id = AddTimerEx( 0, 732, TimerProc, 1234 );
	</code>
	Remarks
	if a timer is dispatched and needs to wait - please link with idlelib, and call Idle.
	this will allow other timers to fire on schedule.  The timer that is waiting is not
	in the list of timers to process.
	*/
TIMER_PROC( uint32_t, AddTimerExx )( uint32_t start, uint32_t frequency
					, TimerCallbackProc callback
					, uintptr_t user DBG_PASS);
/* <combine sack::timers::AddTimerExx@uint32_t@uint32_t@TimerCallbackProc@uintptr_t user>
   \ \                                                                         */
#define AddTimerEx( s,f,c,u ) AddTimerExx( (s),(f),(c),(u) DBG_SRC )
/* <combine sack::timers::AddTimerExx@uint32_t@uint32_t@TimerCallbackProc@uintptr_t user>
   \ \                                                                         */
#define AddTimer( f, c, u ) AddTimerExx( (f), (f), (c), (u) DBG_SRC)
/* Stops a timer. The next time this timer would run, it will be
   removed. If it is currently dispatched, it is safe to remove
   from within the timer itself.
   Parameters
   timer :  32 bit timer ID from AddTimer.                       */
TIMER_PROC( void, RemoveTimer )( uint32_t timer );
/* Reschedule when a timer can fire. The delay can be 0 to make
   wake the timer.
   Parameters
   timer :  32 bit timer identifier from AddTimer.
   delay :  How long before the timer should run now.<p />If 0,
            will issue timer immediately.<p />If not specified,
            using the macro, the default delay is the timer's
            frequency. (can prevent the timer from firing until
            it's frequency from now.)                           */
TIMER_PROC( void, RescheduleTimerEx )( uint32_t timer, uint32_t delay );
/* <combine sack::timers::RescheduleTimerEx@uint32_t@uint32_t>
   \ \                                               */
TIMER_PROC( void, RescheduleTimer )( uint32_t timer );
/* Changes the frequency of a timer. Reschedule timer only
   changes the next time it fires, this can adjust the
   frequency. The simple ChangeTimer macro is sufficient.
   Parameters
   ID :         32 bit ID of the time created by AddTimer.
   initial :    initial delay of the timer. (Might matter if the
                timer hasn't fired the first time)
   frequency :  new delay between timer callback invokations.    */
TIMER_PROC( void, ChangeTimerEx )( uint32_t ID, uint32_t initial, uint32_t frequency );
/* <combine sack::timers::ChangeTimerEx@uint32_t@uint32_t@uint32_t>
   \ \                                               */
#define ChangeTimer( ID, Freq ) ChangeTimerEx( ID, Freq, Freq )
/* This is the type returned by MakeThread, and passed to
   ThreadTo. This is a private structure, and no definition is
   publicly available, this should be treated like a handle.   */
typedef struct threads_tag *PTHREAD;
/* Function signature for a thread entry point passed to
   ThreadTo.                                             */
typedef uintptr_t (CPROC*ThreadStartProc)( PTHREAD );
/* Function signature for a thread entry point passed to
   ThreadToSimple.                                             */
typedef uintptr_t (*ThreadSimpleStartProc)( POINTER );
/*
  OnThreadCreate allows registering a procedure to run
  when a thread is created.  (Or an existing thread becomes
  tracked within this library, via MakeThread() ).
  It is called once per thread, for each thread created
  after registering the callback.
*/
TIMER_PROC( void, OnThreadCreate )( void ( *v )( void ) );
/* Create a separate thread that starts in the routine
   specified. The uintptr_t value (something that might be a
   pointer), is passed in the PTHREAD structure. (See
   GetThreadParam)
   Parameters
   proc :       starting routine for the thread
   user_data :  some value that can be stored in the number of
                bits that a pointer is. This is passed to the
                proc when the thread starts.
   Example
   See WakeableSleepEx.                                        */
TIMER_PROC( PTHREAD, ThreadToEx )( ThreadStartProc proc, uintptr_t param DBG_PASS );
/* <combine sack::timers::ThreadToEx@ThreadStartProc@uintptr_t param>
   \ \                                                               */
#define ThreadTo(proc,param) ThreadToEx( proc,param DBG_SRC )
/* Create a separate thread that starts in the routine
   specified. The uintptr_t value (something that might be a
   pointer), is passed in the PTHREAD structure. (See
   GetThreadParam)
   Parameters
   proc :       starting routine for the thread
   user_data :  some value that can be stored in the number of
                bits that a pointer is. This is passed to the
                proc when the thread starts.
   Example
   See WakeableSleepEx.                                        */
TIMER_PROC( PTHREAD, ThreadToSimpleEx )( ThreadSimpleStartProc proc, POINTER param DBG_PASS );
/* <combine sack::timers::ThreadToEx@ThreadStartProc@uintptr_t param>
   \ \                                                               */
#define ThreadToSimple(proc,param) ThreadToSimpleEx( proc,param DBG_SRC )
/* \Returns a PTHREAD that represents the current thread. This
   can be used to create a PTHREAD identifier for the main
   thread.
   Parameters
   None.
   Returns
   a pointer to a thread structure that identifies the current
   thread. If this thread already has this structure created,
   the same one results on subsequent MakeThread calls.        */
TIMER_PROC( PTHREAD, MakeThread )( void );
/* This returns the parameter passed as user data to ThreadTo.
   Parameters
   thread :  thread to get the parameter from.
   Example
   See WakeableSleepEx.                                        */
TIMER_PROC( uintptr_t, GetThreadParam )( PTHREAD thread );
/* \returns the numeric THREAD_ID from a PTHREAD.
   Parameters
   thread :  thread to get the system wide unique ID of. */
TIMER_PROC( THREAD_ID, GetThreadID )( PTHREAD thread );
/* \returns the numeric THREAD_ID from a PTHREAD.
   Parameters
   thread :  thread to get the system wide unique ID of. */
TIMER_PROC( THREAD_ID, GetThisThreadID )( void );
/* Symbol defined to pass to Wakeable_Sleep to sleep until
   someone calls WakeThread.                               */
#define SLEEP_FOREVER 0xFFFFFFFF
/* Sleeps a number of milliseconds or until the thread is passed
   to WakeThread.
   Parameters
   dwMilliseconds :  How long to sleep. Can be indefinite if
                     value is SLEEP_FOREVER.
   Example
   <code lang="c++">
   PTHREAD main_thread;
   uintptr_t CPROC WakeMeThread( PTHREAD thread )
   {
      // get the value passed to ThreadTo as user_data.
      uintptr_t user_data = GetThreadParam( thread );
      // let the main thread sleep a little wile
       WakeableSleep( 250 );
      // then wake it up
       WakeThread( main_thread );
       return 0;
   }
   int main( void )
   {
       // save my PTHREAD globally.
       main_thread = MakeThread();
       // create a thread that can wake us
       ThreadTo( WakeMeThread, 0 );
       // demonstrate sleeping
       WakableSleep( SLEEP_FOREVER );
       return 0;
   }
   </code>                                                       */
TIMER_PROC( void, WakeableSleepEx )( uint32_t milliseconds DBG_PASS );
TIMER_PROC( void, WakeableSleep )( uint32_t milliseconds );
TIMER_PROC( void, WakeableNamedSleepEx )( CTEXTSTR name, uint32_t n DBG_PASS );
#define WakeableNamedSleep( name, n )   WakeableNamedSleepEx( name, n DBG_SRC )
TIMER_PROC( void, WakeNamedSleeperEx )( CTEXTSTR name DBG_PASS );
#define WakeNamedSleeper( name )   WakeNamedSleeperEx( name DBG_SRC )
TIMER_PROC( void, WakeableNamedThreadSleepEx )( CTEXTSTR name, uint32_t n DBG_PASS );
#define WakeableNamedThreadSleep( name, n )   WakeableNamedThreadSleepEx( name, n DBG_SRC )
TIMER_PROC( void, WakeNamedThreadSleeperEx )( CTEXTSTR name, THREAD_ID therad DBG_PASS );
#define WakeNamedThreadSleeper( name, thread )   WakeNamedThreadSleeperEx( name, thread DBG_SRC )
#ifdef USE_PIPE_SEMS
TIMER_PROC( int, GetThreadSleeper )( PTHREAD thread );
#endif
/* <combine sack::timers::WakeableSleepEx@uint32_t milliseconds>
   \ \                                                      */
#define WakeableSleep(n) WakeableSleepEx(n DBG_SRC )
/* Wake a thread by ID, if the pThread is not available. Can be
   used cross-process for instance. Although someone could add a
   method to provide a PTHREAD wrapper around THREAD_ID for
   threads in remote processes, this may not be a best practice.
   Parameters
   thread_id :  THREAD_ID from GetMyThreadID, which is a macro
                appropriate for a platform.                      */
TIMER_PROC( void, WakeThreadIDEx )( THREAD_ID thread DBG_PASS );
/* Wake a thread.
   Example
   See WakeableSleepEx.
   Parameters
   pThread :  thread to wake up from a WakeableSleep. */
TIMER_PROC( void, WakeThreadEx )( PTHREAD thread DBG_PASS );
/* <combine sack::timers::WakeThreadIDEx@THREAD_ID thread>
   \ \                                                     */
#define WakeThreadID(thread) WakeThreadIDEx( thread DBG_SRC )
/* <combine sack::timers::WakeThreadEx@PTHREAD thread>
   \ \                                                 */
#define WakeThread(t) WakeThreadEx(t DBG_SRC )
/* This can be checked to see if the THREAD_ID to wake still has
   an event. Sometimes threads end.
   Parameters
   thread :  thread identifier to check to see if it exists/can be
             woken.
   Returns
   TRUE if the thread can be signaled to wake up.
   FALSE if the thread cannot be found or cannot be woken up.      */
TIMER_PROC( int, TestWakeThreadID )( THREAD_ID thread );
/* This can be checked to see if the PTHREAD to wake still has
   an event. Sometimes threads call UnmakeThread(). This is a
   more practical test using a THREAD_ID instead. See
   TestWakeThreadID.
   Returns
   TRUE if the thread can be signaled to wake up.
   FALSE if the thread cannot be found or cannot be woken up.  */
TIMER_PROC( int, TestWakeThread )( PTHREAD thread );
//TIMER_PROC( void, WakeThread )( PTHREAD thread );
TIMER_PROC( void, EndThread )( PTHREAD thread );
/* This tests to see if a pointer to a thread references the
   current thread.
   Parameters
   thread :  thread to check to see if it is the current thread.
   Returns
   TRUE if this thread is the same as the PTHREAD passed.
   otherwise FALSE.
   Example
   <code lang="c++">
   PTHREAD main_thread;
   LOGICAL thread_finished_check;
   uintptr_t CPROC ThreadProc( PTHREAD thread )
   {
       if( IsThisThread( main_thread ) )
            printf( "This thread is not the main thread.\\n" );
       else
            printf( "This is the main thread - cannot happen :)\\n" );
   </code>
   <code>
       // mark that this thread is complete
       thread_finished_check = TRUE;
   </code>
   <code lang="c++">
       // hmm - for some reason, just pass the uintptr_t that was passed to ThreadTo as the result.
       return GetThreadParam( thread );
   }
   int main( void )
   {
        main_thread = MakeThread();
        ThreadTo( ThreadProc, 0 );
        // wait for the thread to finish its thread identity check.
        while( !thread_finished_check )
            Relinquish();
        return 0;
   }
   </code>                                                                                         */
TIMER_PROC( int, IsThisThreadEx )( PTHREAD pThreadTest DBG_PASS );
/* <combine sack::timers::IsThisThreadEx@PTHREAD pThreadTest>
   \ \                                                        */
#define IsThisThread(thread) IsThisThreadEx(thread DBG_SRC)
/* Enter a critical section. Only a single thread may be in a
   critical section, if a second thread attempts to enter the
   section while another thread is in it will block until the
   original thread leaves the section. The same thread may enter
   a critical section multiple times. For each time a critical
   section is entered, the thread must also leave the critical
   section (See LeaveCriticalSection).
   Parameters
   pcs :  pointer to a critical section to enter                 */
TIMER_PROC( LOGICAL, EnterCriticalSecEx )( PCRITICALSECTION pcs DBG_PASS );
/* Leaves a critical section. See EnterCriticalSecEx.
   Parameters
   pcs :  pointer to a critical section.              */
TIMER_PROC( LOGICAL, LeaveCriticalSecEx )( PCRITICALSECTION pcs DBG_PASS );
/* Does nothing. There are no extra resources required for
   critical sections, and the memory is allocated by the
   application.
   Parameters
   pcs :  pointer to critical section to do nothing with.  */
TIMER_PROC( void, DeleteCriticalSec )( PCRITICALSECTION pcs );
#ifdef _WIN32
	TIMER_PROC( HANDLE, GetWakeEvent )( void );
	TIMER_PROC( HANDLE, GetThreadHandle )( PTHREAD thread );
#endif
#ifdef __LINUX__
	TIMER_PROC( pthread_t, GetThreadHandle )(PTHREAD thread);
#endif
#ifdef USE_NATIVE_CRITICAL_SECTION
#define EnterCriticalSec(pcs) EnterCriticalSection( pcs )
#define LeaveCriticalSec(pcs) LeaveCriticalSection( pcs )
#else
/* <combine sack::timers::EnterCriticalSecEx@PCRITICALSECTION pcs>
   \ \                                                             */
#define EnterCriticalSec( pcs ) EnterCriticalSecEx( (pcs) DBG_SRC )
/* <combine sack::timers::LeaveCriticalSecEx@PCRITICALSECTION pcs>
   \ \                                                             */
#define LeaveCriticalSec( pcs ) LeaveCriticalSecEx( (pcs) DBG_SRC )
#endif
TIMER_NAMESPACE_END
#ifdef __cplusplus
using namespace sack::timers;
#endif
#endif
// $Log: timers.h,v $
// Revision 1.37  2005/05/16 19:06:58  jim
// Extend wakeable sleep to know the originator of the sleep.
//
// Revision 1.36  2004/09/29 16:42:51  d3x0r
// fixed queues a bit - added a test wait function for timers/threads
//
// Revision 1.35  2004/07/07 15:33:54  d3x0r
// Cleaned c++ warnings, bad headers, fixed make system, fixed reallocate...
//
// Revision 1.34  2004/05/02 02:04:16  d3x0r
// Begin border exclusive option, define PushMethod explicitly, fix LaunchProgram in timers.h
//
// Revision 1.33  2003/12/10 15:38:25  panther
// Move Sleep and GetTickCount to real code
//
// Revision 1.32  2003/11/02 00:31:47  panther
// Added debuginfo pass to wakethread
//
// Revision 1.31  2003/10/24 14:59:21  panther
// Added Load/Unload Function for system shared library abstraction
//
// Revision 1.30  2003/10/17 00:56:04  panther
// Rework critical sections.
// Moved most of heart of these sections to timers.
// When waiting, sleep forever is used, waking only when
// released... This is preferred rather than continuous
// polling of section with a Relinquish.
// Things to test, event wakeup order uner linxu and windows.
// Even see if the wake ever happens.
// Wake should be able to occur across processes.
// Also begin implmeenting MessageQueue in containers....
// These work out of a common shared memory so multiple
// processes have access to this region.
//
// Revision 1.29  2003/09/21 04:03:30  panther
// Build thread ID with pthread_self and getgid
//
// Revision 1.28  2003/07/29 10:41:25  panther
// Predefine struct threads_tag to avoid warning
//
// Revision 1.27  2003/07/24 22:49:20  panther
// Define callback procs as CDECL
//
// Revision 1.26  2003/07/24 16:56:41  panther
// Updates to expliclity define C procedure model for callbacks and assembly modules - incomplete
//
// Revision 1.25  2003/07/22 15:33:19  panther
// Added comment about idle()
//
// Revision 1.24  2003/04/03 10:10:20  panther
// Add file/line debugging to addtimer
//
// Revision 1.23  2003/03/27 13:47:14  panther
// Immplement a EndThread
//
// Revision 1.22  2003/03/25 08:38:11  panther
// Add logging
//
#ifndef MAXPATH
// windef.h has MAX_PATH
#  define MAXPATH MAX_PATH
#  if (!MAXPATH)
#    undef MAXPATH
#    define MAXPATH 256
#  endif
#endif
#ifndef PATH_MAX
// sometimes PATH_MAX is what's used, well it's should be MAXPATH which is MAX_PATH
# define PATH_MAX MAXPATH
#endif
#ifdef _WIN32
#  ifdef CONSOLE_SHELL
 // in order to get wide characters from the commandline we have to use the GetCommandLineW function, convert it to utf8 for internal usage.
#    define SaneWinMain(a,b) int main( int a, char **argv_real ) { char *tmp; TEXTCHAR **b; ParseIntoArgs( tmp = WcharConvert( GetCommandLineW() ), &a, &b ); Deallocate( char*, tmp ); {
#    define EndSaneWinMain() } }
#  else
#    define SaneWinMain(a,b) int APIENTRY WinMain( HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow ) { int a; char *tmp; TEXTCHAR **b; ParseIntoArgs( tmp = WcharConvert( GetCommandLineW() ), &a, &b ); {
#    define EndSaneWinMain() } }
#  endif
#else
#  if defined( __ANDROID__ ) && !defined( ANDROID_CONSOLE_UTIL )
#    define SaneWinMain(a,b) int SACK_Main( int a, char **b )
#    define EndSaneWinMain()
#  else
#    define SaneWinMain(a,b) int main( int a, char **b ) { char **argv_real = b; {
#    define EndSaneWinMain() } }
#  endif
#endif
//  these are rude defines overloading otherwise very practical types
// but - they have to be dispatched after all standard headers.
#ifndef FINAL_TYPES
#define FINAL_TYPES
#  ifdef __WATCOMC__
 //__WATCOMC__
#  endif
#  ifdef _WIN32
#    include <basetsd.h>
  // this redefines lprintf sprintf etc... and strsafe is preferred
 // more things that need override by strsafe.h
#    include <tchar.h>
 // added for mingw64 actually
#    ifdef __GNUC__
#      undef __CRT__NO_INLINE
#    endif
#    ifndef MINGW_SUX
#      include <strsafe.h>
#    else
#      define STRSAFE_E_INSUFFICIENT_BUFFER  0x8007007AL
#    endif
#  else
#  endif
// may consider changing this to uint16_t* for unicode...
#ifdef UNICODE
#  ifndef NO_UNICODE_C
#    define strrchr          wcsrchr
#    define strchr           wcschr
#    define strncpy          wcsncpy
#    ifdef strcpy
#      undef strcpy
#    endif
#    define strcpy           wcscpy
#    define strcmp           wcscmp
#    ifndef __LINUX__
// linux also translates 'i' to 'case' in sack_typelib.h
#      define stricmp          wcsicmp
#      define strnicmp         wcsnicmp
//#  define strlen           mbrlen
#    endif
#    define strlen           wcslen
#    ifdef WIN32
#      define stat(a,b)        _wstat(a,b)
#    else
#    endif
#    define printf           wprintf
#    define fprintf          fwprintf
#    define fputs            fputws
#    define fgets            fgetws
#    define atoi             _wtoi
#    ifdef __WATCOMC__
#      undef atof
#    endif
//#    define atof             _wtof
#    ifdef _MSC_VER
#      ifndef __cplusplus_cli
#        define fprintf   fwprintf
#        define atoi      _wtoi
// define sprintf here.
#      endif
#    endif
#    if defined( _ARM_ ) && defined( WIN32 )
// len should be passed as character count. this was the wrongw ay to default this.
#      define snprintf StringCbPrintf
//#define snprintf StringCbPrintf
#    endif
#  else
//#    define atoi             wtoi
#  endif
 // not unicode...
#else
#endif
#  ifdef _MSC_VER
#    define snprintf _snprintf
#    define vsnprintf _vsnprintf
#    if defined( _UNICODE )
#      define tnprintf _snwprintf
#      define vtnprintf _vsnwprintf
#    else
#      define tnprintf _snprintf
#      define vtnprintf _vsnprintf
#    endif
#    define snwprintf _snwprintf
#    if defined( _UNICODE ) && !defined( NO_UNICODE_C )
#    define tscanf swscanf_s
#    else
#    define tscanf sscanf_s
#    endif
#    define scanf sscanf_s
#    define swcanf swscanf_s
 // _MSC_VER
#  endif
#  ifdef  __GNUC__
#      if defined( _UNICODE )
#        define VSNPRINTF_FAILS_RETURN_SIZE
#        define tnprintf  swprintf
#        define vtnprintf vswprintf
#        if !defined( NO_UNICODE_C )
#           define snprintf   swprintf
#           define vsnprintf  vswprintf
//#           define sscanf     swscanf
#        else
#        endif
#      else
#        define tnprintf snprintf
#        define vtnprintf vsnprintf
//#        define snprintf snprintf
//#        define vsnprintf vsnprintf
#    if defined( _UNICODE ) && !defined( NO_UNICODE_C )
#    define tscanf swscanf
#    else
#    define tscanf sscanf
#    endif
#      endif
 // __GNUC__
#  endif
#  ifdef __WATCOMC__
#      if defined( _UNICODE )
#        define tnprintf  _snwprintf
#        define vtnprintf _vsnwprintf
#        if !defined( NO_UNICODE_C )
#           define snprintf  _snwprintf
#           define vsnprintf _vsnwprintf
#           define sscanf     swscanf
#        else
#        endif
#      else
#         define tnprintf  snprintf
#         define vtnprintf vsnprintf
//#        define snprintf snprintf
//#        define vsnprintf vsnprintf
#      endif
#        define snwprintf  _snwprintf
 // __WATCOMC__
#  endif
#endif
#endif
#ifdef __cplusplus
#ifdef _D3D_DRIVER
#define RENDER_NAMESPACE namespace sack { namespace image { namespace render { namespace d3d {
#define _RENDER_NAMESPACE namespace render { namespace d3d {
#define RENDER_NAMESPACE_END }}}}
#elif defined( _D3D10_DRIVER )
#define RENDER_NAMESPACE namespace sack { namespace image { namespace render { namespace d3d10 {
#define _RENDER_NAMESPACE namespace render { namespace d3d10 {
#define RENDER_NAMESPACE_END }}}}
#elif defined( _D3D11_DRIVER )
#define RENDER_NAMESPACE namespace sack { namespace image { namespace render { namespace d3d11 {
#define _RENDER_NAMESPACE namespace render { namespace d3d11 {
#define RENDER_NAMESPACE_END }}}}
#else
#define RENDER_NAMESPACE namespace sack { namespace image { namespace render {
/* <copy render.h>
   \ \             */
#define _RENDER_NAMESPACE namespace render {
#define RENDER_NAMESPACE_END }}}
#endif
#else
#define RENDER_NAMESPACE
#define _RENDER_NAMESPACE
#define RENDER_NAMESPACE_END
#endif
#ifndef KEYBOARD_DEFINITION
#  define KEYBOARD_DEFINITION
#  ifdef __cplusplus
#    define _RENDER_KEYBOARD_NAMESPACE namespace keyboard {
#    define _RENDER_KEYBOARD_NAMESPACE_END }
#  else
#    define _RENDER_KEYBOARD_NAMESPACE
#    define _RENDER_KEYBOARD_NAMESPACE_END
#  endif
RENDER_NAMESPACE
   _RENDER_KEYBOARD_NAMESPACE
			/* Keyboard state tracking structure... not very optimal...
			   \internal usage might be different.                      */
			enum KeyUpDownState {
KEYISUP   =2,
KEYISDOWN =1
			};
/* <combine sack::image::render::keyboard::keyboard_tag>
   \ \                                                   */
typedef struct keyboard_tag KEYBOARD;
/* <combine sack::image::render::keyboard::keyboard_tag>
   \ \                                                   */
typedef struct keyboard_tag *PKEYBOARD;
struct keyboard_tag
{
#define NUM_KEYS 256
   /* one byte index... more than sufficient
      if character in array is '1' key is down, '2' key is up. */
   char keyupdown[NUM_KEYS];
   /* Indicator that the key is a double-tap, not just a single.
      "!! is different that "!" "!                               */
   char keydouble[NUM_KEYS];
   /* time of the last key event */
   unsigned int  keytime[NUM_KEYS];
   /* I'm not sure, maybe it's the printable key char? */
		unsigned char key[NUM_KEYS];
#if 0
	// void (*Proc)(uintptr_t psv)[NUM_KEYS][8];
#endif
};
_RENDER_KEYBOARD_NAMESPACE_END
RENDER_NAMESPACE_END
#ifdef __cplusplus
#  ifdef _D3D_DRIVER
     using namespace sack::image::render::d3d::keyboard;
#  elif defined( _D3D10_DRIVER )
     using namespace sack::image::render::d3d10::keyboard;
#  elif defined( _D3D11_DRIVER )
     using namespace sack::image::render::d3d11::keyboard;
#  else
     using namespace sack::image::render::keyboard;
#  endif
#endif
//#include "vidlib.h"
	// some common things which are specific to this
   // library, and independant of implementation (so far)
#define KEY_MOD_SHIFT 1
#define KEY_MOD_CTRL  2
#define KEY_MOD_ALT   4
#define KEY_MOD_META  64
// call trigger on release also...
#define KEY_MOD_RELEASE  8
 // application wants both press and release events.
#define KEY_MOD_ALL_CHANGES  16
 // key match must be extended also... (extra arrow keys for instance.. what about SDL)
#define KEY_MOD_EXTENDED 32
#define KEY_PRESSED         0x80000000
#define IsKeyPressed( keycode ) ( (keycode) & 0x80000000 )
#define KEY_ALT_DOWN        0x40000000
#define KEY_CONTROL_DOWN    0x20000000
#define KEY_SHIFT_DOWN      0x10000000
#define KEY_MOD_DOWN (KEY_ALT_DOWN|KEY_CONTROL_DOWN)
#define KEY_ALPHA_LOCK_ON   0x08000000
#define KEY_NUM_LOCK_ON     0x04000000
#define KEY_MOD(key)        ( ( (key) & 0x70000000 ) >> 28 )
#define KEY_REAL_CODE(key)  ( ( (key) & 0x00FF0000 ) >> 16 )
#define KEY_CODE(key)       ( (key) & 0xFF )
#define IsKeyExtended(key)  ( ( (key) & 0x00000100 ) >> 8 )
#if defined( _WIN32 ) || defined( WIN32 ) || defined( __CYGWIN__ ) || defined( USE_WIN32_KEY_DEFINES )
// mirrored KEY_ definitions from allegro.H library....
//#include <windows.h>
#  define BIT_7           0x80
#  define KEY_TAB          9
#  define KEY_CENTER       12
#  define KEY_PAD5         12
#  define KEY_ENTER        13
#  define KEY_LSHIFT       16
#  define KEY_SHIFT        16
#  define KEY_LEFT_SHIFT   0x10
 // maybe?
#  define KEY_RIGHT_SHIFT  0x10
#  define KEY_SHIFT_LEFT KEY_LEFT_SHIFT
#  define KEY_SHIFT_RIGHT KEY_RIGHT_SHIFT
#  define KEY_CTRL         17
#  define KEY_CONTROL      17
#  define KEY_LEFT_CONTROL  17
#  define KEY_RIGHT_CONTROL 17
 // can't get usually under windows?(keyhook!)
#  define KEY_ALT          18
#  define KEY_LEFT_ALT      18
#  define KEY_RIGHT_ALT     18
#  define KEY_CAPS_LOCK    20
#  define KEY_ESC          27
#  define KEY_ESCAPE       27
#  define KEY_PGUP         33
#  define KEY_PAGE_UP     KEY_PGUP
#  define KEY_PGDN         34
#  define KEY_PAGE_DOWN   KEY_PGDN
#  define KEY_END          35
#  define KEY_HOME         36
#  define KEY_LEFT         37
#  define KEY_UP           38
#  define KEY_RIGHT        39
#  define KEY_DOWN         40
#  define KEY_GRAY_UP  38
#  define KEY_GRAY_LEFT   37
#  define KEY_GRAY_RIGHT  39
#  define KEY_GRAY_DOWN    40
//#  define KEY_GRAY_UP      BIT_7+0x48
#  define KEY_GRAY_PGUP   BIT_7+0x49
#  define KEY_GRAY_MINUS  BIT_7+0x4A
//#  define KEY_GRAY_LEFT BIT_7+0x4B
//#  define KEY_GRAY_RIGHT   BIT_7+0x4D
#  define KEY_GRAY_PLUS   BIT_7+0x4E
#  define KEY_GRAY_END    BIT_7+0x4F
#  define KEY_PAD_PLUS   BIT_7+0x4E
//#  define KEY_GRAY_DOWN BIT_7+0x50
#  define KEY_GRAY_PGDN   BIT_7+0x51
#  define KEY_GRAY_INS    BIT_7+0x52
#  define KEY_GRAY_DEL    BIT_7+0x53
#  define KEY_GRAY_DELETE    BIT_7+0x53
#  define KEY_GREY_DELETE    BIT_7+0x53
#  define KEY_INSERT       45
#  define KEY_DEL          46
#  define KEY_DELETE       KEY_DEL
#  define KEY_PRINT_SCREEN1  VK_PRINT
#  define KEY_PRINT_SCREEN2  VK_SNAPSHOT
 // windows keys keys
#  define KEY_WINDOW_2     0x50
 // windows keys keys
#  define KEY_WINDOW_1     0x5c
#  define KEY_GRAY_STAR     0x6a
#  define KEY_PLUS_PAD     0x6b
//#  define KEY_GRAY_MINUS    0x6d
#  define KEY_GRAY_SLASH    VK_OEM_5
//#  define KEY_GRAY_PLUS     107
#  define KEY_NUM_LOCK      VK_NUMLOCK
#  define KEY_SCROLL_LOCK VK_SCROLL
#  define KEY_SLASH        VK_OEM_2
#  define KEY_BACKSPACE   '\b'
#  define KEY_SPACE        ' '
#  define KEY_COMMA      0xBC
 // should be some sort of VK_ definitions....
#  define KEY_STOP       0xBE
#  define KEY_PERIOD     KEY_STOP
#  define KEY_A         'A'
#  define KEY_B         'B'
#  define KEY_C         'C'
#  define KEY_D         'D'
#  define KEY_E         'E'
#  define KEY_F         'F'
#  define KEY_G         'G'
#  define KEY_H         'H'
#  define KEY_I         'I'
#  define KEY_J         'J'
#  define KEY_K         'K'
#  define KEY_L         'L'
#  define KEY_F12  VK_F12
#  define KEY_F11  VK_F11
#  define KEY_F10  VK_F10
#  define KEY_F9  VK_F9
#  define KEY_F8  VK_F8
#  define KEY_F7  VK_F7
#  define KEY_F6  VK_F6
#  define KEY_F5  VK_F5
#  define KEY_F4  VK_F4
#  define KEY_F3  VK_F3
#  define KEY_F2  VK_F2
#  define KEY_F1  VK_F1
#  define KEY_M        77
#  define KEY_N         78
#  define KEY_O         79
#  define KEY_P        80
#  define KEY_Q         'Q'
#  define KEY_R         'R'
#  define KEY_S         'S'
#  define KEY_T         'T'
#  define KEY_U         'U'
#  define KEY_V         'V'
#  define KEY_W         'W'
#  define KEY_X         'X'
#  define KEY_Y         'Y'
#  define KEY_Z         'Z'
#  define KEY_1         '1'
#  define KEY_2         '2'
#  define KEY_3         '3'
#  define KEY_4         '4'
#  define KEY_5         '5'
#  define KEY_6         '6'
#  define KEY_7         '7'
#  define KEY_8         '8'
#  define KEY_9         '9'
#  define KEY_0         '0'
#  define KEY_MINUS    KEY_DASH
#  ifndef VK_OEM_1
// native windows OEM definitions
#    define VK_OEM_1   186
#    define VK_OEM_2   191
#    define VK_OEM_4   219
#    define VK_OEM_5   220
#    define VK_OEM_6   221
#    define VK_OEM_7   222
#    define VK_OEM_MINUS  189
#    define VK_OEM_PLUS    187
#  endif
#  define KEY_SEMICOLON     VK_OEM_1
#  define KEY_QUOTE         VK_OEM_7
#  define KEY_LEFT_BRACKET  VK_OEM_4
#  define KEY_RIGHT_BRACKET VK_OEM_6
#  define KEY_BACKSLASH     VK_OEM_5
//'-'
#  define KEY_DASH     VK_OEM_MINUS
#  define KEY_EQUAL    VK_OEM_PLUS
#  define KEY_EQUALS   KEY_EQUAL
#  define KEY_ACCENT 192
#  define KEY_GRAVE  KEY_ACCENT
#  define KEY_APOSTROPHE  KEY_ACCENT
#  define KEY_F1  VK_F1
#  define KEY_F2  VK_F2
#  define KEY_F3  VK_F3
#  define KEY_F4  VK_F4
#  define KEY_F5  VK_F5
#  define KEY_F6  VK_F6
#  define KEY_F7  VK_F7
#  define KEY_F8  VK_F8
#  define KEY_F9  VK_F9
#  define KEY_F10  VK_F10
#  define KEY_F1  VK_F1
#  define VK_NUMPAD0        0x60
#  define VK_NUMPAD1        0x61
#  define VK_NUMPAD2        0x62
#  define VK_NUMPAD3        0x63
#  define VK_NUMPAD4        0x64
#  define VK_NUMPAD5        0x65
#  define VK_NUMPAD6        0x66
#  define VK_NUMPAD7        0x67
#  define VK_NUMPAD8        0x68
#  define VK_NUMPAD9        0x69
#  define VK_MULTIPLY       0x6A
#  define VK_ADD            0x6B
#  define VK_SEPARATOR      0x6C
#  define VK_SUBTRACT       0x6D
#  define VK_DECIMAL        0x6E
#  define VK_DIVIDE         0x6F
#  define KEY_PAD_MULT VK_MULTIPLY
#  define KEY_PAD_DOT VK_DECIMAL
#  define KEY_PAD_DIV VK_DIVIDE
#  define KEY_PAD_0 VK_NUMPAD0
#  define KEY_GREY_INSERT VK_NUMPAD0
#  define KEY_PAD_1 VK_NUMPAD1
#  define KEY_PAD_2 VK_NUMPAD2
#  define KEY_PAD_3 VK_NUMPAD3
#  define KEY_PAD_4 VK_NUMPAD4
#  define KEY_PAD_5 VK_NUMPAD5
#  define KEY_PAD_6 VK_NUMPAD6
#  define KEY_PAD_7 VK_NUMPAD7
#  define KEY_PAD_8 VK_NUMPAD8
#  define KEY_PAD_9 VK_NUMPAD9
#  define KEY_PAD_ENTER VK_RETURN
#  define KEY_PAD_DELETE VK_SEPARATOR
#  define KEY_PAD_MINUS VK_SUBTRACT
#endif
#if defined( __EMSCRIPTEN__ ) || defined( __EMSCRIPTEN__ )
	  /*   https://w3c.github.io/uievents/#fixed-virtual-key-codes
      // for keyCode
      */
#define KEY_BACKSPACE 8
#define KEY_TAB 9
#define KEY_ENTER 13
#define KEY_SHIFT 16
#define KEY_LEFT_SHIFT 16
#define KEY_RIGHT_SHIFT 16
#define KEY_CTRL 17
#define KEY_CONTROL 17
#define KEY_LEFT_CONTROL 17
#define KEY_RIGHT_CONTROL 17
#define KEY_ALT 18
#define KEY_LEFT_ALT 18
#define KEY_RIGHT_ALT 18
#define KEY_F1  112
#define KEY_F2  113
#define KEY_F3  114
#define KEY_F4  115
#define KEY_F5  116
#define KEY_F6  117
#define KEY_F7  118
#define KEY_F8  119
#define KEY_F9  120
#define KEY_F10  121
#define KEY_F11  122
#define KEY_F12  123
#  undef KEY_SPACE
#  define KEY_SPACE        ' '
#  define KEY_A         'A'
#  define KEY_B         'B'
#  define KEY_C         'C'
#  define KEY_D         'D'
#  define KEY_E         'E'
#  define KEY_F         'F'
#  define KEY_G         'G'
#  define KEY_H         'H'
#  define KEY_I         'I'
#  define KEY_J         'J'
#  define KEY_K         'K'
#  define KEY_L         'L'
#  define KEY_M        77
#  define KEY_N         78
#  define KEY_O         79
#  define KEY_P        80
#  define KEY_Q         'Q'
#  define KEY_R         'R'
#  define KEY_S         'S'
#  define KEY_T         'T'
#  define KEY_U         'U'
#  define KEY_V         'V'
#  define KEY_W         'W'
#  define KEY_X         'X'
#  define KEY_Y         'Y'
#  define KEY_Z         'Z'
#  define KEY_1         '1'
#  define KEY_2         '2'
#  define KEY_3         '3'
#  define KEY_4         '4'
#  define KEY_5         '5'
#  define KEY_6         '6'
#  define KEY_7         '7'
#  define KEY_8         '8'
#  define KEY_9         '9'
#  define KEY_0         '0'
   //';'
#define KEY_SEMICOLON	186
   //':'
#define KEY_COLON	    186
  //'='	//187
#define KEY_EQUAL        187
  //'+'	//187
#define KEY_PLUS	     187
   //','	//188
#define KEY_COMMA	      188
     //'<'		//188
#define KEY_LESS_THAN	188
     //'-'		//189
#define KEY_MINUS	     189
    //'-'       //189
#define KEY_DASH         189
  //'_'		//189
#define KEY_Underscore	189
   //'.'		//190
#define KEY_STOP	     190
   //'.'		//190
#define KEY_PERIOD	     190
   //'>'		//190
#define KEY_GREATER_THAN	190
    //'/'		//191
#define KEY_SLASH	   191
   //'?'		//191
#define KEY_QUESTION	191
 //'`'		//192
#define KEY_ACCENT	     192
 //'~'		//192
#define KEY_TILDE	    192
  //'['		//219
#define KEY_LEFT_BRACKET	219
  //'{'		//219
#define KEY_OPEN_BRACE	219
  //'\\'		//220
#define KEY_BACKSLASH	220
 //'|'		//220
#define KEY_PIPE	     220
//']'		//221
#define KEY_RIGHT_BRACKET	221
//	'}'		//221
#define KEY_CLOSE_BRACE     221
   //'\''		//222
#define KEY_QUOTE	222
//Double quote	'\"'		//222
#  define KEY_ESCAPE       27
#  define KEY_PGUP         33
#  define KEY_PAGE_UP      33
#  define KEY_PGDN         34
#  define KEY_PAGE_DOWN    34
#  define KEY_END          35
#  define KEY_HOME         36
#  define KEY_LEFT         37
#  define KEY_UP           38
#  define KEY_RIGHT        39
#  define KEY_DOWN         40
#  define KEY_GRAY_UP  38
#  define KEY_GRAY_LEFT   37
#  define KEY_GRAY_RIGHT  39
#  define KEY_GRAY_DOWN    40
//#  define KEY_GRAY_UP      0x48
#  define KEY_GRAY_PGUP     33
#  define KEY_GRAY_MINUS    109
//#  define KEY_GRAY_LEFT 0x4B
//#  define KEY_GRAY_RIGHT   0x4D
#  define KEY_GRAY_PLUS   107
#  define KEY_GRAY_END    0x4F
#  define KEY_PAD_PLUS   107
//#  define KEY_GRAY_DOWN 0x50
#  define KEY_GRAY_PGDN   34
#  define KEY_GRAY_INS    45
#  define KEY_GRAY_INSERT    45
#  define KEY_GREY_INSERT   45
#  define KEY_GRAY_DEL       46
#  define KEY_GRAY_DELETE    46
#  define KEY_GREY_DELETE    47
#  define KEY_INSERT       45
#  define KEY_DEL          46
#  define KEY_DELETE       KEY_DEL
#  define KEY_PAD_MULT 106
#  define KEY_PAD_DOT  110
#  define KEY_PAD_DELETE 110
#  define KEY_PAD_DIV 111
#  define KEY_PAD_0 96
#  define KEY_PAD_1 97
#  define KEY_PAD_2 98
#  define KEY_PAD_3 99
#  define KEY_PAD_4 100
#  define KEY_PAD_5 101
#  define KEY_PAD_6 102
#  define KEY_PAD_7 103
#  define KEY_PAD_8 104
#  define KEY_PAD_9 105
#  define KEY_PAD_ENTER KEY_ENTER
#  define KEY_PAD_MINUS 109
#define KEY_NUM_LOCK 144
#undef KEY_SCROLL_LOCK
#define KEY_SCROLL_LOCK 145
     /*   https://w3c.github.io/uievents/#fixed-virtual-key-codes
     Backspace	8
Tab	9
Enter	13
Shift	16
Control	17
Alt	18
CapsLock	20
Escape	27	Esc
Space	32
PageUp	33
PageDown	34
End	35
Home	36
ArrowLeft	37
ArrowUp	38
ArrowRight	39
ArrowDown	40
Delete	46	Del
Semicolon	";"	186
Colon	":"	186
Equals sign	"="	187
Plus	"+"	187
Comma	","	188
Less than sign	"<"	188
Minus	"-"	189
Underscore	"_"	189
Period	"."	190
Greater than sign	">"	190
Forward slash	"/"	191
Question mark	"?"	191
Backtick	"`"	192
Tilde	"~"	192
Opening squace bracket	"["	219
Opening curly brace	"{"	219
Backslash	"\"	220
Pipe	"|"	220
Closing square bracket	"]"	221
Closing curly brace	"}"	221
Single quote	"'"	222
Double quote	"""	222
     */
#  endif
// if any key...
#if !defined( KEY_1 )
#  if defined( __ANDROID__ )
#    include <android/keycodes.h>
#    define KEY_SHIFT        AKEYCODE_SHIFT_LEFT
#    define KEY_LEFT_SHIFT   AKEYCODE_SHIFT_LEFT
 // maybe?
#    define KEY_RIGHT_SHIFT  AKEYCODE_SHIFT_RIGHT
#    ifndef AKEYCODE_CTRL_LEFT
#      define AKEYCODE_CTRL_LEFT 113
#    endif
#    ifndef AKEYCODE_CTRL_RIGHT
#      define AKEYCODE_CTRL_RIGHT 114
#    endif
#    define KEY_CTRL          AKEYCODE_CTRL_LEFT
#    define KEY_CONTROL       AKEYCODE_CTRL_LEFT
#    define KEY_LEFT_CONTROL  AKEYCODE_CTRL_LEFT
#    define KEY_RIGHT_CONTROL AKEYCODE_CTRL_RIGHT
 // can't get usually under windows?(keyhook!)
#    define KEY_ALT           AKEYCODE_ALT_LEFT
#    define KEY_LEFT_ALT      AKEYCODE_ALT_LEFT
#    define KEY_RIGHT_ALT     AKEYCODE_ALT_RIGHT
#    ifndef AKEYCODE_CAPS_LOCK
#      define AKEYCODE_CAPS_LOCK 115
#    endif
#    define KEY_CAPS_LOCK     AKEYCODE_CAPS_LOCK
#    define KEY_NUM_LOCK      0
#    ifndef AKEYCODE_SCROLL_LOCK
#      define AKEYCODE_SCROLL_LOCK 116
#    endif
 // unsure about this
#    define KEY_SCROLL_LOCK   AKEYCODE_SCROLL_LOCK
#    ifndef AKEYCODE_ESCAPE
#      define AKEYCODE_ESCAPE 111
#    endif
#    define KEY_ESC           AKEYCODE_ESCAPE
#    define KEY_ESCAPE        AKEYCODE_ESCAPE
#    ifndef AKEYCODE_MOVE_HOME
#      define AKEYCODE_MOVE_HOME 122
#    endif
#    ifndef AKEYCODE_MOVE_END
#      define AKEYCODE_MOVE_END 123
#    endif
#    define KEY_HOME          AKEYCODE_MOVE_HOME
#    define KEY_PAD_HOME      AKEYCODE_MOVE_HOME
#    define KEY_PAD_7         0
#    define KEY_GREY_HOME     0
#    define KEY_UP            AKEYCODE_DPAD_UP
#    define KEY_PAD_8         0
#    define KEY_PAD_UP        0
#    define KEY_GREY_UP       0
#    define KEY_PGUP          0
#    define KEY_PAD_9         0
#    define KEY_PAD_PGUP      0
#    define KEY_GREY_PGUP     0
#    define KEY_LEFT          AKEYCODE_DPAD_LEFT
#    define KEY_PAD_4         0
#    define KEY_PAD_LEFT      0
#    define KEY_GREY_LEFT     0
#    define KEY_CENTER        AKEYCODE_DPAD_CENTER
#    define KEY_PAD_5         0
#    define KEY_PAD_CENTER    0
#    define KEY_GREY_CENTER   0
#    define KEY_RIGHT         AKEYCODE_DPAD_RIGHT
#    define KEY_PAD_6         0
#    define KEY_PAD_RIGHT     0
#    define KEY_GREY_RIGHT    0
#    define KEY_END           AKEYCODE_MOVE_END
#    define KEY_PAD_1         0
#    define KEY_PAD_END       0
#    define KEY_GREY_END      0
#    define KEY_DOWN          AKEYCODE_DPAD_DOWN
#    define KEY_PAD_2         0
#    define KEY_PAD_DOWN      0
#    define KEY_GREY_DOWN     0
#    define KEY_PGDN          0
#    define KEY_PAD_3         0
#    define KEY_PAD_PGDN      0
#    define KEY_GREY_PGDN     0
#    define KEY_INSERT        0
#    define KEY_PAD_0         0
#    define KEY_PAD_INSERT    0
#    define KEY_GREY_INSERT   0
#    define KEY_DELETE        0
#    define KEY_PAD_DOT       0
#    define KEY_PAD_DELETE    0
#    define KEY_GREY_DELETE   0
#    define KEY_PLUS          0
#    define KEY_PAD_PLUS      0
#    define KEY_GREY_PLUS     0
#    define KEY_MINUS         0
#    define KEY_PAD_MINUS     0
#    define KEY_GREY_MINUS    0
#    define KEY_MULT          0
#    define KEY_PAD_MULT      0
#    define KEY_GREY_MULT     0
#    define KEY_DIV           0
#    define KEY_PAD_DIV       0
#    define KEY_GREY_DIV      0
#    define KEY_ENTER         AKEYCODE_ENTER
#    define KEY_PAD_ENTER     AKEYCODE_ENTER
#    define KEY_NORMAL_ENTER  AKEYCODE_ENTER
 // windows keys keys
#    define KEY_WINDOW_1      0
 // windows keys keys
#    define KEY_WINDOW_2      0
#    define KEY_TAB           AKEYCODE_TAB
#    define KEY_SLASH         AKEYCODE_SLASH
#    define KEY_BACKSPACE     AKEYCODE_DEL
#    define KEY_SPACE         AKEYCODE_SPACE
#    define KEY_COMMA         AKEYCODE_COMMA
 // should be some sort of VK_ definitions....
#    define KEY_STOP          AKEYCODE_PERIOD
#    define KEY_PERIOD        AKEYCODE_PERIOD
#    define KEY_SEMICOLON     AKEYCODE_SEMICOLON
#    define KEY_QUOTE         AKEYCODE_APOSTROPHE
#    define KEY_LEFT_BRACKET  AKEYCODE_LEFT_BRACKET
#    define KEY_RIGHT_BRACKET AKEYCODE_RIGHT_BRACKET
#    define KEY_BACKSLASH     AKEYCODE_BACKSLASH
#    define KEY_DASH          AKEYCODE_MINUS
#    define KEY_EQUAL         AKEYCODE_EQUALS
#    define KEY_ACCENT        AKEYCODE_GRAVE
#    define KEY_1         AKEYCODE_1
#    define KEY_2         AKEYCODE_2
#    define KEY_3         AKEYCODE_3
#    define KEY_4         AKEYCODE_4
#    define KEY_5         AKEYCODE_5
#    define KEY_6         AKEYCODE_6
#    define KEY_7         AKEYCODE_7
#    define KEY_8         AKEYCODE_8
#    define KEY_9         AKEYCODE_9
#    define KEY_0         AKEYCODE_0
#    define KEY_F1        0
#    define KEY_F2        0
#    define KEY_F3        0
#    define KEY_F4        0
#    define KEY_F5        0
#    define KEY_F6        0
#    define KEY_F7        0
#    define KEY_F8        0
#    define KEY_F9        0
#    define KEY_F10       0
#    define KEY_F11       0
#    define KEY_F12       0
#    define KEY_A   AKEYCODE_A
#    define KEY_B   AKEYCODE_B
#    define KEY_C   AKEYCODE_C
#    define KEY_D   AKEYCODE_D
#    define KEY_E   AKEYCODE_E
#    define KEY_F   AKEYCODE_F
#    define KEY_G   AKEYCODE_G
#    define KEY_H   AKEYCODE_H
#    define KEY_I   AKEYCODE_I
#    define KEY_J   AKEYCODE_J
#    define KEY_K   AKEYCODE_K
#    define KEY_L   AKEYCODE_L
#    define KEY_M   AKEYCODE_M
#    define KEY_N   AKEYCODE_N
#    define KEY_O   AKEYCODE_O
#    define KEY_P   AKEYCODE_P
#    define KEY_Q   AKEYCODE_Q
#    define KEY_R   AKEYCODE_R
#    define KEY_S   AKEYCODE_S
#    define KEY_T   AKEYCODE_T
#    define KEY_U   AKEYCODE_U
#    define KEY_V   AKEYCODE_V
#    define KEY_W   AKEYCODE_W
#    define KEY_X   AKEYCODE_X
#    define KEY_Y   AKEYCODE_Y
#    define KEY_Z   AKEYCODE_Z
#  elif defined( __LINUX__ )
	  //#define USE_SDL_KEYSYM
// ug - KEYSYMS are too wide...
// so - we fall back to x scancode tables - and translate sym to these
// since the scancodes which come from X are not the same as from console Raw
// but - perhaps we should re-translate these to REAL scancodes... but in either
// case - these fall to under 256 characters, and can therefore be used...
#    define USE_X_RAW_SCANCODES
#    ifdef USE_X_RAW_SCANCODES
#      define KEY_SHIFT        0xFF
#      define KEY_LEFT_SHIFT   50
 // maybe?
#      define KEY_RIGHT_SHIFT  62
#      define KEY_CTRL          0xFE
#      define KEY_CONTROL       0xFE
#      define KEY_LEFT_CONTROL  37
#      define KEY_RIGHT_CONTROL 109
 // can't get usually under windows?(keyhook!)
#      define KEY_ALT           0xFD
#      define KEY_LEFT_ALT      64
#      define KEY_RIGHT_ALT     113
#      define KEY_CAPS_LOCK     66
#      define KEY_NUM_LOCK      77
 // unsure about this
#      define KEY_SCROLL_LOCK   78
#      define KEY_ESC           9
#      define KEY_ESCAPE        9
#      define KEY_HOME          0xFC
#      define KEY_PAD_HOME      79
#      define KEY_PAD_7         79
#      define KEY_GREY_HOME     97
#      define KEY_UP            0xFB
#      define KEY_PAD_8         80
#      define KEY_PAD_UP        80
#      define KEY_GREY_UP       98
#      define KEY_PGUP          0xFA
#      define KEY_PAGE_UP       KEY_PGUP
#      define KEY_PAD_9         81
#      define KEY_PAD_PGUP      81
#      define KEY_GREY_PGUP     99
#      define KEY_LEFT          0xF9
#      define KEY_PAD_4         83
#      define KEY_PAD_LEFT      83
#      define KEY_GREY_LEFT     100
#      define KEY_CENTER        0xF8
#      define KEY_PAD_5         84
#      define KEY_PAD_CENTER    84
#      define KEY_GREY_CENTER   0
#      define KEY_RIGHT         0xF7
#      define KEY_PAD_6         85
#      define KEY_PAD_RIGHT     85
#      define KEY_GREY_RIGHT    102
#      define KEY_END           0xF6
#      define KEY_PAD_1         87
#      define KEY_PAD_END       87
#      define KEY_GREY_END      103
#      define KEY_DOWN          0xF5
#      define KEY_PAD_2         88
#      define KEY_PAD_DOWN      88
#      define KEY_GREY_DOWN     104
#      define KEY_PGDN          0xF4
#      define KEY_PAGE_DOWN     KEY_PGDN
#      define KEY_PAD_3         89
#      define KEY_PAD_PGDN      89
#      define KEY_GREY_PGDN     105
#      define KEY_INSERT        0xF3
#      define KEY_PAD_0         90
#      define KEY_PAD_INSERT    90
#      define KEY_GREY_INSERT   106
#      define KEY_DELETE        0xF2
#      define KEY_DEL           KEY_DELETE
#      define KEY_PAD_DOT       91
#      define KEY_PAD_DELETE    91
#      define KEY_GREY_DELETE   107
#      define KEY_PLUS          0xF1
#      define KEY_PAD_PLUS      86
#      define KEY_GREY_PLUS     0
#      define KEY_MINUS         0xF0
#      define KEY_PAD_MINUS     82
#      define KEY_GREY_MINUS    0
#      define KEY_MULT          0xEF
#      define KEY_PAD_MULT      63
#      define KEY_GREY_MULT     0
#      define KEY_DIV           0xEE
#      define KEY_PAD_DIV       112
#      define KEY_GREY_DIV      0
#      define KEY_ENTER         0xED
#      define KEY_PAD_ENTER     108
#      define KEY_NORMAL_ENTER  36
 // windows keys keys
#      define KEY_WINDOW_1      115
 // windows keys keys
#      define KEY_WINDOW_2      117
#      define KEY_TAB           23
#      define KEY_SLASH         61
#      define KEY_BACKSPACE     22
#      define KEY_SPACE         65
#      define KEY_COMMA         59
 // should be some sort of VK_ definitions....
#      define KEY_STOP          60
#      define KEY_PERIOD        KEY_STOP
#      define KEY_SEMICOLON     47
#      define KEY_QUOTE         48
#      define KEY_LEFT_BRACKET  34
#      define KEY_RIGHT_BRACKET 35
#      define KEY_BACKSLASH     51
#      define KEY_DASH          20
#      define KEY_EQUAL         21
#      define KEY_EQUALS       KEY_EQUAL
#      define KEY_ACCENT        49
#      define KEY_APOSTROPHE    KEY_QUOTE
#      define KEY_GRAVE        KEY_ACCENT
#      define KEY_SHIFT_LEFT   KEY_LEFT_SHIFT
#      define KEY_SHIFT_RIGHT  KEY_RIGHT_SHIFT
#      define KEY_1         10
#      define KEY_2         11
#      define KEY_3         12
#      define KEY_4         13
#      define KEY_5         14
#      define KEY_6         15
#      define KEY_7         16
#      define KEY_8         17
#      define KEY_9         18
#      define KEY_0         19
#      define KEY_F1        67
#      define KEY_F2        68
#      define KEY_F3        69
#      define KEY_F4        70
#      define KEY_F5        71
#      define KEY_F6        72
#      define KEY_F7        73
#      define KEY_F8        74
#      define KEY_F9        75
#      define KEY_F10       76
#      define KEY_F11       95
#      define KEY_F12       96
#      define KEY_A         38
#      define KEY_B         56
#      define KEY_C         54
#      define KEY_D         40
#      define KEY_E         26
#      define KEY_F         41
#      define KEY_G         42
#      define KEY_H         43
#      define KEY_I         31
#      define KEY_J         44
#      define KEY_K         45
#      define KEY_L         46
#      define KEY_M         58
#      define KEY_N         57
#      define KEY_O         32
#      define KEY_P         33
#      define KEY_Q         24
#      define KEY_R         27
#      define KEY_S         39
#      define KEY_T         28
#      define KEY_U         30
#      define KEY_V         55
#      define KEY_W         25
#      define KEY_X         53
#      define KEY_Y         29
#      define KEY_Z         52
#    elif defined( USE_SDL_KEYSYM )
#      include <SDL.h>
#      define KEY_SHIFT        0xFF
#      define KEY_LEFT_SHIFT   SDLK_LSHIFT
#      define KEY_RIGHT_SHIFT  SDLK_RSHIFT
#      define KEY_CTRL          0xFE
#      define KEY_CONTROL       0xFE
#      define KEY_LEFT_CONTROL  SDLK_LCTRL
#      define KEY_RIGHT_CONTROL SDLK_RCTRL
 // can't get usually under windows?(keyhook!)
#      define KEY_ALT           0xFD
#      define KEY_LEFT_ALT      SDLK_LALT
#      define KEY_RIGHT_ALT     SDLK_RALT
#      define KEY_CAPS_LOCK     SDLK_CAPSLOCK
#      define KEY_NUM_LOCK      SDLK_NUMLOCK
#      define KEY_SCROLL_LOCK   SDLK_SCROLLOCK
#      define KEY_ESC           SDLK_ESCAPE
#      define KEY_ESCAPE        SDLK_ESCAPE
#      define KEY_HOME          0xFC
#      define KEY_PAD_HOME      SDLK_KP7
#      define KEY_PAD_7         SDLK_KP7
#      define KEY_GREY_HOME     SDLK_HOME
#      define KEY_UP            0xFB
#      define KEY_PAD_8         SDLK_KP8
#      define KEY_PAD_UP        SDLK_KP8
#      define KEY_GREY_UP       SDLK_UP
#      define KEY_PGUP          0xFA
#      define KEY_PAD_9         SDLK_KP9
#      define KEY_PAD_PGUP      SDLK_KP9
#      define KEY_GREY_PGUP     SDLK_PAGEUP
#      define KEY_LEFT          0xF9
#      define KEY_PAD_4         SDLK_KP4
#      define KEY_PAD_LEFT      SDLK_KP4
#      define KEY_GREY_LEFT     SDLK_LEFT
#      define KEY_CENTER        0xF8
#      define KEY_PAD_5         SDLK_KP5
#      define KEY_PAD_CENTER    SDLK_KP5
#      define KEY_GREY_CENTER   0
#      define KEY_RIGHT         0xF7
#      define KEY_PAD_6         SDLK_KP6
#      define KEY_PAD_RIGHT     SDLK_KP6
#      define KEY_GREY_RIGHT    SDLK_RIGHT
#      define KEY_END           0xF6
#      define KEY_PAD_1         SDLK_KP1
#      define KEY_PAD_END       SDLK_KP1
#      define KEY_GREY_END      SDLK_END
#      define KEY_DOWN          0xF5
#      define KEY_PAD_2         SDLK_KP2
#      define KEY_PAD_DOWN      SDLK_KP2
#      define KEY_GREY_DOWN     SDLK_DOWN
#      define KEY_PGDN          0xF4
#      define KEY_PAD_3         SDLK_KP3
#      define KEY_PAD_PGDN      SDLK_KP3
#      define KEY_GREY_PGDN     SDLK_PAGEDN
#      define KEY_INSERT        0xF3
#      define KEY_PAD_0         SDLK_KP0
#      define KEY_PAD_INSERT    SDLK_KP0
#      define KEY_GREY_INSERT   SDLK_INSERT
#      define KEY_DELETE        0xF2
#      define KEY_PAD_DOT       SDLK_KP_PERIOD
#      define KEY_PAD_DELETE    SDLK_KP_PERIOD
#      define KEY_GREY_DELETE   SDLK_DELETE
#      define KEY_PLUS          0xF1
#      define KEY_PAD_PLUS      SDLK_KP_PLUS
#      define KEY_GREY_PLUS     0
#      define KEY_MINUS         0xF0
#      define KEY_PAD_MINUS     SDLK_KP_MINUS
#      define KEY_GREY_MINUS    0
#      define KEY_MULT          0xEF
#      define KEY_PAD_MULT      SDLK_KP_MULTIPLY
#      define KEY_GREY_MULT     0
#      define KEY_DIV           0xEE
#      define KEY_PAD_DIV       SDLK_KP_DIVIDE
#      define KEY_GREY_DIV      0
#      define KEY_ENTER         0xED
#      define KEY_PAD_ENTER     SDLK_KP_ENTER
#      define KEY_NORMAL_ENTER  SDLK_RETURN
 // windows keys keys
#      define KEY_WINDOW_1      115
 // windows keys keys
#      define KEY_WINDOW_2      117
#      define KEY_TAB           SDLK_TAB
#      define KEY_SLASH         SDLK_SLASH
#      define KEY_BACKSPACE     SDLK_BACKSPACE
#      define KEY_SPACE         SDLK_SPACE
#      define KEY_COMMA         SDLK_COMMA
 // should be some sort of VK_ definitions....
#      define KEY_STOP          SDLK_PERIOD
#      define KEY_PERIOD        KEY_STOP
#      define KEY_SEMICOLON     SDLK_SEMICOLON
#      define KEY_QUOTE         SDLK_QUOTE
#      define KEY_LEFT_BRACKET  SDLK_LEFTBRACKET
#      define KEY_RIGHT_BRACKET SDLK_RIGHTBRACKET
#      define KEY_BACKSLASH     SDLK_BACKSLASH
#      define KEY_DASH          SDLK_MINUS
#      define KEY_EQUAL         SDLK_EQUALS
 // grave
#      define KEY_ACCENT        SDLK_BACKQUOTE
#      define KEY_1         SDLK_1
#      define KEY_2         SDLK_2
#      define KEY_3         SDLK_3
#      define KEY_4         SDLK_4
#      define KEY_5         SDLK_5
#      define KEY_6         SDLK_6
#      define KEY_7         SDLK_7
#      define KEY_8         SDLK_8
#      define KEY_9         SDLK_9
#      define KEY_0         SDLK_0
#      define KEY_F1        SDLK_F1
#      define KEY_F2        SDLK_F2
#      define KEY_F3        SDLK_F3
#      define KEY_F4        SDLK_F4
#      define KEY_F5        SDLK_F5
#      define KEY_F6        SDLK_F6
#      define KEY_F7        SDLK_F7
#      define KEY_F8        SDLK_F8
#      define KEY_F9        SDLK_F9
#      define KEY_F10       SDLK_F10
#      define KEY_F11       SDLK_F11
#      define KEY_F12       SDLK_F12
#      define KEY_A         SDLK_A
#      define KEY_B         SDLK_B
#      define KEY_C         SDLK_C
#      define KEY_D         SDLK_D
#      define KEY_E         SDLK_E
#      define KEY_F         SDLK_F
#      define KEY_G         SDLK_G
#      define KEY_H         SDLK_H
#      define KEY_I         SDLK_I
#      define KEY_J         SDLK_J
#      define KEY_K         SDLK_K
#      define KEY_L         SDLK_L
#      define KEY_M         SDLK_M
#      define KEY_N         SDLK_N
#      define KEY_O         SDLK_O
#      define KEY_P         SDLK_P
#      define KEY_Q         SDLK_Q
#      define KEY_R         SDLK_R
#      define KEY_S         SDLK_S
#      define KEY_T         SDLK_T
#      define KEY_U         SDLK_U
#      define KEY_V         SDLK_V
#      define KEY_W         SDLK_W
#      define KEY_X         SDLK_X
#      define KEY_Y         SDLK_Y
#      define KEY_Z         SDLK_Z
#    elif defined( USE_RAW_SCANCODE )
#      error RAW_SCANCODES have not been defined yet.
#      define KEY_SHIFT        0xFF
#      define KEY_LEFT_SHIFT   50
 // maybe?
#      define KEY_RIGHT_SHIFT  62
#      define KEY_CTRL          0xFE
#      define KEY_CONTROL       0xFE
#      define KEY_LEFT_CONTROL  37
#      define KEY_RIGHT_CONTROL 109
 // can't get usually under windows?(keyhook!)
#      define KEY_ALT           0xFD
#      define KEY_LEFT_ALT      64
#      define KEY_RIGHT_ALT     113
#      define KEY_CAPS_LOCK     66
#      define KEY_NUM_LOCK      77
 // unsure about this
#      define KEY_SCROLL_LOCK   78
#      define KEY_ESC           9
#      define KEY_ESCAPE        9
#      define KEY_HOME          0xFC
#      define KEY_PAD_HOME      79
#      define KEY_PAD_7         79
#      define KEY_GREY_HOME     97
#      define KEY_UP            0xFB
#      define KEY_PAD_8         80
#      define KEY_PAD_UP        80
#      define KEY_GREY_UP       98
#      define KEY_PGUP          0xFA
#      define KEY_PAD_9         81
#      define KEY_PAD_PGUP      81
#      define KEY_GREY_PGUP     99
#      define KEY_LEFT          0xF9
#      define KEY_PAD_4         83
#      define KEY_PAD_LEFT      83
#      define KEY_GREY_LEFT     100
#      define KEY_CENTER        0xF8
#      define KEY_PAD_5         84
#      define KEY_PAD_CENTER    84
#      define KEY_GREY_CENTER   0
#      define KEY_RIGHT         0xF7
#      define KEY_PAD_6         85
#      define KEY_PAD_RIGHT     85
#      define KEY_GREY_RIGHT    102
#      define KEY_END           0xF6
#      define KEY_PAD_1         87
#      define KEY_PAD_END       87
#      define KEY_GREY_END      103
#      define KEY_DOWN          0xF5
#      define KEY_PAD_2         88
#      define KEY_PAD_DOWN      88
#      define KEY_GREY_DOWN     104
#      define KEY_PGDN          0xF4
#      define KEY_PAD_3         89
#      define KEY_PAD_PGDN      89
#      define KEY_GREY_PGDN     105
#      define KEY_INSERT        0xF3
#      define KEY_PAD_0         90
#      define KEY_PAD_INSERT    90
#      define KEY_GREY_INSERT   106
#      define KEY_DELETE        0xF2
#      define KEY_PAD_DOT       91
#      define KEY_PAD_DELETE    91
#      define KEY_GREY_DELETE   107
#      define KEY_PLUS          0xF1
#      define KEY_PAD_PLUS      86
#      define KEY_GREY_PLUS     0
#      define KEY_MINUS         0xF0
#      define KEY_PAD_MINUS     82
#      define KEY_GREY_MINUS    0
#      define KEY_MULT          0xEF
#      define KEY_PAD_MULT      63
#      define KEY_GREY_MULT     0
#      define KEY_DIV           0xEE
#      define KEY_PAD_DIV       112
#      define KEY_GREY_DIV      0
#      define KEY_ENTER         0xED
#      define KEY_PAD_ENTER     108
#      define KEY_NORMAL_ENTER  36
 // windows keys keys
#      define KEY_WINDOW_1      115
 // windows keys keys
#      define KEY_WINDOW_2      117
#      define KEY_TAB           23
#      define KEY_SLASH         61
#      define KEY_BACKSPACE     22
#      define KEY_SPACE         65
#      define KEY_COMMA         59
 // should be some sort of VK_ definitions....
#      define KEY_STOP          60
#      define KEY_PERIOD        KEY_STOP
#      define KEY_SEMICOLON     47
#      define KEY_QUOTE         48
#      define KEY_LEFT_BRACKET  34
#      define KEY_RIGHT_BRACKET 35
#      define KEY_BACKSLASH     51
#      define KEY_DASH          20
#      define KEY_EQUAL         21
#      define KEY_ACCENT        49
#      define KEY_1         10
#      define KEY_2         11
#      define KEY_3         12
#      define KEY_4         13
#      define KEY_5         14
#      define KEY_6         15
#      define KEY_7         16
#      define KEY_8         17
#      define KEY_9         18
#      define KEY_0         19
#      define KEY_F1        67
#      define KEY_F2        68
#      define KEY_F3        69
#      define KEY_F4        70
#      define KEY_F5        71
#      define KEY_F6        72
#      define KEY_F7        73
#      define KEY_F8        74
#      define KEY_F9        75
#      define KEY_F10       76
#      define KEY_F11       95
#      define KEY_F12       96
#      define KEY_A         38
#      define KEY_B         56
#      define KEY_C         54
#      define KEY_D         40
#      define KEY_E         26
#      define KEY_F         41
#      define KEY_G         42
#      define KEY_H         43
#      define KEY_I         31
#      define KEY_J         44
#      define KEY_K         45
#      define KEY_L         46
#      define KEY_M         58
#      define KEY_N         57
#      define KEY_O         32
#      define KEY_P         33
#      define KEY_Q         24
#      define KEY_R         27
#      define KEY_S         39
#      define KEY_T         28
#      define KEY_U         30
#      define KEY_V         55
#      define KEY_W         25
#      define KEY_X         53
#      define KEY_Y         29
#      define KEY_Z         52
#    endif
#  endif
#endif
#if defined( DEFINE_HARDWARE_SCANCODES )
#  ifndef KBD_HPP
#    define KBD_HPP
#    define KBD_INT            9
#    define KBD_EXTENDED_CODE     0xE0
#    define LOW_ASCII(asc)     (asc&0x7F)
#    define NUM_KEYS        256
#    ifdef WIN32
//#    define KEY_ESC       27
//#    define KEY_LEFT      37
//#    define KEY_CENTER    KB_CENTER
//#    define KEY_RIGHT     39
//#    define KEY_DOWN      40
//#    define KEY_GRAY_UP   38
//#    define KEY_GRAY_LEFT 37
//#    define KEY_GRAY_RIGHT   39
//#    define KEY_GRAY_DOWN    40
//#    define KEY_LEFT_SHIFT   16
//#    define KEY_RIGHT_SHIFT  16
//#    define KEY_GRAY_PGUP 33
//#    define KEY_GRAY_PGDN 34
//#    define KEY_GRAY_INS  45
//#    define KEY_GRAY_DEL  46
//#    define KEY_P         80
//#    define KEY_M         77
#    else
#      define KEY_ESC       0x01
#      define KEY_1         0x02
#      define KEY_2         0x03
#      define KEY_3         0x04
#      define KEY_4         0x05
#      define KEY_5         0x06
#      define KEY_6         0x07
#      define KEY_7         0x08
#      define KEY_8         0x09
#      define KEY_9         0x0A
#      define KEY_0         0x0B
#      define KEY_MINUS     0x0C
#      define KEY_PLUS         0x0D
#      define  KEY_BKSP        0x0E
#      define KEY_TAB       0x0F
#      define KEY_Q         0x10
#      define KEY_W         0x11
#      define KEY_E         0x12
#      define KEY_R         0x13
#      define KEY_T         0x14
#      define KEY_Y         0x15
#      define KEY_U         0x16
#      define KEY_I         0x17
#      define  KEY_O        0x18
#      define KEY_P         0x19
#      define KEY_BRACK_OPEN   0x1A
#      define KEY_BRACK_CLOSE  0x1B
#      define KEY_ENTER     0x1C
#      define KEY_LEFT_CTRL 0x1D
#      define KEY_A         0x1E
#      define KEY_S         0x1F
#      define KEY_D         0x20
#      define KEY_F         0x21
#      define KEY_X         0x2D
#      define KEY_C         0x2E
#      define KEY_V         0x2F
#      define KEY_B         0x30
#      define KEY_N         0x31
#      define KEY_M         0x32
#      define KEY_GRAY_SLASH   0x35
#      define KEY_RIGHT_SHIFT  0x36
#      define KEY_GRAY_STAR 0x37
#      define KEY_LEFT_ALT     0x38
#      define KEY_SPACE     0x39
#      define KEY_CAPS         0x3A
#      define KEY_F1        0x3B
#      define KEY_F2        0x3C
#      define KEY_F3        0x3D
#      define KEY_F4        0x3E
#      define KEY_F5        0x3F
#      define KEY_F6        0x40
#      define KEY_F7        0x41
#      define KEY_F8        0x42
#      define KEY_F9        0x43
#      define KEY_F10       0x44
#      define KEY_UP        0x48
#      define KEY_LEFT      0x4B
#      define KEY_CENTER    0x4C
#      define KEY_RIGHT     0x4D
#      define KEY_DOWN      0x50
#      define KEY_DEL       0x53
#      define KEY_F11       0x57
#      define KEY_F12       0x58
#      define KEY_RIGHT_CTRL   BIT_7+0x1D
#      define KEY_RIGHT_ALT BIT_7+0x38
#      define KEY_GRAY_UP      BIT_7+0x48
#      define KEY_GRAY_PGUP BIT_7+0x49
#      define KEY_GRAY_MINUS   BIT_7+0x4A
#      define KEY_GRAY_LEFT BIT_7+0x4B
#      define KEY_GRAY_RIGHT   BIT_7+0x4D
#      define KEY_GRAY_PLUS BIT_7+0x4E
#      define KEY_GRAY_END     BIT_7+0x4F
#      define KEY_GRAY_DOWN BIT_7+0x50
#      define KEY_GRAY_PGDN BIT_7+0x51
#      define KEY_GRAY_INS     BIT_7+0x52
#      define KEY_GRAY_DEL     BIT_7+0x53
#    endif
#  endif
#endif
#endif
// $Log: keybrd.h,v $
// Revision 1.16  2004/08/11 11:41:06  d3x0r
// Begin seperation of key and render
//
// Revision 1.15  2004/06/01 21:53:43  d3x0r
// Fix PUBLIC dfeinitions from Windoze-centric to system nonspecified
//
// Revision 1.14  2004/04/27 04:58:16  d3x0r
// Forgot to macro a function..
//
// Revision 1.13  2004/04/27 03:06:16  d3x0r
// Define F1-F10
//
// Revision 1.12  2004/03/05 23:33:21  d3x0r
// Missing keydefs - may be wrong.
//
// Revision 1.11  2003/03/25 08:38:11  panther
// Add logging
//
/* Crafted by Jim Buckeyne (c)1999-2006++ Freedom Collective
   Image building tracking, and simple manipulations.        */
// if the library is to have it's own idea of what
// an image is - then it should have included
// the definition for 'SFTFont', and 'Image' before
// including this... otherwise, it is assumed to
// be a client, and therefore does not need the information
// if a custom structure is used - then it MUST define
// it's ACTUAL x,y,width,height as the first 4 int32_t bit values.
#ifndef IMAGE_H
// multiple inclusion protection symbol
#define IMAGE_H
#if defined( _MSC_VER ) && defined( SACK_BAG_EXPORTS ) && 0
#define HAS_ASSEMBLY
#endif
/* Define COLOR type. Basically the image library regards color
   as 32 bits of data. User applications end up needing to
   specify colors in the correct method for the platform they
   are working on. This provides aliases to rearrange colors.
   For instance the colors on windows and the colors for OpenGL
   are not exactly the same. If the OpenGL driver is specified
   as the output device, the entire code would need to be
   rebuilt for specifying colors correctly for opengl. While
   otherwise they are both 32 bits, and pieces work, they get
   very ugly colors output.
   See Also
   <link Colors>                                                */
#ifndef COLOR_STRUCTURE_DEFINED
/* An exclusion symbol for defining CDATA and color operations. */
#define COLOR_STRUCTURE_DEFINED
#ifdef __cplusplus
SACK_NAMESPACE
	namespace image {
#endif
		// byte index values for colors on the video buffer...
		enum color_byte_index {
 I_BLUE  = 0,
 I_GREEN = 1,
 I_RED   = 2,
 I_ALPHA = 3
		};
#if defined( __ANDROID__ ) || defined( _OPENGL_DRIVER )
#  define USE_OPENGL_COMPAT_COLORS
#endif
#if ( !defined( IMAGE_LIBRARY_SOURCE_MAIN ) && ( !defined( FORCE_NO_INTERFACE ) || defined( ALLOW_IMAGE_INTERFACE ) ) )      && !defined( FORCE_COLOR_MACROS )
#define Color( r,g,b ) MakeColor(r,g,b)
#define AColor( r,g,b,a ) MakeAlphaColor(r,g,b,a)
#define SetAlpha( rgb, a ) SetAlphaValue( rgb, a )
#define SetGreen( rgb, g ) SetGreeValue(rgb,g )
#define AlphaVal(color) GetAlphaValue( color )
#define RedVal(color)   GetRedValue(color)
#define GreenVal(color) GetGreenValue(color)
#define BlueVal(color)  GetBlueValue(color)
#else
#if defined( _OPENGL_DRIVER ) || defined( USE_OPENGL_COMPAT_COLORS )
#  define Color( r,g,b ) (((uint32_t)( ((uint8_t)(r))|((uint16_t)((uint8_t)(g))<<8))|(((uint32_t)((uint8_t)(b))<<16)))|0xFF000000)
#  define AColor( r,g,b,a ) (((uint32_t)( ((uint8_t)(r))|((uint16_t)((uint8_t)(g))<<8))|(((uint32_t)((uint8_t)(b))<<16)))|((a)<<24))
#  define SetAlpha( rgb, a ) ( ((rgb)&0x00FFFFFF) | ( (a)<<24 ) )
#  define SetGreen( rgb, g ) ( ((rgb)&0xFFFF00FF) | ( ((g)&0xFF)<<8 ) )
#  define SetBlue( rgb, b )  ( ((rgb)&0xFF00FFFF) | ( ((b)&0xFF)<<16 ) )
#  define SetRed( rgb, r )   ( ((rgb)&0xFFFFFF00) | ( ((r)&0xFF)<<0 ) )
#  define GLColor( c )  (c)
#  define AlphaVal(color) ((color&0xFF000000) >> 24)
#  define RedVal(color)   ((color&0x000000FF) >> 0)
#  define GreenVal(color) ((color&0x0000FF00) >> 8)
#  define BlueVal(color)  ((color&0x00FF0000) >> 16)
#else
#  ifdef _WIN64
#    define AND_FF &0xFF
#  else
/* This is a macro to cure a 64bit warning in visual studio. */
#    define AND_FF
#  endif
/* A macro to create a solid color from R G B coordinates.
   Example
   <code lang="c++">
   CDATA color1 = Color( 255,0,0 ); // Red only, so this is bright red
   CDATA color2 = Color( 0,255,0); // green only, this is bright green
   CDATA color3 = Color( 0,0,255); // blue only, this is birght blue
   CDATA color4 = Color(93,93,32); // this is probably a goldish grey
   </code>                                                             */
#define Color( r,g,b ) (((uint32_t)( ((uint8_t)((b)AND_FF))|((uint16_t)((uint8_t)((g))AND_FF)<<8))|(((uint32_t)((uint8_t)((r))AND_FF)<<16)))|0xFF000000)
/* Build a color with alpha specified. */
#define AColor( r,g,b,a ) (((uint32_t)( ((uint8_t)((b)AND_FF))|((uint16_t)((uint8_t)((g))AND_FF)<<8))|(((uint32_t)((uint8_t)((r))AND_FF)<<16)))|(((a)AND_FF)<<24))
/* Sets the alpha part of a color. (0-255 value, 0 being
   transparent, and 255 solid(opaque))
   Example
   <code lang="c++">
   CDATA color = BASE_COLOR_RED;
   CDATA hazy_color = SetAlpha( color, 128 );
   </code>
 */
#define SetAlpha( rgb, a ) ( ((rgb)&0x00FFFFFF) | ( (a)<<24 ) )
/* Sets the green channel of a color. Expects a value 0-255.  */
#define SetGreen( rgb, g ) ( ((rgb)&0xFFFF00FF) | ( ((g)&0x0000FF)<<8 ) )
/* Sets the blue channel of a color. Expects a value 0-255.  */
#define SetBlue( rgb, b ) ( ((rgb)&0xFFFFFF00) | ( ((b)&0x0000FF)<<0 ) )
/* Sets the red channel of a color. Expects a value 0-255.  */
#define SetRed( rgb, r ) ( ((rgb)&0xFF00FFFF) | ( ((r)&0x0000FF)<<16 ) )
/* Return a CDATA that is meant for output to OpenGL. */
#define GLColor( c )  (((c)&0xFF00FF00)|(((c)&0xFF0000)>>16)|(((c)&0x0000FF)<<16))
/* Get the alpha value of a color. This is a 0-255 unsigned
   byte.                                                    */
#define AlphaVal(color) (((color) >> 24) & 0xFF)
/* Get the red value of a color. This is a 0-255 unsigned byte. */
#define RedVal(color)   (((color) >> 16) & 0xFF)
/* Get the green value of a color. This is a 0-255 unsigned
   byte.                                                    */
#define GreenVal(color) (((color) >> 8) & 0xFF)
/* Get the blue value of a color. This is a 0-255 unsigned byte. */
#define BlueVal(color)  (((color)) & 0xFF)
#endif
 // IMAGE_LIBRARY_SOURCE
#endif
		/* a definition for a single color channel - for function replacements for ___Val macros*/
		typedef unsigned char COLOR_CHANNEL;
        /* a 4 byte array of color (not really used, we mostly went with CDATA and PCDATA instead of COLOR and PCOLOR */
		typedef COLOR_CHANNEL COLOR[4];
		// color data raw...
		typedef uint32_t CDATA;
		/* pointer to an array of 32 bit colors */
		typedef uint32_t *PCDATA;
		/* A Pointer to <link COLOR>. Probably an array of color (a
		 block of pixels for instance)                            */
		typedef COLOR *PCOLOR;
//-----------------------------------------------
// common color definitions....
//-----------------------------------------------
// both yellows need to be fixed.
#define BASE_COLOR_BLACK         Color( 0,0,0 )
#define BASE_COLOR_BLUE          Color( 0, 0, 128 )
#define BASE_COLOR_DARKBLUE          Color( 0, 0, 42 )
/* An opaque Green.
   See Also
   <link Colors>    */
#define BASE_COLOR_GREEN         Color( 0, 128, 0 )
/* An opaque cyan - kind of a light sky like blue.
   See Also
   <link Colors>                                   */
#define BASE_COLOR_CYAN          Color( 0, 128, 128 )
/* An opaque red.
   See Also
   <link Colors>  */
#define BASE_COLOR_RED           Color( 192, 32, 32 )
/* An opaque BROWN. Brown is dark yellow... so this might be
   more like a gold sort of color instead.
   See Also
   <link Colors>                                             */
#define BASE_COLOR_BROWN         Color( 140, 140, 0 )
#define BASE_COLOR_LIGHTBROWN         Color( 221, 221, 85 )
#define BASE_COLOR_MAGENTA       Color( 160, 0, 160 )
#define BASE_COLOR_LIGHTGREY     Color( 192, 192, 192 )
/* An opaque darker grey (gray?).
   See Also
   <link Colors>                  */
#define BASE_COLOR_DARKGREY      Color( 128, 128, 128 )
/* An opaque a bight or light color blue.
   See Also
   <link Colors>                          */
#define BASE_COLOR_LIGHTBLUE     Color( 0, 0, 255 )
/* An opaque lighter, brighter green color.
   See Also
   <link Colors>                            */
#define BASE_COLOR_LIGHTGREEN    Color( 0, 255, 0 )
/* An opaque a lighter, more bight cyan color.
   See Also
   <link Colors>                               */
#define BASE_COLOR_LIGHTCYAN     Color( 0, 255, 255 )
/* An opaque bright red.
   See Also
   <link Colors>         */
#define BASE_COLOR_LIGHTRED      Color( 255, 0, 0 )
/* An opaque Lighter pink sort of red-blue color.
   See Also
   <link Colors>                                  */
#define BASE_COLOR_LIGHTMAGENTA  Color( 255, 0, 255 )
/* An opaque bright yellow.
   See Also
   <link Colors>            */
#define BASE_COLOR_YELLOW        Color( 255, 255, 0 )
/* An opaque White.
   See Also
   <link Colors>    */
#define BASE_COLOR_WHITE         Color( 255, 255, 255 )
#define BASE_COLOR_ORANGE        Color( 204,96,7 )
#define BASE_COLOR_NICE_ORANGE   Color( 0xE9, 0x7D, 0x26 )
#define BASE_COLOR_PURPLE        Color( 0x7A, 0x11, 0x7C )
#ifdef __cplusplus
 //	 namespace image {
}
SACK_NAMESPACE_END
using namespace sack::image;
#endif
#endif
// $Log: colordef.h,v $
// Revision 1.4  2003/04/24 00:03:49  panther
// Added ColorAverage to image... Fixed a couple macros
//
// Revision 1.3  2003/03/25 08:38:11  panther
// Add logging
//
/* Defines a simple FRACTION type. Fractions are useful for
   scaling one value to another. These operations are handles
   continously. so iterating a fraction like 13 denominations of
   100 will be smooth.                                           */
#ifndef FRACTIONS_DEFINED
/* Multiple inclusion protection symbol. */
#define FRACTIONS_DEFINED
#ifdef __cplusplus
#  define _FRACTION_NAMESPACE namespace fraction {
#  define _FRACTION_NAMESPACE_END }
#  ifndef _MATH_NAMESPACE
#    define _MATH_NAMESPACE namespace math {
#  endif
#  define	 SACK_MATH_FRACTION_NAMESPACE_END } } }
#else
#  define _FRACTION_NAMESPACE
#  define _FRACTION_NAMESPACE_END
#  ifndef _MATH_NAMESPACE
#    define _MATH_NAMESPACE
#  endif
#  define	 SACK_MATH_FRACTION_NAMESPACE_END
#endif
SACK_NAMESPACE
	/* Namespace of custom math routines.  Contains operators
	 for Vectors and fractions. */
	_MATH_NAMESPACE
	/* Fraction namespace contains a PFRACTION type which is used to
   store integer fraction values. Provides for ration and
   proportion scaling. Can also represent fractions that contain
   a whole part and a fractional part (5 2/3 : five and
	two-thirds).                                                  */
	_FRACTION_NAMESPACE
/* Define the call type of the function. */
#define FRACTION_API CPROC
#  ifdef FRACTION_SOURCE
#    define FRACTION_PROC EXPORT_METHOD
#  else
/* Define the library linkage for a these functions. */
#    define FRACTION_PROC IMPORT_METHOD
#  endif
/* The faction type. Stores a fraction as integer
   numerator/denominator instead of a floating point scalar. */
/* Pointer to a <link sack::math::fraction::FRACTION, FRACTION>. */
/* The faction type. Stores a fraction as integer
   numerator/denominator instead of a floating point scalar. */
typedef struct fraction_tag {
	/* Numerator of the fraction. (This is the number on top of a
	   fraction.)                                                 */
	int numerator;
	/* Denominator of the fraction. (This is the number on bottom of
	   a fraction.) This specifies the denominations.                */
	int denominator;
} FRACTION, *PFRACTION;
#ifdef HAVE_ANONYMOUS_STRUCTURES
typedef struct coordpair_tag {
	union {
		FRACTION x;
		FRACTION width;
	};
	union {
		FRACTION y;
		FRACTION height;
	};
} COORDPAIR, *PCOORDPAIR;
#else
/* A coordinate pair is a 2 dimensional fraction expression. can
   be regarded as x, y or width,height. Each coordiante is a
   Fraction type.                                                */
typedef struct coordpair_tag {
	       /* The x part of the coordpair. */
	       FRACTION x;
	       /* The y part of the coordpair. */
	       FRACTION y;
} COORDPAIR, *PCOORDPAIR;
#endif
/* \ \
   Parameters
   fraction :     the fraction to set
   numerator :    numerator of the fraction
   demoninator :  denominator of the fraction */
#define SetFraction(f,n,d) ((((f).numerator=((int)(n)) ),((f).denominator=((int)(d)))),(f))
/* Sets the value of a FRACTION. This is passed as the whole
   number and the fraction.
   Parameters
   fraction :  the fraction to set
   w :         this is the whole number to set
   n :         numerator of remainder to set
   d :         denominator of fraction to set.
   Example
   Fraction f = 3 1/2;
   <code lang="c++">
   FRACTION f;
   SetFractionV( f, 3, 1, 2 );
   // the resulting fraction will be 7/2
   </code>                                                   */
#define SetFractionV(f,w,n,d) (  (d)?	 ((((f).numerator=((int)((n)*(w))) )	  ,((f).denominator=((int)(d)))),(f))	  :	 ((((f).numerator=((int)((w))) )	  ,((f).denominator=((int)(1)))),(f))  )
/* \ \
   Parameters
   base :    origin point (content is modified by adding offset
             to it)
   offset :  offset point                                       */
FRACTION_PROC  void FRACTION_API  AddCoords ( PCOORDPAIR base, PCOORDPAIR offset );
/* Add one fraction to another.
   Parameters
   base :    This is the starting value, and recevies the result
             of (base+offset)
   offset :  This is the fraction to add to base.
   Returns
   base                                                          */
FRACTION_PROC  PFRACTION FRACTION_API  AddFractions ( PFRACTION base, PFRACTION offset );
/* Add one fraction to another.
   Parameters
   base :    This is the starting value, and recevies the result
             of (base+offset)
   offset :  This is the fraction to add to base.
   Returns
   base                                                          */
FRACTION_PROC  PFRACTION FRACTION_API  SubtractFractions ( PFRACTION base, PFRACTION offset );
/* NOT IMPLEMENTED */
FRACTION_PROC  PFRACTION FRACTION_API  MulFractions ( PFRACTION f, PFRACTION x );
/* Log a fraction into a string. */
FRACTION_PROC  int FRACTION_API  sLogFraction ( TEXTCHAR *string, PFRACTION x );
/* Unsafe log of a coordinate pair's value into a string. The
   string should be at least 69 characters long.
   Parameters
   string :  the string to print the fraction into
   pcp :     the coordinate pair to print                     */
FRACTION_PROC  int FRACTION_API  sLogCoords ( TEXTCHAR *string, PCOORDPAIR pcp );
/* Log coordpair to logfile. */
FRACTION_PROC  void FRACTION_API  LogCoords ( PCOORDPAIR pcp );
/* scales a fraction by a signed integer value.
   Parameters
   result\ :  pointer to a FRACTION to receive the result
   value :    the amount to be scaled
   f :        the fraction to multiply the value by
   Returns
   \result; the pointer the fraction to receive the result. */
FRACTION_PROC  PFRACTION FRACTION_API  ScaleFraction ( PFRACTION result, int32_t value, PFRACTION f );
/* Results in the integer part of the fraction. If the faction
   was 330/10 then the result would be 33.                     */
FRACTION_PROC  int32_t FRACTION_API  ReduceFraction ( PFRACTION f );
/* Scales a 32 bit integer value by a fraction. The result is
   the scaled value result.
   Parameters
   f :      pointer to the faction to multiply value by
   value :  the value to scale
   Returns
   The (value * f) integer value of.                          */
FRACTION_PROC  uint32_t FRACTION_API  ScaleValue ( PFRACTION f, int32_t value );
/* \ \
   Parameters
   f :      The fraction to scale the value by
   value :  the value to scale by (1/f)
   Returns
   the value of ( value * 1/ f )               */
FRACTION_PROC  uint32_t FRACTION_API  InverseScaleValue ( PFRACTION f, int32_t value );
	SACK_MATH_FRACTION_NAMESPACE_END
#ifdef __cplusplus
using namespace sack::math::fraction;
#endif
#endif
//---------------------------------------------------------------------------
// $Log: fractions.h,v $
// Revision 1.6  2004/09/03 14:43:40  d3x0r
// flexible frame reactions to font changes...
//
// Revision 1.5  2003/03/25 08:38:11  panther
// Add logging
//
// Revision 1.4  2003/01/27 09:45:03  panther
// Fix lack of anonymous structures
//
// Revision 1.3  2002/10/09 13:16:02  panther
// Support for linux shared memory mapping.
// Support for better linux compilation of configuration scripts...
// Timers library is now Threads AND Timers.
//
//
#ifndef __NO_INTERFACES__
/*
 * Create: James Buckeyne
 *
 * Purpose: Provide a general structure to register names of
 *   routines and data structures which may be consulted
 *   for runtime linking.  Aliases and other features make this
 *   a useful library for tracking interface registration...
 *
 *  The namespace may be enumerated.
 */
#ifndef PROCEDURE_REGISTRY_LIBRARY_DEFINED
#define PROCEDURE_REGISTRY_LIBRARY_DEFINED
#ifndef DEADSTART_DEFINED
#define DEADSTART_DEFINED
#ifdef WIN32
//#include <stdhdrs.h>
#endif
 // leach, assuming this will be compiled with this part at least.
#define pastejunk_(a,b) a##b
#define pastejunk(a,b) pastejunk_(a,b)
#ifdef __cplusplus
#define USE_SACK_DEADSTART_NAMESPACE using namespace sack::app::deadstart;
#define SACK_DEADSTART_NAMESPACE   SACK_NAMESPACE namespace app { namespace deadstart {
#define SACK_DEADSTART_NAMESPACE_END    } } SACK_NAMESPACE_END
SACK_NAMESPACE
	namespace app{
/* Application namespace. */
/* These are compiler-platform abstractions to provide a method
   of initialization that allows for creation of threads, and
   transparent (easy to use) method of scheduling routines for
   initialization.
   Example
   This schedules a routine to run at startup. Fill in the
   routine with the code you want, and it will run at
   DEFAULT_PRELOAD_PRIORITY which is the number 69.
   <code lang="c++">
   PRELOAD( MyCustomInit )
   {
       // do something here (do anything here,
       // without limitations that are imposed by DllMain/LibMain.
   }
   </code>
   If you wanted a routine which was guaranteed to run before
   MyCustomInit you might use PRIORITY_PRELOAD whcih allows you
   to specify a priority.
   <code lang="c++">
   PRIORITY_PRELOAD( MyOtherInit, DEFAULT_PRELOAD_PRIORITY-10 )
   {
      // this will run before other things.
   }
   </code>
   Priorities are listed in deadstart.h and exit_priorities.h. The
   priorities are treated backwards, so low number startup
   priorities go first, and higher number shutdown priorities go
   first.
   Remarks
   In some compilers and compile modes this is also fairly easy
   to do. A lot of compilers do not offer priority, and are
   impossible to maintain an order in. Some compilers only
   provide startup priority for C++ mode. This system works as
   \long as there is a way to run a single function at some
   point before main() and after C runtime initializes.
   In Windows, you might think you have this ability with
   DllMain, but there are severe limitations that you would have
   to get around; primary is the inability to create a thread,
   well, you can create it, but it will remain suspended until
   you leave DllMains and all DllMains finish. There is also no
   way to consistantly provide initialization order, like memory
   needs to be initialized before anything else.
                                                                   */
		namespace deadstart {
#else
#define USE_SACK_DEADSTART_NAMESPACE
#define SACK_DEADSTART_NAMESPACE
#define SACK_DEADSTART_NAMESPACE_END
#endif
#ifdef TYPELIB_SOURCE
#define DEADSTART_SOURCE
#endif
/* A macro to specify the call type of schedule routines. This
   can be changed in most projects without affect, it comes into
   play if plugins built by different compilers are used,
   __cdecl is most standard.                                     */
#define DEADSTART_CALLTYPE CPROC
#  if defined( _TYPELIBRARY_SOURCE_STEAL )
#    define DEADSTART_PROC extern
#  elif defined( _TYPELIBRARY_SOURCE )
#    define DEADSTART_PROC EXPORT_METHOD
#  else
/* A definition for how to declare these functions. if the
   source itself is comipling these are _export, otherwise
   external things linking here are _import.               */
#    define DEADSTART_PROC IMPORT_METHOD
#  endif
   /* this is just a global space initializer (shared, named
      region, allows static link plugins to share information)
      Allocates its shared memory global region, so if this library
      is built statically and referenced in multiple plugins
      ConfigScript can share the same symbol tables. This also
      provides sharing between C++ and C.                           */
#define CONFIG_SCRIPT_PRELOAD_PRIORITY    (SQL_PRELOAD_PRIORITY-3)
   // this is just a global space initializer (shared, named region, allows static link plugins to share information)
#define SQL_PRELOAD_PRIORITY    (SYSLOG_PRELOAD_PRIORITY-1)
/* Level at which logging is initialized. Nothing under this
   should be doing logging, if it does, the behavior is not as
   well defined.                                               */
#define SYSLOG_PRELOAD_PRIORITY 35
   // global_init_preload_priority-1 is used by sharemem.. memory needs init before it can register itself
#define GLOBAL_INIT_PRELOAD_PRIORITY 37
 // OS A[bstraction] L[ayer] O[n] T[op] - system lib
#define OSALOT_PRELOAD_PRIORITY (CONFIG_SCRIPT_PRELOAD_PRIORITY-1)
/* Level which names initializes. Names is the process
   registration code. It has a common shared global registered.
   <link sack::app::registry, procreg; aka names.c>             */
#define NAMESPACE_PRELOAD_PRIORITY 39
/* image_preload MUST be after Namespce preload (anything that
   uses RegisterAndCreateGlobal) should init this before vidlib
   (which needs image?)                                         */
#define IMAGE_PRELOAD_PRIORITY  45
/* Level at which the video render library performs its
   initialization; RegisterClass() level code.          */
#define VIDLIB_PRELOAD_PRIORITY 46
/* Initialization level where PSI registers its builtin
   controls.                                            */
#define PSI_PRELOAD_PRIORITY    47
// need to open the queues and threads before the service server can begin...
#define MESSAGE_CLIENT_PRELOAD_PRIORITY 65
/* Level which message core service initializes. During startup
   message services can register themselves also; but not before
   this priority level.                                          */
#define MESSAGE_SERVICE_PRELOAD_PRIORITY 66
/* Routines are scheduled at this priority when the PRELOAD
   function is used.                                        */
#define DEFAULT_PRELOAD_PRIORITY (DEADSTART_PRELOAD_PRIORITY-1)
/* Not sure where this is referenced, this the core routine
   itself is scheduled with this symbol to the compiler if
   appropriate.                                             */
#define DEADSTART_PRELOAD_PRIORITY 70
#define PRIORITY_UNLOAD(proc,priority) PRIORITY_ATEXIT( proc##_unload, priority )
/* Used by PRELOAD and PRIORITY_PRELOAD macros to register a
   startup routine at a specific priority. Lower number
   priorities are scheduled to run before higher number
   priorities*backwards from ATEXIT priorities*. Using this
   scheduling mechanisms, routines which create threads under
   windows are guaranteed to run before main, and are guaranteed
   able to create threads. (They are outside of the loader lock)
   Parameters
   function :  pointer to a function to call at startup.
   name :      text name of the function
   priority :  priority at which to call the function.
   unused :    this is an unused parameter. The macros fill it
               with &amp;ThisRegisteringRoutine, so that the
               routine itself is referenced by code, and helps
               the compile not optimize out this code. The
               functions which perform the registration are prone
               to be optimized because it's hard for the compiler
               to identify that they are refernced by other names
               indirectly.
   file\ :     usually DBG_PASS of the code doing this
               registration.
   line :      usually DBG_PASS of the code doing this
               registration.                                      */
DEADSTART_PROC  void DEADSTART_CALLTYPE  RegisterPriorityStartupProc( void(CPROC*)(void), CTEXTSTR,int,void* unused DBG_PASS);
/* Used by ATEXIT and PRIORITY_ATEXIT macros to register a
   shutdown routine at a specific priority. Higher number
   priorities are scheduled to run before lower number
   priorities. *backwards from PRELOAD priorities* This
   registers functions which are run while the program exits if
   it is at all able to run when exiting. calling exit() or
   BAG_Exit() will invoke these.
   Parameters
   function :  pointer to a function to call at shutdown.
   name :      text name of the function
   priority :  priority at which to call the function.
   unused :    this is an unused parameter. The macros fill it
               with &amp;ThisRegisteringRoutine, so that the
               routine itself is referenced by code, and helps
               the compile not optimize out this code. The
               functions which perform the registration are prone
               to be optimized because it's hard for the compiler
               to identify that they are refernced by other names
               indirectly.
   file\ :     usually DBG_PASS of the code doing this
               registration.
   line :      usually DBG_PASS of the code doing this
               registration.                                      */
DEADSTART_PROC  void DEADSTART_CALLTYPE  RegisterPriorityShutdownProc( void(CPROC*)(void), CTEXTSTR,int,void* unused DBG_PASS);
/* This routine is used internally when LoadFunction is called.
   After MarkDeadstartComplete is called, any call to a
   RegisterPriorityStartupProc will call the startup routine
   immediately instead of waiting. This function disables the
   auto-running of this function, and instead enques the startup
   to the list of startups. When completed, at some later point,
   call ResumeDeadstart() to dispatched all scheduled routines,
   and release the suspend; however, if initial deastart was not
   dispatched, then ResumeDeadstart does not do the invoke, it
   only releases the suspend.                                    */
DEADSTART_PROC  void DEADSTART_CALLTYPE  SuspendDeadstart ( void );
/* Resumes a suspended deadstart. If root deadstart is
   completed, then ResumeDeadstart will call InvokeDeadstarts
   after resuming deadstart.                                  */
DEADSTART_PROC  void DEADSTART_CALLTYPE  ResumeDeadstart ( void );
/* Not usually used by user code, but this invokes all the
   routines which have been scheduled to run for startup. If
   your compiler doesn't have a method of handling deadstart
   code, this can be manually called. It can also be called if
   you loaded a library yourself without using the LoadFunction
   interface, to invoke startups scheduled in the loaded
   library.                                                     */
DEADSTART_PROC  void DEADSTART_CALLTYPE  InvokeDeadstart (void);
/* This just calls the list of shutdown procedures. This should
   not be used usually from user code, since internally this is
   handled by catching atexit() or with a static destructor.    */
DEADSTART_PROC  void DEADSTART_CALLTYPE  InvokeExits (void);
/* This is typically called after the first InvokeDeadstarts
   completes. The code that runs this is usually a routine just
   before main(). So once code in main begins to run, all prior
   initialization has been performed.                           */
DEADSTART_PROC  void DEADSTART_CALLTYPE  MarkRootDeadstartComplete ( void );
/* \returns whether InvokeDeadstarts has been called. */
DEADSTART_PROC  LOGICAL DEADSTART_CALLTYPE  IsRootDeadstartStarted ( void );
/* \returns whether MarkRootDeadstartComplete has been called. */
DEADSTART_PROC  LOGICAL DEADSTART_CALLTYPE  IsRootDeadstartComplete ( void );
#if defined( __LINUX__ )
// call this after a fork().  Otherwise, it will falsely invoke shutdown when it exits.
DEADSTART_PROC  void DEADSTART_CALLTYPE  DispelDeadstart ( void );
#endif
#ifdef DOC_O_MAT
// call this after a fork().  Otherwise, it will falsely invoke shutdown when it exits.
DEADSTART_PROC  void DEADSTART_CALLTYPE  DispelDeadstart ( void );
#endif
#ifdef __cplusplus
/* Defines some code to run at program inialization time. Allows
   specification of a priority. Lower priorities run first. (default
   is 69).
   Example
   <code>
   PRIORITY_PRELOAD( MyOtherInit, 153 )
   {
      // run some code probably after most all other initializtion is done.
   }
   </code>
   See Also
   <link sack::app::deadstart, deadstart Namespace>                         */
#define PRIORITY_PRELOAD(name,priority) static void CPROC name(void);	 namespace { static class pastejunk(schedule_,name) {        public:pastejunk(schedule_,name)() {	    RegisterPriorityStartupProc( name,TOSTR(name),priority,(void*)this DBG_SRC);	  }	  } pastejunk(do_schedule_,name);   }	  static void name(void)
/* This is used once in deadstart_prog.c which is used to invoke
   startups when the program finishes loading.                   */
#define MAGIC_PRIORITY_PRELOAD(name,priority) static void CPROC name(void);	 namespace { static class pastejunk(schedule_,name) {	     public:pastejunk(schedule_,name)() {	  name();	    }	  } pastejunk(do_schedul_,name);   }	  static void name(void)
/* A macro to define some code to run during program shutdown. An
   additional priority may be specified if the order matters. Higher
   numbers are called first.
                                                                     */
#define ATEXIT_PRIORITY(name,priority) static void CPROC name(void);    static class pastejunk(schedule_,name) {        public:pastejunk(schedule_,name)() {	    RegisterPriorityShutdownProc( name,TOSTR(name),priority,(void*)this DBG_SRC );	  }	  } pastejunk(do_schedule_,name);	     static void name(void)
/* Defines some code to run at program shutdown time. Allows
   specification of a priority. Higher priorities are run first.
   Example
   <code>
   PRIORITY_ATEXIT( MyOtherShutdown, 153 )
   {
      // run some code probably before most library code dissolves.
      // last to load, first to unload.
   }
   </code>
   See Also
   <link sack::app::deadstart, deadstart Namespace>                 */
	/*name(); / * call on destructor of static object.*/
#define PRIORITY_ATEXIT(name,priority) static void CPROC name(void);    static class pastejunk(shutdown_,name) {	   public:pastejunk(shutdown_,name)() {       RegisterPriorityShutdownProc( name,TOSTR(name),priority,(void*)this DBG_SRC );	   }	  } do_shutdown_##name;	     void name(void)
/* This is the most basic way to define some code to run
   initialization before main.
   Example
   <code lang="c++">
   PRELOAD( MyInitCode )
   {
      // some code here
   }
   </code>
   See Also
   <link sack::app::deadstart, deadstart Namespace>      */
#define PRELOAD(name) PRIORITY_PRELOAD(name,DEFAULT_PRELOAD_PRIORITY)
/* Basic way to register a routine to run when the program exits
   gracefully.
   Example
   \    <code>
   ATEXIT( MyExitRoutine )
   {
       // this will be run sometime during program shutdown
   }
   </code>                                                       */
#define ATEXIT(name)      PRIORITY_ATEXIT(name,ATEXIT_PRIORITY_DEFAULT)
/* This is the core atexit. It dispatches all other exit
   routines. This is defined for internal use only...    */
#define ROOT_ATEXIT(name) ATEXIT_PRIORITY(name,ATEXIT_PRIORITY_ROOT)
//------------------------------------------------------------------------------------
// Win32 Watcom
//------------------------------------------------------------------------------------
#elif defined( __WATCOMC__ )
#pragma off (check_stack)
/* code taken from openwatcom/bld/watcom/h/rtinit.h */
typedef unsigned char   __type_rtp;
typedef unsigned short  __type_pad;
typedef void(*__type_rtn ) ( void );
#ifdef __cplusplus
#pragma pack(1)
#else
#pragma pack(1)
#endif
 // structure placed in XI/YI segment
struct rt_init
{
#define DEADSTART_RT_LIST_START 0xFF
 // - near=0/far=1 routine indication
    __type_rtp  rtn_type;
                          //   also used when walking table to flag
                          //   completed entries
 // - priority (0-highest 255-lowest)
    __type_rtp  priority;
      // - routine
    __type_rtn  rtn;
};
#pragma pack()
/* end code taken from openwatcom/bld/watcom/h/rtinit.h */
//------------------------------------------------------------------------------------
// watcom
//------------------------------------------------------------------------------------
//void RegisterStartupProc( void (*proc)(void) );
#define PRIORITY_PRELOAD(name,priority) static void pastejunk(schedule_,name)(void); static void CPROC name(void);	 static struct rt_init __based(__segname("XI")) pastejunk(name,_ctor_label)={0,(DEADSTART_PRELOAD_PRIORITY-1),pastejunk(schedule_,name)};	 static void pastejunk(schedule_,name)(void) {	                 RegisterPriorityStartupProc( name,TOSTR(name),priority,&pastejunk(name,_ctor_label) DBG_SRC );	}	                                       void name(void)
#define ATEXIT_PRIORITY(name,priority) static void pastejunk(schedule_exit_,name)(void); static void CPROC name(void);	 static struct rt_init __based(__segname("XI")) pastejunk(name,_dtor_label)={0,69,pastejunk(schedule_exit_,name)};	 static void pastejunk(schedule_exit_,name)(void) {	                                              RegisterPriorityShutdownProc( name,TOSTR(name),priority,&name##_dtor_label DBG_SRC );	}	                                       void name(void)
// syslog runs preload at priority 65
// message service runs preload priority 66
// deadstart itself tries to run at priority 70 (after all others have registered)
#define PRELOAD(name) PRIORITY_PRELOAD(name,DEFAULT_PRELOAD_PRIORITY)
// this is a special case macro used in client.c
// perhaps all PRIORITY_ATEXIT routines should use this
// this enables cleaning up things that require threads to be
// active under windows... (message disconnect)
// however this routine is only triggered in windows by calling
// BAG_Exit(nn) which is aliased to replace exit(n) automatically
#define PRIORITY_ATEXIT(name,priority) ATEXIT_PRIORITY( name,priority)
/*
static void name(void); static void name##_x_(void);	static struct rt_init __based(__segname("YI")) name##_dtor_label={0,priority,name##_x_};	 static void name##_x_(void) { char myname[256];myname[0]=*(CTEXTSTR)&name##_dtor_label;GetModuleFileName(NULL,myname,sizeof(myname));name(); }	 static void name(void)
  */
#define ROOT_ATEXIT(name) ATEXIT_PRIORITY(name,ATEXIT_PRIORITY_ROOT)
#define ATEXIT(name)      PRIORITY_ATEXIT(name,ATEXIT_PRIORITY_DEFAULT)
// if priority_atexit is used with priority 0 - the proc is scheduled into
// atexit, and exit() is then invoked.
//#define PRIORITY_ATEXIT(name,priority) ATEXIT_PRIORITY(name,priority )
//------------------------------------------------------------------------------------
// Linux
//------------------------------------------------------------------------------------
#elif defined( __GNUC__ )
/* code taken from openwatcom/bld/watcom/h/rtinit.h */
typedef unsigned char   __type_rtp;
typedef void(*__type_rtn ) ( void );
 // structure placed in XI/YI segment
struct rt_init
{
#define DEADSTART_RT_LIST_START 0xFF
 // - near=0/far=1 routine indication
    __type_rtp  rtn_type;
                          //   also used when walking table to flag
                          //   completed entries
 // has this been scheduled? (0 if no)
    __type_rtp  scheduled;
 // - priority (0-highest 255-lowest)
    __type_rtp  priority;
#if defined( __64__ ) ||defined( __arm__ )||defined( __GNUC__ )
#define INIT_PADDING ,{0}
 // need this otherwise it's 23 bytes and that'll be bad.
	 char padding[1];
#else
#define INIT_PADDING
#endif
 // 32 bits in 64 bits....
	 int line;
// this ends up being nicely aligned for 64 bit platforms
// specially with the packed attributes
      // - routine (rtn)
	 __type_rtn  routine;
#if defined( _DEBUG ) || defined( _DEBUG_INFO )
	 CTEXTSTR file;
#endif
	 CTEXTSTR funcname;
	 struct rt_init *junk;
#if defined( _DEBUG ) || defined( _DEBUG_INFO )
#if defined( __GNUC__ ) && defined( __64__)
    // this provides padding - inter-object segments are packed
    // to 32 bytes...
	 struct rt_init *junk2[3];
#endif
#endif
} __attribute__((packed));
#if defined( _DEBUG ) || defined( _DEBUG_INFO )
#  if defined( __GNUC__ ) && defined( __64__)
#    define JUNKINIT(name) ,&pastejunk(name,_ctor_label), {0,0}
#  else
#    define JUNKINIT(name) ,&pastejunk(name,_ctor_label)
#  endif
#else
#  define JUNKINIT(name) ,&pastejunk(name,_ctor_label)
#endif
#define RTINIT_STATIC static
#define ATEXIT_PRIORITY PRIORITY_ATEXIT
#if defined( _DEBUG ) || defined( _DEBUG_INFO )
#  define PASS_FILENAME ,WIDE__FILE__
#else
#  define PASS_FILENAME
#endif
#ifdef __MAC__
#  define DEADSTART_SECTION "TEXT,deadstart_list"
#else
#  define DEADSTART_SECTION "deadstart_list"
#endif
#ifdef __MANUAL_PRELOAD__
#define PRIORITY_PRELOAD(name,pr) static void name(void);	 RTINIT_STATIC struct rt_init pastejunk(name,_ctor_label)		__attribute__((section(DEADSTART_SECTION))) __attribute__((used))	 =	 {0,0,pr INIT_PADDING, __LINE__, name PASS_FILENAME	, TOSTR(name) JUNKINIT(name)} ;	 void name(void);	 void pastejunk(registerStartup,name)(void) __attribute__((constructor));	 void pastejunk(registerStartup,name)(void) {	 RegisterPriorityStartupProc(name,TOSTR(name),pr,NULL DBG_SRC); }	 void name(void)
#else
#if defined( _WIN32 ) && defined( __GNUC__ )
#  define HIDDEN_VISIBILITY
#else
#  define HIDDEN_VISIBILITY  __attribute__((visibility("hidden")))
#endif
#define PRIORITY_PRELOAD(name,pr) static void name(void);	         RTINIT_STATIC struct rt_init pastejunk(name,_ctor_label)	         __attribute__((section(DEADSTART_SECTION))) __attribute__((used)) HIDDEN_VISIBILITY	 ={0,0,pr INIT_PADDING	                                           ,__LINE__,name	                                                 PASS_FILENAME	                                                 ,TOSTR(name)	                                                   JUNKINIT(name)};	                                               static void name(void) __attribute__((used)) HIDDEN_VISIBILITY;	 void name(void)
#endif
typedef void(*atexit_priority_proc)(void (*)(void),CTEXTSTR,int DBG_PASS);
#define PRIORITY_ATEXIT(name,priority) static void name(void);           static void pastejunk(atexit,name)(void) __attribute__((constructor));   void pastejunk(atexit,name)(void)                                        {	                                                                        RegisterPriorityShutdownProc(name,TOSTR(name),priority,NULL DBG_SRC); }                                                                        void name(void)
#define ATEXIT(name) PRIORITY_ATEXIT( name,ATEXIT_PRIORITY_DEFAULT )
#define ROOT_ATEXIT(name) static void name(void) __attribute__((destructor));    static void name(void)
#define PRELOAD(name) PRIORITY_PRELOAD(name,DEFAULT_PRELOAD_PRIORITY)
//------------------------------------------------------------------------------------
// CYGWIN (-mno-cygwin)
//------------------------------------------------------------------------------------
#elif defined( __CYGWIN__ )
/* code taken from openwatcom/bld/watcom/h/rtinit.h */
typedef unsigned char   __type_rtp;
typedef void(*__type_rtn ) ( void );
 // structure placed in XI/YI segment
struct rt_init
{
#ifdef __cplusplus
	//rt_init( int _rtn_type ) { rt_init::rtn_type = _rtn_type; }
	/*rt_init( int _priority, CTEXTSTR name, __type_rtn rtn, CTEXTSTR _file, int _line )
	{
		rtn_type = 0;
		scheduled = 0;
		priority = priority;
		file = _file;
		line = _line;
      routine = rtn;
		}
      */
#endif
#define DEADSTART_RT_LIST_START 0xFF
 // - near=0/far=1 routine indication
    __type_rtp  rtn_type;
                          //   also used when walking table to flag
                          //   completed entries
 // has this been scheduled? (0 if no)
    __type_rtp  scheduled;
 // - priority (0-highest 255-lowest)
    __type_rtp  priority;
#if defined( __GNUC__ ) || defined( __64__ ) || defined( __arm__ ) || defined( __CYGWIN__ )
#define INIT_PADDING ,{0}
 // need this otherwise it's 23 bytes and that'll be bad.
	 char padding[1];
#else
#define INIT_PADDING
#endif
 // 32 bits in 64 bits....
	 int line;
// this ends up being nicely aligned for 64 bit platforms
// specially with the packed attributes
      // - routine (rtn)
	 __type_rtn  routine;
	 CTEXTSTR file;
	 CTEXTSTR funcname;
	 struct rt_init *junk;
#if defined( __GNUC__ ) && defined( __64__ )
    // this provides padding - inter-object segments are packed
    // to 32 bytes...
	 struct rt_init *junk2[3];
#endif
} __attribute__((packed));
#define JUNKINIT(name) ,&pastejunk(name,_ctor_label)
#ifdef __cplusplus
#define RTINIT_STATIC
#else
#define RTINIT_STATIC static
#endif
typedef void(*atexit_priority_proc)(void (*)(void),CTEXTSTR,int DBG_PASS);
#define ATEXIT_PRIORITY(name,priority) static void name(void); static void atexit##name(void) __attribute__((constructor));	  void atexit_failed##name(void(*f)(void),int i,CTEXTSTR s1,CTEXTSTR s2,int n) { lprintf( "Failed to load atexit_priority registerar from core program." );} void atexit##name(void)                                                  {	                                                                        static char myname[256];HMODULE mod;if(myname[0])return;myname[0]='a';GetModuleFileName( NULL, myname, sizeof( myname ) );	mod=LoadLibrary(myname);if(mod){   typedef void (*x)(void);void(*rsp)( x,const CTEXTSTR,int,const CTEXTSTR,int);	 if((rsp=((void(*)(void(*)(void),const CTEXTSTR,int,const CTEXTSTR,int))(GetProcAddress( mod, "RegisterPriorityShutdownProc")))))	 {rsp( name,TOSTR(name),priority DBG_SRC);}	 else atexit_failed##name(name,priority,TOSTR(name) DBG_SRC);	        }     FreeLibrary( mod);	 }             void name( void)
#ifdef _DEBUG
#  define PASS_FILENAME ,WIDE__FILE__
#else
#  define PASS_FILENAME
#endif
#define PRIORITY_PRELOAD(name,pr) static void name(void);	 RTINIT_STATIC struct pastejunk(rt_init name,_ctor_label)	   __attribute__((section("deadstart_list")))	 ={0,0,pr INIT_PADDING	     ,__LINE__,name	          PASS_FILENAME	        ,TOSTR(name)	        JUNKINIT(name)};	 static void name(void)
#define ATEXIT(name)      ATEXIT_PRIORITY(name,ATEXIT_PRIORITY_DEFAULT)
#define PRIORITY_ATEXIT ATEXIT_PRIORITY
#define ROOT_ATEXIT(name) static void name(void) __attribute__((destructor));    static void name(void)
#define PRELOAD(name) PRIORITY_PRELOAD(name,DEFAULT_PRELOAD_PRIORITY)
//------------------------------------------------------------------------------------
// WIN32 MSVC
//------------------------------------------------------------------------------------
#elif defined( _MSC_VER ) && defined( _WIN32 )
//#define PRELOAD(name) __declspec(allocate(".CRT$XCAA")) void CPROC name(void)
//#pragma section(".CRT$XCA",long,read)
//#pragma section(".CRT$XCZ",long,read)
// put init in both C startup and C++ startup list...
// looks like only one or the other is invoked, not both?
/////// also the variables to be put into these segments
#if defined( __cplusplus_cli )
#define LOG_ERROR(n) System::Console::WriteLine( gcnew System::String(n) + gcnew System::String( myname) ) )
#else
#define LOG_ERROR(n) SystemLog( n )
// since we get linked first, then the runtime is added, we have to link against the last indicator of section,
// so we get put between start to end.
#define _STARTSEG_ ".CRT$XIM"
#define _STARTSEG2_ ".CRT$XCY"
#define _ENDSEG_ ".CRT$XTM"
//#pragma data_seg(".CRT$XIA")
#pragma data_seg(".CRT$XIM")
#pragma section(".CRT$XIM",long,read)
#pragma data_seg(".CRT$XCY")
#pragma section(".CRT$XCY",long,read)
//#pragma data_seg(".CRT$XIZ")
//#pragma data_seg(".CRT$YCZ")
#pragma data_seg(".CRT$XTM")
#pragma section(".CRT$XTM",long,read)
#pragma data_seg()
	                                       /*static __declspec(allocate(_STARTSEG_)) void (CPROC*pointer_##name)(void) = pastejunk(schedule_,name);*/
#define PRIORITY_PRELOAD(name,priority) static void CPROC name(void);    static int CPROC pastejunk(schedule_,name)(void);	   __declspec(allocate(_STARTSEG_)) int (CPROC*pastejunk(TARGET_LABEL,pastejunk( pastejunk(x_,name),__LINE__)))(void) = pastejunk(schedule_,name);	 int CPROC pastejunk(schedule_,name)(void) {	                 RegisterPriorityStartupProc( name,TOSTR(name),priority,pastejunk(TARGET_LABEL,pastejunk( pastejunk(x_,name),__LINE__)) DBG_SRC );	return 0;	 }	 static void CPROC name(void)
#define ROOT_ATEXIT(name) static void name(void);	 __declspec(allocate(_ENDSEG_)) static void (*f##name)(void)=name;    static void name(void)
#define ATEXIT(name) PRIORITY_ATEXIT(name,ATEXIT_PRIORITY_DEFAULT)
typedef void(*atexit_priority_proc)(void (*)(void),int,CTEXTSTR DBG_PASS);
#define PRIORITY_ATEXIT(name,priority) static void CPROC name(void);    static int schedule_atexit_##name(void);	   __declspec(allocate(_STARTSEG_)) void (CPROC*pastejunk(TARGET_LABEL,pastejunk( x_##name,__LINE__)))(void) = (void(CPROC*)(void))schedule_atexit_##name;	 static int schedule_atexit_##name(void) {	                 RegisterPriorityShutdownProc( name,TOSTR(name),priority,pastejunk(TARGET_LABEL,pastejunk( x_##name,__LINE__)) DBG_SRC );	return 0;	 }	                                       static void CPROC name(void)
#define ATEXIT_PRIORITY(name,priority) PRIORITY_ATEXIT(name,priority)
#endif
#ifdef __cplusplus_cli
#define InvokeDeadstart() do {	                                              TEXTCHAR myname[256];HMODULE mod;	 mod=LoadLibrary("sack_bag.dll");if(mod){           void(*rsp)(void);	 if((rsp=((void(*)(void))(GetProcAddress( mod, "RunDeadstart"))))){rsp();}else{lprintf( "Hey failed to get proc %d", GetLastError() );}	FreeLibrary( mod); }} while(0)
#else
#endif
#define PRELOAD(name) PRIORITY_PRELOAD(name,DEFAULT_PRELOAD_PRIORITY)
//extern uint32_t deadstart_complete;
//#define DEADSTART_LINK uint32_t *deadstart_link_couple = &deadstart_complete; // make sure we reference this symbol
//#pragma data_seg(".CRT$XCAA")
//extern void __cdecl __security_init_cookie(void);
//static _CRTALLOC(".CRT$XCAA") _PVFV init_cookie = __security_init_cookie;
//#pragma data_seg()
//------------------------------------------------------------------------------------
// UNDEFINED
//------------------------------------------------------------------------------------
#else
#error "there's nothing I can do to wrap PRELOAD() or ATEXIT()!"
/* This is the most basic way to define some startup code that
   runs at some point before the program starts. This code is
   declared as static, so the same preload initialization name
   can be used in multiple files.
   <link sack::app::deadstart, See Also.>                      */
#define PRELOAD(name)
#endif
// the higher the number the earlier it is run
#define ATEXIT_PRIORITY_SHAREMEM  1
#define ATEXIT_PRIORITY_THREAD_SEMS ATEXIT_PRIORITY_SYSLOG-1
#define ATEXIT_PRIORITY_SYSLOG    35
#define ATEXIT_PRIORITY_MSGCLIENT 85
#define ATEXIT_PRIORITY_DEFAULT   90
#define ATEXIT_PRIORITY_TIMERS   (ATEXIT_PRIORITY_DEFAULT+1)
// this is the first exit to be run.
// under linux it is __attribute__((destructor))
// under all it is registered during preload as atexit()
// only the runexits in deadstart should use ROOT_ATEXIT
#ifdef __WATCOMC__
#define ATEXIT_PRIORITY_ROOT 255
#else
#define ATEXIT_PRIORITY_ROOT 101
#endif
SACK_DEADSTART_NAMESPACE_END
USE_SACK_DEADSTART_NAMESPACE
#endif
#ifdef PROCREG_SOURCE
#define PROCREG_PROC(type,name) EXPORT_METHOD type CPROC name
#else
#define PROCREG_PROC(type,name) IMPORT_METHOD type CPROC name
#endif
#ifdef __cplusplus
#ifdef __cplusplus_cli
//using namespace System;
#endif
#   define _INTERFACE_NAMESPACE namespace Interface {
#   define _INTERFACE_NAMESPACE_END }
#define PROCREG_NAMESPACE namespace sack { namespace app { namespace registry {
#define _PROCREG_NAMESPACE namespace registry {
#define _APP_NAMESPACE namespace app {
#define PROCREG_NAMESPACE_END }}}
//extern "C"  {
#else
#   define _INTERFACE_NAMESPACE
#   define _INTERFACE_NAMESPACE_END
#define _PROCREG_NAMESPACE
#define _APP_NAMESPACE
#define PROCREG_NAMESPACE
#define PROCREG_NAMESPACE_END
#endif
SACK_NAMESPACE
/* Deadstart is support which differs per compiler, but allows
   applications access a C++ feature - static classes with
   constructors that initialize at loadtime, but, have the
   feature that you can create threads. Deadstart code is run
   after the DLL load lock under windows that prevents creation
   of threads; however, deadstart is run before main. Deadstart
   routines can have a priority. Certain features require others
   to be present always. This allows explicit control of
   priority unlink using classes with static constructors, which
   requires ordering of objects to provide linking order. Also
   provides a similar registration mechanism for atexit, but
   extending with priority. Deadstop registrations are done
   sometime during normal C atexit() handling, but may be
   triggered first by calling BAG_Exit.
   Registry offers support to register functions, and data under
   a hierarchy of names. Names are kept in a string cache, which
   applications can take benefit of. Strings will exist only a
   single time. This table could be saved, and a look-aside
   table for language translation purposes. Registry is the
   support that the latest PSI relies on for registering event
   callbacks for controls. The registry was always used, but,
   the access to it was encapsulated by DoRegisterControl
   registering the appropriate methods.                          */
	_APP_NAMESPACE
   /* Contains methods dealing with registering routines and values
      in memory. Provisions are available to save the configuration
      state, but the best that can be offered here would be a
      translation tool for text strings. The namespace is savable,
      but most of the content of the registration space are short
      term pointers. Namespace containing registry namespace.
      old notes - very discongruant probably should delete them.
      Process name registry
      it's a tree of names.
      there are paths, and entries
      paths are represented as class_name
      PCLASSROOT is also a suitable class name
      PCLASSROOT is defined as a valid CTEXTSTR.
      there is (apparently) a name that is not valid as a path name
      that is TREE
      guess.
      POINTER in these two are equal to (void(*)(void)) but -
      that's rarely the most useful thing... so
      name class is a tree of keys... /\<...\>
      psi/control/## might contain procs Init Destroy Move
      RegAlias( "psi/control/3", "psi/control/button"
      ); psi/control/button and psi/control/3 might reference the
      same routines
      psi/frame Init Destroy Move memlib Alloc Free
      network/tcp
      I guess name class trees are somewhat shallow at the moment
      not going beyond 1-3 layers
      names may eventually be registered and reference out of body
      services, even out of box...
      the values passed as returntype and parms/args need not be
      real genuine types, but do need to be consistant between the
      registrant and the requestor... this provides for full name
      dressing, return type and paramter type may both cause
      overridden functions to occur...                              */
_PROCREG_NAMESPACE
#ifndef REGISTRY_STRUCTURE_DEFINED
	// make these a CTEXTSTR to be compatible with name_class...
#ifdef __cplusplus
	// because of name mangling and stronger type casting
	// it becomes difficult to pass a tree_def_tag * as a CTEXTSTR classname
	// as valid as this is.
	typedef struct tree_def_tag const * PCLASSROOT;
#else
	typedef CTEXTSTR PCLASSROOT;
#endif
	typedef void (CPROC *PROCEDURE)(void);
#ifdef __cplusplus_cli
	typedef void (__stdcall *STDPROCEDURE)(array<System::Object^>^);
#endif
#else
	typedef struct tree_def_tag const * PCLASSROOT;
	typedef void (CPROC *PROCEDURE)(void);
#ifdef __cplusplus_cli
	typedef void (__stdcall *STDPROCEDURE)(array<System::Object^>^);
#endif
#endif
/* CheckClassRoot reads for a path of names, but does not create
   it if it does not exist.                                      */
PROCREG_PROC( PCLASSROOT, CheckClassRoot )( CTEXTSTR class_name );
/* \Returns a PCLASSROOT of a specified path. The path may be
   either a PCLASSROOT or a text string indicating the path. the
   Ex versions allow passing a base PCLASSROOT path and an
   additional subpath to get. GetClassRoot will always create
   the path if it did not exist before, and will always result
   with a root.
   Remarks
   a CTEXTSTR (plain text string, probably wide character if
   compiled unicode) and a PCLASSROOT are always
   interchangeable. Though you may need a forced type cast, I
   have defined both CTEXTSTR and PCLASSROOT function overloads
   for c++ compiled code, and C isn't so unkind about the
   conversion. I think problem might lie that CTEXTSTR has a
   const qualifier and PCLASSROOT doesn't (but should).
   Example
   <code lang="c++">
   PCLASSROOT root = GetClassRoot( "psi/resource" );
   // returns the root of all resource names.
   </code>
   <code>
   PCLASSROOT root2 = GetClassRootEx( "psi/resource", "buttons" );
   </code>                                                         */
PROCREG_PROC( PCLASSROOT, GetClassRoot )( CTEXTSTR class_name );
/* <combine sack::app::registry::GetClassRoot@CTEXTSTR>
   \ \                                                  */
PROCREG_PROC( PCLASSROOT, GetClassRootEx )( PCLASSROOT root, CTEXTSTR name_class );
#ifdef __cplusplus
/* <combine sack::app::registry::GetClassRoot@CTEXTSTR>
   \ \                                                  */
PROCREG_PROC( PCLASSROOT, GetClassRoot )( PCLASSROOT class_name );
/* <combine sack::app::registry::GetClassRoot@CTEXTSTR>
   \ \                                                  */
PROCREG_PROC( PCLASSROOT, GetClassRootEx )( PCLASSROOT root, PCLASSROOT name_class );
#endif
/* Fills a string with the path name to the specified node */
PROCREG_PROC( int, GetClassPath )( TEXTSTR out, size_t len, PCLASSROOT root );
PROCREG_PROC( void, SetInterfaceConfigFile )( TEXTCHAR *filename );
/* Get[First/Next]RegisteredName( "classname", &amp;data );
   these operations are not threadsafe and multiple thread
   accesses will cause mis-stepping
   These functions as passed the address of a POINTER. this
   POINTER is for the use of the browse routines and should is
   meaningless to he calling application.
   Parameters
   root :       The root to search from
   classname :  A sub\-path from the root to search from
   data :       the address of a pointer that keeps track of
                information about the search. (opaque to user)
   Example
   Usage:
   <code lang="c++">
   CTEXTSTR result;
   POINTER data = NULL;
   for( result = GetFirstRegisteredName( "some/class/path", &amp;data );
        \result;
        \result = GetNextRegisteredName( &amp;data ) )
   {
        // result is a string name of the current node.
        // can use that name and GetRegistered____ (function/int/value)
        if( NameHasBranches( &amp;data ) ) // for consitancy in syntax
        {
            // consider recursing through tree, name becomes a valid classname for GetFirstRegisteredName()
        }
   }
   </code>                                                                                                  */
PROCREG_PROC( CTEXTSTR, GetFirstRegisteredNameEx )( PCLASSROOT root, CTEXTSTR classname, PCLASSROOT *data );
#ifdef __cplusplus
/* <combine sack::app::registry::GetFirstRegisteredNameEx@PCLASSROOT@CTEXTSTR@PCLASSROOT *>
   \ \                                                                                      */
	PROCREG_PROC( CTEXTSTR, GetFirstRegisteredName )( PCLASSROOT classname, PCLASSROOT *data );
#endif
/* <combine sack::app::registry::GetFirstRegisteredNameEx@PCLASSROOT@CTEXTSTR@PCLASSROOT *>
   \ \                                                                                      */
PROCREG_PROC( CTEXTSTR, GetFirstRegisteredName )( CTEXTSTR classname, PCLASSROOT *data );
/* Steps to the next registered name being browsed. Is passed
   only the pointer to data. See GetFirstRegisteredName for
   usage.
   See Also
   <link sack::app::registry::GetFirstRegisteredNameEx@PCLASSROOT@CTEXTSTR@PCLASSROOT *, sack::app::registry::GetFirstRegisteredNameEx Function> */
PROCREG_PROC( CTEXTSTR, GetNextRegisteredName )( PCLASSROOT *data );
/* When using GetFirstRegisteredName and GetNextRegisteredName
   to browse through names, this function is able to get the
   current PCLASSROOT of the current node, usually you end up
   with just the content of that registered name.
   \result with the current node ( useful for pulling registered
   subvalues like description, or file and line )
                                                                 */
PROCREG_PROC( PCLASSROOT, GetCurrentRegisteredTree )( PCLASSROOT *data );
#ifdef __cplusplus
//PROCREG_PROC( CTEXTSTR, GetFirstRegisteredName )( CTEXTSTR classname, POINTER *data );
//PROCREG_PROC( CTEXTSTR, GetNextRegisteredName )( POINTER *data );
#endif
// while doing a scan for registered procedures, allow applications to check for branches
//PROCREG_PROC( int, NameHasBranches )( POINTER *data );
PROCREG_PROC( int, NameHasBranches )( PCLASSROOT *data );
// while doing a scan for registered procedures, allow applications to ignore aliases...
PROCREG_PROC( int, NameIsAlias )( PCLASSROOT *data );
/*
 * RegisterProcedureExx(
 *
 */
 // root name or PCLASSROOT of base path
PROCREG_PROC( int, RegisterProcedureExx )( PCLASSROOT root
 // an additional path on root
													  , CTEXTSTR name_class
 // the name of the value entry saved in the tree
													  , CTEXTSTR public_name
 // the text return type of this function - may be checked to validate during GetRegisteredProcedure
													  , CTEXTSTR returntype
 // name of the library this symbol is in - may be checked to validate during GetRegisteredProcedure
													  , CTEXTSTR library
 // actual C function name in library - may be checked to validate during GetRegisteredProcedure
													  , CTEXTSTR name
 // preferably the raw argument string of types and no variable references "([type][,type]...)"
													  , CTEXTSTR args
 // file and line of the calling application.  May be no parameter in release mode.
													  DBG_PASS
													  );
/*
 * RegisterProcedureEx( root       // root path
 *                    , name_class // additional name
 *                    , nice_name  // nice name
 *                    , return type not in quotes  'void'
 *                    , function_name in quotes '"Function"'
 *                    , args not in quotes '(int,char,float,UserType*)'
 */
#define RegisterProcedureEx(root,nc,n,rtype,proc,args)  RegisterProcedureExx( (root),(nc),(n),#rtype,TARGETNAME,(proc), #args DBG_SRC)
/*
 * RegisterProcedure( name_class // additional name
 *                    , nice_name  // nice name
 *                    , return type not in quotes  'void'
 *                    , function_name in quotes '"Function"'
 *                    , args not in quotes '(int,char,float,UserType*)'
 */
#define RegisterProcedure(nc,n,rtype,proc,args)  RegisterProcedureExx( NULL, (nc),(n),#rtype,TARGETNAME,(proc), #args DBG_SRC)
/*
 * Branches on the tree may be aliased together to form a single branch
 *
 */
				// RegisterClassAlias( "psi/control/button", "psi/control/3" );
				// then the same set of values can be referenced both ways with
				// really only a single modified value.
/* parameters to RegisterClassAliasEx are the original name, and the new alias name for the origianl branch*/
PROCREG_PROC( PCLASSROOT, RegisterClassAliasEx )( PCLASSROOT root, CTEXTSTR original, CTEXTSTR alias );
/* <combine sack::app::registry::RegisterClassAliasEx@PCLASSROOT@CTEXTSTR@CTEXTSTR>
   \ \                                                                              */
PROCREG_PROC( PCLASSROOT, RegisterClassAlias )( CTEXTSTR original, CTEXTSTR newalias );
// root, return, public, args, address
PROCREG_PROC( PROCEDURE, ReadRegisteredProcedureEx )( PCLASSROOT root
                                                    , CTEXTSTR returntype
																	 , CTEXTSTR parms
																  );
#define ReadRegisteredProcedure( root,rt,a) ((rt(CPROC*)a)ReadRegisteredProcedureEx(root,#rt,#a))
/* Gets a function that has been registered. */
PROCREG_PROC( PROCEDURE, GetRegisteredProcedureExxx )( PCLASSROOT root
																	 , PCLASSROOT name_class
                                                    , CTEXTSTR returntype
																	 , CTEXTSTR name
																	 , CTEXTSTR parms
																	 );
#define GetRegisteredProcedureExx(root,nc,rt,n,a) ((rt (CPROC*)a)GetRegisteredProcedureExxx(root,nc,#rt,n,#a))
#define GetRegisteredProcedure2(nc,rtype,name,args) (rtype (CPROC*)args)GetRegisteredProcedureEx((nc),#rtype, name, #args )
#define GetRegisteredProcedureNonCPROC(nc,rtype,name,args) (rtype (*)args)GetRegisteredProcedureEx((nc),#rtype, name, #args )
/* <combine sack::app::registry::GetRegisteredProcedureExxx@PCLASSROOT@PCLASSROOT@CTEXTSTR@CTEXTSTR@CTEXTSTR>
   \ \                                                                                                        */
PROCREG_PROC( PROCEDURE, GetRegisteredProcedureEx )( PCLASSROOT name_class
																	, CTEXTSTR returntype
																	, CTEXTSTR name
																	, CTEXTSTR parms
																	);
PROCREG_PROC( LOGICAL, RegisterFunctionExx )( PCLASSROOT root
													, PCLASSROOT name_class
													, CTEXTSTR public_name
													, CTEXTSTR returntype
													, PROCEDURE proc
													 , CTEXTSTR args
													 , CTEXTSTR library
													 , CTEXTSTR real_name
													  DBG_PASS
														  );
#ifdef __cplusplus
/* <combine sack::app::registry::GetRegisteredProcedureExxx@PCLASSROOT@PCLASSROOT@CTEXTSTR@CTEXTSTR@CTEXTSTR>
   \ \                                                                                                        */
PROCREG_PROC( PROCEDURE, GetRegisteredProcedureExxx )( CTEXTSTR root
																	 , CTEXTSTR name_class
                                                    , CTEXTSTR returntype
																	 , CTEXTSTR name
																	 , CTEXTSTR parms
																	 );
/* <combine sack::app::registry::GetRegisteredProcedureExxx@PCLASSROOT@PCLASSROOT@CTEXTSTR@CTEXTSTR@CTEXTSTR>
   \ \                                                                                                        */
PROCREG_PROC( PROCEDURE, GetRegisteredProcedureExxx )( CTEXTSTR root
																	 , PCLASSROOT name_class
                                                    , CTEXTSTR returntype
																	 , CTEXTSTR name
																	 , CTEXTSTR parms
																	 );
/* <combine sack::app::registry::GetRegisteredProcedureExxx@PCLASSROOT@PCLASSROOT@CTEXTSTR@CTEXTSTR@CTEXTSTR>
   \ \                                                                                                        */
PROCREG_PROC( PROCEDURE, GetRegisteredProcedureExxx )( PCLASSROOT root
																	 , CTEXTSTR name_class
                                                    , CTEXTSTR returntype
																	 , CTEXTSTR name
																	 , CTEXTSTR parms
																	 );
/* <combine sack::app::registry::GetRegisteredProcedureExxx@PCLASSROOT@PCLASSROOT@CTEXTSTR@CTEXTSTR@CTEXTSTR>
   \ \                                                                                                        */
PROCREG_PROC( PROCEDURE, GetRegisteredProcedureEx )( CTEXTSTR name_class
																	, CTEXTSTR returntype
																	, CTEXTSTR name
																	, CTEXTSTR parms
																	);
PROCREG_PROC( LOGICAL, RegisterFunctionExx )( CTEXTSTR root
													, CTEXTSTR name_class
													, CTEXTSTR public_name
													, CTEXTSTR returntype
                                       , PROCEDURE proc
													 , CTEXTSTR args
													 , CTEXTSTR library
													 , CTEXTSTR real_name
													  DBG_PASS
														  );
#endif
//#define RegisterFunctionExx( r,nc,p,rt,proc,ar ) RegisterFunctionExx( r,nc,p,rt,proc,ar,TARGETNAME,NULL DBG_SRC )
//#define RegisterFunctionEx(r,nc,pn,rt,proc,args,lib,rn) RegisterFunctionExx(r,nc,pn,rt,proc,args,lib,rn DBG_SRC)
#define RegisterFunctionEx( root,proc,rt,pn,a) RegisterFunctionExx( root,NULL,pn,rt,(PROCEDURE)(proc),a,NULL,NULL DBG_SRC )
#define RegisterFunction( nc,proc,rt,pn,a) RegisterFunctionExx( (PCLASSROOT)NULL,nc,pn,rt,(PROCEDURE)(proc),a,TARGETNAME,NULL DBG_SRC )
#define SimpleRegisterMethod(r,proc,rt,name,args) RegisterFunctionExx(r,NULL,name,rt,(PROCEDURE)proc,args,NULL,NULL DBG_SRC )
#define GetRegisteredProcedure(nc,rtype,name,args) (rtype (CPROC*)args)GetRegisteredProcedureEx((nc),#rtype, #name, #args )
PROCREG_PROC( int, RegisterIntValueEx )( PCLASSROOT root, CTEXTSTR name_class, CTEXTSTR name, uintptr_t value );
PROCREG_PROC( int, RegisterIntValue )( CTEXTSTR name_class, CTEXTSTR name, uintptr_t value );
PROCREG_PROC( int, RegisterValueExx )( PCLASSROOT root, CTEXTSTR name_class, CTEXTSTR name, int bIntVal, CTEXTSTR value );
PROCREG_PROC( int, RegisterValueEx )( CTEXTSTR name_class, CTEXTSTR name, int bIntVal, CTEXTSTR value );
PROCREG_PROC( int, RegisterValue )( CTEXTSTR name_class, CTEXTSTR name, CTEXTSTR value );
/* \ \
   Parameters
   root :        Root class to start searching from
   name_class :  An additional sub\-path to get the name from
   name :        the name within the path specified
   bIntVal :     a true/false whether to get the string or
                 integer value from the specified node.
   Returns
   A pointer to a string if bIntVal is not set. (NULL if there
   was no string).
   Otherwise will be an int shorter than or equal to the size of
   a pointer, which should be cast to an int if bIntVal is set,
   and there is a value registered there. Probably 0 if no
   value, so registered 0 value and no value is
   indistinguisable.                                             */
PROCREG_PROC( CTEXTSTR, GetRegisteredValueExx )( PCLASSROOT root, CTEXTSTR name_class, CTEXTSTR name, int bIntVal );
/* <combine sack::app::registry::GetRegisteredValueExx@PCLASSROOT@CTEXTSTR@CTEXTSTR@int>
   \ \                                                                                   */
PROCREG_PROC( CTEXTSTR, GetRegisteredValueEx )( CTEXTSTR name_class, CTEXTSTR name, int bIntVal );
/* <combine sack::app::registry::GetRegisteredValueExx@PCLASSROOT@CTEXTSTR@CTEXTSTR@int>
   \ \                                                                                   */
PROCREG_PROC( CTEXTSTR, GetRegisteredValue )( CTEXTSTR name_class, CTEXTSTR name );
#ifdef __cplusplus
/* <combine sack::app::registry::GetRegisteredValueExx@PCLASSROOT@CTEXTSTR@CTEXTSTR@int>
   \ \                                                                                   */
PROCREG_PROC( CTEXTSTR, GetRegisteredValueExx )( CTEXTSTR root, CTEXTSTR name_class, CTEXTSTR name, int bIntVal );
PROCREG_PROC( int, RegisterIntValueEx )( CTEXTSTR root, CTEXTSTR name_class, CTEXTSTR name, uintptr_t value );
#endif
/* This is like GetRegisteredValue, but takes the address of the
   type to return into instead of having to cast the final
   \result.
   if bIntValue, result should be passed as an (&amp;int)        */
PROCREG_PROC( int, GetRegisteredStaticValue )( PCLASSROOT root, CTEXTSTR name_class, CTEXTSTR name
															, CTEXTSTR *result
															, int bIntVal );
#define GetRegisteredStaticIntValue(r,nc,name,result) GetRegisteredStaticValue(r,nc,name,(CTEXTSTR*)result,TRUE )
/* <combine sack::app::registry::GetRegisteredValueExx@PCLASSROOT@CTEXTSTR@CTEXTSTR@int>
   \ \                                                                                   */
PROCREG_PROC( int, GetRegisteredIntValueEx )( PCLASSROOT root, CTEXTSTR name_class, CTEXTSTR name );
/* <combine sack::app::registry::GetRegisteredIntValueEx@PCLASSROOT@CTEXTSTR@CTEXTSTR>
   \ \                                                                                 */
PROCREG_PROC( int, GetRegisteredIntValue )( CTEXTSTR name_class, CTEXTSTR name );
#ifdef __cplusplus
/* <combine sack::app::registry::GetRegisteredIntValueEx@PCLASSROOT@CTEXTSTR@CTEXTSTR>
   \ \                                                                                 */
PROCREG_PROC( int, GetRegisteredIntValue )( PCLASSROOT name_class, CTEXTSTR name );
#endif
typedef void (CPROC*OpenCloseNotification)( POINTER, uintptr_t );
#define PUBLIC_DATA( public, struct, open, close )	    PRELOAD( Data_##open##_##close ) {	 RegisterDataType( "system/data/structs"	        , public, sizeof(struct)	    , (OpenCloseNotification)open, (OpenCloseNotification)close ); }
#define PUBLIC_DATA_EX( public, struct, open, update, close )	    PRELOAD( Data_##open##_##close ) {	 RegisterDataTypeEx( "system/data/structs"	        , public, sizeof(struct)	    , (OpenCloseNotification)open, (OpenCloseNotification)update, (OpenCloseNotification)close ); }
#define GET_PUBLIC_DATA( public, type, instname )    (type*)CreateRegisteredDataType( "system/data/structs", public, instname )
PROCREG_PROC( uintptr_t, RegisterDataType )( CTEXTSTR classname
												 , CTEXTSTR name
												 , uintptr_t size
												 , OpenCloseNotification open
												 , OpenCloseNotification close );
/* Registers a structure as creatable in shared memory by name.
   So a single name of the structure can be used to retrieve a
   pointer to one created.
   Example
   \ \
   <code lang="c++">
   POINTER p = CreateRegisteredDataType( "My types", "my_registered_type", "my instance" );
   // p will result to a region of type 'my_registered_type' called 'my_instance'
   // if it did not exist, it will be created, otherwise the one existing is returned.
   </code>
   Parameters
   root :          optional root name (ex version uses this)
   classname :     path to the type
   name :          name of the type to create an instance of
   instancename :  a name for the instance created.                                         */
PROCREG_PROC( uintptr_t, CreateRegisteredDataType)( CTEXTSTR classname
																 , CTEXTSTR name
																 , CTEXTSTR instancename );
PROCREG_PROC( uintptr_t, RegisterDataTypeEx )( PCLASSROOT root
													, CTEXTSTR classname
													, CTEXTSTR name
													, uintptr_t size
													, OpenCloseNotification Open
													, OpenCloseNotification Close );
/* <combine sack::app::registry::CreateRegisteredDataType@CTEXTSTR@CTEXTSTR@CTEXTSTR>
   \ \                                                                                */
PROCREG_PROC( uintptr_t, CreateRegisteredDataTypeEx)( PCLASSROOT root
																	, CTEXTSTR classname
																	, CTEXTSTR name
																	, CTEXTSTR instancename );
/* Outputs through syslog a tree dump of all names registered. */
PROCREG_PROC( void, DumpRegisteredNames )( void );
/* Dumps through syslog all names registered from the specified
   root point. (instead of dumping the whole tree)              */
PROCREG_PROC( void, DumpRegisteredNamesFrom )( PCLASSROOT root );
PROCREG_PROC( int, SaveTree )( void );
PROCREG_PROC( int, LoadTree )( void );
#define METHOD_PTR(type,name) type (CPROC *_##name)
#define DMETHOD_PTR(type,name) type (CPROC **_##name)
#define METHOD_ALIAS(i,name) ((i)->_##name)
#define PDMETHOD_ALIAS(i,name) (*(i)->_##name)
/* Releases an interface. When interfaces are registered, they
   register with a OnGetInterface and an OnDropInterface
   callback so that it may do additional work to cleanup from
   giving you a copy of the interface.
   Example
   <code lang="c++">
   POINTER p = GetInterface( "image" );
   DropInterface( p );
   </code>                                                     */
PROCREG_PROC( void, DropInterface )( CTEXTSTR pServiceName, POINTER interface_x );
PROCREG_PROC( POINTER, GetInterface_v4 )( CTEXTSTR pServiceName, LOGICAL ReadConfig, int quietFail DBG_PASS );
#define GetInterfaceV4( a, b )  GetInterface_v4( a, FALSE, b DBG_SRC )
/* \Returns the pointer to a registered interface. This is
   typically a structure that contains pointer to functions. Takes
   a text string to an interface. Interfaces are registered at a
   known location in the registry tree.                            */
PROCREG_PROC( POINTER, GetInterfaceDbg )( CTEXTSTR pServiceName DBG_PASS );
#define GetInterface(n) GetInterfaceDbg( n DBG_SRC )
#define GetRegisteredInterface(name) GetInterface(name)
PROCREG_PROC( LOGICAL, RegisterInterfaceEx )( CTEXTSTR name, POINTER(CPROC*load)(void), void(CPROC*unload)(POINTER) DBG_PASS );
//PROCREG_PROC( LOGICAL, RegisterInterface )(CTEXTSTR name, POINTER( CPROC*load )(void), void(CPROC*unload)(POINTER));
#define RegisterInterface(n,l,u) RegisterInterfaceEx( n,l,u DBG_SRC )
// unregister a function, should be smart and do full return type
// and parameters..... but for now this only references name, this indicates
// that this has not been properly(fully) extended, and should be layered
// in such a way as to allow this function work in it's minimal form.
PROCREG_PROC( int, ReleaseRegisteredFunctionEx )( PCLASSROOT root
													, CTEXTSTR name_class
													, CTEXTSTR public_name
													  );
#define ReleaseRegisteredFunction(nc,pn) ReleaseRegisteredFunctionEx(NULL,nc,pn)
/* This is a macro used to paste two symbols together. */
#define paste_(a,b) a##b
#define paste(a,b) paste_(a,b)
#define preproc_symbol(a)  a
#ifdef __cplusplus
#define EXTRA_PRELOAD_SYMBOL _
#else
#define EXTRA_PRELOAD_SYMBOL
#endif
#define DefineRegistryMethod2_i(task,name,classtype,methodname,desc,returntype,argtypes,line)	   CPROC paste(name,line)argtypes;	       PRIORITY_PRELOAD( paste(paste(paste(paste(Register,name),Method),preproc_symbol(EXTRA_PRELOAD_SYMBOL)),line), SQL_PRELOAD_PRIORITY ) {	  SimpleRegisterMethod( task "/" classtype, paste(name,line)	  , #returntype, methodname, #argtypes );    RegisterValue( task "/" classtype "/" methodname, "Description", desc ); }	                                                                          static returntype CPROC paste(name,line)
#define DefineRegistryMethod2(task,name,classtype,methodname,desc,returntype,argtypes,line)	   DefineRegistryMethod2_i(task,name,classtype,methodname,desc,returntype,argtypes,line)
/* Dekware uses this macro.
     passes preload priority override.
	 so it can register new internal commands before initial macros are run.
*/
#define DefineRegistryMethod2P_i(priority,task,name,classtype,methodname,desc,returntype,argtypes,line)	   CPROC paste(name,line)argtypes;	       PRIORITY_PRELOAD( paste(paste(paste(paste(Register,name),Method),preproc_symbol(EXTRA_PRELOAD_SYMBOL)),line), priority ) {	  SimpleRegisterMethod( task "/" classtype, paste(name,line)	  , #returntype, methodname, #argtypes );    RegisterValue( task "/" classtype "/" methodname, "Description", desc ); }	                                                                          static returntype CPROC paste(name,line)
/* This macro indirection is to resolve inner macros like "" around text.  */
#define DefineRegistryMethod2P(priority,task,name,classtype,methodname,desc,returntype,argtypes,line)	   DefineRegistryMethod2P_i(priority,task,name,classtype,methodname,desc,returntype,argtypes,line)
/*
    This method is used by PSI/Intershell.
	no description
*/
#define DefineRegistryMethod_i(task,name,classtype,classbase,methodname,returntype,argtypes,line)	   CPROC paste(name,line)argtypes;	       PRELOAD( paste(paste(Register##name##Button,preproc_symbol(EXTRA_PRELOAD_SYMBOL)),line) ) {	  SimpleRegisterMethod( task "/" classtype "/" classbase, paste(name,line)	  , #returntype, methodname, #argtypes ); }	                                                                          static returntype CPROC paste(name,line)
#define DefineRegistryMethod(task,name,classtype,classbase,methodname,returntype,argtypes,line)	   DefineRegistryMethod_i(task,name,classtype,classbase,methodname,returntype,argtypes,line)
/*
#define _0_DefineRegistryMethod(task,name,classtype,classbase,methodname,returntype,argtypes,line)	   static returntype _1__DefineRegistryMethod(task,name,classtype,classbase,methodname,returntype,argtypes,line)
#define DefineRegistryMethod(task,name,classtype,classbase,methodname,returntype,argtypes)	  _1__DefineRegistryMethod(task,name,classtype,classbase,methodname,returntype,argtypes,__LINE__)
*/
// this macro is used for ___DefineRegistryMethodP. Because this is used with complex names
// an extra define wrapper of priority_preload must be used to fully resolve paramters.
/*
#define DefineRegistryMethodP(priority,task,name,classtype,classbase,methodname,returntype,argtypes,line)	   CPROC paste(name,line)argtypes;	       PRIOR_PRELOAD( paste(paset(Register##name##Button,preproc_symbol(EXTRA_PRELOAD_SYMBOL),line), priority ) {	  SimpleRegisterMethod( task "/" classtype "/" classbase, paste(name,line)	  , #returntype, methodname, #argtypes ); }	                                                                          static returntype CPROC paste(name,line)
*/
/* <combine sack::app::registry::SimpleRegisterMethod>
   General form to build a registered procedure. Used by simple
   macros to create PRELOAD'ed registered functions. This flavor
   requires the user to provide 'static' and a return type that
   matches the return type specified in the macro. This makes
   usage most C-like, and convenient to know what the return
   value of a function should be (if any).
   Parameters
   priority :    The preload priority to load at.
   task :        process level name registry. This would be
                 "Intershell" or "psi" or some other base prefix.
                 The prefix can contain a path longer than 1
                 level.
   name :        This is the function name to build. (Can be used
                 for link debugging sometimes)
   classtype :   class of the name being registered
   methodname :  name of the routine to register
   returntype :  the literal type of the return type of this
                 function (void, int, PStruct* )
   argtypes :    Argument signature of the routine in parenthesis
   line :        this is usually filled with __LINE__ so that the
                 same function name (name) will be different in
                 different files (even in the same file)
   Remarks
   This registers a routine at the specified preload priority.
   Registers under [task]/[classname]/methodname. The name of
   the registered routine from a C perspective is [name][line]. This
   function is not called directly, but will only be referenced
   from the registered name.
   Example
   See <link sack::app::registry::GetFirstRegisteredNameEx@PCLASSROOT@CTEXTSTR@PCLASSROOT *, GetFirstRegisteredNameEx> */
/*
#define _1__DefineRegistryMethodP(priority,task,name,classtype,classbase,methodname,returntype,argtypes,line)	   _2___DefineRegistryMethodP(priority,task,name,classtype,classbase,methodname,returntype,argtypes,line)
#define _0_DefineRegistryMethodP(priority,task,name,classtype,classbase,methodname,returntype,argtypes,line)	   _1__DefineRegistryMethodP(priority,task,name,classtype,classbase,methodname,returntype,argtypes,line)
#define DefineRegistryMethodP(priority,task,name,classtype,classbase,methodname,returntype,argtypes)	  _0_DefineRegistryMethodP(priority,task,name,classtype,classbase,methodname,returntype,argtypes,__LINE__)
*/
#define DefineRegistrySubMethod_i(task,name,classtype,classbase,methodname,subname,returntype,argtypes,line)	   CPROC paste(name,line)argtypes;	       PRELOAD( paste(paste(Register##name##Button,preproc_symbol(EXTRA_PRELOAD_SYMBOL)),line) ) {	  SimpleRegisterMethod( task "/" classtype "/" classbase "/" methodname, paste(name,line)	  , #returntype, subname, #argtypes ); }	                                                                          static returntype CPROC paste(name,line)
#define DefineRegistrySubMethod(task,name,classtype,classbase,methodname,subname,returntype,argtypes)	  DefineRegistrySubMethod_i(task,name,classtype,classbase,methodname,subname,returntype,argtypes,__LINE__)
/* attempts to use dynamic linking functions to resolve passed
   global name if that fails, then a type is registered for this
   global, and an instance created, so that that instance may be
   reloaded again, otherwise the data in the main application is
   used... actually we should deprecate the dynamic loading
   part, and just register the type.
   SimpleRegisterAndCreateGlobal Simply registers the type as a
   global variable type. Allows creation of the global space
   later.
   Parameters
   name :         name of the pointer to global type to create.<p />text
                  string to register this created global as.
   ppGlobal :     address of the pointer to global memory.
   global_size :  size of the global area to create
   Example
   <code lang="c++">
   typedef struct {
      int data;
   } my_global;
   my_global *global;
   PRELOAD( Init )
   {
       SimpleRegisterAndCreateGlobal( global );
   }
   </code>                                                               */
PROCREG_PROC( void, RegisterAndCreateGlobal )( POINTER *ppGlobal, uintptr_t global_size, CTEXTSTR name );
/* <combine sack::app::registry::RegisterAndCreateGlobal@POINTER *@uintptr_t@CTEXTSTR>
   \ \                                                                                   */
#define SimpleRegisterAndCreateGlobal( name )	 RegisterAndCreateGlobal( (POINTER*)&name, sizeof( *name ), #name )
/* Init routine is called, otherwise a 0 filled space is
   returned. Init routine is passed the pointer to the global
   and the size of the global block the global data block is
   zero initialized.
   Parameters
   ppGlobal :     Address of the pointer to the global region
   global_size :  size of the global region to create
   name :         name of the global region to register (so
                  future users get back the same data area)
   Init :         function to call to initialize the region when
                  created. (doesn't have to be a global. Could be
                  used to implement types that have class
                  constructors \- or not, since there's only one
                  instance of a global \- this is more for
                  singletons).
   Example
   <code>
   typedef struct {
      int data;
   } my_global;
   my_global *global;
   </code>
   <code lang="c++">
   void __cdecl InitRegion( POINTER region, uintptr_t region_size )
   {
       // do something to initialize 'region'
   }
   PRELOAD( InitGlobal )
   {
       SimpleRegisterAndCreateGlobalWithInit( global, InitRegion );
   }
   </code>                                                          */
PROCREG_PROC( void, RegisterAndCreateGlobalWithInit )( POINTER *ppGlobal, uintptr_t global_size, CTEXTSTR name, void (CPROC*Init)(POINTER,uintptr_t) );
/* <combine sack::app::registry::RegisterAndCreateGlobalWithInit@POINTER *@uintptr_t@CTEXTSTR@void __cdecl*InitPOINTER\,uintptr_t>
   \ \                                                                                                                              */
#define SimpleRegisterAndCreateGlobalWithInit( name,init )	 RegisterAndCreateGlobalWithInit( (POINTER*)&name, sizeof( *name ), #name, init )
/* a tree dump will result with dictionary names that may translate automatically. */
/* This has been exported as a courtesy for StrDup.
 * this routine MAY result with a translated string.
 * this routine MAY result with the same pointer.
 * this routine MAY need to be improved if MANY more strdups are replaced
 * Add a binary tree search index when large.
 * Add a transaltion tree index at the same time.
 */
PROCREG_PROC( CTEXTSTR, SaveNameConcatN )( CTEXTSTR name1, ... );
// no space stripping, saves literal text
PROCREG_PROC( CTEXTSTR, SaveText )( CTEXTSTR text );
PROCREG_NAMESPACE_END
#ifdef __cplusplus
	using namespace sack::app::registry;
#endif
#endif
#endif
# ifndef SECOND_IMAGE_LEVEL
#  define SECOND_IMAGE_LEVEL _2
/* This is a macro used for building name changes for
   interfaces.                                        */
#  define PASTE(sym,name) name
# else
#  define PASTE2(sym,name) sym##name
#  define PASTE(sym,name) PASTE2(sym,name)
# endif
/* Macro to do symbol concatenation. */
#define _PASTE2(sym,name) sym##name
/* A second level paste macro so macro substitution is done on
   \parameters.                                                */
#define _PASTE(sym,name) _PASTE2(sym,name)
/* Define the default call type of image routines. CPROC is
   __cdecl.                                                 */
#define IMAGE_API CPROC
#     ifdef IMAGE_LIBRARY_SOURCE
#        define IMAGE_PROC  EXPORT_METHOD
// this sometimes needs an extra 'extern'
//#           ifdef IMAGE_MAIN
//#        define IMAGE_PROC_D EXPORT_METHOD
//#           else
//#        define IMAGE_PROC_D extern EXPORT_METHOD
//#           endif
#     else
/* Define the linkage type of the routine... probably
   __declspec(dllimport) if not building the library. */
#        define IMAGE_PROC IMPORT_METHOD
// this sometimes needs an extra 'extern'
//#        define IMAGE_PROC_D  IMPORT_METHOD
#     endif
#if defined( _WIN32 ) && !defined( _OPENGL_DRIVER ) && !defined( _D3D_DRIVER ) && !defined( _D3D10_DRIVER ) && !defined( _D3D11_DRIVER )
#define _INVERT_IMAGE
#endif
#ifdef __cplusplus
/* Define the namespace of image routines, when building under
   C++.                                                        */
#ifdef _D3D_DRIVER
#  define IMAGE_NAMESPACE namespace sack { namespace image { namespace d3d {
#  define _IMAGE_NAMESPACE namespace image { namespace d3d {
#  define BASE_IMAGE_NAMESPACE namespace image {
/* Define the namespace of image routines, when building under
   C++. This ends a namespace.                                 */
#  define IMAGE_NAMESPACE_END }}}
#elif defined( _D3D10_DRIVER )
#  define IMAGE_NAMESPACE namespace sack { namespace image { namespace d3d10 {
#  define _IMAGE_NAMESPACE namespace image { namespace d3d10 {
#  define BASE_IMAGE_NAMESPACE namespace image {
/* Define the namespace of image routines, when building under
   C++. This ends a namespace.                                 */
#  define IMAGE_NAMESPACE_END }}}
#elif defined( _D3D11_DRIVER )
#  define IMAGE_NAMESPACE namespace sack { namespace image { namespace d3d11 {
#  define _IMAGE_NAMESPACE namespace image { namespace d3d11 {
#  define BASE_IMAGE_NAMESPACE namespace image {
/* Define the namespace of image routines, when building under
   C++. This ends a namespace.                                 */
#  define IMAGE_NAMESPACE_END }}}
#else
#  define BASE_IMAGE_NAMESPACE namespace image {
#  define IMAGE_NAMESPACE namespace sack { namespace image {
#  define _IMAGE_NAMESPACE namespace image {
/* Define the namespace of image routines, when building under
   C++. This ends a namespace.                                 */
#  define IMAGE_NAMESPACE_END }}
#endif
/* Define the namespace of image routines, when building under
   C++. This ends the namespace. Assembly routines only have the
   ability to export C names, so extern"c" { } is used instead
   of namespace ___ { }.                                         */
#define ASM_IMAGE_NAMESPACE extern "C" {
/* Define the namespace of image routines, when building under
   C++. This ends the namespace. Assembly routines only have the
   ability to export C names, so extern"c" { } is used instead
   of namespace ___ { }.                                         */
#define ASM_IMAGE_NAMESPACE_END }
#else
#define BASE_IMAGE_NAMESPACE
#define IMAGE_NAMESPACE
#define _IMAGE_NAMESPACE
#define IMAGE_NAMESPACE_END
#define ASM_IMAGE_NAMESPACE
 /* Defined Image API.
   See Also
   <link sack::image::Image, Image>
   <link sack::image::SFTFont, SFTFont>
   <link Colors>
                                    */
#define ASM_IMAGE_NAMESPACE_END
#endif
#ifdef USE_API_ALIAS_PREFIX
#  define IMGVER__(a,b) a##b
#  define IMGVER_(a,b) IMGVER__(a,b)
#  define IMGVER(n)   IMGVER_(USE_API_ALIAS_PREFIX,n)
#else
#  define IMGVER(n)   n
#endif
SACK_NAMESPACE
/* Deals with images and image processing.
   Image is the primary type of this.
   SFTFont is a secondary type for putting text on images.
   render namespace is contained in image, because without
   image, there could be no render. see PRENDERER.         */
	_IMAGE_NAMESPACE
/* A fixed point decimal number (for freetype font rendering) */
typedef int64_t fixed;
//#ifndef IMAGE_STRUCTURE_DEFINED
//#define IMAGE_STRUCTURE_DEFINED
// consider minimal size - +/- 32000 should be enough for display purposes.
// print... well that's another matter.
   typedef int32_t IMAGE_COORDINATE;
   /* Represents the width and height of an image (unsigned values) */
   typedef uint32_t  IMAGE_SIZE_COORDINATE;
   /* An array of 2 IMAGE_COORDINATES - [0] = x, [1] = y */
   typedef IMAGE_COORDINATE IMAGE_POINT[2];
   /* An unsigned value coordinate pair to track the size of
      images.                                                */
   typedef IMAGE_SIZE_COORDINATE IMAGE_EXTENT[2];
   /* Pointer to an <link sack::image::IMAGE_POINT, IMAGE_POINT> */
   typedef IMAGE_COORDINATE *P_IMAGE_POINT;
   /* Pointer to a <link sack::image::IMAGE_EXTENT, IMAGE_EXTENT> */
   typedef IMAGE_SIZE_COORDINATE *P_IMAGE_EXTENT;
#ifdef HAVE_ANONYMOUS_STRUCTURES
typedef struct boundry_rectangle_tag
{
   union {
      IMAGE_POINT position;
      struct {
         IMAGE_COORDINATE x, y;
      };
   };
   union {
      IMAGE_EXTENT size;
      struct {
         IMAGE_SIZE_COORDINATE width, height;
      };
   };
} IMAGE_RECTANGLE, *P_IMAGE_RECTANGLE;
#else
/* Defines the coordinates of a rectangle. */
/* Pointer to an image rectangle.  */
typedef struct boundry_rectangle_tag
{
   /* anonymous union containing position information. */
   union {
      /* An anonymous structure containing x,y and width,height of a
         rectangle.                                                  */
      struct {
         /* the left coordinate of a rectangle. */
         /* the top coordinate of a rectangle */
         IMAGE_COORDINATE x, y;
         /* The Y span of the rectangle */
         /* the X Span of the rectangle */
         IMAGE_SIZE_COORDINATE width, height;
      };
      /* Anonymous structure containing position (x,y) and size
         (width,height).                                        */
      struct {
         /* The location of a rectangle (upper left x, y) */
         IMAGE_POINT position;
         /* the size of a rectangle (width and height) */
         IMAGE_EXTENT size;
      };
   };
} IMAGE_RECTANGLE, *P_IMAGE_RECTANGLE;
#endif
/* A macro for accessing vertical (Y) information of an <link sack::image::IMAGE_POINT, IMAGE_POINT>. */
#define IMAGE_POINT_H(ImagePoint) ((ImagePoint)[0])
/* A macro for accessing vertical (Y) information of an <link sack::image::IMAGE_POINT, IMAGE_POINT>. */
#define IMAGE_POINT_V(ImagePoint) ((ImagePoint)[1])
// the image at exactly this position and size
// is the one being referenced, the actual size and position
// may vary depending on use (a sub-image outside the
// boundry of its parent).
#define ImageData union {                                 struct {                                               IMAGE_COORDINATE x, y;                              IMAGE_SIZE_COORDINATE width, height;             };                                                  struct {                                               IMAGE_POINT position;                               IMAGE_EXTENT size;                               };                                               }
/* One of the two primary types that the image library works
   with.
   Example
   <code lang="c++">
   void LoadImage( char *name )
   {
       Image image = LoadImageFile( name );
       if( image )
       {
          // the image file loaded successfully.
       }
   }
   </code>                                                   */
typedef struct ImageFile_tag *Image;
typedef struct SlicedImageFile *SlicedImage;
#if defined( IMAGE_STRUCTURE_DEFINED )
#if defined(__cplusplus_cli ) && !defined( IMAGE_SOURCE )
IMAGE_PROC  PCDATA IMAGE_API ImageAddress( Image image, int32_t x, int32_t y );
#define IMG_ADDRESS(i,x,y) ImageAddress( i,x,y )
#endif
#endif
#if defined( __cplusplus )
IMAGE_NAMESPACE_END
#endif
#ifndef IMAGE_STRUCTURE_DEFINED
#ifdef _D3D_DRIVER
#include <d3d9.h>
#endif
#ifdef _D3D10_DRIVER
#include <D3D10_1.h>
#include <D3D10.h>
#endif
#ifdef _D3D11_DRIVER
#include <D3D11.h>
#endif
#ifdef _VULKAN_DRIVER
#  ifdef _WIN32
#    define VK_USE_PLATFORM_WIN32_KHR
#  else
#    define VK_USE_PLATFORM_XCB_KHR
#  endif
#  include <vulkan/vulkan.h>
#endif
// one day I'd like to make a multidimensional library
// but for now - 3D is sufficient - it can handle everything
// under 2D ignoring the Z axis... although it would be more
// efficient to do 2D implementation alone...
// but without function overloading the names of all the functions
// become much too complex.. well perhaps - maybe I can
// make all the required functions with a suffix - and
// supply defines to choose the default based on the dimension number
#ifndef ROTATE_DECLARATION
// vector multiple inclusion protection
#define ROTATE_DECLARATION
#if !defined(__STATIC__) && !defined(__LINUX__)
#  ifdef VECTOR_LIBRARY_SOURCE
#    define MATHLIB_EXPORT EXPORT_METHOD
#    if defined( __WATCOMC__ ) || defined( _MSC_VER )
// data requires an extra extern to generate the correct code *boggle*
#      define MATHLIB_DEXPORT extern EXPORT_METHOD
#    else
#      define MATHLIB_DEXPORT EXPORT_METHOD
#    endif
#  else
#    define MATHLIB_EXPORT IMPORT_METHOD
#    if ( defined( __WATCOMC__ ) || defined( _MSC_VER ) || defined( __GNUC__ ) ) && !defined( __ANDROID__ )
// data requires an extra extern to generate the correct code *boggle*
#      ifndef __cplusplus_cli
#        define MATHLIB_DEXPORT extern IMPORT_METHOD
#      else
#        define MATHLIB_DEXPORT extern IMPORT_METHOD
#      endif
#    else
#      define MATHLIB_DEXPORT IMPORT_METHOD
#    endif
#  endif
#else
#ifndef VECTOR_LIBRARY_SOURCE
#define MATHLIB_EXPORT extern
#define MATHLIB_DEXPORT extern
#else
#define MATHLIB_EXPORT
#define MATHLIB_DEXPORT
#endif
#endif
#define DIMENSIONS 3
#if( DIMENSIONS > 0 )
   #define vRight   0
   #define _1D(exp)  exp
   #if( DIMENSIONS > 1 )
      #define vUp      1
      #define _2D(exp)  exp
      #if( DIMENSIONS > 2 )
         #define vForward 2
         #define _3D(exp)  exp
         #if( DIMENSIONS > 3 )
  // 4th dimension 'IN'/'OUT' since projection is scaled 3d...
            #define vIn      3
            #define _4D(exp)  exp
         #else
            #define _4D(exp)
         #endif
      #else
         #define _3D(exp)
         #define _4D(exp)
      #endif
   #else
      #define _2D(exp)
      #define _3D(exp)
      #define _4D(exp)
   #endif
#else
   // print out a compiler message can't perform zero-D transformations...
#endif
#if defined( _D3D_DRIVER ) || defined( _D3D10_DRIVER )
#  ifndef MAKE_RCOORD_SINGLE
#    define MAKE_RCOORD_SINGLE
#  endif
#endif
#ifdef __cplusplus
#  ifndef MAKE_RCOORD_SINGLE
#    define VECTOR_NAMESPACE SACK_NAMESPACE namespace math { namespace vector { namespace Double {
#    define _MATH_VECTOR_NAMESPACE namespace math { namespace vector { namespace Double {
#    define _VECTOR_NAMESPACE namespace vector { namespace Double {
#    define USE_VECTOR_NAMESPACE using namespace sack::math::vector::Double;
#  else
#    define VECTOR_NAMESPACE SACK_NAMESPACE namespace math { namespace vector { namespace Float {
#    define _MATH_VECTOR_NAMESPACE namespace math { namespace vector { Float {
#    define _VECTOR_NAMESPACE namespace vector { namespace Float {
#    define USE_VECTOR_NAMESPACE using namespace sack::math::vector::Float;
#  endif
#  define _MATH_NAMESPACE namespace math {
#  define VECTOR_NAMESPACE_END } } } SACK_NAMESPACE_END
#else
#  define VECTOR_NAMESPACE
#  define _MATH_VECTOR_NAMESPACE
#  define _MATH_NAMESPACE
#  define _VECTOR_NAMESPACE
#  define VECTOR_NAMESPACE_END
#  define USE_VECTOR_NAMESPACE
#endif
#undef EXTERNAL_NAME
#undef VECTOR_METHOD
#ifdef MAKE_RCOORD_SINGLE
#  define VECTOR_METHOD(r,n,args) MATHLIB_EXPORT r n##f args
#  define EXTERNAL_NAME(n)  n##f
#else
#  define VECTOR_METHOD(r,n,args) MATHLIB_EXPORT r n##d args
#  define EXTERNAL_NAME(n)  n##d
#endif
#ifndef VECTOR_TYPES_DEFINED
#define VECTOR_TYPES_DEFINED
// this file merely defines the basic calculation unit...
// more types are defined in VECTLIB.H which has the number
// of dimensions defined...
#include <math.h>
#include <float.h>
SACK_NAMESPACE
	_MATH_NAMESPACE
   _VECTOR_NAMESPACE
   // requires C++ to make RCOORD single - otherwise it is a double.
#ifndef MAKE_RCOORD_SINGLE
/* Define that an RCOORD is represented by the basic type
   'double' for platforms with smaller fast floating point
   types, this could be float.                             */
#define RCOORD_IS_DOUBLE 1
	/* basic type that Vectlib is based on.
	 This specifies a 'real' (aka float) coordinate.
	 Combinations of coordinates create vectors and points.  */
typedef double RCOORD;
/* <combine sack::math::vector::double::RCOORD>
   \ \                                  */
	typedef double *PRCOORD;
#else
	/* basic type that Vectlib is based on.
	 This specifies a 'real' (aka float) coordinate.
	 Combinations of coordinates create vectors and points.  */
typedef float RCOORD;
/* <combine sack::math::vector::float::RCOORD>
   \ \                                  */
typedef float *PRCOORD;
#endif
// these SHOULD be dimension relative, but we lack much code for that...
typedef RCOORD MATRIX[4][4];
typedef MATRIX *PMatrix;
/* Describes the rotation matrix for a PTRANSFORM. */
typedef RCOORD PMATRIX[][4];
typedef RCOORD RQUATERNION[4];
typedef RCOORD PRQUATERNION[4];
#ifdef RCOORD_IS_DOUBLE
#define RCOORDBITS(v)  (*(uint64_t*)&(v))
#else
/* A macro to get the literal bits into an unsigned value of the
   same size. Shift and binary operators do not apply to
   floating point values, but floating point values are fields
   of bits that represent fractional parts of integers. This
   gets the bits so the fields can be tested.                    */
#define RCOORDBITS(v)  (*(uint32_t*)&(v))
#endif
/* a symbol which is effectively the largest negative value of
   the space, anything less than this is untrackable, and is the
   same as infinity.                                             */
#define NEG_INFINITY ((RCOORD)-9999999999.0)
/* a symbol which is effectively the largest value of the space,
   anything more than this is untrackable, and is the same as
   infinity.                                                     */
#define POS_INFINITY ((RCOORD)9999999999.0)
/* This is 'epsilon' that is used with by NearZero comparison
   macro.                                                     */
#define e1 (0.00001)
/* Checks to see if a coordinate is 0 or near 0. */
#define NearZero( n ) (fabs(n)<e1)
#ifndef __cplusplus
#endif
#ifdef RCOORD_IS_DOUBLE
// THRESHOLD may be any number more than 1.
// eveything 0 or more makes pretty much every number
// which is anything like another number equal...
// threshold is in count of bits...
// therefore for 1millionth digit accuracy that would be 20 bits.
// 10 bits is thousanths different are near
// 0 bits is if the same magnitude of number... but may be
//   quite different
// -10 bits is thousands different are near
// 1 == 1.5
// 1 == 1.01
#define THRESHOLD 16
#ifdef _MSC_VER
#define EXPON(f) ((int)(( RCOORDBITS(f) & 0x4000000000000000I64 ) ?                       (( RCOORDBITS(f) &  0x3FF0000000000000I64 ) >> (20+32)) :                        ((( RCOORDBITS(f) & 0x3FF0000000000000I64 ) >> (20+32)) - 1024)))
#else
#define EXPON(f) ((int)(( RCOORDBITS(f) & 0x4000000000000000LL ) ?                       (( RCOORDBITS(f) &  0x3FF0000000000000LL ) >> (20+32)) :                        ((( RCOORDBITS(f) & 0x3FF0000000000000LL ) >> (20+32)) - 1024)))
#endif
#else
// THRESHOLD may be any number more than 1.
// eveything 0 or more makes pretty much every number
// which is anything like another number equal...
// threshold is in count of bits...
// therefore for 1millionth digit accuracy that would be 20 bits.
// 10 bits is thousanths different are near
// 0 bits is if the same magnitude of number... but may be
//   quite different
// -10 bits is thousands different are near
// 1 == 1.5
// 1 == 1.01
#define THRESHOLD 10
/* Macro to extract the exponent part of a RCOORD.
                                                   */
#define EXPON(f) ((int)(( RCOORDBITS(f) & 0x40000000L ) ?                       (( RCOORDBITS(f) & 0x3F800000L ) >> 23) :                        ((( RCOORDBITS(f) & 0x3F800000L ) >> 23) - 128)))
#endif
#ifdef NEED_VECTLIB_COMPARE
//cpg26Dec2006 c:\work\sack\include\vectypes.h(75): Warning! W202: Symbol 'COMPARE' has been defined, but not referenced
static int COMPARE( RCOORD n1, RCOORD n2 )
{
	RCOORD tmp1, tmp2;
	int compare_result;
	tmp1=n1-n2;
	//lprintf( "exponents %ld %ld", EXPON( n1 ), EXPON( n2 ) );
	 //lprintf("%9.9g-%9.9g=%9.9g %s %s %ld %ld %ld"
	//		 , (n1),(n2),(tmp1)
	//		 ,!RCOORDBITS(n1)?"zero":"    ",!RCOORDBITS(n2)?"zero":"    "
	  //	 ,EXPON(n1)-THRESHOLD
	//	    ,EXPON(n2)-THRESHOLD
	//		 ,EXPON(tmp1) );
	tmp2=n2-n1;
	//lprintf("%9.9g-%9.9g=%9.9g %s %s %ld %ld %ld"
	//		 , (n2),(n1),(tmp2)
	//		 ,!RCOORDBITS(n2)?"zero":"    ",!RCOORDBITS(n1)?"zero":"    "
	//		 ,EXPON(n2)-THRESHOLD,EXPON(n1)-THRESHOLD,EXPON(tmp2));
	compare_result = ( ( !RCOORDBITS(n1) )?( (n2) <  0.0000001 &&
														 (n2) > -0.0000001 )?1:0
							:( !RCOORDBITS(n2) )?( (n1) <  0.0000001 &&
														 (n1) > -0.0000001 )?1:0
							:( (n1) == (n2) )?1
							:( ( EXPON(n1) - THRESHOLD ) >=
							  ( EXPON( tmp1 ) ) ) &&
							( ( EXPON(n2) - THRESHOLD ) >=
							 ( EXPON( tmp2) ) ) ? 1 : 0
						  );
	return compare_result;
}
/*
static RCOORD CompareTemp1, CompareTemp2;
#define COMPARE( n1, n2 ) ( RCOORDBITS(n1)                                            ? ( CompareTemp1 = (n1)+(n1),                                 CompareTemp2 = (n1)+(n2),                   (RCOORDBITS(CompareTemp1)&0xFFFFFFF0)==                                     (RCOORDBITS(CompareTemp2)&0xFFFFFFF0) )                                         : RCOORDBITS(n2)                                              ? ( CompareTemp1 = (n2)+(n1),                                   CompareTemp2 = (n2)+(n2),                                  (RCOORDBITS(CompareTemp1)&0xFFFFFFF0)==                                   (RCOORDBITS(CompareTemp2)&0xFFFFFFF0) )                                                  : 1 )
*/
/*
                           ( ( ( RCOORDBITS(n1) & 0x80000000 ) !=                                      ( RCOORDBITS(n2) & 0x80000000 ) )                                       ? ( NearZero(n1)                                                           && ( ( RCOORDBITS(n1)&0x7FFFFFF0 ) ==                                        ( RCOORDBITS(n2)&0x7FFFFFF0 ) ) )                               : ( ( RCOORDBITS(n1)&0xFFFFFFF0 ) ==                                        ( RCOORDBITS(n2)&0xFFFFFFF0 ) ) )
*/
#if 1
#else
inline int COMPARE( RCOORD n1, RCOORD n2 )
{
	RCOORD CompareTemp1, CompareTemp2;
   return  RCOORDBITS(n1)                                            ? ( CompareTemp1 = (n1)+(n1),                                 CompareTemp2 = (n1)+(n2),                   (RCOORDBITS(CompareTemp1)&0xFFFFFFF0)==                                     (RCOORDBITS(CompareTemp2)&0xFFFFFFF0) )                                         : RCOORDBITS(n2)                                              ? ( CompareTemp1 = (n2)+(n1),                                   CompareTemp2 = (n2)+(n2),                                  (RCOORDBITS(CompareTemp1)&0xFFFFFFF0)==                                   (RCOORDBITS(CompareTemp2)&0xFFFFFFF0) )                                                  : 1;
}
#endif
#endif
VECTOR_NAMESPACE_END
#endif
// $Log: vectypes.h,v $
// Revision 1.12  2005/01/27 08:21:39  panther
// Linux cleaned.
//
// Revision 1.11  2004/02/08 05:42:29  d3x0r
// associate comparetemp1, 2 with routine which needs it.
//
// Revision 1.10  2003/11/28 00:10:39  panther
// fix compare function...
//
// Revision 1.9  2003/11/23 08:42:41  panther
// Toying with the nearness floating point operator
//
// Revision 1.8  2003/09/01 20:04:37  panther
// Added OpenGL Interface to windows video lib, Modified RCOORD comparison
//
// Revision 1.7  2003/08/29 10:26:17  panther
// Checkpoint - converted vectlib to be native double
//
// Revision 1.6  2003/08/29 02:07:41  panther
// Fixed logging, and nearness comparison
//
// Revision 1.5  2003/08/27 07:56:40  panther
// Replace COMPARE macro with one that works a little better
//
// Revision 1.4  2003/03/25 08:38:11  panther
// Add logging
//
SACK_NAMESPACE
	_MATH_NAMESPACE
	/* Vector namespace contains methods for operating on vectors. Vectors
	   are multi-dimensional scalar quantities, often used to
	   represent coordinates and directions in space.                      */
   _VECTOR_NAMESPACE
//#include "../src/vectlib/vecstruc.h"
typedef RCOORD _POINT4[4];
typedef RCOORD _POINT[DIMENSIONS];
/* pointer to a point. */
typedef RCOORD *P_POINT;
/* pointer to a constant point. */
typedef const RCOORD *PC_POINT;
/* A vector type. Contains 3 values by default, library can
   handle 4 dimensional transformations(?)                  */
typedef _POINT VECTOR;
/* pointer to a vector. */
typedef P_POINT PVECTOR;
/* pointer to a constant vector. */
typedef PC_POINT PCVECTOR;
/* <combine sack::math::vector::RAY@1>
   \ \                                 */
typedef struct vectlib_ray_type *PRAY;
/* <combine sack::math::vector::RAY@1>
   \ \                                 */
typedef struct vectlib_ray_type RAY;
/* A ray is a type that has an origin and a direction. (It is a
   pair of vectors actually)                                    */
struct vectlib_ray_type {
 // origin
   _POINT o;
 // normal
   _POINT n;
};
/* <combinewith sack::math::vector::lineseg_tag>
   \ \                                           */
typedef struct lineseg_tag  LINESEG;
/* <combine sack::math::vector::lineseg_tag>
   \ \                                       */
typedef struct lineseg_tag *PLINESEG;
/* This is a pure abstraction of a Line. It is used in the basis
   of 3d graphics.                                               */
struct lineseg_tag {
   /* a ray type that is the origin and slope of the line. */
	RAY r;
	/* scalar along direction vector that indicates where the line
	   segment ends. (origin + (direction * dTo) ) = end           */
	/* scalar along direction vector that indicates where the line
	   segment ends. (origin + (direction * dTo) ) = start         */
	RCOORD dFrom, dTo;
};
/* <combine sack::math::vector::orthoarea_tag>
   \ \                                         */
typedef struct orthoarea_tag ORTHOAREA;
/* <combine sack::math::vector::orthoarea_tag>
   \ \                                         */
typedef struct orthoarea_tag *PORTHOAREA;
/* A representation of a rectangular 2 dimensional area. */
struct orthoarea_tag {
    /* x coorindate of a rectangular area. */
    /* y coordinate of a rectangular area. */
    RCOORD x, y;
    /* height (y + h = area end). height may be negative. */
    /* with (x + w = area end). with may be negative. */
    RCOORD w, h;
} ;
// relics from fixed point math dayz....
#define ZERO (0.0f)
/* Special symbol that is the unit quantity. */
#define ONE  (1.0f)
#ifndef M_PI
/* symbol to define a double precision value for PI if it
   doesn't exist in the compiler.                         */
#ifdef MAKE_RCOORD_SINGLE
#  define M_PI (3.1415926535f)
#else
#  define M_PI (3.1415926535)
#endif
#endif
/* a hard coded define that represents a 5 degree angle in
   radians.                                                */
#define _5  (RCOORD)((5.0/180.0)*M_PI )
/* a hard coded define that represents a 15 degree angle in
   radians.                                                 */
#define _15 (RCOORD)((15.0/180.0)*M_PI )
/* a hard coded define that represents a 30 degree angle in
   radians.                                                 */
#define _30 (RCOORD)((30.0/180.0)*M_PI )
/* a hard coded define that represents a 45 degree angle in
   radians.                                                 */
#define _45 (RCOORD)((45.0/180.0)*M_PI )
#define SetPoint( d, s ) ( (d)[0] = (s)[0], (d)[1]=(s)[1], (d)[2]=(s)[2] )
#define SetPoint4( d, s ) ( (d)[0] = (s)[0], (d)[1]=(s)[1], (d)[2]=(s)[2], (d)[3]=(s)[3] )
/* Inverts a vector. that is vector * -1. (a,b,c) = (-a,-b,-c)
   <b>Parameters</b>
                                                               */
VECTOR_METHOD( P_POINT, Invert, ( P_POINT a ) );
/* Macro which can be used to make a vector's direction be
   exactly opposite of what it is now.                     */
#define InvertVector( a ) ( a[0] = -a[0], a[1]=-a[1], a[2]=-a[2] )
/* Logs the vector and leader to log. the leader is called
   lpName, cause it was inteded to be used by just the vector
   name.
   Parameters
   lpName :  text leader to print before the vector.
   v :       vector to log
   Example
   <code lang="c++">
   PrintVector( _X );
   // expands to
   PrintVectorEx( "_X", _X DBG_SRC );
   </code>                                                    */
VECTOR_METHOD( void, PrintVectorEx, ( CTEXTSTR lpName, PCVECTOR v DBG_PASS ) );
/* <combine sack::math::vector::PrintVectorEx@CTEXTSTR@PCVECTOR v>
   \ \                                                               */
#define PrintVector(v) PrintVectorEx( #v, v DBG_SRC )
/* Same as PrintVectorEx, but prints to standard output using
   printf.                                                    */
VECTOR_METHOD( void, PrintVectorStdEx, ( CTEXTSTR lpName, VECTOR v DBG_PASS ) );
/* <combine sack::math::vector::PrintVectorStdEx@CTEXTSTR@VECTOR v>
   \ \                                                                */
#define PrintVectorStd(v) PrintVectorStd( #v, v DBG_SRC )
/* Dumps to syslog a current matrix. Shows both matrix content,
   and the cross products between the matrix that cross1 should
   be row 0, cross2 should be row 1 and cross3 should be row2.
   Pass a text name to identify this matrix from others.
   Parameters
   lpName :    Name to write into the log.
   m :         the matrix to dump.
   DBG_PASS :  standard debug paramters
   Remarks
   A PTRANSFORM is not a MATRIX; there is a matrix in a
   transform, and is the first member, so a ptransform can be
   cast to a matrix and logged with this function.              */
VECTOR_METHOD( void, PrintMatrixEx, ( CTEXTSTR lpName, MATRIX m DBG_PASS ) );
/* <combine sack::math::vector::PrintMatrixEx@CTEXTSTR@MATRIX m>
   \ \                                                             */
#define PrintMatrix(m) PrintMatrixEx( #m, m DBG_SRC )
/* <combine sack::math::vector::TransformationMatrix>
   \ \                                                */
typedef struct transform_tag *PTRANSFORM;
/* <combine sack::math::vector::TransformationMatrix>
   \ \                                                */
typedef struct transform_tag	 TRANSFORM;
/* Pointer to a constant transform. */
typedef const TRANSFORM *PCTRANSFORM;
/* Constant pointer to a constant transform. For things like _I
   transformation which is the identity translation.            */
typedef const PCTRANSFORM *CPCTRANSFORM;
#define VectorConst_0 EXTERNAL_NAME(VectorConst_0)
#define VectorConst_X EXTERNAL_NAME(VectorConst_X)
#define VectorConst_Y EXTERNAL_NAME(VectorConst_Y)
#define VectorConst_Z EXTERNAL_NAME(VectorConst_Z)
#define VectorConst_I EXTERNAL_NAME(VectorConst_I)
#ifdef __cplusplus
#define VECTLIBCONST
#else
#define VECTLIBCONST const
#endif
//------ Constants for origin(0,0,0), and axii
#if !defined( VECTOR_LIBRARY_SOURCE ) || defined( VECTOR_LIBRARY_IS_EXTERNAL )
MATHLIB_DEXPORT VECTLIBCONST PC_POINT VectorConst_0;
/* Specifies the coordinate system's X axis direction. static
   constant.                                                  */
MATHLIB_DEXPORT VECTLIBCONST PC_POINT VectorConst_X;
/* Specifies the coordinate system's Y axis direction. static
   constant.                                                  */
MATHLIB_DEXPORT VECTLIBCONST PC_POINT VectorConst_Y;
/* Specifies the coordinate system's Z axis direction. static
   constant.                                                  */
MATHLIB_DEXPORT VECTLIBCONST PC_POINT VectorConst_Z;
/* This is a static constant identity matrix, which can be used
   to initialize a matrix transform (internally).               */
MATHLIB_DEXPORT VECTLIBCONST PCTRANSFORM VectorConst_I;
#define _0 ((PC_POINT)VectorConst_0)
#  ifndef _X
#    define _X ((PC_POINT)VectorConst_X)
#  else
#  endif
#define _Y ((PC_POINT)VectorConst_Y)
#define _Z ((PC_POINT)VectorConst_Z)
#define _I ((PC_POINT)VectorConst_I)
#endif
/* compares two vectors to see if they are near each other. Boundary
   \conditions exist around 0, when the values are on opposite
   sides, but it's pretty good.                                      */
#define Near( a, b ) ( COMPARE(a[0],b[0]) && COMPARE( a[1], b[1] ) && COMPARE( a[2], b[2] ) )
/* Add two vectors together. (a1,b1,c1) + (a2,b2,c2) =
   (a1+a2,b1+b2,c1+c2)
   Remarks
   The result vector may be the same as one of the source
   vectors.                                               */
VECTOR_METHOD( P_POINT, add, ( P_POINT pr, PC_POINT pv1, PC_POINT pv2 ) );
/* subtracts two vectors and stores the result in another
   vector.
   Remarks
   The result vector may be the same as one of the source
   vectors.                                               */
VECTOR_METHOD( P_POINT, sub, ( P_POINT pr, PC_POINT pv1, PC_POINT pv2 ) );
/* Scales a vector by a scalar
   Parameters
   pr :   \result vector
   pv1 :  vector to scale
   k :    constant scalar to apply to vector
   Example
   <code lang="c#">
   VECTOR result;
   VECTOR start;
   SetPoint( start, _X );
   scale( result, start, 3 );
   </code>                                   */
VECTOR_METHOD( P_POINT, scale, ( P_POINT pr, PC_POINT pv1, RCOORD k ) );
/* Adds a vector scaled by a scalar to another vector, results
   in a third vector.
   Parameters
   pr :   pointer to a result vector
   pv1 :  pointer to vector 1
   pv2 :  pointer to vector 2
   k :    scalar quantity to apply to vector 2 when adding to
          vector 1.
   Remarks
   The pointer to the result vector may be the same vector as
   vector 1 or vector 2.
   Example
   <code lang="c++">
   _POINT result;
   P_POINT v1 = _X;
   P_POINT v2 = _Y;
   RCOORD k = 1.414;
   addscaled( result, v1, v2, k );
   // result is ( 1, 1.414, 0 )
   </code>                                                     */
VECTOR_METHOD( P_POINT, addscaled, ( P_POINT pr, PC_POINT pv1, PC_POINT pv2, RCOORD k ) );
/* Normalizes a non-zero vector. That is the resulting length of
   the vector is 1.0. Modifies the vector in place.              */
VECTOR_METHOD( void, normalize, ( P_POINT pv ) );
VECTOR_METHOD( void, crossproduct, ( P_POINT pr, PC_POINT pv1, PC_POINT pv2 ) );
/* \Returns the sin of the angle between two vectors.
   Parameters
   pv1 :  one vector
   pv2 :  another vector
   Remarks
   If the length of either vector is 0, then a divide by zero
   exception will happen.                                     */
VECTOR_METHOD( RCOORD, SinAngle, ( PC_POINT pv1, PC_POINT pv2 ) );
/* \Returns the cos (cosine) of the angle between two vectors.
   Parameters
   pv1 :  one vector
   pv2 :  another vector
   Remarks
   If the length of either vector is 0, then a divide by zero
   exception will happen.                                      */
VECTOR_METHOD( RCOORD, CosAngle, ( PC_POINT pv1, PC_POINT pv2 ) );
VECTOR_METHOD( RCOORD, dotproduct, ( PC_POINT pv1, PC_POINT pv2 ) );
// result is the projection of project onto onto
VECTOR_METHOD( P_POINT, project, ( P_POINT pr, PC_POINT onto, PC_POINT project ) );
/* \Returns the scalar length of a vector. */
VECTOR_METHOD( RCOORD, Length, ( PC_POINT pv ) );
/* \Returns the distance between two points.
   Parameters
   v1 :  some point
   v2 :  another point
   Returns
   The distance between the two points.      */
VECTOR_METHOD( RCOORD, Distance, ( PC_POINT v1, PC_POINT v2 ) );
/* \Returns the distance a point is as projected on another
   vector. The result is the distance along that vector from the
   origin.
   Parameters
   pvOn :  Vector to project on
   pvOf :  vector to get projection length of.                   */
VECTOR_METHOD( RCOORD, DirectedDistance, ( PC_POINT pvOn, PC_POINT pvOf ) );
/* copies the value of a ray into another ray
   Parameters
   ray to set :   target value
   ray to copy :  value to copy.
   Example
   <code>
   RAY ray;
   RAY ray2;
   // set ray to ray2
   SetRay( ray, ray2 );
   </code>                                    */
#define SetRay( pr1, pr2 ) { SetPoint( (pr1)->o, (pr2)->o ),                               SetPoint( (pr1)->n, (pr2)->n ); }
		/* Allocates and initializes a new transform for the user.
		 if name is NULL, allocates an unnamed transform; otherwise
       the transform is created in a known namespace that can be browsed.
		 */
VECTOR_METHOD( PTRANSFORM, CreateNamedTransform, ( CTEXTSTR name ) );
#define CreateTransform() CreateNamedTransform( NULL )
VECTOR_METHOD( PTRANSFORM, CreateTransformMotion, ( PTRANSFORM pt ) );
VECTOR_METHOD( PTRANSFORM, CreateTransformMotionEx, ( PTRANSFORM pt, int rocket ) );
VECTOR_METHOD( void, DestroyTransform     , ( PTRANSFORM pt ) );
/* Resets a transform back to initial conitions. */
VECTOR_METHOD( void, ClearTransform       , ( PTRANSFORM pt ) );
/* Badly named function.
   InvertTransform turns a transform sideways, that is takes
   axis-normal transforms and turns them for sending to other
   graphic systems.
   <code lang="c++">
     \+-         -+
     | 0   1   2 |
     | 3   4   5 |
     | 6   7   8 |
     \+-         -+
   becomes
     \+-         -+
     | 0   3   6 |
     | 1   4   7 |
     | 2   5   8 |
     \+-         -+
   Not entirely useful at all :)
   </code>                                                    */
VECTOR_METHOD( void, InvertTransform        , ( PTRANSFORM pt ) );
VECTOR_METHOD( void, Scale                 , ( PTRANSFORM pt, RCOORD sx, RCOORD sy, RCOORD sz ) );
VECTOR_METHOD( void, Translate             , ( PTRANSFORM pt, RCOORD tx, RCOORD ty, RCOORD tz ) );
VECTOR_METHOD( void, TranslateV            , ( PTRANSFORM pt, PC_POINT t ) );
VECTOR_METHOD( void, TranslateRel          , ( PTRANSFORM pt, RCOORD tx, RCOORD ty, RCOORD tz ) );
VECTOR_METHOD( void, TranslateRelV         , ( PTRANSFORM pt, PC_POINT t ) );
VECTOR_METHOD( void, RotateAbs, ( PTRANSFORM pt, RCOORD rx, RCOORD ry, RCOORD rz ) );
VECTOR_METHOD( void, RotateAbsV, ( PTRANSFORM pt, PC_POINT ) );
/* Updates the current rotation matrix of a transform by a
   relative amount. Amounts to rotate about the x, y and z axii
   are given in radians.
   Parameters
   pt :  transform to rotate
   rx :  amount around the x axis to rotate (pitch)(positive is
         clockwise looking at the object from the right, axis up is
         moved towards forward )
   ry :  amount around the y axis to rotate (yaw) (positive is
         counter clockwise, moves right to forward)
   rz :  amount around the z axis to rotate (roll) (positive is
         clockwise, moves up towards right )
   See Also
   RotateRelV                                                       */
VECTOR_METHOD( void, RotateRel, ( PTRANSFORM pt, RCOORD rx, RCOORD ry, RCOORD rz ) );
/* Update a transformation matrix by relative degress about the
   x axix, y axis and z axis.
   Parameters
   pt :  transform to update
   v :   vector containing x,y and z relative roll coordinate.  */
VECTOR_METHOD( void, RotateRelV, ( PTRANSFORM pt, PC_POINT ) );
/* Rotates a transform around some arbitrary axis. (any line may
   be used to rotate the transformation's rotation matrix)
   Parameters
   pt :      transform to update
   p :       P defines an axis around which the rotation portion
             of the matrix is rotated by an amount. Can be any
             arbitrary axis.
   amount :  an amount to rotate by.
   Note
   coded from
   http://www.mines.edu/~gmurray/ArbitraryAxisRotation/ArbitraryAxisRotation.html
   and
   http://www.siggraph.org/education/materials/HyperGraph/modeling/mod_tran/3drota.htm
   and http://astronomy.swin.edu.au/~pbourke/geometry/rotate/.
                                                                                       */
VECTOR_METHOD( void, RotateAround, ( PTRANSFORM pt, PC_POINT p, RCOORD amount ) );
/* Sets the current 'up' axis of a transformation. The forward
   axis is adjusted so that it remains perpendicular to the mast
   axis vs the right axis. After the forward axis is updated,
   the right axis is adjusted to be perpendicular to up and
   forward.
   Parameters
   pt :   transform to set the up direction of
   vup :  new direction for 'up'
   Remarks
   RotateMast is based on the idea that your current frame is
   something like a boat. As the boat moves along a surface,
   it's version of 'up' may change depending on the landscape. This
   keeps up up. (Actually, the computation was used for an
   object running along the interior of a sphere, and this
   normalizes their 'up' to the center of the sphere.               */
VECTOR_METHOD( void, RotateMast, ( PTRANSFORM pt, PCVECTOR vup ) );
/* Rotates around the 'up' of the current rotation matrix. Same
   as a yaw rotation.
   Parameters
   pt :     transformation to rotate
   angle :  angle to rotate \- positive should be clockwise,
            looking from top down.                              */
VECTOR_METHOD( void, RotateAroundMast, ( PTRANSFORM pt, RCOORD angle ) );
/* Recovers a transformation state from a file.
   Parameters
   pt :        transform to read into
   filename :  filename with the transform in it. */
VECTOR_METHOD( void, LoadTransform, ( PTRANSFORM pt, CTEXTSTR filename ) );
/* Provides a way to save a matrix ( direct binary file dump)
   Parameters
   pt :        transform matrix to save
   filename :  \file to save the transformation in.           */
VECTOR_METHOD( void, SaveTransform, ( PTRANSFORM pt, CTEXTSTR filename ) );
VECTOR_METHOD( void, RotateTo, ( PTRANSFORM pt, PCVECTOR vforward, PCVECTOR vright ) );
VECTOR_METHOD( void, RotateRight, ( PTRANSFORM pt, int A1, int A2 ) );
VECTOR_METHOD( void, Apply           , ( PCTRANSFORM pt, P_POINT dest, PC_POINT src ) );
VECTOR_METHOD( void, ApplyR          , ( PCTRANSFORM pt, PRAY dest, PRAY src ) );
VECTOR_METHOD( void, ApplyT          , ( PCTRANSFORM pt, PTRANSFORM dest, PCTRANSFORM src ) );
// I know this was a result - unsure how it was implented...
//void ApplyT              (PTRANFORM pt, PTRANSFORM pt1, PTRANSFORM pt2 );
VECTOR_METHOD( void, ApplyInverse    , ( PCTRANSFORM pt, P_POINT dest, PC_POINT src ) );
VECTOR_METHOD( void, ApplyInverseR   , ( PCTRANSFORM pt, PRAY dest, PRAY src ) );
VECTOR_METHOD( void, ApplyInverseT   , ( PCTRANSFORM pt, PTRANSFORM dest, PCTRANSFORM src ) );
// again note there was a void ApplyInverseT
VECTOR_METHOD( void, ApplyRotation        , ( PCTRANSFORM pt, P_POINT dest, PC_POINT src ) );
VECTOR_METHOD( void, ApplyRotationT       , ( PCTRANSFORM pt, PTRANSFORM ptd, PCTRANSFORM pts ) );
VECTOR_METHOD( void, ApplyInverseRotation , ( PCTRANSFORM pt, P_POINT dest, PC_POINT src ) );
VECTOR_METHOD( void, ApplyInverseRotationT, ( PCTRANSFORM pt, PTRANSFORM ptd, PCTRANSFORM pts ) );
VECTOR_METHOD( void, ApplyTranslation     , ( PCTRANSFORM pt, P_POINT dest, PC_POINT src ) );
VECTOR_METHOD( void, ApplyTranslationT    , ( PCTRANSFORM pt, PTRANSFORM ptd, PCTRANSFORM pts ) );
VECTOR_METHOD( void, ApplyInverseTranslation, ( PCTRANSFORM pt, P_POINT dest, PC_POINT src ) );
VECTOR_METHOD( void, ApplyInverseTranslationT, ( PCTRANSFORM pt, PTRANSFORM ptd, PCTRANSFORM pts ) );
// after Move() these callbacks are invoked.
typedef void (*MotionCallback)( uintptr_t, PTRANSFORM );
/* When Move is called on the transform, these callbacks are
   invoked so user code can get even update for motion.
   Parameters
   pt :        PTRANSFORM transform matrix to hook to
   callback :  user callback routine
   psv :       pointer size value data to be passed to user
               callback routine.                             */
VECTOR_METHOD( void, AddTransformCallback, ( PTRANSFORM pt, MotionCallback callback, uintptr_t psv ) );
/* Set the speed vector used when Move is applied to a
   PTRANSFORM.
   Parameters
   pt :  transform to set the current speed of.
   s :   the speed vector to set.                      */
VECTOR_METHOD( PC_POINT, SetSpeed, ( PTRANSFORM pt, PC_POINT s ) );
VECTOR_METHOD( P_POINT, GetSpeed, ( PTRANSFORM pt, P_POINT s ) );
/* Sets the acceleration applied to the speed when Move is
   called.                                                 */
VECTOR_METHOD( PC_POINT, SetAccel, ( PTRANSFORM pt, PC_POINT s ) );
VECTOR_METHOD( P_POINT, GetAccel, ( PTRANSFORM pt, P_POINT s ) );
/* Sets the forward direction speed in a PTRANSFORM.
   Parameters
   pt :        Transform to set the right speed of.
   distance :  How far it should go in the next time interval. */
VECTOR_METHOD( void, Forward, ( PTRANSFORM pt, RCOORD distance ) );
VECTOR_METHOD( void, MoveForward, ( PTRANSFORM pt, RCOORD distance ) );
VECTOR_METHOD( void, MoveRight, ( PTRANSFORM pt, RCOORD distance ) );
VECTOR_METHOD( void, MoveUp, ( PTRANSFORM pt, RCOORD distance ) );
/* Sets the up direction speed in a PTRANSFORM.
   Parameters
   pt :        Transform to set the right speed of.
   distance :  How far it should go in the next time interval. */
VECTOR_METHOD( void, Up, ( PTRANSFORM pt, RCOORD distance ) );
/* Sets the right direction speed in a PTRANSFORM.
   Parameters
   pt :        Transform to set the right speed of.
   distance :  How far it should go in the next time interval. */
VECTOR_METHOD( void, Right, ( PTRANSFORM pt, RCOORD distance ) );
VECTOR_METHOD( PC_POINT, SetRotation, ( PTRANSFORM pt, PC_POINT r ) );
VECTOR_METHOD( P_POINT, GetRotation, ( PTRANSFORM pt, P_POINT r ) );
VECTOR_METHOD( PC_POINT, SetRotationAccel, ( PTRANSFORM pt, PC_POINT r ) );
/* Set how long it takes, in milliseconds, to move 1 unit of
   speed vector or rotate 1 unit of rotation vector when Move is
   called. Each matrix maintains a last tick. If many thousands
   of matrixes were used, probably a batch move could be
   implemented that would maintain tick counts for a group of
   matrixes... don't know how long it takes to compute move, but
   timeGetTime will slow it down a lot.
   Parameters
   pt :                 transform to set the time interval on.
   speed_interval :     what the time interval should be for
                        speed.
   rotation_interval :  what the time interval should be for
                        rotation.
   Remarks
   A default interval of 1000 is used. So it will take 1000
   milliseconds to move one unit of speed. This could be set to
   3600000 and then it would take one hour to move one unit of
   speed. (miles per hour)
   Rotation has its own interval that affects rotation the same
   way; If your rotation was set to roll 2*pi radians, then it
   would revolve one full rotation in the said time.
                                                                 */
VECTOR_METHOD( void, SetTimeInterval, ( PTRANSFORM pt, RCOORD speed_interval, RCOORD rotation_interval ) );
/* Updates a transform by it's current speed and rotation
   assuming speed and rotation are specified in x per 1 second.
   Applies the fraction of time between now and the prior time
   move was called and scales speed and rotation by that when
   applying it.
   Parameters
   pt :  Pointer to a transform to update.
   See Also
   <link sack::math::vector::SetTimeInterval@PTRANSFORM@RCOORD@RCOORD, SetTimeInterval> */
VECTOR_METHOD( LOGICAL, Move, ( PTRANSFORM pt ) );
#if 0
	VECTOR_METHOD( void, Unmove, ( PTRANSFORM pt ) );
#endif
VECTOR_METHOD( void, showstdEx, ( PTRANSFORM pt, char *header ) );
VECTOR_METHOD( void, ShowTransformEx, ( PTRANSFORM pt, char *header DBG_PASS ) );
/* <combine sack::math::vector::ShowTransformEx@PTRANSFORM@char *header>
   \ \                                                                   */
#define ShowTransform( n ) ShowTransformEx( n, #n DBG_SRC )
VECTOR_METHOD( void, showstd, ( PTRANSFORM pt, char *header ) );
VECTOR_METHOD( void, GetOriginV, ( PTRANSFORM pt, P_POINT o ) );
VECTOR_METHOD( PC_POINT, GetOrigin, ( PTRANSFORM pt ) );
VECTOR_METHOD( void, GetAxisV, ( PTRANSFORM pt, P_POINT a, int n ) );
VECTOR_METHOD( PC_POINT, GetAxis, ( PTRANSFORM pt, int n ) );
VECTOR_METHOD( void, SetAxis, ( PTRANSFORM pt, RCOORD a, RCOORD b, RCOORD c, int n ) );
VECTOR_METHOD( void, SetAxisV, ( PTRANSFORM pt, PC_POINT a, int n ) );
// matrix is suitable to set as the first matrix for opengl rendering.
// it is an inverse application that uses the transform's origin as camera origin
// and the rotation matrix as what to look at.
VECTOR_METHOD( void, GetGLCameraMatrix, ( PTRANSFORM pt, PMATRIX out ) );
VECTOR_METHOD( void, GetGLMatrix, ( PTRANSFORM pt, PMATRIX out ) );
VECTOR_METHOD( void, SetGLMatrix, ( PMATRIX in, PTRANSFORM pt ) );
VECTOR_METHOD( void, SetRotationMatrix, ( PTRANSFORM pt, RCOORD *quat ) );
VECTOR_METHOD( void, GetRotationMatrix, ( PTRANSFORM pt, RCOORD *quat ) );
  // line m, b
VECTOR_METHOD( RCOORD, IntersectLineWithPlane, (PCVECTOR Slope, PCVECTOR Origin,
  // plane n, o
	PCVECTOR n, PCVECTOR o,
	RCOORD *time) );
VECTOR_METHOD( RCOORD, PointToPlaneT, (PCVECTOR n, PCVECTOR o, PCVECTOR p) );
#if ( !defined( VECTOR_LIBRARY_SOURCE ) && !defined( NO_AUTO_VECTLIB_NAMES ) ) || defined( NEED_VECTLIB_ALIASES )
#define add EXTERNAL_NAME(add)
#define sub EXTERNAL_NAME(sub)
#define scale EXTERNAL_NAME(scale)
#define Scale EXTERNAL_NAME(Scale)
#define Invert EXTERNAL_NAME(Invert)
#define GetOrigin EXTERNAL_NAME(GetOrigin)
#define GetOriginV EXTERNAL_NAME(GetOriginV)
#define GetAxis EXTERNAL_NAME(GetAxis)
#define GetAxisV EXTERNAL_NAME(GetAxisV)
#define GetGLCameraMatrix EXTERNAL_NAME(GetGLCameraMatrix)
#define ApplyInverse EXTERNAL_NAME(ApplyInverse)
#define Move EXTERNAL_NAME(Move)
#define MoveForward EXTERNAL_NAME(MoveForward)
#define MoveRight EXTERNAL_NAME(MoveRight)
#define MoveUp EXTERNAL_NAME(MoveUp)
#define Forward EXTERNAL_NAME(Forward)
#define Right EXTERNAL_NAME(Right)
#define Up EXTERNAL_NAME(Up)
#define PrintVectorEx EXTERNAL_NAME(PrintVectorEx)
#define PrintMatrixEx EXTERNAL_NAME(PrintMatrixEx)
#define ShowTransformEx EXTERNAL_NAME(ShowTransformEx)
#define addscaled EXTERNAL_NAME(addscaled)
#define Length EXTERNAL_NAME(Length)
#define PointToPlaneT EXTERNAL_NAME(PointToPlaneT)
#define normalize EXTERNAL_NAME(normalize)
#define Translate EXTERNAL_NAME(Translate)
#define TranslateV EXTERNAL_NAME(TranslateV)
#define Apply EXTERNAL_NAME(Apply)
#define ApplyR EXTERNAL_NAME(ApplyR)
#define ApplyT EXTERNAL_NAME(ApplyT)
#define ApplyTranslation EXTERNAL_NAME(ApplyTranslation)
#define ApplyTranslationR EXTERNAL_NAME(ApplyTranslationR)
#define ApplyTranslationT EXTERNAL_NAME(ApplyTranslationT)
#define ApplyInverseRotation EXTERNAL_NAME(ApplyInverseRotation)
#define ApplyInverseR EXTERNAL_NAME(ApplyInverseR)
#define ApplyRotation EXTERNAL_NAME(ApplyRotation)
#define ApplyRotationT EXTERNAL_NAME(ApplyRotationT)
#define RotateAround EXTERNAL_NAME(RotateAround)
#define RotateRel EXTERNAL_NAME(RotateRel)
#define SetRotation EXTERNAL_NAME(SetRotation)
#define GetRotation EXTERNAL_NAME(GetRotation)
#define SetRotationAccel EXTERNAL_NAME(SetRotationAccel)
#define RotateRight EXTERNAL_NAME(RotateRight)
#define dotproduct EXTERNAL_NAME(dotproduct)
#define DestroyTransform EXTERNAL_NAME(DestroyTransform)
#define SinAngle EXTERNAL_NAME(SinAngle)
#define CosAngle EXTERNAL_NAME(CosAngle)
#define crossproduct EXTERNAL_NAME(crossproduct)
#define CreateTransformMotion EXTERNAL_NAME(CreateTransformMotion)
#define CreateTransformMotionEx EXTERNAL_NAME(CreateTransformMotionEx)
#define CreateNamedTransform EXTERNAL_NAME(CreateNamedTransform)
#define ClearTransform EXTERNAL_NAME(ClearTransform)
#define RotateTo EXTERNAL_NAME(RotateTo)
#define SetSpeed EXTERNAL_NAME(SetSpeed)
#define SetAccel EXTERNAL_NAME(SetAccel)
#define TranslateRel EXTERNAL_NAME( TranslateRel )
#define TranslateRelV EXTERNAL_NAME( TranslateRelV )
#define RotateAbs EXTERNAL_NAME(RotateAbs)
#define RotateAbsV EXTERNAL_NAME(RotateAbsV)
#define GetRotationMatrix EXTERNAL_NAME(GetRotationMatrix)
#define SetRotationMatrix EXTERNAL_NAME(SetRotationMatrix)
#endif
#ifdef __cplusplus
VECTOR_NAMESPACE_END
USE_VECTOR_NAMESPACE
#endif
#endif
// $Log: vectlib.h,v $
// Revision 1.13  2004/08/22 09:56:41  d3x0r
// checkpoint...
//
// Revision 1.12  2004/02/02 22:43:35  d3x0r
// Add lineseg type and orthoarea (min/max box)
//
// Revision 1.11  2004/01/11 23:24:15  panther
// Fix type warnings, conflicts, fix const issues
//
// Revision 1.10  2004/01/11 23:11:49  panther
// Fix const typings
//
// Revision 1.9  2004/01/11 23:10:38  panther
// Include keyboard to avoid windows errors
//
// Revision 1.8  2004/01/04 20:54:18  panther
// Use PCTRANSFORM for prototypes
//
// Revision 1.7  2003/12/29 08:10:18  panther
// Added more functions for applying transforms
//
// Revision 1.6  2003/11/22 23:27:11  panther
// Fix type passed to printvector
//
// Revision 1.5  2003/09/01 20:04:37  panther
// Added OpenGL Interface to windows video lib, Modified RCOORD comparison
//
// Revision 1.4  2003/03/25 08:38:11  panther
// Add logging
//
#if defined( _WIN32 ) && !defined( _INVERT_IMAGE ) && !defined( _OPENGL_DRIVER ) && !defined( _D3D_DRIVER )
#define _INVERT_IMAGE
#endif
#define WILL_DEFINE_IMAGE_STRUCTURE
#define IMAGE_STRUCTURE_DEFINED
IMAGE_NAMESPACE
#ifdef __cplusplus
	namespace Interface
{
	struct image_interface_tag;
}
#ifdef _D3D_DRIVER
	using namespace sack::image::d3d::Interface;
#elif defined( _D3D10_DRIVER )
	using namespace sack::image::d3d10::Interface;
#elif defined( _D3D11_DRIVER )
	using namespace sack::image::d3d11::Interface;
#else
	using namespace sack::image::Interface;
#endif
#endif
#ifndef PSPRITE_METHOD
#define PSPRITE_METHOD PSPRITE_METHOD
/* pointer to a structure defining a sprite draw method this should be defined in render namespace...*/
	typedef struct sprite_method_tag *PSPRITE_METHOD;
#endif
	/* Flags which may be combined in <link sack::image::ImageFile_tag::flags, Image.flags> */
	enum ImageFlags {
 // this has been freed - but contains sub images
IF_FLAG_FREE   =0x00001,
 // moved beyond parent image's bound
IF_FLAG_HIDDEN =0x00002,
 // built with a *image from external sources
IF_FLAG_EXTERN_COLORS =0x00004,
 // pay attention to (clips) array.
IF_FLAG_HAS_CLIPS     =0x00008,
// with no _X_STRING flag - characters are shown as literal character glyph.
 // strings on this use 'c' processing
IF_FLAG_C_STRING       = 0x00010,
 // strings on this use menu processing ( &underline )
IF_FLAG_MENU_STRING    = 0x00020,
 // strings use control chars (newline, tab)
IF_FLAG_CONTROL_STRING = 0x00040,
 // this has been freed - but contains sub images
IF_FLAG_OWN_DATA       = 0x00080,
  // image is inverted (standard under windows, but this allows images to be configured dynamically - a hack to match SDL lameness )
IF_FLAG_INVERTED       = 0x00100,
// DisplayLib uses this flag - indicates panel root
 // please #define user flag to this
IF_FLAG_USER1          = 0x10000,
// DisplayLib uses this flag - indicates is part of a displayed panel
IF_FLAG_USER2          = 0x20000,
IF_FLAG_USER3          = 0x40000,
 // output should render to opengl target (with transform); also used with proxy
IF_FLAG_FINAL_RENDER   = 0x00200,
 // set when a operation has changed the surface of a local image; requires update to remote device(s)
IF_FLAG_UPDATED        = 0x00400,
 // set when a operation has changed the surface of a local image; requires update to remote device(s)
IF_FLAG_HAS_PUTSTRING  = 0x00800,
 // is an in-memory image; that is the surface can be written to directly with pixel ops (putstring)
IF_FLAG_IN_MEMORY      = 0x01000,
	};
//#define _DRAWPOINT_X 0
//#define _DRAWPOINT_Y 1
struct ImageFile_tag
{
#if defined( IMAGE_LIBRARY_SOURCE ) || defined( NEED_REAL_IMAGE_STRUCTURE )
	int real_x;
	int real_y;
   // desired height and width may not be actual cause of
	int real_width;
  // resizing of parent image....
	int real_height;
# ifdef HAVE_ANONYMOUS_STRUCTURES
	IMAGE_RECTANGLE;
# else
 // need this for sub images - otherwise is irrelavent
	int x;
	int y;
  /// Width of image.
	int width;
 /// Height of image.
	int height;
# endif
#else
	/* X coordinate of the image within another image. */
	int x;
	/* Y coordinate of an image within another image. */
	int y;
   // desired height and width may not be actual cause of
	int width;
  // resizing of parent image....
	int height;
 // need this for sub images - otherwise is irrelavent
	int actual_x;
	int actual_y;
  // Width of image.
	int actual_width;
 // Height of image.
	int actual_height;
#endif
 // width of real physical layer
	int pwidth;
	// The image data.
	PCOLOR image;
	/* a combination of <link ImageFlags, IF_FLAG_> (ImageFile Flag)
	   which apply to this image.                                    */
	int flags;
	/* This points to a peer image that existed before this one. If
	   NULL, there is no elder, otherwise, contains the next peer
	   image in the same parent image.                              */
	/* Points to the parent image of a sub-image. (The parent image
	   contains this image)                                         */
	/* Pointer to the youngest child sub-image. If there are no sub
	   images pChild will be NULL. Otherwise, pchild points at the
	   first of one or more sub images. Other sub images in this one
	   are found by following the pElder link of the pChild.         */
	/* This points at a more recently created sub-image. (another
	   sub image within the same parent, but younger)             */
	struct ImageFile_tag *pParent, *pChild, *pElder, *pYounger;
	   // effective x - clipped by reality real coordinate.
	           // (often eff_x = -real_x )
	int eff_x;
	/* this is used internally for knowing what the effective y of
	   the image is. If the sub-image spans a boundry of a parent
	   image, then the effective Y that will be worked with is only
	   a part of the subimage.                                      */
	int eff_y;
		// effective max - maximum coordinate...
	int eff_maxx;
		// effective maximum Y
	int eff_maxy;
		/* An extra rectangle that can be used to carry additional
		 information like update region.                         */
	IMAGE_RECTANGLE auxrect;
	// fonts need a way to output the font character subimages to the real image...
	// or for 3D; to reverse scale appropriately
	struct image_interface_tag  *reverse_interface;
 // what the interface thinks this is...
	POINTER reverse_interface_instance;
	void (*extra_close)( struct ImageFile_tag *);
//DOM-IGNORE-BEGIN
#if defined( __3D__ )
	PTRANSFORM transform;
#endif
#ifdef _OPENGL_DRIVER
	/* gl context? */
	LOGICAL depthTest;
	PLIST glSurface;
 // most things will still use this, since reload image is called first, reload will set active
	int glActiveSurface;
  // updated with SetTransformRelation, otherwise defaults to image size.
	VECTOR coords[4];
#endif
#ifdef _D3D10_DRIVER
	PLIST Surfaces;
	ID3D10Texture2D *pActiveSurface;
#endif
#ifdef _D3D11_DRIVER
	PLIST Surfaces;
	ID3D11Texture2D *pActiveSurface;
#endif
#ifdef _D3D_DRIVER
	/* gl context? */
	PLIST Surfaces;
	IDirect3DBaseTexture9 *pActiveSurface;
#endif
#ifdef _VULKAN_DRIVER
	LOGICAL depthTest;
	PLIST vkSurface;
 // most things will still use this, since reload image is called first, reload will set active
	int vkActiveSurface;
  // updated with SetTransformRelation, otherwise defaults to image size.
	VECTOR coords[4];
	VkCommandBuffer* commandBuffers;
#endif
#ifdef __cplusplus
 // watcom limits protections in structs to protected and public
#ifndef __WATCOMC__
private:
#endif
#endif
//DOM-IGNORE-END
};
enum SlicedImageSection {
	SLICED_IMAGE_TOP_LEFT,
	SLICED_IMAGE_TOP,
	SLICED_IMAGE_TOP_RIGHT,
	SLICED_IMAGE_LEFT,
	SLICED_IMAGE_CENTER,
	SLICED_IMAGE_RIGHT,
	SLICED_IMAGE_BOTTOM_LEFT,
	SLICED_IMAGE_BOTTOM,
	SLICED_IMAGE_BOTTOM_RIGHT,
};
struct SlicedImageFile {
	struct ImageFile_tag *image;
	struct ImageFile_tag *slices[9];
	uint32_t left, right, top, bottom;
	uint32_t center_w, center_h;
	uint32_t right_w;
	uint32_t bottom_h;
	LOGICAL output_center;
	LOGICAL extended_slice;
};
/* The basic structure. This is referenced by applications as '<link sack::image::Image, Image>'
	This is the primary type that the image library works with.
	This is the internal definition.
	This is a actual data content, Image is (ImageFile *).                                        */
typedef struct ImageFile_tag ImageFile;
/* A simple wrapper to add dynamic changing position and
	orientation to an image. Sprites can be output at any angle. */
struct sprite_tag
{
	/* Current location of the sprite's origin. */
	/* Current location of the sprite's origin. */
  // current x and current y for placement on image.
	int32_t curx, cury;
  // int of bitmap hotspot... centers cur on hot
	int32_t hotx, hoty;
	Image image;
	// curx,y are kept for moving the sprite independantly
	fixed scalex, scaley;
	// radians from 0 -> 2*pi.  there is no negative...
 // radians for now... (used internally, set by blot rotated sprite)
	float angle;
	// should consider keeping the angle of rotation
	// and also should cosider keeping velocity/acceleration
	// but then limits would have to be kept also... so perhaps
	// the game module should keep such silly factors... but then couldn't
	// it also keep curx, cury ?  though hotx hoty is the actual
	// origin to rotate this image about, and to draw ON curx 0 cury 0
	// int orgx, orgy;  // rotated origin of bitmap.
	// after being drawn the min(x,y) and max(x,y) are set.
 // after draw, these are the extent of the sprite.
	int32_t minx, maxx;
 // after draw, these are the extent of the sprite.
	int32_t miny, maxy;
	PSPRITE_METHOD pSpriteMethod;
};
/* A Sprite type. Adds position and rotation and motion factors
	to an image. Hooks into the render system to get an update to
	draw on a temporary layer after the base rendering is done.   */
typedef struct sprite_tag SPRITE;
#ifdef _INVERT_IMAGE
// inversion does not account for eff_y - only eff_maxy
// eff maxy - eff_minY???
/*+((i)->eff_y)*/
#define INVERTY(i,y)     ( (((i)->eff_maxy) - (y)))
#else
/* This is a macro is used when image data is inverted on a
	platform. (Windows images, the first row of data is the
	bottom of the image, all Image operations are specified from
	the top-left as 0,0)                                         */
#define INVERTY(i,y)     ((y) - (i)->eff_y)
#endif
/*+((i)->eff_y)*/
#define INVERTY_INVERTED(i,y)     ( (((i)->eff_maxy) - (y)))
#define INVERTY_NON_INVERTED(i,y)     ((y) - (i)->eff_y)
#if defined(__cplusplus_cli ) && !defined( IMAGE_SOURCE )
//IMAGE_PROC( PCDATA, ImageAddress )( Image image, int32_t x, int32_t y );
//#define IMG_ADDRESS(i,x,y) ImageAddress( i,x,y )
#else
#define IMG_ADDRESS(i,x,y)    ((CDATA*)	                             ((i)->image + (( (x) - (i)->eff_x )	 +(((i)->flags&IF_FLAG_INVERTED)?(INVERTY_INVERTED( (i), (y) ) * (i)->pwidth ):(INVERTY_NON_INVERTED( (i), (y) ) * (i)->pwidth ))	                             ))										   )
#endif
#if 0
#if defined( __arm__ ) && defined( IMAGE_LIBRARY_SOURCE ) && !defined( DISPLAY_SOURCE )
extern unsigned char AlphaTable[256][256];
static CDATA DOALPHA( CDATA over, CDATA in, uint8_t a )
{
	int r, g, b, aout;
	if( !a )
		return over;
	if( a > 255 )
		a = 255;
	if( a == 255 )
 // force alpha full on.
		return (in | 0xFF000000);
	aout = AlphaTable[a][AlphaVal( over )] << 24;
	r = ((((RedVal(in))  *(a+1)) + ((RedVal(over))  *(256-(a)))) >> 8 );
	if( r > (255) ) r = (255);
	g = (((GreenVal(in))*(a+1)) + ((GreenVal(over))*(256-(a)))) >> 8;
	if( g > (255) ) g = (255);
	b = ((((BlueVal(in)) *(a+1)) + ((BlueVal(over)) *(256-(a)))) >> 8 );
	if( b > 255 ) b = 255;
	return aout|(r<<16)|(g<<8)|b;
	//return AColor( r, g, b, aout );
}
#endif
#endif
IMAGE_NAMESPACE_END
// end if_not_included
#endif
// $Log: imagestruct.h,v $
// Revision 1.2  2005/04/05 11:56:04  panther
// Adding sprite support - might have added an extra draw callback...
//
// Revision 1.1  2004/06/21 07:38:39  d3x0r
// Move structures into common...
//
// Revision 1.20  2003/10/14 20:48:55  panther
// Tweak mmx a bit - no improvement visible but shorter
//
// Revision 1.19  2003/10/14 16:36:45  panther
// Oops doalpha was outside of known inclusion frame
//
// Revision 1.18  2003/10/14 00:43:03  panther
// Arm optimizations.  Looks like I'm about maxed.
//
// Revision 1.17  2003/09/15 17:06:37  panther
// Fixed to image, display, controls, support user defined clipping , nearly clearing correct portions of frame when clearing hotspots...
//
// Revision 1.16  2003/04/25 08:33:09  panther
// Okay move the -1's back out of IMG_ADDRESS
//
// Revision 1.15  2003/04/21 23:33:09  panther
// fix certain image ops - should check blot direct...
//
// Revision 1.14  2003/03/30 18:39:03  panther
// Update image blotters to use IMG_ADDRESS
//
// Revision 1.13  2003/03/30 16:11:03  panther
// Clipping images works now... blat image untested
//
// Revision 1.12  2003/03/30 06:24:56  panther
// Turns out I had badly implemented clipping...
//
// Revision 1.11  2003/03/25 08:45:51  panther
// Added CVS logging tag
//
#if defined( __cplusplus )
IMAGE_NAMESPACE
#endif
/* pointer to a sprite type. */
typedef struct sprite_tag *PSPRITE;
//#endif
// at some point, it may be VERY useful
// to have this structure also have a public member.
//
#ifndef NO_FONT
typedef struct simple_font_tag {
 // all characters same height
   uint16_t height;
 // number of characters in the set
   uint16_t characters;
 // open ended array size characters...
   uint8_t char_width[1];
} FontData;
/* Contains information about a font for drawing and rendering
   from a font file.                                           */
typedef struct font_tag *SFTFont;
#endif
/* A definition of a block structure to transport font and image
   data across message queues.                                   */
/* Type of buffer used to transfer data across message queues. */
typedef struct data_transfer_state_tag {
   /* size of this block of data. */
   uint32_t size;
   /* offset of the data in the total message. Have to break up
      large buffers into smaller chunks for transfer.           */
   uint32_t offset;
   /* buffer containing the data to transfer. */
   CDATA buffer;
} *DataState;
//-----------------------------------------------------
enum string_behavior {
   // every character assumed to have a glyph-including '\0'
   STRING_PRINT_RAW
 // control characters perform 'typical' actions - newline, tab, backspace...
   ,STRING_PRINT_CONTROL
  // c style escape characters are handled \n \b \x## - literal text
   ,STRING_PRINT_C
   ,STRING_PRINT_MENU
 /* &amp; performs an underline, also does C style handling. \\&amp;
                         == &amp;                                                         */
};
/* Definitions of symbols to pass to <link SetBlotMethod> to
   specify optimization method.                              */
enum blot_methods {
    /* A Symbol to pass to <link SetBlotMethod> to specify using C
      coded primitives. (for shading and alpha blending).         */
    BLOT_C
   , BLOT_ASM
/* A Symbol to pass to <link SetBlotMethod> to specify using
      primitives with assembly optimization (for shading and alpha
      blending).                                                   */
						,
                  /* A Symbol to pass to <link SetBlotMethod> to specify using
      primitives with MMX optimization (for shading and alpha
      blending).                                                */
    BLOT_MMX
};
// specify the method that pixels are copied from one image to another
enum BlotOperation {
   /* copy the pixels from one image to another with no color transform*/
 BLOT_COPY = 0,
   // copy the pixels from one image to another with no color transform, scaling by a single color
 BLOT_SHADED = 1,
   // copy the pixels from one image to another with no color transform, scaling independant R, G and B color channels to a combination of an R Color, B Color, G Color
 BLOT_MULTISHADE = 2,
   /* copy the pixels from one image to another with simple color inversion transform*/
 BLOT_INVERTED = 3,
 /* orientation blots for fonts to 3D and external displays */
 BLOT_ORIENT_NORMAL = 0x00,
 BLOT_ORIENT_INVERT = 0x04,
 BLOT_ORIENT_VERTICAL = 0x08,
 BLOT_ORIENT_VERTICAL_INVERT = 0x0C,
 BLOT_ORIENTATTION = 0x0C,
};
/* Transparency parameter definition
   0 : no transparency - completely opaque
   1 (TRUE): 0 colors (absolute transparency) only
   2-255 : 0 color transparent, plus transparency factor applied
   to all 2 - mostly almost completely transparent 255 not
   transparent (opaque)
   257-511 : alpha transparency in pixel plus transparency value
   \- 256 0 pixels will be transparent 257 - slightly more
   opaquen than the original 511 - image totally opaque - alpha
   will be totally overriden no addition 511 nearly completely
   transparent 512-767 ; the low byte of this is subtracted from
   the alpha of the image ; this allows images to be more
   transparent than they were originally 512 - no modification
   alpha imge normal 600 - mid range... more transparent 767 -
   totally transparent any value of transparent greater than the
   max will be clipped to max this will make very high values
   opaque totally...                                             */
enum AlphaModifier {
   /* Direct alpha copy - whatever the alpha is is what the output will be.  Adding a value of 0-255 here will increase the base opacity by that much */
	ALPHA_TRANSPARENT = 0x100,
   // Inverse alpha copy - whatever the alpha is is what the output will be.  Adding a value of 0-255 here will decrease the base opacity by that much
ALPHA_TRANSPARENT_INVERT = 0x200,
   // more than this clips to total transparency
	// for line, plot more than 255 will
// be total opaque... this max only
	// applies to blotted images
ALPHA_TRANSPARENT_MAX = 0x2FF
};
/* library global changes. string behavior cannot be tracked per
   image. string behavior should, for all strings, be the same
   usage for an application... so behavior is associated with
   the particular stream and/or image family. does not modify
   character handling behavior - only strings.
   See Also
   <link sack::image::string_behavior, String Behaviors>         */
   IMAGE_PROC  void IMAGE_API  IMGVER(SetStringBehavior)( Image pImage, uint32_t behavior );
   /* Specify the optimized code to draw with. There are 3 levels,
      C - routines coded in C, ASM - assembly optimization (32bit
      NASM), MMX assembly but taking advantage of MMX features.    */
   IMAGE_PROC  void IMAGE_API  IMGVER(SetBlotMethod)    ( uint32_t method );
   /* This routine can be used to generically scale to any point
      between two colors.
      Parameters
      Color 1 :   CDATA color to scale from
      Color 2 :   CDATA color to scale to
      distance :  How from from 0 to max distance to scale.
      max :       How wide the scalar is.
      Remarks
      Max is the scale that distance can go from. Distance 0 is the
      first color, Distance == max is the second color. The
      distance from 0 to max proportionately scaled the color....
      Example
      <code lang="c++">
      CDATA green = BASE_COLOR_GREEN;
      CDATA blue = BASE_COLOR_BLUE;
      CDATA red = BASE_COLOR_RED;
      </code>
      Compute a color that is halfway from blue to green. (if the
      total distance is 100, then 50 is half way).
      <code lang="c++">
      CDATA blue_green = ColorAverage( blue, green, 50, 100 );
      </code>
      Compute a color that's mostly red.
      <code lang="c++">
      CDATA red_blue_green = ColorAverage( blue_green, red, 240, 255 );
      </code>
      Iterate through a whole scaled range...
      <code lang="c++">
      int n;
      for( n = 0; n \< 100; n++ )
      {
          CDATA scaled = ColorAverage( BASE_COLOR_WHITE, BASE_COLOR_BLACK, n, 100 );
          // as n increases, the color slowly goes from WHITE to BLACK.
      }
      </code>                                                                        */
   IMAGE_PROC  CDATA IMGVER(ColorAverage)( CDATA c1, CDATA c2, int d, int max );
   /* Creates an image from user defined parts. The buffer used is
      from the user. This was used by the video library, but
      RemakeImage accomplishes this also.
      Parameters
      pc :      the color buffer to use for the image.
      width :   how wide the color buffer is
      height :  How tall the color buffer is                       */
   IMAGE_PROC  Image IMAGE_API IMGVER(BuildImageFileEx) ( PCOLOR pc, uint32_t width, uint32_t height DBG_PASS);
   /* <combine sack::image::MakeImageFile>
      Adds <link sack::DBG_PASS, DBG_PASS> parameter. */
   /* Creates an Image with a specified width and height. The
      image's color is undefined to start.
      Parameters
      Width :     how wide to make the image. Cannot be negative.
      Height :    how tall to make the image. Cannot be negative.
      DBG_PASS :  _nt_
      Example
      See <link sack::image::Image, Image>                        */
   IMAGE_PROC  Image IMAGE_API IMGVER(MakeImageFileEx  )(uint32_t Width, uint32_t Height DBG_PASS);
   /* Creates a sub image region on an image. Sub-images may be
      used like any other image. There are two uses for this sort
      of thing. OH, the sub image shares the exact data of the
      parent image, and is not a copy.
      Parameters
      pImage :  image to make the sub image in
      x :       signed location of the top side of the sub\-image
      y :       signed location of the left side of the sub\-image
      width :   how wide to make the sub\-image
      height :  how tall to make the sub\-image
      Returns
      NULL if the input image is NULL.
      Otherwise returns an Image.
      Example
      Use 1: An image might contain a grid of symbols or
      characters, each exactly the same size. These may be token
      pieces used in a game or a special graphic font.
      <code lang="c++">
      Image pieces_image = LoadImageFile( "Game Pieces.image" );
      PLIST pieces = NULL;
      int x, y;
      \#define PIECE_WIDTH 32
      \#define PIECE_HEIGHT 32
      for( x = 0; x \< 10; x++ )
         for( y = 0; y \< 2; y++ )
         {
             AddLink( &amp;pieces, MakeSubImage( pieces_image
                                           , x * PIECE_WIDTH, y * PIECE_HEIGHT
                                           , PIECE_WIDTH, PIECE_HEIGHT );
         }
      // at this point there we have a list with all the tokens,
      // which were 32x32 pixels each.
      // Any of these piece images may be output using a scaled or direct blot.
      </code>
      Use 2: Partitioning views on an image for things like
      controls and other clipped regions.
      <code lang="c++">
      Image image = MakeImageFile( 1024, 768 );
      Image clock = MakeSubImage( image, 32, 32, 150, 16 );
      DrawString( clock, 0, 0, BASE_COLOR_WHITE, BASE_COLOR_BLACK, "Current Time..." );
      </code>                                                                           */
   IMAGE_PROC  Image IMAGE_API IMGVER(MakeSubImageEx   )( Image pImage, int32_t x, int32_t y, uint32_t width, uint32_t height DBG_PASS );
   /* Adds an image as a sub-image of another image. The image
      being added as a sub image must not already have a parent.
      Sub-images are like views into the parent, and share the same
      pixel buffer that the parent has.
      Parameters
      pFoster :  This is the parent image to received the new
                 subimage
      pOrphan :  this is the subimage to be added                   */
   IMAGE_PROC  void IMAGE_API IMGVER(AdoptSubImage    )( Image pFoster, Image pOrphan );
   /* Removes a sub-image (child image) from a parent image. The
      sub image my then be moved to another image with
      AdoptSubImage.
      Parameters
      pImage :  the sub\-image to orphan.                        */
   IMAGE_PROC  void IMAGE_API IMGVER(OrphanSubImage   )( Image pImage );
   /* Create or recreate an image using the specified color buffer,
      and size. All sub-images have their color data reference
      updated.
      Example
      <code>
      Image image = NULL;
      POINTER data = NewArray( CDATA, 100* 100 );
      image = RemakeImage( image, data, 100, 100 );
      </code>
      Remarks
      If the source image is NULL, a new image will be built using
      the color buffer and size specified.
      Image.flags has IF_FLAG_EXTERN_COLORS set if made this way,
      since the color buffer is an external resource. This causes
      UnmakeImage() to not attempt to free the color buffer.
      If the original image does exist, its color buffer is swapped
      for the one specified, and coordinates are updated. The video
      system uses this to create an image that has the color data
      surface the surface of the display.
      See Also
      <link sack::image::BuildImageFile, BuildImageFile>
      GetDisplayImage
      Parameters
      data :    Pointer to a buffer of 32 bit color data. ARGB and
                ABGR available via compile option.
      width :   the width of the data in pixels.
      height :  the height of the data in pixels.
      Returns
      \Returns the original image if not NULL, otherwise results
      with an image who's color plane is defined by a user defined
      buffer of width by height size. The user must have allocated
      this buffer appropriately, and is responsible for its
      destruction.                                                  */
   IMAGE_PROC  Image IMAGE_API IMGVER(RemakeImageEx    )( Image pImage, PCOLOR pc, uint32_t width, uint32_t height DBG_PASS);
   /* Load an image file. Today we support PNG, JPG, GIF, BMP.
      Tomorrow consider tapping into that FreeImage project on
      sourceforge, that combines all readers into one.
      Parameters
      name :      Filename to read from. Opens in 'Current Directory'
                  if not an absolute path.
      DBG_PASS :  _nt_
      Example
      See <link sack::image::Image, Image>                            */
	IMAGE_PROC  Image IMAGE_API IMGVER(LoadImageFileEx  )( CTEXTSTR name DBG_PASS );
	/* <combinewith sack::image::LoadImageFileEx@CTEXTSTR name>
	   Extended load image file. This allows specifying a file group
	   to load from. (Groups were added for platforms without
	   support of current working directory).
	   Parameters
	   group :  Group to load the file from
	   _nt_ :   _nt_                                                 */
	IMAGE_PROC Image  IMAGE_API IMGVER(LoadImageFileFromGroupEx )( INDEX group, CTEXTSTR filename DBG_PASS );
   /* Decodes a block of memory into an image. This is used
      internally so, LoadImageFile() opens the file and reads it
      into a buffer, which it then passes to DecodeMemoryToImage().
      Images stored in custom user structures may be passed for
      decoding also.
      Parameters
      buf :   Pointer to bytes of data to decode
      size :  the size of the buffer to decode
      Returns
      NULL is returned if the data does not decode as an image.
      an Image is returned otherwise.
      Example
      This pretends that you have a FILE* open to some image
      already, and that the image is tiny (less than 4k bytes).
      <code lang="c#">
      char buffer[4096];
      int length;
      length = fread( buffer, 1, 4096, some_file );
      Image image = DecodeMemoryToImage( buffer, length );
      if( image )
      {
         // buffer decoded okay.
      }
      </code>                                                       */
			IMAGE_PROC  Image IMAGE_API IMGVER(DecodeMemoryToImage )( uint8_t* buf, size_t size );
#ifdef __cplusplus
		namespace loader{
#endif
	IMAGE_PROC  LOGICAL IMAGE_API IMGVER(PngImageFile )( Image image, uint8_t* *buf, size_t *size );
	IMAGE_PROC  LOGICAL IMAGE_API IMGVER(JpgImageFile )( Image image, uint8_t* *buf, size_t *size, int Q );
#ifdef __cplusplus
		}
#endif
      /* direct hack for processing clipboard data... probably does some massaging of the databefore calling DecodeMemoryToImage */
   IMAGE_PROC  Image IMAGE_API IMGVER(ImageRawBMPFile )(uint8_t* ptr, uint32_t filesize);
	/* Releases an image, has extra debug parameters.
	   Parameters
	   Image :     the Image to release.
	   DBG_PASS :  Adds <link sack::DBG_PASS, DBG_PASS> parameter for
	               the release memory tracking.                       */
	IMAGE_PROC  void IMAGE_API IMGVER(UnmakeImageFileEx )( Image pif DBG_PASS );
   /* Sets the active image rectangle to the bounding rectangle
      specified. This can be used to limit artificially drawing
      onto an image. (It is easier to track to create a subimage in
      the location to draw instead of masking with a bound rect,
      which has problems restoring back to initial conditions)
      Parameters
      pImage :  Image to set the drawing clipping rectangle.
      bound :   a pointer to an IMAGE_RECTANGLE to set the image
                boundaries to.                                      */
   IMAGE_PROC  void  IMAGE_API IMGVER(SetImageBound    )( Image pImage, P_IMAGE_RECTANGLE bound );
// reset clip rectangle to the full image (subimage part )
// Some operations (move, resize) will also reset the bound rect,
// this must be re-set afterwards.
// ALSO - one SHOULD be nice and reset the rectangle when done,
// otherwise other people may not have checked this.
/* Change the size of an image, reallocating the color buffer as
   necessary.
   <b>Parameters</b>
   <b>Remarks</b>
   If the image is a sub image (located within a parent), the
   subimage view on the parent image is updated to the new width
   and height. The color buffer remains the parent's buffer.
   If the image is a parent, a new buffer is allocated. If the
   previous buffer was specified by the user in RemakeImage,
   that buffer is not freed, but a new buffer is still created.
   <b>Bugs</b>
   If the image is a parent image, the child images are not
   updated to the newly allocated buffer. Resize works really
   well for subimages though.                                    */
   IMAGE_PROC  void IMAGE_API IMGVER(ResizeImageEx     )( Image pImage, int32_t width, int32_t height DBG_PASS);
   /* Moves an image within a parent image. Top level images and
      images which have a user color buffer do not move.
      Parameters
      pImage :  The image to move.
      x :       the new X coordinate of the image.
      y :       the new Y coordinate of the image.               */
   IMAGE_PROC  void IMAGE_API IMGVER(MoveImage         )( Image pImage, int32_t x, int32_t y );
//-----------------------------------------------------
   IMAGE_PROC  void IMAGE_API IMGVER(BlatColor         )( Image pifDest, int32_t x, int32_t y, uint32_t w, uint32_t h, CDATA color );
   /* Blat is the sound a trumpet makes when it spews forth
      noise... so Blat color is just fill a rectangle with a color,
      quickly. Apply alpha transparency of the color specified.
      Parameters
      pifDest :  The destination image to fill the rectangle on
      x :        left coordinate of the rectangle
      y :        right coordinate of the rectangle
      w :        width of the rectangle
      h :        height of the rectangle
      color :    color to fill the rectangle with. The alpha of this
                 color will be applied.                              */
   IMAGE_PROC  void IMAGE_API IMGVER(BlatColorAlpha    )( Image pifDest, int32_t x, int32_t y, uint32_t w, uint32_t h, CDATA color );
   /* \ \
      Parameters
      pDest :         destination image (the one to copy to)
      pIF :           source image
      x :             destination top coordinate
      y :             destination left coordinate
      nTransparent :  <link sack::image::AlphaModifier, Alpha Operation>
      method :        <link sack::image::blot_methods, Blot Method>
      _nt_ :          _nt_                                               */
   IMAGE_PROC  void IMAGE_API IMGVER(BlotImageEx       )( Image pDest, Image pIF, int32_t x, int32_t y, uint32_t nTransparent, uint32_t method, ... );
   /* Copies an image from one image onto another. The copy is done
      directly and no scaling is applied. If a width or height
      larget than the image to copy is specified, only the amount
      of the image that is valid is copied.
      Parameters
      pDest :         Destination image
      pIF :           Image file to copy
      x :             X position to put copy at
      y :             Y position to put copy at
      xs :            X position to copy from.
      ys :            Y position to copy from.
      wd :            how much of the image horizontally to copy
      ht :            how much of the image vertically to copy
      nTransparent :  <link sack::image::AlphaModifier, Alpha Transparency method>
      method :        <link sack::image::blot_methods, BlotMethods>
      <b>Method == BLOT_SHADED extra parameters</b>
      red :    Color to use the red channel to output the scale from
               black to color
      green :  Color to use the red channel to output the scale from
               black to color
      blue :   Color to use the red channel to output the scale from
               black to color
      <b>Method == BLOT_SHADED extra parameters</b>
      shade :  _nt_
      See Also                                                                     */
   IMAGE_PROC  void IMAGE_API IMGVER(BlotImageSizedEx  )( Image pDest, Image pIF, int32_t x, int32_t y, int32_t xs, int32_t ys, uint32_t wd, uint32_t ht, uint32_t nTransparent, uint32_t method, ... );
   /* Copies some or all of an image to a destination image of
      specified width and height. This does linear interpolation
      scaling.
      There are simple forms of this function as macros, since
      commonly you want to output the entire image, a macro which
      automatically sets (0,0),(width,height) as the source
      \parameters to output the whole image exists.
      Parameters
      \ \
      pifDest :       Destination image
      pifSrc :        image to copy from
      xd :            destination x coordinate
      yd :            destination y coordinate
      wd :            destination width (source image width will be
                      scaled to this)
      hd :            destination height (source image height will
                      be scaled to this)
      xs :            source x coordinate (where to copy from)
      ys :            source y coordinate (where to copy from)
      ws :            source width (how much of the image to copy)
      hs :            source height (how much of the image to copy)
      nTransparent :  Alpha method...
      method :        specifies how the source color data is
                      transformed if at all. See BlotMethods
      ... :           possible extra parameters depending on method
      <b>Method == BLOT_MULTISHADE extra parameters</b>
      red :    Color to use the red channel to output the scale from
               black to color
      green :  Color to use the red channel to output the scale from
               black to color
      blue :   Color to use the red channel to output the scale from
               black to color
      <b>Method == BLOT_SHADED extra parameters</b>
      shade :  _nt_
      See Also
      <link sack::image::AlphaModifier, Alpha Methods>
      <link sack::image::blot_methods, Blot Methods>
      <link sack::image::BlotScaledImage, BlotScaledImage>
      <link sack::image::BlotScaledImageShaded, BlotScaledImageShaded>
      <link sack::image::BlotScaledImageShadedAlpha, BlotScaledImageShadedAlpha>
                                                                                 */
   IMAGE_PROC  void IMAGE_API IMGVER(BlotScaledImageSizedEx)( Image pifDest, Image pifSrc
                                   , int32_t xd, int32_t yd
                                   , uint32_t wd, uint32_t hd
                                   , int32_t xs, int32_t ys
                                   , uint32_t ws, uint32_t hs
                                   , uint32_t nTransparent
                                   , uint32_t method, ... );
/* Your basic PLOT functions (Image.C, plotasm.asm)
   A function pointer to the function which sets a pixel in an
   image at a specified x, y coordinate.
   Parameters
   Image :  The image to get the pixel from
   X :      x coordinate to get pixel color
   Y :      y coordinate to get pixel color
   Color :  color to put at the coordinate. image will be set
            exactly to this color, and whatever the alpha of the
            color is.                                            */
   IMAGE_PROC  void plot       ( Image pi, int32_t x, int32_t y, CDATA c );
   /* A function pointer to the function which sets a pixel in an
      image at a specified x, y coordinate.
      Parameters
      Image :  The image to get the pixel from
      X :      x coordinate to get pixel color
      Y :      y coordinate to get pixel color
      Color :  color to put at the coordinate. Alpha blending will be
               done.                                                  */
   IMAGE_PROC  void plotalpha  ( Image pi, int32_t x, int32_t y, CDATA c );
   /* A function pointer to the function which gets a pixel from an
      image at a specified x, y coordinate.
      Parameters
      Image :  The image to get the pixel from
      X :      x coordinate to get pixel color
      Y :      y coordinate to get pixel color
      Returns
      CDATA color in the Image at the specified coordinate.         */
   IMAGE_PROC  CDATA getpixel  ( Image pi, int32_t x, int32_t y );
//-------------------------------
// Line functions  (lineasm.asm) // should include a line.c ... for now core was assembly...
//-------------------------------
  // d is color data...
   IMAGE_PROC  void do_line      ( Image pBuffer, int32_t x, int32_t y, int32_t xto, int32_t yto, CDATA color );
  // d is color data...
   IMAGE_PROC  void do_lineAlpha ( Image pBuffer, int32_t x, int32_t y, int32_t xto, int32_t yto, CDATA color);
   /* This is a function pointer that references a function to do
      optimized horizontal lines. The function pointer is updated
      when SetBlotMethod() is called.
      Parameters
      Image :   the image to draw to
      Y :       the y coordinate of the line (how far down from top to
                draw it)
      x_from :  X coordinate to draw from
      x_to :    X coordinate to draw to
      color :   the color of the line. This color will be set to the
                surface, the alpha result will be the alpha of this
                color.                                                 */
   IMAGE_PROC  void do_hline      ( Image pImage, int32_t y, int32_t xfrom, int32_t xto, CDATA color );
   /* This is a function pointer that references a function to do
      optimized vertical lines. The function pointer is updated
      when SetBlotMethod() is called.
      Parameters
      Image :   the image to draw to
      X :       the x coordinate of the line (how far over to draw
                it)
      y_from :  Y coordinate to draw from
      y_to :    Y coordinate to draw to
      color :   the color of the line. This color will be set to the
                surface, the alpha result will be the alpha of this
                color.                                               */
   IMAGE_PROC  void do_vline      ( Image pImage, int32_t x, int32_t yfrom, int32_t yto, CDATA color );
   /* This is a function pointer that references a function to do
      optimized horizontal lines with alpha blending. The function
      pointer is updated when SetBlotMethod() is called.
      Parameters
      Image :   the image to draw to
      Y :       the Y coordinate of the line (how far down from top
                of image to draw it)
      x_from :  X coordinate to draw from
      x_to :    X coordinate to draw to
      color :   the color of the line (alpha component of the color
                will be applied)                                    */
   IMAGE_PROC  void do_hlineAlpha ( Image pImage, int32_t y, int32_t xfrom, int32_t xto, CDATA color );
   /* This is a function pointer that references a function to do
      optimized vertical lines with alpha blending. The function
      pointer is updated when SetBlotMethod() is called.
      Parameters
      Image :   the image to draw to
      X :       the x coordinate of the line (how far over to draw
                it)
      y_from :  Y coordinate to draw from
      y_to :    Y coordinate to draw to
      color :   the color of the line (alpha component of the color
                will be applied)                                    */
   IMAGE_PROC  void do_vlineAlpha ( Image pImage, int32_t x, int32_t yfrom, int32_t yto, CDATA color );
	/* routine which iterates through the points along a lone from
	   x,y to xto,yto, calling a user function at each point.
	   Parameters
	   Image :  the image to pretend to draw on
	   x :      draw from this x coordinate
	   y :      draw from this y coordinate
	   xto :    draw to this x coordinate
	   yto :    draw to this y coordinate
	   d :      userdata (color data)
	   func :   user callback function to a function of type...<p />void
	            func( Image pif, int32_t x, int32_t y, int d ) ;
	   Remarks
	   The Image passed does not HAVE to be an Image, it can be any
	   user POINTER.
	   The data passed is limited to 32 bits, and will not hold a
	   pointer if built for 64 bit platform.
	   Example
	   <code lang="c++">
	   Image image;
	   void MyPlotter( Image image, int32_t x, int32_t y, CDATA color )
	   {
	       // do something with the image at x,y
	   }
	   void UseMyPlotter( Image image )
	   {
	       do_lineExV( image, 10, 10, 80, 80, BASE_COLOR_BLACK, MyPlotter );
	   }
	   </code>                                                               */
	IMAGE_PROC  void do_lineExV    ( Image pImage, int32_t x, int32_t y
									, int32_t xto, int32_t yto, uintptr_t color
		                            , void (*func)( Image pif, int32_t x, int32_t y, uintptr_t d ) );
   /* \Returns the correct SFTFont pointer to the default font. In all
      font functions, NULL may be used as the font, and this is the
      font that will be used.
      Parameters
      None.
      Example
      <code lang="c++">
      SFTFont font = GetDefaultFont();
      </code>                                                       */
   IMAGE_PROC  SFTFont IMAGE_API IMGVER(GetDefaultFont )( void );
   /* \Returns the height of a font for purposes of spacing between
      lines. Characters may render outside of this height.
      Parameters
      SFTFont :  SFTFont to get the height of. if NULL returns an internal
              font's height.
      Returns
      the height of the font.                                        */
   IMAGE_PROC  uint32_t  IMAGE_API IMGVER(GetFontHeight  )( SFTFont );
   /* \Returns the approximate rectangle that would be used for a
      string. It only counts using the line measurement. Newlines
      in strings count to wrap text to subsequent lines and start
      recounting the width, returning the maximum length of string
      horizontally.
      Parameters
      pString :  The string to measure
      len :      the length of characters to count in string
      width :    a pointer to a uint32_t to receive the width of the
                 string
      height :   a pointer to a uint32_t to receive the height of the
                 string
      UseFont :  A SFTFont to use.
      Returns
      \Returns the width parameter. If NULL are passed for width
      and height, this is OK. One of the simple macros just passes
      the string and gets the return - this is for how wide the
      string would be.                                             */
   IMAGE_PROC  uint32_t  IMAGE_API IMGVER(GetStringSizeFontEx)( CTEXTSTR pString, size_t len, uint32_t *width, uint32_t *height, SFTFont UseFont );
   /* Fill the width and height with the actual size of the string
      as it is drawn. (may be above or below the original
      rectangle)
      Parameters
      pString :     the string to measure
      nLen :        the number of characters in the string
      width :       a pointer to a 32 bit value to get resulting
                    width
      height :      a pointer to a 32 bit value to get resulting
                    height
      charheight :  the actual height of the characters (as reports
                    by line)
      UseFont :     a SFTFont to use. If NULL use a default internal
                    font.                                           */
   IMAGE_PROC  uint32_t IMAGE_API IMGVER(GetStringRenderSizeFontEx )( CTEXTSTR pString, size_t nLen, uint32_t *width, uint32_t *height, uint32_t *charheight, SFTFont UseFont );
// background of color 0,0,0 is transparent - alpha component does not
// matter....
   IMAGE_PROC  void IMAGE_API IMGVER(PutCharacterFont              )( Image pImage
                                                  , int32_t x, int32_t y
                                                  , CDATA color, CDATA background,
                                                   TEXTCHAR c, SFTFont font );
   /* Outputs a string in the specified font, from the specified
      point, text is drawn from the point up, with the characters
      aligned with the top to the left; it goes up from the point.
      the point becomes the bottom left of the rectangle output.
      Parameters
      pImage :      image to draw string into
      x :           x position of the string
      y :           y position of the string
      color :       color of the data drawn in the font
      background :  color of the data not drawn in the font
      c :           the character to output
      font :        the font to use. NULL use an internal default
                    font.                                          */
   IMAGE_PROC  void IMAGE_API IMGVER(PutCharacterVerticalFont      )( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, TEXTCHAR c, SFTFont font );
   /* Outputs a string in the specified font, from the specified
      point, text is drawn from the point to the left, with the
      characters aligned with the top to the left; it goes up from
      the point. the point becomes the bottom left of the rectangle
      \output.
      Parameters
      pImage :      image to draw string into
      x :           x position of the string
      y :           y position of the string
      color :       color of the data drawn in the font
      background :  color of the data not drawn in the font
      pc :          pointer to constant text
      nLen :        length of text to output
      font :        the font to use. NULL use an internal default
                    font.                                           */
   IMAGE_PROC  void IMAGE_API IMGVER(PutCharacterInvertFont        )( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, TEXTCHAR c, SFTFont font );
   /* Outputs a character in the specified font, from the specified
      point, text is drawn from the point up, with the characters
      aligned with the top to the left; it goes up from the point. the
      point becomes the bottom left of the rectangle output.
      Parameters
                                                                       */
   IMAGE_PROC  void IMAGE_API IMGVER(PutCharacterVerticalInvertFont)( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, TEXTCHAR c, SFTFont font );
   /* Outputs a string in the specified font, from the specified
      point, text is drawn right side up and godes from left to
      right. The point becomes the top left of the rectangle
      \output.
      Parameters
      pImage :      image to draw string into
      x :           x position of the string
      y :           y position of the string
      color :       color of the data drawn in the font
      background :  color of the data not drawn in the font
      pc :          pointer to constant text
      nLen :        length of text to output
      font :        the font to use. NULL use an internal default
                    font.                                         */
   IMAGE_PROC  void IMAGE_API IMGVER(PutStringFontEx              )( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, CTEXTSTR pc, size_t nLen, SFTFont font );
   /* justification 0 is left, 1 is right, 2 is center */
   IMAGE_PROC  void IMAGE_API IMGVER(PutStringFontExx              )( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, CTEXTSTR pc, size_t nLen, SFTFont font, int justication, uint32_t _width );
   /* Outputs a string in the specified font, from the specified
      point, text is drawn from the point down, with the characters
      aligned with the top to the right; it goes down from the
      point. the point becomes the top right of the rectangle
      \output.
      Parameters
      pImage :      image to draw string into
      x :           x position of the string
      y :           y position of the string
      color :       color of the data drawn in the font
      background :  color of the data not drawn in the font
      pc :          pointer to constant text
      nLen :        length of text to output
      font :        the font to use. NULL use an internal default
                    font.                                           */
   IMAGE_PROC  void IMAGE_API IMGVER(PutStringVerticalFontEx      )( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, CTEXTSTR pc, size_t nLen, SFTFont font );
   /* Outputs a string in the specified font, from the specified
      point, text is drawn upside down, and goes to the left from
      the point. the point becomes the bottom right of the
      rectangle output.
      Parameters
      pImage :      image to draw string into
      x :           x position of the string
      y :           y position of the string
      color :       color of the data drawn in the font
      background :  color of the data not drawn in the font
      pc :          pointer to constant text
      nLen :        length of text to output
      font :        the font to use. NULL use an internal default
                    font.                                         */
   IMAGE_PROC  void IMAGE_API IMGVER(PutStringInvertFontEx        )( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, CTEXTSTR pc, size_t nLen, SFTFont font );
   /* Outputs a string in the specified font, from the specified
      point, text is drawn from the point up, with the characters
      aligned with the top to the left; it goes up from the point. the
      point becomes the bottom left of the rectangle output.
      Parameters
      pImage :      image to draw string into
      x :           x position of the string
      y :           y position of the string
      color :       color of the data drawn in the font
      background :  color of the data not drawn in the font
      pc :          pointer to constant text
      nLen :        length of text to output
      font :        the font to use. NULL use an internal default
                    font.                                              */
   IMAGE_PROC  void IMAGE_API IMGVER(PutStringInvertVerticalFontEx)( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, CTEXTSTR pc, size_t nLen, SFTFont font );
   //uint32_t (*PutMenuStringFontEx)            ( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, CTEXTSTR pc, uint32_t nLen, SFTFont font );
   //uint32_t (*PutCStringFontEx)               ( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, CTEXTSTR pc, uint32_t nLen, SFTFont font );
   IMAGE_PROC  uint32_t IMAGE_API IMGVER(GetMaxStringLengthFont  )( uint32_t width, SFTFont UseFont );
   /* Used as a proper accessor method to get an image's width and
      height. Decided to allow the image structure to be mostly
      public, so the first 4 members are the images x,y, width and
      height, and are immediately accessable by the Image pointer.
      Parameters
      pImage :  image to get the size of
      width :   pointer to a 32 bit unsigned value to result with the
                width, if NULL ignored.
      height :  pointer to a 32 bit unsigned value to result with the
                height, if NULL ignored.                              */
   IMAGE_PROC  void IMAGE_API IMGVER(GetImageSize            )( Image pImage, uint32_t *width, uint32_t *height );
   /* \Returns the pointer to the color buffer currently used
      \internal to the image.
      Parameters
      pImage :  Image to get the surface of.
      Example
      <code lang="c#">
      Image image = MakeImageFile( 100, 100 );
      PCDATA pointer_color_data = GetImageSurface( image );
      </code>
      Note
      This might be used to do an optimized output routine. Drawing
      to the image with plot and line are not necessarily the best
      for things like circles. Provides ability for user to output
      directly to the color buffer.                                 */
   IMAGE_PROC  PCDATA IMAGE_API IMGVER(GetImageSurface        )( Image pImage );
   // would seem silly to load fonts - but for server implementations
   // the handle received is not the same as the font sent.
   IMAGE_PROC  SFTFont IMAGE_API IMGVER(LoadFont                )( SFTFont font );
   /* Destroys a font, releasing all resources associated with
      character data and font rendering.                       */
   IMAGE_PROC  void IMAGE_API IMGVER(UnloadFont              )( SFTFont font );
	/* This is a function used to synchronize image operations when
	   the image interface is across a message server.              */
	IMAGE_PROC  void IMAGE_API IMGVER(SyncImage                  )( void );
	// intersect rectangle, results with the overlapping portion of R1 and R2
   // into R ...
   IMAGE_PROC  int IMAGE_API IMGVER(IntersectRectangle )( IMAGE_RECTANGLE *r, IMAGE_RECTANGLE *r1, IMAGE_RECTANGLE *r2 );
   /* Merges two image rectangles. The resulting rectangle is a
      rectangle that includes both rectangles.
      Parameters
      r :   Pointer to an IMAGE_RECTANGLE for the result.
      r1 :  PIMAGE_RECTANGLE one rectangle.
      r2 :  PIMAGE_RECTANGLE the other rectangle.               */
   IMAGE_PROC  int IMAGE_API IMGVER(MergeRectangle )( IMAGE_RECTANGLE *r, IMAGE_RECTANGLE *r1, IMAGE_RECTANGLE *r2 );
   /* User applications may use an aux rect attatched to an image. The
      'Display' render library used this itself however, making
      this mostly an internal feature.
      Parameters
      pImage :  image to get the aux rect of.
      pRect :   pointer to an IMAGE_RECTANGLE to get the aux
                rectangle data in.                                     */
   IMAGE_PROC  void IMAGE_API IMGVER(GetImageAuxRect    )( Image pImage, P_IMAGE_RECTANGLE pRect );
   /* User applications may use an aux rect attatched to an image.
      The 'Display' render library used this itself however, making
      this mostly an internal feature.
      Parameters
      pImage :  image to set the aux rect of.
      pRect :   pointer to an IMAGE_RECTANGLE to set the aux
                rectangle to.                                       */
   IMAGE_PROC  void IMAGE_API IMGVER(SetImageAuxRect    )( Image pImage, P_IMAGE_RECTANGLE pRect );
	/* \ \
	   Parameters
	   Filename :  \file name of image to load. Converts image into
	               sprite automatically, resulting with a sprite.
	   DBG_PASS :  See <link sack::DBG_PASS, DBG_PASS>              */
		IMAGE_PROC  PSPRITE IMAGE_API IMGVER(MakeSpriteImageFileEx )( CTEXTSTR fname DBG_PASS );
      /* create a sprite from an Image */
	IMAGE_PROC  PSPRITE IMAGE_API IMGVER(MakeSpriteImageEx )( Image image DBG_PASS );
	/* Release a Sprite. */
	IMAGE_PROC  void IMAGE_API IMGVER(UnmakeSprite )( PSPRITE sprite, int bForceImageAlso );
	// angle is a fixed scaled integer with 0x1 0000 0000 being the full circle.
	IMAGE_PROC  void IMAGE_API IMGVER(rotate_scaled_sprite )(Image bmp, PSPRITE sprite, fixed angle, fixed scale_width, fixed scale_height );
   /* output a rotated sprite to destination image, using and angle specified.  The angle is represented as 0x1 0000 0000 is 360 degrees */
	IMAGE_PROC  void IMAGE_API IMGVER(rotate_sprite )(Image bmp, PSPRITE sprite, fixed angle);
   /* output a sprite at its current location */
	IMAGE_PROC  void IMAGE_API IMGVER(BlotSprite )( Image pdest, PSPRITE ps );
/* Sets the point on a sprite which is the 'hotspot' the hotspot
   is the point that is drawn at the specified coordinate when
   outputting a sprite.
   Parameters
   sprite :  The PSPRITE to set the hotspot of.
   x :       x coordinate in the sprite's image that becomes the
             hotspot.
   y :       y coordinate in the sprite's image that becomes the
             hotspot.                                            */
IMAGE_PROC  PSPRITE IMAGE_API IMGVER(SetSpriteHotspot )( PSPRITE sprite, int32_t x, int32_t y );
/* This function sets the current location of a sprite. When
   asked to render, the sprite will draw itself here.
   Parameters
   sprite :  the sprite to move
   x :       the new x coordinate of the parent image to draw at
   y :       the new y coordinate of the parent image to draw at */
IMAGE_PROC  PSPRITE IMAGE_API IMGVER(SetSpritePosition )( PSPRITE sprite, int32_t x, int32_t y );
/* Use a font file to get a font that can be used for outputting
   characters and strings.
   Parameters
   file\ :    Filename of a font to render.
   nWidth :   desired width in pixels to render the font.
   nHeight :  desired height in pixels to render the font.
   flags :    0 = render mono. 2=render 2 bits, 3=render 8 bit.  */
IMAGE_PROC  SFTFont IMAGE_API IMGVER(InternalRenderFontFile )( CTEXTSTR file
																		, int32_t nWidth
																		, int32_t nHeight
																		, PFRACTION width_scale
																		, PFRACTION height_scale
																		, uint32_t flags
																		);
/* Rerender the current font with a new size. */
IMAGE_PROC void IMAGE_API IMGVER(RerenderFont)( SFTFont font, int32_t width, int32_t height, PFRACTION width_scale, PFRACTION height_scale );
	/* Dumps the whole cache to log file, shows family, style, path and filename.
    Is the same sort of dump that OpenFontFile uses.
	 */
IMAGE_PROC void IMAGE_API IMGVER(DumpFontCache)( void );
#ifndef INTERNAL_DUMP_FONT_FILE
/* takes a font and dumps a header-file formatted file; then the font can be
 statically built into code. */
IMAGE_PROC void IMAGE_API IMGVER(DumpFontFile)( CTEXTSTR name, SFTFont font_to_dump );
#endif
/* Creates a font based on indexes from the internal font cache.
   This is used by the FontPicker dialog to choose a font. The
   data the dialog used to render the font is available to the
   application, and may be passed back for rendering a font
   without knowing specifically what the values mean.
   Parameters
   nFamily :  The number of the family in the cache.
   nStyle :   The number of the style in the cache.
   nFile :    The number of the file in the cache.
   nWidth :   the width to use for rendering characters (in
              pixels)
   nHeight :  the height to use for rendering characters (in
              pixels)
   flags :    0 = render mono. 2=render 2 bits, 3=render 8 bit.
   Returns
   A SFTFont which can be used to output. If the file exists. NULL
   on failure.
   Example
   Used internally for FontPicker dialog, see <link sack::image::InternalRenderFontFile@CTEXTSTR@int32_t@int32_t@uint32_t, InternalRenderFontFile> */
IMAGE_PROC  SFTFont IMAGE_API IMGVER(InternalRenderFont )( uint32_t nFamily
																  , uint32_t nStyle
																  , uint32_t nFile
																  , int32_t nWidth
																  , int32_t nHeight
																  , PFRACTION width_scale
																  , PFRACTION height_scale
																  , uint32_t flags
																  );
/* Releases all resources for a SFTFont.  */
IMAGE_PROC  void IMAGE_API IMGVER(DestroyFont)( SFTFont *font );
/* Get the global font data structure. This is an internal
   structure, and it's definition may not be exported. Currently
   the definition is in documentation.
   See Also
   <link sack::image::FONT_GLOBAL, SFTFont Global>                  */
IMAGE_PROC  struct font_global_tag * IMAGE_API IMGVER(GetGlobalFonts)( void );
// types of data which may result...
typedef struct font_data_tag *PFONTDATA;
/* Information to render a font from a file to memory. */
typedef struct render_font_data_tag *PRENDER_FONTDATA;
/* Recreates a SFTFont based on saved FontData. The resulting font
   may be scaled from its original size.
   Parameters
   pfd :           pointer to font data.
   width_scale :   FRACTION to scale the original font height
                   \description by. if NULL uses the original
                   font's size.
   height_scale :  FRACTION to scale the original font height
                   \description by.  if NULL uses the original
                   font's size.
   Example
   <code lang="c++">
   POINTER some_loaded_data; // pretend it is initialized to something valid
   SFTFont font = RenderScaledFontData( some_loaded_data, NULL, NULL );
   PutStringFont( image, 0, 0, BASE_COLOR_WHITE, 0, "Hello World", font );
   </code>
   Or, maybe your original designed screen was 1024x768, and
   it's now showing on 1600x1200, for the text to remain the
   same...
   <code lang="c++">
   FRACTION width_scale;
   FRACTION height_scale;
   uint32_t w, h;
   GetDisplaySize( &amp;w, &amp;h );
   SetFraction( width_scale, w, 1024 );
   SetFraction( height_scale, h, 768 );
   SFTFont font2 = RenderScaledFontData( some_loaded_data, &amp;width_scale, &amp;height_scale );
   PutStringFont( image, 0, 0, BASE_COLOR_WHITE, 0, "Hello World", font2 );
   </code>                                                                                     */
IMAGE_PROC  SFTFont IMAGE_API IMGVER(RenderScaledFontData)( PFONTDATA pfd, PFRACTION width_scale, PFRACTION height_scale );
/* <combine sack::image::RenderScaledFontData@PFONTDATA@PFRACTION@PFRACTION>
   \ \                                                                       */
#define RenderFontData(pfd) RenderScaledFontData( pfd,NULL,NULL )
#define ogl_RenderFontData(pfd) ogl_RenderScaledFontData( pfd,NULL,NULL )
/* <combinewith sack::image::RenderScaledFontEx@CTEXTSTR@uint32_t@uint32_t@PFRACTION@PFRACTION@uint32_t@size_t *@POINTER *, sack::image::RenderScaledFontData@PFONTDATA@PFRACTION@PFRACTION>
   \ \                                                                                                                                                                        */
IMAGE_PROC SFTFont IMAGE_API IMGVER(RenderScaledFontEx)( CTEXTSTR name, uint32_t width, uint32_t height, PFRACTION width_scale, PFRACTION height_scale, uint32_t flags, size_t *pnFontDataSize, POINTER *pFontData );
/* Renders a font with a FRACTION scalar for the X and Y sizes.
   Parameters
   name :          Name of the font (file).
   width :         Original width (in pels) to make the font.
   height :        Original height (in pels) to make the font.
   width_scale :   scalar to apply to the width
   height_scale :  scalar to apply to the height
   flags :         Flags specifying how many bits to render the
                   font with (and other info?) See enum
                   FontFlags.                                   */
IMAGE_PROC SFTFont IMAGE_API IMGVER(RenderScaledFont)( CTEXTSTR name, uint32_t width, uint32_t height, PFRACTION width_scale, PFRACTION height_scale, uint32_t flags );
#define RenderScaledFont(n,w,h,ws,hs) RenderScaledFontEx(n,w,h,ws,hs,NULL,NULL)
/* Renders a font file and returns a SFTFont. The font can then be
   used in string output functions to images.
   Parameters
   file\ :           \File name of a font to render. Any font
                     that freetype supports.
   width :           width of characters to render in.
   height :          height of characters to render.
   flags :           if( ( flags &amp; 3 ) == 3 )<p /> font\-\>flags
                     = FONT_FLAG_8BIT;<p /> else if( ( flags &amp;
                     3 ) == 2 )<p /> font\-\>flags =
                     FONT_FLAG_2BIT;<p /> else<p /> font\-\>flags
                     = FONT_FLAG_MONO;<p />
   pnFontDataSize :  optional pointer to a 32 bit value to
                     receive the size of rendered data.
   pFontData :       The render data. This data can be used to
                     recreate this font.                             */
IMAGE_PROC  SFTFont IMAGE_API IMGVER(RenderFontFileScaledEx )( CTEXTSTR file, uint32_t width, uint32_t height, PFRACTION width_scale, PFRACTION height_scale, uint32_t flags, size_t *pnFontDataSize, POINTER *pFontData );
/* <combine sack::image::RenderFontFileEx@CTEXTSTR@uint32_t@uint32_t@uint32_t@uint32_t*@POINTER *>
   \ \                                                                         */
#define RenderFontFile(file,w,h,flags) RenderFontFileScaledEx(file,w,h,NULL,NULL,flags,NULL,NULL)
#define RenderFontFileEx(file,w,h,flags,a,b) RenderFontFileScaledEx(file,w,h,NULL,NULL,flags,a,b )
		/* This can be used to get the internal description of a font,
		   which the user may then save, and use later to recreate the
		   font the same way.
		   Parameters
		   font :         SFTFont to get the render description from.
		   fontdata :     a pointer to a pointer which will be filled
		                  with a pointer buffer that has the font data.
		   fontdatalen :  a pointer to 32 bit value to receive the length
		                  of data.                                        */
		IMAGE_PROC  int IMAGE_API IMGVER(GetFontRenderData )( SFTFont font, POINTER *fontdata, size_t *fontdatalen );
// exported for the PSI font chooser to set the data for the font
// to be retreived later when only the font handle remains.
IMAGE_PROC  void IMAGE_API IMGVER(SetFontRendererData )( SFTFont font, POINTER pResult, size_t size );
#ifndef PSPRITE_METHOD
/* <combine sack::image::PSPRITE_METHOD>
   \ \                                   */
#define PSPRITE_METHOD PSPRITE_METHOD
	typedef struct sprite_method_tag *PSPRITE_METHOD;
#endif
	// provided for display rendering portion to define this method for sprites to use.
   // deliberately out of namespace... please do not move this up.
IMAGE_PROC  void IMAGE_API IMGVER(SetSavePortion )( void (CPROC*_SavePortion )( PSPRITE_METHOD psm, uint32_t x, uint32_t y, uint32_t w, uint32_t h ) );
/* \Returns the red channel of the color
   Parameters
   color :  Color to get the red channel of.
   Returns
   The COLOR_CHANNEL (byte) of the red channel in the color. */
IMAGE_PROC COLOR_CHANNEL IMAGE_API IMGVER(GetRedValue)( CDATA color ) ;
/* \Returns the green channel of the color
   Parameters
   color :  Color to get the green channel of.
   Returns
   The COLOR_CHANNEL (byte) of the green channel in the color. */
IMAGE_PROC COLOR_CHANNEL IMAGE_API IMGVER(GetGreenValue)( CDATA color );
/* \Returns the blue channel of the color
   Parameters
   color :  Color to get the blue channel of.
   Returns
   The COLOR_CHANNEL (byte) of the blue channel in the color. */
IMAGE_PROC COLOR_CHANNEL IMAGE_API IMGVER(GetBlueValue)( CDATA color );
/* \Returns the alpha channel of the color
   Parameters
   color :  Color to get the alpha channel of.
   Returns
   The COLOR_CHANNEL (byte) of the alpha channel in the color. */
IMAGE_PROC COLOR_CHANNEL IMAGE_API IMGVER(GetAlphaValue)( CDATA color );
/* Sets the red channel in a color value.
   Parameters
   color :  Original color to modify
   b :      new red channel value         */
IMAGE_PROC CDATA IMAGE_API IMGVER(SetRedValue)( CDATA color, COLOR_CHANNEL r ) ;
/* Sets the green channel in a color value.
   Parameters
   color :  Original color to modify
   g :      new green channel value         */
IMAGE_PROC CDATA IMAGE_API IMGVER(SetGreenValue)( CDATA color, COLOR_CHANNEL green );
/* Sets the blue channel in a color value.
   Parameters
   color :  Original color to modify
   b :      new blue channel value         */
IMAGE_PROC CDATA IMAGE_API IMGVER(SetBlueValue)( CDATA color, COLOR_CHANNEL b );
/* Sets the alpha channel in a color value.
   Parameters
   color :  Original color to modify
   a :      new alpha channel value         */
IMAGE_PROC CDATA IMAGE_API IMGVER(SetAlphaValue)( CDATA color, COLOR_CHANNEL a );
/* Makes a CDATA color from the RGB components. Sets the alpha
   as 100% opaque.
   Parameters
   r :      red channel of new color
   green :  green channel of new color
   b :      blue channel of new color                          */
IMAGE_PROC CDATA IMAGE_API IMGVER(MakeColor)( COLOR_CHANNEL r, COLOR_CHANNEL green, COLOR_CHANNEL b );
/* Create a CDATA color from components.
   Parameters
   r :      Red channel value
   green :  green channel value
   b :      blue channel value
   a :      alpha channel value
   Returns
   A CDATA representing the color specified. */
IMAGE_PROC CDATA IMAGE_API IMGVER(MakeAlphaColor)( COLOR_CHANNEL r, COLOR_CHANNEL green, COLOR_CHANNEL b, COLOR_CHANNEL a );
/* With 3d renderer, images have a transformation matrix. This
   function allows you to get the transformation matrix.
   Parameters
   pImage :  image to get the transformation matrix of.        */
IMAGE_PROC  PTRANSFORM IMAGE_API IMGVER(GetImageTransformation)( Image pImage );
enum image_translation_relation
{
   IMAGE_TRANSFORM_RELATIVE_CENTER = 0,
   IMAGE_TRANSFORM_RELATIVE_LEFT,
   IMAGE_TRANSFORM_RELATIVE_RIGHT,
   IMAGE_TRANSFORM_RELATIVE_TOP,
   IMAGE_TRANSFORM_RELATIVE_BOTTOM,
   IMAGE_TRANSFORM_RELATIVE_TOP_LEFT,
   IMAGE_TRANSFORM_RELATIVE_TOP_RIGHT,
   IMAGE_TRANSFORM_RELATIVE_BOTTOM_LEFT,
   IMAGE_TRANSFORM_RELATIVE_BOTTOM_RIGHT,
 // only mode that uses the 'aux' parameter of SetImageTransformRelation
   IMAGE_TRANSFORM_RELATIVE_OTHER
};
/*
 This sets flags on the image, so when it's called for rendering to the screen
 this is how
    */
IMAGE_PROC  void IMAGE_API IMGVER(SetImageTransformRelation)( Image pImage, enum image_translation_relation relation, PRCOORD aux );
/*
 This just draws the image into the current 3d context.
 This is a point-sprite engine too....
 It does not setup anything about rendering this, just generates the texture at the right coords.
 Parameters
 render_pixel_scaled : when drawing, reverse compute from the angle of the view, and the depth of the thing to scale orthagonal, but at depth.  (help 3d vision)
 */
IMAGE_PROC  void IMAGE_API IMGVER(Render3dImage)( Image pImage, PCVECTOR o, LOGICAL render_pixel_scaled );
IMAGE_PROC  void IMAGE_API IMGVER(Render3dText)( CTEXTSTR string, int characters, CDATA color, SFTFont font, PCVECTOR o, LOGICAL render_pixel_scaled );
/*
  Utilized by fonts with images with reverse_interface set to transfer child images;
  may be generally useful; but had to be exposed through interface
  Might be a shallow move....
 */
IMAGE_PROC  void IMAGE_API IMGVER(TransferSubImages)( Image pImageTo, Image pImageFrom );
IMAGE_PROC  LOGICAL IMAGE_API IMGVER(IsImageTargetFinal)( Image image );
/* These flags are used in SetImageRotation and RotateImageAbout
   functions - these are part of the 3D driver interface
   extension. They allow for controlling how the rotation is
   performed.                                                    */
enum image_rotation_flags {
 // relative to center of image (center if not left, right, top or bottom )
	IMAGE_ROTATE_FLAG_CENTER = 0,
 // relative to top edge (center if not left or right)
   IMAGE_ROTATE_FLAG_TOP,
 // relative to left edge (center if not top or bottom)
   IMAGE_ROTATE_FLAG_LEFT,
 // relative to right edge (center if not top or bottom)
   IMAGE_ROTATE_FLAG_RIGHT,
 // relative to bottom edge (center if not left or right )
   IMAGE_ROTATE_FLAG_BOTTOM,
 // use the offset relative to the image orientation
	IMAGE_ROTATE_FLAG_ADD_CUSTOM_OFFSET
};
/* Sets the rotation matrix of an image to an arbitrary
   yaw/pitch/roll coordinate.
   Parameters
   pImage :     Image to rotate
   edge_flag :  what edge the rotation is relative to
   offset_x :   offset from the edge to get the center
   offset_y :   offset from the edge to get the center
   rx :         rotation about x axis (horizontal)
   ry :         rotation about y axis (vertical)
   rz :         rotation about z axis (into screen)     */
IMAGE_PROC void IMAGE_API IMGVER(SetImageRotation)( Image pImage, int edge_flag, RCOORD offset_x, RCOORD offset_y, RCOORD rx, RCOORD ry, RCOORD rz );
/* Allows arbitrary rotation of an image in 3d render mode.
   Parameters
   pImage :     image to rotate
   edge_flag :  see enum image_rotation_flags
   offset_x :   offset from top left of image to center the
                rotation
   offset_y :   offset from top left of image to center the
                rotation
   vAxis :      axis to rotate around, can be any arbitrary
                direction
   angle :      angle of rotation around the axis.
   Remarks
   \See Also <link sack::image::image_rotation_flags, image_rotation_flags Enumeration> */
IMAGE_PROC void IMAGE_API IMGVER(RotateImageAbout)( Image pImage, int edge_flag, RCOORD offset_x, RCOORD offset_y, PVECTOR vAxis, RCOORD angle );
IMAGE_PROC void IMAGE_API IMGVER(MarkImageDirty)( Image pImage );
_INTERFACE_NAMESPACE
/* Defines a pointer member of the interface structure. */
#define IMAGE_PROC_PTR(type,name) type (CPROC*_##name)
/* Macro to build function pointer entries in the image
   interface.                                           */
//#define DIMAGE_PROC_PTR(type,name) type (CPROC**_##name)
/* This defines the interface call table. each function
   available in the API is reflected in this interface. It
   provdes a function table so applications don't have to be
   directly linked to the image API. This allows replacing the
   image API.                                                  */
typedef struct image_interface_tag
{
/* <combine sack::image::SetStringBehavior@Image@uint32_t>
   Internal
   Interface index 4                                  */
 IMAGE_PROC_PTR( void, SetStringBehavior) ( Image pImage, uint32_t behavior );
/* <combine sack::image::SetBlotMethod@uint32_t>
   \ \
   Internal
   Interface index 5                        */
 IMAGE_PROC_PTR( void, SetBlotMethod)     ( uint32_t method );
/*
   Internal
   Interface index 6*/
   IMAGE_PROC_PTR( Image,BuildImageFileEx) ( PCOLOR pc, uint32_t width, uint32_t height DBG_PASS);
/* <combine sack::image::MakeImageFileEx@uint32_t@uint32_t Height>
   Internal
   Interface index 7*/
  IMAGE_PROC_PTR( Image,MakeImageFileEx)  (uint32_t Width, uint32_t Height DBG_PASS);
/* <combine sack::image::MakeSubImageEx@Image@int32_t@int32_t@uint32_t@uint32_t height>
   Internal
   Interface index 8                                                                    */
   IMAGE_PROC_PTR( Image,MakeSubImageEx)   ( Image pImage, int32_t x, int32_t y, uint32_t width, uint32_t height DBG_PASS );
/* <combine sack::image::RemakeImageEx@Image@PCOLOR@uint32_t@uint32_t height>
   \ \
   <b>Internal</b>
   Interface index 9                                                */
   IMAGE_PROC_PTR( Image,RemakeImageEx)    ( Image pImage, PCOLOR pc, uint32_t width, uint32_t height DBG_PASS);
/* <combine sack::image::LoadImageFileEx@CTEXTSTR name>
   Internal
   Interface index 10                                                   */
  IMAGE_PROC_PTR( Image,LoadImageFileEx)  ( CTEXTSTR name DBG_PASS );
/* <combine sack::image::UnmakeImageFileEx@Image pif>
   Internal
   Interface index 11                                                 */
  IMAGE_PROC_PTR( void,UnmakeImageFileEx) ( Image pif DBG_PASS );
//-----------------------------------------------------
/* <combine sack::image::ResizeImageEx@Image@int32_t@int32_t height>
   Internal
   Interface index 14                                                          */
  IMAGE_PROC_PTR( void,ResizeImageEx)     ( Image pImage, int32_t width, int32_t height DBG_PASS);
/* <combine sack::image::MoveImage@Image@int32_t@int32_t>
   Internal
   Interface index 15                                               */
   IMAGE_PROC_PTR( void,MoveImage)         ( Image pImage, int32_t x, int32_t y );
//-----------------------------------------------------
/* <combine sack::image::BlatColor@Image@int32_t@int32_t@uint32_t@uint32_t@CDATA>
   Internal
   Interface index 16                                                             */
   IMAGE_PROC_PTR( void,BlatColor)     ( Image pifDest, int32_t x, int32_t y, uint32_t w, uint32_t h, CDATA color );
/* <combine sack::image::BlatColorAlpha@Image@int32_t@int32_t@uint32_t@uint32_t@CDATA>
   Internal
   Interface index 17                                                                  */
   IMAGE_PROC_PTR( void,BlatColorAlpha)( Image pifDest, int32_t x, int32_t y, uint32_t w, uint32_t h, CDATA color );
/* <combine sack::image::BlotImageEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@...>
   Internal
	Interface index 18*/
   IMAGE_PROC_PTR( void,BlotImageEx)     ( Image pDest, Image pIF, int32_t x, int32_t y, uint32_t nTransparent, uint32_t method, ... );
 /* <combine sack::image::BlotImageSizedEx@Image@Image@int32_t@int32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   Internal
	Interface index 19*/
   IMAGE_PROC_PTR( void,BlotImageSizedEx)( Image pDest, Image pIF, int32_t x, int32_t y, int32_t xs, int32_t ys, uint32_t wd, uint32_t ht, uint32_t nTransparent, uint32_t method, ... );
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
  Internal
   Interface index  20                                                                                                        */
   IMAGE_PROC_PTR( void,BlotScaledImageSizedEx)( Image pifDest, Image pifSrc
                                   , int32_t xd, int32_t yd
                                   , uint32_t wd, uint32_t hd
                                   , int32_t xs, int32_t ys
                                   , uint32_t ws, uint32_t hs
                                   , uint32_t nTransparent
                                   , uint32_t method, ... );
/*Internal
   Interface index 21*/
   IMAGE_PROC_PTR( void,plot)      ( Image pi, int32_t x, int32_t y, CDATA c );
/*Internal
   Interface index 22*/
   IMAGE_PROC_PTR( void,plotalpha) ( Image pi, int32_t x, int32_t y, CDATA c );
/*Internal
   Interface index 23*/
   IMAGE_PROC_PTR( CDATA,getpixel) ( Image pi, int32_t x, int32_t y );
/*Internal
   Interface index 24*/
  // d is color data...
   IMAGE_PROC_PTR( void,do_line)     ( Image pBuffer, int32_t x, int32_t y, int32_t xto, int32_t yto, CDATA color );
/*Internal
   Interface index 25*/
  // d is color data...
   IMAGE_PROC_PTR( void,do_lineAlpha)( Image pBuffer, int32_t x, int32_t y, int32_t xto, int32_t yto, CDATA color);
/*Internal
   Interface index 26*/
   IMAGE_PROC_PTR( void,do_hline)     ( Image pImage, int32_t y, int32_t xfrom, int32_t xto, CDATA color );
/*Internal
   Interface index 27*/
   IMAGE_PROC_PTR( void,do_vline)     ( Image pImage, int32_t x, int32_t yfrom, int32_t yto, CDATA color );
/*Internal
   Interface index 28*/
   IMAGE_PROC_PTR( void,do_hlineAlpha)( Image pImage, int32_t y, int32_t xfrom, int32_t xto, CDATA color );
/*Internal
   Interface index 29*/
   IMAGE_PROC_PTR( void,do_vlineAlpha)( Image pImage, int32_t x, int32_t yfrom, int32_t yto, CDATA color );
/* <combine sack::image::GetDefaultFont>
   Internal
   Interface index 30                    */
   IMAGE_PROC_PTR( SFTFont,GetDefaultFont) ( void );
/* <combine sack::image::GetFontHeight@SFTFont>
   Internal
   Interface index 31                                        */
   IMAGE_PROC_PTR( uint32_t ,GetFontHeight)  ( SFTFont );
/* <combine sack::image::GetStringSizeFontEx@CTEXTSTR@size_t@uint32_t *@uint32_t *@SFTFont>
   Internal
   Interface index 32                                                          */
   IMAGE_PROC_PTR( uint32_t ,GetStringSizeFontEx)( CTEXTSTR pString, size_t len, uint32_t *width, uint32_t *height, SFTFont UseFont );
/* <combine sack::image::PutCharacterFont@Image@int32_t@int32_t@CDATA@CDATA@uint32_t@SFTFont>
   Internal
   Interface index 33                                                           */
   IMAGE_PROC_PTR( void,PutCharacterFont)              ( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, TEXTCHAR c, SFTFont font );
/* <combine sack::image::PutCharacterVerticalFont@Image@int32_t@int32_t@CDATA@CDATA@TEXTCHAR@SFTFont>
   Internal
   Interface index 34                                                                                        */
   IMAGE_PROC_PTR( void,PutCharacterVerticalFont)      ( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, TEXTCHAR c, SFTFont font );
/* <combine sack::image::PutCharacterInvertFont@Image@int32_t@int32_t@CDATA@CDATA@TEXTCHAR@SFTFont>
   Internal
   Interface index 35                                                                                      */
   IMAGE_PROC_PTR( void,PutCharacterInvertFont)        ( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, TEXTCHAR c, SFTFont font );
/* <combine sack::image::PutCharacterVerticalInvertFont@Image@int32_t@int32_t@CDATA@CDATA@TEXTCHAR@SFTFont>
   Internal
   Interface index 36                                                                                              */
   IMAGE_PROC_PTR( void,PutCharacterVerticalInvertFont)( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, TEXTCHAR c, SFTFont font );
/* <combine sack::image::PutStringFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   Internal
   Interface index 37                                                                                   */
   IMAGE_PROC_PTR( void,PutStringFontEx)              ( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, CTEXTSTR pc, size_t nLen, SFTFont font );
/* <combine sack::image::PutStringVerticalFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   Internal
   Interface index 38                                                                                           */
   IMAGE_PROC_PTR( void,PutStringVerticalFontEx)      ( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, CTEXTSTR pc, size_t nLen, SFTFont font );
/* <combine sack::image::PutStringInvertFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   Internal
   Interface index 39                                                                                         */
   IMAGE_PROC_PTR( void,PutStringInvertFontEx)        ( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, CTEXTSTR pc, size_t nLen, SFTFont font );
/* <combine sack::image::PutStringInvertVerticalFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   Internal
   Interface index 40                                                                                                 */
   IMAGE_PROC_PTR( void,PutStringInvertVerticalFontEx)( Image pImage, int32_t x, int32_t y, CDATA color, CDATA background, CTEXTSTR pc, size_t nLen, SFTFont font );
/* <combine sack::image::GetMaxStringLengthFont@uint32_t@SFTFont>
   Internal
   Interface index 41                                     */
   IMAGE_PROC_PTR( uint32_t, GetMaxStringLengthFont )( uint32_t width, SFTFont UseFont );
/* <combine sack::image::GetImageSize@Image@uint32_t *@uint32_t *>
   Internal
   Interface index 42                                                    */
   IMAGE_PROC_PTR( void, GetImageSize)                ( Image pImage, uint32_t *width, uint32_t *height );
/* <combine sack::image::LoadFont@SFTFont>
   Internal
   Interface index 43                                   */
   IMAGE_PROC_PTR( SFTFont, LoadFont )                   ( SFTFont font );
         /* <combine sack::image::UnloadFont@SFTFont>
            \ \                                    */
         IMAGE_PROC_PTR( void, UnloadFont )                 ( SFTFont font );
/* Internal
   Interface index 44
   This is used by internal methods to transfer image and font
   data to the render agent.                                   */
   IMAGE_PROC_PTR( DataState, BeginTransferData )    ( uint32_t total_size, uint32_t segsize, CDATA data );
/* Internal
   Interface index 45
   Used internally to transfer data to render agent. */
   IMAGE_PROC_PTR( void, ContinueTransferData )      ( DataState state, uint32_t segsize, CDATA data );
/* Internal
   Interface index 46
   Command issues at end of data transfer to decode the data
   into an image.                                            */
   IMAGE_PROC_PTR( Image, DecodeTransferredImage )    ( DataState state );
/* After a data transfer decode the information as a font.
   Internal
   Interface index 47                                      */
   IMAGE_PROC_PTR( SFTFont, AcceptTransferredFont )     ( DataState state );
/*Internal
   Interface index 48*/
   IMAGE_PROC_PTR( CDATA, ColorAverage )( CDATA c1, CDATA c2
                                              , int d, int max );
/* <combine sack::image::SyncImage>
   Internal
   Interface index 49               */
   IMAGE_PROC_PTR( void, SyncImage )                 ( void );
         /* <combine sack::image::GetImageSurface@Image>
            \ \                                          */
         IMAGE_PROC_PTR( PCDATA, GetImageSurface )       ( Image pImage );
         /* <combine sack::image::IntersectRectangle@IMAGE_RECTANGLE *@IMAGE_RECTANGLE *@IMAGE_RECTANGLE *>
            \ \                                                                                             */
         IMAGE_PROC_PTR( int, IntersectRectangle )      ( IMAGE_RECTANGLE *r, IMAGE_RECTANGLE *r1, IMAGE_RECTANGLE *r2 );
   /* <combine sack::image::MergeRectangle@IMAGE_RECTANGLE *@IMAGE_RECTANGLE *@IMAGE_RECTANGLE *>
      \ \                                                                                         */
   IMAGE_PROC_PTR( int, MergeRectangle )( IMAGE_RECTANGLE *r, IMAGE_RECTANGLE *r1, IMAGE_RECTANGLE *r2 );
   /* <combine sack::image::GetImageAuxRect@Image@P_IMAGE_RECTANGLE>
      \ \                                                            */
   IMAGE_PROC_PTR( void, GetImageAuxRect )   ( Image pImage, P_IMAGE_RECTANGLE pRect );
   /* <combine sack::image::SetImageAuxRect@Image@P_IMAGE_RECTANGLE>
      \ \                                                            */
   IMAGE_PROC_PTR( void, SetImageAuxRect )   ( Image pImage, P_IMAGE_RECTANGLE pRect );
   /* <combine sack::image::OrphanSubImage@Image>
      \ \                                         */
   IMAGE_PROC_PTR( void, OrphanSubImage )  ( Image pImage );
   /* <combine sack::image::AdoptSubImage@Image@Image>
      \ \                                              */
   IMAGE_PROC_PTR( void, AdoptSubImage )   ( Image pFoster, Image pOrphan );
	/* <combine sack::image::MakeSpriteImageFileEx@CTEXTSTR fname>
	   \ \                                                         */
	IMAGE_PROC_PTR( PSPRITE, MakeSpriteImageFileEx )( CTEXTSTR fname DBG_PASS );
	/* <combine sack::image::MakeSpriteImageEx@Image image>
	   \ \                                                  */
	IMAGE_PROC_PTR( PSPRITE, MakeSpriteImageEx )( Image image DBG_PASS );
	/* <combine sack::image::rotate_scaled_sprite@Image@PSPRITE@fixed@fixed@fixed>
	   \ \                                                                         */
	IMAGE_PROC_PTR( void   , rotate_scaled_sprite )(Image bmp, PSPRITE sprite, fixed angle, fixed scale_width, fixed scale_height);
	/* <combine sack::image::rotate_sprite@Image@PSPRITE@fixed>
	   \ \                                                      */
	IMAGE_PROC_PTR( void   , rotate_sprite )(Image bmp, PSPRITE sprite, fixed angle);
 /* <combine sack::image::BlotSprite@Image@PSPRITE>
	 Internal
   Interface index 61                                              */
		IMAGE_PROC_PTR( void   , BlotSprite )( Image pdest, PSPRITE ps );
    /* <combine sack::image::DecodeMemoryToImage@uint8_t*@uint32_t>
       \ \                                                */
    IMAGE_PROC_PTR( Image, DecodeMemoryToImage )( uint8_t* buf, size_t size );
   /* <combine sack::image::InternalRenderFontFile@CTEXTSTR@int32_t@int32_t@uint32_t>
      \returns a SFTFont                                                      */
	IMAGE_PROC_PTR( SFTFont, InternalRenderFontFile )( CTEXTSTR file
																 , int32_t nWidth
																 , int32_t nHeight
																		, PFRACTION width_scale
																		, PFRACTION height_scale
																 , uint32_t flags
																 );
   /* <combine sack::image::InternalRenderFont@uint32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t>
      requires knowing the font cache....                                 */
	IMAGE_PROC_PTR( SFTFont, InternalRenderFont )( uint32_t nFamily
															, uint32_t nStyle
															, uint32_t nFile
															, int32_t nWidth
															, int32_t nHeight
																		, PFRACTION width_scale
																		, PFRACTION height_scale
															, uint32_t flags
															);
/* <combine sack::image::RenderScaledFontData@PFONTDATA@PFRACTION@PFRACTION>
   \ \                                                                       */
IMAGE_PROC_PTR( SFTFont, RenderScaledFontData)( PFONTDATA pfd, PFRACTION width_scale, PFRACTION height_scale );
/* <combine sack::image::RenderFontFileEx@CTEXTSTR@uint32_t@uint32_t@uint32_t@uint32_t*@POINTER *>
   \ \                                                                         */
IMAGE_PROC_PTR( SFTFont, RenderFontFileScaledEx )( CTEXTSTR file, uint32_t width, uint32_t height, PFRACTION width_scale, PFRACTION height_scale, uint32_t flags, size_t *size, POINTER *pFontData );
/* <combine sack::image::DestroyFont@SFTFont *>
   \ \                                       */
IMAGE_PROC_PTR( void, DestroyFont)( SFTFont *font );
/* <combine sack::image::GetGlobalFonts>
   global_font_data in interface is really a global font data. Don't
   have to call GetGlobalFont to get this.                           */
struct font_global_tag *_global_font_data;
/* <combine sack::image::GetFontRenderData@SFTFont@POINTER *@uint32_t *>
   \ \                                                           */
IMAGE_PROC_PTR( int, GetFontRenderData )( SFTFont font, POINTER *fontdata, size_t *fontdatalen );
/* <combine sack::image::SetFontRendererData@SFTFont@POINTER@uint32_t>
   \ \                                                         */
IMAGE_PROC_PTR( void, SetFontRendererData )( SFTFont font, POINTER pResult, size_t size );
/* <combine sack::image::SetSpriteHotspot@PSPRITE@int32_t@int32_t>
   \ \                                                       */
IMAGE_PROC_PTR( PSPRITE, SetSpriteHotspot )( PSPRITE sprite, int32_t x, int32_t y );
/* <combine sack::image::SetSpritePosition@PSPRITE@int32_t@int32_t>
   \ \                                                        */
IMAGE_PROC_PTR( PSPRITE, SetSpritePosition )( PSPRITE sprite, int32_t x, int32_t y );
	/* <combine sack::image::UnmakeImageFileEx@Image pif>
	   \ \                                                */
	IMAGE_PROC_PTR( void, UnmakeSprite )( PSPRITE sprite, int bForceImageAlso );
/* <combine sack::image::GetGlobalFonts>
   \ \                                   */
IMAGE_PROC_PTR( struct font_global_tag *, GetGlobalFonts)( void );
/* <combinewith sack::image::GetStringRenderSizeFontEx@CTEXTSTR@uint32_t@uint32_t *@uint32_t *@uint32_t *@SFTFont, sack::image::GetStringRenderSizeFontEx@CTEXTSTR@size_t@uint32_t *@uint32_t *@uint32_t *@SFTFont>
   \ \                                                                                                                                                                     */
IMAGE_PROC_PTR( uint32_t, GetStringRenderSizeFontEx )( CTEXTSTR pString, size_t nLen, uint32_t *width, uint32_t *height, uint32_t *charheight, SFTFont UseFont );
IMAGE_PROC_PTR( Image, LoadImageFileFromGroupEx )( INDEX group, CTEXTSTR filename DBG_PASS );
IMAGE_PROC_PTR( SFTFont, RenderScaledFont )( CTEXTSTR name, uint32_t width, uint32_t height, PFRACTION width_scale, PFRACTION height_scale, uint32_t flags );
IMAGE_PROC_PTR( SFTFont, RenderScaledFontEx )( CTEXTSTR name, uint32_t width, uint32_t height, PFRACTION width_scale, PFRACTION height_scale, uint32_t flags, size_t *pnFontDataSize, POINTER *pFontData );
IMAGE_PROC_PTR( COLOR_CHANNEL, GetRedValue )( CDATA color ) ;
IMAGE_PROC_PTR( COLOR_CHANNEL, GetGreenValue )( CDATA color );
IMAGE_PROC_PTR( COLOR_CHANNEL, GetBlueValue )( CDATA color );
IMAGE_PROC_PTR( COLOR_CHANNEL, GetAlphaValue )( CDATA color );
IMAGE_PROC_PTR( CDATA, SetRedValue )( CDATA color, COLOR_CHANNEL r ) ;
IMAGE_PROC_PTR( CDATA, SetGreenValue )( CDATA color, COLOR_CHANNEL green );
IMAGE_PROC_PTR( CDATA, SetBlueValue )( CDATA color, COLOR_CHANNEL b );
IMAGE_PROC_PTR( CDATA, SetAlphaValue )( CDATA color, COLOR_CHANNEL a );
IMAGE_PROC_PTR( CDATA, MakeColor )( COLOR_CHANNEL r, COLOR_CHANNEL green, COLOR_CHANNEL b );
IMAGE_PROC_PTR( CDATA, MakeAlphaColor )( COLOR_CHANNEL r, COLOR_CHANNEL green, COLOR_CHANNEL b, COLOR_CHANNEL a );
IMAGE_PROC_PTR( PTRANSFORM, GetImageTransformation )( Image pImage );
IMAGE_PROC_PTR( void, SetImageRotation )( Image pImage, int edge_flag, RCOORD offset_x, RCOORD offset_y, RCOORD rx, RCOORD ry, RCOORD rz );
IMAGE_PROC_PTR( void, RotateImageAbout )( Image pImage, int edge_flag, RCOORD offset_x, RCOORD offset_y, PVECTOR vAxis, RCOORD angle );
IMAGE_PROC_PTR( void, MarkImageDirty )( Image pImage );
IMAGE_PROC_PTR( void, DumpFontCache )( void );
IMAGE_PROC_PTR( void, RerenderFont )( SFTFont font, int32_t width, int32_t height, PFRACTION width_scale, PFRACTION height_scale );
// option(1) == use GL_RGBA_EXT; option(2)==clamp; option(4)==repeat
IMAGE_PROC_PTR( int, ReloadTexture )( Image child_image, int option );
// option(1) == use GL_RGBA_EXT; option(2)==clamp; option(4)==repeat
IMAGE_PROC_PTR( int, ReloadShadedTexture )( Image child_image, int option, CDATA color );
// option(1) == use GL_RGBA_EXT; option(2)==clamp; option(4)==repeat
IMAGE_PROC_PTR( int, ReloadMultiShadedTexture )( Image child_image, int option, CDATA red, CDATA green, CDATA blue );
IMAGE_PROC_PTR( void, SetImageTransformRelation )( Image pImage, enum image_translation_relation relation, PRCOORD aux );
IMAGE_PROC_PTR( void, Render3dImage )( Image pImage, PCVECTOR o, LOGICAL render_pixel_scaled );
IMAGE_PROC_PTR( void, DumpFontFile )( CTEXTSTR name, SFTFont font_to_dump );
IMAGE_PROC_PTR( void, Render3dText )( CTEXTSTR string, int characters, CDATA color, SFTFont font, PCVECTOR o, LOGICAL render_pixel_scaled );
// transfer all sub images to new image using appropriate methods
// extension for internal fonts to be utilized by external plugins...
IMAGE_PROC_PTR( void, TransferSubImages )( Image pImageTo, Image pImageFrom );
// when using reverse interfaces, need a way to get the real image
// from the fake image (proxy image)
IMAGE_PROC_PTR( Image, GetNativeImage )( Image pImageTo );
// low level support for proxy; this exposes some image_common.c routines
IMAGE_PROC_PTR( Image, GetTintedImage )( Image child_image, CDATA color );
IMAGE_PROC_PTR( Image, GetShadedImage )( Image child_image, CDATA red, CDATA green, CDATA blue );
// test for IF_FLAG_FINAL_RENDER (non physical surface/prevent local copy-restore)
IMAGE_PROC_PTR( LOGICAL, IsImageTargetFinal )( Image image );
// use image data to create a clone of the image for the new application instance...
// this is used when a common image resource is used for all application instances
// it should be triggered during onconnect.
// it is a new image instance that should be used for future app references...
IMAGE_PROC_PTR( Image, ReuseImage )( Image image );
IMAGE_PROC_PTR( void, PutStringFontExx )( Image pImage
											 , int32_t x, int32_t y
											 , CDATA color, CDATA background
											 , CTEXTSTR pc, size_t nLen, SFTFont font, int justification, uint32_t _width );
// sometimes it's not possible to use blatcolor to clear an imate...
// sometimes its parent is not redrawn?
IMAGE_PROC_PTR( void, ResetImageBuffers )( Image image, LOGICAL image_only );
	IMAGE_PROC_PTR(  LOGICAL, PngImageFile )( Image image, uint8_t* *buf, size_t *size );
	IMAGE_PROC_PTR(  LOGICAL, JpgImageFile )( Image image, uint8_t* *buf, size_t *size, int Q );
	IMAGE_PROC_PTR(  void, SetFontBias )( SFTFont font, int32_t x, int32_t y );
	IMAGE_PROC_PTR( SlicedImage, MakeSlicedImage )( Image source, uint32_t left, uint32_t right, uint32_t top, uint32_t bottom, LOGICAL output_center );
	IMAGE_PROC_PTR( SlicedImage, MakeSlicedImageComplex )( Image source
										, uint32_t top_left_x, uint32_t top_left_y, uint32_t top_left_width, uint32_t top_left_height
										, uint32_t top_x, uint32_t top_y, uint32_t top_width, uint32_t top_height
										, uint32_t top_right_x, uint32_t top_right_y, uint32_t top_right_width, uint32_t top_right_height
										, uint32_t left_x, uint32_t left_y, uint32_t left_width, uint32_t left_height
										, uint32_t center_x, uint32_t center_y, uint32_t center_width, uint32_t center_height
										, uint32_t right_x, uint32_t right_y, uint32_t right_width, uint32_t right_height
										, uint32_t bottom_left_x, uint32_t bottom_left_y, uint32_t bottom_left_width, uint32_t bottom_left_height
										, uint32_t bottom_x, uint32_t bottom_y, uint32_t bottom_width, uint32_t bottom_height
										, uint32_t bottom_right_x, uint32_t bottom_right_y, uint32_t bottom_right_width, uint32_t bottom_right_height
										, LOGICAL output_center );
	IMAGE_PROC_PTR( void, UnmakeSlicedImage )( SlicedImage image );
	IMAGE_PROC_PTR( void, BlotSlicedImageEx )( Image dest, SlicedImage source, int32_t x, int32_t y, uint32_t width, uint32_t height, int alpha, enum BlotOperation op, ... );
	IMAGE_PROC_PTR( void, SetSavePortion )( void (CPROC*_SavePortion )( PSPRITE_METHOD psm, uint32_t x, uint32_t y, uint32_t w, uint32_t h ) );
} IMAGE_INTERFACE, *PIMAGE_INTERFACE;
/* Method to define automatic name translation from standard
   names Like BlatColorAlphaEx to the interface the user has
   specified to be using.                                    */
#define PROC_ALIAS(name) ((USE_IMAGE_INTERFACE)->_##name)
/* Method to define automatic name translation from standard
   names Like BlatColorAlphaEx to the interface the user has
   specified to be using. For function pointers.             */
#define PPROC_ALIAS(name) (*(USE_IMAGE_INTERFACE)->_##name)
#ifdef DEFINE_DEFAULT_IMAGE_INTERFACE
//static PIMAGE_INTERFACE always_defined_interface_that_makes_this_efficient;
#  define USE_IMAGE_INTERFACE GetImageInterface()
#endif
#if defined( FORCE_NO_INTERFACE ) && !defined( ALLOW_IMAGE_INTERFACES )
#  undef USE_IMAGE_INTERFACE
#else
#  define GetImageInterface() (PIMAGE_INTERFACE)GetInterface( "image" )
/* <combine sack::image::DropImageInterface@PIMAGE_INTERFACE>
   \ \                                                        */
#  define DropImageInterface(x) DropInterface( "image", NULL )
#endif
#ifdef USE_IMAGE_INTERFACE
#define GetRedValue                          PROC_ALIAS(GetRedValue )
#define GetBlueValue                          PROC_ALIAS(GetBlueValue )
#define GetGreenValue                          PROC_ALIAS(GetGreenValue )
#define GetAlphaValue                          PROC_ALIAS(GetAlphaValue )
#define SetRedValue                          PROC_ALIAS(SetRedValue )
#define SetBlueValue                          PROC_ALIAS(SetBlueValue )
#define SetGreenValue                          PROC_ALIAS(SetGreenValue )
#define SetAlphaValue                          PROC_ALIAS(SetAlphaValue )
#define MakeColor                          PROC_ALIAS(MakeColor )
#define MakeAlphaColor                          PROC_ALIAS(MakeAlphaColor )
#define MarkImageDirty                    PROC_ALIAS(MarkImageDirty)
#define GetStringRenderSizeFontEx          PROC_ALIAS(GetStringRenderSizeFontEx )
#define LoadImageFileFromGroupEx          PROC_ALIAS(LoadImageFileFromGroupEx )
#define SetStringBehavior                  PROC_ALIAS(SetStringBehavior )
                      //PROC_ALIAS(SetBlotMethod )
#define SetBlotMethod
#define BuildImageFileEx                   PROC_ALIAS(BuildImageFileEx )
#define MakeImageFileEx                    PROC_ALIAS(MakeImageFileEx )
/* <combine sack::image::MakeImageFileEx@uint32_t@uint32_t>
   \ \                                                   */
#define MakeImageFile(w,h)                 PROC_ALIAS(MakeImageFileEx)( w,h DBG_SRC )
#define MakeSubImageEx                     PROC_ALIAS(MakeSubImageEx )
/* <combine sack::image::MakeSubImageEx@Image@int32_t@int32_t@uint32_t@uint32_t>
   \ \                                                                  */
#define MakeSubImage( image, x, y, w, h )  PROC_ALIAS(MakeSubImageEx)( image, x, y, w, h DBG_SRC )
#define RemakeImageEx                      PROC_ALIAS(RemakeImageEx )
#define ResizeImageEx                      PROC_ALIAS(ResizeImageEx )
#define MoveImage                          PROC_ALIAS(MoveImage )
#define LoadImageFileEx                    PROC_ALIAS(LoadImageFileEx )
#define DecodeMemoryToImage                PROC_ALIAS(DecodeMemoryToImage )
/* <combine sack::image::UnmakeImageFileEx@Image pif>
   Destroys an image. Does not automatically destroy child
   images created on the image.
   Parameters
   Image :  an image to destroy
   Example
   <code lang="c++">
   Image image = MakeImageFile( 100, 100 );
   UnmakeImageFile( image );
   </code>                                                 */
#define UnmakeImageFile(pif)               PROC_ALIAS(UnmakeImageFileEx )( pif DBG_SRC )
#define UnmakeImageFileEx                  PROC_ALIAS(UnmakeImageFileEx )
#define BlatColor                          PROC_ALIAS(BlatColor )
#define BlatColorAlpha                     PROC_ALIAS(BlatColorAlpha )
#define BlotImageSizedEx                   PROC_ALIAS(BlotImageSizedEx )
#define BlotImageEx                        PROC_ALIAS(BlotImageEx )
#define BlotScaledImageSizedEx             PROC_ALIAS(BlotScaledImageSizedEx )
#define plot                               PPROC_ALIAS(plot )
#define plotalpha                          PPROC_ALIAS(plotalpha )
#define getpixel                           PPROC_ALIAS(getpixel )
#define do_line                            PPROC_ALIAS(do_line )
#define do_lineAlpha                       PPROC_ALIAS(do_lineAlpha )
#define do_hline                           PPROC_ALIAS(do_hline )
#define do_vline                           PPROC_ALIAS(do_vline )
#define do_hlineAlpha                      PPROC_ALIAS(do_hlineAlpha )
#define do_vlineAlpha                      PPROC_ALIAS(do_vlineAlpha )
#define GetDefaultFont                     PROC_ALIAS(GetDefaultFont )
#define GetFontHeight                      PROC_ALIAS(GetFontHeight )
#define GetStringSizeFontEx                PROC_ALIAS(GetStringSizeFontEx )
#define PutCharacterFont                   PROC_ALIAS(PutCharacterFont )
#define PutCharacterVerticalFont           PROC_ALIAS(PutCharacterVerticalFont )
#define PutCharacterInvertFont             PROC_ALIAS(PutCharacterInvertFont )
#define PutCharacterVerticalInvertFont     PROC_ALIAS(PutCharacterVerticalInvertFont )
#define PutStringFontExx                   PROC_ALIAS(PutStringFontExx)
#define PutStringFontEx                    PROC_ALIAS(PutStringFontEx )
#define PutStringVerticalFontEx            PROC_ALIAS(PutStringVerticalFontEx )
#define PutStringInvertFontEx              PROC_ALIAS(PutStringInvertFontEx )
#define PutStringInvertVerticalFontEx      PROC_ALIAS(PutStringInvertVerticalFontEx )
#define GetMaxStringLengthFont             PROC_ALIAS(GetMaxStringLengthFont )
#define GetImageSize                       PROC_ALIAS(GetImageSize )
#define LoadFont                           PROC_ALIAS(LoadFont )
#define UnloadFont                         PROC_ALIAS(UnloadFont )
#define ColorAverage                       PPROC_ALIAS(ColorAverage)
#define TransferSubImages                  PROC_ALIAS(TransferSubImages)
#define SyncImage                          PROC_ALIAS(SyncImage )
#define IntersectRectangle                 PROC_ALIAS(IntersectRectangle)
#define MergeRectangle                     PROC_ALIAS(MergeRectangle)
#define GetImageSurface                    PROC_ALIAS(GetImageSurface)
#define SetImageAuxRect                    PROC_ALIAS(SetImageAuxRect)
#define GetImageAuxRect                    PROC_ALIAS(GetImageAuxRect)
#define OrphanSubImage                     PROC_ALIAS(OrphanSubImage)
#define GetGlobalFonts                     PROC_ALIAS(GetGlobalFonts)
#define GetTintedImage                     PROC_ALIAS(GetTintedImage)
#define GetShadedImage                     PROC_ALIAS(GetShadedImage)
#define AdoptSubImage                      PROC_ALIAS(AdoptSubImage)
#define MakeSpriteImageFileEx   PROC_ALIAS(MakeSpriteImageFileEx)
#define MakeSpriteImageEx       PROC_ALIAS(MakeSpriteImageEx)
#define UnmakeSprite            PROC_ALIAS(UnmakeSprite )
#define rotate_scaled_sprite    PROC_ALIAS(rotate_scaled_sprite)
#define rotate_sprite           PROC_ALIAS(rotate_sprite)
#define BlotSprite              PROC_ALIAS(BlotSprite)
#define SetSpritePosition  PROC_ALIAS(  SetSpritePosition )
#define SetSpriteHotspot  PROC_ALIAS(  SetSpriteHotspot )
#define InternalRenderFont          PROC_ALIAS(InternalRenderFont)
#define InternalRenderFontFile      PROC_ALIAS(InternalRenderFontFile)
#define RenderScaledFontData              PROC_ALIAS(RenderScaledFontData)
//#define RenderScaledFont              PROC_ALIAS(RenderScaledFont)
#define RenderScaledFontEx              PROC_ALIAS(RenderScaledFontEx)
#define DumpFontCache              PROC_ALIAS(DumpFontCache)
#define RerenderFont              PROC_ALIAS(RerenderFont)
// option(1) == use GL_RGBA_EXT; option(2)==clamp; option(4)==repeat
#define ReloadTexture              PROC_ALIAS(ReloadTexture)
// option(1) == use GL_RGBA_EXT; option(2)==clamp; option(4)==repeat
#define ReloadShadedTexture              PROC_ALIAS(ReloadShadedTexture)
// option(1) == use GL_RGBA_EXT; option(2)==clamp; option(4)==repeat
#define ReloadMultiShadedTexture              PROC_ALIAS(ReloadMultiShadedTexture)
#define DestroyFont              PROC_ALIAS(DestroyFont)
#define GetFontRenderData              PROC_ALIAS(GetFontRenderData)
#define SetFontRendererData              PROC_ALIAS(SetFontRendererData)
#define RenderFontFileScaledEx              PROC_ALIAS(RenderFontFileScaledEx)
#define GetImageTransformation              PROC_ALIAS(GetImageTransformation)
#define SetImageTransformRelation      PROC_ALIAS( SetImageTransformRelation )
#define Render3dImage                  PROC_ALIAS( Render3dImage )
#define Render3dText                   PROC_ALIAS( Render3dText )
#define DumpFontFile                   PROC_ALIAS( DumpFontFile )
#define IsImageTargetFinal                   PROC_ALIAS( IsImageTargetFinal )
#define ReuseImage                      if((USE_IMAGE_INTERFACE)->_ReuseImage) PROC_ALIAS( ReuseImage )
#define ResetImageBuffers                      if((USE_IMAGE_INTERFACE)->_ResetImageBuffers) PROC_ALIAS( ResetImageBuffers )
#define PngImageFile                    PROC_ALIAS( PngImageFile )
#define JpgImageFile                    PROC_ALIAS( JpgImageFile )
#define SetFontBias                     PROC_ALIAS( SetFontBias )
#define MakeSlicedImage                 PROC_ALIAS( MakeSlicedImage )
#define MakeSlicedImageComplex          PROC_ALIAS( MakeSlicedImageComplex )
#define UnmakeSlicedImage                 PROC_ALIAS( UnmakeSlicedImage )
#define BlotSlicedImageEx               PROC_ALIAS( BlotSlicedImageEx )
#define SetSavePortion                          PROC_ALIAS(SetSavePortion )
//#define global_font_data         (*PROC_ALIAS(global_font_data))
#endif
/* <combine sack::image::GetMaxStringLengthFont@uint32_t@SFTFont>
   \ \                                                    */
#define GetMaxStringLength(w) GetMaxStringLengthFont(w, NULL )
#ifdef DEFINE_IMAGE_PROTOCOL
//#include <msgprotocol.h>
// need to define BASE_IMAGE_MESSAGE_ID before hand to determine what the base message is.
//#define MSG_ID(method)  ( ( offsetof( struct image_interface_tag, _##method ) / sizeof( void(*)(void) ) ) + BASE_IMAGE_MESSAGE_ID + MSG_EventUser )
#define MSG_SetStringBehavior                  MSG_ID( SetStringBehavior )
#define MSG_SetBlotMethod                      MSG_ID( SetBlotMethod )
#define MSG_BuildImageFileEx                   MSG_ID( BuildImageFileEx )
#define MSG_MakeImageFileEx                    MSG_ID( MakeImageFileEx )
#define MSG_MakeSubImageEx                     MSG_ID( MakeSubImageEx )
#define MSG_RemakeImageEx                      MSG_ID( RemakeImageEx )
#define MSG_UnmakeImageFileEx                  MSG_ID( UnmakeImageFileEx )
#define MSG_ResizeImageEx                      MSG_ID( ResizeImageEx )
#define DecodeMemoryToImage                    MSG_ID( DecodeMemoryToImage )
#define MSG_MoveImage                          MSG_ID( MoveImage )
#define MSG_BlatColor                          MSG_ID( BlatColor )
#define MSG_BlatColorAlpha                     MSG_ID( BlatColorAlpha )
#define MSG_BlotImageSizedEx                   MSG_ID( BlotImageSizedEx )
#define MSG_BlotImageEx                        MSG_ID( BlotImageEx )
#define MSG_BlotScaledImageSizedEx             MSG_ID( BlotScaledImageSizedEx )
#define MSG_plot                               MSG_ID( plot )
#define MSG_plotalpha                          MSG_ID( plotalpha )
#define MSG_getpixel                           MSG_ID( getpixel )
#define MSG_do_line                            MSG_ID( do_line )
#define MSG_do_lineAlpha                       MSG_ID( do_lineAlpha )
#define MSG_do_hline                           MSG_ID( do_hline )
#define MSG_do_vline                           MSG_ID( do_vline )
#define MSG_do_hlineAlpha                      MSG_ID( do_hlineAlpha )
#define MSG_do_vlineAlpha                      MSG_ID( do_vlineAlpha )
#define MSG_GetDefaultFont                     MSG_ID( GetDefaultFont )
#define MSG_GetFontHeight                      MSG_ID( GetFontHeight )
#define MSG_GetStringSizeFontEx                MSG_ID( GetStringSizeFontEx )
#define MSG_PutCharacterFont                   MSG_ID( PutCharacterFont )
#define MSG_PutCharacterVerticalFont           MSG_ID( PutCharacterVerticalFont )
#define MSG_PutCharacterInvertFont             MSG_ID( PutCharacterInvertFont )
#define MSG_PutCharacterVerticalInvertFont     MSG_ID( PutCharacterVerticalInvertFont )
#define MSG_PutStringFontEx                    MSG_ID( PutStringFontEx )
#define MSG_PutStringVerticalFontEx            MSG_ID( PutStringVerticalFontEx )
#define MSG_PutStringInvertFontEx              MSG_ID( PutStringInvertFontEx )
#define MSG_PutStringInvertVerticalFontEx      MSG_ID( PutStringInvertVerticalFontEx )
#define MSG_GetMaxStringLengthFont             MSG_ID( GetMaxStringLengthFont )
#define MSG_GetImageSize                       MSG_ID( GetImageSize )
#define MSG_ColorAverage                       MSG_IC( ColorAverage )
// these messages follow all others... and are present to handle
// LoadImageFile
// #define MSG_LoadImageFile (no message)
// #define MSG_LoadFont      (no message)
#define MSG_UnloadFont                         MSG_ID( UnloadFont )
#define MSG_BeginTransferData                  MSG_ID( BeginTransferData )
#define MSG_ContinueTransferData               MSG_ID( ContinueTransferData )
#define MSG_DecodeTransferredImage             MSG_ID( DecodeTransferredImage )
#define MSG_AcceptTransferredFont              MSG_ID( AcceptTransferredFont )
#define MSG_SyncImage                          MSG_ID( SyncImage )
#define MSG_IntersectRectangle                 MSG_ID( IntersectRectangle )
#define MSG_MergeRectangle                     MSG_ID( MergeRectangle)
#define MSG_GetImageSurface                    MSG_ID( GetImageSurface )
#define MSG_SetImageAuxRect                    MSG_ID(SetImageAuxRect)
#define MSG_GetImageAuxRect                    MSG_ID(GetImageAuxRect)
#define MSG_OrphanSubImage                     MSG_ID(OrphanSubImage)
#define MSG_GetGlobalFonts                     MSG_ID(GetGlobalFonts)
#define MSG_AdoptSubImage                      MSG_ID(AdoptSubImage)
#define MSG_MakeSpriteImageFileEx   MSG_ID(MakeSpriteImageFileEx)
#define MSG_MakeSpriteImageEx       MSG_ID(MakeSpriteImageEx)
#define MSG_UnmakeSprite            MSG_ID(UnmakeSprite )
#define MSG_rotate_scaled_sprite    MSG_ID(rotate_scaled_sprite)
#define MSG_rotate_sprite           MSG_ID(rotate_sprite)
#define MSG_BlotSprite              MSG_ID(BlotSprite)
#define MSG_SetSpritePosition  MSG_ID(  SetSpritePosition )
#define MSG_SetSpriteHotspot  MSG_ID(  SetSpriteHotspot )
#define MSG_InternalRenderFont          MSG_ID(InternalRenderFont)
#define MSG_InternalRenderFontFile      MSG_ID(InternalRenderFontFile)
#define MSG_RenderScaledFontData              MSG_ID(RenderScaledFontData)
#define MSG_RenderScaledFont              MSG_ID(RenderScaledFont)
#define MSG_RenderFontData              MSG_ID(RenderFontData)
#define MSG_DestroyFont              MSG_ID(DestroyFont)
#define MSG_GetFontRenderData              MSG_ID(GetFontRenderData)
#define MSG_SetFontRendererData              MSG_ID(SetFontRendererData)
#endif
#ifdef USE_IMAGE_LEVEL
#define PASTELEVEL(level,name) level##name
#define LEVEL_ALIAS(name)      PASTELEVEL(USE_IMAGE_LEVEL,name)
#  ifdef STUPID_NO_DATA_EXPORTS
#define PLEVEL_ALIAS(name)      (*PASTELEVEL(USE_IMAGE_LEVEL,_PASTE(_,name)))
#  else
#define PLEVEL_ALIAS(name)      (*PASTELEVEL(USE_IMAGE_LEVEL,name))
#  endif
#define SetStringBehavior                  LEVEL_ALIAS(SetStringBehavior )
                      //LEVEL_ALIAS(SetBlotMethod )
#define SetBlotMethod
#define BuildImageFileEx                   LEVEL_ALIAS(BuildImageFileEx )
#define MakeImageFileEx                    LEVEL_ALIAS(MakeImageFileEx )
#define MakeSubImageEx                     LEVEL_ALIAS(MakeSubImageEx )
#define RemakeImageEx                      LEVEL_ALIAS(RemakeImageEx )
#define ResizeImageEx                      LEVEL_ALIAS(ResizeImageEx )
#define MoveImage                          LEVEL_ALIAS(MoveImage )
#define LoadImageFileEx                    LEVEL_ALIAS(LoadImageFileEx )
#define DecodeMemoryToImage                LEVEL_ALIAS(DecodeMemoryToImage )
#define UnmakeImageFileEx                  LEVEL_ALIAS(UnmakeImageFileEx )
#define BlatColor                          LEVEL_ALIAS(BlatColor )
#define BlatColorAlpha                     LEVEL_ALIAS(BlatColorAlpha )
#define BlotImageSizedEx                   LEVEL_ALIAS(BlotImageSizedEx )
#define BlotImageEx                        LEVEL_ALIAS(BlotImageEx )
#define BlotScaledImageSizedEx             LEVEL_ALIAS(BlotScaledImageSizedEx )
#define plot                               LEVEL_ALIAS(plot )
#define plotalpha                          LEVEL_ALIAS(plotalpha )
#error 566
#define getpixel                           LEVEL_ALIAS(getpixel )
#define do_line                            LEVEL_ALIAS(do_line )
#define do_lineAlpha                       LEVEL_ALIAS(do_lineAlpha )
#define do_hline                           LEVEL_ALIAS(do_hline )
#define do_vline                           LEVEL_ALIAS(do_vline )
#define do_hlineAlpha                      LEVEL_ALIAS(do_hlineAlpha )
#define do_vlineAlpha                      LEVEL_ALIAS(do_vlineAlpha )
#define GetDefaultFont                     LEVEL_ALIAS(GetDefaultFont )
#define GetFontHeight                      LEVEL_ALIAS(GetFontHeight )
#define GetStringSizeFontEx                LEVEL_ALIAS(GetStringSizeFontEx )
#define PutCharacterFont                   LEVEL_ALIAS(PutCharacterFont )
#define PutCharacterVerticalFont           LEVEL_ALIAS(PutCharacterVerticalFont )
#define PutCharacterInvertFont             LEVEL_ALIAS(PutCharacterInvertFont )
#define PutCharacterVerticalInvertFont     LEVEL_ALIAS(PutCharacterVerticalInvertFont )
#define PutStringFontEx                    LEVEL_ALIAS(PutStringFontEx )
#define PutStringVerticalFontEx            LEVEL_ALIAS(PutStringVerticalFontEx )
#define PutStringInvertFontEx              LEVEL_ALIAS(PutStringInvertFontEx )
#define PutStringInvertVerticalFontEx      LEVEL_ALIAS(PutStringInvertVerticalFontEx )
#define GetMaxStringLengthFont             LEVEL_ALIAS(GetMaxStringLengthFont )
#define GetImageSize                       LEVEL_ALIAS(GetImageSize )
#define LoadFont                           LEVEL_ALIAS(LoadFont )
#define UnloadFont                         LEVEL_ALIAS(UnloadFont )
#define ColorAverage                       LEVEL_ALIAS(ColorAverage)
#define SyncImage                          LEVEL_ALIAS(SyncImage )
#define IntersectRectangle                 LEVEL_ALIAS( IntersectRectangle )
#define MergeRectangle                     LEVEL_ALIAS(MergeRectangle)
#define GetImageSurface                    LEVEL_ALIAS(GetImageSurface)
#define SetImageAuxRect                    LEVEL_ALIAS(SetImageAuxRect)
#define GetImageAuxRect                    LEVEL_ALIAS(GetImageAuxRect)
#define OrphanSubImage                     LEVEL_ALIAS(OrphanSubImage)
#define GetGlobalFonts                     LEVEL_ALIAS(GetGlobalFonts)
#define AdoptSubImage                      LEVEL_ALIAS(AdoptSubImage)
#define InternalRenderFont          LEVEL_ALIAS(InternalRenderFont)
#define InternalRenderFontFile      LEVEL_ALIAS(InternalRenderFontFile)
#define RenderScaledFontData              LEVEL_ALIAS(RenderScaledFontData)
#define RenderFontData              LEVEL_ALIAS(RenderFontData)
#define RenderFontFileScaledEx              LEVEL_ALIAS(RenderFontFileScaledEx)
#endif
_INTERFACE_NAMESPACE_END
#ifdef __cplusplus
#ifdef _D3D_DRIVER
	using namespace sack::image::d3d::Interface;
#elif defined( _D3D10_DRIVER )
	using namespace sack::image::d3d10::Interface;
#elif defined( _D3D11_DRIVER )
	using namespace sack::image::d3d11::Interface;
#else
	using namespace sack::image::Interface;
#endif
#endif
// these macros provide common extensions for
// commonly used shorthands of the above routines.
// no worry - one way or another, the extra data is
// created, and the base function called, it's a sad
// truth of life, that one codebase is easier to maintain
// than a duplicate copy for each minor case.
// although - special forwards - such as DBG_SRC will just dissappear
// in certain compilation modes (NON_DEBUG)
/* <combine sack::image::BuildImageFileEx@PCOLOR@uint32_t@uint32_t>
   \ \                                                           */
#define BuildImageFile(p,w,h) BuildImageFileEx( p,w,h DBG_SRC )
/* <combine sack::image::RemakeImageEx@Image@PCOLOR@uint32_t@uint32_t>
   \ \                                                              */
#define RemakeImage(p,pc,w,h) RemakeImageEx(p,pc,w,h DBG_SRC)
/* <combine sack::image::ResizeImageEx@Image@uint32_t@uint32_t>
   \ \                                                              */
#define ResizeImage( p,w,h) ResizeImageEx( p,w,h DBG_SRC )
/* <combine sack::image::MakeSpriteImageEx@Image image>
   \ \                                                  */
#define MakeSpriteImage(image) MakeSpriteImageEx(image DBG_SRC)
/* <combine sack::image::MakeSpriteImageFileEx@CTEXTSTR fname>
   \ \                                                         */
#define MakeSpriteImageFile(file) MakeSpriteImageFileEx( image DBG_SRC )
/* This function flips an image top to bottom. This if for
   building windows compatible images. Internally images are
   kept in platform-native direction. If an image is created
   from another source, this might be a method to flip the image
   top-to-bottom if required.
   Parameters
   pImage :                           Image to flip.
   <link sack::DBG_PASS, DBG_PASS> :  _nt_
   Note
   There has been a warning around flip image for a while, it
   does its job right now (reversing jpeg images on windows),
   but not necessarily suited for the masses.                    */
IMAGE_PROC  void IMAGE_API IMGVER(FlipImageEx )( Image pif DBG_PASS );
/* <combine sack::image::FlipImageEx@Image pif>
   \ \                                          */
#define FlipImage(pif) FlipImageEx( pif DBG_SRC )
/* <combine sack::image::LoadImageFileEx@CTEXTSTR name>
   \ \                                                  */
#define LoadImageFile(file) LoadImageFileEx( file DBG_SRC )
/* <combine sack::image::LoadImageFileEx@CTEXTSTR name>
   \ \                                                  */
#define LoadImageFileFromGroup(group,file) LoadImageFileFromGroupEx( group, file DBG_SRC )
/* <combine sack::image::BlatColor@Image@int32_t@int32_t@uint32_t@uint32_t@CDATA>
   \ \                                                            */
#define ClearImageTo(img,color) BlatColor(img,0,0,(img)->width,(img)->height, color )
#define ogl_ClearImageTo(img,color) ogl_BlatColor(img,0,0,(img)->width,(img)->height, color )
/* <combine sack::image::BlatColor@Image@int32_t@int32_t@uint32_t@uint32_t@CDATA>
   \ \                                                            */
#define ClearImage(img) BlatColor(img,0,0,(img)->width,(img)->height, 0 )
#define ogl_ClearImage(img) ogl_BlatColor(img,0,0,(img)->width,(img)->height, 0 )
/* Copy one image to another. Copies the source from 0,0 to the
   destination 0,0 of the minimum width and height of the
   smaller of the source or destination.
   Parameters
   pifDest :  Image to copy to
   pifSrc :   Image to copy from
   X :        left coordinate to copy image to
   Y :        upper coordinate to copy image to
   Example
   This creates an image to write to, creates an image to copy
   (a 64 by 64 square that is filled with 50% green color). And
   copies the image to the output buffer.
   <code>
   Image output = MakeImageFile( 1024, 768 );
   Image source = MakeImageFile( 64, 64 );
   // 50% transparent
   ClearImageTo( source, SetAlpha( BASE_COLOR_GREEN, 128 ) );
   ClearImage( output );
   BlotImage( output, source, 100, 100 );
   BlotImageAlpha( output, source, 200, 200 );
   </code>                                                      */
#define BlotImage( pd, ps, x, y ) BlotImageEx( pd, ps, x, y, 0, BLOT_COPY )
#define ogl_BlotImage( pd, ps, x, y ) ogl_BlotImageEx( pd, ps, x, y, 0, BLOT_COPY )
/* Output a sliced image to an image surface
  sliced images scale center portions, but copy output corner images
  */
#define BlotSlicedImage( pd, ps, x, y, w, h ) BlotSlicedImageEx( pd, ps, x, y, w, h, ALPHA_TRANSPARENT, BLOT_COPY )
/* Copy one image to another at the specified coordinate in the
   destination.
   Parameters
   Destination :  Image to output to
   Source :       Image to copy
   X :            Location to copy to
   Y :            Location to copy to <link sack::image::AlphaModifier, Alpha>
                  \: Specify how to write the alpha                            */
#define BlotImageAlpha( pd, ps, x, y, a ) BlotImageEx( pd, ps, x, y, a, BLOT_COPY )
/* <combine sack::image::BlotImageSizedEx@Image@Image@int32_t@int32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                         */
#define BlotImageSized( pd, ps, x, y, w, h ) BlotImageSizedEx( pd, ps, x, y, 0, 0, w, h, TRUE, BLOT_COPY )
#define ogl_BlotImageSized( pd, ps, x, y, w, h ) ogl_BlotImageSizedEx( pd, ps, x, y, 0, 0, w, h, TRUE, BLOT_COPY )
/* <combine sack::image::BlotImageSizedEx@Image@Image@int32_t@int32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                         */
#define BlotImageSizedAlpha( pd, ps, x, y, w, h, a ) BlotImageSizedEx( pd, ps, x, y, 0, 0, w, h, a, BLOT_COPY )
#define ogl_BlotImageSizedAlpha( pd, ps, x, y, w, h, a ) ogl_BlotImageSizedEx( pd, ps, x, y, 0, 0, w, h, a, BLOT_COPY )
/* <combine sack::image::BlotImageSizedEx@Image@Image@int32_t@int32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                         */
#define BlotImageSizedTo( pd, ps, xd, yd, xs, ys, w, h )  BlotImageSizedEx( pd, ps, xd, yd, xs, ys, w, h, TRUE, BLOT_COPY )
#define ogl_BlotImageSizedTo( pd, ps, xd, yd, xs, ys, w, h )  ogl_BlotImageSizedEx( pd, ps, xd, yd, xs, ys, w, h, TRUE, BLOT_COPY )
/* Copy one image to another at the specified coordinate in the
   destination. Shade the image on copy with a color.
   Parameters
   Destination :  Image to output to
   Source :       Image to copy
   X :            Location to copy to
   Y :            Location to copy to
   Color :        color to multiply the source color by to shade
                  on copy.                                       */
#define BlotImageShaded( pd, ps, xd, yd, c ) BlotImageEx( pd, ps, xd, yd, TRUE, BLOT_SHADED, c )
/* <combine sack::image::BlotImageSizedEx@Image@Image@int32_t@int32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                         */
#define BlotImageShadedSized( pd, ps, xd, yd, xs, ys, ws, hs, c ) BlotImageSizedEx( pd, ps, xd, yd, xs, ys, ws, hs, TRUE, BLOT_SHADED, c )
#define ogl_BlotImageShadedSized( pd, ps, xd, yd, xs, ys, ws, hs, c ) ogl_BlotImageSizedEx( pd, ps, xd, yd, xs, ys, ws, hs, TRUE, BLOT_SHADED, c )
/* Copy one image to another at the specified coordinate in the
   destination. Scale RGB channels to specified colors.
   Parameters
   Destination :  Image to output to
   Source :       Image to copy
   X :            Location to copy to
   Y :            Location to copy to
   X_source :     the left coordinate of the image source
   Y_source :     the top coordinate of the image source
   Width :        How wide to copy the image
   Height :       How wide to copy the image
   color :        color mutiplier to shade the image.           */
#define BlotImageMultiShaded( pd, ps, xd, yd, r, g, b ) BlotImageEx( pd, ps, xd, yd, ALPHA_TRANSPARENT, BLOT_MULTISHADE, r, g, b )
/* Copy one image to another at the specified coordinate in the
   destination. Scale RGB channels to specified colors.
   Parameters
   Destination :  Image to output to
   Source :       Image to copy
   X :            Location to copy to
   Y :            Location to copy to
   X_source :     the left coordinate of the image source
   Y_source :     the top coordinate of the image source
   Width :        How wide to copy the image
   Height :       How wide to copy the image
   color :        color mutiplier to shade the image.           */
#define BlotImageMultiShadedSized( pd, ps, xd, yd, xs, ys, ws, hs, r, g, b ) BlotImageSizedEx( pd, ps, xd, yd, xs, ys, ws, hs, TRUE, BLOT_MULTISHADE, r, g, b )
#define ogl_BlotImageMultiShadedSized( pd, ps, xd, yd, xs, ys, ws, hs, r, g, b ) ogl_BlotImageSizedEx( pd, ps, xd, yd, xs, ys, ws, hs, TRUE, BLOT_MULTISHADE, r, g, b )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImageSized( pd, ps, xd, yd, wd, hd, xs, ys, ws, hs ) BlotScaledImageSizedEx( pd, ps, xd, yd, wd, hd, xs, ys, ws, hs, 0, BLOT_COPY )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImageSizedMultiShaded( pd, ps, xd, yd, wd, hd, xs, ys, ws, hs,r,g,b ) BlotScaledImageSizedEx( pd, ps, xd, yd, wd, hd, xs, ys, ws, hs, 0, BLOT_MULTISHADE,r,g,b )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImageSizedTo( pd, ps, xd, yd, wd, hd) BlotScaledImageSizedEx( pd, ps, xd, yd, wd, hd, 0, 0, (ps)->width, (ps)->height, 0, BLOT_COPY )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImageSizedToAlpha( pd, ps, xd, yd, wd, hd, a) BlotScaledImageSizedEx( pd, ps, xd, yd, wd, hd, 0, 0, (ps)->width, (ps)->height, a, BLOT_COPY )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImageSizedToShaded( pd, ps, xd, yd, wd, hd,shade) BlotScaledImageSizedEx( pd, ps, xd, yd, wd, hd, 0, 0, (ps)->width, (ps)->height, 0,BLOT_SHADED, shade )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImageSizedToShadedAlpha( pd, ps, xd, yd, wd, hd,a,shade) BlotScaledImageSizedEx( pd, ps, xd, yd, wd, hd, 0, 0, (ps)->width, (ps)->height, a, BLOT_SHADED, shade )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImageSizedToMultiShaded( pd, ps, xd, yd, wd, hd,r,g,b) BlotScaledImageSizedEx( pd, ps, xd, yd, wd, hd, 0, 0, (ps)->width, (ps)->height, 0,BLOT_MULTISHADE, r,g,b )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImageSizedToMultiShadedAlpha( pd, ps, xd, yd, wd, hd,a,r,g,b) BlotScaledImageSizedEx( pd, ps, xd, yd, wd, hd, 0, 0, (ps)->width, (ps)->height, a,BLOT_MULTISHADE, r,g,b )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImageAlpha( pd, ps, t ) BlotScaledImageSizedEx( pd, ps, 0, 0, (pd)->width, (pd)->height, 0, 0, (ps)->width, (ps)->height, t, BLOT_COPY )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImageShadedAlpha( pd, ps, t, shade ) BlotScaledImageSizedEx( pd, ps, 0, 0, (pd)->width, (pd)->height, 0, 0, (ps)->width, (ps)->height, t, BLOT_SHADED, shade )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImageMultiShadedAlpha( pd, ps, t, r, g, b ) BlotScaledImageSizedEx( pd, ps, 0, 0, (pd)->width, (pd)->height, 0, 0, (ps)->width, (ps)->height, t, BLOT_MULTISHADE, r, g, b )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImage( pd, ps ) BlotScaledImageSizedEx( pd, ps, 0, 0, (pd)->width, (pd)->height, 0, 0, (ps)->width, (ps)->height, 0, BLOT_COPY )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImageShaded( pd, ps, shade ) BlotScaledImageSizedEx( pd, ps, 0, 0, (pd)->width, (pd)->height, 0, 0, (ps)->width, (ps)->height, 0, BLOT_SHADED, shade )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImageMultiShaded( pd, ps, r, g, b ) BlotScaledImageSizedEx( pd, ps, 0, 0, (pd)->width, (pd)->height, 0, 0, (ps)->width, (ps)->height, 0, BLOT_MULTISHADE, r, g, b )
/* <combine sack::image::BlotScaledImageSizedEx@Image@Image@int32_t@int32_t@uint32_t@uint32_t@int32_t@int32_t@uint32_t@uint32_t@uint32_t@uint32_t@...>
   \ \                                                                                                       */
#define BlotScaledImageTo( pd, ps )  BlotScaledImageToEx( pd, ps, FALSE, BLOT_COPY )
/* now why would we need an inverse line? I don't get it....
   anyhow this would draw from the end to the start... basically
   this accounts for rounding errors on the orward way.          */
#define do_inv_line(pb,x,y,xto,yto,d) do_line( pb,y,x,yto,xto,d)
/* <combine sack::image::PutCharacterFont@Image@int32_t@int32_t@CDATA@CDATA@TEXTCHAR@SFTFont>
   \ \                                                                               */
#define PutCharacter(i,x,y,fore,back,c)               PutCharacterFont(i,x,y,fore,back,c,NULL )
/* <combine sack::image::PutCharacterVerticalFont@Image@int32_t@int32_t@CDATA@CDATA@TEXTCHAR@SFTFont>
   Passes default font if not specified.                                                     */
#define PutCharacterVertical(i,x,y,fore,back,c)       PutCharacterVerticalFont(i,x,y,fore,back,c,NULL )
/* <combine sack::image::PutCharacterInvertFont@Image@int32_t@int32_t@CDATA@CDATA@TEXTCHAR@SFTFont>
   \ \                                                                                     */
#define PutCharacterInvert(i,x,y,fore,back,c)         PutCharacterInvertFont(i,x,y,fore,back,c,NULL )
/* <combine sack::image::PutCharacterVerticalInvertFont@Image@int32_t@int32_t@CDATA@CDATA@TEXTCHAR@SFTFont>
   \ \                                                                                             */
#define PutCharacterInvertVertical(i,x,y,fore,back,c) PutCharacterInvertVerticalFont(i,x,y,fore,back,c,NULL )
/* <combine sack::image::PutCharacterVerticalInvertFont@Image@int32_t@int32_t@CDATA@CDATA@TEXTCHAR@SFTFont>
   \ \                                                                                             */
#define PutCharacterInvertVerticalFont(i,x,y,fore,back,c,f) PutCharacterVerticalInvertFont(i,x,y,fore,back,c,f )
/* <combine sack::image::PutStringFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   \ \                                                                                  */
#define PutString(pi,x,y,fore,back,pc) PutStringFontEx( pi, x, y, fore, back, pc, StrLen(pc), NULL )
/* <combine sack::image::PutStringFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   \ \                                                                                  */
#define PutStringEx(pi,x,y,color,back,pc,len) PutStringFontEx( pi, x, y, color,back,pc,len,NULL )
/* <combine sack::image::PutStringFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   \ \                                                                                  */
#define PutStringFont(pi,x,y,fore,back,pc,font) PutStringFontEx(pi,x,y,fore,back,pc,StrLen(pc), font )
/* <combine sack::image::PutStringVerticalFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   \ \                                                                                          */
#define PutStringVertical(pi,x,y,fore,back,pc) PutStringVerticalFontEx( pi, x, y, fore, back, pc, StrLen(pc), NULL )
/* <combine sack::image::PutStringVerticalFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   \ \                                                                                          */
#define PutStringVerticalEx(pi,x,y,color,back,pc,len) PutStringVerticalFontEx( pi, x, y, color,back,pc,len,NULL )
/* <combine sack::image::PutStringVerticalFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   \ \                                                                                          */
#define PutStringVerticalFont(pi,x,y,fore,back,pc,font) PutStringVerticalFontEx(pi,x,y,fore,back,pc,StrLen(pc), font )
/* <combine sack::image::PutStringInvertFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   \ \                                                                                        */
#define PutStringInvert( pi, x, y, fore, back, pc ) PutStringInvertFontEx( pi, x, y, fore, back, pc,StrLen(pc), NULL )
/* <combine sack::image::PutStringInvertFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   \ \                                                                                        */
#define PutStringInvertEx( pi, x, y, fore, back, pc, nLen ) PutStringInvertFontEx( pi, x, y, fore, back, pc, nLen, NULL )
/* <combine sack::image::PutStringInvertFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   The non Ex Version doesn't pass the string length.                                         */
#define PutStringInvertFont( pi, x, y, fore, back, pc, nLen ) PutStringInvertFontEx( pi, x, y, fore, back, pc, StrLen(pc), font )
/* <combine sack::image::PutStringInvertVerticalFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   \ \                                                                                                */
#define PutStringInvertVertical( pi, x, y, fore, back, pc ) PutStringInvertVerticalFontEx( pi, x, y, fore, back, pc, StrLen(pc), NULL )
/* <combine sack::image::PutStringInvertVerticalFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   \ \                                                                                                */
#define PutStringInvertVerticalEx( pi, x, y, fore, back, pc, nLen ) PutStringInvertVerticalFontEx( pi, x, y, fore, back, pc, nLen, NULL )
/* <combine sack::image::PutStringInvertVerticalFontEx@Image@int32_t@int32_t@CDATA@CDATA@CTEXTSTR@uint32_t@SFTFont>
   \ \                                                                                                */
#define PutStringInvertVerticalFont( pi, x, y, fore, back, pc, font ) PutStringInvertVerticalFontEx( pi, x, y, fore, back, pc, StrLen(pc), font )
//IMG_PROC uint32_t PutMenuStringFontEx        ( ImageFile *pImage, int32_t x, int32_t y, CDATA color, CDATA background, CTEXTSTR pc, uint32_t nLen, PFONT font );
//#define PutMenuStringFont(img,x,y,fore,back,string,font) PutMenuStringFontEx( img,x,y,fore,back,string,StrLen(string),font)
//#define PutMenuString(img,x,y,fore,back,str)           PutMenuStringFont(img,x,y,fore,back,str,NULL)
//
//IMG_PROC uint32_t PutCStringFontEx           ( ImageFile *pImage, int32_t x, int32_t y, CDATA color, CDATA background, CTEXTSTR pc, uint32_t nLen, PFONT font );
//#define PutCStringFont(img,x,y,fore,back,string,font) PutCStringFontEx( img,x,y,fore,back,string,StrLen(string),font)
//#define PutCString( img,x,y,fore,back,string) PutCStringFont(img,x,y,fore,back,string,NULL )
/* <combine sack::image::GetStringSizeFontEx@CTEXTSTR@uint32_t@uint32_t *@uint32_t *@SFTFont>
   \ \                                                                      */
#define GetStringSizeEx(s,len,pw,ph) GetStringSizeFontEx( (s),len,pw,ph,NULL)
/* <combine sack::image::GetStringSizeFontEx@CTEXTSTR@size_t@uint32_t *@uint32_t *@SFTFont>
   \ \                                                                         */
#define GetStringSize(s,pw,ph)       GetStringSizeFontEx( (s),StrLen(s),pw,ph,NULL)
/* <combine sack::image::GetStringSizeFontEx@CTEXTSTR@uint32_t@uint32_t *@uint32_t *@SFTFont>
   \ \                                                                      */
#define GetStringSizeFont(s,pw,ph,f) GetStringSizeFontEx( (s),StrLen(s),pw,ph,f )
#ifdef __cplusplus
IMAGE_NAMESPACE_END
#ifdef _D3D_DRIVER
using namespace sack::image::d3d;
#elif defined( _D3D10_DRIVER )
using namespace sack::image::d3d10;
#elif defined( _D3D11_DRIVER )
using namespace sack::image::d3d11;
#else
using namespace sack::image;
#endif
#endif
#endif
/*   */
#ifndef __NO_INTERFACES__
   // for interface, can omit if no interfaces
#endif
#ifndef __NO_MSGSVR__
  // for interface across the message service
#ifndef MESSAGE_SERVICE_PROTOCOL
#define MESSAGE_SERVICE_PROTOCOL
#ifdef __cplusplus
using namespace sack;
#endif
#ifdef __cplusplus
#define _MSG_NAMESPACE  namespace msg {
#define _PROTOCOL_NAMESPACE namespace protocol {
#define MSGPROTOCOL_NAMESPACE namespace sack { _MSG_NAMESPACE _PROTOCOL_NAMESPACE
#define MSGPROTOCOL_NAMESPACE_END }} }
#else
#define _MSG_NAMESPACE
#define _PROTOCOL_NAMESPACE
#define MSGPROTOCOL_NAMESPACE
#define MSGPROTOCOL_NAMESPACE_END
#endif
SACK_NAMESPACE
	/* This namespace contains an implmentation of inter process
	   communications using a set of message queues which result
	   from 'msgget' 'msgsnd' and 'msgrcv'. This are services
	   available under a linux kernel. Reimplemented a version to
	   service for windows. This is really a client/service
	   registration and message routing system, it is not the
	   message queue itself. See <link sack::containers::message, message>
	   for the queue implementation (again, under linux, does not
	   use this custom queue).
	   See Also
	   RegisterService
	   LoadService                                                         */
	_MSG_NAMESPACE
/* Defines structures and methods for receiving and sending
	   messages. Also defines some utility macros for referencing
		message ID from a user interface structure.                */
	_PROTOCOL_NAMESPACE
#define MSGQ_ID_BASE "Srvr"
// this is a fun thing, in order to use it,
// undefine MyInterface, and define your own to your
// library's interface structure name (the tag of the structure)
#define MSG_ID(method)  BASE_MESSAGE_ID,( ( offsetof( struct MyInterface, _##method ) / sizeof( void(*)(void) ) ) +  MSG_EventUser )
#define MSG_OFFSET(method)  ( ( offsetof( struct MyInterface, _##method ) / sizeof( void(*)(void) ) ) + MSG_EventUser )
#define INTERFACE_METHOD(type,name) type (CPROC*_##name)
// this is the techincal type of SYSV IPC MSGQueues
#define MSGIDTYPE long
#ifdef __64__
#  ifdef __LINUX__
#    define _MsgID_f  _64fs
#  else
#    define _MsgID_f  _32fs
#  endif
#else
#  define _MsgID_f  _32fs
#endif
// this will determine the length of parameter list
// based on the first and last parameters.
#define ParamLength( first, last ) ( ((uintptr_t)((&(last))+1)) - ((uintptr_t)(&(first))) )
#ifdef _MSC_VER
#pragma pack (push, 1)
#endif
typedef PREFIX_PACKED struct buffer_len_tag {
	CPOINTER buffer;
	size_t len;
} PACKED BUFFER_LENGTH_PAIR;
#ifdef _MSC_VER
#pragma pack (pop)
#endif
// Dispach Pending - particularly display mouse event messages
//                   needed to be accumulated before being dispatched
//                   this event is generated when no more messages
//                   have been received.
#define MSG_EventDispatchPending   0
#define MSG_DispatchPending   MSG_EventDispatchPending
// these are event message definitions.
// server events come through their function table, clients
// register an event handler... these are low numbered since
// they are guaranteed from the client/server respectively.
// Mate ended - for the client, this means that the server
//              has become defunct.  For the server, this
//              means that a client is no longer present.
//              also issued when a client volentarily leaves
//              which in effect is the same as being discovered gone.
//    param[0] = Process ID of client disconnecting
//  result_length = INVALID_INDEX - NO RESULT DATA, PLEASE!
#define MSG_MateEnded         MSG_ServiceUnload
#define MSG_ServiceUnload     0
//#define MSG_ServiceClose    MSG_ServiceUnload
//#define MSG_ServiceUnload        MSG_MateEnded
// finally - needed to define a way for the service
// to actually know when a client connects... so that
// it may validate commands as being froma good source.
// also, a multiple service server may want this to know which
// service is being loaded.
//     params + 0 = text string of the service to load
//  on return result[1] is the number of messages this routine
//  expects.
//     result[0] is the number of events this service may generate
#define MSG_MateStarted      1
#define MSG_ServiceLoad      MSG_MateStarted
// Service is about to be unloaded - here's a final chance to
// cleanup before being yanked from existance.
// Last reference to the service is now gone, going to do the unload.
#define MSG_UndefinedMessage2      2
// no defined mesasage fo this
#define MSG_UndefinedMessage3       3
// Other messages may be filled in here...
// skip a couple messages so we don't have to recompile everything
// very soon...
#define MSG_EventUser       MSG_UserServiceMessages
#define MSG_UserServiceMessages 16
// skip a couple messages so we don't have to recompile everything
// very soon...
#define MSG_EventInternal       MSG_InternalServiceMessages
#define MSG_InternalServiceMessages 4
enum server_event_messages {
	// these messages are sent to client's event channel
	// within the space of core service requests (0-256?)
	// it's on top of client event user - cause the library
	// may also receive client_disconnect/connect messages
   //
	MSG_SERVICE_DATA = MSG_EventInternal
 // end of list - zero or more MSG_SERVICE_DATA mesasges will preceed this.
      , MSG_SERVICE_NOMORE
	, MSG_SERVICE_MAX_ID
};
enum server_failure_messages {
	CLIENT_UNKNOWN
									  , MESSAGE_UNKNOWN
 // sending server(sourced) messages to server
									  , MESSAGE_INVALID
 // could not find a service for the message.
									  , SERVICE_UNKNOWN
									  , UNABLE_TO_LOAD
};
enum service_messages {
 // no message ID 0 ever.
	INVALID_MESSAGE  = 0
 // server responce to clients - failure
							 , SERVER_FAILURE   = 0x80000000
							 // failure may result for the above reasons.
 // server responce to clients - success
							 , SERVER_SUCCESS   = 0x40000000
 // server needs more time to complete...
							 , SERVER_NEED_TIME = 0x20000000
 // server had no method to process the message
							 , SERVER_UNHANDLED = 0x10000000
 // client requests a service (load by name)
							 , CLIENT_LOAD_SERVICE = 1
 // client no longer needs a service (unload msgbase)
							 , CLIENT_UNLOAD_SERVICE
       // new client wants to connect
							 , CLIENT_CONNECT
    // client disconnects (no responce)
							 , CLIENT_DISCONNECT
             // server/client message to other requesting status
							 , RU_ALIVE
             // server/client message to other responding status
							 , IM_ALIVE
 // client register service (name, serivces, callback table.)
							 , CLIENT_REGISTER_SERVICE
 // client requests a list of services (optional param partial filter?)
                      , CLIENT_LIST_SERVICES
   // Service needs more time, and passes back a millisecond delay-reset
                      , IM_TARDY
};
#define LOWEST_BASE_MESSAGE 0x100
typedef struct ServiceRoute_tag SERVICE_ROUTE;
typedef struct ServiceRoute_tag *PSERVICE_ROUTE;
typedef struct ServiceEndPoint_tag SERVICE_ENDPOINT, *PSERVICE_ENDPOINT;
// this is part of the message structure
//
// this structure is avaialble at ((PSERVICE_ROUTE)(((uint32_t*)params)-1)[-1])
// (to explain that, the first uint32_t back is the MsgID... to get JUST the route tag
//  have to go back one Dword then back a service_route struct...
#ifdef _MSC_VER
#pragma pack (push, 1)
#endif
PREFIX_PACKED struct ServiceEndPoint_tag
{
   // remote process ID
	MSGIDTYPE process_id;
   // service (either served or connected as client) remote id
	MSGIDTYPE service_id;
}PACKED;
PREFIX_PACKED struct ServiceRoute_tag
{
   SERVICE_ENDPOINT dest;
	//MSGIDTYPE process_id;   // remote process ID
   //MSGIDTYPE service_id;   // service (either served or connected as client) remote id
   SERVICE_ENDPOINT source;
   //uint32_t source_process_id; // need this defined here anyway; so this can be used in receivers
	//uint32_t source_service_id;  // the service this is connected to, or is a connection for local ID
}PACKED;
#ifdef _MSC_VER
#pragma pack (pop)
#endif
#define GetServiceRoute(data)   ((PSERVICE_ROUTE)(((uint32_t*)data)-1)-1)
// server functions will return TRUE if no failure
// server functions will return FALSE on failure
// FAILURE or SUCCESS will be returned to the client,
//   along with any result data set.
// native mode (unspecified... one would assume
// stack passing, but the world is bizarre and these are
// probably passed by registers.
typedef int (CPROC *server_message_handler)( PSERVICE_ROUTE SourceRouteID, uint32_t MsgID
														 , uint32_t *params, size_t param_length
														 , uint32_t *result, size_t *result_length );
typedef int (CPROC *server_message_handler_ex)( uintptr_t psv
															 , PSERVICE_ROUTE SourceRouteID, uint32_t MsgID
															 , uint32_t *params, size_t param_length
															 , uint32_t *result, size_t *result_length );
// params[-1] == Source Process ID
// params[-2] == Source Route ID
typedef int (CPROC *server_function)( PSERVICE_ROUTE route, uint32_t *params, size_t param_length
										 , uint32_t *result, size_t *result_length );
typedef struct server_function_entry_tag{
#if defined( _DEBUG ) || defined( _DEBUG_INFO )
	CTEXTSTR name;
#endif
	server_function function;
} SERVER_FUNCTION;
#if defined( _DEBUG ) || defined( _DEBUG_INFO )
#define ServerFunctionEntry(name) { #name, name }
#else
#define ServerFunctionEntry(name) { name }
#endif
typedef SERVER_FUNCTION *server_function_table;
// MsgID will be < MSG_EventUser if it's a system message...
// MsgID will be service msgBase + Remote ID...
//    so the remote needs to specify a unique base... so ...
//    entries must still be used...
typedef int (CPROC*EventHandlerFunction)( MSGIDTYPE MsgID, uint32_t*params, size_t paramlen);
typedef int (CPROC*EventHandlerFunctionEx)( PSERVICE_ROUTE SourceID, MSGIDTYPE MsgID, uint32_t*params, size_t paramlen);
typedef int (CPROC*EventHandlerFunctionExx)( uintptr_t psv, PSERVICE_ROUTE SourceID, MSGIDTYPE MsgID
														 , uint32_t*params, size_t paramlen);
// result of EventHandlerFunction shall be one fo the following values...
//   EVENT_HANDLED
// 0 - no futher action required
//   EVENT_WAIT_DISPATCH
// 1 - when no further events are available, please send event_dispatched.
//     this Event was handled by an internal queuing for later processing.
enum EventResult {
	EVENT_HANDLED = 0,
	EVENT_WAIT_DISPATCH = 1
};
//------------------- Begin Server Message Structs ----------------
#ifdef _MSC_VER
#pragma pack (push, 1)
#endif
typedef struct MsgSvr_RegisterRequest_msg MsgSvr_RegisterRequest;
PREFIX_PACKED struct MsgSvr_RegisterRequest_msg
{
	MSGIDTYPE RouteID;
  // service_id...
   MSGIDTYPE ClientID;
}PACKED;
typedef struct MsgSrv_ReplyServiceLoad_msg MsgSrv_ReplyServiceLoad;
PREFIX_PACKED struct MsgSrv_ReplyServiceLoad_msg
{
	MSGIDTYPE ServiceID;
 // if this is a local service, it's dispatched this way?
	THREAD_ID thread;
}PACKED;
#ifdef _MSC_VER
#pragma pack (pop)
#endif
MSGPROTOCOL_NAMESPACE_END
#ifdef __cplusplus
using namespace sack::msg::protocol;
#endif
#endif
#endif
#ifndef SECOND_RENDER_LEVEL
#define SECOND_RENDER_LEVEL
#define PASTE(sym,name) name
#else
#define PASTE2(sym,name) sym##name
#define PASTE(sym,name) PASTE2(sym,name)
#endif
#ifdef USE_API_ALIAS_PREFIX
#  define RVER(n)   IMGVER_(USE_API_ALIAS_PREFIX,n)
#else
#  define RVER(n)   n
#endif
#        ifdef RENDER_LIBRARY_SOURCE
#           define RENDER_PROC(type,name) EXPORT_METHOD type CPROC PASTE(SECOND_RENDER_LEVEL,RVER(name))
#        else
#           define RENDER_PROC(type,name) IMPORT_METHOD type CPROC PASTE(SECOND_RENDER_LEVEL,RVER(name))
#        endif
SACK_NAMESPACE
/* <copy render.h>
   \ \             */
BASE_IMAGE_NAMESPACE
/* PRENDERER is the primary object this namespace deals with.
   See Also
   <link render.h>                                            */
_RENDER_NAMESPACE
/* Application layer abstract structure to handle displays. This
 is the type returned by OpenDisplay.                          */
typedef struct HVIDEO_tag *PRENDERER;
typedef struct key_function  KEY_FUNCTION;
typedef struct key_function *PKEY_FUNCTION;
// disable this functionality, it was never fully implemented, and is a lot to document.
#if ACTIVE_MESSAGE_IMPLEMENTED
// Message IDs 0-99 are reserved for
// very core level messages.
// Message IDs 100-999 are for general purpose window input/output
// Message ID 1000+ Usable by applications to transport messages via
//                  the image's default message loop.
enum active_msg_id {
    // Message ID 0 - contains a active image to respond to
   ACTIVE_MSG_PING
    // Message ID 0 - contains a active image to respond to
   , ACTIVE_MSG_PONG
   , ACTIVE_MSG_MOUSE = 100
   , ACTIVE_MSG_GAIN_FOCUS
   , ACTIVE_MSG_LOSE_FOCUS
   , ACTIVE_MSG_DRAG
   , ACTIVE_MSG_KEY
   , ACTIVE_MSG_DRAW
   , ACTIVE_MSG_CREATE
   , ACTIVE_MSG_DESTROY
   , ACTIVE_MSG_USER = 1000
};
typedef struct {
   enum active_msg_id ID;
 // the size of the cargo potion of the message. (mostly data.raw)
   uint32_t  size;
   union {
  //--------------------
      struct {
         PRENDERER respondto;
      } ping;
  //--------------------
      struct {
         int x, y, b;
      } mouse;
  //--------------------
      struct {
         PRENDERER lose;
      } gain_focus;
  //--------------------
      struct {
         PRENDERER gain;
      } lose_focus;
  //--------------------
      struct {
         uint8_t no_informaiton;
      } draw;
  //--------------------
      struct {
         uint8_t no_informaiton;
      } close;
  //--------------------
      struct {
         uint8_t no_informaiton;
      } create;
  //--------------------
      struct {
         uint8_t no_informaiton;
      } destroy;
  //--------------------
      struct {
         uint32_t key;
      } key;
  //--------------------
      uint8_t raw[1];
   } data;
} ACTIVEMESSAGE, *PACTIVEMESSAGE;
#endif
// Event Message ID's CANNOT be 0
// Message Event ID (base+0) is when the
// server teriminates, and ALL client resources
// are lost.
// Message Event ID (base+1) is when the
// final message has been received, and any
// pending events collected should be dispatched.
#ifndef __NO_MSGSVR__
enum {
   /* These are internal messages to pass to the display handling
    thread. most are unimplemented.                             */
 MSG_CloseMethod = MSG_EventUser,
  /* These are internal messages to pass to the display handling
    thread. most are unimplemented.                             */
 MSG_RedrawMethod
 ,
 /* These are internal messages to pass to the display handling
    thread. most are unimplemented.                             */
 MSG_MouseMethod
 ,
 /* These are internal messages to pass to the display handling
    thread. most are unimplemented.                             */
 MSG_LoseFocusMethod
 ,
 /* These are internal messages to pass to the display handling
    thread. most are unimplemented.                             */
 MSG_KeyMethod
 ,
 /* These are internal messages to pass to the display handling
    thread. most are unimplemented.                             */
 MSG_GeneralMethod
 ,
 /* These are internal messages to pass to the display handling
    thread. most are unimplemented.                             */
 MSG_RedrawFractureMethod
 ,
 /* These are internal messages to pass to the display handling
    thread. most are unimplemented.                             */
 MSG_ThreadEventPost
};
#endif
#ifdef __WATCOMC__
#if ( __WATCOMC__ < 1291 )
#define NO_TOUCH
#endif
#endif
#ifndef WIN32
#define NO_TOUCH
#endif
#if defined( __LINUX__ )
#define NO_TOUCH
#endif
#if defined( __ANDROID__ )
// definately IS touch
#undef NO_TOUCH
#define MINGW_SUX
#endif
// static void OnBeginShutdown( "Unique Name" )( void ) { /* run shutdown code */ }
#define OnBeginShutdown(name)	 DefineRegistryMethod("SACK",BeginShutdown,"System","Begin Shutdown",name "_begin_shutdown",void,(void),__LINE__)
/* function signature for the close callback  which can be specified to handle events to close the display.  see SetCloseHandler. */
typedef void (CPROC*CloseCallback)( uintptr_t psvUser );
/* function signature to define hide/restore callback, it just gets the user data of the callback... */
typedef void (CPROC*HideAndRestoreCallback)( uintptr_t psvUser );
/* function signature for the redraw callback  which can be specified to handle events to redraw the display.  see SetRedrawHandler. */
typedef void (CPROC*RedrawCallback)( uintptr_t psvUser, PRENDERER self );
/* function signature for the mouse callback  which can be specified to handle events from mouse motion on the display.  see SetMouseHandler.
  would be 'wise' to retun 0 if ignored, 1 if observed (perhaps not used), but NOT ignored.*/
typedef uintptr_t  (CPROC*MouseCallback)( uintptr_t psvUser, int32_t x, int32_t y, uint32_t b );
typedef struct input_point
{
   //
	RCOORD x, y;
	struct {
  // set on first down, clear on subsequent events
		BIT_FIELD new_event : 1;
 // set on first up, clear on first down,
		BIT_FIELD end_event : 1;
	} flags;
} *PINPUT_POINT;
#ifndef NO_TOUCH
#if defined( MINGW_SUX )
/*
 * Touch input mask values (TOUCHINPUT.dwMask)
 */
  // the dwTime field contains a system generated value
#define TOUCHINPUTMASKF_TIMEFROMSYSTEM  0x0001
  // the dwExtraInfo field is valid
#define TOUCHINPUTMASKF_EXTRAINFO       0x0002
  // the cxContact and cyContact fields are valid
#define TOUCHINPUTMASKF_CONTACTAREA     0x0004
#ifndef __ANDROID__
typedef HANDLE HTOUCHINPUT;
#endif
#define WM_TOUCH 0x0240
#define TWF_FINETOUCH 0x00000001
#define TWF_WANTPALM 0x00000002
#endif
 // added to flags as touches are used.  Controls may use some of the touches but not all.
#define TOUCHEVENTF_USED 0x8000
/* function signature for the touch callback  which can be specified to handle events from touching the display.  see SetMouseHandler.
  would be 'wise' to retun 0 if ignored, 1 if observed (perhaps not used), but NOT ignored.  Return 1 if some of the touches are used.
  This will trigger a check to see if there are unused touches to continue sending... oh but on renderer there's only one callback, more
  important as a note of the control touch event handerer.
  */
typedef int  (CPROC*TouchCallback)( uintptr_t psvUser, PINPUT_POINT pTouches, int nTouches );
#endif
/* function signature for the close callback  which can be specified to handle events to redraw the display.  see SetLoseFocusHandler. */
typedef void (CPROC*LoseFocusCallback)( uintptr_t dwUser, PRENDERER pGain );
// without a keyproc, you will still get key notification in the mousecallback
// if KeyProc returns 0 or is NULL, then bound keys are checked... otherwise
// priority is given to controls with focus that handle keys.
typedef int (CPROC*KeyProc)( uintptr_t dwUser, uint32_t keycode );
// without any other proc, you will get a general callback message.
#if ACTIVE_MESSAGE_IMPLEMENTED
typedef void (CPROC*GeneralCallback)( uintptr_t psvUser
                                     , PRENDERER image
												, PACTIVEMESSAGE msg );
#endif
typedef void (CPROC*RenderReadCallback)(uintptr_t psvUser, PRENDERER pRenderer, TEXTSTR buffer, INDEX len );
// called before redraw callback to update the background on the scene...
typedef void (CPROC*_3DUpdateCallback)( uintptr_t psvUser );
// callback type for clipborad event reception.
typedef void (CPROC*ClipboardCallback)(uintptr_t psvUser);
//----------------------------------------------------------
//   Mouse Button definitions
//----------------------------------------------------------
// the prefix of these may either be interpreted as MAKE - as in
// a make/break state of a switch.  Or may be interpreted as
// MouseKey.... such as KB_ once upon a time stood for KeyBoard,
// and not Keebler as some may have suspected.
enum ButtonFlags {
#ifndef MK_LBUTTON
 // left mouse button  MouseKey_ ?
	MK_LBUTTON = 0x01,
#endif
#ifndef MK_MBUTTON
  // right mouse button MouseKey_ ?
	MK_RBUTTON = 0x02,
#endif
#ifndef MK_RBUTTON
  // middle mouse button MouseKey_ ?
	MK_MBUTTON = 0x10,
#endif
#ifndef MK_CONTROL
  // the control key on the keyboard
  MK_CONTROL = 0x08,
#endif
#ifndef MK_ALT
   // the alt key on the keyboard
  MK_ALT = 0x20,
#endif
#ifndef MK_SHIFT
   // the shift key on the keyboard
  MK_SHIFT = 0x40,
#endif
  // scroll wheel click down
  MK_SCROLL_DOWN  = 0x100,
  // scroll wheel click up
  MK_SCROLL_UP    = 0x200,
  // scroll wheel click left
  MK_SCROLL_LEFT  = 0x400,
  // scroll wheel click right
  MK_SCROLL_RIGHT = 0x800,
#ifndef MK_NO_BUTTON
// used to indicate that there is
// no known button information available.  The mouse
// event which triggered this was beyond the realm of
// this mouse handler, but it is entitled to know that
// it now knows nothing.
  MK_NO_BUTTON = 0xFFFFFFFF,
#endif
// this bit will NEVER NEVER NEVER be set
// for ANY reason whatsoever. ( okay except when it's in MK_NO_BUTTON )
  MK_INVALIDBUTTON = 0x80000000,
// One or more other buttons were pressed.  These
// buttons are available by querying the keyboard state.
 // any other button (keyboard)
  MK_OBUTTON = 0x80,
 // any other button (keyboard) went up
  MK_OBUTTON_UP = 0x1000
};
// mask to test to see if some button (physical mouse, not logical)
// is currently pressed...
#define MK_SOMEBUTTON       (MK_LBUTTON|MK_RBUTTON|MK_MBUTTON)
// test to see if any button is clicked */
#define MAKE_SOMEBUTTONS(b)     ((b)&(MK_SOMEBUTTON))
// test to see if a specific button is clicked
#define BUTTON_STILL_DOWN(b,button)     ((b)&(button))
// test a button variable to see if no buttons are currently pressed
// NOBUTTON, NOBUTTONS may be confusing, consider renaming these....
#define MAKE_NOBUTTONS(b)     ( !((b) & MK_SOMEBUTTON ) )
// break of some button
#define BREAK_NEWBUTTON(b,_b) ((((b)^(_b))&(_b))&MK_SOMEBUTTON)
// break a specific button (the last up of the button)
#define BREAK_A_BUTTON(b,_b,button) ((((b)^(_b))&(_b))&(button))
// make of some button (the first down of a button)
#define MAKE_NEWBUTTON(b,_b) ((((b)^(_b))&(b))&MK_SOMEBUTTON)
// make a specific button (the first down of the button)
#define MAKE_A_BUTTON(b,_b,button) ((((b)^(_b))&(b))&(button))
// test current b vs prior _b to see if the  last button pressed is
// now not pressed...
#define BREAK_LASTBUTTON(b,_b)  ( BREAK_NEWBUTTON(b,_b) && MAKE_NOBUTTONS(b) )
// test current b vs prior _b to see if there is now some button pressed
// when previously there were no buttons pressed...
#define MAKE_FIRSTBUTTON(b,_b) ( MAKE_NEWBUTTON(b,_b) && MAKE_NOBUTTONS(_b) )
// these button states may reflect the current
// control, alt, shift key states.  There may be further
// definitions (meta?) And as of the writing of this comment
// these states may not be counted on, if you care about these
// please do validate that the code gives them to you all the way
// from the initial mouse message through all layers to the final
// application handler.
//----------------------------------------------------------
enum DisplayAttributes {
   /* when used by the Display Lib manager, this describes how to manage the subsurface */
  PANEL_ATTRIBUTE_ALPHA    = 0x10000,
   /* when used by the Display Lib manager, this describes how to manage the subsurface */
  PANEL_ATTRIBUTE_HOLEY    = 0x20000,
// when used by the Display Lib manager, this describes how to manage the subsurface
// focus on this window excludes any of it's parent/sibling panels
// from being able to focus.
  PANEL_ATTRIBUTE_EXCLUSIVE = 0x40000,
// when used by the Display Lib manager, this describes how to manage the subsurface
// child attribute affects the child is contained within this parent
  PANEL_ATTRIBUTE_INTERNAL  = 0x88000,
    // open the window as layered - allowing full transparency.
  DISPLAY_ATTRIBUTE_LAYERED = 0x0100,
    // window will not be in alt-tab list
  DISPLAY_ATTRIBUTE_CHILD = 0x0200,
    // set to WS_EX_TRANSPARENT - all mouse is passed, regardless of alpha/shape
  DISPLAY_ATTRIBUTE_NO_MOUSE = 0x0400,
    // when created, the display does not attempt to set itself into focus, otherwise we try to focus self.
  DISPLAY_ATTRIBUTE_NO_AUTO_FOCUS = 0x0800,
  // when created, set topmost as soon as possible
  DISPLAY_ATTRIBUTE_TOPMOST = 0x1000,
};
 // does not HAVE to be called but may
    RENDER_PROC( int , InitDisplay) (void);
	 // this generates a mouse event though the mouse system directly
    // there is no queuing, and the mouse is completed before returning.
    RENDER_PROC( void, GenerateMouseRaw)( int32_t x, int32_t y, uint32_t b );
	 /* Create mouse events to self?
	    Parameters
	    x :  x of the mouse
	    y :  y of the mouse
	    b :  buttons of the mouse    */
	 RENDER_PROC( void, GenerateMouseDeltaRaw )( int32_t x, int32_t y, uint32_t b );
    /* Sets the title of the application window. Once upon a time,
       applications only were able to make a SINGLE window. Internally,
       all windows are mounted against a hidden application window,
       and this appilcation window gets the title.
       Parameters
       title :  Title for the application                               */
    RENDER_PROC( void , SetApplicationTitle) (const TEXTCHAR *title );
    /* Sets the title of the window (shows up in windows when
       alt-tabbing). Also shows up on the task tray icon (if there
       is one)
       Parameters
       render :  display to set the title of
       title :   new text for the title.                           */
    RENDER_PROC( void , SetRendererTitle) ( PRENDERER render, const TEXTCHAR *title );
    /* Sets the icon to show for the application's window.
       Parameters
       Icon :  this really has to be an HICON I think... it's for
               setting the icon on Windows' windows.              */
    RENDER_PROC( void , SetApplicationIcon)  (Image Icon);
    /* Gets the size of the default desktop screen.
       Parameters
       width :   pointer to a 32 value for the display's width.
       height :  pointer to a 32 value for the display's height.
       Example
       <code lang="c++">
       uint32_t w, h;
       GetDisplaySize( &amp;w, &amp;h );
       </code>
       See Also
       <link sack::image::render::GetDisplaySizeEx@int@int32_t *@int32_t *@uint32_t *@uint32_t *, GetDisplaySizeEx> */
    RENDER_PROC( void , GetDisplaySize)      ( uint32_t *width, uint32_t *height );
	 /* \ \
	    Parameters
	    nDisplay :  display to get the coordinates of. 0 is the
	                default display from GetDesktopWindow(). 1\-n are
	                displays for multiple display systems, 1,2,3,4
	                etc..
	    x :         left screen coordinate of this display
	    y :         top screen coordinate of this display
	    width :     how wide this display is
	    height :    how tall this display is
	    Example
	    <code lang="c#">
	    int32_t x, y;
	    uint32_t w, h;
	    GetDisplaySizeEx( 1, &amp;x, &amp;y, &amp;w, &amp;h );
	    </code>                                                       */
	 RENDER_PROC (void, GetDisplaySizeEx) ( int nDisplay
													  , int32_t *x, int32_t *y
													  , uint32_t *width, uint32_t *height);
    /* Sets the first displayed physical window to a certain size. This
       should actually adjust the screen size. Like GetDisplaySize
       \returns the size of the actual display, this should set the
       size of the actual display.
       Parameters
       width :   new width of the screen
       height :  new height of the screen.                              */
    RENDER_PROC( void , SetDisplaySize)      ( uint32_t width, uint32_t height );
#ifdef WIN32
    /* Enable logging when updates happen to the real display.
       Parameters
       bEnable :  TRUE to enable, FALSE to disable.            */
    RENDER_PROC (void, EnableLoggingOutput)( LOGICAL bEnable );
	 /* A method to promote any arbitrary HWND to a PRENDERER. This
	    can be used to put SACK display surfaces in .NET
	    applications.
	    Parameters
	    hWnd :  HWND to make into a renderer.
	    Returns
	    PRENDERER new renderer that uses HWND to update to.         */
	 RENDER_PROC (PRENDERER, MakeDisplayFrom) (HWND hWnd);
#endif
    /* This opens a display for output. It is opened hidden, so the
       application might draw to its surface before it is shown.
       This is not the most capable creation routine, but it is the
       most commonly aliased.
       Parameters
       attributes :  one or more <link sack::image::render::DisplayAttributes, DisplayAttributes>
                     or'ed togeteher.
       width :       width of the display
       height :      height of the display
       x :           x position of left the display
       y :           y position of the top of the display                                         */
    RENDER_PROC( PRENDERER, OpenDisplaySizedAt)     ( uint32_t attributes, uint32_t width, uint32_t height, int32_t x, int32_t y );
    /* This opens a display for output. It is opened hidden, so the
       application might draw to its surface before it is shown.
       This is not the most capable creation routine, but it is the
       most commonly aliased.
       Parameters
       attributes :  one or more <link sack::image::render::DisplayAttributes, DisplayAttributes>
                     or'ed togeteher.
       width :       width of the display
       height :      height of the display
       x :           x position of left the display
       y :           y position of the top of the display
       above :       display to put this one above.                                               */
    RENDER_PROC( PRENDERER, OpenDisplayAboveSizedAt)( uint32_t attributes, uint32_t width, uint32_t height, int32_t x, int32_t y, PRENDERER above );
    /* This opens a display for output. It is opened hidden, so the
       application might draw to its surface before it is shown.
       This is not the most capable creation routine, but it is the
       most commonly aliased.
       Parameters
       attributes :  one or more <link sack::image::render::DisplayAttributes, DisplayAttributes>
                     or'ed togeteher.
       width :       width of the display
       height :      height of the display
       x :           x position of left the display
       y :           y position of the top of the display
       above :       display to put this one above.
       below :       display to put this one under. (for building
                     behind a cover window)                                                       */
    RENDER_PROC( PRENDERER, OpenDisplayAboveUnderSizedAt)( uint32_t attributes, uint32_t width, uint32_t height, int32_t x, int32_t y, PRENDERER above, PRENDERER under );
	 /* Sets the alpha level of the overall display window.
	    Parameters
	    hVideo :  display to set the overall fade level on
	    level :   the level of fade from 0 (transparent) to 255
	              (opaque)
	    Example
	    <code lang="c++">
	    PRENDERER render = OpenDisplay( 0 );
	    int i;
	    UpdateDisplay( render );
	    </code>
	    <code>
	    // the window will slowly fade out
	    for( i = 255; i \> 0; i-- )
	    </code>
	    <code lang="c++">
	        SetDisplayFade( render, i );
	    CloseDisplay( render );  // Hiding the display works too, if it is to be reused.
	    </code>                                                                          */
	 RENDER_PROC( void, SetDisplayFade )( PRENDERER hVideo, int level );
    /* closes a display, releasing all resources assigned to it.
       Parameters
       hDisplay :  Render display to close.                      */
    RENDER_PROC( void         , CloseDisplay) ( PRENDERER );
    /* Updates just a portion of a display window. Minimizing the
       size required for screen output greatly increases efficiency.
       Also on vista+, this will update just a portion of a
       transparent display.
       Parameters
       hVideo :  the display to update
       x :       the left coordinate of the region to update
       y :       the top coordinate of the region to update
       width :   the width of the region to update
       height :  the height of the region to update
       DBG_PASS information is used to track who is doing updates
       when update logging is enabled.                               */
    RENDER_PROC( void , UpdateDisplayPortionEx) ( PRENDERER, int32_t x, int32_t y, uint32_t width, uint32_t height DBG_PASS );
/* <combine sack::image::render::UpdateDisplayPortionEx@PRENDERER@int32_t@int32_t@uint32_t@uint32_t height>
   \ \                                                                                      */
#define UpdateDisplayPortion(r,x,y,w,h) UpdateDisplayPortionEx(r,x,y,w,h DBG_SRC )
	 /* Updates the entire surface of a display.
	    Parameters
	    display :  display to update
	    DBG_PASS information is passed for logging writing to
	    physical display.
	                                                          */
	 RENDER_PROC( void , UpdateDisplayEx)        ( PRENDERER DBG_PASS );
#define UpdateDisplay(r) UpdateDisplayEx(r DBG_SRC)
/* Gets the current location and size of a display.
       Parameters
       hVideo :  display to get the position of
       x :       pointer to a signed 32 bit value to get the left
                 edge of the display.
       y :       pointer to a signed 32 bit value to get the top edge
                 of the display.
       width :   pointer to a unsigned 32 bit value to get the width.
       height :  pointer to a unsigned 32 bit value to get the
                 height.                                              */
    RENDER_PROC( void, GetDisplayPosition)   ( PRENDERER, int32_t *x, int32_t *y, uint32_t *width, uint32_t *height );
    /* Moves a display to an absolute position.
       Parameters
       render :  the display to move
       x :       new X coordinate for the left of the display
       y :       new Y coordinate for the top of the display  */
    RENDER_PROC( void , MoveDisplay)          ( PRENDERER, int32_t x, int32_t y );
    /* Moves a display relative to its current position.
       Parameters
       render :  the display to move
       delx :    a signed amount to add to its X coordiante
       dely :    a signed amount ot add to its Y coordinate. ( bigger
                 values go down the screen )                          */
    RENDER_PROC( void , MoveDisplayRel)       ( PRENDERER, int32_t delx, int32_t dely );
    /* Sets the display's current size. If it is different than
       before, will invoke render's redraw callback.
       Parameters
       display :  the display to set the size of
       w :        new width of the display
       h :        new height of the display                     */
    RENDER_PROC( void , SizeDisplay)          ( PRENDERER, uint32_t w, uint32_t h );
    /* Sets the display's current size relative to what it currently
       is. If it is different than before, will invoke render's
       redraw callback.
       Parameters
       display :  the display to set the size of
       w :        signed value to add to current width
       h :        signed value to add to current height              */
    RENDER_PROC( void , SizeDisplayRel)       ( PRENDERER, int32_t delw, int32_t delh );
   /* Change the position and size of a display.
      Parameters
      hVideo :  display to move and size
      x :       new left coordinate of the display
      y :       new top coordinate of the display
      w :       new width of the display
      h :       new height of the display          */
   RENDER_PROC( void, MoveSizeDisplay )( PRENDERER hVideo
                                        , int32_t x, int32_t y
                                        , int32_t w, int32_t h );
   /* Moves and changes the display size relative to its current
      size. All parameters are relative to current.
      Parameters
      hVideo :  display to move and change the size of
      delx :    amount to modify the left coordinate by
      dely :    amount to modify the top coordinate by
      delw :    amount to change the width by
      delh :    amount to change the height by                   */
   RENDER_PROC( void, MoveSizeDisplayRel )( PRENDERER hVideo
                                        , int32_t delx, int32_t dely
                                        , int32_t delw, int32_t delh );
		/* Put the display above another display. This makes sure that
		   the displays are stacked at least in this order.
		   Parameters
		   this_display :  the display to put above another
		   that_display :  the display that will be on the bottom.     */
		RENDER_PROC( void , PutDisplayAbove)      ( PRENDERER this_display, PRENDERER that_display );
      /* put this in container
	   Parameters
	   hVideo :      Display to put into another display surface
	   hContainer :  The new parent window of the hVideo.
	   Example
	   <code lang="c#">
	   Render render = OpenDisplay( 0 );
	   Render parent = OpenDisplay( 0 );
	   PutDisplayIn( render, parent );
	   </code>                                                   */
	 RENDER_PROC (void, PutDisplayIn) (PRENDERER hVideo, PRENDERER hContainer);
    /* Gets the Image from the Render.
       Parameters
       renderer :  the display window to get the surface of.
       Returns
       Image that is the surface of the window to draw to.   */
    RENDER_PROC( Image , GetDisplayImage)     ( PRENDERER );
    /* Sets the close handler callback. Called when a window is
       closed externally.
       Parameters
       hVideo :     display to set the close handler for
       callback :   close method to call when the display is called
       user_data :  user data passed to close method when invoked.  */
    RENDER_PROC( void , SetCloseHandler)      ( PRENDERER, CloseCallback, uintptr_t );
    /* Specifies the mouse event handler for a display.
       Parameters
       hVideo :     display to set the mouse handler for
       callback :   the routine to call when a mouse event happens.
       user_data :  this value is passed to the callback routine when
                    it is called.                                     */
    RENDER_PROC( void , SetMouseHandler)      ( PRENDERER, MouseCallback, uintptr_t );
    /* Specifies the hide event handler for a display.
       Parameters
       hVideo :     display to set the hide handler for
       callback :   the routine to call when a hide event happens.
       user_data :  this value is passed to the callback routine when
                    it is called.                                     */
    RENDER_PROC( void , SetHideHandler)      ( PRENDERER, HideAndRestoreCallback, uintptr_t );
    /* Specifies the restore event handler for a display.
       Parameters
       hVideo :     display to set the restore handler for
       callback :   the routine to call when a restore event happens.
       user_data :  this value is passed to the callback routine when
                    it is called.                                     */
    RENDER_PROC( void , SetRestoreHandler)      ( PRENDERER, HideAndRestoreCallback, uintptr_t );
#ifndef NO_TOUCH
    /* Specifies the touch event handler for a display.
       Parameters
       hVideo :     display to set the touch handler for
       callback :   the routine to call when a touch event happens.
       user_data :  this value is passed to the callback routine when
                    it is called.                                     */
	   RENDER_PROC( void , SetTouchHandler)      ( PRENDERER, TouchCallback, uintptr_t );
#endif
	 /* Sets the function to call when a redraw event is required.
	    Parameters
	    hVideo :     display to set the handler for
	    callback :   function to call when a redraw is required (or
	                 requested).
	    user_data :  this value is passed to the redraw callback.
	    Example
	    See <link render.h>
	    See Also
	    <link sack::image::render::Redraw@PRENDERER, Redraw>        */
	 RENDER_PROC( void , SetRedrawHandler)     ( PRENDERER, RedrawCallback, uintptr_t );
	 // call this to call the callback registered. as appropriate.  Said callback
    // should never be directly called by application.
    RENDER_PROC( void, Redraw )( PRENDERER hVideo );
    /* Sets the keyboard handler callback for a display
       Parameters
       hVideo :     display to receive key events for.
       callback :   callback invoked when a key event happens.
       user_data :  user data passed to the callback when invoked.
       Remarks
       the keyboard handler may make use of the scan code itself for
       PKEYDEFINE structures. There are also a variety of methods
       for checking the 32 bit key value. The value passed to the
       keyboard handler contains most all of the information about
       the state of the keyboard and specific key.                   */
    RENDER_PROC( void , SetKeyboardHandler)   ( PRENDERER, KeyProc, uintptr_t );
    /* Sets a callback handler called when focus is gained or lost
       by the display.
       Parameters
       hVideo :     display to set the event on
       callback :   the user callback to call when focus is lost or
                    gained.
       user_data :  user data passed to the callback when invoked.
       Note
       When the LoseFocusCallback is called, the renderer is the one
       that is getting the focus. This may be you, may be NULL
       (everyone losing focus) or may be another PRENDERER in your
       application.                                                  */
    RENDER_PROC( void , SetLoseFocusHandler)  ( PRENDERER, LoseFocusCallback, uintptr_t );
    /* Undefined */
    RENDER_PROC( void, SetRenderReadCallback )( PRENDERER pRenderer, RenderReadCallback callback, uintptr_t psv );
#if ACTIVE_MESSAGE_IMPLEMENTED
    RENDER_PROC( void , SetDefaultHandler)    ( PRENDERER, GeneralCallback, uintptr_t );
#endif
    /* Receives the current global mouse state, and position in
       screen coordinates.
       Parameters
       x :  pointer to a signed 32 bit value for the mouse X position.
       y :  pointer to a signed 32 bit value for the mouse Y position.
       b :  current state of mouse buttons. See <link sack::image::render::ButtonFlags, ButtonFlags>. */
    RENDER_PROC( void , GetMouseState )        ( int32_t *x, int32_t *y, uint32_t *b );
    /* Gets the current mouse position in screen coordinates.
       Parameters
       x :  pointer to a signed 32 bit value for the mouse position
       y :  pointer to a signed 32 bit value for the mouse position
       Example
       <code lang="c++">
       int32_t x, y;
       GetMousePosition( &amp;x, &amp;y );
       </code>                                                      */
    RENDER_PROC( void , GetMousePosition)     ( int32_t *x, int32_t *y );
    /* Sets the mouse pointer at the specified display coordinates.
       Parameters
       hDisplay :  display to use to where to position the mouse. Will
                   fault if NULL is passed.
       x :         x relative to the display to set the mouse
       y :         y relative to the display to set the mouse          */
    RENDER_PROC( void , SetMousePosition)     ( PRENDERER, int32_t x, int32_t y );
    /* Test a display to see if it is focused.
       Parameters
       hVideo :  display to check to see if it has focus. (keyboard
                 \input)
       Returns
       TRUE if focused, else FALSE.                                 */
    RENDER_PROC( LOGICAL , HasFocus)          ( PRENDERER );
#if ACTIVE_MESSAGE_IMPLEMENTED
    RENDER_PROC( int, SendActiveMessage)     ( PRENDERER dest, PACTIVEMESSAGE msg );
    RENDER_PROC( PACTIVEMESSAGE , CreateActiveMessage) ( int ID, int size, ... );
#endif
    /* Translates a key's scancode into text. Handles things like
       capslock, shift...
       Parameters
       key :  KEY_ to translate
       Returns
       char that the key represents. (should implement a method to
       get back the UNICODE character).                            */
    RENDER_PROC( const TEXTCHAR *, GetKeyText)             ( int key );
    /* Simple check to see if a key is in a pressed state.
       Parameters
       display :  display to check the key state in
       key :      KEY_ symbol to check.                    */
    RENDER_PROC( uint32_t, IsKeyDown )              ( PRENDERER display, int key );
    /* \ \
       Parameters
       display :  display to test the key status in
       key :      KEY_ symbol to check if the key is pressed
       Returns
       TRUE if the key is down, else FALSE.                  */
    RENDER_PROC( uint32_t, KeyDown )                ( PRENDERER display, int key );
    /* Sometimes displays can be closed by external forces (the
       close button on most windows). This tests to see if a display
       is still valid, or if it has been closed externally.
       Returns
       TRUE if display is still okay. FALSE if the display is no
       longer able to be used.
       Parameters
       display :  the display to check the validity of.              */
    RENDER_PROC( LOGICAL, DisplayIsValid )     ( PRENDERER display );
    /* Assigns all mouse input to a window. This allows the window
       to process messages which are outside of itself normally.
       Parameters
       display :  which window wants to own the mouse
       own :      1 to own, 0 to release ownership.                */
    RENDER_PROC( void, OwnMouseEx )            ( PRENDERER display, uint32_t bOwn DBG_PASS );
    /* Proprietary routine for reading touch screen serial devices
       directly and performing self calibration. Should rely on
       system driver and it's calibration instead.                 */
    RENDER_PROC( int, BeginCalibration )       ( uint32_t points );
    /* Used when display is accessed via a remote message pipe, this
       allows all render operations to be flushed and processed.
       Parameters
       display :  display to flush                                   */
    RENDER_PROC( void, SyncRender )            ( PRENDERER display );
/* Makes a display topmost. There isn't a way to un-topmost a
   window.
   Parameters
   hVideo :  display to make topmost
   Note
   Windows maintains at least two distinct stacks of windows. Normal
   windows in the normal window stack, and a set of windows that
   are above all other windows (except other windows that are
   also topmost).                                                    */
RENDER_PROC( void, MakeTopmost )( PRENDERER hVideo );
/* This makes the display topmost, but more so, any window that
   gets put over it it will attempt put itself over it.
   Parameters
   hVideo :  display to make top top most.                      */
RENDER_PROC (void, MakeAbsoluteTopmost) (PRENDERER hVideo);
/* Tests a display to see if it is set as topmost.
   Parameters
   hVideo :  display to inquire if it's topmost.
   Returns
   TRUE if display is topmost, else FALSE.         */
RENDER_PROC( int, IsTopmost )( PRENDERER hVideo );
/* Hides a display. That is, the content no longer shows on the
   users display.
   Parameters
   hVideo :  the handle of the Render to hide.
   See Also
   <link sack::image::render::RestoreDisplay@PRENDERER, RestoreDisplay> */
RENDER_PROC( void, HideDisplay )( PRENDERER hVideo );
/* Puts a display back on the screen. This is used in
   conjunction with HideDisplay().
   Parameters
   hVideo :  display to restore                       */
RENDER_PROC( void, RestoreDisplay )( PRENDERER hVideo );
	RENDER_PROC( void, RestoreDisplayEx )( PRENDERER hVideo DBG_PASS );
#define RestoreDisplay(n) RestoreDisplayEx( n DBG_SRC )
/* A check to see if HideDisplay has been applied to the
   display.
   Returns
   TRUE if the display is hidden, otherwise FALSE.
   Parameters
   video :  the display to check if hidden               */
RENDER_PROC( LOGICAL, IsDisplayHidden )( PRENDERER video );
// set focus to display, no events are generated if display already
// has the focus.
RENDER_PROC( void, ForceDisplayFocus )( PRENDERER display );
// display set as topmost within it's group (normal/bottommost/topmost)
RENDER_PROC( void, ForceDisplayFront )( PRENDERER display );
// display is force back one layer... or forced to bottom?
// alt-n pushed the display to the back... alt-tab is different...
RENDER_PROC( void, ForceDisplayBack )( PRENDERER display );
/* Not implemented on windows native, is for getting back
   display information over message service abstraction.
   if a readcallback is enabled, then this will be no-wait, and
   one will expect to receive the read data in the callback.
   Otherwise this will return any data which is present already,
   also non wait. Returns length read, INVALID_INDEX if no data
   read.
   If there IS a read callback, return will be 1 if there was no
   previous read queued, and 0 if there was already a read
   pending there may be one and only one read queued (for now)
   In either case if the read could not be queued, it will be
   0..
   If READLINE is true - then the result of the read will be a
   completed line. if there is no line present, and no callback
   defined, this will return INVALID_INDEX characters... 0
   characters is a n only (in line mode) 0 will be returned for
   no characters in non line mode...
   it will not have the end of line terminator (as generated by
   a non-bound enter key) I keep thinking there must be some
   kinda block mode read one can do, but no, uhh no, there's no
   way to get the user to put in X characters exactly....?
   Parameters
   pRenderer :  display to read from
   buffer :     buffer to read into
   maxlen :     maximum length of buffer to read
   bReadLine :  ???                                              */
RENDER_PROC( uint32_t, ReadDisplayEx )( PRENDERER pRenderer, TEXTSTR buffer, uint32_t maxlen, LOGICAL bReadLine );
/* Unused. Incomplete. */
#define ReadDisplay(r,b,len)      ReadDisplayEx(r,b,len,FALSE)
/* Unused. Incomplete. */
#define ReadDisplayLine(r,b,len)  ReadDisplayEx(r,b,len,TRUE)
/* Issues an update to a layered (transparent) window. This does
   the update directly, and does not have to be done within the
   redraw event.
   Parameters
   hVideo :    display to update a part of
   bContent :  TRUE is only the passed rectangle should update
   x :         left coordinate of the region to update to
               physical display
   y :         top coordinate of the region to update to physical
               display
   w :         width of the region to update to physical display
   h :         height of the region to update to physical display */
RENDER_PROC( void, IssueUpdateLayeredEx )( PRENDERER hVideo, LOGICAL bContent, int32_t x, int32_t y, uint32_t w, uint32_t h DBG_PASS );
#ifndef KEY_STRUCTURE_DEFINED
typedef LOGICAL (CPROC*KeyTriggerHandler)(uintptr_t,uint32_t keycode);
typedef struct KeyDefine *PKEYDEFINE;
#endif
/* Can create an external key binder to store key event
   bindings. One of these is available per display.
   Example
   <code lang="c++">
   void Alt_A_Pressed(uintptr_t user_data,uint32_t keycode)
   {
       // do something when alt-a is pressed.
   }
   {
      PKEYDEFINE my_key_events = CreateKeyBinder();
      BindKeyToEventEx( my_key_events, KEY_A, KEY_MOD_ALT, Alt_A_Pressed, 0 );
   }
   // then later, in a KeyProc handler...
   HandleKeyEvents( my_key_events, keycode );
   </code>                                                                     */
RENDER_PROC( PKEYDEFINE, CreateKeyBinder )( void );
/* Destroyes a PKEYDEFINE previously created with
   CreateKeyBinder.
   Parameters
   pKeyDef :  key binder to destroy.              */
RENDER_PROC( void, DestroyKeyBinder )( PKEYDEFINE pKeyDef );
/* Evaluates a key against the key defines to trigger possible
   events.
   Parameters
   KeyDefs :  PKEYDEFINE keystate which has keys bound to it.
   keycode :  the keycode passed to a KeyProc handler.         */
RENDER_PROC( int, HandleKeyEvents )( PKEYDEFINE KeyDefs, uint32_t keycode );
/* Assigns a callback routine to a key event.
   Parameters
   KeyDefs :   pointer to key table to set event in
   scancode :  scancode of the key \- this is a KEY_ code from
               keybrd.h
   modifier :  specific modifiers pressed for this event (control,
               alt, shift)
   trigger :   the trigger function to invoke when the key is
               pressed
   psv :       a uintptr_t user data passed to the trigger function
               when invoked.                                       */
RENDER_PROC( int, BindEventToKeyEx )( PKEYDEFINE KeyDefs, uint32_t scancode, uint32_t modifier, KeyTriggerHandler trigger, uintptr_t psv );
/* Binds a key to a display.
   Parameters
   pRenderer :  display to set the event in (each display has a
                PKEYDEFINE internally. If this is NULL, then the
                event is bound to global events, an applies for
                any display window that gets a key input.
   scancode :   key scancode (a KEY_ identifier from keybrd.h)
   modifier :   key state modifier to apply to match the trigger
                on (control, alt, shift)
   trigger :    callback to invoke when the key combination is
                pressed
   psv :        user data to pass to the trigger when invoked.   */
RENDER_PROC( int, BindEventToKey )( PRENDERER pRenderer, uint32_t scancode, uint32_t modifier, KeyTriggerHandler trigger, uintptr_t psv );
/* Remove a previous binding to a key.
   Parameters
   pRenderer :  renderer to remove the key bind from
   scancode :   key scancode to stop checking
   modifier :   key modifier to stop checking        */
RENDER_PROC( int, UnbindKey )( PRENDERER pRenderer, uint32_t scancode, uint32_t modifier );
/* A way to test to see if the current input device is a touch
   display. This can affect how mouse clicks are handles for
   things like buttons.
   Parameters
   None.
   Returns
   0.                                                          */
RENDER_PROC( int, IsTouchDisplay )( void );
// static void OnInputTouch( "Touch Handler" )(
#define OnSurfaceInput(name)	 DefineRegistryMethod("sack/render",SurfaceInput,"surface input","SurfaceInput",name,void,( int nInputs, PINPUT_POINT pInputs ),__LINE__)
#ifndef PSPRITE_METHOD
/* Unused. Incomplete. */
#define PSPRITE_METHOD PSPRITE_METHOD
RENDER_NAMESPACE_END
IMAGE_NAMESPACE
   /* define sprite draw method structure */
	typedef struct sprite_method_tag *PSPRITE_METHOD;
IMAGE_NAMESPACE_END
RENDER_NAMESPACE
#endif
/* Adds a sprite rendering method to the display. Just before
   updating to the display, the display is saved, and sprite
   update callbacks are issued. then the resulting display is
   \output. Sprite data only exists on the output image just
   before it is put on the physical display.
   Parameters
   render :    the display to attach a sprite render method to
   callback :  callback to draw sprites
   psv :       user data passed to callback when it is called
   Returns
   Pointer to a SpriteMethod that can be used in SavePortion...
   uhmm
   Note
   Has fallen into disrepair, and may need work before sprites
   work this way.                                               */
RENDER_PROC( PSPRITE_METHOD, EnableSpriteMethod )(PRENDERER render, void(CPROC*RenderSprites)(uintptr_t psv, PRENDERER renderer, int32_t x, int32_t y, uint32_t w, uint32_t h ), uintptr_t psv );
/* signature for callback method to pass to
   WinShell_AcceptDroppedFiles.             */
typedef LOGICAL (CPROC*dropped_file_acceptor)(uintptr_t psv, CTEXTSTR filename, int32_t x, int32_t y );
/* Adds a callback to call when a file is dropped. Each callback
   can return 0 that it did not accept the file, or 1 that it
   did. once the file is accepted by a handler, it is not passed
   to any other handlers.
   Parameters
   renderer :  display to handle dropped files for
   f :         callback to acceptor
   psvUser :   user data passed to acceptor when it is invoked   */
RENDER_PROC( void, WinShell_AcceptDroppedFiles )( PRENDERER renderer, dropped_file_acceptor f, uintptr_t psvUser );
/* Enables a timer on the mouse to hide the cursor after a
   second that the mouse is not being moved.
   Parameters
   hVideo :   display to hide the mouse automatically for
   bEnable :  enable automatic hiding. After a few seconds, the
              mouse goes away until it moves(not click).        */
RENDER_PROC (void, DisableMouseOnIdle) (PRENDERER hVideo, LOGICAL bEnable );
/* Sets whether the display wants to get any mouse events at
   all.
   Parameters
   hVideo :    display to set the property for
   bNoMouse :  if 1, disables any mouse events. if 0, enables mouse
               events to the display.                               */
RENDER_PROC( void, SetDisplayNoMouse )( PRENDERER hVideo, int bNoMouse );
#ifdef WIN32
	/* \returns the native handle used to output to. this can be an
	   SDL_Screen or HWND depending on platform.
	   Parameters
	   video :  display to get the native handle for
	   Returns
	   the system handle of the display object being used to output. */
	RENDER_PROC( HWND, GetNativeHandle )( PRENDERER video );
#endif
/* <combine sack::image::render::OwnMouseEx@PRENDERER@uint32_t bOwn>
   \ \                                                          */
#define OwnMouse(d,o) OwnMouseEx( d, o DBG_SRC )
/* <combine sack::image::render::OpenDisplaySizedAt@uint32_t@uint32_t@uint32_t@int32_t@int32_t>
   \ \                                                                     */
#define OpenDisplay(a)            OpenDisplaySizedAt(a,-1,-1,-1,-1)
/* <combine sack::image::render::OpenDisplaySizedAt@uint32_t@uint32_t@uint32_t@int32_t@int32_t>
   \ \                                                                     */
#define OpenDisplaySized(a,w,h)   OpenDisplaySizedAt(a,w,h,-1,-1)
/* <combine sack::image::render::OpenDisplayAboveSizedAt@uint32_t@uint32_t@uint32_t@int32_t@int32_t@PRENDERER>
   \ \                                                                                    */
#define OpenDisplayAbove(p,a)            OpenDisplayAboveSizedAt(p,-1,-1,-1,-1,a)
/* <combine sack::image::render::OpenDisplayAboveSizedAt@uint32_t@uint32_t@uint32_t@int32_t@int32_t@PRENDERER>
   \ \                                                                                    */
#define OpenDisplayAboveSized(p,a,w,h)   OpenDisplayAboveSizedAt(p,w,h,-1,-1,a)
/* <combine sack::image::render::OpenDisplayAboveUnderSizedAt@uint32_t@uint32_t@uint32_t@int32_t@int32_t@PRENDERER@PRENDERER>
   \ \                                                                                                   */
#define OpenDisplayUnderSizedAt(p,a,w,h,x,y) OpenDisplayAboveUnderSizedAt(a,w,h,x,y,NULL,p)
/* Lock the renderer for this thread to use. */
RENDER_PROC( void, LockRenderer )( PRENDERER render );
/* Unlock the renderer for other threads to use. */
RENDER_PROC( void, UnlockRenderer )( PRENDERER render );
/* Function to check if the draw mode of the renderer requires
   an ALL update (opengl/direct3d) every frame the whole display
   must be drawn.                                                */
RENDER_PROC( LOGICAL, RequiresDrawAll )( void );
RENDER_PROC( void, MarkDisplayUpdated )( PRENDERER );
#ifndef __NO_INTERFACES__
/* Interface defines the functions that are exported from the
   render library. This interface may be retrieved with
   LoadInterface( "\<appropriate name" ).                     */
_INTERFACE_NAMESPACE
/* Macro to define exports for render.h */
#define RENDER_PROC_PTR(type,name) type  (CPROC*_##name)
/* <combine sack::image::render::render_interface_tag>
	\ \                                                 */
typedef struct render_interface_tag RENDER_INTERFACE;
/* <combine sack::image::render::render_interface_tag>
	\ \                                                 */
typedef struct render_interface_tag *PRENDER_INTERFACE;
/* This is a function table interface to the video library. Allows
   application to not be linked to the video portion directly,
   allowing dynamic replacement.                                   */
struct render_interface_tag
{
      /* <combine sack::image::render::InitDisplay>
         \ \                                        */
       RENDER_PROC_PTR( int , InitDisplay) (void);
       /* <combine sack::image::render::SetApplicationTitle@TEXTCHAR *>
          \ \                                                           */
			 RENDER_PROC_PTR( void , SetApplicationTitle) (const TEXTCHAR *title );
          /* <combine sack::image::render::SetApplicationIcon@Image>
                                                    \ \                                                     */
       RENDER_PROC_PTR( void , SetApplicationIcon)  (Image Icon);
    /* <combine sack::image::render::GetDisplaySize@uint32_t *@uint32_t *>
       \ \                                                       */
    RENDER_PROC_PTR( void , GetDisplaySize)      ( uint32_t *width, uint32_t *height );
    /* <combine sack::image::render::SetDisplaySize@uint32_t@uint32_t>
       \ \                                                   */
    RENDER_PROC_PTR( void , SetDisplaySize)      ( uint32_t width, uint32_t height );
    /* <combine sack::image::render::OpenDisplaySizedAt@uint32_t@uint32_t@uint32_t@int32_t@int32_t>
       \ \                                                                     */
    RENDER_PROC_PTR( PRENDERER , OpenDisplaySizedAt)     ( uint32_t attributes, uint32_t width, uint32_t height, int32_t x, int32_t y );
    /* <combine sack::image::render::OpenDisplayAboveSizedAt@uint32_t@uint32_t@uint32_t@int32_t@int32_t@PRENDERER>
       \ \                                                                                    */
    RENDER_PROC_PTR( PRENDERER , OpenDisplayAboveSizedAt)( uint32_t attributes, uint32_t width, uint32_t height, int32_t x, int32_t y, PRENDERER above );
    /* <combine sack::image::render::CloseDisplay@PRENDERER>
       \ \                                                   */
    RENDER_PROC_PTR( void        , CloseDisplay) ( PRENDERER );
    /* <combine sack::image::render::UpdateDisplayPortionEx@PRENDERER@int32_t@int32_t@uint32_t@uint32_t height>
       \ \                                                                                      */
    RENDER_PROC_PTR( void, UpdateDisplayPortionEx) ( PRENDERER, int32_t x, int32_t y, uint32_t width, uint32_t height DBG_PASS );
    /* <combine sack::image::render::UpdateDisplayEx@PRENDERER>
       \ \                                                      */
    RENDER_PROC_PTR( void, UpdateDisplayEx)        ( PRENDERER DBG_PASS);
    /* <combine sack::image::render::GetDisplayPosition@PRENDERER@int32_t *@int32_t *@uint32_t *@uint32_t *>
       \ \                                                                                   */
    RENDER_PROC_PTR( void, GetDisplayPosition)   ( PRENDERER, int32_t *x, int32_t *y, uint32_t *width, uint32_t *height );
    /* <combine sack::image::render::MoveDisplay@PRENDERER@int32_t@int32_t>
       \ \                                                            */
    RENDER_PROC_PTR( void, MoveDisplay)          ( PRENDERER, int32_t x, int32_t y );
    /* <combine sack::image::render::MoveDisplayRel@PRENDERER@int32_t@int32_t>
       \ \                                                               */
    RENDER_PROC_PTR( void, MoveDisplayRel)       ( PRENDERER, int32_t delx, int32_t dely );
    /* <combine sack::image::render::SizeDisplay@PRENDERER@uint32_t@uint32_t>
       \ \                                                          */
    RENDER_PROC_PTR( void, SizeDisplay)          ( PRENDERER, uint32_t w, uint32_t h );
    /* <combine sack::image::render::SizeDisplayRel@PRENDERER@int32_t@int32_t>
       \ \                                                               */
    RENDER_PROC_PTR( void, SizeDisplayRel)       ( PRENDERER, int32_t delw, int32_t delh );
    /* <combine sack::image::render::MoveSizeDisplayRel@PRENDERER@int32_t@int32_t@int32_t@int32_t>
       \ \                                                                             */
    RENDER_PROC_PTR( void, MoveSizeDisplayRel )  ( PRENDERER hVideo
                                                 , int32_t delx, int32_t dely
                                                 , int32_t delw, int32_t delh );
    RENDER_PROC_PTR( void, PutDisplayAbove)      ( PRENDERER, PRENDERER );
 /* <combine sack::image::render::PutDisplayAbove@PRENDERER@PRENDERER>
                                                              \ \                                                                */
    /* <combine sack::image::render::GetDisplayImage@PRENDERER>
       \ \                                                      */
    RENDER_PROC_PTR( Image, GetDisplayImage)     ( PRENDERER );
    /* <combine sack::image::render::SetCloseHandler@PRENDERER@CloseCallback@uintptr_t>
       \ \                                                                             */
    RENDER_PROC_PTR( void, SetCloseHandler)      ( PRENDERER, CloseCallback, uintptr_t );
    /* <combine sack::image::render::SetMouseHandler@PRENDERER@MouseCallback@uintptr_t>
       \ \                                                                             */
    RENDER_PROC_PTR( void, SetMouseHandler)      ( PRENDERER, MouseCallback, uintptr_t );
    /* <combine sack::image::render::SetRedrawHandler@PRENDERER@RedrawCallback@uintptr_t>
       \ \                                                                               */
    RENDER_PROC_PTR( void, SetRedrawHandler)     ( PRENDERER, RedrawCallback, uintptr_t );
    /* <combine sack::image::render::SetKeyboardHandler@PRENDERER@KeyProc@uintptr_t>
       \ \                                                                          */
    RENDER_PROC_PTR( void, SetKeyboardHandler)   ( PRENDERER, KeyProc, uintptr_t );
    /* <combine sack::image::render::SetLoseFocusHandler@PRENDERER@LoseFocusCallback@uintptr_t>
       \ \                                                                                     */
    RENDER_PROC_PTR( void, SetLoseFocusHandler)  ( PRENDERER, LoseFocusCallback, uintptr_t );
    /* <combine sack::image::render::SetDefaultHandler@PRENDERER@GeneralCallback@uintptr_t>
       \ \                                                                                 */
#if ACTIVE_MESSAGE_IMPLEMENTED
			 RENDER_PROC_PTR( void, SetDefaultHandler)    ( PRENDERER, GeneralCallback, uintptr_t );
#else
       POINTER junk1;
#endif
    /* <combine sack::image::render::GetMousePosition@int32_t *@int32_t *>
		 \ \                                                           */
    RENDER_PROC_PTR( void, GetMousePosition)     ( int32_t *x, int32_t *y );
    /* <combine sack::image::render::SetMousePosition@PRENDERER@int32_t@int32_t>
       \ \                                                                 */
    RENDER_PROC_PTR( void, SetMousePosition)     ( PRENDERER, int32_t x, int32_t y );
    /* <combine sack::image::render::HasFocus@PRENDERER>
       \ \                                               */
    RENDER_PROC_PTR( LOGICAL, HasFocus)          ( PRENDERER );
    /* <combine sack::image::render::GetKeyText@int>
       \ \                                           */
    RENDER_PROC_PTR( const TEXTCHAR *, GetKeyText)           ( int key );
    /* <combine sack::image::render::IsKeyDown@PRENDERER@int>
       \ \                                                    */
    RENDER_PROC_PTR( uint32_t, IsKeyDown )        ( PRENDERER display, int key );
    /* <combine sack::image::render::KeyDown@PRENDERER@int>
       \ \                                                  */
    RENDER_PROC_PTR( uint32_t, KeyDown )         ( PRENDERER display, int key );
    /* <combine sack::image::render::DisplayIsValid@PRENDERER>
       \ \                                                     */
    RENDER_PROC_PTR( LOGICAL, DisplayIsValid )  ( PRENDERER display );
    /* <combine sack::image::render::OwnMouseEx@PRENDERER@uint32_t bOwn>
       \ \                                                          */
    RENDER_PROC_PTR( void, OwnMouseEx )            ( PRENDERER display, uint32_t Own DBG_PASS);
    /* <combine sack::image::render::BeginCalibration@uint32_t>
       \ \                                                 */
    RENDER_PROC_PTR( int, BeginCalibration )       ( uint32_t points );
    /* <combine sack::image::render::SyncRender@PRENDERER>
       \ \                                                 */
    RENDER_PROC_PTR( void, SyncRender )            ( PRENDERER pDisplay );
    /* DEPRICATED; left in structure for compatibility.  Removed define and export definition. */
	 /* <combine sack::image::render::MoveSizeDisplay@PRENDERER@int32_t@int32_t@int32_t@int32_t>
	    \ \                                                                          */
	 RENDER_PROC_PTR( void, MoveSizeDisplay )( PRENDERER hVideo
                                        , int32_t x, int32_t y
                                        , int32_t w, int32_t h );
   /* <combine sack::image::render::MakeTopmost@PRENDERER>
      \ \                                                  */
   RENDER_PROC_PTR( void, MakeTopmost )    ( PRENDERER hVideo );
   /* <combine sack::image::render::HideDisplay@PRENDERER>
      \ \                                                  */
   RENDER_PROC_PTR( void, HideDisplay )      ( PRENDERER hVideo );
   /* <combine sack::image::render::RestoreDisplay@PRENDERER>
      \ \                                                     */
   RENDER_PROC_PTR( void, RestoreDisplay )   ( PRENDERER hVideo );
	/* <combine sack::image::render::ForceDisplayFocus@PRENDERER>
	   \ \                                                        */
	RENDER_PROC_PTR( void, ForceDisplayFocus )( PRENDERER display );
	/* <combine sack::image::render::ForceDisplayFront@PRENDERER>
	   \ \                                                        */
	RENDER_PROC_PTR( void, ForceDisplayFront )( PRENDERER display );
	/* <combine sack::image::render::ForceDisplayBack@PRENDERER>
	   \ \                                                       */
	RENDER_PROC_PTR( void, ForceDisplayBack )( PRENDERER display );
	/* <combine sack::image::render::BindEventToKey@PRENDERER@uint32_t@uint32_t@KeyTriggerHandler@uintptr_t>
	   \ \                                                                                        */
	RENDER_PROC_PTR( int, BindEventToKey )( PRENDERER pRenderer, uint32_t scancode, uint32_t modifier, KeyTriggerHandler trigger, uintptr_t psv );
	/* <combine sack::image::render::UnbindKey@PRENDERER@uint32_t@uint32_t>
	   \ \                                                        */
	RENDER_PROC_PTR( int, UnbindKey )( PRENDERER pRenderer, uint32_t scancode, uint32_t modifier );
	/* <combine sack::image::render::IsTopmost@PRENDERER>
	   \ \                                                */
	RENDER_PROC_PTR( int, IsTopmost )( PRENDERER hVideo );
	/* Used as a point to sync between applications and the message
	   display server; Makes sure that all draw commands which do
	   not have a response are done.
	   Waits until all commands are processed; which is wait until
	   this command is processed.                                   */
	RENDER_PROC_PTR( void, OkaySyncRender )            ( void );
   /* <combine sack::image::render::IsTouchDisplay>
      \ \                                           */
   RENDER_PROC_PTR( int, IsTouchDisplay )( void );
	/* <combine sack::image::render::GetMouseState@int32_t *@int32_t *@uint32_t *>
	   \ \                                                              */
	RENDER_PROC_PTR( void , GetMouseState )        ( int32_t *x, int32_t *y, uint32_t *b );
	/* <combine sack::image::render::EnableSpriteMethod@PRENDERER@void__cdecl*RenderSpritesPTRSZVAL psv\, PRENDERER renderer\, int32_t x\, int32_t y\, uint32_t w\, uint32_t h@uintptr_t>
	   \ \                                                                                                                                                               */
	RENDER_PROC_PTR ( PSPRITE_METHOD, EnableSpriteMethod )(PRENDERER render, void(CPROC*RenderSprites)(uintptr_t psv, PRENDERER renderer, int32_t x, int32_t y, uint32_t w, uint32_t h ), uintptr_t psv );
	/* <combine sack::image::render::WinShell_AcceptDroppedFiles@PRENDERER@dropped_file_acceptor@uintptr_t>
	   \ \                                                                                                 */
	RENDER_PROC_PTR( void, WinShell_AcceptDroppedFiles )( PRENDERER renderer, dropped_file_acceptor f, uintptr_t psvUser );
	/* <combine sack::image::render::PutDisplayIn@PRENDERER@PRENDERER>
	   \ \                                                             */
	RENDER_PROC_PTR(void, PutDisplayIn) (PRENDERER hVideo, PRENDERER hContainer);
#ifdef WIN32
	/* <combine sack::image::render::MakeDisplayFrom@HWND>
	   \ \                                                 */
			RENDER_PROC_PTR (PRENDERER, MakeDisplayFrom) (HWND hWnd) ;
#else
      POINTER junk4;
#endif
	/* <combine sack::image::render::SetRendererTitle@PRENDERER@TEXTCHAR *>
	   \ \                                                                  */
	RENDER_PROC_PTR( void , SetRendererTitle) ( PRENDERER render, const TEXTCHAR *title );
	/* <combine sack::image::render::DisableMouseOnIdle@PRENDERER@LOGICAL>
	   \ \                                                                 */
	RENDER_PROC_PTR (void, DisableMouseOnIdle) (PRENDERER hVideo, LOGICAL bEnable );
	/* <combine sack::image::render::OpenDisplayAboveUnderSizedAt@uint32_t@uint32_t@uint32_t@int32_t@int32_t@PRENDERER@PRENDERER>
	   \ \                                                                                                   */
	RENDER_PROC_PTR( PRENDERER, OpenDisplayAboveUnderSizedAt)( uint32_t attributes, uint32_t width, uint32_t height, int32_t x, int32_t y, PRENDERER above, PRENDERER under );
	/* <combine sack::image::render::SetDisplayNoMouse@PRENDERER@int>
	   \ \                                                            */
	RENDER_PROC_PTR( void, SetDisplayNoMouse )( PRENDERER hVideo, int bNoMouse );
	/* <combine sack::image::render::Redraw@PRENDERER>
	   \ \                                             */
	RENDER_PROC_PTR( void, Redraw )( PRENDERER hVideo );
	/* <combine sack::image::render::MakeAbsoluteTopmost@PRENDERER>
	   \ \                                                          */
	RENDER_PROC_PTR(void, MakeAbsoluteTopmost) (PRENDERER hVideo);
	/* <combine sack::image::render::SetDisplayFade@PRENDERER@int>
	   \ \                                                         */
	RENDER_PROC_PTR( void, SetDisplayFade )( PRENDERER hVideo, int level );
	/* <combine sack::image::render::IsDisplayHidden@PRENDERER>
	   \ \                                                      */
	RENDER_PROC_PTR( LOGICAL, IsDisplayHidden )( PRENDERER video );
#ifdef WIN32
	/* <combine sack::image::render::GetNativeHandle@PRENDERER>
	   \ \                                                      */
	RENDER_PROC_PTR( HWND, GetNativeHandle )( PRENDERER video );
#endif
		 /* <combine sack::image::render::GetDisplaySizeEx@int@int32_t *@int32_t *@uint32_t *@uint32_t *>
		    \ \                                                                           */
		 RENDER_PROC_PTR (void, GetDisplaySizeEx) ( int nDisplay
														  , int32_t *x, int32_t *y
														  , uint32_t *width, uint32_t *height);
	/* Locks a video display. Applications shouldn't be locking
	   this, but if for some reason they require it; use this
	   function.                                                */
	RENDER_PROC_PTR( void, LockRenderer )( PRENDERER render );
	/* Release renderer lock critical section. Applications
	   shouldn't be locking this surface.                   */
	RENDER_PROC_PTR( void, UnlockRenderer )( PRENDERER render );
	/* Provides a way for applications to cause the window to flush
	   to the display (if it's a transparent window)                */
	RENDER_PROC_PTR( void, IssueUpdateLayeredEx )( PRENDERER hVideo, LOGICAL bContent, int32_t x, int32_t y, uint32_t w, uint32_t h DBG_PASS );
	/* Check to see if the render mode is always redraw; changes how
	   smudge works in PSI. If always redrawn, then the redraw isn't
	   done during the smudge, and instead is delayed until a draw
	   is triggered at which time all controls are drawn.
	   Returns
	   TRUE if full screen needs to be drawn during a draw,
	   otherwise partial updates may be done.                        */
	RENDER_PROC_PTR( LOGICAL, RequiresDrawAll )( void );
#ifndef NO_TOUCH
		/* <combine sack::image::render::SetTouchHandler@PRENDERER@fte inc asdfl;kj
		 fteTouchCallback@uintptr_t>
       \ \                                                                             */
			RENDER_PROC_PTR( void, SetTouchHandler)      ( PRENDERER, TouchCallback, uintptr_t );
#endif
    RENDER_PROC_PTR( void, MarkDisplayUpdated )( PRENDERER );
    /* <combine sack::image::render::SetHideHandler@PRENDERER@HideAndRestoreCallback@uintptr_t>
       \ \                                                                             */
    RENDER_PROC_PTR( void, SetHideHandler)      ( PRENDERER, HideAndRestoreCallback, uintptr_t );
    /* <combine sack::image::render::SetRestoreHandler@PRENDERER@HideAndRestoreCallback@uintptr_t>
       \ \                                                                             */
    RENDER_PROC_PTR( void, SetRestoreHandler)      ( PRENDERER, HideAndRestoreCallback, uintptr_t );
		 RENDER_PROC_PTR( void, RestoreDisplayEx )   ( PRENDERER hVideo DBG_PASS );
		 /* added for android extensions; call to enable showing the keyboard in the correct thread
        ; may have applications for windows tablets
		  */
       RENDER_PROC_PTR( void, SACK_Vidlib_ShowInputDevice )( void );
		 /* added for android extensions; call to enable hiding the keyboard in the correct thread
		  ; may have applications for windows tablets */
       RENDER_PROC_PTR( void, SACK_Vidlib_HideInputDevice )( void );
	/* Check to see if the render mode is allows updates from any thread.
	   If supported can simplify updates (requiring less scheduling queues).
	   If it is not supported (such as an X display where only a single thread
	   can write to the server, otherwise the socket state gets confused) then
	   Redraw() should be used to dispatch appriorately.  PSI Implements this
	   internally, so smudge() on a control will behave appriopriately.
	   If RequiresDrawAll() this is irrelavent.
	   Returns
	   TRUE if any thread is allowed to generate UpdateDisplayPortion().
	   otherwise must call Redraw() on the surface to get a event in the
	   correct thread.*/
			 RENDER_PROC_PTR( LOGICAL, AllowsAnyThreadToUpdate )( void );
		/* This method takes the renderer and either A) resizes it to the display
		 and issues a redraw; or it sets the screen to the size of the renderer
		 and scales the image direct to the display.  Pass NULL or another window
		 to clear the current fullscreen app.
		 Second paramter is the display to show full on; 0 is 'default'...
		 1, 2, 3, etc... will be absolute number... if not supported will be same a 0 */
		RENDER_PROC_PTR( void, SetDisplayFullScreen )( PRENDERER renderer, int nDisplay );
		/* like full screen, some applications may want to
       enable owning the screen... (media player) */
      RENDER_PROC_PTR( void, SuspendSystemSleep )( int bool_suspend_enable );
	RENDER_PROC_PTR( LOGICAL, RenderIsInstanced )( void );
	RENDER_PROC_PTR( LOGICAL, VidlibRenderAllowsCopy )( void );
	RENDER_PROC_PTR( void, SetDisplayCursor )( CTEXTSTR nCursor );
	RENDER_PROC_PTR( LOGICAL, IsDisplayRedrawForced )( PRENDERER renderer );
 // only valid during a headless display event....
	RENDER_PROC_PTR( void, ReplyCloseDisplay )( void );
		/* Clipboard Callback */
	RENDER_PROC_PTR( void, SetClipboardEventCallback )(PRENDERER pRenderer, ClipboardCallback callback, uintptr_t psv);
};
#ifdef DEFINE_DEFAULT_RENDER_INTERFACE
#define USE_RENDER_INTERFACE GetDisplayInterface()
#endif
#ifdef FORCE_NO_INTERFACE
#undef USE_RENDER_INTERFACE
#endif
#ifdef FORCE_NO_RENDER_INTERFACE
#undef USE_RENDER_INTERFACE
#endif
#if !defined(FORCE_NO_RENDER_INTERFACE)
/* RENDER_PROC( PRENDER_INTERFACE, GetDisplayInterface )( void
 );
   Gets the interface the proper way - by name.
   Returns
   Pointer to the render interface.                            */
#  define GetDisplayInterface() (PRENDER_INTERFACE)GetInterface( "render" )
/* RENDER_PROC( void, DropDisplayInterface )( PRENDER_INTERFACE interface );
   release the interface (could be cleanup, most are donothing....
   parameters
   interface   - Pointer to the render interface.                            */
#  define DropDisplayInterface(x) DropInterface( "render", x )
#endif
#ifdef USE_RENDER_INTERFACE
typedef int check_this_variable;
// these methods are provided for backwards compatibility
// these should not be used - but rather use the interface defined below
// (the ones not prefixed by ActImage_ - except for ActImage_Init, which
// may(should) be called before any other function.
#define REND_PROC_ALIAS(name) ((USE_RENDER_INTERFACE)->_##name)
#define REND_PROC_ALIAS_VOID(name) if(USE_RENDER_INTERFACE)(USE_RENDER_INTERFACE)->_##name
#define SetApplicationTitle       REND_PROC_ALIAS(SetApplicationTitle)
#define SetRendererTitle       REND_PROC_ALIAS(SetRendererTitle)
#define SetApplicationIcon        REND_PROC_ALIAS(SetApplicationIcon)
#define GetDisplaySize            REND_PROC_ALIAS(GetDisplaySize)
#define GetDisplaySizeEx            REND_PROC_ALIAS(GetDisplaySizeEx)
#define MarkDisplayUpdated            REND_PROC_ALIAS(MarkDisplayUpdated)
#define SetDisplaySize            REND_PROC_ALIAS(SetDisplaySize)
#define GetDisplayPosition        REND_PROC_ALIAS(GetDisplayPosition)
#define IssueUpdateLayeredEx      REND_PROC_ALIAS(IssueUpdateLayeredEx)
#define MakeDisplayFrom        REND_PROC_ALIAS(MakeDisplayFrom)
#define OpenDisplaySizedAt        REND_PROC_ALIAS(OpenDisplaySizedAt)
#define OpenDisplayAboveSizedAt   REND_PROC_ALIAS(OpenDisplayAboveSizedAt)
#define OpenDisplayAboveUnderSizedAt   REND_PROC_ALIAS(OpenDisplayAboveUnderSizedAt)
#define CloseDisplay              REND_PROC_ALIAS(CloseDisplay)
#define UpdateDisplayPortionEx    REND_PROC_ALIAS(UpdateDisplayPortionEx)
#define UpdateDisplayEx             REND_PROC_ALIAS(UpdateDisplayEx)
#define SetMousePosition          REND_PROC_ALIAS(SetMousePosition)
#define GetMousePosition          REND_PROC_ALIAS(GetMousePosition)
#define GetMouseState          REND_PROC_ALIAS(GetMouseState)
#define EnableSpriteMethod          REND_PROC_ALIAS(EnableSpriteMethod)
#define WinShell_AcceptDroppedFiles REND_PROC_ALIAS(WinShell_AcceptDroppedFiles)
#define MoveDisplay               REND_PROC_ALIAS(MoveDisplay)
#define MoveDisplayRel            REND_PROC_ALIAS(MoveDisplayRel)
#define SizeDisplay               REND_PROC_ALIAS(SizeDisplay)
#define Redraw               REND_PROC_ALIAS(Redraw)
#define RequiresDrawAll()        (USE_RENDER_INTERFACE)?((USE_RENDER_INTERFACE)->_RequiresDrawAll()):0
#define SizeDisplayRel            REND_PROC_ALIAS(SizeDisplayRel)
#define MoveSizeDisplay        REND_PROC_ALIAS(MoveSizeDisplay)
#define MoveSizeDisplayRel        REND_PROC_ALIAS(MoveSizeDisplayRel)
#define PutDisplayAbove           REND_PROC_ALIAS(PutDisplayAbove)
#define PutDisplayIn           REND_PROC_ALIAS(PutDisplayIn)
#define GetDisplayImage           REND_PROC_ALIAS(GetDisplayImage)
#define LockRenderer              REND_PROC_ALIAS(LockRenderer)
#define UnlockRenderer              REND_PROC_ALIAS(UnlockRenderer)
#define SetCloseHandler           REND_PROC_ALIAS(SetCloseHandler)
#define SetMouseHandler           REND_PROC_ALIAS(SetMouseHandler)
#define SetHideHandler           REND_PROC_ALIAS(SetHideHandler)
#define SetRestoreHandler           REND_PROC_ALIAS(SetRestoreHandler)
#define AllowsAnyThreadToUpdate()          ((USE_RENDER_INTERFACE)?((USE_RENDER_INTERFACE)->_AllowsAnyThreadToUpdate)?(USE_RENDER_INTERFACE)->_AllowsAnyThreadToUpdate():1:1)
#define VidlibRenderAllowsCopy()        ((USE_RENDER_INTERFACE)?((USE_RENDER_INTERFACE)->_VidlibRenderAllowsCopy)?(USE_RENDER_INTERFACE)->_VidlibRenderAllowsCopy():1:1)
#ifndef __LINUX__
#define SetTouchHandler           REND_PROC_ALIAS(SetTouchHandler)
#endif
#define SetRedrawHandler          REND_PROC_ALIAS(SetRedrawHandler)
#define SetKeyboardHandler        REND_PROC_ALIAS(SetKeyboardHandler)
#define SetLoseFocusHandler       REND_PROC_ALIAS(SetLoseFocusHandler)
#define SetDefaultHandler         REND_PROC_ALIAS(SetDefaultHandler)
#define GetKeyText                REND_PROC_ALIAS(GetKeyText)
#define HasFocus                  REND_PROC_ALIAS(HasFocus)
#define SACK_Vidlib_ShowInputDevice REND_PROC_ALIAS( SACK_Vidlib_ShowInputDevice )
#define SACK_Vidlib_HideInputDevice REND_PROC_ALIAS( SACK_Vidlib_HideInputDevice )
#define CreateMessage             REND_PROC_ALIAS(CreateMessage)
#define SendActiveMessage         REND_PROC_ALIAS(SendActiveMessage)
#define IsKeyDown                 REND_PROC_ALIAS(IsKeyDown)
#define KeyDown                   REND_PROC_ALIAS(KeyDown)
#define DisplayIsValid            REND_PROC_ALIAS(DisplayIsValid)
#define OwnMouseEx                REND_PROC_ALIAS(OwnMouseEx)
#define BeginCalibration          REND_PROC_ALIAS(BeginCalibration)
#define SyncRender                REND_PROC_ALIAS(SyncRender)
#define OkaySyncRender                REND_PROC_ALIAS(OkaySyncRender)
#define HideDisplay               REND_PROC_ALIAS(HideDisplay)
#define IsDisplayHidden               REND_PROC_ALIAS(IsDisplayHidden)
/* <combine sack::image::render::GetNativeHandle@PRENDERER>
   \ \                                                      */
#define GetNativeHandle             REND_PROC_ALIAS(GetNativeHandle)
//#define RestoreDisplay             REND_PROC_ALIAS(RestoreDisplay)
#define RestoreDisplayEx             REND_PROC_ALIAS(RestoreDisplayEx)
#define MakeTopmost               REND_PROC_ALIAS_VOID(MakeTopmost)
#define MakeAbsoluteTopmost               REND_PROC_ALIAS_VOID(MakeAbsoluteTopmost)
#define IsTopmost               REND_PROC_ALIAS(IsTopmost)
#define SetDisplayFade               REND_PROC_ALIAS(SetDisplayFade)
#define ForceDisplayFocus         REND_PROC_ALIAS(ForceDisplayFocus)
#define ForceDisplayFront       REND_PROC_ALIAS(ForceDisplayFront)
#define ForceDisplayBack          REND_PROC_ALIAS(ForceDisplayBack)
#define BindEventToKey          REND_PROC_ALIAS(BindEventToKey)
#define UnbindKey               REND_PROC_ALIAS(UnbindKey)
#define IsTouchDisplay          REND_PROC_ALIAS(IsTouchDisplay)
#define DisableMouseOnIdle      REND_PROC_ALIAS(DisableMouseOnIdle )
#define SetDisplayNoMouse      REND_PROC_ALIAS(SetDisplayNoMouse )
#define SetTouchHandler        REND_PROC_ALIAS(SetTouchHandler)
#define ReplyCloseDisplay      if(USE_RENDER_INTERFACE) if((USE_RENDER_INTERFACE)->_ReplyCloseDisplay) (USE_RENDER_INTERFACE)->_ReplyCloseDisplay
#define SetClipboardEventCallback   REND_PROC_ALIAS( SetClipboardEventCallback )
#define SetDisplayFullScreen    REND_PROC_ALIAS_VOID( SetDisplayFullScreen )
#define SuspendSystemSleep      REND_PROC_ALIAS_VOID( SuspendSystemSleep )
#define RenderIsInstanced()       ((USE_RENDER_INTERFACE)?((USE_RENDER_INTERFACE)->_RenderIsInstanced)?(USE_RENDER_INTERFACE)->_RenderIsInstanced():0:0)
#define SetDisplayCursor(n)           {if((USE_RENDER_INTERFACE)&&(USE_RENDER_INTERFACE)->_SetDisplayCursor)REND_PROC_ALIAS(SetDisplayCursor)(n);}
#define IsDisplayRedrawForced(r)    ((USE_RENDER_INTERFACE)?((USE_RENDER_INTERFACE)->_IsDisplayRedrawForced)?(USE_RENDER_INTERFACE)->_IsDisplayRedrawForced(r):0:0)
#endif
	_INTERFACE_NAMESPACE_END
#ifdef __cplusplus
#ifdef _D3D_DRIVER
	using namespace sack::image::render::d3d::Interface;
#elif defined( _D3D10_DRIVER )
	using namespace sack::image::render::d3d10::Interface;
#elif defined( _D3D11_DRIVER )
	using namespace sack::image::render::d3d11::Interface;
#else
	using namespace sack::image::render::Interface;
#endif
#endif
#endif
#ifndef __NO_MSGSVR__
#ifdef DEFINE_RENDER_PROTOCOL
  // offsetof
// need to define BASE_RENDER_MESSAGE_ID before including this.
//#define MSG_ID(method)  ( ( offsetof( struct render_interface_tag, _##method ) / sizeof( void(*)(void) ) ) + BASE_RENDER_MESSAGE_ID + MSG_EventUser )
#define MSG_DisplayClientClose        MSG_ID(DisplayClientClose)
#define MSG_SetApplicationTitle       MSG_ID(SetApplicationTitle)
#define MSG_SetRendererTitle       MSG_ID(SetRendererTitle)
#define MSG_SetApplicationIcon        MSG_ID(SetApplicationTitle)
#define MSG_GetDisplaySize            MSG_ID(GetDisplaySize)
#define MSG_SetDisplaySize            MSG_ID(SetDisplaySize)
#define MSG_GetDisplayPosition        MSG_ID(GetDisplayPosition)
#define MSG_OpenDisplaySizedAt        MSG_ID(OpenDisplaySizedAt)
#define MSG_OpenDisplayAboveSizedAt   MSG_ID(OpenDisplayAboveSizedAt)
#define MSG_CloseDisplay              MSG_ID(CloseDisplay)
#define MSG_UpdateDisplayPortionEx    MSG_ID(UpdateDisplayPortionEx)
#define MSG_UpdateDisplay             MSG_ID(UpdateDisplayEx)
#define MSG_SetMousePosition          MSG_ID(SetMousePosition)
#define MSG_GetMousePosition          MSG_ID(GetMousePosition)
#define MSG_GetMouseState             MSG_ID(GetMouseState )
#define MSG_Redraw               MSG_ID(Redraw)
#define MSG_EnableSpriteMethod             MSG_ID(EnableSpriteMethod )
#define MSG_WinShell_AcceptDroppedFiles    MSG_ID(WinShell_AcceptDroppedFiles )
#define MSG_MoveDisplay               MSG_ID(MoveDisplay)
#define MSG_MoveDisplayRel            MSG_ID(MoveDisplayRel)
#define MSG_SizeDisplay               MSG_ID(SizeDisplay)
#define MSG_SizeDisplayRel            MSG_ID(SizeDisplayRel)
#define MSG_MoveSizeDisplay           MSG_ID(MoveSizeDisplay)
#define MSG_MoveSizeDisplayRel        MSG_ID(MoveSizeDisplayRel)
#define MSG_PutDisplayAbove           MSG_ID(PutDisplayAbove)
#define MSG_GetDisplayImage           MSG_ID(GetDisplayImage)
#define MSG_SetCloseHandler           MSG_ID(SetCloseHandler)
#define MSG_SetMouseHandler           MSG_ID(SetMouseHandler)
#define MSG_SetRedrawHandler          MSG_ID(SetRedrawHandler)
#define MSG_SetKeyboardHandler        MSG_ID(SetKeyboardHandler)
#define MSG_SetLoseFocusHandler       MSG_ID(SetLoseFocusHandler)
#define MSG_SetDefaultHandler         MSG_ID(SetDefaultHandler)
// -- all other handlers - client side only
#define MSG_HasFocus                  MSG_ID(HasFocus)
#define MSG_SendActiveMessage         MSG_ID(SendActiveMessage)
#define MSG_GetKeyText                MSG_ID(GetKeyText)
#define MSG_IsKeyDown                 MSG_ID(IsKeyDown)
#define MSG_KeyDown                   MSG_ID(KeyDown)
#define MSG_DisplayIsValid            MSG_ID(DisplayIsValid)
#define MSG_OwnMouseEx                 MSG_ID(OwnMouseEx)
#define MSG_BeginCalibration           MSG_ID(BeginCalibration)
#define MSG_SyncRender                 MSG_ID(SyncRender)
#define MSG_OkaySyncRender                 MSG_ID(OkaySyncRender)
#define MSG_HideDisplay               MSG_ID(HideDisplay)
#define MSG_IsDisplayHidden               MSG_ID(IsDisplayHidden)
#define MSG_RestoreDisplay             MSG_ID(RestoreDisplay)
#define MSG_MakeTopmost               MSG_ID(MakeTopmost)
#define MSG_BindEventToKey          MSG_ID(BindEventToKey)
#define MSG_UnbindKey               MSG_ID(UnbindKey)
#define MSG_IsTouchDisplay          MSG_ID(IsTouchDisplay )
#define MSG_GetNativeHandle             MSG_ID(GetNativeHandle)
#endif
#endif
// static void OnDisplayChangedSize( "" )( PRENDERER, int nDisplay, uint32_t x, uint32_t y, uint32_t width, uint32_t height )
	// OnDisplayPause is called on systems that allow the application to suspend its display.
	// Sleep mode may also trigger such an event, allows application to save state
   // a media player, for instance, may recover unplayed buffers to prepare for resume
#define OnDisplaySizeChange(name)	 DefineRegistryMethod("sack/render",OnDisplaySizeChange,"display",name,"on_display_size_change",void,( uintptr_t psv_redraw, int nDisplay, int32_t x, int32_t y, uint32_t width, uint32_t height ),__LINE__)
// static void OnDisplayPause( "" )( void )
	// OnDisplayPause is called on systems that allow the application to suspend its display.
	// Sleep mode may also trigger such an event, allows application to save state
   // a media player, for instance, may recover unplayed buffers to prepare for resume
#define OnDisplayPause(name)	 DefineRegistryMethod("sack/render/android",OnDisplayPause,"display",name,"on_display_pause",void,(void),__LINE__)
// static void OnDisplayResume( "" )( void )
	// OnDisplayResume is called on systems that allow the application to suspend its display.
	// Wake from sleep mode may also trigger such an event, allows application to restore saved state
   // a media player, for instance, may continue playing ( it might be good to wait just a little longer than 'now')
#define OnDisplayResume(name)	 DefineRegistryMethod("sack/render/android",OnDisplayResume,"display",name,"on_display_resume",void,(void),__LINE__)
	struct display_app;
	struct display_app_local;
	// static void OnDisplayConnect( "" )( struct display_app*app, struct display_app_local ***pppLocal )
	//  app is a unique handle to the display instance.  Can be used as a key to locate resources for the display
	//  pppLocal is ... ugly.
	//  ThreadLocal struct instance_local *_thread_local;
	//  static void OnDisplayConnect( "" )( struct display_app*app, struct display_app_local ***pppLocal )
	//  {
	//	    _thread_local = New( struct instance_local );
	//      MemSet( option_thread, 0, sizeof( option_thread ) );
	//      (*local) = (struct display_app_local**)&option_thread;
	//       //... init local here
	//  }
	//
#define OnDisplayConnect(name)	 DefineRegistryMethod("/sack/render/remote display",OnDisplayConnect,"connect",name,"new_display_connect",void,(struct display_app*app, struct display_app_local ***),__LINE__)
	// unimplemented.
#define OnDisplayConnected(name)	 DefineRegistryMethod("/sack/render/remote display",OnDisplayConnect,"connect",name,"new_display_connected",void,(struct display_app*app),__LINE__)
RENDER_NAMESPACE_END
#ifdef __cplusplus
#ifdef _D3D_DRIVER
	using namespace sack::image::render::d3d;
#elif defined( _D3D10_DRIVER )
	using namespace sack::image::render::d3d10;
#elif defined( _D3D11_DRIVER )
	using namespace sack::image::render::d3d11;
#else
	using namespace sack::image::render;
#endif
#endif
#endif
// : $
// $Log: render.h,v $
// Revision 1.48  2005/05/25 16:50:09  d3x0r
// Synch with working repository.
//
// Revision 1.10  2003/03/25 08:38:11  panther
// Add logging
//
#ifndef INCLUDED__RENDER3D_EXTENSIONS_DEFINED__
#define INCLUDED__RENDER3D_EXTENSIONS_DEFINED__
typedef struct render_3d_interface_tag
{
	RENDER_PROC_PTR( PTRANSFORM, GetRenderTransform)         ( PRENDERER );
	// returns TRUE if at least one of the points is inside the camera clip
	RENDER_PROC_PTR( LOGICAL, ClipPoints)         ( P_POINT points, int nPoints );
 // array of 6 planes
	RENDER_PROC_PTR( void, GetViewVolume )( PRAY *planes );
 // 0=world;1=local;2=view
	RENDER_PROC_PTR( void, SetRendererAnchorSpace )( PRENDERER display, int anchor );
#ifdef _D3D_DRIVER
	IDirect3DDevice9 *current_device;
#endif
#ifdef _D3D10_DRIVER
	ID3D10Device *current_device;
	ID3D10RenderTargetView *current_target;
	IDXGISwapChain *current_chain;
#endif
#ifdef _D3D11_DRIVER
	ID3D11Device *current_device;
	ID3D11DeviceContext *current_device_context;
	ID3D11RenderTargetView *current_target;
	IDXGISwapChain *current_chain;
#endif
#ifdef _VULKAN_DRIVER
	RENDER_PROC_PTR( struct cmdBuf *, getCommandBuffer )( void );
	RENDER_PROC_PTR( struct cmdBuf *, getGeometryBuffer )( void );
	RENDER_PROC_PTR( struct cmdBuf *, getVertexBuffer )( void );
#endif
} RENDER3D_INTERFACE, *PRENDER3D_INTERFACE;
#ifdef _VULKAN_DRIVER
RENDER_PROC( void, createCommandBuffers )( struct SwapChain* chain, VkCommandBuffer* buffers, uint32_t count, LOGICAL primary );
#endif
#if defined( _VULKAN_DRIVER )
#  define EXTRA_INIT_PARAM struct SwapChain* chain,
#  define EXTRA_INIT_ARG_TYPE struct SwapChain*,
#else
#  define EXTRA_INIT_PARAM
#  define EXTRA_INIT_ARG_TYPE
#endif
#if defined( _D3D_DRIVER ) || defined( _D3D10_DRIVER ) || defined( _D3D11_DRIVER )
#define g_d3d_device  (USE_RENDER3D_INTERFACE)->current_device
#define g_d3d_device_context  (USE_RENDER3D_INTERFACE)->current_device_context
#endif
// static uintptr_t OnInit3d( "Virtuality" )( PMatrix projection, PTRANSFORM camera, RCOORD *identity_depth, RCOORD *aspect )
// each Init is called once for each display that is opened; the application recevies the reference
// of the transform used for the camera.  This may be used to clip objects that are out of scene.
// If you return 0 as the uintptr_t result, you will not get any further events.
//
// - (passes identity_depth as a reference to identity depty, because if the screen size changes, the identity depth also changes)
// identity_depth this is relative to how the display was opened.  identity is where there is a 1:1 relation
// between rendered image pixels rendered and physical display pixels.
// This may be arbitrarily overridden to be closer than normal in case the physical display dpi is too dense.
// identity depth should be used for rendering icons on objects.
#define OnInit3d(name)	 DefineRegistryMethod("sack/render/puregl",Init3d,"init3d",name,"ExtraInit3d",uintptr_t,( EXTRA_INIT_PARAM PMatrix projection, PTRANSFORM camera, RCOORD *identity_depth, RCOORD *aspect ),__LINE__)
// static void OnClose3d( "Virtuality" )(uintptr_t psvInit)
// handle event when the specified display context is closed.  The value passed is the result from Init3d();
// All resources relavent to the 3d engine should be released.  (shaders)  and statuses cleared so reinitialization can occur.
#define OnClose3d(name)	 DefineRegistryMethod("sack/render/puregl",Close3d,"draw3d",name,"ExtraClose3d",void,(uintptr_t psvInit),__LINE__)
// static void OnResume3d( "Virtuality" )( void) { }
// On Resume is invoked when a significant time has passed and simulations should consider working from 'NOW'
// instead of the prior tick.  So basically the next 'update' should be a delta of 0; or that the delta should be from
// this point.  So maybe the draw is called first before the update in the case of resume.
// Android targets can invoke a pause state, from which resume will be required for smooth animations
// (excessive time steps will cause overflow conditions)
#define OnResume3d(name)	 DefineRegistryMethod("sack/render/puregl",Resume3d,"draw3d",name,"Resume3d",void,(void),__LINE__)
// static LOGICAL OnUpdate3d( "Virtuality" )( uintptr_t psvInit, PTRANSFORM eye_transform );
// called when a new frame will be rendered.  Once per frame.  All others are called per-camera per-frame.
// can update the common viewpoint here, and it will propagate, otherwise the camera ends up resetting to
// this point, unless the named transformation matrix is loaded manually.
// it is passed the origin transformation view into the universe)
// since this is potentially one for multiple Init instances, cannot pass the instance this applies to, since it is all.
// the PTRANSFORM origin has just previously been updated with Move(), so it can have speed and acceleration applied
// return true/false to indicate a desire to draw this frame.  If this and nothing else changed, the frame will be skipped.
#define OnUpdate3d(name)	 DefineRegistryMethod("sack/render/puregl",Update3d,"draw3d",name,"Update3d",LOGICAL,(PTRANSFORM origin ),__LINE__)
// static void OnFirstDraw3d( "Virtuality" )( uintptr_t psvInit );
// called the first time a camera draws itself;
// allows user code to load geometry into the camera...
// it is passed the instance handle returned from Init3d
#define OnFirstDraw3d(name)	 DefineRegistryMethod("sack/render/puregl",FirstDraw3d,"draw3d",name,"FirstDraw3d",void,(uintptr_t psvInit ),__LINE__)
// static void OnBeginDraw3d( "Virtuality" )( uintptr_t psvInit, PTRANSFORM camera )
// this is called once for each display that is opened, and for each OnInit3d that did not return 0.
// the psvInit is the init value returned, the mouse is the mouse as it is; it will be NULL if the mouse
// is not in the current display and we are just drawing.
// opportunity to override the camera position.
#define OnBeginDraw3d(name)	 DefineRegistryMethod("sack/render/puregl",BeginDraw3d,"draw3d",name,"ExtraBeginDraw3d",void,(uintptr_t psvUser,PTRANSFORM camera),__LINE__)
// static void OnDraw3d( "Virtuality" )( uintptr_t psvInit )
// this is called once for each display that is opened, and for each OnInit3d that did not return 0.
// the psvInit is the init value returned.
#define OnDraw3d(name)	 DefineRegistryMethod("sack/render/puregl",Draw3d,"draw3d",name,"ExtraDraw3d",void,(uintptr_t psvUser),__LINE__)
// static LOGICAL OnMouse3d( "Virtuality" )( uintptr_t psvInit, PRAY mouse, int32_t x, int32_t y, uint32_t b )
// this is a real mouse event that is in a display that you returned non 0 during Init3d.
// PRAY is the line represenging the point that the user has the mouse over at the moemnt.
// mouse buttons are passed. for mouse state (may also be key state)
// return FALSE if you did not use the mouse.
// return TRUE if you did, and therefore the event is used and noonne else should make two things happen...
#define OnMouse3d(name)	 DefineRegistryMethod("sack/render/puregl",Mouse3d,"draw3d",name,"ExtraMouse3d",LOGICAL,(uintptr_t psvUser, PRAY mouse_ray, int32_t x, int32_t y, uint32_t b),__LINE__)
// static LOGICAL OnKey3d( "Virtuality" )( uintptr_t psvInit, uint32_t key )
// this is a real key event that is in a display that you returned non 0 during Init3d.
// return FALSE if you did not use the key.
// return TRUE if you did use the key, and therefore the event is used and noonne else should make two things happen...
#define OnKey3d(name)	 DefineRegistryMethod("sack/render/puregl",Key3d,"draw3d",name,"ExtraKey3d",LOGICAL,(uintptr_t psvUser, uint32_t key),__LINE__)
#if !defined( FORCE_NO_INTERFACE ) && !defined( FORCE_NO_RENDER_INTERFACE )
#define REND3D_PROC_ALIAS(name) ((USE_RENDER_3D_INTERFACE)->_##name)
#  define GetRender3dInterface() (PRENDER3D_INTERFACE)GetInterface( "render.3d" )
#  define GetRenderTransform             REND3D_PROC_ALIAS(GetRenderTransform)
#  define ClipPoints             REND3D_PROC_ALIAS(ClipPoints)
#  define GetViewVolume             REND3D_PROC_ALIAS(GetViewVolume)
#endif
 // __RENDER3D_EXTENSIONS_DEFINED__
#endif
