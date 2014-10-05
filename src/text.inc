HIGHSCORE_DATA:
.db "HIGHSCORE" $00
	
SCORE_DATA:
.db "SCORE" $00

SCORE_DIGITS_DATA:
.db "00000000" $00

WIN:
.db "You win!" $00

WIN_HELP:
.db "Press button 2,reach 8192 now!" $00
	
LOSE:
.db "You lose!" $00

; intro screen
PRESS_START:
.db "Press START to play." $00
	
BASED:
.db "based on" $00
CREDIT:
.db "http://git.io/2048" $00
AUTHOR:
.db "by Grz-" $00

; help screen
HELP_L1:
.db "The Dpad" $00
HELP_L2:
.db "move the tiles" $00
HELP_L3:
.db "START button" $00
HELP_L4:
.db "restart the game" $00
HELP_L5:
.db "Same tiles" $00
HELP_L6:
.db "add together" $00
HELP_L7:
.db "Main goal is" $00
HELP_L8:
.db "to reach 2048." $00
HELP_L9:
.db "If you win" $00
HELP_L10:
.db "another goal" $00
HELP_L11:
.db "will appear!" $00
HELP_L12:
.db "Press START." $00