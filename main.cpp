#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <fstream>
#include "./libraries/AudioFile.h"
#include <SDL2/SDL.h>

using namespace std;

AudioFile<double> audioFile;

int main() {
    audioFile.load("./sampleAudio/FireIgnite.wav");

    int numSamples = audioFile.getNumSamplesPerChannel();
    cout << "Sample Frequency = " << audioFile.getSampleRate() << "kHz\n";
    cout << "Number of Samples (Per Channel) = " << numSamples << "\n";

    //Drawing setup with SDL
    if (SDL::SDL_Init(SDL_INIT_VIDEO) != 0) {
        return 1;
    }

    SDL_Window* graphicsWindow = NULL;
    SDL_Renderer* graphicsRenderer = NULL;

    //Samples for Channel 1
    for (int i = 0; i < numSamples; i++) {
        double currentSample = audioFile.samples[0][i];
        cout << currentSample << "\n";
    }

    //Samples for Channel 2
    if (audioFile.isStereo()) {
        for (int j = 0; j < numSamples; j++) {
            double currentSample = audioFile.samples[1][j];
        }
    }

    SDL_Quit();
    return 0;
}