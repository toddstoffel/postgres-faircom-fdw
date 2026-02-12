/*
 *      OFFICIAL NOTIFICATION: the following CONFIDENTIAL and PROPRIETARY
 * 	property legend shall not be removed from this source code module
 * 	for any reason.
 *
 *	This program is the CONFIDENTIAL and PROPRIETARY property
 *	of FairCom(R) Corporation. Any unauthorized use, reproduction or
 *	transfer of this computer program is strictly prohibited. The
 *      contents of this file may not be disclosed to third parties, copied or
 *      duplicated in any form, in whole or in part, without the prior written
 *      permission of the FairCom(R) Corporation.
 *
 *      Copyright (c) 1992 - 2024 FairCom Corporation.
 *	This is an unpublished work, and is subject to limited distribution and
 *	restricted disclosure only. ALL RIGHTS RESERVED.
 *
 *			RESTRICTED RIGHTS LEGEND
 *	Use, duplication, or disclosure by the Government is subject to
 *	restrictions set forth in subparagraph (c)(1)(ii) of the Rights in
 * 	Technical Data and Computer Software clause at DFARS 252.227-7013, and/or
 *      in similar or successor clauses in the FAR, DOD or NASA FAR Supplement.
 *      Unpublished rights reserved under the Copyright Laws of the United States.
 *	FairCom Corporation, 6300 West Sugar Creek Drive, Columbia, MO 65203.
 *
 *	c-treeACE(tm)	Version 13
 */

#ifdef _WIN32_WCE
#include "ctnlibwce.h"
#define ctNETWORKLIB
#endif

#ifndef ctNETWORKLIB
#define ctNETWORKLIB

#ifdef CTNLIBV6
#if defined(ctCLIENT) || defined(ctLOCLIB)
#define at_SessionSetAttribute	IPV6at_SessionSetAttribute
#define at_SessionValidate	IPV6at_SessionValidate
#define at_SessionNew		IPV6at_SessionNew
#define	at_SessionDispose	IPV6at_SessionDispose
#define at_SessionOpen		IPV6at_SessionOpen
#define at_SessionClose		IPV6at_SessionClose
#define at_SessionSend		IPV6at_SessionSend
#define at_SessionReceive	IPV6at_SessionReceive
#define at_SessionMake		IPV6at_SessionMake
#define at_SessionConnect	IPV6at_SessionConnect
#define at_SessionDisconnect	IPV6at_SessionDisconnect
#define at_CommunicationPolice	IPV6at_CommunicationPolice
#define at_CommunicationInit	IPV6at_CommunicationInit
#define at_CommunicationStart	IPV6at_CommunicationStart
#define at_CommunicationStop	IPV6at_CommunicationStop
#define GetServerInfoXtd	IPV6GetServerInfoXtd
#define iGetServerInfoXtd	IPV6iGetServerInfoXtd
#define SetNonBlocking		IPV6SetNonBlocking
#define SetOtherSockOpts	IPV6SetOtherSockOpts
#endif
#endif /* CTNLIBV6 */

/*
 * Set size of descriptor array for select().
 * This should be as large as maximum number of file descriptors available
 * to a process.
 */
#ifdef ctSRVR
#ifndef ctNO_OVERRIDE_FD_SETSIZE /* TCP/IP protocol: Do not adjust systems default for FD_SETSIZE */
#ifndef FD_SETSIZE
#define FD_SETSIZE 4096
#endif
#endif
#endif

#ifdef ctCLIENT
#ifdef ctFeatNEW_DEAD_CLIENT
/* Dead client detection on client side. */
#define ctFeatCLIENT_DEAD_CLIENT
#endif
#endif

/* system dependent headers */
#include <stdio.h>
#include <stdlib.h>
#ifndef  ctNO_STDDEFH
#include <stddef.h>
#endif /* ctNO_STDDEFH */
#include <string.h>
#include <errno.h>

#ifdef OS2_INCLUDED
#include <types.h>
#define  ctIOCTL_H	<sys\ioctl.h>
#include ctIOCTL_H
#define	 ctNETDB_H	<netdb.h>
#include ctNETDB_H
#define  ctIN_H		<netinet\in.h>
#include ctIN_H
#ifndef ctPortOS2
#define  ctTCP_H	<netinet\tcp.h>
#include ctTCP_H
#endif
#define  ctSOCKET_H	<sys\socket.h>
#include ctSOCKET_H
#include <process.h>

#define __BSD43_SOCKETS__
#define sockread(a,b,c)	recv(a,b,c,0)
#define sockwrite(a,b,c)	send(a,b,c,0)

#ifndef ctPortOS2_2x
#define ctPortOS2_2x

