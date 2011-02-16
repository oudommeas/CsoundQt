see http://booki.flossmanuals.net/csound/

<CsoundSynthesizer>; START OF A CSOUND FILE

<CsOptions> ; CSOUND CONFIGURATION
-odac
</CsOptions>

<CsInstruments> ; INSTRUMENT DEFINITIONS GO HERE
;Example by Alex Hofmann, Andrés Cabrera and Joachim Heintz
; Set the audio sample rate to 44100 Hz
sr = 44100

instr 1
; a 440 Hz Sine Wave
aSin      oscils    0dbfs/4, 440, 0
          out       aSin
endin
</CsInstruments>

<CsScore> ; SCORE EVENTS GO HERE
i 1 0 1
</CsScore>

</CsoundSynthesizer> ; END OF THE CSOUND FILE
; Anything after is ignored by Csound
<bsbPanel>
 <label>Widgets</label>
 <objectName/>
 <x>630</x>
 <y>260</y>
 <width>380</width>
 <height>205</height>
 <visible>true</visible>
 <uuid/>
 <bgcolor mode="background">
  <r>230</r>
  <g>140</g>
  <b>36</b>
 </bgcolor>
</bsbPanel>
<bsbPresets>
</bsbPresets>
<MacGUI>
ioView background {59110, 35980, 9252}
</MacGUI>