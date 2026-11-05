/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 21:45:13 by romasant          #+#    #+#             */
/*   Updated: 2026/11/05 11:59:37 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*   ft_printf_tester.c — testeur autonome pour ft_printf (42, sujet v1.1)   */
/*                                                                            */
/*   Conversions du sujet : c s p d i u x X %                                */
/*   Chaque groupe de tests tourne dans un processus fils : un segfault,     */
/*   une boucle infinie ou un double free n'arrête pas les autres groupes.   */
/*   Chaque cas compare ft_printf à printf : octets écrits ET valeur de      */
/*   retour. Le testeur ne contient aucune solution : il ne fait que casser. */
/*                                                                            */
/*   Compilation (depuis la racine du dépôt) :                               */
/*     cc -Wall -Wextra -g3 tester/ft_printf_tester.c \                      */
/*        -L. -lftprintf -o tester/ft_printf_tester                          */
/*   Avec détection de fuites / débordements (si ta machine a libasan) :     */
/*     ajoute -fsanitize=address                                              */
/*                                                                            */
/*   Ce fichier n'a pas vocation à passer la Norminette : ne le rends pas.   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*   ft_printf_tester.c — testeur autonome pour ft_printf (42, sujet v1.1)   */
/*                                                                            */
/*   Conversions du sujet : c s p d i u x X %                                */
/*   Chaque groupe de tests tourne dans un processus fils : un segfault,     */
/*   une boucle infinie ou un double free n'arrête pas les autres groupes.   */
/*   Chaque cas compare ft_printf à printf : octets écrits ET valeur de      */
/*   retour. Le testeur ne contient aucune solution : il ne fait que casser. */
/*                                                                            */
/*   Compilation (depuis la racine du dépôt) :                               */
/*     cc -Wall -Wextra -g3 tester/ft_printf_tester.c \                      */
/*        -L. -lftprintf -o tester/ft_printf_tester                          */
/*   -g3 est important : il donne le fichier:ligne exact de chaque erreur    */
/*   (segfault, boucle infinie, récursion infinie, double free...).          */
/*   Avec détection de fuites / débordements (si ta machine a libasan) :     */
/*     ajoute -fsanitize=address                                              */
/*                                                                            */
/*   Ce fichier n'a pas vocation à passer la Norminette : ne le rends pas.   */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include <execinfo.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "../ft_printf.h"

#if defined(__clang__)
# pragma clang diagnostic ignored "-Wformat"
# pragma clang diagnostic ignored "-Wformat-security"
# pragma clang diagnostic ignored "-Wformat-extra-args"
# pragma clang diagnostic ignored "-Wformat-nonliteral"
#else
# pragma GCC diagnostic ignored "-Wformat"
# pragma GCC diagnostic ignored "-Wformat-security"
# pragma GCC diagnostic ignored "-Wformat-extra-args"
# pragma GCC diagnostic ignored "-Wformat-overflow"
# pragma GCC diagnostic ignored "-Wformat-truncation"
# pragma GCC diagnostic ignored "-Wformat-zero-length"
# pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif

/*
** Symbole faible fourni par LeakSanitizer (présent seulement avec
** -fsanitize=address). Sans sanitizer, le pointeur vaut NULL : aucun effet.
*/
extern int	__lsan_do_recoverable_leak_check(void) __attribute__((weak));
extern void	__asan_init(void) __attribute__((weak));

/*
** UBSan : demande la pile d'appels avec chaque "runtime error". Ignoré si
** UBSan n'est pas utilisé.
*/
const char	*__ubsan_default_options(void)
{
	return ("print_stacktrace=1");
}

/* ----------------------------- couleurs ---------------------------------- */

static const char	*g_r = "";
static const char	*g_b = "";
static const char	*g_d = "";
static const char	*g_acc = "";
static const char	*g_txt = "";

static const char	*g_p_ok = "";
static const char	*g_p_ko = "";
static const char	*g_p_cr = "";
static const char	*g_p_lk = "";
static const char	*g_p_tm = "";
static const char	*g_p_in = "";
static const char	*g_p_ub = "";
static const char	*g_f_ok = "";
static const char	*g_f_ko = "";

static void	set_colors(void)
{
	if (getenv("NO_COLOR") != NULL || !isatty(STDOUT_FILENO))
		return ;
	g_r = "\033[0m";
	g_b = "\033[1m";
	g_d = "\033[38;5;245m";
	g_acc = "\033[38;5;213m";
	g_txt = "\033[38;5;252m";
	g_p_ok = "\033[30;48;5;78m";
	g_p_ko = "\033[30;48;5;203m";
	g_p_cr = "\033[97;48;5;125m";
	g_p_lk = "\033[30;48;5;221m";
	g_p_tm = "\033[30;48;5;141m";
	g_p_in = "\033[38;5;245;48;5;236m";
	g_p_ub = "\033[30;48;5;215m";
	g_f_ok = "\033[38;5;78m";
	g_f_ko = "\033[38;5;203m";
}

/* ------------------------- mémoire partagée ------------------------------ */

# define MAXF 6
# define DESCSZ 320
# define FAILSZ 1000
# define BTMAX 48

typedef struct s_shm
{
	int		total;
	int		ko;
	int		soft;
	int		nfail;
	int		leak;
	int		nbt;
	uintptr_t	fault;
	void	*bt[BTMAX];
	char	cur[DESCSZ];
	char	fail[MAXF][FAILSZ];
}	t_shm;

static t_shm		*g_shm;
static int			g_tmp = -1;
static int			g_soft;
static unsigned int	g_fuzz = 500;
static unsigned int	g_timeout = 10;
static int			g_verbose;
static uint64_t		g_seed;
static uint64_t		g_rng;

/* ------------------------------ aléatoire -------------------------------- */

static uint64_t	rng_next(void)
{
	g_rng ^= g_rng << 13;
	g_rng ^= g_rng >> 7;
	g_rng ^= g_rng << 17;
	return (g_rng);
}

static int	rng_int(int min, int max)
{
	return (min + (int)(rng_next() % (uint64_t)(max - min + 1)));
}

static void	rnd_text(char *buf, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		buf[i] = (char)rng_int(32, 126);
		if (buf[i] == '%')
			buf[i] = '#';
		i++;
	}
	buf[len] = '\0';
}

static const int	g_ints[] = {0, 1, -1, 2, 7, 9, 10, 11, 42, -42, 99, 100,
	101, 255, 256, 999, 1000, 1001, 12345, -12345, 65535, 65536, 1000000,
	-1000000, 1000000000, -1000000000, 2147483646, INT_MAX, INT_MIN,
	INT_MIN + 1};
static const unsigned int	g_uints[] = {0u, 1u, 9u, 10u, 15u, 16u, 17u,
	255u, 256u, 4095u, 4096u, 65535u, 65536u, 1000000u, 2147483647u,
	2147483648u, 4294967294u, 4294967295u, 0xdeadbeefu, 0xcafebabeu,
	0x12345678u, 0xabcdefu};
# define NINTS ((int)(sizeof(g_ints) / sizeof(g_ints[0])))
# define NUINTS ((int)(sizeof(g_uints) / sizeof(g_uints[0])))

static int	rnd_int(void)
{
	int	mode;

	mode = rng_int(0, 3);
	if (mode == 0)
		return ((int)rng_next());
	if (mode == 1)
		return (rng_int(-1000, 1000));
	if (mode == 2)
		return (g_ints[rng_int(0, NINTS - 1)]);
	return ((int)(rng_next() >> rng_int(33, 63)) * (rng_int(0, 1) ? 1 : -1));
}

static unsigned int	rnd_uint(void)
{
	int	mode;

	mode = rng_int(0, 3);
	if (mode == 0)
		return ((unsigned int)rng_next());
	if (mode == 1)
		return ((unsigned int)rng_int(0, 1000));
	if (mode == 2)
		return (g_uints[rng_int(0, NUINTS - 1)]);
	return ((unsigned int)(rng_next() >> rng_int(32, 63)));
}