#ifdef TCPDLLOAD
HMODULE	hTcpipDLL,
			hSocketsDLL;

NINT _System (*gethostname)( pTEXT , NINT );
struct hostent * _System (*gethostbyname)( pTEXT );
NINT _System (*accept)( NINT, struct sockaddr *, pNINT );
NINT _System (*bind)( NINT, struct sockaddr *, NINT );
NINT _System (*connect)( NINT, struct sockaddr *, NINT );
NINT _System (*getsockname)( NINT, struct sockaddr *, pNINT );
NINT _System (*ioctl)(NINT, NINT, pTEXT, NINT);
NINT _System (*listen)( NINT, NINT);
NINT _System (*recv)( NINT, pTEXT, NINT, NINT );
NINT _System (*send)( NINT, pTEXT, NINT, NINT );
NINT _System (*setsockopt)( NINT, NINT, NINT, pTEXT, NINT );
NINT _System (*sock_init)( VOID );
NINT _System (*sock_errno)( VOID );
NINT _System (*socket)( NINT, NINT, NINT );
NINT _System (*soclose)( NINT );
NINT _System (*shutdown)(NINT, NINT);
#endif

#endif

#endif

#ifndef ctPortMSDOS

#ifndef ctPortWINDOWSNT

#ifndef ctPortOS2_2x

#define ctPortUNIXCOMPATIBLE

#endif /* ctPortOS2_2x */

#endif /* ctPortWINDOWSNT */

#endif /* ctPortMSDOS */

#ifdef ctPortUNIXCOMPATIBLE


#define __BSD43_SOCKETS__

#ifdef fd
#define ctFD	fd
#undef fd
#endif /* fd */

#ifdef ctPortLYNXOS386
/*
The following definitions are ifdef'd out of the system headers because
of an ifdef _POSIX_SOURCE. This POSIX is on because of the -X switch on
compile with is required for the -m multithreaded switch. Therefore we
added the defines here.
*/

#ifdef _POSIX_SOURCE /* from -X switch */
typedef unsigned short u_short;
typedef unsigned long  u_long;
typedef unsigned char  u_char;
#endif /* _POSIX_SOURCE */

#ifndef EBADF
#define EBADF		9       /* Bad file descriptor */
#endif
#ifndef EAGAIN
#define EAGAIN		11      /* Resource temporarily unavailable */
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK     EAGAIN  /* Operation would block */
#endif
#ifndef EFAULT
#define EFAULT		14      /* Bad address */
#endif
#ifndef EINPROGRESS
#define EINPROGRESS     36      /* Operation in progress (POSIX.4) */
#endif
#ifndef EALREADY
#define EALREADY        37      /* Operation already in progress */
#endif
#ifndef ENOTSOCK
#define ENOTSOCK        38      /* Socket operation on non-socket */
#endif
#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT    47      /* Address family not supported by protocol family */
#endif
#ifndef EADDRNOTAVAIL
#define EADDRNOTAVAIL   49      /* Can't assign requested address */
#endif
#ifndef EADDRINUSE
#define EADDRINUSE      48      /* Address already in use */
#endif
#ifndef ENETUNREACH
#define ENETUNREACH     51      /* Network is unreachable */
#endif
#ifndef ENOBUFS
#define ENOBUFS         55      /* No buffer space available */
#endif
#ifndef EISCONN
#define EISCONN         56      /* Socket is already connected */
#endif
#ifndef ETIMEDOUT
#define ETIMEDOUT       60      /* Connection timed out */
#endif
#ifndef ECONNREFUSED
#define ECONNREFUSED    61      /* Connection refused */
#endif
#endif /* ctPortLYNXOS386 */


#include <signal.h>
#define ctNETDB_H	<netdb.h>
#include ctNETDB_H
#include <sys/types.h>
#include <sys/socket.h>
#define  ctIN_H	<netinet/in.h>
#include ctIN_H
#define  ctTCP_H <netinet/tcp.h>
#include ctTCP_H

#ifdef ctPortUNIX
#include <arpa/inet.h>
#endif

#ifdef ctPortLYNXOS386
#include	<sys/errno.h>
#else
#ifndef EWOULDBLOCK
#define ctERRNO_H	<net/errno.h>
#include ctERRNO_H
#endif /* EWOULDBLOCK */
#endif /* ~ctPortLYNXOS386 */

#ifdef ctFCNTL
#include <fcntl.h>

#ifdef ctPortQNX
#define ctUNIX_H	<unix.h>
#include ctUNIX_H
#endif

#else /* ctFCNTL */
#define ctIOCTL_H	<sys/ioctl.h>
#include ctIOCTL_H
#endif /* ctFCNTL */

