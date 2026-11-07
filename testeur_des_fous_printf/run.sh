#!/usr/bin/env bash
# testeur_des_fous_printf (BETA) - testeur strict de ft_printf (projet 42)
# usage : ./run.sh [--valgrind] [--bonus] [--only %d] [--seed N] [options]
set -u

TDF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT="$(dirname "$TDF_DIR")"
MODE="asan"
ONLY=""
SEED=""
BONUS_OPT=""
STRESS=""
TIMEOUT=""
VERBOSE=""
DO_NORM=1

R=$'\033[1;31m'; G=$'\033[1;32m'; Y=$'\033[1;33m'; B=$'\033[1;34m'; C=$'\033[0;36m'
D=$'\033[2m'; N=$'\033[0m'

usage() {
	cat <<EOF
testeur_des_fous_printf (BETA)

usage : ./run.sh [options]
  --valgrind        valgrind (--leak-check=full --show-leak-kinds=all --track-origins=yes)
  --ubsan           ajoute UBSan (dépassements signés, décalages...) en plus d'ASan
  --no-sanitize     aucun sanitizer (traceur malloc + gardes maison seulement)
                    (défaut : ASan seul)
  --bonus           force les tests bonus (KO si un flag n'est pas géré)
  --no-bonus        ignore le bonus (défaut : détection automatique)
  --only CAT        une ou plusieurs catégories séparées par des virgules :
                    c s p d i u x X % mix ret limites write malloc stress bonus
                    makefile nm norme      ex : --only %d   --only s,p,malloc
  --seed N          graine du stress (pour rejouer un échec)
  --stress N        nombre d'appels aléatoires (défaut 10000)
  --timeout S       timeout par test en secondes (défaut 3, 10 sous valgrind)
  --dir CHEMIN      dossier du projet ft_printf (défaut : dossier parent)
  --no-norm         ne lance pas norminette
  -v                détaille tous les KO / avertissements
EOF
	exit "${1:-0}"
}

while [ $# -gt 0 ]; do
	case "$1" in
		--valgrind) MODE="valgrind" ;;
		--asan-only) MODE="asan" ;;
		--ubsan) MODE="asan+ubsan" ;;
		--no-sanitize) MODE="none" ;;
		--bonus) BONUS_OPT="--bonus" ;;
		--no-bonus) BONUS_OPT="--no-bonus" ;;
		--only) shift; ONLY="${1:-}" ;;
		--only=*) ONLY="${1#--only=}" ;;
		--seed) shift; SEED="${1:-}" ;;
		--stress) shift; STRESS="${1:-}" ;;
		--timeout) shift; TIMEOUT="${1:-}" ;;
		--dir) shift; PROJECT="$(cd "${1:-.}" && pwd)" ;;
		--no-norm) DO_NORM=0 ;;
		-v) VERBOSE="-v" ;;
		-h|--help) usage 0 ;;
		*) echo "option inconnue : $1"; usage 2 ;;
	esac
	shift
done

[ -z "$SEED" ] && SEED=$(( (RANDOM << 15 | RANDOM) ))
OS="$(uname -s)"
BUILD="$TDF_DIR/.build"
COPY="$BUILD/project"
STATIC_KO=0
STATIC_WARN=0

want() {   # want CAT : la catégorie est-elle demandée ?
	[ -z "$ONLY" ] && return 0
	local c
	IFS=',' read -ra cs <<< "$ONLY"
	for c in "${cs[@]}"; do [ "$c" = "$1" ] && return 0; done
	return 1
}
line() {   # line OK|KO|~~ titre [détail]
	local col="$G"
	[ "$1" = "KO" ] && col="$R" && STATIC_KO=$((STATIC_KO + 1))
	[ "$1" = "~~" ] && col="$Y" && STATIC_WARN=$((STATIC_WARN + 1))
	[ "$1" = ".." ] && col="$D"
	printf "  %s%-3s%s %s %s\n" "$col" "$1" "$N" "$2" "${3:-}"
}
detail() { printf "      %s\n" "$@"; }

