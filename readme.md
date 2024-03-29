## TGX4 - Teensy Guitar Amp Project

This project aims at creating a nice usable set of guitar and bass tones while using relatively inexpensive hardware and being simple to build.  
It combines a Neural Network amp modeler with stereo IR cabinet emulation, versatile stereo Ping-Pong delay and 3 different reverbs. There is also a Compressor and Booster with octave up mode for further input tone shaping.  

### Change-log  
03.2024 - Initial release.  

### User Interface  
In general all the parameters are controlled via MIDI. There is a lot of controls and to make the hardware less expensive and easier to build the starting point is a simple html page using WebMIDI + WebSerial, which, opened in one of the supported browsers (Chrome, Opera, Edge) offers a full GUI interface.  
Teensy is using the USB Serial port to show the status and other info about the current state.  
Second control option for less frequently used configuration and debugging is a CLI interface.

![Control interface](img/TGX4_AmpControls.gif)  

### Internal structure  

![Internal structure](img/TGX4_blockSchem.gif)  

## Signal Flow  
The signal flow, except two components is stereo. The two mono components are **Booster** and **Neural Amp Modeler**. Mono means, these two sum the input signal into one and send one output to both L and R channels when enabled. In bypass mode both mono blocks pass the stereo signal unaffected. This way the TGX4 can be used either with mono source or as stereo effects processor (with Booster and Amp Modeler disabled).
1. **Input Select** - controls which input channel use as source:  
	* L+R - stereo input
	* L+L - left mono input
	* R+R - right mono input
2. **Compressor** - stereo dynamic compressor with wide varierty of controls:  
	* Pregain - input gain  
	* Threshold  
	* Ratio  
	* Attack, Release  
	* PostGain (MakeUp)  
3. **Booster** - an oversampled wave shaper based on 12AX7 tube curve:  
	* Drive - overdrive level  
	* Bottom - input highpass filter  
	* Presence - treble control  
	* Mix - dry/wet(distorted) mixer ratio, useful for Bass!  
	* Level - Booster output volume
	* Octave - analog octave up emulation  
4. **Neural Amp Modeler** 8 models available, from, clean channels, through light crunch to high gain lead ones:
	* Gain - controls the input signal level
5. **3 Band EQ** - this EQ is a "passive" one, meaning the flat response is when all controls are at max. Use for slight tone adjustments. Most of the "amp" sounds are already modelled in the previous block.  
6. **Gate** - adjustable Noise Gate. Noise gate is using the clean input signal for the side chain path, this way the threshold does not have to be adjusted for different distortion levels.
7. **Time Based FX Section** consisting of two effects in parallel: Stereo Ping-Pong delay with modulation and one of 3 available Reverbs. There are two main controls for this section:  
	* FX Mix - controls the dry sound vs Time FX sound mix ratio. Use to set the balance between the main sound and the Delay + Reverb combo  
	* Delay/Reverb Mix - this mixer controls the balance between the delay and reverb outputs.
8. **Stereo Cabinet Emulator / IR convolver**:  
	* 7 guitar cabinet models  
	* 3 bass cabinet models  
	* Stereo Doubler function emulating double tracking  
  9. **Output Section**:  
      * Master Lowcut - output highpass filter to control the low end range  
      * Pan - Left/Right panorama setting  
      * Volume - final output volume   
  
### Stereo Ping-Pong delay  
One of the time based effects, versatile 1 second delay with modulation and Freeze feature (looper mode). Controls:  
   * Time - delay time  
   * Inertia - how fast the delay time is updated after change. Creates weird pitch shifting effects  
   * Repeats - amount of feedback  
   * HiCut - low pass filter inside the delay path, controls the treble loss for each repeat. Many delays implement such feature and claim to make the sound "analog"  
   * LowCut - high pass filer inside the delay path, controls the bass loss for each repeat  
   * Treble - overal treble control to make the sound darker
   * Bass - output bass control for futher delay tone shaping  
   * ModRate - modulation rate (internal LFO speed)
   * ModDepth - modulation depth (internal LFO depth)
   * Tap Tempo  
   * Freeze mode - mutes the input signal and lets the echo repeat infinitely  

 ![Delay controls](img/TGX4_DelayControls.gif)    

## Reverb Section  
There are 3 types of reverb built in, **only one can be active** at a time:  
1. **Plate Reverb** - versatile lush sounding long reverb:  
	* Size - reverb time  
	* Diffusion - lower values create more echo effects inside the reverb tail  
	* Treble Cut - treble loss in the reverb tail, afffects the reverb time  
	* Bass Cut - bass loss in the reverb tail, affects the reverb time  
	* Treble - output treble control for the reverb wet signal  
	* Bass - output bass controls fot the reverb wet signal  
	* Pitch - reverb pitch set in semitones (range -12 to +24)  
	* Pitch Mix - mix ratio for the pitched up/down and natural pitch signal (creates harmonies)  
	* Shimmer - amount of shimmer effect  
	* Pitch Shim - shimmer effect pitch setting  
	* Freeze mode - creates infinite reverb sound  
	* Bleed In - amount of input signal fed into the reverb while in Freeze mode.  
