# SPECTRUM

## Arduino Pro Mini LED PWM Controller:

Basic Function:

 * Reads a potentiometre ( aka cheap rotary encoder )
 * Map potentiometre reading to a Hue
 * Signal filtering & Logic
 * Converts Hue, Sat, Brightness to RGB
 * Make Beautiful & playfull light for Sydney VIVId 2016


Deployed for Sydney Vivid 2016 artwork "Spectrum"

To Do:

 * IP67/68 Connectors -> alibaba led 4pin connectors
 * Calibration TrimPot on board + code + new board
 * Smaller Gauge Cable !!!!!!!
 * Beautify Hue Variation
 * WS281'ing the setup
 * Install a webcam so I can see it from afar.
 * Idle Mode ?

And here's some code!

```CPP
$(function(){
  $('div').html('I am a div.');
});
```

Check the work out  [on Vivid Website](http://www.vividsydney.com/event/light/spectrum) it's the render view, hopefully Vivid changes it sometime.


Shot to all the ressources online, great mind sharing amazing code.


### Stuff used to make this:

 * [markdown-it](https://github.com/markdown-it/markdown-it) for Markdown parsing
 * [CodeMirror](http://codemirror.net/) for the awesome syntax-highlighted editor
 * [highlight.js](http://softwaremaniacs.org/soft/highlight/en/) for syntax highlighting in output code blocks
 * [js-deflate](https://github.com/dankogai/js-deflate) for gzipping of data to make it fit in URLs
