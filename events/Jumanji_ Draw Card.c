// NAME: Jumanji: Draw Card
// GAMES: MP3_USA
// EXECUTION: Direct

#include "ultra64.h"

struct coords3d {
    f32 x;
    f32 y;
    f32 z;
};

struct spacedata {
    s8 pad[8];
    struct coords3d coords;
    s8 pad2[8];
    void *unk24;
};

struct objecttype {
    void *unk0;
    void *unk4;
    s8 pad1[2];
    u16 unka;
    struct coords3d coords;
    void *unk24;
    s8 pad3[8];
    s8 unk36;
    f32 unk40;
    s8 pad4[4];
    f32 unk48;
    f32 funk52;
    f32 funk56;
    void *unk60;
    s8 pad5[6];
    s16 unk70;
};

struct player {
    s8 unk0;
    s8 cpu_difficulty;
    s8 controller;
    u8 character;
    /**
     * Miscellaneous flags.
     * 1: Is CPU player
     */
    u8 flags;
    s8 pad0[5];
    s16 coins; // 10
    s16 minigame_coins; // 12
    s8 stars; // 14

    u8 cur_chain_index; // 15
    u8 cur_space_index; // 16
    u8 next_chain_index; // 17
    u8 next_space_index; // 18
    u8 unk1_chain_index; // 19
    u8 unk1_space_index; // 20
    u8 reverse_chain_index; // 21
    u8 reverse_space_index; // 22

    u8 flags2; // 23
    u8 items[3]; // 24
    u8 bowser_suit_flag; // 27
    u8 turn_color_status; // 28

    s8 pad1[7]; // 29 - 35

    void *obj; // 36 // struct object *
    s16 minigame_star; // 40
    s16 coin_star; // 42
    s8 happening_space_count; // 44
    s8 red_space_count;
    s8 blue_space_count;
    s8 chance_space_count;
    s8 bowser_space_count; // 48
    s8 battle_space_count;
    s8 item_space_count;
    s8 bank_space_count;
    s8 game_guy_space_count; // 52

    // s8 pad2[3];
}; // sizeof == 56

#define ITEM_NONE 				0xFF
#define ITEM_MUSHROOM 			0x00
#define ITEM_SKELETON_KEY 		0x01
#define ITEM_POISON_MUSHROOM 	0x02
#define ITEM_REVERSE_MUSHROOM 	0x03
#define ITEM_CELLULAR_SHOPPER 	0x04
#define ITEM_WARP_BLOCK 		0x05
#define ITEM_PLUNDER_CHEST 		0x06
#define ITEM_BOWSER_PHONE 		0x07
#define ITEM_DUELING_GLOVE 		0x08
#define ITEM_LUCKY_LAMP 		0x09
#define ITEM_GOLDEN_MUSHROOM 	0x0A
#define ITEM_BOO_BELL			0x0B
#define ITEM_BOO_REPELLANT 		0x0C
#define ITEM_BOWSER_SUIT 		0x0D
#define ITEM_MAGIC_LAMP 		0x0E
#define ITEM_KOOPA_CARD 		0x0F
#define ITEM_BARTER_BOX 		0x10
#define ITEM_LUCKY_COIN 		0x11
#define ITEM_WACKY_WATCH 		0x12

#define PLAYER_INDEX			GetCurrentPlayerIndex()


/*Memory Addresses*/
extern u16 D_800CD0B4;	//Koopa Bank Total
extern u8  D_800CD05B;	//Current Turn

extern u16 D_800D1112;	//Player 1 Coins
extern u16 D_800D114A;	//Player 2 Coins
extern u16 D_800D1182;	//Player 3 Coins
extern u16 D_800D11BA;	//Player 4 Coins

extern u8 D_800D110B;	//Player 1 Character
extern u8 D_800D1143;	//Player 2 Character
extern u8 D_800D117B;	//Player 3 Character
extern u8 D_800D11B3;	//Player 4 Character

extern u8 D_800D111F;	//Player 1 Status Flags
extern u8 D_800D1157;	//Player 2 Status Flags
extern u8 D_800D118F;	//Player 3 Status Flags
extern u8 D_800D11C7;	//Player 4 Status Flags

/********************************************************
********************* CARD MESSAGES *********************
********************************************************/

