//main loop begin (this runs continuously)

void loop() {

  //while loop for discreet notes state
  while (player_state == STATE_DISCREET_NOTES) {

    DacAudio.FillBuffer();
    check_for_state_change();

    if (touch1.update()) {
      play_note_by_name("F3");
    }

    if (touch2.update()) {
      play_note_by_name("Ab3");
    }

    if (touch3.update()) {
      play_note_by_name("Bb3");
    }

    if (touch4.update()) {
      play_note_by_name("B3");
    }

  }

  //while loop for the melody state
  while (player_state == STATE_MELODY) {

    DacAudio.FillBuffer();
    check_for_state_change();

    // Check for touch events
    bool touch1_state = touch1.update();
    bool touch2_state = touch2.update();
    bool touch3_state = touch3.update();
    bool touch4_state = touch4.update();

    // Play the next note in the melody when a touch is detected
    if (touch1_state || touch2_state || touch3_state || touch4_state ) {
      play_melody_current_note();
    }
  }

}  // end loop