; NAME: Buy a Star (Airsola™)
; GAMES: MP3_USA
; EXECUTION: Direct
; PARAM: +Number|Price



;====================================================================
; IMPORTANT: This code is specifically designed to allow the landing
;            or passing player to purchase a Star at a cost of your
;            choice. It includes the "Star Get" jingle and dance
;            animation for your character. Make sure you change the
;            music track it plays near the end of the code to match
;            the one selected for your board.
;===================================================================



addiu SP, SP, -0x60
sw    RA, 0x34(SP)
SW S4 24(SP)
SW S1 40(SP)
SW S2 36(SP)
SW S3 28(SP)
SW S0 32(SP)



;BEING CHECKING PLAYER FOR NECESSARY COINS============================
JAL GetCurrentPlayerIndex ; Current Player Index at V0
NOP

MOVE S4 V0 ; Copied Current Player Index to S4
MOVE A0 S4 ; Copied Current Player Index to A0
LI A1 Price ; Amount of coins to check for
JAL PlayerHasCoins ; Checks if Player has A1's amount of Coins
NOP

BEQ V0 R0 NotEnoughCoins ; If insufficient funds, branch
NOP
;END CHECKING PLAYER FOR NECESSARY COINS==============================

;BEGIN CHECKING IF PLAYER CAN CARRY A STAR============================
LUI S1 hi(p1_stars)
ADDIU S1 S1 lo(p1_stars) ; Load Player 1's Star address
LI S3 0x38 ; This is the distance between each player's Star address
MULT S4 S3 ;  Multiply Current Player Index by S3
MFLO S3 ; Move the result to S3

ADDU S1 S1 S3 ; Add result to get current Player's Star address
LBU S2 0(S1) ; Load address value into S2

LI T0 0x63 ; 99
BEQ S2 T0 TooManyStars ; If the player has 99 Stars, exit
NOP
;END CHECKING IF PLAYER CAN CARRY A STAR==============================

;BEGIN LOADING STRING VALUES FOR PROMPT===============================
LUI S0 hi(p1_char)
ADDIU S0 S0 lo(p1_char) ; Load Player 1's Character address
ADDU S0 S0 S3 ; Add result from above to S0
LBU S2 0(S0) ; Load the value address value of S0 into S2

LI T0 0x1c00 ; Load value of MARIO string to T0
ADDU S2 S2 T0 ; Add S2 to the string value to obtain the character

LUI A0 hi(coin_string_loc)
ADDIU A0 A0 lo(coin_string_loc) ; Parse Destination
LUI A1 hi(percent_d)
ADDIU A1 A1 lo(percent_d) ; String Format
JAL sprintf
LI A2 Price ; Convert Price into string
;END LOADING STRING VALUES FOR PROMPT=================================

;BEGIN PROMPT=========================================================
START:
SW R0 16(SP) ; A4
SW R0 20(SP) ; A5
SW R0 24(SP) ; A6
LI A0 3 ; Character image (Toad)
LUI A1 hi(promptMessage)
ADDIU A1 A1 lo(promptMessage)
MOVE A2 S2 ; Pass Character Name to A2
LUI A3 hi(coin_string_loc)
ADDIU A3 A3 lo(coin_string_loc) ; Pass Star Price to A3
JAL 0x800EC8EC ; ShowMessage
NOP

; Get the selection, either from the player or CPU.
; If A0 is a pointer to AI data, AI logic is ran to pick for CPUs.
; If A0 is 0 or 1, the 0th or 1st option is chosen by CPUs.
; If A0 is 2, then the value of A1 is the CPUs option index choice.
LI A0 0
JAL GetBasicPromptSelection
LI A1 0
MOVE S0 V0 ; S0 now has the chosen option index

; Obligatory message box closing/cleanup calls.
JAL 0x800EC6C8
NOP
JAL 0x800EC6EC
NOP

; Change the outcome based on the choice.
LI A0 0
BEQ S0 A0 yes
NOP
LI A0 1
BEQ S0 A0 Declined
NOP
LI A0 2
BEQ S0 A0 view_map
NOP
;END PROMPT===========================================================


;BEGIN Taking Coins From Player=======================================
yes:
LI S0 Price

MOVE A0 S4 ; Current Player's Index moved to A0
SUBU A1 R0 S0 ; Amount to pay
JAL AdjustPlayerCoinsGradual ; Changes Player's coin amount
NOP

MOVE A0 S4 ; Current Player's Index moved to A0
SUBU A1 R0 S0 ; Amount to display
JAL ShowPlayerCoinChange ; Displays Player's coin loss on-screen 
NOP

ADDIU A0 R0 35
JAL SleepProcess ; Sleeps for 35 frames
NOP
;END Taking Coins From Player=========================================

