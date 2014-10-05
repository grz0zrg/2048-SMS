; ======= data load =======
loadFont:
	; store font data at the end
	ld hl,addr_font_position
	call VRAMToHL
	ld hl,FontData
	ld bc,FontDataEnd-FontData 
	WriteTilesLoop:
		ld a,(hl)
		out ($be),a
		xor a
		out ($be),a
		out ($be),a
		out ($be),a
		inc hl
		dec c
		jp nz,WriteTilesLoop
		dec b
		jp nz,WriteTilesLoop
	ret
	
loadBoardTiles:
	ld de,$4000
	ld hl,psntile
	call LoadTiles4BitRLENoDI

	ld de,$4200
	ld hl,psn1
	call LoadTiles4BitRLENoDI

	ld de,$4400
	ld hl,psn2
	call LoadTiles4BitRLENoDI

	ld de,$4600
	ld hl,psn3
	call LoadTiles4BitRLENoDI

	ld de,$4800
	ld hl,psn4
	call LoadTiles4BitRLENoDI

	ld de,$4A00
	ld hl,psn5
	call LoadTiles4BitRLENoDI

	ld de,$4C00
	ld hl,psn6
	call LoadTiles4BitRLENoDI

	ld de,$4E00
	ld hl,psn7
	call LoadTiles4BitRLENoDI

	ld de,$5000
	ld hl,psn8
	call LoadTiles4BitRLENoDI

	ld de,$5200
	ld hl,psn9
	call LoadTiles4BitRLENoDI

	ld de,$5400
	ld hl,psn10
	call LoadTiles4BitRLENoDI

	ld de,$5600
	ld hl,psn11
	call LoadTiles4BitRLENoDI

	ld de,$5800
	ld hl,psn12
	call LoadTiles4BitRLENoDI

	ld de,$5A00
	ld hl,psn13
	call LoadTiles4BitRLENoDI
	ret
	
loadIntroBackground:
	; palette
	ld hl,$C000
	call CRAMToHL

	ld hl,BackgroundIntroPal
	ld b,BackgroundIntroPalEnd-BackgroundIntroPal
	ld c,$be
	otir
		
	ld hl,$C010
	call CRAMToHL

	ld hl,SpritesIntroPal
	ld b,SpritesIntroPalEnd-SpritesIntroPal
	ld c,$be
	otir

	ld de,$4000
	ld hl,pslogo
	call LoadTiles4BitRLENoDI

	ld hl,pslogomap
	ld de,$3800|$4000
	call LoadTilemapToVRAM
	ret
	
loadBackground:
	call loadPalettes
		
	ld de,const_background_tiles_adress
	ld hl,psboard
	call LoadTiles4BitRLENoDI
		
	call loadBackgroundTileMap
	ret
	
loadBackground8192:
	call loadPalettes
		
	ld de,const_background_tiles_adress
	ld hl,psboard8192
	call LoadTiles4BitRLENoDI
		
	call loadBackground8192TileMap
	ret
	
loadBackgroundTileMap:
	ld hl,$3800
	call VRAMToHL

	ld hl,BackgroundMap
	ld bc,BackgroundMapEnd-BackgroundMap
	call WriteVRAM
	ret
	
loadBackground8192TileMap:
	ld hl,$3800
	call VRAMToHL

	ld hl,Background8192Map
	ld bc,Background8192MapEnd-Background8192Map
	call WriteVRAM
	ret
	
loadPalettes:
	; palette
	ld hl,$C000
	call CRAMToHL

	ld hl,BackgroundPal
	ld b,BackgroundPalEnd-BackgroundPal
	ld c,$be
	otir
		
	ld hl,$C010
	call CRAMToHL

	ld hl,SpritesPal
	ld b,SpritesPalEnd-SpritesPal
	ld c,$be
	otir
	ret
	
loadHelpPalettes:
	ld hl,$C010
	call CRAMToHL

	ld hl,SpritesIntroPal
	ld b,SpritesIntroPalEnd-SpritesIntroPal
	ld c,$be
	otir