char *msg_mushroom = "\x0B"
    "You find a fungus growing here" "\x82" "\n"
    "Filled with " "\x05" "life" "\x08" "\x82 or full of " "\x03" "fear" "\x08" "" "\xC3" "\xFF";

char *msg_give = "\x0B"
    "The jungle\x5Cs bounty knows no bound" "\x82" "\n"
    "Take this " "\x07" "gold" "\x08" " that you just found" "\xC2"
    "\xFF";

char *msg_take = "\x0B"
    "A tiny serpent finds its prey\x82" "\n"
    "You feel your coins\x82 " "\x03" "stolen away" "\x08" "\xC2"
    "\xFF";

char *msg_communism = "\x0B"
    "To horde gold is selfish\x82 no matter who you are\x82" "\n"
    "All are seen equal\x82 praise the " "\x03" "sickle and star" "\x08" "\xC2" "\xFF";

char *msg_bank = "\x0B"
    "A fabulous treasure\x82 yours to keep\xC2" "\n"
    "Your friends have sowed\x82 and you shall reap\x85\x85\x85" "\xFF";

char *msg_plunder = "\x0B"
    "Take this chest\x82 with which to plunder" "\n"
    "Who will fate\x5Cs hand choose\x82 I wonder\xC3\xFF";

char *msg_purchase = "\x0B"
    "A mystery item\x82 blind to your eye" "\n"
    "For " "\x07" "\x12 Coins" "\x08" "\x82 would you like to buy\xC3" "\n"
    "\x1A\x1A\x1A" // Little more for option indent
    "\x0C" // Start option
    "Of course"
    "\xC2" // !
    "\x0D" // End option
    "\x0A" // Newline
    "\x1A\x1A\x1A"
    "\x0C" // Start option
    "Nope"
    "\xC2" // !
    "\x0D" // End option
    "\x0A" // Newline
    "\x1A\x1A\x1A"
    "\x0C" // Start option
    "View Map"
    "\x0D" // End option
    "\xFF";

char *not_enough_coins_msg = "\x0B"
    "You wanted to swindle me\xC3 Think twice\xC2 " "\n"
    "You\x5Cve angered the game\x82 now " "\x03" "pay the price" "\x08" "\x85\x85\x85"
    "\xFF";

char *msg_nothing_1 = "\x0B"
    "Fate can be altered\x82 as fast as quicksilver" "\n"
    "But fortunately\x82 you get a bunch of\x85\x85\x85" "\xFF";

char *msg_nothing_2 = "\x0B" "\x85\x85\x85" "uh" "\x85\x85\x85" "\xFF";

char *msg_nothing_3 = "\x0B"
    "Weird\x82 this card doesn\x5Ct do anything" "\xFF";

char *msg_possess = "\x0B"
    "Your body is forfeit\x82 you\x5Cve lost control" "\xC2" "\n"
    "Perhaps you\x5Cll get it back \x06next roll\x08\x85\x85\x85" "\xFF";

char *msg_unpossess = "\x0B"
    "You feel the spirit leave you be" "\x85\x85\x85" "\n"
    "Once again\x82 you control your destiny\xC2" "\xFF";

char *msg_nice = "\x0B"
    "This space won\x5Ct give you a chance to roll twice\x82" "\n"
    "But this song\x5Cs music index is " "\x05" "69" "x08" "    (nice) \xFF";

/********************************************************
********************* SUB-FUNCTIONS *********************
********************************************************/

void msgBox(char *message){
    // First argument is character image (-1 for none).
	ShowMessage(-1, message, 0, 0);
    
    // The rest here perform the "wait for confirmation"
	// and messagebox teardown.
    func_800EC9DC();
    CloseMessage();
    func_800EC6EC();
}

int giveItem(u8 givenitem){
    struct player *player = GetPlayerStruct(PLAYER_INDEX);
    u8 open_index = PlayerHasEmptyItemSlot(PLAYER_INDEX);
    
    if(open_index != 0xFF){
        //the item is placed in the first empty slot of player
        player->items[open_index] = givenitem;
        
        //Fix / refresh player item HUDs
        FixUpPlayerItemSlots(PLAYER_INDEX);
        RefreshHUD(PLAYER_INDEX);
        
        return 1;
    }
    return 0;
}

