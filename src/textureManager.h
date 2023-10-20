



#define POSITION_LEFT 320.0f
#define FADE_SLOW 0.005f
#define FADE_FAST 0.01f

// For main.c
void initTextureManager();
void renderFuncLoop();



// Render Targets
void setRenderCharacterSprite(bool option);
void setRenderTextBox(bool option);
void setRenderText(bool option);
void setRenderNameBox(bool option);
void setRenderOptions(bool option);

void loadBGTextures();
void loadTextBoxTextures();

void setBGTextureName(char spriteName[64]);
void setCharacterSprite(char spriteName[64]);
void setCharacterPosition(float XPos);
void setCharacterName(char name[64]);

void setFadeBGIn(float BGFadeInRate);
void setFadeBGOut(float BGFadeOutRate);

void setText(char text[512]);
void setOptions(int numOptions, char OptionsText[3][128]);
void setOptionSelection(int selected);


void loadCharacterTextures();