static void	*rnd_ptr(void)
{
	static int	local;
	int			mode;

	mode = rng_int(0, 4);
	if (mode == 0)
		return (NULL);
	if (mode == 1)
		return ((void *)&local);
	if (mode == 2)
		return ((void *)(uintptr_t)(rng_next() >> rng_int(0, 63)));
	if (mode == 3)
		return ((void *)(uintptr_t)rng_int(0, 300));
	return ((void *)(uintptr_t)rng_next());
}

/* ---------------------- formatage de messages ---------------------------- */

static char	*dd(const char *fmt, ...)
{
	static char	buf[DESCSZ];
	va_list		ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	return (buf);
}

static void	esc(char *out, size_t cap, const char *s, size_t len)
{
	size_t			i;
	size_t			o;
	unsigned char	c;

	i = 0;
	o = 0;
	while (i < len && o + 8 < cap)
	{
		c = (unsigned char)s[i++];
		if (c == '\n')
			o += (size_t)snprintf(out + o, cap - o, "\\n");
		else if (c == '\t')
			o += (size_t)snprintf(out + o, cap - o, "\\t");
		else if (c == '\0')
			o += (size_t)snprintf(out + o, cap - o, "\\0");
		else if (c == '"')
			o += (size_t)snprintf(out + o, cap - o, "\\\"");
		else if (c == '\\')
			o += (size_t)snprintf(out + o, cap - o, "\\\\");
		else if (c < 32 || c >= 127)
			o += (size_t)snprintf(out + o, cap - o, "\\x%02x", c);
		else
			out[o++] = (char)c;
	}
	if (i < len)
		o += (size_t)snprintf(out + o, cap - o, "...(+%zu)", len - i);
	out[o < cap ? o : cap - 1] = '\0';
}

/* ------------------------- capture des sorties --------------------------- */

static char		*g_a;
static size_t	g_alen;
static int		g_ret_ft;
static int		g_ret_std;

static void	tmp_reset(void)
{
	if (ftruncate(g_tmp, 0) != 0 || lseek(g_tmp, 0, SEEK_SET) < 0)
		_exit(2);
}

static char	*slurp(size_t *len)
{
	off_t	size;
	char	*buf;
	size_t	got;
	ssize_t	n;

	size = lseek(g_tmp, 0, SEEK_END);
	if (size < 0 || lseek(g_tmp, 0, SEEK_SET) < 0)
		_exit(2);
	buf = malloc((size_t)size + 1);
	if (buf == NULL)
		_exit(2);
	got = 0;
	while (got < (size_t)size)
	{
		n = read(g_tmp, buf + got, (size_t)size - got);
		if (n <= 0)
			break ;
		got += (size_t)n;
	}
	buf[got] = '\0';
	*len = got;
	return (buf);
}

static void	cap_begin(const char *desc)
{
	snprintf(g_shm->cur, DESCSZ, "%s", desc);
	fflush(stdout);
	tmp_reset();
}

static void	cap_mid(void)
{
	fflush(stdout);
	g_a = slurp(&g_alen);
	tmp_reset();
}

static void	record(const char *desc, const char *b, size_t blen)
{
	char	ea[300];
	char	eb[300];

	if (g_soft)
		g_shm->soft++;
	else
		g_shm->ko++;
	if (g_shm->nfail >= MAXF)
		return ;
	esc(ea, sizeof(ea), g_a, g_alen);
	esc(eb, sizeof(eb), b, blen);
	snprintf(g_shm->fail[g_shm->nfail++], FAILSZ,
		"%.150s\n  obtenu  : \"%s\"  (retour %d)\n"
		"  attendu : \"%s\"  (retour %d)",
		desc, ea, g_ret_ft, eb, g_ret_std);
}

static void	cap_end(const char *desc)
{
	char	*b;
	size_t	blen;

	fflush(stdout);
	b = slurp(&blen);
	g_shm->total++;
	if (g_alen != blen || memcmp(g_a, b, blen) != 0 || g_ret_ft != g_ret_std)
		record(desc, b, blen);
	free(b);
	free(g_a);
	g_a = NULL;
}

# define CASE_D(desc, ...) do { \
	const char	*d__ = (desc); \
	cap_begin(d__); \
	g_ret_ft = ft_printf(__VA_ARGS__); \
	cap_mid(); \
	g_ret_std = printf(__VA_ARGS__); \
	cap_end(d__); \
} while (0)

# define CASE(...) CASE_D("ft_printf(" #__VA_ARGS__ ")", __VA_ARGS__)

/* ------------------------------ groupes ---------------------------------- */

static void	grp_c(void)
{
	int	v;

	v = 0;
	while (v < 256)
	{
		CASE_D(dd("\"%%c\", %d", v), "%c", v);
		v++;
	}
	CASE_D(dd("\"%%c\", -1"), "%c", -1);
	CASE_D(dd("\"%%c\", 300"), "%c", 300);
	CASE("[%c]", 'A');
	CASE("%c%c%c", 'A', 'B', 'C');
	CASE("%c%c%c", 'A', '\0', 'B');
	CASE("%c%c", '\0', '\0');
	CASE("%c\n", '\n');
	CASE("a%cb%cc", ' ', '\t');
	CASE("%c%c%c%c%c", '4', '2', ' ', '!', '\n');
	CASE("%c", (char)126);
	CASE("%c", (char)-1);
}

static void	grp_s(void)
{
	static const char	*pool[] = {"", "a", "hello", "Hello, World!", "%d",
		"%%", "%s", "%", "\n", "tab\there", " ", "   ", "TEsT!?~", "8940",
		"\x80\xff", "h\xc3\xa9llo w\xc3\xb6rld \xe2\x9c\x93",
		"ligne1\nligne2\nligne3", "0123456789abcdefghijklmnopqrstuvwxyz"};
	char				*big;
	int					i;

	i = 0;
	while (i < (int)(sizeof(pool) / sizeof(pool[0])))
	{
		CASE_D(dd("\"%%s\", \"%.60s\"", pool[i]), "%s", pool[i]);
		CASE_D(dd("\"[%%s]\", \"%.60s\"", pool[i]), "[%s]", pool[i]);
		CASE_D(dd("\"%%s|%%s\", \"%.40s\" x2", pool[i]), "%s|%s", pool[i],
			pool[i]);
		i++;
	}
	CASE("%s", (char *)NULL);
	CASE("[%s]", (char *)NULL);
	CASE("%s%s", (char *)NULL, "x");
	CASE("%s %s %s", "a", (char *)NULL, "b");
	CASE("%s", "\0");
	CASE("[char*]  : %s\n%s\n%s\n%s\n", "\0", "TEsT!?~", "8940", "");
	big = malloc(10001);
	if (big == NULL)
		return ;
	memset(big, 'x', 10000);
	big[10000] = '\0';
	CASE_D("\"%s\", <10000 x 'x'>", "%s", big);
	CASE_D("\"a%sb\", <10000 x 'x'>", "a%sb", big);
	free(big);
}

static void	grp_p(void)
{
	int			local;
	char		c;
	char		*h;
	void		*pool[12];
	int			i;

	local = 42;
	c = '1';
	h = malloc(5);
	pool[0] = NULL;
	pool[1] = (void *)(uintptr_t)1;
	pool[2] = (void *)(uintptr_t)15;
	pool[3] = (void *)(uintptr_t)16;
	pool[4] = (void *)(uintptr_t)255;
	pool[5] = (void *)(uintptr_t)0xdeadbeefUL;
	pool[6] = (void *)(uintptr_t)0x7fffffffffffUL;
	pool[7] = (void *)(uintptr_t)ULONG_MAX;
	pool[8] = (void *)(uintptr_t)LONG_MAX;
	pool[9] = (void *)(uintptr_t)LONG_MIN;
	pool[10] = (void *)&local;
	pool[11] = (void *)&c;
	i = 0;
	while (i < 12)
	{
		CASE_D(dd("\"%%p\", %p", pool[i]), "%p", pool[i]);
		CASE_D(dd("\"[%%p]\", %p", pool[i]), "[%p]", pool[i]);
		CASE_D(dd("\"%%p %%p\", %p x2", pool[i]), "%p %p", pool[i], pool[i]);
		i++;
	}
	CASE("%p", (void *)h);
	CASE("%p %p", (void *)h, (void *)&h);
	CASE("%p%p%p", (void *)&local, (void *)&c, (void *)h);
	CASE("[void*]  : %p\n%p\n%p\n%p\n", (void *)&local, (void *)&c,
		(void *)h, (void *)&local);
	CASE("%p", (void *)0);
	CASE("%p", (void *)"texte");
	free(h);
}

