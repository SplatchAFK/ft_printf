# Le testeur des fous · Version printf

> **Version 1.0.1** · [historique des mises à jour](CHANGELOG.md) · `bash testeur_des_fous_printf/run.sh --version`

> Créé et vibe-codé par **Splatch** (login 42 : `romasant`). Moteur de tests et lore : Splatch.
> Pour les étudiants du cursus **42 Next**, à Paris et dans les autres campus.

Un testeur de `ft_printf` qui se joue comme un jeu vidéo, dans le terminal. Chaque appel de ton `ft_printf` est comparé au vrai `printf`, octet par octet, avec la valeur de retour. Ensuite, ton chevalier Marvin brise les chaînes qui retiennent Moulinette, puis affronte **ft_printf** lui-même, qui vérifie tout.

C'est la suite du [testeur des fous version Libft](https://github.com/SplatchAFK/testeur_des_fous_libft) : ta sauvegarde, ton chevalier et tes objets te suivent d'un testeur à l'autre.

## Vibe-coding et règle IA de 42

Ce projet a été **vibe-codé**, c'est-à-dire écrit en dialoguant avec une IA. C'est un outil à côté du projet, pas un rendu 42. Il ne contient aucune `ft_printf`, ne donne aucune solution (ni au projet, ni à l'entraînement) et n'écrit rien à ta place : il casse ton code et te dit où regarder.

## Installation

Clone ce dépôt **dans** ton dépôt ft_printf (le nom du dossier n'a pas d'importance) :

```bash
git clone https://github.com/SplatchAFK/testeur_des_fous_printf.git testeur_des_fous_printf
```

Puis lance :

```bash
bash testeur_des_fous_printf/run.sh
```

Il te faut `cc`, `make`, `ar`, `nm`, `bash` et un terminal d'au moins 80 x 24 (les cinématiques sont prévues pour 120 x 36 : si ton terminal est plus petit, le testeur s'ouvre tout seul dans une nouvelle fenêtre de 140 x 42, sous WSL, Linux ou macOS ; pour rester dans le terminal actuel : `TDF_FENETRE=1 bash testeur_des_fous_printf/run.sh`). Facultatifs : `norminette`, `valgrind`. Testé sous Linux (Ubuntu 24.04, WSL) ; macOS prévu mais pas vérifié.

**Ne rends jamais ce dossier.** Ajoute-le à ton `.gitignore` :

```bash
echo "testeur_des_fous_printf/" >> .gitignore
```

Ton dépôt n'est jamais modifié : les tests tournent sur une copie, dans `testeur_des_fous_printf/.build/`.

## Le premier lancement

Avant toute chose, le testeur te demande si tu veux **l'histoire** (cinématiques et lore) ou si tu préfères aller droit au but. Ce choix vaut pour tous les testeurs des fous et se change dans **Paramètres**. Pour les tests seuls, sans jeu du tout : `--no-lord`.

Ensuite vient la **connexion** : si tu as déjà joué au testeur libft sur ce poste, ta sauvegarde est reconnue toute seule. Sinon, tu peux charger un `save.txt` copié, ou continuer sans.

## Corriger, relancer : la boucle rapide

Le tout premier lancement montre tout : titre, connexion, histoire, chaînes et big boss. Ensuite, le testeur va droit aux résultats :

- **les cinématiques ne reviennent que pour un événement** : une chaîne brisée pour la première fois, ft_printf vaincu, un boss du DLC battu ;
- s'il reste des KO, les **Résultats détaillés** s'ouvrent tout seuls, sur le premier KO ;
- **`r` relance les tests** (depuis les résultats ou le menu) : tu corriges, tu appuies sur `r`, environ 2 à 3 secondes plus tard tu as la réponse ;
- l'audit du Makefile (avec son test de relink), le contrôle rapide de ta `libft/`, la norminette et le testeur lui-même sont gardés en cache dans `.build_jeu/cache` tant que les fichiers concernés ne changent pas. Pour tout refaire : `TDF_CACHE=0 bash testeur_des_fous_printf/run.sh`.

Toutes les cinématiques restent visibles dans **Boss et cinématiques**.

## L'histoire

À peine libérée de la Tour de la Libft, Moulinette, la chatte grise de 42, est capturée par **ft_printf** et enchaînée dans la **Machine de la Norminette**.

