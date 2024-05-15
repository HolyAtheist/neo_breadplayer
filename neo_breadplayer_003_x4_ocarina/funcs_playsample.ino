//use XT_DAC_Audio library to play soundsample at specific speed
void playSampleNote(XT_Wav_Class& sound, float speed) {
  sound.Speed = speed;
  DacAudio.Play(&sound);
}


// Function to play a note by name using the best/closet sample
void play_note_by_name(const char* name) {
    // Find the best sample for the target note
    const NoteSample* best_sample = find_best_sample(name);

    if (best_sample != nullptr) {
        // Calculate the target speed for the sample
        float target_speed = calculate_speed(best_sample->notename, name);

        // Play the best sample at the calculated speed
        playSampleNote(*(best_sample->sample), target_speed);
    } else {
        Serial.println("No sample found for the given note.");
    }
}
// Example usage:
//play_note_by_name("C3"); // Plays C3 using the C3_sample at speed 1.0
//play_note_by_name("G#2"); // Plays G#2 using the G2_sample at speed 1.111

void play_melody_current_note() {
  const char* note_name = current_melody[current_note];
  play_note_by_name(note_name);

  current_note = (current_note + 1) % melody_notes;
}

