# Muse Dash BMS Format

This file is just a remainder of all the things I know of the BMS files
for Muse Dash. Because that's hard to find information.

Every command in a BMS file is a line who start with a `#` and every
other line is a comment.

There is two type of commands, the header ones and the other ones (idk
how to call them).

## Header

The header commands is basically assigning values to variables and it
look like this:

```text
#NAME VALUE
```

I think i don't need to explain more, seems obvious to me.

The header is composed of minimum 2 values, **GENRE** and **BPM**, and
some optional **WAVxx** values (for modding only).

### **GENRE**

The scene used for the chart. It's value is `scene_xx` where `xx` is the
corresponding number of the scene.

- 01 Space Station
- 02 Retrocity
- 03 Castle
- 04 Rainy Night
- 05 Candyland
- 06 Oriental
- 07 Let's Groove
- 08 Touhou
- 09 DJMAX

### **BPM**

The original BPM of the chart (can be changed during the gameplay).

### **WAVxx**

Used for custom sounds and custom beats (not implemented yet, I'm
looking forward to it). I will explain them here if needed.

## Main data

The main data is composed of lines that look like this:

```text
#xxxyy:aabbccdd
```

- `xxx` value is a 3 digit number for the measure (from 000 to 999).
- `yy` value is a two digit number (i don't think it's hexadecimal or
    something) for the channel.
- `aabbccdd` are the notes, each note (`aa`, `bb`, `cc` or `dd`) is a
    two base36 character (base36 is basically hexadecimal but it goes to
    Z). It can be other kind of values than notes for some specials
    channels.

### Channels

------------------------------------------------------------------------

Muse Dash use only a few channels but they all have their own duty:

- Channel 02: Beat
- Channel 03: BPM
- Channel 13: High notes
- Channel 14: Low notes
- Channel 15: Events
- Channel 53: High hold notes
- Channel 54: Low hold notes

Other channels could be used by mods but idk any.

> Channel 02

Channel 02 is used to change the number of beat in one measure, and it
doesn't ask for notes but for a number (can be decimal). This number is
multiplied by 4 and it gives you the number of beats per measure. Set it
to 1 for 4 beats per measure, to 1.5 for 6 beats, 1.25 for 5 beats, etc.
Can be called once per measure, obviously.

> Channel 03

Channel 03 is used to change the BPM. The BPM can be changed at any time
(or any beat/note to be exact). The value is in hexadecimal and can't be
more than 255 (255 = FF; you can't put more than 2 characters). The
value is placed on the desired beat/note, see the note placing
explanation for more info.

> Channel 13

Channel 13 is used to set the upper notes.

> Channel 14

Channel 14 is used to set the lower notes.

> Channel 15

Channel 15 is used for the events and effects, like Boss events,
changing the scene, hidding the notes, etc. Those events are placed like
notes.

> Channel 53

Channel 53 is used to set other upper notes (Putting sliders here allow
us to place other notes on top of it with the 13 channel).

> Channel 54

Channel 54 is used to set other lower notes (Putting sliders here allow
us to place other notes on top of it with the 14 channel).

### Notes

------------------------------------------------------------------------

The notes in BMS are two base36 character that act like an id. Some
notes are events, but this doesn't change the rules of where they are
placed in the chart.

Those are the rules:

- The 00 value is a blank note
- To place notes on half beats, the number of written notes is doubled
    (i.e. normal beats: *aabbccdd* ; half beats:
    *aa**ii**bb**ii**cc**ii**dd**ii***)
- This work also for optimzing with blank notes (i.e. this *aa00bb00*
    is the same as *aabb*)

Here are all the different notes in MD, with their value:

- 01 Small
- 02 Small (Up)
- 03 Small (Down)
- 04 Medium 1
- 05 Medium 1 (Up)
- 06 Medium 1 (Down)
- 07 Medium 2
- 08 Medium 2 (Up)
- 09 Medium 2 (Down)
- 0A Large 1
- 0B Large 2
- 0C Raider
- 0D Hammer
- 0E Gemini
- 0F Hold
- 0G Masher
- 0H Gear
- 0O Speed 1 (Both)
- 0P Speed 2 (Both)
- 0Q Speed 3 (Both)
- 0R Speed 1 (Low)
- 0S Speed 2 (Low)
- 0T Speed 3 (Low)
- 0U Speed 1 (High)
- 0V Speed 2 (High)
- 0W Speed 3 (High)
- 10 Reserved and not used
- 11 Boss Melee 1
- 12 Boss Melee 2
- 13 Boss Projectile 1
- 14 Boss Projectile 2
- 15 Boss Projectile 3
- 16 Boss Masher 1
- 17 Boss Masher 2
- 18 Boss Gear
- 1A Boss Entrance
- 1B Boss Exit
- 1C Boss Ready Phase 1
- 1D Boss End Phase 1
- 1E Boss Ready Phase 2
- 1F Boss End Phase 2
- 1G Boss Swap Phase 1-2
- 1H Boss Swap Phase 2-1
- 1J Hide Notes
- 1K Unhide Notes
- 1L Hide Boss
- 1M Unhide boss
- 1O Scene Switch (Space Station)
- 1P Scene Switch (Retrocity)
- 1Q Scene Switch (Castle)
- 1R Scene Switch (Rainy Night)
- 1S Scene Switch (Candyland)
- 1T Scene Switch (Oriental)
- 1U Scene Switch (Groove Coaster)
- 1V Scene Switch (Touhou)
- 1W Scene Switch (DJMAX)
- 20 P Item
- 21 Ghost
- 22 Heart
- 23 Note
- 25 Hide Background
- 26 Unhide Background
- 27 Screen Scroll UP
- 28 Screen Scroll DOWN
- 29 Screen Scroll OFF
- 2A Scanline Ripples ON
- 2B Scanline Ripples OFF
- 2C Chromatic Aberration ON
- 2D Chromatic Aberration OFF
- 2E Vignette ON
- 2F Vignette OFF
- 2G TV static ON
- 2H TV static OFF
- 2I Flashbang START
- 2J Flashbang MID
- 2K Flashbang END
- 30 Medium Bullet
- 31 Medium Bullet (Up)
- 32 Medium Bullet (Down)
- 33 Medium Bullet (Laneshift)
- 34 Small Bullet
- 35 Small Bullet (Up)
- 36 Small Bullet (Down)
- 37 Small Bullet (Laneshift)
- 38 Large Bullet
- 39 Large Bullet (Up)
- 3A Large Bullet (Down)
- 3B Large Bullet (Laneshift)
- 3C Boss Bullet 1
- 3D Boss Bullet 1 (Laneshift)
- 3E Boss Bullet 2
- 3F Boss Bullet 2 (Laneshift)

It look like they are organized in ranges.

- \[01 - 0W\] Normal notes
- \[11 - 1H\] Boss events
- \[1J - 1M\] Hide Notes/Boss events
- \[1O - 1x\] Scene switching events
- \[20 - 23\] Special notes
- \[25 - 2x\] High visual effects
- \[30 - 3x\] Boss notes

Gears can only be used in the lower channels, boss gears in the upper.

Event can only be used in the event channel.

Sliders work by pairs, one for the beginning, one for the end.

------------------------------------------------------------------------

Written by Boubli, need english check because it's not my native
langage, and corrections/addings are welcome.
