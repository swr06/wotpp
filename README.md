<p align=center><img alt="logo" src="docs/img/logo.png" width=20%></p>

# wot++
Wot++ is a macro language which allows you to abstract away the boilerplate when writing things like HTML by hand or even
more low level tasks which require bit-level manipulation.

![c++](https://img.shields.io/badge/c%2B%2B-%3E%3D17-blue.svg?style=flat)
[![license](https://img.shields.io/github/license/Jackojc/wotpp.svg?style=flat)](./LICENSE)
![code size](https://img.shields.io/github/languages/code-size/Jackojc/wotpp?style=flat-square)
[![issues](https://img.shields.io/github/issues/Jackojc/wotpp.svg?style=flat)](https://github.com/Jackojc/wotpp/issues)
[![discord](https://img.shields.io/discord/537732103765229590.svg?label=discord&style=flat)](https://discord.gg/RmgjcES)

### What does Wot++ do differently to things like LaTeX and Groff?
In contrast to LaTeX and Groff, Wot++ is a macro-first, text-second language. What do I mean by this?
In LaTeX or Groff, the text is the primary focus with macros being an "opt-in" thing you can use inbetween the words.
It's the exact opposite in Wot++, you have to "opt-in" to using text by quoting it.

Wot++ is far more flexible through its ability to use meta-programming, recursion, conditionals, external shell programs etc.

### Prerequisites
- A C++17 compliant compiler. (GCC & Clang work)

### Build & Run
```
$ meson build
$ ninja -C build
```

Supported flags:
```
-Dnative=true               # enable host machine specific optimisations
-Dsanitizers=true           # enable sanitizers (undefined,address)
-Dprofile=true              # enable profiling support (uftrace etc.)
-Ddisable_run=false         # disable the `run` intrinsic for security purposes
-Dbuildtype=debugoptimised  # enable symbols
```
> List of built-in Meson options can be found [here](https://mesonbuild.com/Builtin-options.html).

An example:
```
$ meson -Dsanitizers=true -Dprofile=true build
```

If you already have configured a build directory, you'll need to add `--reconfigure`:
```
$ meson --reconfigure -Dsanitizers=true -Dprofile=true build
```

To run the tests:
```
$ ninja -C build test
```

### Installation
```
$ cd build/
$ meson install  # requires root perms (sudo/su/doas)
```

If you want to specify a custom install directory:
```
$ cd build/
$ DESTDIR=/usr/local/bin/ meson install
```

### Acknowledgements
Thanks, in no particular order:
- [Noxabellus](https://github.com/noxabellus)
- [DeKrain](https://github.com/dekrain)
- [AnonymousPlayer](https://github.com/Anonymus-Player)
- [Ciarán](https://github.com/iCiaran)
- [Binkiklou](https://github.com/binkiklou)
- [Strexicious](https://github.com/strexicious/)
- [Eremiell](https://github.com/Eremiell)
- [Violetas](https://github.com/violetastcs)
- [Moonsheep](https://github.com/jlagarespo)

And everyone else who I may have failed to mention over the years.

### License
This project uses the MPL-2.0 license. (check [LICENSE.md](LICENSE.md))

### Progress & Discussion
You can join the discord server in order to follow progress and/or contribute to discussion of the project. (https://discord.gg/RmgjcES)