printf "\n%s ═══════════════════════════════════════════════════════════════%s\n" "$B" "$N"
printf "%s   testeur_des_fous_printf  %s(BETA)%s   mode : %s%s%s\n" "$B" "$Y" "$B" "$C" "$MODE" "$N"
printf "%s ═══════════════════════════════════════════════════════════════%s\n" "$B" "$N"
printf "  projet : %s\n  OS : %s   graine du stress : %s\n" "$PROJECT" "$OS" "$SEED"

if [ ! -f "$PROJECT/Makefile" ]; then
	printf "\n  %sKO%s pas de Makefile dans %s\n" "$R" "$N" "$PROJECT"
	printf "     Clone ce testeur DANS ton dépôt ft_printf, ou utilise --dir.\n\n"
	exit 1
fi

# ---------------------------------------------------------------- copie
# Tout est fait sur une copie : ton dépôt n'est jamais modifié.
rm -rf "$BUILD"
mkdir -p "$COPY" "$BUILD/obj" "$BUILD/obj_bonus" "$BUILD/vg"
EXCL=()
case "$TDF_DIR/" in "$PROJECT/"*) EXCL+=(--exclude="./${TDF_DIR#$PROJECT/}");; esac
tar -C "$PROJECT" "${EXCL[@]}" --exclude=./.git -cf - . 2>/dev/null | tar -C "$COPY" -xf -
( cd "$COPY" && make fclean >/dev/null 2>&1 )

TESTER_DIRS='printfTester|testeur_des_fous|francinette|ft_printf_tester|printf-tester|tester'
student_find() {   # fichiers de l'étudiant, hors testeurs
	find "$COPY" "$@" -not -path '*/.git/*' 2>/dev/null | sed "s#^$COPY/##" \
		| grep -Ev "(^|/)($TESTER_DIRS)[^/]*/" | sed "s#^#$COPY/#" | sort
}

# ---------------------------------------------------------------- Makefile
printf "\n%s  ── Contrôles statiques ──%s\n" "$B" "$N"
MK_LOG="$BUILD/make.log"
if ! ( cd "$COPY" && make ) >"$MK_LOG" 2>&1; then
	line KO "Makefile : 'make' échoue"
	tail -n 15 "$MK_LOG" | sed 's/^/      /'
	exit 1
fi
if [ ! -f "$COPY/libftprintf.a" ]; then
	line KO "Makefile : libftprintf.a absent à la racine du dépôt"
	exit 1
fi

