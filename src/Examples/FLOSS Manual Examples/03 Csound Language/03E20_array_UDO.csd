<CsoundSynthesizer>
<CsOptions>
--env:SSDIR+=../SourceMaterials -nm128
</CsOptions>
<CsInstruments>
ksmps = 32

  opcode FirstEl, k, k[]
  ;returns the first element of vector kArr
kArr[] xin
xout kArr[0]
  endop

  instr 1
kArr[] array   6, 3, 9, 5, 1
kFirst FirstEl kArr
       printf  "kFirst = %d\n", 1, kFirst
       turnoff
  endin

</CsInstruments>
<CsScore>
i 1 0 .1
</CsScore>
</CsoundSynthesizer>
;example by joachim heintz
