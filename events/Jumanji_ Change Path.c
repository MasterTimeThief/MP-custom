// NAME: Jumanji: Change Path
// GAMES: MP3_USA
// EXECUTION: Direct
// PARAM: Space|alt
// PARAM: Space|alt_prev
// PARAM: Space|alt_next
// PARAM: Space|Start
// PARAM: Space|Start_prev
// PARAM: Space|Start_next

// Here's a list of the most common bytes you'll need
// 0x01 ; Black Font
// 0x03 ; Red Font
// 0x04 ; Purple Font
// 0x05 ; Green Font
// 0x06 ; Blue font
// 0x07 ; Yellow Font
// 0x08 ; White Font
// 0x85 ; Period (.)
// 0xC2 ; Exclamation Mark (!)
// 0xC3 ; Question Mark (?)
// 0x82 ; Comma (,)
// 0x0A ; New Line (Writes Below)
// 0x5C ; Apostrophe (')
// 0x29 ; Coin icon
// 0x3D ; - (minus)
// 0x3E ; x (multiply)
// 0xFF,0 ; FF=Pause


char *msg_path = "\x0B"
    "Here your walkway splits in two\x82" "\n"
    "Time to " "\x07" "alter course" "\x08" " anew\xC2"
    "\xFF";

char *msg_start = "\x0B"
    "Your journey halts for you this day\x82" "\n"
    "Now " "\x03" "back to start" "\x08" "\x82 away\x82 away\xC2"
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
	int rng = guRandom() % 100;
    
	if(rng > 0 && rng <= 90){
        msgBox(msg_path);
        SetPlayerOntoChain(curr_player,alt_chain_index,alt_chain_space_index);
        SetPrevChainAndSpace(curr_player,alt_prev_chain_index,alt_prev_chain_space_index);
        SetNextChainAndSpace(curr_player,alt_next_chain_index,alt_next_chain_space_index);

    }
    else{
        //Go To Start
        msgBox(msg_start);
        SetPlayerOntoChain(curr_player,Start_chain_index,Start_chain_space_index);
        SetPrevChainAndSpace(curr_player,Start_prev_chain_index,Start_prev_chain_space_index);
        SetNextChainAndSpace(curr_player,Start_next_chain_index,Start_next_chain_space_index);
    
    }
    
    
}


