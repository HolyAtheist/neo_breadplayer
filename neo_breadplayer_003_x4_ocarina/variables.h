
const float MAX_FLOAT = 3.402823466e+38;  // A very large value; used for sorting

//pins to use for touch sensing
//note that e.g. A4 is an arduino designation,
//on e.g. the wrover board A4 == 15 (GPIO15) == T3 (TOUCH3)
//use what makes the most sense to you

// wrover pins
const int TOUCH1_PIN = A4;
const int TOUCH2_PIN = D12;
const int TOUCH3_PIN = D13;
const int TOUCH4_PIN = D5;


//firebeetle wroom-32e pins
// const int TOUCH1_PIN = D12;
// const int TOUCH2_PIN = D13;
// const int TOUCH3_PIN = D5;
// const int TOUCH4_PIN = D6;
// const int TOUCH5_PIN = D7;
// const int TOUCH6_PIN = A4;


//on most boards DAC1=25, DAC2=26
const int DAC_PIN = DAC2; // the dac is what will be outputting the sound waveforms


// threshold to trigger touch event
const int TOUCH_THRESHOLD = 15;

// Default touchpin update interval in milliseconds
const unsigned long UPDATE_INTERVAL = 80;

// number of baseline reads to average during setup
const int BASELINE_READS = 15;


//the states of the program
const int STATE_MELODY = 0;  // these numbers could be anything, but need to be different
const int STATE_DISCREET_NOTES = 1;

int player_state = STATE_DISCREET_NOTES;  // which state to start in

XT_DAC_Audio_Class DacAudio(DAC2, 0);  //gpio pin (where to output sound wave), hw timer for library to use

// (multi-)samples for instrument
XT_Wav_Class D3_sample(Ocarina_D3_short);
XT_Wav_Class F3_sample(Ocarina_F3_short);
XT_Wav_Class A3_sample(Ocarina_A3_short);
XT_Wav_Class B3_sample(Ocarina_B3_short);
XT_Wav_Class D4_sample(Ocarina_D4_short);

// Define the NoteSample struct
struct NoteSample {
  const char* notename;
  XT_Wav_Class* sample;
};

// Define your array of NoteSamples
const NoteSample samples[] = {
  { "D3", &D3_sample },
  { "F3", &F3_sample },
  { "A3", &A3_sample },
  { "B3", &B3_sample },
  { "D4", &D4_sample },
  // Add more entries for other samples...
};
const int num_samples = sizeof(samples) / sizeof(NoteSample);

//variables for keepin track of the melody and current note
int current_melody_index = 0;
int current_note = 0;
const char** current_melody;
int melody_notes = 0;

//example melody arrays

const char* mary_had_a_little_lamb[] = {
  "E4", "D4", "C4", "D4", "E4", "E4", "E4",
  "D4", "D4", "D4", "E4", "E4", "E4",
  "E4", "D4", "C4", "D4", "E4", "E4", "E4",
  "E4", "D4", "D4", "E4", "D4", "C4"
};

const char* mario[] = {
  "E4", "E4", "E4",
  "C4", "E4", "G4", "G3",

  "C4", "G3", "E3",
  "A3", "B3", "Bb3", "A3",
  "G3", "E4", "G4", "A4",
  "F4", "G4", "E4", "C4", "D4", "B3",

  "C4", "G3", "E3",
  "A3", "B3", "Bb3", "A3",
  "G3", "E4", "G4", "A4",
  "F4", "G4", "E4", "C4", "D4", "B3",

  "G4", "F#4", "F4", "D4", "E4",
  "G3", "A3", "C4",
  "A3", "C4", "D4",
  "G4", "F#4", "F4", "D4", "E4",
  "C5", "C5", "C5",

  "G4", "F#4", "F4", "D4", "E4",
  "G3", "A3", "C4",
  "A3", "C4", "D4",
  "D#4", "D4", "C4",

  "C4", "C4", "C4",
  "C4", "D4", "E4", "C4", "A3", "G3",
  "C4", "C4", "C4",
  "C4", "D4", "E4",

  "C4", "C4", "C4",
  "C4", "D4", "E4", "C4", "A3", "G3",
  "E4", "E4", "E4",
  "C4", "D4", "G4",
  "G3",

  "C4", "G3", "E3",
  "A3", "B3", "Bb3", "A3",
  "G3", "E4", "G4", "A4",
  "F4", "G4", "E4", "C4", "D4", "B3",

  "C4", "G3", "E3",
  "A3", "B3", "Bb3", "A3",
  "G3", "E4", "G4", "A4",
  "F4", "G4", "E4", "C4", "D4", "B3",

  "E4", "C4", "G3",
  "G3", "A3", "F4", "F4", "A3",
  "B3", "A4", "A4", "A4", "G4", "F4",
  "E4", "C4", "A3", "G3",

  "E4", "C4", "G3",
  "G3", "A3", "F4", "F4", "A3",
  "B3", "F4", "F4", "F4", "E4", "D4", "C4",
  "G3", "E3", "C3",

  "C4", "G3", "E3",
  "A3", "B3", "A3",
  "G#3", "Bb3", "G#3",
  "G3", "F#3", "G3"
};

