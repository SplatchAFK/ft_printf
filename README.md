*This activity has been created as part of the 42 curriculum by romasant.*

# ft_printf

## Description

Reproduire la fonction `printf` de la libc : `ft_printf` écrit sur la sortie standard selon un format, et renvoie le nombre de caractères écrits (ou `-1` si `write` échoue).

> [!NOTE]
> Conversions gérées : `%c` `%s` `%p` `%d` `%i` `%u` `%x` `%X` `%%`. Prototype : `int ft_printf(const char *, ...);`

## Instructions

```bash
make
```

Génère `libftprintf.a` à la racine (via `ar`). Autres règles : `make clean`, `make fclean`, `make re`.

Pour l'utiliser dans un programme :

```bash
cc main.c libftprintf.a -o prog
```

```c
#include "ft_printf.h"

int	len = ft_printf("Hello %s, tu as %d ans\n", "42", 18);
```

## Conversions

| Format | Affiche |
|--------|---------|
| `%c` | un caractère |
| `%s` | une chaîne (`(null)` si `NULL`) |
| `%p` | un pointeur `void *` en hexadécimal (`(nil)` si `NULL`) |
| `%d` `%i` | un entier signé en base 10 |
| `%u` | un entier non signé en base 10 |
| `%x` `%X` | un entier non signé en hexadécimal (minuscules / majuscules) |
| `%%` | un signe `%` |

## Algorithme et structure de données

**Pas de structure de données** : conformément au sujet, aucun buffer n'est géré. Chaque morceau de sortie est écrit directement avec `write`, et seule la `va_list` (`va_start`, `va_arg`, `va_end`) permet de parcourir les arguments variadiques.

**Principe** : le format est parcouru caractère par caractère.
- Un caractère ordinaire est écrit tel quel.
- Un `%` est suivi d'une lettre de conversion : elle est identifiée, puis la fonction d'affichage correspondante est appelée avec l'argument suivant de la `va_list`.
- Les nombres (`%d`, `%u`, `%x`...) sont affichés **récursivement** : on affiche d'abord `n / base`, puis le dernier chiffre. Cela évite tout tableau intermédiaire. `INT_MIN` est traité à part (on ne peut pas faire `-n`).

**Valeur de retour** : chaque fonction d'affichage renvoie le nombre d'octets écrits, ou `-1` si `write` échoue. `ft_printf` additionne ces valeurs et s'arrête immédiatement avec `-1` au premier échec (en appelant `va_end` avant de sortir), comme le fait le vrai `printf`.

**Choix** : un découpage en petites fonctions (une par conversion) plus un point central qui dispatche, pour que le code reste lisible, conforme à la Norme et facile à étendre.

## Resources

- Pages de manuel (`man 3 printf`, `man 3 stdarg`, `man 2 write`, `man ar`), documentation de `make`.
- Wikipédia, Stack Overflow, GeeksforGeeks.

### Utilisation de l'IA

> [!WARNING]
> Le code des fonctions a été entièrement écrit par moi.

J'ai utilisé Claude comme tuteur, pour comprendre des concepts (fonctions variadiques, retour de `write`, étapes de compilation) et relire mon code en identifiant mes bugs, pas pour obtenir des réponses toutes faites.

<!-- version=v2 -->