- **9 chaînes**, une par conversion du sujet : `%c` le Caractère solitaire, `%s` la Longue Chaîne, `%p` l'Adresse perdue, `%d` le Signé, `%i` le Jumeau, `%u` le Sans-Signe, `%x` le Petit Hexa, `%X` le Grand Hexa, `%%` le Pourcent. Marvin les attaque toutes en même temps : une chaîne saute seulement si **tous** ses tests passent, sans aucune fuite.
- **ft_printf**, le big boss, vérifie tout en 7 phases : la Norme ; Makefile, en-tête et README ; fonctions interdites ; mélanges et valeur de retour ; cas limites et `write` en échec ; `malloc` qui échoue ; le Dévoreur (10 000 appels aléatoires).
- La victoire se fête au restaurant, avec Moulinette et deux chocolats chauds. Une **fin secrète** attend ceux qui ont aussi validé la Libft.

Une barre en haut de l'écran montre où tu en es : mini-boss, puis big boss. Une touche passe n'importe quelle cinématique.

Les cinématiques sont de vrais dessins (dossier `assets/`), pensés pour la fenêtre de 140 x 42 que le testeur ouvre. Dans un terminal plus grand, l'image est agrandie sans déformation (x2, x3...).

## Les privilèges des Vétérans de la Libft

Si ta sauvegarde montre une libft validée (43/43 en local et les 3 boss), tu deviens **Vétéran de la Libft** :

- le chapitre « Précédemment, dans la Libft » et la fin secrète ;
- ton chevalier, tes objets, titres, Octets et XP ;
- des exclusifs introuvables autrement : cape et casque du Vétéran, le familier Moulinette, le thème doré, le titre « Vétéran de la Libft » ;
- le DLC débloqué tout de suite et 300 Octets offerts.

Sans testeur libft, tout le jeu et tous les tests restent disponibles.

## Le menu

| Bouton | Ce qu'il fait |
|---|---|
| Relancer les tests (r) | relance les tests ; les cinématiques seulement pour un événement |
| DLC | le monde des flags (6 boss) et l'entraînement (16 niveaux) |
| Boss et cinématiques | revoir chaque cinématique débloquée |
| Boutique / Avatar | objets, thèmes, titres, garde-robe |
| Profil et succès | bannière, photo, bio, vitrine de badges, 25 succès |
| Badges et holygraph 42 Next | les 12 badges et les 28 projets du Common Core |
| Sauvegarde et connexion | exporter ta sauvegarde, ou te reconnecter avec un fichier |
| Nous aider | créer ta trace pour améliorer le testeur |
| Résultats détaillés | chaque contrôle et chaque catégorie, avec le détail des erreurs |
| Paramètres | aide, histoire, animations, langue, touches, thème, tests |

## Ce qui est testé

| Catégorie | Contenu |
|---|---|
| `c` `s` `p` `d` `i` `u` `x` `X` `%` | chaque conversion et ses cas limites (`\0`, `NULL`, `INT_MIN`, `UINT_MAX`, adresses hautes...), plus de 6 arguments |
| `mix` | toutes les conversions ensemble, format vide, format de 20 000 caractères |
| `ret` | valeur de retour, `\0` dans la sortie, sorties de plus de 300 Ko |
| `limites` | `ft_printf(NULL)`, `%` final, conversion inconnue : comportements indéfinis, donc avertissements |
| `write` | sortie fermée et `/dev/full` : `-1` attendu, sans crash ni boucle |
| `malloc` | chaque `malloc` de ta lib échoue tour à tour : pas de crash, pas de fuite, `-1` |
| `stress` | 10 000 appels aléatoires, graine affichée pour rejouer |
| statique | Makefile (règles, `cc`, flags, `ar`, relink, libft compilée par son propre Makefile), `ft_printf.h`, `README.md` (sujet, chapitre 5), fonctions externes (`nm`), norminette et sa version |
| `libft/` | contrôle rapide des 43 fonctions de ta libft (preuve locale du holygraph) |

Ta lib est recompilée avec **AddressSanitizer et UndefinedBehaviorSanitizer** : une erreur mémoire ou un comportement indéfini pointe vers **ta ligne**. Un traceur de `malloc` voit les fuites, même gardées dans une variable statique. Le sujet v1.1 n'a pas de bonus : les flags ne sont testés que dans le DLC.

## Lire les résultats

Code couleur : **vert** OK, **rouge** KO, **jaune** avertissement (comportement indéfini), **gris** non testé ou verrouillé. Les conseils s'affichent dans la couleur normale de ton terminal.