static void	grp_int(const char *fmt)
{
	int	i;

	i = 0;
	while (i < NINTS)
	{
		CASE_D(dd("\"%s\", %d", fmt, g_ints[i]), fmt, g_ints[i]);
		i++;
	}
	i = 0;
	while (i < NINTS)
	{
		CASE_D(dd("\"<%s>\", %d", fmt, g_ints[i]), "<%d>", g_ints[i]);
		i++;
	}
	CASE("%d%d", 1, 2);
	CASE("%d %d %d", INT_MIN, 0, INT_MAX);
	CASE("%d,%d,%d,%d,%d\n", 42, INT_MAX, INT_MIN, 0, -42);
	CASE("[int]  : %d\n%d\n%d\n%d\n%d\n", 42, INT_MAX, INT_MIN, 0, -42);
	CASE("%d%%", 50);
}

static void	grp_d(void)
{
	int	i;

	grp_int("%d");
	i = 0;
	while (i < 100)
	{
		CASE_D(dd("\"%%d\", %d", i), "%d", i);
		CASE_D(dd("\"%%d\", %d", -i), "%d", -i);
		i++;
	}
}

static void	grp_i(void)
{
	int	i;

	i = 0;
	while (i < NINTS)
	{
		CASE_D(dd("\"%%i\", %d", g_ints[i]), "%i", g_ints[i]);
		CASE_D(dd("\"<%%i>\", %d", g_ints[i]), "<%i>", g_ints[i]);
		i++;
	}
	i = 0;
	while (i < 100)
	{
		CASE_D(dd("\"%%i\", %d", i), "%i", i);
		CASE_D(dd("\"%%i\", %d", -i), "%i", -i);
		i++;
	}
	CASE("%i%i", 1, -2);
	CASE("%i %d", INT_MIN, INT_MIN);
	CASE("%i", 042);
	CASE("%i", 0x2a);
}

static void	grp_u(void)
{
	int	i;

	i = 0;
	while (i < NUINTS)
	{
		CASE_D(dd("\"%%u\", %uu", g_uints[i]), "%u", g_uints[i]);
		CASE_D(dd("\"<%%u>\", %uu", g_uints[i]), "<%u>", g_uints[i]);
		i++;
	}
	CASE("%u", (unsigned int)-1);
	CASE("%u", (unsigned int)INT_MIN);
	CASE("%u", (unsigned int)-42);
	CASE("%u", -1);
	CASE("%u", UINT_MAX);
	CASE("%u %u", 0u, UINT_MAX);
	i = 0;
	while (i < 100)
	{
		CASE_D(dd("\"%%u\", %d", i), "%u", (unsigned int)i);
		i++;
	}
}

static void	grp_x(void)
{
	int	i;

	i = 0;
	while (i < NUINTS)
	{
		CASE_D(dd("\"%%x\", %#xu", g_uints[i]), "%x", g_uints[i]);
		CASE_D(dd("\"<%%x>\", %#xu", g_uints[i]), "<%x>", g_uints[i]);
		i++;
	}
	CASE("%x", -1);
	CASE("%x", INT_MIN);
	CASE("%x", -42);
	CASE("%x", UINT_MAX);
	CASE("%x %x", 0, 0xabcdef);
	CASE("0x%x", 255);
	i = 0;
	while (i < 300)
	{
		CASE_D(dd("\"%%x\", %d", i), "%x", (unsigned int)i);
		i++;
	}
}

static void	grp_xx(void)
{
	int	i;

	i = 0;
	while (i < NUINTS)
	{
		CASE_D(dd("\"%%X\", %#xu", g_uints[i]), "%X", g_uints[i]);
		CASE_D(dd("\"<%%X>\", %#xu", g_uints[i]), "<%X>", g_uints[i]);
		i++;
	}
	CASE("%X", -1);
	CASE("%X", INT_MIN);
	CASE("%X", UINT_MAX);
	CASE("%X %x", 0xabcdef, 0xabcdef);
	CASE("0x%X", 255);
	i = 0;
	while (i < 300)
	{
		CASE_D(dd("\"%%X\", %d", i), "%X", (unsigned int)i);
		i++;
	}
}

static void	grp_percent(void)
{
	CASE("%%");
	CASE("%%%%");
	CASE("%%%%%%");
	CASE("a%%b");
	CASE("100%%");
	CASE("%%d");
	CASE("%%s");
	CASE("%%c");
	CASE("%%%%d");
	CASE("%%%d", 42);
	CASE("%d%%", 42);
	CASE("%c%%%c", 'a', 'b');
	CASE("%%%s%%", "mid");
	CASE("%%%%%%%%");
	CASE("%% %% %%");
	CASE("%%\n");
	CASE("\n%%");
}

static void	grp_texte(void)
{
	char	*big;

	CASE("");
	CASE("a");
	CASE(" ");
	CASE("\n");
	CASE("\n\n\n");
	CASE("Hello");
	CASE("Hello, World!\n");
	CASE("42 \t tabulation");
	CASE("h\xc3\xa9llo w\xc3\xb6rld \xe2\x9c\x93\n");
	big = malloc(10001);
	if (big == NULL)
		return ;
	memset(big, 'z', 10000);
	big[10000] = '\0';
	CASE_D("<10000 x 'z'>", big);
	free(big);
}

static void	grp_melanges(void)
{
	int	local;

	local = 7;
	CASE("%c%s%d", 'A', "bc", 42);
	CASE("%s %s", "Hello", "World");
	CASE("%s%s%s", "a", "b", "c");
	CASE("%d %i %u", -1, -1, 1u);
	CASE("%x %X %x", 255, 255, 3054);
	CASE("%c %s %p %d %i %u %x %X %%", 'a', "str", (void *)&local, 1, 2, 3u,
		255u, 255u);
	CASE("%X %x %u %i %d %p %s %c", 255u, 255u, 3u, 2, 1, (void *)&local,
		"str", 'a');
	CASE("Nombre : %d, hex : %x, ptr : %p\n", 42, 42, (void *)&local);
	CASE("%s a %d b %c c %u", "s", 1, 'x', 2u);
	CASE("<%d><%s><%c>", 0, "", 'z');
	CASE("%d%d%d%d%d", 1, 2, 3, 4, 5);
	CASE("%s%d%s%d", "a", 1, "b", 2);
	CASE("%c%c%c", 'a', 'b', 'c');
	CASE("%%%c%%%s%%%d%%", 'a', "b", 3);
	CASE("%s", "%d");
	CASE("%s%s", "%", "d");
	CASE("[%d]\n[%s]\n[%c]\n", 5, "five", '5');
	CASE("[int]  : %d\n%d\n%d\n%d\n%d\n", 42, INT_MAX, INT_MIN, 0, -42);
	CASE("%d%s", INT_MIN, (char *)NULL);
}

