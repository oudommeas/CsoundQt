<CsoundSynthesizer>

; Id: A06_ENV03.CSD mg (2006, rev.2009)
; author: marco gasperini (marcogsp at yahoo dot it)

; G.M. Koenig
; ESSAY (1957)

<CsOptions>
-W -f -oA06_ENV03.wav
</CsOptions>

<CsInstruments>

sr     = 192000
kr     = 192000
ksmps  = 1
nchnls = 1

;=============================================
; 215.3 INTENSITY CURVES
;=============================================
	instr 1
iamp1	= ampdb(p4)
iamp2	= ampdb(p5)

ifile 	= p6

a1	diskin2 ifile, 1

aenv	expseg iamp1,1.3,iamp2,p3-1.3,iamp2	
att	linen 1 , .01 , p3 , .01

aout	=  a1*aenv *att

	out aout
	endin
;=============================================

</CsInstruments>
<CsScore>
t0	4572		; 76.2 cm/sec. tape speed (durations in cm)

;			p4	p5	p6
;			iamp1	iamp2	ifile
i1	0	3459	-24	-16	"A05_REV03.wav"	

e

</CsScore>
</CsoundSynthesizer>