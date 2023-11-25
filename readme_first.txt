

Building:
Do not compile example .ino's from this directory.
Copy an example .ino to the src/ directory then rename to src.ino
Build with Faster (-O2). If building with Fastest (-O3) then USE_DMAMEM_BUFFER (config.h) may need to be set.


Fonts:
Font are sourced from a mixture of original BDF and TTF/TTC converted to BDF, modified to suit then converted to the custom binary .uf format.
Internally, a Font is divided in to blocks where each block references a unicode codepoint range. Each font is intended for rendered either as Mono fixed width or Proportional space, but this may be ignored at runtime through a Print flag.