static void	grp_args(void)
{
	CASE("%d %d %d %d %d %d %d %d %d %d %d %d", 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12);
	CASE("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 1, 2, 3, 4, 5, 6,
		7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	CASE("%c%c%c%c%c%c%c%c%c%c%c%c", 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
		'i', 'j', 'k', 'l');
	CASE("%s%s%s%s%s%s%s%s", "1", "2", "3", "4", "5", "6", "7", "8");
	CASE("%u %u %u %u %u %u %u %u %u", 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u);
	CASE("%x %X %x %X %x %X %x %X", 10, 11, 12, 13, 14, 15, 16, 17);
	CASE("%p %p %p %p %p %p %p %p", (void *)1, (void *)2, (void *)3,
		(void *)4, (void *)5, (void *)6, (void *)7, (void *)8);
	CASE("%d %s %c %u %x %X %p %i %d %s %c %u", 1, "a", 'b', 2u, 3u, 4u,
		(void *)5, 6, 7, "c", 'd', 8u);
	CASE("%d%%%d%%%d%%%d%%%d%%%d%%%d%%%d", 1, 2, 3, 4, 5, 6, 7, 8);
}

static void	grp_gros(void)
{
	static const size_t	sizes[] = {1, 255, 256, 1000, 4095, 4096, 4097, 8192,
		65535, 65536, 100000, 200000};
	char				*big;
	char				*fmt;
	size_t				i;
	size_t				k;

	i = 0;
	while (i < sizeof(sizes) / sizeof(sizes[0]))
	{
		big = malloc(sizes[i] + 1);
		fmt = malloc(sizes[i] + 8);
		if (big == NULL || fmt == NULL)
			return ;
		k = 0;
		while (k < sizes[i])
		{
			big[k] = (char)('a' + (k % 26));
			k++;
		}
		big[k] = '\0';
		CASE_D(dd("\"%%s\", <%zu caracteres>", sizes[i]), "%s", big);
		CASE_D(dd("\"<%%s>\", <%zu caracteres>", sizes[i]), "<%s>", big);
		CASE_D(dd("\"%%s%%d\", <%zu car.>, 42", sizes[i]), "%s%d", big, 42);
		CASE_D(dd("<format de %zu caracteres, sans conversion>", sizes[i]),
			big);
		snprintf(fmt, sizes[i] + 8, "%s", big);
		if (sizes[i] > 3)
			memcpy(fmt + sizes[i] / 2, "%d", 2);
		CASE_D(dd("<format de %zu car. avec un %%d au milieu>, 7", sizes[i]),
			fmt, 7);
		free(big);
		free(fmt);
		i++;
	}
}

static void	fuzz_one(char conv)
{
	unsigned int	i;
	char			pre[16];
	char			suf[16];
	char			fmt[48];
	char			str[160];
	char			sdesc[200];

	i = 0;
	while (i++ < g_fuzz)
	{
		rnd_text(pre, rng_int(0, 8));
		rnd_text(suf, rng_int(0, 8));
		snprintf(fmt, sizeof(fmt), "%s%%%c%s", pre, conv, suf);
		if (conv == 'c')
		{
			int v = rng_int(-128, 400);
			CASE_D(dd("\"%s\", %d", fmt, v), fmt, v);
		}
		else if (conv == 's')
		{
			char	*sp;
			size_t	len;

			len = (size_t)rng_int(0, 120);
			rnd_text(str, (int)len);
			sp = (rng_int(0, 19) == 0) ? NULL : str;
			esc(sdesc, sizeof(sdesc), sp ? sp : "(NULL)", sp ? len : 6);
			CASE_D(dd("\"%s\", \"%.100s\"", fmt, sdesc), fmt, sp);
		}
		else if (conv == 'p')
		{
			void *v = rnd_ptr();
			CASE_D(dd("\"%s\", %p", fmt, v), fmt, v);
		}
		else if (conv == 'd' || conv == 'i')
		{
			int v = rnd_int();
			CASE_D(dd("\"%s\", %d", fmt, v), fmt, v);
		}
		else
		{
			unsigned int v = rnd_uint();
			CASE_D(dd("\"%s\", %uu", fmt, v), fmt, v);
		}
	}
}

static void	fuzz_c(void) { fuzz_one('c'); }
static void	fuzz_s(void) { fuzz_one('s'); }
static void	fuzz_p(void) { fuzz_one('p'); }
static void	fuzz_d(void) { fuzz_one('d'); }
static void	fuzz_i(void) { fuzz_one('i'); }
static void	fuzz_u(void) { fuzz_one('u'); }
static void	fuzz_x(void) { fuzz_one('x'); }
static void	fuzz_xx(void) { fuzz_one('X'); }

static void	fuzz_mix(void)
{
	unsigned int	i;
	char			s1[64];
	char			s2[64];
	char			s3[64];
	int				a[12];
	unsigned int	u[8];
	void			*p[8];
	int				k;

	i = 0;
	while (i++ < g_fuzz)
	{
		rnd_text(s1, rng_int(0, 30));
		rnd_text(s2, rng_int(0, 30));
		rnd_text(s3, rng_int(0, 30));
		k = 0;
		while (k < 12)
		{
			a[k] = rnd_int();
			if (k < 8)
			{
				u[k] = rnd_uint();
				p[k] = rnd_ptr();
			}
			k++;
		}
		k = rng_int(0, 8);
		if (k == 0)
			CASE_D(dd("\"%%c%%s%%p%%d%%i%%u%%x%%X%%%%\" c=%d s=\"%.20s\" p=%p "
					"d=%d i=%d u=%u x=%u X=%u", a[0] & 255, s1, p[0], a[1],
					a[2], u[0], u[1], u[2]),
				"%c%s%p%d%i%u%x%X%%", a[0] & 255, s1, p[0], a[1], a[2], u[0],
				u[1], u[2]);
		else if (k == 1)
			CASE_D(dd("\"%%X|%%x|%%u|%%i|%%d|%%p|%%s|%%c\" X=%u x=%u u=%u "
					"i=%d d=%d p=%p s=\"%.20s\" c=%d", u[0], u[1], u[2], a[0],
					a[1], p[0], s1, a[2] & 255),
				"%X|%x|%u|%i|%d|%p|%s|%c", u[0], u[1], u[2], a[0], a[1], p[0],
				s1, a[2] & 255);
		else if (k == 2)
			CASE_D(dd("\"%%d%%s%%d%%s%%d%%s\" %d \"%.15s\" %d \"%.15s\" %d "
					"\"%.15s\"", a[0], s1, a[1], s2, a[2], s3),
				"%d%s%d%s%d%s", a[0], s1, a[1], s2, a[2], s3);
		else if (k == 3)
			CASE_D(dd("12 x %%d : %d %d %d %d %d %d %d %d %d %d %d %d",
					a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8],
					a[9], a[10], a[11]),
				"%d %d %d %d %d %d %d %d %d %d %d %d", a[0], a[1], a[2],
				a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11]);
		else if (k == 4)
			CASE_D(dd("10 x %%c : %d %d %d %d %d %d %d %d %d %d", a[0] & 255,
					a[1] & 255, a[2] & 255, a[3] & 255, a[4] & 255,
					a[5] & 255, a[6] & 255, a[7] & 255, a[8] & 255,
					a[9] & 255),
				"%c%c%c%c%c%c%c%c%c%c", a[0] & 255, a[1] & 255, a[2] & 255,
				a[3] & 255, a[4] & 255, a[5] & 255, a[6] & 255, a[7] & 255,
				a[8] & 255, a[9] & 255);
		else if (k == 5)
			CASE_D(dd("8 x %%x/%%X : %u %u %u %u %u %u %u %u", u[0], u[1],
					u[2], u[3], u[4], u[5], u[6], u[7]),
				"%x %X %x %X %x %X %x %X", u[0], u[1], u[2], u[3], u[4], u[5],
				u[6], u[7]);
		else if (k == 6)
			CASE_D(dd("8 x %%p : %p %p %p %p %p %p %p %p", p[0], p[1], p[2],
					p[3], p[4], p[5], p[6], p[7]),
				"%p %p %p %p %p %p %p %p", p[0], p[1], p[2], p[3], p[4], p[5],
				p[6], p[7]);
		else if (k == 7)
			CASE_D(dd("8 x %%u : %u %u %u %u %u %u %u %u", u[0], u[1], u[2],
					u[3], u[4], u[5], u[6], u[7]),
				"%u %u %u %u %u %u %u %u", u[0], u[1], u[2], u[3], u[4], u[5],
				u[6], u[7]);
		else
			CASE_D(dd("\"txt %%%% %%d %%%% %%s %%%% %%c %%%%\" %d \"%.20s\" "
					"%d", a[0], s1, a[1] & 255),
				"txt %% %d %% %s %% %c %%", a[0], s1, a[1] & 255);
	}
}

