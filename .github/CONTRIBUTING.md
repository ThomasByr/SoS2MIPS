# Contributing to SoS2MIPS!

Thank you for considering a contribution to the project! Here's some information on how that can be done optimally.

## Code of Conduct

In general, if you wouldn't do it at your workplace, don't do it here.

At it's core we are:

- Inclusive
- Supportive
- Constructive
- Friendly

Any behavior that fails to meet these core values will result in your issue or pull request being closed & you may be prevented from interacting with this project again.

## Pull Requests

Pull requests to fix issues or add new features are greatly appreciated, but having to outright reject contributions due to them being "not a good fit" is something we don't like to do. We ask that you coordinate changes with us to prevent any wasted time, as your time is valuable.

## Rejection

> If you send us a patch without coordinating it with us first, it will probably be immediately rejected, or sit in limbo for a long time and eventually be rejected. The reasons we do this vary from patch to patch, but some of the most common reasons are:
>
> **Unjustifiable Costs**: We support code in the upstream forever. Support is enormously expensive and takes up a huge amount of our time. The cost to support a change over its lifetime is often 10x or 100x or 1000x greater than the cost to write the first version of it. Many uncoordinated patches we receive are "white elephants", which would cost much more to maintain than the value they provide.
>
> As an author, it may look like you're giving us free work and we're rejecting it as too expensive, but this viewpoint doesn't align with the reality of a large project which is actively supported by a small, experienced team. Writing code is cheap; maintaining it is expensive.
>
> By coordinating with us first, you can make sure the patch is something we consider valuable enough to put long-term support resources behind, and that you're building it in a way that we're comfortable taking over.
>
> **Not a Good Fit**: Many patches aren't good fits for the upstream: they implement features we simply don't want. You can find more information in Contributing Feature Requests. Coordinating with us first helps make sure we're on the same page and interested in a feature.
>
> The most common type of patch along these lines is a patch which adds new configuration options. We consider additional configuration options to have an exceptionally high lifetime support cost and are very unlikely to accept them. Coordinate with us first.
>
> **Not a Priority**: If you send us a patch against something which isn't a priority, we probably won't have time to look at it. We don't give special treatment to low-priority issues just because there's code written: we'd still be spending time on something lower-priority when we could be spending it on something higher-priority instead.
>
> If you coordinate with us first, you can make sure your patch is in an area of the codebase that we can prioritize.
>
> **Overly Ambitious Patches**: Sometimes we'll get huge patches from new contributors. These can have a lot of fundamental problems and require a huge amount of our time to review and correct. If you're interested in contributing, you'll have more success if you start small and learn as you go.
>
> We can help you break a large change into smaller pieces and learn how the codebase works as you proceed through the implementation, but only if you coordinate with us first.
>
> **Generality**: We often receive several feature requests which ask for similar features, and can come up with a general approach which covers all of the use cases. If you send us a patch for your use case only, the approach may be too specific. When a cleaner and more general approach is available, we usually prefer to pursue it.
>
> By coordinating with us first, we can make you aware of similar use cases and opportunities to generalize an approach. These changes are often small, but can have a big impact on how useful a piece of code is.
>
> **Infrastructure and Sequencing**: Sometimes patches are written against a piece of infrastructure with major planned changes. We don't want to accept these because they'll make the infrastructure changes more difficult to implement.
>
> Coordinate with us first to make sure a change doesn't need to wait on other pieces of infrastructure. We can help you identify technical blockers and possibly guide you through resolving them if you're interested.

## Project structure

If you're looking for something - here's a breakdown of our codebase

```
./
├── .github
│   ├── ISSUE_TEMPLATE/            # template for new issues (we do not allow blank issues)
│   ├── workflows/                 # GitHub workflows (CI, code checking & build)
│   ├── CONTRIBUTING.md            # contributing guidelines (this file)
│   └── FUNDING.yml                # we all love money
│
├── .vscode/
│   └── settings.json              # plz use these settings 🥲
│
├── assets/                        # generic assets
│   └── imgs/                      # all images used in this project
│       └── ...
│
├── docs/
│   ├── projet.pdf                 # subject paper
│   └── spim_documentation.pdf     # spim and qtspim documentation
│
├── examples/                      # some .sos files examples
│   └── ...
│
├── gen/                           # all lex/yacc files
│   └── ...
│
├── helper/                        # MIPS32 assembly helper files
│   └── ...
│
├── inc/                           # include directory
│   ├── app.h                      # main function that parses the input file and launches qtspim
│   ├── dict.h                     # efficient key-value pair storage with hash map
│   ├── io.h                       # input and output handling for command line arguments
│   ├── lib.h                      # very generic utility functions
│   ├── protocol.h                 # utility functions and enums for MIPS32
│   ├── quad.h                     # quad structure
│   ├── symtable.h                 # symbol table
│   ├── threadpool.h               # a threadpool in c, because why not
│   └── vec.h                      # thread safe dynamic array
│
├── scripts/
│   ├── crlf-lf.bash               # dos2unix but using 8 threads 
│   └── details.py                 # to make the <details> section because I'm lazy
│
├── src/                           # source directory
│   ├── app.c
│   ├── dict.c
│   ├── io.c
│   ├── lib.c
│   ├── protocol.c
│   ├── main.c
│   ├── quad.c
│   ├── symtable.c
│   ├── threadpool.c
│   └── vec.c
│
├── tests/                         # test suite 👻
│   └── ...
│
├── vm/                            # MIPS32 emulator
│   ├── qtspim/                    # qtspim source code
│   └── qtspim.deb                 # qtspim .deb linux package
│
├── .gitignore                     # VS gitignore ⚙️
│
├── changelog.md                   # the full history, or so was I told...
│
├── LICENSE                        # GPL-3.0 License
│
├── makefile                       # makefile for gcc [-std=gnu17]
│
├── README.md                      # latest version of README file 👀
│
└── setup.bash                     # generic test&build script
```
