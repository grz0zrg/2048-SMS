; collision detection for two rectangle
; use hl,de,bc and de'
; x1/w1 -> h/l
; x2/w2 -> d/e
; y1/h1 -> b/c
; y2/h2 -> d'/e'
rectCollision:
	;ex af,af'
		ld a,h
		add a,l
		cp d ; if (x1 + w1) < x2
		jr nc,+
		ld a,d
		add a,e
		cp h ; if (x2 + w2) < x1
		jr nc,+
		ld a,b
		add a,c
		exx
		cp d ; if (y1 + h1) < y2
		jr nc,+
		ld a,d
		add a,e
		exx
		cp b ; if (y2 + h2) < y1
	+:
	;ex af,af'
	ret
	
ClrPSG:
	ld a,$9f
	out ($7f),a
	ld a,$bf
	out ($7f),a
	ld a,$df
	out ($7f),a
	ld a,$ff
	out ($7f),a
	ret

; put sprite list terminator at location pointed by hl
PutSpriteTerminator:
	ex af,af'
		ld a,l
		out ($bf),a
		ld a,h
		or $40
		out ($bf),a
		ld a,$D0
		out ($be),a
	ex af,af'
	ret
	
; disable all sprites starting at the location pointed by hl (output $D0 to y pos)
DisableSprites:
	call VRAMToHL
	ld b,$40
	-:
		ld a,l
		out ($bf),a
		ld a,h
		or $40
		out ($bf),a
		ld a,$D0
		out ($be),a
		inc hl
	djnz -
	ret

; setup all 11 VDP registers in one go from data at (hl)
SetupVDP
	push af
	push bc
	ld b,0
	ld c,11
		-:
			ld a,(hl)
			out ($bf),a
			ld a,b
			or $80
			out ($bf),a
			inc hl
			inc b
			ld a,b
			cp c
			jp nz,-
	pop bc
	pop af
	ret
	
VRAMToHL:
	push af
		ld a,l
		out ($bf),a
		ld a,h
		or $40
		out ($bf),a
	pop af
	ret
	
CRAMToHL:
	push af
		ld a,l
		out ($bf),a
		ld a,$C0
		out ($bf),a
	pop af
	ret

; put a value (located in b) in vram
PutVRAM:
	ld a,b
	out ($be),a
	ret
	
WriteVRAM:
	push af
	-:	
		ld a,(hl)
		out ($be),a
		inc hl
		dec bc
		ld a,c
		or b
		jp nz,-
	pop af
	ret

WriteToHL:
	push af
	push bc 
	push hl
	push de
		-:
			ld a,(hl)
			ld (de),a
			inc hl
			inc de
			dec b
			jp nz,-
	pop de
	pop hl 
	pop bc 
	pop af
	ret	

ClrVRAM:
	push af
	push bc
		ld bc,$4000
		ld a,$00
		-:
			out ($be),a
		    dec c
		    jp nz,-
		    dec b 
		    jp nz,- 
	pop bc
	pop af
	ret
	
ClrRAM:
	push af
	push bc
		ld hl,$C000
		ld bc,$1FFF
		ld a,$00
		-:
			ld (hl),a
			inc hl
		    dec c
		    jp nz,-
		    dec b 
		    jp nz,- 
	pop bc
	pop af
	ret
	
WaitVBL:
	push af
	-:
		in a,($BF)
		and $80
		jp z,-
	pop af
	ret
	
; taken from : http://www.msx.org/forum/development/msx-development/bcdhex-conversion-asm
;======================================================================
;	 a(BIN) =>  a(BCD) 
;	[0..99] => [00h..99h]
;======================================================================
bin2bcd:
	push	bc
	ld	c, a
	ld	b, 8
	xor	a
-:
	sla	c
	adc	a, a
	daa
	djnz -
	pop	bc
	ret
	
;======================================================================
; putText
;
; Input: de = null terminated text data
;        hl = position adress in the tilemap
;======================================================================
putText:
	;call VRAMToHL
	ld a,l
	out ($bf),a
	ld a,h
	or $40
	out ($bf),a
		
	dec de
	jp +
	_putCharacter:
		ld b,33 ; font is starting at space character
		sub b
		
		add a,c
		out ($be),a
		ld a,9 ; 1 for first palette - 9 to use the second
		out ($be),a
			
		+:
		scf
		ccf
		ld hl,const_font_start
		ld bc,$00FF
		sbc hl,bc
		
		ld c,l

		inc de
		ld a,(de)
		or a
	jr nz,_putCharacter
	ret
	
;======================================================================
; putCharacter
;
; Input: de = null terminated text data
;        hl = position adress in the tilemap
;======================================================================
putCharacter:
	push af
	push hl
	ld hl,const_font_start
	ld bc,$00FF
	scf
	ccf
	sbc hl,bc
		
	ld c,l
		
	ld b,33 ; font is starting at space character
	sub b
	add a,c
	
	ex de,hl
	;call VRAMToHL
	push af
		ld a,l
		out ($bf),a
		ld a,h
		or $40
		out ($bf),a
	pop af
	
	;add a,c
	out ($be),a
	ld a,9
	out ($be),a
	ex de,hl
	pop hl
	pop af
	inc de
	inc de
	ret
	
displayOn:
	ld a,(VDP_DATA+1)
	or $40
	out ($bf),a
	ld a,$81
	out ($bf),a	
	ret
		
displayOff:
	ld a,(VDP_DATA+1)
	out ($bf),a
	;--- 12 t state to be secure with the 28 state during active display
	inc hl
	dec hl
	;---
	ld a,$81
	out ($bf),a	
	ret
		
;============================================
;
; Rand16
;
; 16-bit Random Number Generator
; Input: DE = seed
; Output: HL = pseudo-random number, period 65536
;
;=============================================
Rand16:
    ld a,d
    ld h,e
    ld l,253
    or a
    sbc hl,de
    sbc a,0
    sbc hl,de
    ld d,0
    sbc a,d
    ld e,a
    sbc hl,de
    jr nc,Rand
    inc hl
Rand:
;    ld (Seed),hl
	ret