void displayModel(struct objecttype *model_obj){
    struct spacedata *model_space_data;
    model_space_data = GetSpaceData(PLAYER_INDEX);
    
    //Add warp effect
    // This makes the model visible (maybe?)
    func_800ECC54(model_obj);

    // Move the model to the space's coordinates.
    func_80089A20(&model_obj->coords, &model_space_data->coords);
}

void deleteModel(struct objecttype *model_obj){
    // Clean up the model
    func_800D9B54(model_obj);
}

/********************************************************
********************* CARD FUNCTIONS ********************
********************************************************/

void card_communism(){
    //Display card text
    msgBox(msg_communism);
    
    int totalCoins = D_800D1112 + D_800D114A + D_800D1182 + D_800D11BA;
    int newTotal = totalCoins / 4;
    
    AdjustPlayerCoinsGradual(0, -1*D_800D1112);
    AdjustPlayerCoinsGradual(1, -1*D_800D114A);
    AdjustPlayerCoinsGradual(2, -1*D_800D1182);
    AdjustPlayerCoinsGradual(3, -1*D_800D11BA);
    
    SleepProcess(30);
    
    AdjustPlayerCoinsGradual(0, newTotal);
    AdjustPlayerCoinsGradual(1, newTotal);
    AdjustPlayerCoinsGradual(2, newTotal);
    AdjustPlayerCoinsGradual(3, newTotal);
    
    SleepProcess(30);
}

void card_mushroom(){
    struct player *player = GetPlayerStruct(PLAYER_INDEX);
    u8 open_index = PlayerHasEmptyItemSlot(PLAYER_INDEX);
    
    if(open_index != 0xFF){
        u8 mush;
        msgBox(msg_mushroom);
    
        switch(guRandom() % 4){
            case 0:	mush = ITEM_MUSHROOM;			break;
            case 1:	mush = ITEM_POISON_MUSHROOM;	break;
            case 2:	mush = ITEM_REVERSE_MUSHROOM;	break;
            case 3:	mush = ITEM_GOLDEN_MUSHROOM;	break;
        }
    
        //the item is placed in the first empty slot of player
        player->items[open_index] = mush;
        
        //Fix / refresh player item HUDs
        FixUpPlayerItemSlots(PLAYER_INDEX);
        RefreshHUD(PLAYER_INDEX);
    }
    else{
        //No space for item
        card_nothing();
    }
    
    SleepProcess(15);
}


void card_givecoins(){
    int amount = (guRandom() % 10)+5;
    
    msgBox(msg_give);
    AdjustPlayerCoinsGradual(PLAYER_INDEX, amount);
    ShowPlayerCoinChange(PLAYER_INDEX, amount);
    SetBoardPlayerAnimation(-1, 5);
    SleepProcess(30);
}

void card_takecoins(){
    int amount = (guRandom() % 10)+5;
    
    msgBox(msg_take);
    AdjustPlayerCoinsGradual(PLAYER_INDEX, -1*amount);
    ShowPlayerCoinChange(PLAYER_INDEX, -1*amount);
    SetBoardPlayerAnimation(-1, 3);
    SleepProcess(50);
}

void card_bank(){
    msgBox(msg_bank);
    AdjustPlayerCoinsGradual(PLAYER_INDEX, D_800CD0B4);
    ShowPlayerCoinChange(PLAYER_INDEX, D_800CD0B4);
    D_800CD0B4 -= D_800CD0B4;
    SetBoardPlayerAnimation(-1, 5);
    SleepProcess(30);
}

void card_nothing(){
    msgBox(msg_nothing_1);
    msgBox(msg_nothing_2);
    msgBox(msg_nothing_3);
}

void card_nice(){
    func_8004A520(69);
    msgBox(msg_nice);
    func_8004A520(GetBoardAudioIndex());
}

void card_possess(struct player *player){
    msgBox(msg_possess);
    player->flags = player->flags ^ 1;
}

void card_plunder(){
    struct player *player = GetPlayerStruct(PLAYER_INDEX);
    u8 open_index = PlayerHasEmptyItemSlot(PLAYER_INDEX);
    
    if(open_index != 0xFF){
        msgBox(msg_plunder);
        
        //the item is placed in the first empty slot of player
        player->items[open_index] = ITEM_PLUNDER_CHEST;
        
        //Fix / refresh player item HUDs
        FixUpPlayerItemSlots(PLAYER_INDEX);
        RefreshHUD(PLAYER_INDEX);
    }
    else{
        //No space for item
        card_nothing();
    }
    
    SleepProcess(15);
}