if want makefile; then
	MK_PB=()
	MK_WARN=()
	for r in all clean fclean re libftprintf.a; do
		out=$(cd "$COPY" && make -n "$r" 2>&1)
		if [ $? -ne 0 ] || echo "$out" | grep -q "No rule to make target"; then
			MK_PB+=("règle '$r' absente")
		fi
	done
	grep -Eq '^[[:space:]]*NAME[[:space:]]*[:+?]?=[[:space:]]*libftprintf\.a' "$COPY/Makefile" \
		|| MK_PB+=("variable NAME = libftprintf.a introuvable")
	grep -qi 'libtool' "$COPY/Makefile" && MK_PB+=("libtool utilisé (interdit, il faut ar)")
	# flags et compilateur, vus par make -n -B
	CMDS=$(cd "$COPY" && make -n -B all 2>/dev/null)
	COMPILES=$(echo "$CMDS" | grep -E '(^|[[:space:]/])(cc|gcc|clang)[[:space:]].*-c' )
	if [ -z "$COMPILES" ]; then
		MK_WARN+=("impossible de voir les lignes de compilation (make -n -B)")
	else
		NOFLAGS=$(echo "$COMPILES" | grep -v -- '-Wall' ; echo "$COMPILES" | grep -v -- '-Wextra'; echo "$COMPILES" | grep -v -- '-Werror')
		[ -n "$(echo "$NOFLAGS" | tr -d '[:space:]')" ] && MK_PB+=("compilation sans -Wall -Wextra -Werror : $(echo "$NOFLAGS" | head -1 | cut -c1-90)")
		echo "$COMPILES" | grep -Eqv '(^|[[:space:]/])cc[[:space:]]' && MK_WARN+=("le sujet demande 'cc' comme compilateur")
	fi
	echo "$CMDS" | grep -Eq '(^|[[:space:]])ar[[:space:]]' || MK_WARN+=("'ar' introuvable dans make -n (lib créée autrement ?)")
	# relink
	before=$(stat -c '%Y.%y' "$COPY/libftprintf.a" 2>/dev/null || stat -f '%m' "$COPY/libftprintf.a")
	sleep 1
	again=$(cd "$COPY" && make 2>&1)
	after=$(stat -c '%Y.%y' "$COPY/libftprintf.a" 2>/dev/null || stat -f '%m' "$COPY/libftprintf.a")
	if [ "$before" != "$after" ] || echo "$again" | grep -Eq '(^|[[:space:]/])(cc|gcc|clang|ar)[[:space:]]'; then
		MK_PB+=("relink : un 2e 'make' sans modification reconstruit")
	fi
	# clean / fclean / re
	( cd "$COPY" && make clean >/dev/null 2>&1 )
	[ -n "$(student_find -name '*.o')" ] && MK_PB+=("'make clean' laisse des .o")
	( cd "$COPY" && make fclean >/dev/null 2>&1 )
	[ -f "$COPY/libftprintf.a" ] && MK_PB+=("'make fclean' ne supprime pas libftprintf.a")
	( cd "$COPY" && make re >/dev/null 2>&1 )
	[ -f "$COPY/libftprintf.a" ] || MK_PB+=("'make re' ne reconstruit pas libftprintf.a")
	( cd "$COPY" && make >/dev/null 2>&1 )
	if [ ${#MK_PB[@]} -eq 0 ] && [ ${#MK_WARN[@]} -eq 0 ]; then
		line OK "Makefile (règles, NAME, flags, cc, ar, relink, clean/fclean/re)"
	elif [ ${#MK_PB[@]} -eq 0 ]; then
		line "~~" "Makefile"
		for m in "${MK_WARN[@]}"; do detail "${Y}⚠${N} $m"; done
	else
		line KO "Makefile"
		for m in "${MK_PB[@]}"; do detail "${R}✗${N} $m"; done
		for m in "${MK_WARN[@]}"; do detail "${Y}⚠${N} $m"; done
	fi
	HDR=$(student_find -name 'ft_printf.h' | head -1)
	if [ -z "$HDR" ]; then
		line KO "ft_printf.h introuvable"
	elif ! grep -Eq 'int[[:space:]]+ft_printf[[:space:]]*\([[:space:]]*const[[:space:]]+char[[:space:]]*\*' "$HDR"; then
		line KO "ft_printf.h : prototype 'int ft_printf(const char *, ...);' introuvable"
	else
		line OK "ft_printf.h et prototype"
	fi
fi

# ---------------------------------------------------------------- nm
if want nm; then
	UNDERSCORE='s/^//'
	[ "$OS" = "Darwin" ] && UNDERSCORE='s/^_//'
	NMU=$(nm -u "$COPY/libftprintf.a" 2>/dev/null | awk 'NF==2 && $1=="U"{print $2} NF==1 && $1!~/:$/{print $1}' | sed "$UNDERSCORE" | sort -u)
	NMD=$(nm -g "$COPY/libftprintf.a" 2>/dev/null | awk 'NF==3 && $2!="U"{print $3}' | sed "$UNDERSCORE" | sort -u)
	BAD=(); SUSP=()
	for s in $NMU; do
		echo "$NMD" | grep -qx "$s" && continue
		case "$s" in
			write|malloc|free) ;;
			__stack_chk_fail|__stack_chk_guard|_GLOBAL_OFFSET_TABLE_|__gmon_start__|_ITM_*|__cxa_*|stack_chk_fail|stack_chk_guard|_DefaultRuneLocale|dyld_stub_binder) ;;
			memset|memcpy|memmove|bzero) SUSP+=("$s") ;;
			*) BAD+=("$s") ;;
		esac
	done
	if [ ${#BAD[@]} -gt 0 ]; then
		LIBFT_A=$(find "$COPY" -name 'libft*.a' -not -name 'libftprintf.a' 2>/dev/null | head -1)
		LFB=(); [ -n "$LIBFT_A" ] && for s in "${BAD[@]}"; do
			nm -g "$LIBFT_A" 2>/dev/null | awk 'NF==3 && $2!="U"{print $3}' | sed "$UNDERSCORE" | grep -qx "$s" && LFB+=("$s")
		done
		if [ ${#LFB[@]} -gt 0 ]; then
			line KO "libft utilisée mais NON incluse dans libftprintf.a (nm)" "${R}${LFB[*]}${N}"
			detail "la libft est autorisée, mais ton libftprintf.a doit la contenir (make -C libft, puis ar des .o de libft)"
			detail "sinon le correcteur ne pourra pas lier : undefined reference"
		else
			line KO "fonctions externes interdites (nm)" "${R}${BAD[*]}${N}"
			detail "autorisées : write malloc free + ta libft (si elle est dans libftprintf.a)"
		fi
	elif [ ${#SUSP[@]} -gt 0 ]; then
		line "~~" "fonctions externes (nm)" "${Y}${SUSP[*]}${N}"
		detail "pas autorisées par le sujet, mais parfois générées par le compilateur : vérifie ton code"
	else
		line OK "fonctions externes (nm) : seulement write/malloc/free"
	fi
fi

# ---------------------------------------------------------------- objets -> sources
objs_to_srcs() {   # $1 = archive -> chemins des .c correspondants (stdout) ; code 1 si manque
	local o base cands miss=0
	for o in $(ar t "$1"); do
		case "$o" in *.o) ;; *) continue ;; esac
		base="${o%.o}"
		cands=$(student_find -name "$base.c")
		if [ -z "$cands" ]; then
			echo "MISSING:$o"; miss=1
		elif [ "$(echo "$cands" | wc -l)" -gt 1 ]; then
			local od; od=$(dirname "$(student_find -name "$o" | head -1)" 2>/dev/null)
			echo "$cands" | grep -m1 "^$od/" || echo "$cands" | head -1
		else
			echo "$cands"
		fi
	done
	return $miss
}
SRCS_M=$(objs_to_srcs "$COPY/libftprintf.a")
HAS_BONUS=0
if (cd "$COPY" && make -n bonus >/dev/null 2>&1); then
	HAS_BONUS=1
	( cd "$COPY" && make fclean >/dev/null 2>&1 && make bonus >"$BUILD/make_bonus.log" 2>&1 )
	if [ -f "$COPY/libftprintf.a" ]; then
		SRCS_B=$(objs_to_srcs "$COPY/libftprintf.a")
		cp "$COPY/libftprintf.a" "$BUILD/student_bonus.a"
	else
		HAS_BONUS=0
	fi
	( cd "$COPY" && make fclean >/dev/null 2>&1 && make >/dev/null 2>&1 )
fi
cp "$COPY/libftprintf.a" "$BUILD/student.a"

# ---------------------------------------------------------------- norminette
if want norme && [ $DO_NORM -eq 1 ]; then
	if command -v norminette >/dev/null 2>&1; then
		NFILES=$( { echo "$SRCS_M"; [ $HAS_BONUS -eq 1 ] && echo "$SRCS_B"; student_find -name '*.h'; } | grep -v '^MISSING:' | sort -u)
		NOUT=$(cd "$COPY" && norminette $NFILES 2>&1)
		NERR=$(echo "$NOUT" | grep -c ': Error!')
		NTOT=$(echo "$NFILES" | grep -c .)
		if [ "$NERR" -eq 0 ]; then
			line OK "norminette ($NTOT fichiers)"
		else
			line KO "norminette : $NERR fichier(s) sur $NTOT en erreur"
			echo "$NOUT" | grep -E 'Error!|Error:' | head -n 12 | sed "s#$COPY/##" | sed 's/^/      /'
		fi
	else
		line ".." "norminette non installée : vérification de la norme ignorée"
	fi
fi

# ---------------------------------------------------------------- compilation instrumentée
case "$MODE" in
	asan+ubsan) SAN="-fsanitize=address,undefined -fno-sanitize-recover=all" ;;
	asan) SAN="-fsanitize=address" ;;
	none) SAN="-DTDF_GUARD=1" ;;
	valgrind)
		SAN="-DTDF_VALGRIND=1"
		command -v valgrind >/dev/null || { printf "  %sKO%s valgrind n'est pas installé\n" "$R" "$N"; exit 1; } ;;
