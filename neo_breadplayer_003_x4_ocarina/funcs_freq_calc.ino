// Function to calculate the frequency of a note given its MIDI number
float calculate_frequency(int note_number) {
    return 440.0f * pow(2.0f, (note_number - 69) / 12.0f);
}

// Function to get the MIDI number of a note given its name
int get_midi_number(const char* note_name) {
    // Iterate through the midiNotes array to find the matching note name
    for (int i = 0; i < sizeof(midiNotes) / sizeof(MidiNote); i++) {
        if (strcmp(note_name, midiNotes[i].noteName) == 0) {
            return midiNotes[i].midiNumber;
        }
    }
    // Return -1 if the note name is not found
    return -1;
}

// Function to calculate the frequency of a note given its name
float calculate_frequency_from_name(const char* note_name) {
    int midi_number = get_midi_number(note_name);
    if (midi_number != -1) {
        return calculate_frequency(midi_number);
    }
    // Return 0 if the note name is not found
    return 0.0f;
}

// Function to calculate the speed ratio needed to play the source note at the target frequency
float calculate_speed(const char* source_note, const char* target_note) {
    float source_freq = calculate_frequency_from_name(source_note);
    float target_freq = calculate_frequency_from_name(target_note);
    if (source_freq != 0.0f && target_freq != 0.0f) {
        return target_freq / source_freq;
    }
    // Return -1.0f if either source or target note frequency is not found
    return -1.0f;
}

// Example usage:
// float speed = calculate_speed("C3", "D3");
// This will give you the speed ratio needed to play the sample at "C3" as "D3"


// Function to find the best/closet sample to use for a target note
const NoteSample* find_best_sample(const char* name) {
    const NoteSample* best_sample = nullptr;
    float closest_difference = MAX_FLOAT;
    float target_frequency = calculate_frequency_from_name(name);

    // Iterate through the samples array to find the best match
    for (int i = 0; i < num_samples; ++i) {
        float sample_frequency = calculate_frequency_from_name(samples[i].notename);
        float difference = fabs(target_frequency - sample_frequency);
        
        // Check if the current sample is closer to the target frequency
        if (difference < closest_difference) {
            closest_difference = difference;
            best_sample = &samples[i];
        }
    }

    return best_sample;
}

// Example usage:
// NoteSample* best_sample = find_best_sample("A2");
// This will give you the best sample to use for playing the note "A2"