;BEGIN GIVING STAR/JINGLE/ANIMATION===================================
jal   0x8004A520 ; PlayMusic
 li    A0, 111 ; Star Get Jingle

lui   A0, hi(current_player_index)
addiu A0, A0, lo(current_player_index)
lb    V0, 0(A0)
sll   V1, V0, 3
subu  V1, V1, V0
sll   V1, V1, 3
lui   V0, hi(p1_stars)
addu  V0, V0, V1
lbu   V0, lo(p1_stars)(V0)
addiu V0, V0, 1 ; add 1 star
lui   AT, hi(p1_stars)
addu  AT, AT, V1
sb    V0, lo(p1_stars)(AT)
lb    V1, 0(A0)
sll   V0, V1, 3
subu  V0, V0, V1
sll   V1, V0, 3
lui   V0, hi(p1_stars)
addu  V0, V0, V1
lb    V0, lo(p1_stars)(V0)
slti  V0, V0, 0x64
bnez  V0, L80106E30
 li    A0, -1
li    V0, 99
lui   AT, hi(p1_stars)
addu  AT, AT, V1
sb    V0, lo(p1_stars)(AT)
L80106E30:
li    A1, 6
jal   0x800F2304
 move  A2, R0
lui   A1, hi(current_player_index)
lb    A1, lo(current_player_index)(A1)
jal   0x8004ACE0
 li    A0, 610
lui   V0, hi(D_8011D308)
lw    V0, lo(D_8011D308)(V0)
beqz  V0, L80106EA8
       NOP
jal   SleepProcess
 li    A0, 16 ; SLEEP FOR 16 FRAMES
lui   A0, hi(0x800CDBC8)
jal   0x80003A70
 lh    A0, lo(0x800CDBC8)(A0)
jal   0x8004A918
 li    A0, 111
jal   0x8004A880
 move  A0, R0
jal   SleepProcess
 li    A0, 110
lui   A0, hi(0x800CDBC8)
lh    A0, lo(0x800CDBC8)(A0)
jal   0x80003B70
 move  A1, R0
jal   0x8004A72C
 li    A0, 15
j     wait
       NOP
L80106EA8:
jal   SleepProcess
 li    A0, 60

wait:
jal   SleepProcess
 li    A0, 50
 
jal   0x8004A520 ; PlayMusic
 li    A0, 28; <--- Replace with the Index of your board's music.
             ;      Use PartyPlanner's Audio player feature to find
             ;      the correct music index.
 J exit
NOP
;END GIVING STAR/JINGLE/ANIMATION===================================

view_map:
JAL ViewBoardMap
NOP
J START
NOP

;BEGIN CANNOT BUY STAR MESSAGE========================================
Declined:
SW R0 16(SP) ; A4
SW R0 20(SP) ; A5
SW R0 24(SP) ; A6
ADDI A0 R0 0x3 ; Character image (Toad)
LUI A1 hi(Declined_Message)
ADDIU A1 A1 lo(Declined_Message)

J FinalizeMessage
NOP
;END CANNOT BUY STAR MESSAGE==========================================

