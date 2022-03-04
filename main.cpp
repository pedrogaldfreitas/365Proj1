#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <fstream>
#include "./libraries/AudioFile.h"
#include <SDL2/SDL.h>
#include <windows.h>
#include <algorithm>
#include <ShObjIdl.h>


using namespace std;

//QUESTION 1

AudioFile<double> audioFile;

void open_file() {
    OPENFILENAME ofn = {0};
    TCHAR fileSize[260] = {0};

    ofn.lStructSize = sizeof(ofn);
    //ofn.hwndOwner = hWnd;
    ofn.lpstrFile = fileSize;
    ofn.nMaxFile = sizeof(fileSize);
    //ofn.lpstrFilter = _T();

    return;
}

vector<double> normalizeSamples(vector<double> samples) {
    //Prologue: Get important variables (lowest value, and most extreme absolute value in array)
    double minVal = *min_element(samples.begin(), samples.end());
    if (minVal < 0) {
        minVal = -minVal;
    }

    double mostExtremeVal = 0;
    for (int j = 0; j < samples.size(); j++) {
        if (samples[j] < 0) {
            if (-samples[j] > mostExtremeVal) {
                mostExtremeVal = -samples[j];
            }
        } else {
            if (samples[j] > mostExtremeVal) {
                mostExtremeVal = samples[j];
            }
        }
    }
    for (int i = 0; i < samples.size(); i++) {
        //Step 1: Add absolute value of lowest value to all items
        samples[i] += minVal;
        //Step 2: Divide all values in samples by mostExtremeVal.
        samples[i] /= mostExtremeVal;
        //Step 3: Subtract all values by minVal/mostExtremeVal.
        samples[i] -= (double)(minVal/mostExtremeVal);
    }
    return samples;
}

void openAudioFile() {
    audioFile.load("./sampleAudio/pianoStereo.wav");

    int numSamples = audioFile.getNumSamplesPerChannel();
    cout << "Sample Frequency = " << audioFile.getSampleRate() << "kHz\n";
    cout << "Number of Samples (Per Channel) = " << numSamples << "\n";

    //Drawing setup with SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return;
    }

    SDL_Window* graphicsWindow = NULL;
    SDL_Renderer* graphicsRenderer = NULL;
    SDL_Event event;

    if (audioFile.isStereo()) {
        if (SDL_CreateWindowAndRenderer(1280, 431, 0, &graphicsWindow, &graphicsRenderer) != 0) {
            return;
        }
    } else {
        if (SDL_CreateWindowAndRenderer(1280, 231, 0, &graphicsWindow, &graphicsRenderer) != 0) {
            return;
        }
    }

    SDL_SetRenderDrawColor(graphicsRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    SDL_bool done = SDL_FALSE;

    //Samples for Channel 1
    vector<double> normalizedSamples = normalizeSamples(audioFile.samples[0]);

    int sampleDisplayRate = audioFile.getNumSamplesPerChannel()/1280;

    int count = 0;
    for (int i = 0; i < numSamples; i++) {
        if (i%sampleDisplayRate == 0) {
            double currentSample = audioFile.samples[0][i];

            SDL_RenderDrawLine(graphicsRenderer, count, 100, count, 100-(normalizedSamples[i]*100));
            count++;
        }
    }


    //Samples for Channel 2
    if (audioFile.isStereo()) {
        SDL_SetRenderDrawColor(graphicsRenderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(graphicsRenderer, 0, 200, 1500, 200);
        SDL_SetRenderDrawColor(graphicsRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

        normalizedSamples = normalizeSamples(audioFile.samples[1]);
        count = 0;

        for (int j = 0; j < numSamples; j++) {
            if (j%sampleDisplayRate == 0) {
                double currentSample = audioFile.samples[1][j];
                SDL_RenderDrawLine(graphicsRenderer, count, 300, count, 300-(normalizedSamples[j]*100));
                count++;
            }
        }
    }

    SDL_RenderPresent(graphicsRenderer);

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = SDL_TRUE;
            }
        }
    }

    SDL_Quit();
    return;
}

//QUESTION 2

void ditherImage() {
    SDL_Window* graphicsWindow = NULL;
    SDL_Renderer* graphicsRenderer = NULL;
    SDL_Event event;
    SDL_bool done = SDL_FALSE;

    SDL_RenderPresent(graphicsRenderer);

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = SDL_TRUE;
            }
        }
    }

    SDL_Quit();
    return;
}

int main(int argv, char** args) {
    //openAudioFile();
    ditherImage();
    return 0;
}