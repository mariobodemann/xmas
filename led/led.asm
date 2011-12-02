.ORG 0000
RJMP Start

.EQU PORTB, 0x18

Start:
	LDI R16, 255 ; load 0xFF to register 16

Loop:
	OUT PORTB, R16 ; output register 16 to port b

	NOP ; wait some time
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

	COM R16	; invert register 16 (0xff -> 0; 0 -> 0xff)
RJMP Loop ; do it again sam!
