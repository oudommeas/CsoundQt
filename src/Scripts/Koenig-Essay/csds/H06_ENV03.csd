<CsoundSynthesizer>

; Id: H06_ENV03.CSD mg (2006, rev.2009)
; author: marco gasperini (marcogsp at yahoo dot it)

; G.M. Koenig
; ESSAY (1957)

<CsOptions>
-W -f -oH06_ENV03.wav
</CsOptions>

<CsInstruments>

sr     = 192000
kr     = 19200
ksmps  = 10
nchnls = 1

;=============================================
; 285.3 INTENSITY CURVES
;=============================================
	instr 1
iamp	= ampdb(90+p4)/ 29834
ifile	= p5

a1	diskin2 ifile , 1

att	linen iamp , .01 , p3 , .01

aout	=  a1* att

	out aout
	endin
;=============================================

</CsInstruments>
<CsScore>
t0	4572		; 76.2 cm/sec. tape speed (durations in cm)	

;			p4	p5	
;			iamp1	ifile
;			[dB]
i1	0	42.9	-15	"H04_TR03.wav"

e

</CsScore>
</CsoundSynthesizer>