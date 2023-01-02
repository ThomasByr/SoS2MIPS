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
   3. [ThreadPool `threadpool_t`](#threadpool-threadpool_t) -->
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

Le projet a été réalisé en groupe de 4 personnes. La première partie du projet, qui consiste à préparer une application fonctionnelle avec récupération des arguments en ligne de commande n'est pas parallélisable, et a donc été réalisée par une seule personne. Les structures de vecteur `vec_t`, de dictionnaire `dict_t`, et de threadpool `threadpool_t` ont également été réalisées par une seule personne.

Les autres parties du projet ont été réalisées en groupe (notamment la génération de code intermédiaire et la génération de code assembleur).

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

Le vecteur est donc composé d'un tableau de pointeurs `void*` (`data`), d'une taille (`size`), d'une capacité (`capacity`), et d'un mutex (`lock`). Le mutex est utilisé pour rendre le vecteur thread-safe.

Pour initialiser un vecteur, il faut utiliser la macro `vec_new`. Cette macro permet l'utilisation d'un paramètre optionnel pour définir la capacité initiale du vecteur. Par défaut, la capacité initiale est de 0, et le vecteur est réalloué à chaque fois que la capacité est atteinte. Si la capacité initiale est définie, le vecteur est alloué d'un coup avec la capacité initiale donnée.

```c
vec_t vec = vec_new(); // vec_new(10) for a vector with initial capacity of 10
vec_push(vec, (void*) 42);

printf("%d\n", (int) vec_get(vec, 0)); // prints 42
vec_free(vec);
```
---

### Dictionnaire `dict_t`

Le dictionnaire est une structure de données dynamique qui permet de stocker des paires clé-valeur de type `void*` et `void*`. Il est possible d'ajouter des paires clé-valeur, de supprimer des paires clé-valeur, et de récupérer une valeur à partir d'une clé donnée. Son header se trouve dans le fichier [`inc/dict.h`](../inc/dict.h).

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

## Tests

Les tests unitaires sont disponibles dans le dossier [`tests/`](../tests/). Ils sont écrits en C et supposent que [Valgrind](https://valgrind.org/) est installé sur la machine. Pour compiler et exécuter les tests, il faut utiliser la commande `make check`.

```bash
cd tests && make check
```

De manière alternative, `make check_quiet` permet d'exécuter les tests en redirigeant toutes les sorties du programme vers `/dev/null`. `make` n'utilise pas cette commande par défaut.

## Documentation et Utilisation

## Milestones

## License
