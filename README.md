NotNotchFilter
==============

NotNotchFilter is a performance-oriented filter designed to replace the mid-EQ
found in a standard 3-band DJ mixer. The key advantage of this filter is that
it cleanly cuts out single voices or instruments in a track, whereas a
standard 3-band filter dampens them. This is because NotNotchFilter, as the
name suggests, is not actually a notch filter. Rather, it is a combination of
a hipass and lopass filter which work on opposite sides of the target
frequency.

![NotNotchFilter User Interface](http://static.teragonaudio.com/ta_notnotchfilter.png)


Usage
-----

For optimal use, use NotNotchFilter with two knobs, one to control the base
frequency and the other to control the valley size. The "valley size" measures
how far apart the hipass and lopass filter are, so at 0 Hz the filter will be
100% dry, and at 20 kHz it will cut almost all sound. In the above screenshot,
the filter will produce a gap of approximately 351 Hz centered around the base
frequency of 400 Hz.

If you don't have two spare knobs on your controller of choice, then assign a
single knob to the valley size parameter, and then tweak the base frequency to
something in the mid-hi range. You'll probably need to experiment a bit to
find a good frequency which suits your style of play, but having a spare knob
to control the base frequency is preferred, as this allows one to adjust it on
the fly to filter out a given instrument in the track.


Licensing
---------

NotNotchFilter is licensed under the GPL2 License, which can be found in the
LICENSE.txt file included in the source code.