#ifdef ctFD
#define fd ctFD
#undef ctFD
#endif /* ctFD */

#define sockread	read
#define sockwrite	write
#define soclose(a)	close(a)

#ifndef ctPortLINUX
#ifndef ctPortRS6_SYSV_REL_3
#ifndef ctPortLYNXOS386
#ifndef ctPort88OPEN
#ifndef ctPortSCO386
#ifndef ctPortINTERACTIVE
/* New dead client detection requires non-vectored socket I/O on */
/* client side due to check for message marker bytes on read. */
#ifndef ctFeatCLIENT_DEAD_CLIENT

#define ctUIO_H <sys/uio.h>
#include ctUIO_H

#ifndef ctFeatSSL
#define sockwritev	writev
#define sockreadv	readv
#endif

#endif /* ~ctFeatCLIENT_DEAD_CLIENT */
#endif
#endif
#endif
#endif
#endif
#endif

#endif /* ctPortUNIXCOMPATIBLE */

#ifdef ctPortMSDOS

#include <dos.h>

#ifndef _MSC_VER

#define setvect	_dos_setvect
#define getvect	_dos_getvect

#endif /* ~_MSC_VER */

#define TIMER_INTERRUPT	8

#ifdef __WINSOCKETS__

#ifdef __BWTCP__

#undef __BWTCP__

#endif /* __BWTCP */

#ifndef MAKEWORD

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))

#endif /* MAKEWORD */

#endif /* __WINSOCKETS__ */

#ifdef __BWTCP__

#define ctSOCKET_H	"socket.h"
#include ctSOCKET_H

#define sockread	soread
#define sockwrite	sowrite

#define shutdown(a, b)
#define bind(a, b, c)		0
#define listen(a, b)		0

/* uncomment if using a socket.c version < 0.96
#define gethostbyname	gethostbynamef
*/

#endif /* __BWTCP__ */

#endif /* __MSDOS __ */



#ifdef ctPortWINDOWSNT
#define __WINSOCKETS__
#endif /* ctPortWINDOWSNT */

#ifdef __WINSOCKETS__

#define getsockerror()	 WSAGetLastError()
#define setsockerror(v)	 WSASetLastError(v)

#define __BSD43_SOCKETS__
#define sockread(a, b, c)	recv((SOCKET)(a), (char FAR *)(b), c, 0)
#define sockwrite(a, b, c)	send((SOCKET)(a), (char FAR *)(b), c, 0)
#define soclose(a)		closesocket(a)

#ifdef ctFeatIPv6
#include <winsock2.h>
#include <ws2tcpip.h>
#elif (defined(ctSQLSRVR) || defined(ctUSE_WINSOCK2_H)) && _MSC_VER > 1200
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include <windows.h>
#include <sys\types.h>
#ifndef ctFeatIPv6
#include <winsock.h>
#endif

#if _MSC_VER <= 1200
#ifndef __BORLANDC__
#if (defined(ctCLIENT) || defined(ctLOCLIB))
#include <ws2tcpip.h>
#endif
#endif
#endif /* _MSC_VER */

#undef  ETIMEDOUT
#define ETIMEDOUT	WSAETIMEDOUT
#undef  ECONNREFUSED
#define ECONNREFUSED	WSAECONNREFUSED
#undef  ENOTSOCK
#define ENOTSOCK	WSAENOTSOCK
#undef  EADDRNOTAVAIL
#define EADDRNOTAVAIL	WSAEADDRNOTAVAIL
#undef  EAFNOSUPPORT
#define EAFNOSUPPORT	WSAEAFNOSUPPORT
#undef  EISCONN
#define EISCONN		WSAEISCONN
#undef  ENETUNREACH
#define ENETUNREACH	WSAENETUNREACH
#undef  EADDRINUSE
#define EADDRINUSE	WSAEADDRINUSE
#undef  EWOULDBLOCK
#define EWOULDBLOCK	WSAEWOULDBLOCK
#undef  ENOBUFS
#define ENOBUFS		WSAENOBUFS

#else /* __WINSOCKETS__ */

#ifdef ctPortOS2_2x

#define getsockerror()		sock_errno()
#define setsockerror(v)

#else /* ctPortOS2_2x */

#define getsockerror()	errno
#define setsockerror(v)	errno = v

#endif /* ctPortOS2_2x */

#define INVALID_SOCKET	-1
typedef NINT SOCKET;

#endif /* __WINSOCKETS__ */

#ifdef ctPortWINDOWSMSDOS

#include <windows.h>

#endif /* ctPortWINDOWSMSDOS */

#ifndef FAR
#define FAR
#endif /* FAR */

