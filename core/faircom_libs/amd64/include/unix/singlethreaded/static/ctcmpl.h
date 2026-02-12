/* LINUX Intel compiler setup (POSIX thread enabled) */

#ifndef ctCTCMPL
#define ctCTCMPL

#define  ctPortLINUX_REDHAT_5_2

#ifdef 	FD_SETSIZE   	/* Possibly set to larger value in ctnlib.c 	*/
#undef  FD_SETSIZE	/* Linux headers define without a ifndef 	*/
#endif

#define ctrt_monitor	ctrt_printf
#define ctrt_fncmon	ctrt_printf
#define ctrt_msgmon	ctrt_printf

#define ctUSE_mkstemp
#define ctrt_tmpnam 	mkstemp

/*^***********************************/
#ifndef ctPortFAIRCOM_THREADS
#ifndef ctPortNATIVE_THREADS
#include <stdio.h>
#include <fcntl.h>
#include <setjmp.h>
#include <sys/resource.h>
#include <memory.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdarg.h>
#endif  /* ~ctPortNATIVE_THREADS */
#endif  /* ~ctPortFAIRCOM_THREADS */
/*~***********************************/

/*^***********************************/
#ifdef ctPortFAIRCOM_THREADS

#ifdef ctSRVR
#ifndef SERVER_OSMSG
#define SERVER_OSMSG 		"For Linux(Intel)"
#endif
#endif

#include <stdio.h>
#include <fcntl.h>
#include <setjmp.h>
#include <sys/resource.h>
#include <memory.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdarg.h>
#endif /* ctPortFAIRCOM_THREADS */
/*~***********************************/


/*^***********************************/
#ifdef ctPortNATIVE_THREADS
#if (defined(ctSRVR) || defined(ctBNDSRVR))

#ifndef SERVER_OSMSG
#define SERVER_OSMSG "For Linux(Intel) - native POSIX multi threading enabled"
#endif

#define	__PTHREAD__		/* POSIX thread */
#define ctSTATIC_SEMA		/* Needed for pthread code */
#define	ctSEMTIM		/* enable pthread_cond_timedwait() */
#ifndef	_REENTRANT
#define	_REENTRANT		/* pthread switch */
#endif

#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <fcntl.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

#endif /* ctSRVR or ctBNDSRVR */
#endif /* ctPortNATIVE_THREADS */
/*~***********************************/


/*^***********************************/
#ifdef ctPortNATIVE_THREADS
#if (defined(ctMTCLIENT) || defined(ctMTFPG))

#define	__PTHREAD__		/* POSIX thread */
#ifndef	_REENTRANT
#define	_REENTRANT		/* pthread switch */
#endif

#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <fcntl.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

#endif /* ctMTCLIENT */
#endif /* ctPortNATIVE_THREADS */
/*~***********************************/

#ifndef ctPortLINUX
#define ctPortLINUX
#endif
#define C255			-1
#define LOW_HIGH

#ifdef ctPortNATIVE_THREADS
#define ctDEFER_SLEEP           select
#define ctHAS_SELECT
#else
#define ctDEFER_SLEEP 		usleep
#define ctDEFER_USLEEP_OVERRIDE
#endif /* ctPortNATIVE_THREADS */

#define ctFCNTL
/* #define ctMEMCHRtxt  	pVOID */
#define ctSAVEunxFS
/* #define ctSIZET		unsigned int  */

#define ctrt_stricmp 		strcasecmp
#define ctrt_strnicmp 		strncasecmp

#ifdef ctSRVR
#ifdef ctPortNATIVE_THREADS
#define ctHAS_ALTERNATE_ctrt_exit
#endif
#define ctNO_OVERRIDE_FD_SETSIZE /* TCP/IP protocol: Do not adjust systems default for FD_SETSIZE */
#define ctSELECTthrd
#endif	/* ctSRVR */

#ifdef ctBNDSRVR
#ifdef ctPortNATIVE_THREADS
#define ctHAS_ALTERNATE_ctrt_exit
#endif
#endif	/* ctBNDSRVR */

#ifndef ctLINUX_LARGE_FILES
#define ctNO_64BIT_FILE
#endif

#define SYS_LONG8
typedef long long LONG8;
#define ctLLd           "%lld"
#define ctLLu           "%llu"
#define ctLLx           "%llx"
#define ctLL03d         "%03lld"
#define ctLL04d         "%04lld"
#define ctLL05d         "%05lld"
#define ctLL06d         "%06lld"
#define ctLL12d		"%12lld"

#ifdef ctPortLINUX_64
#define ctMEMALIGN8
#define ct8P
#define SYS_LONG
typedef int LONG;
#define SYS_ULONG
typedef unsigned int ULONG;
#define ctSTRICTANSI
#define ctNoUnalignedLongs
#endif
#define ctNO_WCHAR

#include "ctcmpl_u.h" 	/* Unix Specific defines */
#include "ctsysi.h" 	/* FairCom Server ctPortid header file */

#endif /* ~ctCTCMPL */
/* end of ctcmpl.h */