;BEGIN CANNOT BUY STAR MESSAGE========================================
TooManyStars:
JAL PlaySound
LI A0 0x29A ; Toad: Oooh... :(

SW R0 16(SP) ; A4
SW R0 20(SP) ; A5
SW R0 24(SP) ; A6
ADDI A0 R0 0x3 ; Character image (Toad)
LUI A1 hi(TooManyStars_Message)
ADDIU A1 A1 lo(TooManyStars_Message)

J FinalizeMessage
NOP
;END CANNOT BUY STAR MESSAGE==========================================

;BEGIN CANNOT BUY STAR MESSAGE========================================
NotEnoughCoins:
JAL PlaySound
LI A0 0x29A ; Toad: Oooh... :(

SW R0 16(SP) ; A4
SW R0 20(SP) ; A5
SW R0 24(SP) ; A6
ADDI A0 R0 0x3 ; Character image (Toad)
LUI A1 hi(NotEnoughCoins_Message)
ADDIU A1 A1 lo(NotEnoughCoins_Message)

J FinalizeMessage
NOP
;END CANNOT BUY STAR MESSAGE==========================================

FinalizeMessage:
ADDU A2 R0 R0
JAL 0x800EC8EC ; ShowMessage
ADDU A3 R0 R0

JAL 0x800EC9DC
NOP
JAL 0x800EC6C8
NOP
JAL 0x800EC6EC
NOP



exit:
LW S0 32(SP)
LW S3 28(SP)
LW S1 40(SP)
LW S2 36(SP)
LW S4 24(SP)
lw    RA, 0x34(SP)
jr    RA
 addiu SP, SP, 0x60
 
 percent_d:
.asciiz "%d" ; 0x25640000

coin_string_loc:
.fill 8
 
 D_8011D308:
.word 0x00000000

.align 16
NotEnoughCoins_Message:
.byte 0x1A,0x1A,0x1A,0x1A ; Standard padding for picture
.ascii "Oh dear"
.byte 0x85,0x85,0x85 ; Period (...)
.byte 0x0A ; New Line (Writes Below)
.byte 0x1A,0x1A,0x1A,0x1A ; Standard padding for picture
.ascii "It seems you don"
.byte 0x5C ; Apostrophe (')
.ascii "t have enough coins"
.byte 0x85,0x85,0x85 ; Period (...)
.byte 0x0A ; New Line (Writes Below)
.byte 0x1A,0x1A,0x1A,0x1A ; Standard padding for picture
.ascii "Maybe next time"
.byte 0xC2 ; Exclamation Mark (!)
.byte 0xFF,0 ; FF=Pause

.align 16
TooManyStars_Message:
.byte 0x1A,0x1A,0x1A,0x1A ; Standard padding for picture
.ascii "Nice to see y"
.byte 0x3D ; minus (-)
.byte 0x0A ; New Line (Writes Below)
.byte 0x1A,0x1A,0x1A,0x1A ; Standard padding for picture
.ascii "Whoa"
.byte 0xC2,0xC2,0xC2 ; Exclamation Mark (!!!)
.ascii " You can"
.byte 0x5C ; Apostrophe (')
.ascii "t carry any more stars"
.byte 0xC2 ; Exclamation Mark (!)
.byte 0x0A ; New Line (Writes Below)
.byte 0x1A,0x1A,0x1A,0x1A ; Standard padding for picture
.ascii "I guess I"
.byte 0x5C ; Apostrophe (')
.ascii "ll see you later"
.byte 0xC3 ; Question Mark (?)
.byte 0xFF,0 ; FF=Pause

.align 16
Declined_Message:
.byte 0x1A,0x1A,0x1A,0x1A ; Standard padding for picture
.ascii "Wait"
.byte 0xC2 ; Exclamation Mark (!)
.ascii " Are you sure"
.byte 0xC3 ; Question Mark (?)
.byte 0x0A ; New Line (Writes Below)
.byte 0x1A,0x1A,0x1A,0x1A ; Standard padding for picture
.ascii "Nobody"
.byte 0x5C ; Apostrophe (')
.ascii "s ever said no before"
.byte 0x85,0x85,0x85 ; Period (...)
.byte 0x0A ; New Line (Writes Below)
.byte 0x1A,0x1A,0x1A,0x1A ; Standard padding for picture
.ascii "That"
.byte 0x5C ; Apostrophe (')
.ascii "s new"
.byte 0x85,0x85,0x85 ; Period (...)
.ascii " See ya"
.byte 0xC2 ; Exclamation Mark (!)
.byte 0xFF,0 ; FF=Pause

.align 16
promptMessage:
.byte 0x0B ; Start the message
.byte 0x1A,0x1A,0x1A,0x1A ; Standard padding for picture
.byte 0x11 ; Player Character's Name
.byte 0x82 ; Comma (,)
.ascii " you made it"
.byte 0xC2 ; "!"
.byte 0x0A ; Newline
.byte 0x1A,0x1A,0x1A,0x1A ; Padding for picture
.ascii "Would you like to trade"
.byte 0x0A ; Newline
.byte 0x1A,0x1A,0x1A,0x1A ; Padding for picture
.byte 0x06 ; Blue font
.byte 0x12 ; Star Price
.ascii " Coins "
.byte 0x08 ; White Font
.ascii "for a "
.byte 0x07 ; Yellow Font
.ascii "Star"
.byte 0x08 ; White Font
.byte 0xC3 ; "?"
.byte 0x0A ; Newline
.byte 0x1A,0x1A,0x1A,0x1A,0x1A,0x1A ; Little more for option indent
.byte 0x0C ; Start option
.ascii "Of course"
.byte 0xC2 ; Exclamation Mark (!)
.byte 0x0D ; End option
.byte 0x0A ; Newline
.byte 0x1A,0x1A,0x1A,0x1A,0x1A,0x1A 
.byte 0x0C ; Start option
.ascii "Nope"
.byte 0xC2 ; Exclamation Mark (!)
.byte 0x0D ; End option
.byte 0x0A ; Newline
.byte 0x1A,0x1A,0x1A,0x1A,0x1A,0x1A 
.byte 0x0C ; Start option
.ascii "View Map"
.byte 0x0D ; End option
.byte 0