/*
** Comportements indéfinis ou hors sujet (flags, largeur, précision,
** conversions inconnues) : comparés à printf pour information seulement,
** jamais comptés comme des échecs.
*/
static void	grp_indefini(void)
{
	CASE("%");
	CASE("abc%");
	CASE("%z");
	CASE("%y%d", 1);
	CASE("%5d", 42);
	CASE("%-5d|", 42);
	CASE("%05d", 42);
	CASE("%.3d", 7);
	CASE("%#x", 255);
	CASE("%+d", 42);
	CASE("% d", 42);
	CASE("%10s|", "abc");
	CASE("%.2s", "abcdef");
	CASE("%ld", 123456789012L);
	CASE("%lu", 123456789012UL);
	CASE("%hhd", 300);
	CASE("%5c|", 'x');
	CASE("%20p|", (void *)&g_soft);
}

/* ------------------------- liste et exécution ---------------------------- */

typedef struct s_group
{
	const char	*name;
	const char	*title;
	void		(*fn)(void);
	int			soft;
}	t_group;

static const t_group	g_groups[] = {
	{"conv_c", "%c : tous les octets 0..255, NUL, négatifs", grp_c, 0},
	{"conv_s", "%s : vide, NULL, '%', accents, 10000 caractères", grp_s, 0},
	{"conv_p", "%p : NULL, extrêmes, pile, tas", grp_p, 0},
	{"conv_d", "%d : INT_MIN, INT_MAX, 0, -1, puissances", grp_d, 0},
	{"conv_i", "%i : comme %d", grp_i, 0},
	{"conv_u", "%u : UINT_MAX, négatifs convertis, 0", grp_u, 0},
	{"conv_x", "%x : minuscules, UINT_MAX, négatifs", grp_x, 0},
	{"conv_X", "%X : majuscules, UINT_MAX, négatifs", grp_xx, 0},
	{"percent", "%% : seuls, enchaînés, collés aux autres", grp_percent, 0},
	{"texte", "texte seul, sans conversion", grp_texte, 0},
	{"melanges", "conversions mélangées dans un même appel", grp_melanges, 0},
	{"args", "beaucoup d'arguments (12 à 16, passés sur la pile)", grp_args, 0},
	{"gros", "grosses sorties : 4096, 65536, 200000 octets", grp_gros, 0},
	{"fuzz_c", "fuzz %c : valeurs et textes autour aléatoires", fuzz_c, 0},
	{"fuzz_s", "fuzz %s : longueurs et contenus aléatoires", fuzz_s, 0},
	{"fuzz_p", "fuzz %p : pointeurs aléatoires", fuzz_p, 0},
	{"fuzz_d", "fuzz %d : entiers aléatoires", fuzz_d, 0},
	{"fuzz_i", "fuzz %i : entiers aléatoires", fuzz_i, 0},
	{"fuzz_u", "fuzz %u : non signés aléatoires", fuzz_u, 0},
	{"fuzz_x", "fuzz %x : non signés aléatoires", fuzz_x, 0},
	{"fuzz_X", "fuzz %X : non signés aléatoires", fuzz_xx, 0},
	{"fuzz_mix", "fuzz mélanges : formats et arguments aléatoires", fuzz_mix, 0},
	{"indefini", "hors sujet / indéfini (info seulement, non noté)",
		grp_indefini, 1},
};

# define NGROUPS ((int)(sizeof(g_groups) / sizeof(g_groups[0])))

typedef enum e_status
{
	ST_OK, ST_KO, ST_CRASH, ST_LEAK, ST_TIME, ST_UB, ST_INFO
}	t_status;

static double	now_ms(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ((double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6);
}

static char	*read_file(int fd)
{
	off_t	size;
	char	*buf;
	ssize_t	n;
	size_t	got;

	size = lseek(fd, 0, SEEK_END);
	if (size < 0 || lseek(fd, 0, SEEK_SET) < 0)
		return (NULL);
	if (size > 200000)
		size = 200000;
	buf = malloc((size_t)size + 1);
	if (buf == NULL)
		return (NULL);
	got = 0;
	while (got < (size_t)size)
	{
		n = read(fd, buf + got, (size_t)size - got);
		if (n <= 0)
			break ;
		got += (size_t)n;
	}
	buf[got] = '\0';
	return (buf);
}

static void	detail(const char *s)
{
	const char	*e;

	while (*s != '\0')
	{
		e = strchr(s, '\n');
		printf("            %s%.*s%s\n", g_d, e ? (int)(e - s) : (int)strlen(s),
			s, g_r);
		if (e == NULL)
			break ;
		s = e + 1;
	}
}

/* --------------- localisation exacte : fichier:ligne des erreurs ---------- */

typedef struct s_frame
{
	uintptr_t	pc;
	char		fn[96];
	char		loc[256];
	int			ours;
	int			known;
}	t_frame;

static char	g_altstack[65536];
static int	g_need_g;
static int	g_crashes;

/*
** Dans le fils : à la réception d'un signal fatal (segfault, abort, timeout),
** on note la pile d'appels dans la mémoire partagée, puis on laisse le
** signal tuer le processus (le père voit toujours "tué par SIGxxx").
** Le handler tourne sur une pile à part : il marche même après un
** débordement de pile (récursion infinie).
*/
static void	on_fatal(int sig, siginfo_t *si, void *uc)
{
	void	*bt[BTMAX + 2];
	int		n;
	int		k;

	(void)uc;
	n = backtrace(bt, BTMAX + 2);
	k = 0;
	while (k + 2 < n)
	{
		g_shm->bt[k] = bt[k + 2];
		k++;
	}
	g_shm->nbt = k;
	g_shm->fault = (uintptr_t)si->si_addr;
	raise(sig);
}

static void	install_handlers(void)
{
	struct sigaction	sa;
	stack_t				ss;
	void				*warm[4];
	int					sigs[5];
	int					i;

	backtrace(warm, 4);
	ss.ss_sp = g_altstack;
	ss.ss_size = sizeof(g_altstack);
	ss.ss_flags = 0;
	sigaltstack(&ss, NULL);
	memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = on_fatal;
	sa.sa_flags = SA_SIGINFO | SA_ONSTACK | SA_RESETHAND | SA_NODEFER;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGALRM, &sa, NULL);
	if (__asan_init != NULL)
		return ;
	sigs[0] = SIGSEGV;
	sigs[1] = SIGBUS;
	sigs[2] = SIGFPE;
	sigs[3] = SIGILL;
	sigs[4] = SIGABRT;
	i = 0;
	while (i < 5)
		sigaction(sigs[i++], &sa, NULL);
}

static const char	*base_name(const char *p)
{
	const char	*s;

	s = strrchr(p, '/');
	return (s != NULL ? s + 1 : p);
}

/* Trouve le fichier qui contient l'adresse a (lecture de /proc/self/maps). */
static int	map_find(uintptr_t a, char *path, size_t cap, uintptr_t *base)
{
	FILE			*f;
	char			*line;
	size_t			sz;
	unsigned long	lo;
	unsigned long	hi;
	char			cur[512];
	int				found;

	f = fopen("/proc/self/maps", "r");
	if (f == NULL)
		return (0);
	line = NULL;
	sz = 0;
	found = 0;
	while (getline(&line, &sz, f) > 0)
	{
		cur[0] = '\0';
		if (sscanf(line, "%lx-%lx %*s %*s %*s %*s %511[^\n]", &lo, &hi, cur) < 2)
			continue ;
		if (!found && cur[0] == '/' && a >= lo && a < hi)
		{
			snprintf(path, cap, "%s", cur);
			*base = lo;
			found = 1;
			rewind(f);
		}
		else if (found && !strcmp(cur, path))
		{
			*base = lo;
			break ;
		}
	}
	free(line);
	fclose(f);
	return (found);
}

