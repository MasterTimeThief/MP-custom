// NAME: Jumanji: Bank
// GAMES: MP3_USA
// EXECUTION: Direct

extern unsigned short int D_800CD0B4;	//Bank total

extern unsigned short int D_800D1112;	//Player 1 Coins
extern unsigned short int D_800D114A;	//Player 2 Coins
extern unsigned short int D_800D1182;	//Player 3 Coins
extern unsigned short int D_800D11BA;	//Player 4 Coins

char *msg_bank = "\x0B"
    "If this jungle you wish to enter\x82" "\n"
    "Pay " "\x07" "5 Coins" "\x08" "\x82 and reach the " "\x05" "center" "\x08" "\x85\x85\x85"
    "\xFF";

void msgBox(char *message){
    // First argument is character image (-1 for none).
	ShowMessage(-1, message, 0, 0);
    
    // The rest here perform the "wait for confirmation"
	// and messagebox teardown.
    func_800EC9DC();
    CloseMessage();
    func_800EC6EC();
}

void main() {
    int curr_player = GetCurrentPlayerIndex();
    int coins;
    char sum[5];
    //s16 *bank_total = (s16*) 0x800CD0B4;
    msgBox(msg_bank);
    
    switch(curr_player){
        case 0: coins = D_800D1112;break;
        case 1: coins = D_800D114A;break;
        case 2: coins = D_800D1182;break;
        case 3: coins = D_800D11BA;break;
    }
    
    if(PlayerHasCoins(curr_player,5)){
        AdjustPlayerCoinsGradual(curr_player, -5);
    	ShowPlayerCoinChange(curr_player, -5);
        D_800CD0B4 += 5;
    }
    else if(PlayerHasCoins(curr_player,1)){
        AdjustPlayerCoinsGradual(curr_player, -1*coins);
    	ShowPlayerCoinChange(curr_player, -1*coins);
        D_800CD0B4 += coins;
    }
    //sprintf(sum, "%i", D_800CD0B4);cardText(sum);
    SleepProcess(30);
}