esac
CFLAGS_COMMON="-g -O0 -fno-omit-frame-pointer"
# repli automatique si les bibliothèques des sanitizers manquent sur cette machine
san_ok() { echo 'int main(void){return 0;}' | LC_ALL=C cc $1 -x c - -o "$BUILD/.san_probe" >/dev/null 2>&1; }
if [ "$MODE" = "asan+ubsan" ] && ! san_ok "$SAN"; then
	if san_ok "-fsanitize=address"; then
		line "~~" "UBSan indisponible sur cette machine (libubsan absente) : repli sur ASan seul"
		MODE="asan"; SAN="-fsanitize=address"
	else
		line "~~" "sanitizers indisponibles sur cette machine : repli sur le mode sans sanitizer"
		MODE="none"; SAN="-DTDF_GUARD=1"
	fi
elif [ "$MODE" = "asan" ] && ! san_ok "$SAN"; then
	line "~~" "ASan indisponible sur cette machine : repli sur le mode sans sanitizer"
	MODE="none"; SAN="-DTDF_GUARD=1"
fi
INCS=$(student_find -name '*.h' -exec dirname {} \; | sort -u | sed 's/^/-I/')
LINK_WRAP="-Wl,--wrap=malloc,--wrap=free,--wrap=calloc,--wrap=realloc"
TRACK="-DTDF_TRACK=1"
if [ "$OS" = "Darwin" ]; then LINK_WRAP=""; TRACK="-DTDF_TRACK=0"; fi
CC_T=cc
command -v cc >/dev/null || CC_T=gcc