#ifdef ctPortOS2_2x
#define ctKERNAL_THREADS
#endif

#ifdef ctPortWINDOWSNT
#define ctKERNAL_THREADS
#endif

#ifdef ctPortALPHA
#define ctKERNAL_THREADS
#endif

#ifdef ctPortLYNXOS386
#define ctKERNAL_THREADS
#endif

#define ReservedPortOffset 1024
#define UserPortOffset     (5001 - ReservedPortOffset)

#define MAXCMDSZ				4096

#ifdef sockreadv

/*
 * Operating system supports vectored socket I/O.
 * Use the O/S-defined iovec structure.
 */
typedef struct iovec IO_VEC, *pIO_VEC;

#else  /* ~sockreadv */

/*
 * Operating system does not support vectored socket I/O.
 * Use a custom iovec structure.
 */
typedef struct {
	pTEXT	iov_base;
	LONG	iov_len;
} IO_VEC, ctMEM *pIO_VEC;

#endif /* ~sockreadv */


#ifdef PROTOTYPE

static NINT ParseOutServerName(pTEXT, pTEXT, NINT);
LOCAL NINT ParseOutHostName(pTEXT, pTEXT, NINT);
LOCAL NINT DerivePortNumber(pSrvrGbls pGlobals, pTEXT pServerName);
VOID clrallsem(pSrvrGbls pGlobals);
static NINT VectorToBuffer(pMTEXT, NINT, pIO_VEC, NINT);
static NINT BufferToVector(pMTEXT, NINT, pIO_VEC, NINT);
LOCAL NINT ctSocket_select(NLONG sfd, NINT optype, NINT timeout);

#ifndef sockwritev
#ifndef ctFeatSSL
LONG    sockwritev(NINT, pIO_VEC, NINT);
#endif
#endif

#ifndef sockreadv
#ifndef ctFeatSSL
LONG    sockreadv(NINT, NINT, pIO_VEC, NINT);
#endif
#endif

NINT SetNonBlocking(SOCKET, NINT);
NINT SetOtherSockOpts(SOCKET);

#endif

#ifdef sockreadv
#define DCLI_FLAG
#else
#define DCLI_FLAG	dcli,
#endif

#ifdef ctPort16BIT
#define ctTCP_BUFF_SIZE	2048
#else
#define ctTCP_BUFF_SIZE	4096
#endif

typedef struct {
	NINT	shutDown;
	NLONG	connectSocket;
}SERVER_DATA, ctMEM *pSERVER_DATA;

#ifdef MULTITRD
#ifndef ctKERNAL_THREADS
#ifdef ctSELECTthrd
#include <sys/types.h>
#define ctTIME_H	<sys/time.h>
#include ctTIME_H

#ifndef ctPortSUN41
#ifndef ctPortHP9000
#ifndef ctPortAPPLEAUX
#ifndef ctPort386BSD
#ifndef ctPortDGU386
#ifndef ctPortNETBSD386
#define ctSELECT_H	<sys/select.h>
#include ctSELECT_H
#endif
#endif
#endif
#endif
#endif
#endif

#ifndef ctBADSLCTlimit
#define ctBADSLCTlimit	64
#endif
#endif

extern NINT rqst_delay,rqst_delta,tasker_loop;
#endif
#endif

#ifdef ctFeatNEW_DEAD_CLIENT

#if defined(ctPortLINUX_PREV2_036) || defined(ctPortHP9000) || defined(ctPortSGI) || defined(ctPortSCO386)
/* Need sys/time.h for struct timeval definition. */
#ifndef ctTIME_H
#define ctTIME_H	<sys/time.h>
#include ctTIME_H
#endif
#endif /* ctPortLINUX_PREV2_036 || ctPortHP9000 */

#ifdef ctPortLINUX_PREV2_036
#ifndef FD_SETSIZE
#define FD_SETSIZE 4096
#endif
#endif /* ctPortLINUX_PREV2_036 */

#ifdef ctPortQNX
/* Need sys/select.h for fd_set definition. */
#ifndef ctSELECT_H
#define ctSELECT_H	<sys/select.h>
#include ctSELECT_H
#endif
#endif /* ctPortQNX */

#endif /* ctFeatNEW_DEAD_CLIENT */

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN	255
#endif /* MAXHOSTNAMELEN */
#define MAXHOSTNAME MAXHOSTNAMELEN

#ifndef INADDR_NONE
#ifdef CTNLIBV6
#define INADDR_NONE (LONG)-1
#else
#define INADDR_NONE (ctADDR_T)-1
#endif
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

#ifndef SOMAXCONN
#define SOMAXCONN 16
#endif

#endif /* ctNETWORKLIB */
/* end of ctnlib.h */