static int	is_dyn(const char *path)
{
	unsigned char	h[18];
	int				fd;
	ssize_t			r;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (1);
	r = read(fd, h, sizeof(h));
	close(fd);
	return (r != 18 || h[16] == 3);
}

static void	lib_frame(t_frame *f, const char *path)
{
	void	*one;
	char	**sy;
	char	*p;
	size_t	n;

	f->ours = 0;
	f->known = 1;
	snprintf(f->loc, sizeof(f->loc), "[%s]", base_name(path));
	one = (void *)f->pc;
	sy = backtrace_symbols(&one, 1);
	if (sy == NULL)
		return ;
	p = strchr(sy[0], '(');
	if (p != NULL && p[1] != '+' && p[1] != ')')
	{
		n = strcspn(p + 1, "+)");
		snprintf(f->fn, sizeof(f->fn), "%.*s", (int)n, p + 1);
	}
	free(sy);
}

static void	parse_a2l(char *line, t_frame *f)
{
	char	*at;
	char	*d;
	char	cwd[512];
	size_t	cl;

	line[strcspn(line, "\n")] = '\0';
	at = strstr(line, " at ");
	if (at == NULL)
		return ;
	*at = '\0';
	snprintf(f->fn, sizeof(f->fn), "%s", line);
	at += 4;
	d = strstr(at, " (discriminator");
	if (d != NULL)
		*d = '\0';
	if (!strncmp(at, "??", 2))
		return ;
	f->known = 1;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		cl = strlen(cwd);
		if (!strncmp(at, cwd, cl) && at[cl] == '/')
			at += cl + 1;
	}
	snprintf(f->loc, sizeof(f->loc), "%s", at);
}

/* Remplit fr[] (nom de fonction + fichier:ligne) pour chaque adresse. */
static int	resolve_stack(t_frame *fr)
{
	char		exe[512];
	char		path[512];
	char		cmd[4096];
	char		line[700];
	uintptr_t	base;
	uintptr_t	adj;
	int			idx[BTMAX];
	int			ne;
	int			i;
	int			k;
	ssize_t		len;
	size_t		cl;
	FILE		*f;

	len = readlink("/proc/self/exe", exe, sizeof(exe) - 1);
	if (len <= 0 || memchr(exe, '\'', (size_t)len) != NULL)
		return (0);
	exe[len] = '\0';
	cl = (size_t)snprintf(cmd, sizeof(cmd), "addr2line -f -C -p -e '%s'", exe);
	ne = 0;
	i = -1;
	while (++i < g_shm->nbt)
	{
		memset(&fr[i], 0, sizeof(t_frame));
		snprintf(fr[i].fn, sizeof(fr[i].fn), "??");
		fr[i].pc = (uintptr_t)g_shm->bt[i];
		adj = (i > 0) ? fr[i].pc - 1 : fr[i].pc;
		base = 0;
		if (map_find(adj, path, sizeof(path), &base) && !strcmp(path, exe))
		{
			fr[i].ours = 1;
			idx[ne++] = i;
			cl += (size_t)snprintf(cmd + cl, sizeof(cmd) - cl, " 0x%lx",
					(unsigned long)(is_dyn(exe) ? adj - base : adj));
		}
		else
			lib_frame(&fr[i], path);
	}
	snprintf(cmd + cl, sizeof(cmd) - cl, " 2>/dev/null");
	f = ne > 0 ? popen(cmd, "r") : NULL;
	k = 0;
	while (f != NULL && k < ne && fgets(line, sizeof(line), f) != NULL)
		parse_a2l(line, &fr[idx[k++]]);
	if (f != NULL)
		pclose(f);
	return (1);
}

static int	is_test_frame(const t_frame *f)
{
	char	file[256];
	char	*c;

	if (!strcmp(f->fn, "run_group") || !strcmp(f->fn, "main"))
		return (1);
	if (!f->ours || !f->known)
		return (0);
	snprintf(file, sizeof(file), "%s", f->loc);
	c = strrchr(file, ':');
	if (c != NULL)
		*c = '\0';
	return (!strcmp(base_name(file), base_name(__FILE__)));
}

static int	same_frame(const t_frame *a, const t_frame *b)
{
	return (!strcmp(a->fn, b->fn) && !strcmp(a->loc, b->loc));
}

static void	show_stack(const char *title)
{
	t_frame	fr[BTMAX];
	char	buf[500];
	int		n;
	int		end;
	int		i;
	int		rep;
	int		shown;
	int		rec;
	int		j;
	int		k;

	n = g_shm->nbt;
	if (n <= 0 || !resolve_stack(fr))
		return ;
	end = n - 1;
	i = 0;
	while (i < n && end == n - 1)
		if (is_test_frame(&fr[i++]))
			end = i - 1;
	detail(title);
	i = 0;
	shown = 0;
	rec = 0;
	while (i <= end && shown < (g_verbose ? 24 : 8))
	{
		if (!fr[i].ours && !is_test_frame(&fr[i]))
		{
			j = i;
			while (j + 1 <= end && !fr[j + 1].ours && !is_test_frame(&fr[j + 1]))
				j++;
			k = j;
			while (k > i && !strcmp(fr[k].fn, "??"))
				k--;
			detail(dd("  #%-2d %-22.22s %s", k,
					!strcmp(fr[k].fn, "??") ? "(fonction libc)" : fr[k].fn, fr[k].loc));
			i = j + 1;
			shown++;
			continue ;
		}
		rep = 1;
		while (i + rep <= end && !is_test_frame(&fr[i])
			&& same_frame(&fr[i], &fr[i + rep]))
			rep++;
		snprintf(buf, sizeof(buf), "  #%-2d %-22.22s %s", i, fr[i].fn,
			fr[i].known ? fr[i].loc : "??:0");
		if (fr[i].ours && (!fr[i].known || strstr(fr[i].loc, ":?") != NULL))
			g_need_g = 1;
		if (rep > 1)
			snprintf(buf + strlen(buf), 60, "   (x%d)", rep);
		if (rep >= 10)
			rec = 1;
		if (is_test_frame(&fr[i]))
			snprintf(buf + strlen(buf), 60, "   <- le test qui a déclenché ça");
		detail(buf);
		i += rep;
		shown++;
	}
	if (i <= end)
		detail("  ... (-v pour toute la pile)");
	if (rec)
		detail("la même fonction s'appelle sans fin : récursion infinie ?");
}

static void	show_fault(int sig)
{
	if ((sig != SIGSEGV && sig != SIGBUS) || g_shm->nbt <= 0)
		return ;
	if (g_shm->fault < 4096)
		detail(dd("adresse fautive : 0x%lx -> NULL déréférencé "
				"(ou un champ d'un pointeur NULL)", (unsigned long)g_shm->fault));
	else
		detail(dd("adresse fautive : 0x%lx", (unsigned long)g_shm->fault));
}

/* ---- rapports ASan / UBSan / LSan : on garde seulement TON code ---------- */

static void	strip_cwd(char *s)
{
	char	cwd[512];
	char	*p;
	size_t	cl;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return ;
	cl = strlen(cwd);
	p = strstr(s, cwd);
	if (p != NULL && p[cl] == '/')
		memmove(p, p + cl + 1, strlen(p + cl + 1) + 1);
}

