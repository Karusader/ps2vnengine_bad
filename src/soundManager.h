

void initSoundManager();

void setBGMEnabled(bool enabled);
void setSFXEnabled(bool enabled);

void playbgm(char bgmFile[64], int bgmBits, int bgmFreq, int bgmChannels);
void playSFX(char sfxFile[64], int sfxBits, int sfxFreq, int sfxChannels);
void soundFuncLoop();

void soundSFXLoop();

