---
marp: true
theme: dracula
paginate: true
size: 9:16
---

# Rapport de Compilation

> Rapport en français.
> Auteurs : Thomas D., Ethan H., Mathieu M. et Thomas B.

1. [Introduction](#introduction)
2. [Répartition du travail](#répartition-du-travail)
3. [Structure du projet](#structure-du-projet)
4. [Quelques structures en détail](#quelques-structures-en-détail)
   <!-- 1. [Vecteur `vec_t`](#vecteur-vec_t)
   2. [Dictionnaire `dict_t`](#dictionnaire-dict_t)
   3. [ThreadPool `threadpool_t`](#threadpool-threadpool_t)
   4. [Pile d'instructions assembleur `astack_t`](#pile-dinstructions-assembleur-astack_t) -->
5. [Tests](#tests)
6. [Documentation et Utilisation](#documentation-et-utilisation)
7. [Milestones](#milestones)
8. [License](#license)


---

## Introduction

Le but de ce projet est de réaliser un compilateur pour le langage de programmation SoS vers le langage assembleur MIPS. Le compilateur est écrit en C et utilise les bibliothèques flex et bison.

Avant de commencer, il faut tout d'abord installer les dépendances nécessaires au projet. Pour ce faire, il faut exécuter la commande suivante :

```bash
sudo apt install flex bison
```

Un paquet Debian pour qtspim (l'émulateur MIPS choisi) est également disponible dans le dossier `vm`. Pour l'installer, il faut exécuter les commandes suivantes :

```bash
cd vm && sudo dpkg -i qtspim.deb
sudo apt-get install libgl1 libxkbcommon-x11-0
```

Ensuite, il faut compiler une version de production du programme avec la commande suivante :

```bash
make release
```

Le binaire exécutable produit se trouve dans le dossier `bin`.

## Répartition du travail

Le projet a été réalisé en groupe de 4 personnes. La première partie du projet, qui consiste à préparer une application fonctionnelle avec récupération des arguments en ligne de commande n'est pas parallélisable, et a donc été réalisée par une seule personne. Les structures de vecteur `vec_t`, de dictionnaire `dict_t`, de threadpool `threadpool_t`, et de pile d'instructions `astack_t` ont également été réalisées par une seule personne.

Les autres parties du projet ont été réalisées en groupe (notamment la génération de code intermédiaire avec les quads et la génération de code assembleur).

## Structure du projet

Le projet est composé de plusieurs dossiers :

- `src` : contient le code source du projet
- `inc` : contient les fichiers d'en-tête du projet
- `gen` : contient les fichiers sources flex et bison
- `tests` : contient les fichiers de tests unitaires
- `vm` : contient les fichiers nécessaires à l'émulation du code assembleur
- `docs` : contient les fichiers de documentation du projet ainsi que ce rapport
- `styles` : contient les fichiers de style pour la génération du rapport

Les fichiers et dossiers suivants servent à la maintenance et à l'intégration continue du projet :

- `makefile` : fichier de configuration de la compilation
- `.gitignore` : fichier de configuration de git
- `setup.bash` : tests et compilation générique lors de _releases_ sur github
- `.github/` : contient les fichiers de configuration github
- `.vscode/` : contient les fichiers de configuration de l'éditeur de texte Visual Studio Code
- `scripts/` : contient des scripts pour la génération de documentation

---

## Quelques structures en détail

Pour faciliter la compréhension du code, nous avons défini plusieurs structures de données. Nous allons détailler celles qui ne découlent pas immédiatement de la lecture des documents fournis en cours, en td ou en tp. Nous allons spécifier à chaque fois le header dans lequel la structure est définie, ainsi que la structure elle-même.

### Vecteur `vec_t`

Le vecteur est une structure de données dynamique qui permet de stocker des éléments de type `void*`. Il est possible d'ajouter des éléments à la fin du vecteur, de supprimer des éléments à la fin du vecteur, et de récupérer un élément à une position donnée. Son header se trouve dans le fichier [`inc/vec.h`](../inc/vec.h).

La structure `vec_s` est intentionnellement opaque à l'utilisateur. Elle est définie dans le fichier [`src/vec.c`](../src/vec.c) comme suit :

```c
/// inside vec.h
typedef struct vec_s *vec_t;

/// inside vec.c
struct vec_s {
  void **data;
  size_t size;
  size_t capacity;

  pthread_mutex_t *lock;
};
```

Le vecteur est donc composé d'un tableau de pointeurs `void*` (`data`), d'une taille (`size`), d'une capacité (`capacity`), et d'un mutex (`lock`). Le mutex est utilisé pour rendre le vecteur thread-safe. Le vecteur se redimensionne automatiquement à chaque fois que la capacité est atteinte (à un certain pourcentage) ; et dès que des éléments sont insérés ou supprimés, le vecteur vide l'espace mémoire non utilisé mais le conserve pour les prochaines insertions.

Pour initialiser un vecteur, il faut utiliser la macro `vec_new`. Cette macro permet l'utilisation d'un paramètre optionnel pour définir la capacité initiale du vecteur. Par défaut, la capacité initiale est de 0, et le vecteur est réalloué à chaque fois que la capacité est atteinte. Si la capacité initiale est définie, le vecteur est alloué d'un coup avec la capacité initiale donnée.

```c
vec_t vec = vec_new(); // vec_new(10) for a vector with initial capacity of 10
vec_push(vec, (void*) 42);

printf("%d\n", (int) vec_get(vec, 0)); // prints 42
vec_free(vec);
```

---

### Dictionnaire `dict_t`

Le dictionnaire est une structure de données dynamique qui permet de stocker des paires clé-valeur de type `void*` et `void*` (par défaut, `void *` et `size_t` ont la même taille en mémoire, nous pouvons donc utiliser les deux de manière transparente). Il est possible d'ajouter des paires clé-valeur, de supprimer des paires clé-valeur, et de récupérer une valeur à partir d'une clé donnée. Son header se trouve dans le fichier [`inc/dict.h`](../inc/dict.h).

La structure `dict_s` est intentionnellement opaque à l'utilisateur. Elle est définie dans le fichier [`src/dict.c`](../src/dict.c) comme suit :

```c
/// inside dict.h
typedef struct dict_s *dict_t;

/// inside dict.c
struct dict_s {
  int hash_content;
  size_t nbits;
  size_t mask;

  size_t capacity;
  size_t *keys;
  size_t *values;
  size_t nitems;
  size_t n_deleted_items;
};
```

Le dictionnaire est donc composé d'un tableau de clés (`keys`), d'un tableau de valeurs (`values`), d'une capacité (`capacity`), d'un nombre d'éléments (`nitems`), et d'un nombre d'éléments supprimés (`n_deleted_items`). Le dictionnaire est également composé d'un masque (`mask`) et d'un nombre de bits (`nbits`) qui permettent de calculer l'index d'un élément dans le tableau de clés et de valeurs. Le dictionnaire est également composé d'un booléen (`hash_content`) qui permet de déterminer si les clés doivent être interprétées comme des chaines de caractères ou comme des pointeurs.

Pour initialiser un dictionnaire, il faut utiliser la macro `dict_new`. Cette macro permet l'utilisation d'un paramètre optionnel pour définir le mode de hashage des clés. Par défaut, les clés sont interprétées comme des pointeurs. Si le paramètre optionnel est défini à 1, les clés sont interprétées comme des chaines de caractères.

```c
dict_t dict = dict_new(1); // hash content

// following example also works with default mode
dict_push(dict, (void *)"foo1", (void *)"bar");
dict_push(dict, (void *)"foo2", (void *)"baz");

char *value = (char *)dict_get(dict, (void *)"foo1");
printf("%s\n", value); // prints bar
value = (char *)dict_get(dict, (void *)"foo2");
printf("%s\n", value); // prints baz
dict_free(dict);
```

---

### ThreadPool `threadpool_t`

Le threadpool est une structure de données dynamique qui permet de créer un ensemble de threads qui peuvent exécuter des tâches. Il est possible d'ajouter des tâches au threadpool, et de récupérer le résultat d'une tâche. Son header se trouve dans le fichier [`inc/threadpool.h`](../inc/threadpool.h).

La structure `threadpool_s` est intentionnellement opaque à l'utilisateur. Elle est définie dans le fichier [`src/threadpool.c`](../src/threadpool.c) comme suit :

```c
/// inside threadpool.h
typedef struct threadpool_s *threadpool_t;

/// inside threadpool.c
struct threadpool_s {
  pthread_mutex_t lock;
  pthread_cond_t notify;
  pthread_t *threads;
  threadpool_task_t *queue;
  int thread_count;
  int queue_size;
  int head;
  int tail;
  int count;
  int shutdown;
  int started;
};

typedef struct {
  void (*function)(void *);
  void *argument;
} threadpool_task_t;
```

Le threadpool est donc composé d'un tableau de threads (`threads`), d'un tableau de tâches (`queue`), d'un nombre de threads (`thread_count`), d'une taille de queue (`queue_size`), d'un index de tête de queue (`head`), d'un index de queue (`tail`), d'un nombre de tâches dans la queue (`count`), d'un booléen (`shutdown`) qui indique si le threadpool est en train de se terminer, et d'un booléen (`started`) qui indique si le threadpool a été démarré. Le threadpool est également composé d'un mutex (`lock`) et d'une condition (`notify`) qui permettent de synchroniser les threads.

Pour initialiser un threadpool, il faut utiliser la fonction `threadpool_create`, qui prend en paramètre le nombre de threads à créer, la taille de la queue, et des flags (qui ne sont pas utilisés pour le moment).

```c
threadpool_t pool = threadpool_create(4, 100, 0);

// add tasks to threadpool
for (int i = 0; i < 100; i++) {
  threadpool_add(pool, &some_function, NULL, 0);
}

// wait for all tasks to finish and destroy threadpool
threadpool_destroy(pool, 1)
```

---

### Pile d'instructions assembleur `astack_t`

La pile d'instructions assembleur est une structure de données dynamique qui permet de stocker des instructions assembleur. Elle est utilisée par le compilateur pour stocker les instructions assembleur block par block générées par le compilateur. Il s'agit de la seule structure composite, dont son fonctionnement dépend entièrement du bon fonctionnement des structures précédentes. Son header se trouve dans le fichier [`inc/assstack.h`](../inc/assstack.h).

La structure `astack_s` est intentionnellement opaque à l'utilisateur. Elle est définie dans le fichier [`src/assstack.c`](../src/assstack.c) comme suit :

```c
/// inside assstack.h
typedef struct astack_s *astack_t;

/// inside assstack.c
struct astack_s {
  vec_t data;
  vec_t text;
  vec_t name;
  dict_t text_blocks;
};
```

La pile d'instructions assembleur est composée d'un vecteur de données (`data`), d'un vecteur de vecteurs de textes (`text`), d'un vecteur de noms (`name`), et d'un dictionnaire de blocs de texte (`text_blocks`). Le vecteur de données (`data`) est utilisé pour stocker les directives assembleur de la section `.data`. Le vecteur de vecteurs de textes (`text`) est utilisé pour stocker les directives assembleur de la section `.text` ; chaque vecteur de textes correspond à un bloc de texte. Le vecteur de noms (`name`) est utilisé pour stocker les noms des blocs de texte, dans le même ordre que le vecteur de vecteurs de textes. Le dictionnaire de blocs de texte (`text_blocks`) est utilisé pour stocker les correspondances entre les noms des blocs de texte et leurs index dans le vecteur de vecteurs de textes.

Pour initialiser une pile d'instructions assembleur, il faut utiliser la macro `astack_new`, qui ne prend deux paramètres, la taille initiale du vecteur de données et la taille initiale du vecteur de vecteurs de textes.

```c
// won't work when freeing astack but it's ok for this example
// normally, we would malloc bloc names
// directives are copied, so we can free them after pushing them
// which allows us to also use string literals
astack_t stack = astack_new(2, 10);
astack_push_data(stack, "msg: .asciiz \"Hello World!\"");
astack_push_text(stack, "main", "li $v0, 4");
astack_push_text(stack, "main", "la $a0, msg");
astack_push_text(stack, "main", "syscall");
```

Par défaut, le block `main` de la section `.text` est créé. Il est possible d'ajouter des directives assembleur à la pile d'instructions assembleur avec les fonctions `astack_push_data` et `astack_push_text`. La fonction `astack_push_data` prend en paramètre une directive assembleur de la section `.data` et l'ajoute au vecteur de données. La fonction `astack_push_text` prend en paramètre une directive assembleur de la section `.text` et le nom du bloc de texte auquel elle appartient. Si le bloc de texte n'existe pas, il est créé. La directive assembleur est ensuite ajoutée au vecteur de vecteurs de textes correspondant au bloc de texte.

Une représentation de la pile d'instruction assembleur est disponible ci-dessous.

```c
// .data segment representation
vec_t data : { "msg: .asciiz \"Hello World!\"" }

// .text segment representation
vec_t text : {
  vec_t text_block_1 : { "li $v0, 4", "la $a0, msg", "syscall" },
}
vec_t name : { "main" }
dict_t text_blocks : { "main" => 1 /* index cannot be 0 (will cast to NULL) */ }
```

Le dictionnaire est utile et très efficace lorsqu'il s'avère nécessaire de chercher un bloc de texte dans le vecteur de vecteurs de textes. En effet, la recherche dans un dictionnaire est en moyenne plus rapide que la recherche dans un vecteur. A la fin de la compilation, la fonction `astack_fprintf` permet d'écrire la pile d'instructions assembleur dans un fichier. Le vecteur de données est écrit dans la section `.data` et le vecteur de vecteurs de textes est écrit dans la section `.text`, dans l'ordre des noms des blocs de texte.

---

## Tests

Les tests unitaires sont disponibles dans le dossier [`tests/`](../tests/). Ils sont écrits en C et supposent que [Valgrind](https://valgrind.org/) est installé sur la machine. Pour compiler et exécuter les tests, il faut utiliser la commande `make check`.

```bash
cd tests && make check
```

De manière alternative, `make check_quiet` permet d'exécuter les tests en redirigeant toutes les sorties du programme vers `/dev/null`. `make` n'utilise pas cette commande par défaut.

## Documentation et Utilisation

La documentation intégrée est disponible avec la commande `--help` ou `-h`. Voici néanmoins un résumé des options disponibles. Nous indiquons si une option requiert un argument avec `<>`, et si une option courte n'est pas disponible, nous indiquons `..` à la place.

| commande             | aide                                      | requis ? | défaut |
| -------------------- | ----------------------------------------- | -------- | ------ |
| `-h, --help`         | affiche l'aide et **exit**                | ❔       |        |
| `-v, --version`      | affiche la version et **exit**            | ❔       |        |
| `-l, --license`      | affiche la licence **exit**               | ❔       |        |
| `-i, --in` `<>`      | chemin vers le fichier à compiler         | ✔️       |        |
| `-o, --out` `<>`     | fichier vers lequel sauver le code généré | ❌       | `a.s`  |
| `.., --tos`          | affiche la table des symboles             | ❌       |        |
| `.., --verbose`      | quelques infos supplémentaires            | ❌       |        |
| `.., --no-exe`       | n'exécute pas le fichier produit          | ❌       |        |
| `-O, --opt_lvl` `<>` | niveau d'optimisation (parmis `0` ou `1`) | ❌       | `0`    |

Notez que l'option `-O` n'est à ce jour pas implémentée. Par ailleurs, les fichiers d'entrée et de sortie peuvent être spécifiés sans utiliser les options `-i` et `-o`. Il suffit de les passer en argument de la commande, dans l'ordre si les deux venaient à être spécifiés de cette manière. Si un seul fichier est spécifié sans option, il est considéré comme le fichier manquant parmi les deux.

Un exemple d'utilisation est disponible ci-dessous.

```bash
./bin/sos -o out.s examples/hello_world.sos --tos --verbose
```

---

## Milestones

Nous allons maintenant détailler les différentes étapes de développement du projet. Les exemples de code ci-dessous sont pour la plupart issus de tests, et certains peuvent même être retrouvés dans le dossier [examples/](../examples/). Notez également que ces étapes reflètent l'avancement de notre projet uniquement à l'instant où nous avons écrit ce document.

### opérations arithmétiques

La toute première étape du projet a été de faire fonctionner les opérations arithmétiques basiques (`+`, `-`, `*`, `/`, `%`) qui ne fonctionnent que sur les entiers.

```sos
$( expr 1+2 );
$( expr (1+2)*3 );
$( expr 1+2*3 );
exit
```

```asm

```

### stockage de variables entières

La seconde étape a été de faire fonctionner le stockage de variables. Nous avons choisi d'écrire les variables entières dans le segment `.data`. Il est possible de stocker des variables dans le segment `.text` mais cela nécessite de faire des manipulations supplémentaires pour accéder à la mémoire. Nous avons donc choisi de stocker les variables dans le segment `.data` pour simplifier le code généré.

```sos
i = 1;
i = $( expr ${i}+1 );
exit
```

```asm

```

### stockage de variables de type tableau d'entiers

L'étape naturelle suivante a été de faire fonctionner le stockage de variables de type tableau d'entiers. Nous avons choisi de stocker les tableaux dans le segment `.data` à l'aide de la directive `.word`.

```sos
declare tab[10];
tab[0] = 1;
exit
```

```asm

```

---

## Licence

Ce projet est sous licence GNU GPL v3.0. Pour plus d'informations, veuillez consulter le fichier [LICENSE](../LICENSE). Pour obtenir une copie de la licence, veuillez consulter <https://www.gnu.org/licenses/gpl-3.0.html>. Vous pouvez également obtenir une copie de la licence en écrivant à la Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

Copyright (C) 2022-2023, SOS Authors - all rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