/* Affiche les premières frames utiles d'une pile ASan commençant en p. */
static void	show_asan(const char *p, const char *title, int max)
{
	const char	*e;
	char		line[500];
	char		*in;
	size_t		len;
	int			n;
	int			started;
	int			is_test;
	int			dup;
	char		prev[500];

	n = 0;
	dup = 0;
	prev[0] = '\0';
	started = 0;
	while (p != NULL && *p != '\0' && n < max)
	{
		e = strchr(p, '\n');
		len = e != NULL ? (size_t)(e - p) : strlen(p);
		if (len >= sizeof(line))
			len = sizeof(line) - 1;
		memcpy(line, p, len);
		line[len] = '\0';
		in = strstr(line, " in ");
		if (strchr(line, '#') != NULL && in != NULL)
		{
			started = 1;
			if (!strstr(line, "libsanitizer") && !strstr(line, "interceptor")
				&& !strstr(line, "__libc") && !strstr(line, "libc.so")
				&& !strstr(line, "_start") && !strstr(line, "ld-linux"))
			{
				strip_cwd(in + 4);
				if (n > 0 && !strcmp(prev, in + 4))
				{
					dup++;
					p = e != NULL ? e + 1 : NULL;
					continue ;
				}
				if (n++ == 0)
					detail(title);
				snprintf(prev, sizeof(prev), "%s", in + 4);
				is_test = strstr(in, base_name(__FILE__)) != NULL;
				detail(dd("  #%d  %.150s%s", n - 1, in + 4,
						is_test ? "   <- le test qui a déclenché ça" : ""));
				if (is_test)
					break ;
			}
		}
		else if (started && len == 0)
			break ;
		p = e != NULL ? e + 1 : NULL;
	}
	if (dup > 0)
		detail(dd("  ... la même ligne se répète %d fois de plus : récursion "
				"infinie ?", dup));
}

static void	print_line_with(const char *err, const char *needle)
{
	const char	*p;
	char		line[300];
	size_t		len;

	p = strstr(err, needle);
	if (p == NULL)
		return ;
	len = strcspn(p, "\n");
	if (len >= sizeof(line))
		len = sizeof(line) - 1;
	memcpy(line, p, len);
	line[len] = '\0';
	detail(line);
}

static void	show_asan_error(const char *err, const char *p)
{
	const char	*q;
	const char	*n;
	int			k;

	n = strstr(p + 25, " on ");
	k = (int)strcspn(p + 25, " (");
	if (n != NULL && n - (p + 25) < 40 && !strncmp(p + 25, "attempting", 10))
		k = (int)(n - (p + 25));
	detail(dd("AddressSanitizer : %.*s", k, p + 25));
	q = strstr(p, "READ of size");
	if (q == NULL)
		q = strstr(p, "WRITE of size");
	if (q != NULL)
		detail(dd("%.*s", (int)(strstr(q, " at ") != NULL
					? strstr(q, " at ") - q : (long)strcspn(q, "\n")), q));
	print_line_with(p, "The signal is caused by");
	print_line_with(p, "Hint: address points to");
	print_line_with(p, "is located");
	show_asan(p, "pile d'appels :", g_verbose ? 12 : 5);
	q = strstr(p, "allocated by thread");
	if (q != NULL)
		show_asan(q, "la zone a été allouée ici :", 4);
	q = strstr(p, "freed by thread");
	if (q != NULL)
		show_asan(q, "et libérée ici :", 4);
	(void)err;
}

static void	pill(t_status st)
{
	const char	*c;
	const char	*t;

	c = g_p_ok;
	t = " OK  ";
	if (st == ST_KO)
		c = g_p_ko;
	if (st == ST_KO)
		t = "FAUX ";
	if (st == ST_CRASH)
		c = g_p_cr;
	if (st == ST_CRASH)
		t = "CRASH";
	if (st == ST_LEAK)
		c = g_p_lk;
	if (st == ST_LEAK)
		t = "FUITE";
	if (st == ST_TIME)
		c = g_p_tm;
	if (st == ST_TIME)
		t = "BOUCL";
	if (st == ST_UB)
		c = g_p_ub;
	if (st == ST_UB)
		t = " UB  ";
	if (st == ST_INFO)
		c = g_p_in;
	if (st == ST_INFO)
		t = "INFO ";
	printf("  %s %s %s", c, t, g_r);
}

static const char	*sigtext(int sig)
{
	if (sig == SIGSEGV)
		return ("SIGSEGV (segmentation fault)");
	if (sig == SIGABRT)
		return ("SIGABRT (double free, assert...)");
	if (sig == SIGBUS)
		return ("SIGBUS (bus error)");
	if (sig == SIGFPE)
		return ("SIGFPE (division par zéro)");
	return ("signal fatal");
}

static void	show_fails(int limit)
{
	int	k;

	k = 0;
	while (k < g_shm->nfail && k < limit)
		detail(g_shm->fail[k++]);
	if (g_shm->ko + g_shm->soft > k)
		detail(dd("... et %d autre(s) cas différent(s)",
				g_shm->ko + g_shm->soft - k));
}

static void	show_problem(t_status st, int sig, const char *err)
{
	const char	*p;
	char		line[300];
	size_t		len;

	if (st == ST_KO)
		show_fails(g_verbose ? MAXF : 3);
	else if (st == ST_INFO)
		show_fails(g_verbose ? MAXF : 2);
	else if (st == ST_TIME)
	{
		detail(dd("aucune réponse après %u s : boucle infinie ?", g_timeout));
		show_stack("bloqué ici :");
		detail(dd("dernier cas : %.150s", g_shm->cur));
	}
	else if (st == ST_CRASH)
	{
		p = err ? strstr(err, "ERROR: AddressSanitizer: ") : NULL;
		if (sig != 0)
		{
			detail(dd("processus tué par %s", sigtext(sig)));
			show_fault(sig);
			show_stack("pile d'appels :");
		}
		else if (p != NULL)
			show_asan_error(err, p);
		else
			detail("sortie anormale du processus");
		detail(dd("dernier cas : %.150s", g_shm->cur));
	}
	else if (st == ST_LEAK)
	{
		p = err ? strstr(err, "SUMMARY: ") : NULL;
		detail(dd("fuite mémoire : %.80s", p ? p + 9 : "voir -fsanitize=address"));
		p = err ? strstr(err, "Direct leak") : NULL;
		if (p == NULL && err != NULL)
			p = strstr(err, "Indirect leak");
		show_asan(p, "allouée ici (jamais free) :", 5);
	}
	else if (st == ST_UB)
	{
		p = err ? strstr(err, "runtime error") : NULL;
		if (p == NULL)
			detail("comportement indéfini");
		else
		{
			while (p > err && p[-1] != '\n')
				p--;
			len = strcspn(p, "\n");
			if (len >= sizeof(line))
				len = sizeof(line) - 1;
			memcpy(line, p, len);
			line[len] = '\0';
			strip_cwd(line);
			detail(line);
			show_asan(p + len, "pile d'appels :", 4);
		}
	}
}

static t_status	judge(int st, const char *err, int *sig)
{
	*sig = 0;
	if (WIFSIGNALED(st) && WTERMSIG(st) == SIGALRM)
		return (ST_TIME);
	if (WIFSIGNALED(st))
	{
		*sig = WTERMSIG(st);
		return (ST_CRASH);
	}
	if (err != NULL && strstr(err, "ERROR: AddressSanitizer") != NULL)
		return (ST_CRASH);
	if (!WIFEXITED(st) || WEXITSTATUS(st) != 0)
		return (ST_CRASH);
	if (err != NULL && strstr(err, "runtime error") != NULL)
		return (ST_UB);
	if (g_shm->leak)
		return (ST_LEAK);
	if (g_shm->ko > 0)
		return (ST_KO);
	if (g_shm->soft > 0)
		return (ST_INFO);
	return (ST_OK);
}

