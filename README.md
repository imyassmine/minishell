# minishell

[![CI](https://github.com/imyassmine/minishell/actions/workflows/ci.yml/badge.svg)](https://github.com/imyassmine/minishell/actions/workflows/ci.yml)
![C](https://img.shields.io/badge/language-C-blue)

**A bash-like interactive shell written from scratch in C.** Its own lexer,
recursive-descent parser, AST executor, environment layer, signal handling and
garbage collector — the only external dependency is GNU Readline for line
editing.

*Systems programming · parsing · process management · UNIX signals · memory safety*

## Highlights

- **Complete pipeline** — `lexer → recursive-descent parser → AST → executor`;
  operator precedence and parentheses come from the grammar, not from special
  cases.
- **Real shell features** — pipelines of any length, `<`, `>`, `>>`, heredocs
  (quoted and expanded), single/double quotes, `$VAR` and `$?`, `&&`/`||`
  short-circuiting, `( ... )` grouping, `PATH` resolution and `*` wildcard
  expansion via `fnmatch(3)`.
- **Seven builtins** — `cd`, `echo`, `env`, `exit`, `export`, `pwd`, `unset`,
  with shell-conformant statuses including `1`, `2`, `126`, `127` and `130`,
  plus correct propagation of a pipeline's last command.
- **Signal-safe interactivity** — dedicated signal modes for the prompt,
  running commands and heredoc input: `Ctrl-C` interrupts a command without
  killing the shell, `Ctrl-D` exits, `Ctrl-\` is ignored at the prompt.
- **Memory model** — allocations are registered in a command-scoped garbage
  collector: each command's memory is reclaimed in one call, while the
  environment is explicitly locked and survives. No global state beyond one
  `volatile sig_atomic_t` flag.
- **Product-grade testing** — 20 PTY-driven integration scenarios cover
  builtins, quoting, redirections, heredocs, signals and exit codes, and run in
  CI on every push.

## Quick start

```console
# Debian / Ubuntu
$ sudo apt install build-essential libreadline-dev
$ make
$ ./minishell
```

`python3` is needed by the test suite; `valgrind` is optional (`./val.sh`).

```console
yassabda in /home/yassabda/minishell ⋙ echo "Hello, 42!"
Hello, 42!
yassabda in /home/yassabda/minishell ⋙ cat << EOF | tr a-z A-Z
heredoc > minishell is alive
heredoc > EOF
MINISHELL IS ALIVE
yassabda in /home/yassabda/minishell ⋙ false && echo nope || echo fallback
fallback
```

## Testing

```console
$ make test     # 20 PTY-driven integration scenarios
$ make debug    # rebuild with -g3 -fsanitize=address,undefined
$ ./val.sh      # Valgrind run, with suppressions for Readline internals
```

The suite drives the real binary through a pseudo-terminal — required because
the shell intentionally rejects non-interactive stdin. CI
([`.github/workflows/ci.yml`](.github/workflows/ci.yml)) builds and runs it on
every push to `main` and every pull request.

## Code map

| Area | Location |
| --- | --- |
| Lexer, parser | `src/lexer/`, `src/parser/` |
| Executor, pipes, redirections, expansion | `src/executor/` |
| Builtins | `src/builtins/` |
| Environment | `src/environment/` |
| Signals | `src/signal/` |
| libc helpers, `printf`, GC | `libft-extended/` |

## Scope

Interactive-only by design (it exits when stdin is not a TTY). Not implemented:
`;` separators, `${VAR:-default}`, `$(( ... ))`, `~` expansion, aliases and job
control (`&`, `Ctrl-Z`, `jobs`, `fg`).

---
*Built at [42 Paris](https://42.fr) by [Yassmine Abdallah (@imyassmine)](https://github.com/imyassmine)
and besch.
my scope: lexer, recursive-descent parser, AST executor, expansion, heredocs and the garbage collector; my teammate owned builtins, redirections, environment and signal handling.*