Trois **niveaux d'aide** (Paramètres, ou `--aide`) :

- **0** : le cas fautif seulement (format, attendu, obtenu) ;
- **1** : plus la ligne de ton code ;
- **2** : plus un conseil général.

Jamais de code à ta place.

## Le DLC

- **Le monde des flags** : 6 boss (`-` le Gaucher, `0` le Remplisseur, la largeur le Géant, `.` le Précis, `#` l'Alterné, `+` et espace le Signataire), chacun testé seul contre le vrai `printf`, avec ta règle `make bonus` si elle existe. Il s'ouvre après ft_printf, ou tout de suite pour les Vétérans.
- **L'entraînement** : 16 niveaux, dans l'ordre. Les premiers reprennent les briques du projet (`write` et son retour, `NULL`, `INT_MIN`, bases, pointeurs, lecture du format), les suivants les notions que le sujet ne montre pas (`va_copy`, passer une `va_list` comme `vprintf`, `malloc` de taille exacte, descripteurs, `dprintf`). Tu écris ta fonction dans `entrainement/niveau_XX/`, puis tu appuies sur `t`. Norme obligatoire, aucune solution donnée.

## Sauvegarde et connexion

Ta progression est dans `~/.testeur_des_fous/save.txt`, commune à tous les testeurs des fous : un profil partagé (Octets, objets, titres, avatar, badges, images) et une section par jeu. Tu peux supprimer le dossier du testeur, ta progression reste.

Pour la garder ailleurs : **Sauvegarde et connexion**, puis **Exporter**. Pour te reconnecter : copie-la en `~/.testeur_des_fous/save.txt`, ou **Charger un fichier**, ou `bash testeur_des_fous_printf/run.sh --reco ~/ma_sauvegarde.txt`. Une sauvegarde modifiée à la main, ou d'un autre login, est refusée. Ce contrôle arrête les modifications maladroites, pas les tricheurs décidés : le code est public et tout tourne sur ta machine. Il n'y a pas de classement, tricher ne sert à rien.

## Options de la ligne de commande

| Commande | Effet |
|---|---|
| `bash testeur_des_fous_printf/run.sh` | le jeu complet |
| `... --no-lord` (ou `--lore-off`) | les tests seuls, en texte ; la progression est enregistrée |
| `... --debug` | debug avancé : tout le détail (fuites, mémoire, UB), puis un passage sous valgrind |
| `... --aide 0\|1\|2` | niveau d'aide |
| `... --only s,p,malloc` | seulement ces catégories (en texte) |
| `... --seed N` | rejouer le même stress aléatoire |
| `... --bonus` | tester aussi les flags (DLC) |
| `... --valgrind`, `--asan-only`, `--no-sanitize` | changer d'outil mémoire |
| `... --reco FICHIER` | te connecter avec une sauvegarde copiée |
| `... --version` | le numéro de version |

## Limites connues

- Les sorties attendues viennent du `printf` de **ta machine** : `%p` de `NULL` donne `(nil)` sous Linux et `0x0` sous macOS.
- Un `va_arg` mal typé (`long` au lieu de `int`) n'est visible que si les bits hauts diffèrent : pas toujours détectable.
- Une lecture non initialisée n'est sûre qu'avec `--valgrind` (ou `--debug`).
- macOS : le traceur de `malloc` est désactivé (pas de `--wrap`).

## Nous aider

Que tes tests passent ou non, ta trace aide à trouver les faux positifs et les cas oubliés. Dans le menu **Nous aider**, la touche `t` crée `~/testeur_des_fous_trace_printf.txt` (ni code, ni login). Envoie-la en message privé à **@splatch_afk** sur le serveur 42 Next, avec tes conseils et suggestions : tout est pris. Le testeur reste entièrement hors ligne.

## Crédits et licence

- Créé et vibe-codé par Splatch (`romasant`). Moteur de tests : la beta du testeur printf de Splatch.
- Lore et idées : Splatch. Marvin, le chevalier ; Moulinette, la chatte grise.
- Pixel art dessiné avec Python et Pillow : `tools/dessins.py` redessine les décors, Moulinette et ft_printf dans `assets/` (`python3 tools/dessins.py assets printf`).
- Sujet de référence : ft_printf v1.1.
- Licence MIT (voir `LICENSE`).