void card_purchase(){
    struct player *player = GetPlayerStruct(PLAYER_INDEX);
    u8 open_index = PlayerHasEmptyItemSlot(PLAYER_INDEX);
    
    if(open_index != 0xFF){
        int rnd = GetRandomByte() % 7;
        int item_amount = (GetRandomByte() % 10)+10;
        u8 item;
        char price[16];
        
    	sprintf(price, "%d", item_amount);
        
        if		(rnd == 0){item = ITEM_SKELETON_KEY;}
        else if	(rnd == 1){item = ITEM_CELLULAR_SHOPPER;}
        else if	(rnd == 2){item = ITEM_BOWSER_PHONE;}
        else if	(rnd == 3){item = ITEM_DUELING_GLOVE;}
        else if	(rnd == 4){item = ITEM_BOO_BELL;}
        else if	(rnd == 5){item = ITEM_BOWSER_SUIT;}
        else if	(rnd == 6){item = ITEM_BOO_REPELLANT;}
        
        /*Shop Code*/
        while (1) {
            // Prompt the player to buy.
            ShowMessage(-1,msg_purchase,0,price,0,0,0,0);

            // Get the selection, either from the player or CPU.
            // In this case, we have CPUs always pick Yes (0)
            s32 choice = GetBasicPromptSelection(2, 0);

            CloseMessage();
            func_800EC6EC();

            switch (choice) {
                case 0:
                    //Curse player if they try and cheat the shop
                    if (PlayerHasCoins(PLAYER_INDEX, item_amount) == 0) {
                        PlaySound(0x29A); // Toad: Waoh...

                        msgBox(not_enough_coins_msg);
                        
                        for ( int x = 0; x < 4 ; x++ ){
                            if(x == PLAYER_INDEX){
                                AdjustPlayerCoinsGradual(x, -999);
                            }
                            else{
                                AdjustPlayerCoinsGradual(x, item_amount);
                            }
                        }
                        ShowPlayerCoinChange(PLAYER_INDEX, -999);
                        SetBoardPlayerAnimation(-1, 3);
                        SleepProcess(50);
                        return;
                    }

                    // Take coins
                    AdjustPlayerCoinsGradual(PLAYER_INDEX, -item_amount);
                    ShowPlayerCoinChange(PLAYER_INDEX, -item_amount);
                    SleepProcess(35);

                    // Give item
        			player->items[open_index] = item;
        
        			//Fix / refresh player item HUDs
        			FixUpPlayerItemSlots(PLAYER_INDEX);
        			RefreshHUD(PLAYER_INDEX);
                    
                    return;

                case 1:
                    // Decline and exit
                    return;

                case 2:
                    // Let player view the map, then repeat the loop to pick again.
                    ViewBoardMap();
                    break;
            }
        }
    }
    else{
        //No space for item
        card_nothing();
    }
    SleepProcess(15);
}

/********************************************************
********************* MAIN FUNCTION *********************
********************************************************/

void main() {
    //int curr_player = GetCurrentPlayerIndex();
	int rng;
    struct player *player = GetPlayerStruct(PLAYER_INDEX);
    u8 character;
    char print[5];
    struct objecttype *model_obj;
    model_obj = func_800D90C8(56, 0);	//Stars effect
    model_obj->unka |= 4; // ?
    
    displayModel(model_obj);
    PlaySound(0x13F);	//Green player changing to red/blue before mini-game
    
    
    //If possessed, give control back
    if(player->flags & 1){
        player->flags = player->flags ^ 1;
        msgBox(msg_unpossess);
    }
    //Else, draw a card
    else{
        rng = (GetRandomByte() % 9)+1;
        
        //TEST CARD
    	//rng = 6;
        
        switch(rng){
            case 1:card_nothing();				break;
            case 2:card_givecoins();			break;
            case 3:card_takecoins();			break;
            case 4:card_communism();			break;
            case 5:card_bank();					break;
            case 6:card_plunder(/*player*/);	break;
            case 7:card_mushroom(/*player*/);	break;
            case 8:card_purchase(/*player*/);	break;
            case 9:card_possess(player);		break;
            case 10:card_nice();				break;
        }
    }
    deleteModel(model_obj);
}