2. **Reverb SC** based on Sean Costello's reverb algorithm using multiple feedback delay lines:  
	* Time - reverb time  
	* Lowpass Freq - tail treble loss  
	* Freeze mode - infinite reverb sound  
3. **Spring Reverb** - for more classic guitar tones, emulation of a typical spring reverb tank:  
	* Time - reverb time  
	* Bass Cut - high pass filter
	* Treble Cut - lowpass filter  

 ![Reverb controls](img/TGX4_ReverbControls.gif)  

Press **Plate**, **Spring** or **ReverbSC** button to activate the reverb.  

## Hardware  
In the simplest form, the required hardware can be a Teensy 4.1 with installed PSRAM chip and the audio adapter board.  
Recommended upgrade would be a high impendace input buffer using an opamp or even a simple transistor one. See [here](diy_buffer.md) for a simple DIY project. 
Alternatively, another pedal with buffered bypass can be used instead.  

**Teensy4.1**  
The main project using all the features requires Teensy4.1 with **installed PSRAM** chip.  

**Teensy4.0**  
A stripped down version able to run on Teensy4 is planned for near future.  

**Audio Boards**  
I have developed the project using my own designed HW platform, using the WM8731 codec ship and I2S2 interface for audio.  
There are two other build options available at the moment (03.2024):  
1. Teensy Audio Adaptor board, using the SGTL5000 codec and I2S(1) - default setting for the project.  
2. TGA Pro by Blackaddr, uses WM8731 codec on I2S(1). As of now, the WM8731 is obsolete, but many people might have there boards and this is a much better HW option compared to the TeensyAudioBoard, considering the gain levels used in guitar applications.  
   
The hardware build option is set either in the `platformio.ini` file  
`default_envs = teensy_audio_board`  
or by invoking the build command from the terminal:  
`pio run -e teensy_audio_board`  
The available variable values are:  
```
teensy_audio_board  
blackaddr_tgapr    
hexefx_t41gfx  
```
I do have plans to design a dedicated PCB which can be fitted into one of the common guitar pedal enclosures. When done, it will be available in the *hardware* folder.  
For the available hardware platforms, the codecs and I2S I/O is configured to use full 24bit/44.1kHz audio stream.  

**Adapting to other audio boards**  
Adapting is relatively simple. The two required changes are:  
1. Provide a codec driver and configure it with the following I2S specs:  
	- Sampling frequency: 44.1kHz
	- Master clock = 256 * Fs
	- 32bit left justified word (I2S format)
2. Set the correct I2S inteface. Drivers for I2S (default in Teensy Audio lib) and I2S2 are provided in the `hexefx_audio_F32` library.

## Usage  

1. Open the project in the PlatformIO environment.
2. Open the `platformio.ini` file abd set the required haedware configuration.
3. Build the project and upload it to the Teensy4 board.
4. Open the `TGX4_1.html` file placed in the `html` folder in Chrome, Chromium or Edge browser (others do not implement WebMIDI and WebSerial).
5. Connect to the USB MIDI interface listed as Teensy.  
6. Click `Connect` button on the top of the page and choose Teensy Serial port.
7. Use the dials and buttons to control the device.  

## Preset System  
A snaphot of all controlls (except Delay+Reverb Freeze modes) can be saved in 8 available preset slots:
1. Choose the Preset slot by pressing the buttons **Preset 1** - **Preset 8**.
2. Press **Save** to store the current sound in the chosen preset.  

**Default Presets**:  
1. Clean guitar sound, spring reverb.
2. Lightly overdriven clean guitar sound with modulated delay used as a stereo chorus.  
3. Boost/Overdrive into Clean channel + Reverb SC. Uses Stereo Doubler to emulate double tracking. Bluesy rhythm tone.  
4. Crunch Amp + Boost, mono mode + stereo delay and reverb. Bluesy solo sound.
5. High gain rhythm tone, a bit of Spring Reverb, Stereo Doubler enabled.  Uses Booster for input tone shaping, low end cut and light overdrive.
6. High Gain solo tone with modulated delay and reverb.  
7. Clean Bass sound with compressor.
8. Distorted fuzzy stereo bass sound.  

Presets are stored in EEPROM, pedal remembers the last used one and loads it when booting up.  
Preset system will most likely be expanded in the future. The current 8 slot option is a starting point only.  

## Command Line Interface  
Use the **Term** button places on the main **Amp** page to switch between the status report and the CLI. Type `help` to list all the commands. CLI uses history and autocomplete.  
Avialable commands:  
- `preset-rst` - repopulates the 8 available presets with default "factory" ones.  
- `meminfo`- print RAM memory details  
- `load`- list CPU loads for all components  
- `midi-ch`- assing a new MIDI channel for the Teensy. Use it when controlling the device with other MIDI controllers. The WebMIDI interface sends the data on channel 1.  

## Credits  
The Neural Network Modeler is based on implementation by [Keith Bloemer/GuitarML](https://github.com/GuitarML/Seed)  

[OpenAudio_ArduinoLibrary](https://github.com/chipaudette/OpenAudio_ArduinoLibrary) by Chip Audette  

[embedded_cli](https://github.com/funbiscuit/embedded-cli) by Sviatoslav Kokurin.

---  
(c) 2024 by Piotr Zapart  
www.hexefx.com  
