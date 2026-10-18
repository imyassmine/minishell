#!/usr/bin/env python3
"""Smoke tests for minishell, driven through a pseudo-terminal.

The shell refuses to run when stdin is not a TTY, so each test runs it in a
fresh PTY. Usage: python3 tests/smoke_test.py [path/to/minishell]
"""
import fcntl
import os
import pty
import re
import select
import signal
import struct
import sys
import tempfile
import termios
import time

BIN = os.path.abspath(sys.argv[1] if len(sys.argv) > 1 else "minishell")
TIMEOUT = 10
PROMPT = "⋙".encode()

ANSI = re.compile(r"\x1b\[[0-9;?]*[A-Za-z]")
CTRL = re.compile(r"[\x00-\x08\x0b-\x1f\x7f]")
PROMPTS = re.compile(r"\S+ in \S* ⋙ *|heredoc > ")


def script(*lines):
    """One chunk of input: the given commands, then `exit`."""
    return ["\n".join(lines) + "\nexit\n"]


# (name, input chunks, expected output, expected exit code)
CASES = [
    ("echo", script("echo hello world"), "hello world", 0),
    ("echo -n", script("echo -n a && echo b"), "ab", 0),
    ("quotes", script("echo '$HOME' \"a  b\""), "$HOME a  b", 0),
    ("variables", script("export MS_VAR=42", "echo $MS_VAR", "unset MS_VAR",
                         "echo $MS_VAR"), "42", 0),
    ("external command", script("printf '%s\\n' a b"), "a\nb", 0),
    ("pipe", script("echo abc | tr a-z A-Z"), "ABC", 0),
    ("redirections", script("echo one > f.txt", "echo two >> f.txt",
                            "cat < f.txt"), "one\ntwo", 0),
    ("failed redirection", script("cat < /no/such/file", "echo $?"),
     "/no/such/file: No such file or directory\n1", 0),
    ("heredoc", script("cat << EOF | tr a-z A-Z", "heredoc body", "EOF"),
     "HEREDOC BODY", 0),
    ("&& and ||", script("false && echo no || echo yes"), "yes", 0),
    ("exit status", script("false", "echo $?"), "1", 0),
    ("command not found", script("nope_xyz", "echo $?"),
     "nope_xyz: Command not found\n127", 0),
    ("pipeline status", script("false | true", "echo $?"), "0", 0),
    ("cd + pwd", script("cd /tmp", "pwd"), "/tmp", 0),
    ("group", script("(cd /tmp && pwd)"), "/tmp", 0),
    ("wildcard", script("echo glob_*.md"), "glob_one.md glob_two.md", 0),
    ("quoted wildcard", script("echo 'glob_*.md'"), "glob_*.md", 0),
    ("exit builtin", script("exit 42"), "", 42),
    ("ctrl-c stops a command", ["sleep 30\n", "\x03", "echo $?\nexit\n"],
     "130", 0),
    ("ctrl-d leaves the shell", ["false\n", "\x04"], "", 1),
]


class Shell:
    """A minishell process attached to a pseudo-terminal."""

    def __init__(self, cwd):
        self.buf, self.eof = b"", False
        self.pid, self.fd = pty.fork()
        if self.pid == 0:
            try:
                os.chdir(cwd)
                size = struct.pack("HHHH", 50, 200, 0, 0)  # no line wrapping
                fcntl.ioctl(0, termios.TIOCSWINSZ, size)
                os.execve(BIN, [BIN], {**os.environ, "TERM": "dumb"})
            finally:
                os._exit(127)
        self.read(PROMPT)

    def read(self, until=b""):
        """Collect output until `until` shows up, or until the shell exits."""
        deadline = time.monotonic() + TIMEOUT
        while time.monotonic() < deadline and not (until and until in self.buf):
            if not select.select([self.fd], [], [], 0.1)[0]:
                continue
            try:
                chunk = os.read(self.fd, 65536)
            except OSError:  # EIO: the shell closed its terminal
                chunk = b""
            if not chunk:
                self.eof = True
                return
            self.buf += chunk

    def run(self, chunks):
        """Send the chunks, wait for the shell to exit: (output, exit code)."""
        self.buf = b""
        try:
            for i, chunk in enumerate(chunks):
                time.sleep(0.4 if i else 0)  # let signals/prompts settle
                os.write(self.fd, chunk.encode())
        except OSError:  # the shell is already gone
            pass
        self.read()
        if not self.eof:  # still running after the timeout: it hung
            os.kill(self.pid, signal.SIGKILL)
        _, status = os.waitpid(self.pid, 0)
        os.close(self.fd)
        return self.buf, os.waitstatus_to_exitcode(status)


def clean(raw, sent):
    """Drop escape codes, prompts and the terminal echo of what was typed."""
    text = ANSI.sub("", raw.decode("utf-8", "replace")).replace("^C", "")
    text = CTRL.sub("", text.replace("\r\n", "\n").replace("\r", "\n"))
    echoed = {line.strip() for line in sent.splitlines()} | {"exit"}
    lines = (line.strip() for line in PROMPTS.sub("", text).split("\n"))
    return "\n".join(line for line in lines if line and line not in echoed)


def main():
    if not os.access(BIN, os.X_OK):
        sys.exit(f"{BIN} not found: run `make` first")
    failed = 0
    with tempfile.TemporaryDirectory() as tmp:
        for name in ("glob_one.md", "glob_two.md"):
            open(os.path.join(tmp, name), "w").close()
        for name, chunks, expected, code in CASES:
            raw, status = Shell(tmp).run(chunks)
            out = clean(raw, "\n".join(chunks))
            if (out, status) == (expected, code):
                print(f"PASS  {name}")
                continue
            failed += 1
            print(f"FAIL  {name}")
            print(f"      expected: {expected!r} (exit {code})")
            print(f"      got:      {out!r} (exit {status})")
    print(f"\n{len(CASES) - failed}/{len(CASES)} cases passed")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
