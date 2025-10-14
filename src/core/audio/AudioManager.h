//
// Created by Daniel on 14-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_AUDIOMANAGER_H
#define SOLAR_SYSTEM_OPENGL_AUDIOMANAGER_H

#include <miniaudio/miniaudio.h>
#include <string>

class AudioManager {
public:
  AudioManager();
  ~AudioManager();

  void playBackgroundMusic(const std::string& filepath);
  void stop();
  void setVolume(float volume); // 0.0f to 1.0f

private:
  ma_engine engine;
  ma_sound sound;
  bool isInitialized;
};

#endif  // SOLAR_SYSTEM_OPENGL_AUDIOMANAGER_H
