from io import TextIOWrapper
from os import system, remove
from sys import platform, argv
import re

@(lambda f: f())
def main():
  IFCMDS = {
    "eq": lambda xs: xs[0] == xs[1],
    "!eq": lambda xs: xs[0] != xs[1],
    "is": lambda xs: not not xs[0].strip(),
    "!is": lambda xs: not xs[0]
  }

  VARS = {
    "platform": platform.capitalize(),
    "input": "build.ninjax",
    "output": "build.ninja",
    "ninja": "ninja",
    "ninja_args": ""
  }

  for arg in argv[1:]:
    if '=' in arg:
      name, *vs = arg.split('=')
      VARS[name] = '='.join(vs)

  def evaluate(s: str) -> str:
    def ev(x: str):
      if x[0] == "$":
        if x[1:] in VARS:
          return VARS[x[1:]]
        else:
          return ""
      return x
    
    r = re.sub(r"[^\\]?\%([^\%]+)\%", lambda m: (m[0][0] if m[0][0] != '%' else '') + ev(m[1]), s)
    return r
    
  class Reader:
    def __init__(self, fout: TextIOWrapper, lines: list[str]) -> None:
        self.i = 0
        self.lines = lines
        self.olines: list[str] = []
    
    def write(self, s: str):
      self.olines.append(s)

    def parseLine(self, line: str):
      chunks = line.split()
      cmd = chunks[1]
      arg = ' '.join(chunks[2:])
      return chunks, cmd, arg

    def parseIfLine(self, arg: str):
        other = map(lambda s: s.strip(), arg.split(','))
        other = list(map(evaluate, other))
        return other

    def readLine(self, ignore: bool = False):
      line = self.lines[self.i]
      if line.strip().startswith('@'):
        chunks, cmd, arg = self.parseLine(line)

        if chunks[0] == "@if":
          if not ignore:
            other = self.parseIfLine(arg)
            check = IFCMDS[cmd](other)

            doLoop = self.lines[self.i + 1].strip()[0] == "@"
            if doLoop:
              chunks2, cmd2, arg2 = self.parseLine(self.lines[self.i + 1])

              if chunks2[0] == "@and" and not check:
                doLoop = False
              elif chunks2[0] == "@or" and check:
                doLoop = False
            
            if doLoop:
              while chunks2[0] in ["@or", "@and"]:
                check2 = IFCMDS[cmd2](self.parseIfLine(arg2))
                if chunks2[0] == "@or" and check2:
                    check = True
                    break
                elif chunks2[0] == "@and" and not check2:
                    check = False
                    break
                self.i += 1
                chunks2, cmd2, arg2 = self.parseLine(self.lines[self.i + 1])
          else:
            check = False

          self.i += 1
          if check:
            while not self.lines[self.i].strip() == "@fi":
              self.readLine()
              self.i += 1
          else:
            while not self.lines[self.i].strip() == "@fi":
              self.readLine(ignore = True)
              self.i += 1
        elif chunks[0] == "@var":
          if not ignore:
            name = chunks[1]
            self.write(f"{name} = {evaluate(arg)}")
        elif chunks[0] == "@set":
          if not ignore:
            name = chunks[1]
            VARS[name] = evaluate(arg)
        elif chunks[0] == "@debug":
          if not ignore:
            print("debug:", evaluate(' '.join(chunks[1:])))
      else:
        if not ignore:
          if line.strip():
            self.write(evaluate(line).rstrip('\n'))
          elif not self.lines[self.i + 1].strip(): # 2 empty lines
            self.write("")

  
  with open(VARS["input"]) as f, open(VARS["output"], "w") as fout:
    r = Reader(fout, f.readlines())
    while r.i < len(r.lines):
      r.readLine()
      r.i += 1
    
    for line in r.olines:
      fout.write(line + "\n")
  
  if "norun" not in argv[1:]:
    system(VARS["ninja"] + " " + VARS["ninja_args"])

  if "nodel" not in argv[1:]:
    remove("build.ninja")