build_lib() {   # build_lib "<srcs>" objdir archive studentlib -> 0 instrumentée, 1 repli
	local srcs="$1" od="$2" out="$3" fallback="$4" i=0 s
	if echo "$srcs" | grep -q '^MISSING:'; then
		cp "$fallback" "$out"; return 1
	fi
	: > "$od.list"
	for s in $srcs; do
		i=$((i + 1))
		if ! $CC_T $CFLAGS_COMMON $SAN -w $INCS -c "$s" -o "$od/$i.o" 2>>"$BUILD/build.log"; then
			cp "$fallback" "$out"; return 1
		fi
		echo "$s" >> "$od.list"
	done
	ar rcs "$out" "$od"/*.o
	return 0
}
if ! build_lib "$SRCS_M" "$BUILD/obj" "$BUILD/lib_m.a" "$BUILD/student.a"; then
	line "~~" "lib non instrumentée" "(sources introuvables pour : $(echo "$SRCS_M" | grep MISSING | tr '\n' ' '))"
	detail "les erreurs mémoire seront moins bien localisées"
fi
echo "$SRCS_M" | grep -v '^MISSING:' > "$BUILD/srcs.txt"
RUNNER_SRC="$TDF_DIR/src"
build_runner() {   # build_runner lib exe
	LC_ALL=C $CC_T $CFLAGS_COMMON $SAN $TRACK -DTDF_MODE="\"$MODE\"" -fno-builtin -Wall -Wextra \
		-I"$RUNNER_SRC" "$RUNNER_SRC"/*.c "$1" $LINK_WRAP -ldl -o "$2" 2>"$BUILD/runner_build.log"
}
if ! build_runner "$BUILD/lib_m.a" "$BUILD/tdf_runner"; then
	printf "  %sKO%s impossible de lier le testeur avec ta lib :\n" "$R" "$N"
	grep -v '^$' "$BUILD/runner_build.log" | cut -c1-200 | head -n 14 | sed 's/^/      /'
	exit 1
fi
if [ $HAS_BONUS -eq 1 ]; then
	build_lib "$SRCS_B" "$BUILD/obj_bonus" "$BUILD/lib_b.a" "$BUILD/student_bonus.a" || true
	echo "$SRCS_B" | grep -v '^MISSING:' >> "$BUILD/srcs.txt"
	build_runner "$BUILD/lib_b.a" "$BUILD/tdf_runner_bonus" || HAS_BONUS=0
fi
sort -u -o "$BUILD/srcs.txt" "$BUILD/srcs.txt"

# ---------------------------------------------------------------- exécution
export ASAN_OPTIONS="check_printf=0:detect_leaks=1:halt_on_error=1:symbolize=1:allocator_may_return_null=1:detect_odr_violation=0:print_summary=1"
export UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=1"
export LSAN_OPTIONS="print_suppressions=0"
ARGS=(--seed "$SEED" --project "$COPY" --srclist "$BUILD/srcs.txt" --vglogdir "$BUILD/vg")
[ -n "$STRESS" ] && ARGS+=(--stress "$STRESS")
[ -n "$TIMEOUT" ] && ARGS+=(--timeout "$TIMEOUT")
[ -n "$VERBOSE" ] && ARGS+=("$VERBOSE")
DYN_ONLY=""
if [ -n "$ONLY" ]; then
	DYN_ONLY=$(echo "$ONLY" | tr ',' '\n' | grep -Ev '^(makefile|nm|norme)$' | paste -sd, -)
fi

run_runner() {   # run_runner exe args...
	local exe="$1"; shift
	if [ "$MODE" = "valgrind" ]; then
		valgrind -q --leak-check=full --show-leak-kinds=all --track-origins=yes \
			--errors-for-leak-kinds=definite,indirect,possible --error-exitcode=42 \
			--log-file="$BUILD/vg/vg.%p" "$exe" --exe "$exe" "$@"
	else
		"$exe" "$@"
	fi
}

RC=0
if [ -z "$ONLY" ] || [ -n "$DYN_ONLY" ]; then
	printf "\n%s  ── Tests dynamiques ──%s  %s(chaque test dans un processus fils, comparé au vrai printf)%s\n" "$B" "$N" "$D" "$N"
	M_ARGS=("${ARGS[@]}")
	[ -n "$DYN_ONLY" ] && M_ARGS+=(--only "$DYN_ONLY")
	if [ $HAS_BONUS -eq 1 ]; then
		M_ARGS+=(--no-bonus)
	elif [ -n "$BONUS_OPT" ]; then
		M_ARGS+=("$BONUS_OPT")
	fi
	run_runner "$BUILD/tdf_runner" "${M_ARGS[@]}" || RC=1
	if [ $HAS_BONUS -eq 1 ] && [ "$BONUS_OPT" != "--no-bonus" ] && { [ -z "$DYN_ONLY" ] || echo ",$DYN_ONLY," | grep -q ',bonus,'; }; then
		printf "  %s(bonus : lib construite par 'make bonus')%s\n" "$D" "$N"
		B_ARGS=("${ARGS[@]}" --only bonus)
		[ -n "$BONUS_OPT" ] && B_ARGS+=("$BONUS_OPT")
		run_runner "$BUILD/tdf_runner_bonus" "${B_ARGS[@]}" || RC=1
	fi
fi

# ---------------------------------------------------------------- bilan
printf "\n"
if [ $STATIC_KO -gt 0 ]; then
	printf "  %sContrôles statiques : %d KO%s" "$R" "$STATIC_KO" "$N"
else
	printf "  %sContrôles statiques : OK%s" "$G" "$N"
fi
[ $STATIC_WARN -gt 0 ] && printf "  %s(%d avertissement(s))%s" "$Y" "$STATIC_WARN" "$N"
printf "\n"
if want stress; then
	printf "  %sPour rejouer le stress : ./run.sh --only stress --seed %s%s\n" "$D" "$SEED" "$N"
fi
printf "  %stestable aussi avec : --valgrind | --ubsan | --no-sanitize%s\n\n" "$D" "$N"
[ $STATIC_KO -gt 0 ] && RC=1
exit $RC
