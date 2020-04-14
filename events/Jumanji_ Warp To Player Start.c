// NAME: Jumanji: Warp To Player Start
// GAMES: MP3_USA
// EXECUTION: Direct
// PARAM: Space|p1
// PARAM: Space|p1_prev
// PARAM: Space|p1_next
// PARAM: Space|p2
// PARAM: Space|p2_prev
// PARAM: Space|p2_next
// PARAM: Space|p3
// PARAM: Space|p3_prev
// PARAM: Space|p3_next
// PARAM: Space|p4
// PARAM: Space|p4_prev
// PARAM: Space|p4_next

void main() {
    int curr_player = GetCurrentPlayerIndex();
    
    switch(curr_player){
        case 0:
            SetPlayerOntoChain(0,p1_chain_index,p1_chain_space_index);
            SetPrevChainAndSpace(0,p1_prev_chain_index,p1_prev_chain_space_index);
            SetNextChainAndSpace(0,p1_next_chain_index,p1_next_chain_space_index);
            break;
        case 1:
            SetPlayerOntoChain(1,p2_chain_index,p2_chain_space_index);
            SetPrevChainAndSpace(1,p2_prev_chain_index,p2_prev_chain_space_index);
            SetNextChainAndSpace(1,p2_next_chain_index,p2_next_chain_space_index);
            break;
        case 2:
            SetPlayerOntoChain(2,p3_chain_index,p3_chain_space_index);
            SetPrevChainAndSpace(2,p3_prev_chain_index,p3_prev_chain_space_index);
            SetNextChainAndSpace(2,p3_next_chain_index,p3_next_chain_space_index);
            break;
        case 3:
            SetPlayerOntoChain(3,p4_chain_index,p4_chain_space_index);
            SetPrevChainAndSpace(3,p4_prev_chain_index,p4_prev_chain_space_index);
            SetNextChainAndSpace(3,p4_next_chain_index,p4_next_chain_space_index);
            break;
    }
    
    PlaySound(0x107);
    SleepProcess(5);
}