ret

; ======= data =======
BackgroundPal:
.db $00 $24 $15 $3F $2A $32 $38 $3C $0A $06 $16 $2B $27 $0B $03 $08
BackgroundPalEnd:

BackgroundIntroPal:
.db $00 $3F
BackgroundIntroPalEnd:

SpritesIntroPal:
.db $00 $3f $00
SpritesIntroPalEnd:

SpritesPal:
.db $2A $15 $00
SpritesPalEnd:

.enum $C000
TileMapData: dsb 32*24
.ende

.include "font.inc"	
.include "text.inc"

; score/value of each tiles based on their index (0 to 8192)
BoardTilesValue:
.dw $0000 $0002 $0004 $0008 $0010 $0020 $0040 $0080 $0100 $0200 $0400 $0800 $1000 $2000
	
BackgroundMap:
.dw $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0
.dw $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1
.dw $00E1 $00E1 $00E1 $00E2 $00E3 $00E4 $00E1 $00E1 $00E5 $00E6 $00E7 $00E1 $00E1 $00E8 $00E9 $00E9 $00E9 $00E9 $00E9 $00E9 $00E9 $00E9 $00E9 $00E9 $00E9 $00E9 $00E9 $00E9 $00E9 $00E9 $02E8 $00E1
.dw $00E1 $00E1 $00EA $00EB $00EC $00ED $00EE $00EF $00F0 $00EC $00F1 $00F2 $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $00F5 $00F6 $00E1 $00F7 $00F8 $00F9 $00FA $00E1 $00FB $00FC $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $00FD $00FE $00FF $0100 $0101 $00E1 $0102 $0103 $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $0104 $0105 $06E8 $0100 $0106 $00E1 $0107 $0108 $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $00E1 $0109 $010A $010B $00E1 $010C $010D $00E1 $010E $010F $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $00EA $0110 $0111 $00E3 $0112 $0113 $0114 $0115 $0116 $0117 $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $0118 $0119 $0119 $0119 $011A $00E1 $011B $011C $011D $06E8 $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $00E1 $011E $02E8 $00E1 $011F $0120 $02E5 $00E1 $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $0121 $0122 $0123 $0124 $0125 $0126 $0127 $0128 $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $00E1 $0129 $012A $012B $0123 $012C $012D $00E1 $012E $012F $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $00E1 $0130 $0131 $0132 $0123 $0133 $0134 $0135 $0136 $0137 $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $0138 $0139 $013A $0132 $0123 $013B $013C $013D $013E $013F $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $0140 $0141 $0142 $0143 $0144 $0145 $0146 $00E1 $0147 $0103 $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $0148 $0149 $0149 $014A $014B $014C $014D $014E $014F $0150 $00E1 $00F3 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $00F4 $02F3 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $00E1 $0151 $0152 $04FD $0153 $0154 $0155 $0156 $00E1 $04E8 $04E9 $04E9 $04E9 $04E9 $04E9 $04E9 $04E9 $04E9 $04E9 $04E9 $04E9 $04E9 $04E9 $04E9 $04E9 $04E9 $06E8 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1
.dw $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0 $04E0
BackgroundMapEnd:

Background8192Map:
.dw $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0
.dw $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1
.dw $00E1 $00E1 $00E1 $00E2 $00E3 $00E4 $00E1 $00E1 $00E1 $00E5 $00E6 $00E1 $00E1 $00E7 $00E8 $00E8 $00E8 $00E8 $00E8 $00E8 $00E8 $00E8 $00E8 $00E8 $00E8 $00E8 $00E8 $00E8 $00E8 $00E8 $02E7 $00E1
.dw $00E1 $00E1 $00E9 $00EA $00EB $00EC $00ED $00E1 $00EE $00EF $00F0 $00E1 $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $00F2 $00F3 $00E1 $00F4 $00F5 $00F6 $00F7 $00F8 $00F0 $00E1 $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $00F9 $00FA $00FB $00FC $00FD $00E1 $06E7 $00FE $00F0 $00E1 $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $00FF $0100 $0101 $0102 $0103 $00E1 $00E1 $00FE $00F0 $00E1 $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $0104 $0105 $00E1 $0106 $0107 $00E1 $00E1 $00FE $00F0 $00E1 $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $0108 $0109 $010A $010B $010C $00E1 $00E1 $00FE $00F0 $00E1 $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $010D $010E $010F $0110 $06E7 $00E1 $00E1 $0111 $0112 $00E1 $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $00E1 $0113 $0114 $00E4 $00E1 $00E1 $0113 $0115 $0116 $00E1 $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $0117 $0118 $0119 $011A $011B $011C $011D $011E $011F $0120 $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $0121 $0122 $00E1 $0123 $0124 $0125 $0126 $00E1 $0523 $0127 $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $0128 $0129 $00E1 $012A $012B $00E1 $00E1 $050D $012C $012D $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $012E $012F $0130 $0131 $0132 $00E1 $00E1 $0133 $0134 $00E1 $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $0135 $0136 $00E1 $00E1 $0137 $0138 $0139 $00E1 $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $00E1 $013A $013B $04E6 $00E1 $011C $013C $013D $0115 $013E $00E1 $00F1 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $02F1 $00E1
.dw $00E1 $00E1 $00E1 $013F $0140 $00E1 $00E1 $0141 $0142 $0142 $0142 $0143 $00E1 $04E7 $04E8 $04E8 $04E8 $04E8 $04E8 $04E8 $04E8 $04E8 $04E8 $04E8 $04E8 $04E8 $04E8 $04E8 $04E8 $04E8 $06E7 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1
.dw $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1 $00E1
.dw $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0 $00E0
Background8192MapEnd:

psboard:
.incbin "gfx/compressed/board.pscompr"
psboard8192:
.incbin "gfx/compressed/board8192.pscompr"
pslogo:
.incbin "gfx/compressed/logo.pscompr"
pslogomap:
.incbin "gfx/compressed/logo_map.pscompr"
psn1:
.incbin "gfx/compressed/n1.pscompr"
psn1map:
.incbin "gfx/compressed/n1_map.pscompr"
psn2:
.incbin "gfx/compressed/n2.pscompr"
psn2map:
.incbin "gfx/compressed/n2_map.pscompr"
psn3:
.incbin "gfx/compressed/n3.pscompr"
psn3map:
.incbin "gfx/compressed/n3_map.pscompr"
psn4:
.incbin "gfx/compressed/n4.pscompr"
psn4map:
.incbin "gfx/compressed/n4_map.pscompr"
psn5:
.incbin "gfx/compressed/n5.pscompr"
psn5map:
.incbin "gfx/compressed/n5_map.pscompr"
psn6:
.incbin "gfx/compressed/n6.pscompr"
psn6map:
.incbin "gfx/compressed/n6_map.pscompr"
psn7:
.incbin "gfx/compressed/n7.pscompr"
psn7map:
.incbin "gfx/compressed/n7_map.pscompr"
psn8:
.incbin "gfx/compressed/n8.pscompr"
psn8map:
.incbin "gfx/compressed/n8_map.pscompr"
psn9:
.incbin "gfx/compressed/n9.pscompr"
psn9map:
.incbin "gfx/compressed/n9_map.pscompr"
psn10:
.incbin "gfx/compressed/n10.pscompr"
psn10map:
.incbin "gfx/compressed/n10_map.pscompr"
psn11:
.incbin "gfx/compressed/n11.pscompr"
psn11map:
.incbin "gfx/compressed/n11_map.pscompr"
psn12:
.incbin "gfx/compressed/n12.pscompr"
psn12map:
.incbin "gfx/compressed/n12_map.pscompr"
psn13:
.incbin "gfx/compressed/n13.pscompr"
psn13map:
.incbin "gfx/compressed/n13_map.pscompr"
psntile:
.incbin "gfx/compressed/tile.pscompr"
psntilemap:
.incbin "gfx/compressed/tile_map.pscompr"
