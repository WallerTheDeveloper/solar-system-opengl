//
// Created by Daniel on 14-Oct-25.
//

#include "AudioManager.h"

#include <iostream>

AudioManager::AudioManager() : isInitialized(false) {
  if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
    std::cerr << "Failed to initialize audio engine" << std::endl;
    return;
  }
  isInitialized = true;
}

AudioManager::~AudioManager() {
  if (isInitialized) {
    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
  }
}

void AudioManager::playBackgroundMusic(const std::string& filepath) {
  if (!isInitialized) {
    return;
  }

  if (ma_sound_init_from_file(&engine, filepath.c_str(), MA_SOUND_FLAG_STREAM,
                              NULL, NULL, &sound) != MA_SUCCESS) {
    std::cerr << "Failed to load audio file: " << filepath << std::endl;
    return;
  }

  ma_sound_set_looping(&sound, MA_TRUE);
  ma_sound_start(&sound);
}

void AudioManager::stop() {
  if (isInitialized) {
    ma_sound_stop(&sound);
  }
}

void AudioManager::setVolume(float volume) {
  if (isInitialized) {
    ma_sound_set_volume(&sound, volume);
  }
}