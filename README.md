# tabi

## Overview

Plaintext drum tablatures from MIDI files

This is an abandoned hobby project from 2002 when I was in Finnish high
school. It reads a MIDI file and produces drum tablature in plaintext.
Below is an example text output.

```
  |----------------|----------------|----------------|----------------|

H |x-xxx-xxx-xxx-X-|--X---X---X---X-|x-xxx-xxx-xxx-X-|--X---X---X---X-|
S |----o----o--o---|----@----o--o---|----o----o--o---|----@----o--o---|
B |o-------o-------|o-------o-------|o-------o-------|o-------o-------|

R |--------+-+-+-++|----------------|----------------|----------------|
C |----------------|--x-------------|----------------|----------------|
H |xxxxxxxx--------|----------------|----------------|----------------|
S |-o-o-o-o--------|----------------|----------------|----------------|
B |o-o-o-o-o-o-o-o-|o---------------|----------------|----------------|

H |X-X-X-X-X-X-|XXXXXX-X-X-X-|X-X-X-X-X-X-|XXXXXX-X-X-X-X---|
S |----o-----o-||---oooooo-o-|----o-----o-||---oooooo-o---o-|
B |o------o-o--|oo------o-o--|o------o-o--|oo------o-o--o---|

C |----------------|----------------|----------------|------------x---|
H |x-xxx-xxx-Xxx-xx|x-Xxx-xxx-Xxx-X-|x-xxx-xxx-Xxx-xx|x-xxx-Xxx-xx----|
S |----o----o--o---|-o--o----o--o---|----o----o--o---|o--o--o--o------|
B |o-------o-------|o-------o-------|o-------o-------|--o--o--o--o----|

C |----------------|--------------------x---|----------------|
H |--X---X---X---X-|------------------------|--X---X---X---X-|
S |----@--@----@--@|o---ooo-o---ooo-o---oooo|----@--@----@--@|
B |o-------o-------|o-o-o-o-o-o-o-o-o-o-o-o-|o-------o-------|

C |----------------|------------|x---------------|
F2|----------------|------------|o---------------|
H |--X---X---X---X-|xxxxxxxxxxxx|----------------|
S |----@--@-@--@--@|o--o--o--o--|----------------|
B |o-------o-------|o--o--o--o--|o---------------|
```


Drum symbols
```
B bass drum
S snare drum
H hi-hat
C crash cymbal
F1-F3 floor toms
R ride cymbal
```

Note symbols:
```
o, x regular hit
@, X accentuated hit
+    ride cymbal bell
```

## Features

- 4/4 time signature
- dynamic quantization per measure: Selects 8th, 12th or 16th note pattern whichever applicable
- 80 character limit per row
- dynamic display of drum lines per row: only lines which have notes on current row will be visualized

