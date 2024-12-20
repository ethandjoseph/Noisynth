# Noisynth
 A unique monophonic (for now) whitenoise synth based on my overtonePass Filter.<br>
 Each note consists of whitenoise but a bandpass fitler applied at that note's frequency plus bandpasses on 3 additional overtones (upto 20k Hz).

![Noisynth screenshot](https://github.com/ethandjoseph/Noisynth/blob/main/Noisynth%20plugin%20screenshot.png)

## Code
JUCE 8 project consisting of
1. MIDI input
2. White noise generator
3. [OvertonePass filter](https://github.com/ethandjoseph/Noisynth/blob/main/Source/OvertonePassFilter.cpp)
4. ADSR controls
5. Output Volume control

## [Release](https://github.com/ethandjoseph/Noisynth/releases)
- VST3 instrument
- 64 bit
