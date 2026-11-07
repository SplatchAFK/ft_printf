# testeur_des_fous_printf — BETA

Testeur **strict** de `ft_printf` (projet 42, sujet v1.1). Chaque appel est comparé au
**vrai `printf`**, octet par octet (les `\0` compris), avec la valeur de retour. Chaque test
tourne dans un processus fils avec timeout : un crash ou une boucle infinie donne un KO sur
ce test, pas sur toute la série.

> ⚠️ **BETA** : les résultats peuvent encore évoluer. Un KO doit toujours être vérifié à la main.

## Installation

```sh
cd ton_depot_ft_printf
git clone <ce dépôt> testeur_des_fous_printf     # ou copie le dossier
cd testeur_des_fous_printf
./run.sh
```

Prérequis : `cc`/`gcc`, `make`, `ar`, `nm`, `addr2line`. Facultatifs : `norminette`, `valgrind`.
**Ton dépôt n'est jamais modifié** : le projet est copié dans `testeur_des_fous_printf/.build/`
(pense à ajouter ce dossier à ton `.gitignore`, ou ne pousse pas le testeur).

## Options

```
./run.sh [--valgrind] [--bonus] [--only %d] [--seed N]

  (défaut)        ASan + UBSan (-fsanitize=address,undefined -fno-omit-frame-pointer -g)
  --valgrind      valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
  --ubsan         ajoute UBSan à ASan (optionnel, défaut = ASan seul)
  --no-sanitize   sans sanitizer (traceur malloc + gardes maison seulement)
  --bonus         force les tests bonus (KO si un flag n'est pas géré)
  --no-bonus      ignore le bonus (défaut : détection automatique)
  --only CAT      c s p d i u x X % mix ret limites write malloc stress bonus
                  makefile nm norme      (plusieurs : --only s,p,malloc)
  --seed N        rejoue exactement le même stress aléatoire
  --stress N      nombre d'appels aléatoires (défaut 10000)
  --timeout S     timeout par test (défaut 3 s, 10 s sous valgrind)
  --dir CHEMIN    dossier du projet (défaut : dossier parent)
  --no-norm       saute norminette
  -v              détaille tous les KO et avertissements
```

## Lire les résultats

Une ligne par catégorie :

```
  OK  %c  caractères                         27/27
  KO  %p  pointeurs                          22/26   4 KO
      ✗ KO   tdf_tests_mandatory.c:107  ft_printf("%p", (void *)1)
          CRASH (le programme s'est arrêté pendant ft_printf, code 1)
          ASan : heap-buffer-overflow (WRITE of size 1)
          → ligne du bug : ft_printf_alloc.c:59  (dans ft_ultoa_base)
          attendu : "0x1"  ret=3
          obtenu  : ""  ret=(aucun)
  ~~  cas limites                            15/15   1 avert.
```

- **OK** (vert) : tout est identique au vrai printf.
- **KO** (rouge) : vrai bug. Le détail donne le fichier et la ligne **du test**, le format et
  les arguments, la sortie attendue et obtenue (`\0` en violet, non imprimables en `\xNN`),
  les deux retours et, en cas d'erreur mémoire, **la ligne du bug dans tes sources**.
- **~~ / ⚠ AVERT** (jaune) : comportement indéfini (`%k`, `%` final, `ft_printf(NULL)`,
  combinaisons de flags UB) ou allocation plus grande que nécessaire. Ce n'est pas un échec.
- Le stress affiche sa **graine** : `./run.sh --only stress --seed N` rejoue le même tirage.
  Un KO du stress ou du bonus est repéré par son numéro de cas (`stress:123`).

Le code de sortie vaut 0 s'il n'y a aucun KO, 1 sinon.

## Ce qui est testé