const char* allstar[] = {
  "G3", "D4", "B3", "B3", "A3", "G3",
  "G3", "C4", "B3", "B3", "A3", "A3", "G3",
  "G3", "C4", "B3", "B3", "A3", "A3", "G3", "A3", "E3", "D4",

  "G3", "G3", "D4", "B3", "B3", "A3", "A3",
  "G3", "G3", "C4", "B3", "B3", "A3", "A3",
  "G3", "G3", "C4", "B3", "A3", "A3", "G3", "A3", "E3", "D4",

  "B3", "G3", "G3", "E3", "G3", "G3",
  "G3", "E3", "G3", "G3", "G3", "B3",
  "B3", "G3", "G3", "E3", "G3", "G3",
  "G3", "E3", "G3", "G3", "G3", "B3",
  "G3", "B3", "D4", "C4", "B3", "A3", "A3", "G3",
  "G3", "G3", "A3", "G3", "B3", "A3", "A3", "G3", "A3", "B3", "E3"
};

const char* spongebob[] = {
  "D4", "D4", "E4", "D4", "B3", "G3", "B3", "D4", "E4", "D4", "B3",
  "G4", "D4", "B3", "G3",
  "G4", "G4", "A4", "G4", "E4", "C4", "E4", "G4", "A4", "G4", "E4",
  "C5", "G4", "E4", "C4"
};

//end melody arrays

//initialize the touch pins using MyTouchPad objects
MyTouchPad touch1(TOUCH1_PIN, TOUCH_THRESHOLD, UPDATE_INTERVAL);
MyTouchPad touch2(TOUCH2_PIN, TOUCH_THRESHOLD, UPDATE_INTERVAL);
MyTouchPad touch3(TOUCH3_PIN, TOUCH_THRESHOLD, UPDATE_INTERVAL);
MyTouchPad touch4(TOUCH4_PIN, TOUCH_THRESHOLD, UPDATE_INTERVAL);

//midi piano note number array
struct MidiNote {
  int midiNumber;
  const char* noteName;
};

// Define the notes
const MidiNote midiNotes[] = {
  { 21, "A0" },
  { 22, "A#0" },
  { 22, "Bb0" },
  { 23, "B0" },
  { 24, "C1" },
  { 25, "C#1" },
  { 25, "Db1" },
  { 26, "D1" },
  { 27, "D#1" },
  { 27, "Eb1" },
  { 28, "E1" },
  { 29, "F1" },
  { 30, "F#1" },
  { 30, "Gb1" },
  { 31, "G1" },
  { 32, "G#1" },
  { 32, "Ab1" },
  { 33, "A1" },
  { 34, "A#1" },
  { 34, "Bb1" },
  { 35, "B1" },
  { 36, "C2" },
  { 37, "C#2" },
  { 37, "Db2" },
  { 38, "D2" },
  { 39, "D#2" },
  { 39, "Eb2" },
  { 40, "E2" },
  { 41, "F2" },
  { 42, "F#2" },
  { 42, "Gb2" },
  { 43, "G2" },
  { 44, "G#2" },
  { 44, "Ab2" },
  { 45, "A2" },
  { 46, "A#2" },
  { 46, "Bb2" },
  { 47, "B2" },
  { 48, "C3" },
  { 49, "C#3" },
  { 49, "Db3" },
  { 50, "D3" },
  { 51, "D#3" },
  { 51, "Eb3" },
  { 52, "E3" },
  { 53, "F3" },
  { 54, "F#3" },
  { 54, "Gb3" },
  { 55, "G3" },
  { 56, "G#3" },
  { 56, "Ab3" },
  { 57, "A3" },
  { 58, "A#3" },
  { 58, "Bb3" },
  { 59, "B3" },
  { 60, "C4" },
  { 61, "C#4" },
  { 61, "Db4" },
  { 62, "D4" },
  { 63, "D#4" },
  { 63, "Eb4" },
  { 64, "E4" },
  { 65, "F4" },
  { 66, "F#4" },
  { 66, "Gb4" },
  { 67, "G4" },
  { 68, "G#4" },
  { 68, "Ab4" },
  { 69, "A4" },
  { 70, "A#4" },
  { 70, "Bb4" },
  { 71, "B4" },
  { 72, "C5" },
  { 73, "C#5" },
  { 73, "Db5" },
  { 74, "D5" },
  { 75, "D#5" },
  { 75, "Eb5" },
  { 76, "E5" },
  { 77, "F5" },
  { 78, "F#5" },
  { 78, "Gb5" },
  { 79, "G5" },
  { 80, "G#5" },
  { 80, "Ab5" },
  { 81, "A5" },
  { 82, "A#5" },
  { 82, "Bb5" },
  { 83, "B5" },
  { 84, "C6" },
  { 85, "C#6" },
  { 85, "Db6" },
  { 86, "D6" },
  { 87, "D#6" },
  { 87, "Eb6" },
  { 88, "E6" },
  { 89, "F6" },
  { 90, "F#6" },
  { 90, "Gb6" },
  { 91, "G6" },
  { 92, "G#6" },
  { 92, "Ab6" },
  { 93, "A6" },
  { 94, "A#6" },
  { 94, "Bb6" },
  { 95, "B6" },
  { 96, "C7" },
  { 97, "C#7" },
  { 97, "Db7" },
  { 98, "D7" },
  { 99, "D#7" },
  { 99, "Eb7" },
  { 100, "E7" },
  { 101, "F7" },
  { 102, "F#7" },
  { 102, "Gb7" },
  { 103, "G7" },
  { 104, "G#7" },
  { 104, "Ab7" },
  { 105, "A7" },
  { 106, "A#7" },
  { 106, "Bb7" },
  { 107, "B7" },
  { 108, "C8" }
};
const int num_midi_notes = sizeof(midiNotes) / sizeof(MidiNote);


//end global variables etc
