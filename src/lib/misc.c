/*
 * misc.c	Various miscellaneous functions.
 *
 * Version:	$Id$
 */

static const char rcsid[] =
"$Id$";

#include	"autoconf.h"

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/time.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>

#include	<stdlib.h>
#include	<string.h>
#include	<netdb.h>
#include	<time.h>
#include	<ctype.h>
#include	<signal.h>

#include	"libradius.h"
#include	"missing.h"

int		librad_dodns = 1;
int		librad_debug = 0;

/*
 *	Return a printable host name (or IP address in dot notation)
 *	for the supplied IP address.
 */
char * ip_hostname(uint32_t ipaddr)
{
	struct		hostent *hp;
	static char	hstname[128];

	/*
	 *	No DNS: don't look up host names
	 */
	if (!librad_dodns) {
		ip_ntoa(hstname, ipaddr);
		return(hstname);
	}

	hp = gethostbyaddr((char *)&ipaddr, sizeof (struct in_addr), AF_INET);
	if (hp == 0) {
		ip_ntoa(hstname, ipaddr);
		return(hstname);
	}
	return (char *)hp->h_name;
}


/*
 *	Return an IP address in from a host
 *	name or address in dot notation.
 */
uint32_t ip_getaddr(const char *host)
{
	struct hostent	*hp;
	uint32_t	 a;

	if ((a = ip_addr(host)) != 0)
		return a;

	/*
	 *	No DNS: don't look up host names
	 */
	if (!librad_dodns) {
		return 0;
	}

	if ((hp = gethostbyname(host)) == NULL)
		return (uint32_t)0;

	return (*(uint32_t *)hp->h_addr);
}


/*
 *	Return an IP address in standard dot notation
 */
char *ip_ntoa(char *buffer, uint32_t ipaddr)
{
	struct in_addr	in;
	char		*r;

	in.s_addr = ipaddr;
	r = inet_ntoa(in);

	if (buffer)
		strcpy(buffer, r);
	return buffer ? buffer : r;
}


/*
 *	Return an IP address from
 *	one supplied in standard dot notation.
 */
uint32_t ip_addr(const char *ip_str)
{
	struct in_addr	in;

	if (inet_aton(ip_str, &in) == 0)
		return 0;
	return in.s_addr;
}


/*
 *	Like strncpy, but always adds \0
 */
char *strNcpy(char *dest, const char *src, int n)
{
	if (n > 0)
		strncpy(dest, src, n);
	else
		n = 1;
	dest[n - 1] = 0;

	return dest;
}