| Catégorie | Contenu |
|---|---|
| `c` | lettres, `\0` (seul, répété, au milieu), 127/128/200/255, négatifs, `'a'+256` |
| `s` | normale, vide, `NULL` → `(null)`, `\0` au milieu, UTF-8, octets ≥ 0x80, 100 000 caractères, `%d` dans la chaîne |
| `p` | pile, tas, globale, fonction, littéral, `NULL` (`(nil)` glibc / `0x0` macOS, pris du vrai printf), adresses hautes, `-1` |
| `d` `i` | 0, ±1, 9/10/99/100, 9 et 10 chiffres, INT_MAX, INT_MIN, INT_MIN+1, plus de 6 arguments (passés sur la pile) |
| `u` | 0, -1 (=UINT_MAX), 3000000000, 2147483648… |
| `x` `X` | 0, a–f, -1, UINT_MAX, casse, mélange `%x%X` |
| `%` | seul, doublé, collé aux conversions, en fin de format |
| `mix` | toutes les conversions, format vide, sans conversion, format de 20 000 caractères, 20 arguments |
| `ret` | valeur de retour avec `\0`, sorties de plus de 300 Ko |
| `limites` | `ft_printf(NULL)` (KO seulement si boucle infinie), `%` final (KO si lecture après le `\0` du format, sinon avertissement), conversions inconnues (avertissement) |
| `write` | sortie fermée et `/dev/full` : retour `-1` attendu (comme printf), sans crash ni boucle |
| `malloc` | chaque `malloc` de ta lib échoue tour à tour : pas de crash, pas de fuite, retour `-1` |
| `stress` | 10 000 appels aléatoires (texte, `%%`, 0 à 4 conversions, valeurs limites), graine affichée |
| `bonus` | détecté automatiquement : `- 0 . # + espace`, largeur, précision. 44 544 combinaisons comparées au vrai printf ; combinaisons UB = avertissement |
| `makefile` | règles all/clean/fclean/re/NAME, `cc`, `-Wall -Wextra -Werror`, `ar` (pas libtool), pas de relink, clean et fclean efficaces |
| `nm` | symboles externes : seuls `write`, `malloc` et `free` sont autorisés (printf, vprintf, strlen… = KO) |
| `norme` | norminette sur les sources de la lib et les en-têtes |

### Mémoire

- Ta lib est **recompilée** depuis ses sources (repérées grâce à `ar t libftprintf.a`) avec les
  flags du mode choisi, pour que les erreurs pointent vers **ta ligne**.
- **Traceur d'allocations** (`-Wl,--wrap=malloc,free`) : seuls les appels faits *par ta lib*
  sont suivis. Pointeurs **brouillés** : la table du traceur ne cache aucune fuite à
  LeakSanitizer ni à valgrind. Tout bloc encore vivant au retour de `ft_printf` est une fuite,
  **même s'il est gardé dans une variable globale/statique**. L'endroit de l'allocation est affiché.
- Taille des allocations : trop petite → débordement (ASan, valgrind, ou garde maison en
  `--no-sanitize`) = KO ; trop grande (octets jamais écrits) = avertissement.
- Double free, free invalide, use-after-free, lectures non initialisées (`--valgrind`), UB
  comme le débordement signé ou les décalages (UBSan).
- En `--no-sanitize`, chaque bloc alloué par ta lib est placé sur ses propres pages, collé à
  une page protégée, et ses pages sont verrouillées au `free` (à la manière d'Electric Fence) :
  débordement ou use-after-free = segfault immédiat, et le testeur affiche **la ligne fautive**.

## Limites connues

- **Linux** : testé sous Ubuntu 24.04 (glibc 2.39, gcc 13). **macOS** : prévu mais pas
  vérifié. Le traceur malloc (et donc l'injection d'échec et la détection des fuites cachées)
  y est désactivé (pas de `--wrap`), et LeakSanitizer y est souvent indisponible.
- Les sorties attendues viennent du `printf` **de ta machine** : `%p` de NULL, `%s` de NULL,
  le `%` final… suivent l'OS (Linux `(nil)`, macOS `0x0`).
- Lecture non initialisée : détectée de façon fiable seulement avec `--valgrind` (sans valgrind,
  seulement si la valeur fausse change la sortie).
- Un comportement indéfini qui ne change pas la sortie (ex. `n = -n` sur INT_MIN, qui
  « tombe juste » à l'exécution) n'est visible **qu'avec UBSan**, donc dans le mode par défaut.
- Les petits débordements (moins de 16 octets) en `--no-sanitize` sont vus au `free` : le
  testeur indique alors où le bloc a été alloué, mais pas la ligne de l'écriture.
- Un `va_arg` mal typé (`long` au lieu de `int`) n'est visible que si les bits hauts du registre
  ou de la pile diffèrent : tests avec valeurs négatives et plus de 6 arguments, sans garantie.
- `va_end` oublié, `va_copy` mal utilisé : non détectables en pratique.
- Écriture `write` octet par octet ou bufferisée : non jugée (le sujet interdit seulement de
  reproduire le buffer du vrai printf).
- Le contrôle du Makefile repose sur `make -n` : un Makefile très exotique peut donner un
  avertissement « impossible de vérifier ».

## Lecture des erreurs (regroupées par cause)

Les KO ne sont plus listés un par un : ils sont **regroupés par cause** (ex. « fuite mémoire — libft/ft_itoa.c:37 »).
Pour chaque cause : le nombre de cas, 2 exemples (ligne du test + attendu/obtenu), les autres appels touchés et un **conseil**.
Une cause déjà détaillée dans une catégorie précédente est rappelée en une ligne (« même cause que plus haut »).
En fin de run, **« À corriger, par ordre d'impact »** classe les causes de la plus fréquente à la moins fréquente.
`-v` désactive le regroupement et affiche chaque test en détail.
