void check_for_state_change() {
    static bool state_changed = false; // Flag variable to track state changes
	
    // Check if all four pads have been touched continuously for 2 seconds
    if (touch1.touchedFor(2000) && touch2.touchedFor(2000) && touch3.touchedFor(2000) && touch4.touchedFor(2000)) {
        if (!state_changed) {
            // State has changed, update the flag and toggle the player state
            state_changed = true;
            player_state = (player_state == STATE_MELODY) ? STATE_DISCREET_NOTES : STATE_MELODY; //switch between states
			// ternary operator -- basic syntax of a ternary operator is condition ? value_if_true : value_if_false
            play_note_by_name("B2"); //play a note to let user know state has changed 
        }
    } else {
        // Pads are no longer continuously touched, reset the flag
        state_changed = false;
    }
}