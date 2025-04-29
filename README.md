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

## History

At early 2000s, I played in a band but also composed MIDI music with Voyetra
MIDI Orchestrator Plus. Some of the band members learned to play the guitar
first by downloading text-based guitar tabs. Later there was Guitar Pro which
combined MIDI playback with dynamic guitar tablature display. However, our
drummer didn't have tabs, so I tried transcribing certain songs first as MIDI
and then exporting them as printed, text-based drum tabs.

I had ambitions high: I thought it would be cool to be able to produce tabs
to some Tool songs with odd time signatures, E.g. "Schism" or "Lateralus". ;)