static t_status	run_group(const t_group *g, int *cases)
{
	char		errpath[] = "/tmp/ft_printf_err_XXXXXX";
	char		tmppath[] = "/tmp/ft_printf_out_XXXXXX";
	int			errfd;
	pid_t		pid;
	int			st;
	int			sig;
	double		t0;
	char		*err;
	t_status	res;

	g_shm = mmap(NULL, sizeof(t_shm), PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	errfd = mkstemp(errpath);
	if (g_shm == MAP_FAILED || errfd < 0)
		return (ST_CRASH);
	unlink(errpath);
	fflush(NULL);
	t0 = now_ms();
	pid = fork();
	if (pid == 0)
	{
		g_tmp = mkstemp(tmppath);
		unlink(tmppath);
		dup2(g_tmp, STDOUT_FILENO);
		dup2(errfd, STDERR_FILENO);
		g_soft = g->soft;
		install_handlers();
		alarm(g_timeout);
		g->fn();
		fflush(stdout);
		if (__lsan_do_recoverable_leak_check)
			g_shm->leak = (__lsan_do_recoverable_leak_check() != 0);
		_exit(0);
	}
	waitpid(pid, &st, 0);
	t0 = now_ms() - t0;
	err = read_file(errfd);
	close(errfd);
	res = judge(st, err, &sig);
	if (res == ST_CRASH || res == ST_TIME || res == ST_LEAK || res == ST_UB)
		g_crashes++;
	*cases = g_shm->total;
	pill(res);
	printf(" %s%-10s%s %s%6d cas  %5.2fs%s  %s%s%s\n", g_txt, g->name, g_r, g_d,
		g_shm->total, t0 / 1000.0, g_r, g_d, g->title, g_r);
	show_problem(res, sig, err);
	free(err);
	munmap(g_shm, sizeof(t_shm));
	return (res);
}

/* -------------------------------- main ----------------------------------- */

static void	usage(const char *prog)
{
	printf("Usage : %s [-t secondes] [-f N] [-s graine] [-v] [--no-fuzz] "
		"[motif...]\n", prog);
	printf("  -t N        délai avant timeout de chaque groupe (défaut : 10)\n");
	printf("  -f N        cas aléatoires par groupe de fuzz (défaut : 500)\n");
	printf("  -s N        graine du hasard (pour rejouer une exécution)\n");
	printf("  -v          montre plus de détails sur chaque échec\n");
	printf("  --no-fuzz   désactive les groupes aléatoires (fuzz_*)\n");
	printf("  motif       ne lance que les groupes dont le nom contient motif\n");
	printf("              ex. : %s conv_p fuzz | %s -f 5000 fuzz_s\n", prog,
		prog);
	printf("  groupes     ");
}

static int	name_matches(const char *name, char **filters, int nf)
{
	int	i;

	if (nf == 0)
		return (1);
	i = 0;
	while (i < nf)
	{
		if (strstr(name, filters[i]) != NULL)
			return (1);
		i++;
	}
	return (0);
}

static void	banner(int count)
{
	printf("\n  %s┏━━ FT_PRINTF TESTER ━━ sujet v1.1 ━━━━━━━━━━━━━━━━━━━━┓%s\n",
		g_acc, g_r);
	printf("  %s┃%s %d groupes · conversions cspdiuxX%% · ", g_acc, g_r, count);
	if (g_fuzz > 0)
		printf("fuzz %s%u%s cas · ", g_f_ok, g_fuzz, g_r);
	else
		printf("%sfuzz désactivé%s · ", g_d, g_r);
	printf("timeout %us\n", g_timeout);
	printf("  %s┃%s %sgraine %llu%s (rejouer avec -s %llu)\n", g_acc, g_r, g_d,
		(unsigned long long)g_seed, g_r, (unsigned long long)g_seed);
	printf("  %s┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛%s\n\n",
		g_acc, g_r);
}

static int	parse_args(int ac, char **av, char **filters)
{
	int	i;
	int	nf;

	nf = 0;
	i = 1;
	while (i < ac)
	{
		if (!strcmp(av[i], "-t") && i + 1 < ac)
			g_timeout = (unsigned int)atoi(av[++i]);
		else if (!strcmp(av[i], "-f") && i + 1 < ac)
			g_fuzz = (unsigned int)atoi(av[++i]);
		else if (!strcmp(av[i], "-s") && i + 1 < ac)
			g_seed = (uint64_t)strtoull(av[++i], NULL, 10);
		else if (!strcmp(av[i], "-v"))
			g_verbose = 1;
		else if (!strcmp(av[i], "--no-fuzz"))
			g_fuzz = 0;
		else if (!strcmp(av[i], "-h") || !strcmp(av[i], "--help"))
			return (-1);
		else
			filters[nf++] = av[i];
		i++;
	}
	return (nf);
}

int	main(int ac, char **av)
{
	char		*filters[64];
	int			nf;
	int			i;
	int			counts[7];
	int			cases = 0;
	int			total;
	int			run;
	int			selected;
	double		t0;
	t_status	st;

	memset(counts, 0, sizeof(counts));
	nf = parse_args(ac, av, filters);
	if (nf < 0)
	{
		usage(av[0]);
		i = 0;
		while (i < NGROUPS)
			printf("%s ", g_groups[i++].name);
		printf("\n");
		return (0);
	}
	if (g_timeout == 0)
		g_timeout = 10;
	set_colors();
	if (g_seed == 0)
		g_seed = (uint64_t)time(NULL) ^ ((uint64_t)getpid() << 20)
			^ 0x9E3779B97F4A7C15ULL;
	g_rng = g_seed;
	setvbuf(stdout, NULL, _IOFBF, 1 << 16);
	selected = 0;
	i = 0;
	while (i < NGROUPS)
	{
		if (name_matches(g_groups[i].name, filters, nf)
			&& !(g_fuzz == 0 && !strncmp(g_groups[i].name, "fuzz", 4)))
			selected++;
		i++;
	}
	banner(selected);
	t0 = now_ms();
	total = 0;
	run = 0;
	i = 0;
	while (i < NGROUPS)
	{
		if (name_matches(g_groups[i].name, filters, nf)
			&& !(g_fuzz == 0 && !strncmp(g_groups[i].name, "fuzz", 4)))
		{
			st = run_group(&g_groups[i], &cases);
			counts[st]++;
			total += cases;
			run++;
		}
		i++;
	}
	printf("\n  %s", g_d);
	i = 0;
	while (i++ < 60)
		printf("─");
	printf("%s\n\n  %s%s%d groupe(s) OK%s", g_r, g_f_ok, g_b, counts[ST_OK]
		+ counts[ST_INFO], g_r);
	if (counts[ST_KO] + counts[ST_CRASH] + counts[ST_LEAK] + counts[ST_TIME]
		+ counts[ST_UB] > 0)
		printf("   %s%s%d en échec%s", g_f_ko, g_b, counts[ST_KO]
			+ counts[ST_CRASH] + counts[ST_LEAK] + counts[ST_TIME]
			+ counts[ST_UB], g_r);
	printf("   %s%d cas comparés à printf en %.0f ms%s\n", g_d, total,
		now_ms() - t0, g_r);
	if (run == 0)
		printf("\n  %saucun groupe ne correspond (voir -h)%s\n", g_f_ko, g_r);
	else if (counts[ST_KO] + counts[ST_CRASH] + counts[ST_LEAK]
		+ counts[ST_TIME] + counts[ST_UB] == 0)
		printf("\n  %s%sTout est propre.%s\n", g_f_ok, g_b, g_r);
	else
		printf("\n  %s%sCorrige les rouges d'abord.%s %s-v : plus de détails."
			"%s\n", g_f_ko, g_b, g_r, g_d, g_r);
	if (g_need_g)
		printf("  %sAstuce : recompile avec -g3 pour voir les numéros de ligne."
			"%s\n", g_d, g_r);
	if (g_crashes > 0 && __asan_init == NULL)
		printf("  %sAstuce : ajoute -fsanitize=address -g3 pour localiser à la "
			"ligne près un débordement,\n  un use-after-free, un double free "
			"ou une fuite.%s\n", g_d, g_r);
	printf("\n");
	return (counts[ST_KO] + counts[ST_CRASH] + counts[ST_LEAK]
		+ counts[ST_TIME] + counts[ST_UB] != 0);
}
