/*
// copright fuck all,
// as if anyone would want to use this dogshit code omegalul
//
// Number of times sharkie has crashed this code for no reason i can find: 1 + 1 +1 + 1 +1
*/


#include <stdio.h>
#include <malloc.h>

#include <gsKit.h>
#include <dmaKit.h>
#include <gsToolkit.h>
#include "textureManager.h"

GSGLOBAL *gsGlobal;

/*=========Dynamic Textures=========*/

// Background
GSTEXTURE BGTexture;
char c_BGTextureName[64];
// Character
GSTEXTURE CharacterTexture;
char c_CharacterTextureName[64];

/*========== Static Textures ==========*/

GSTEXTURE TextBox;
GSTEXTURE TextBoxEdge;

GSFONTM *textFont;


// Colors for text and textures (not sure what they do)

u64 BGTexCol  = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
u64 White = GS_SETREG_RGBAQ(0xFF,0xFF,0xFF,0x00,0x00);
u64 LightGrey = GS_SETREG_RGBAQ(0xee,0xee,0xee,0x00,0x00);
u64 DarkGrey = GS_SETREG_RGBAQ(0x22,0x22,0x22,0x00,0x00);
u64 TexColUnSelected = GS_SETREG_RGBAQ(0x40,0x40,0x40,0x80,0x00);



/*========== Bools ==========*/
// Render targets

bool b_renderCharacterSprite = false;
bool b_renderTextBox = false;
bool b_renderText = false;
bool b_renderNameBox = false;
bool b_renderOptions = false;

//Other bools

bool b_reloadVRAM;
bool b_BGFadeIn = false;
bool b_BGFadeOut = false;

/*========== Ints ==========*/

// Options stuff
int i_numberOptions;
int i_selectedOption;

// Useful stuff

int i_ScreenHeight;
int i_ScreenWidth;

/*========== Floats ==========*/

float f_CharacterX;

// Effects stuff
float f_BGFadeIn = 0.0f;
float f_BGFadeInRate;

float f_BGFadeOut = 1.0f;
float f_BGFadeOutRate;

/*========== Chars ==========*/

char c_CharacterName[64];

char c_text[512];

char c_optionsText[3][128];

//Texture Loading Functions

void loadTextBoxTextures(){
    gsKit_texture_png(gsGlobal, &TextBox, "mass:images/gui/textbox.png");
    gsKit_texture_png(gsGlobal, &TextBoxEdge, "mass:images/gui/textboxedge.png");
}

void loadBGTextures(){
    gsKit_texture_png(gsGlobal, &BGTexture, c_BGTextureName);
}

void loadCharacterTextures(){
    gsKit_texture_png(gsGlobal,&CharacterTexture,c_CharacterTextureName);
}
 

// Functions for render targets (needs to be in header)

void setRenderCharacterSprite(bool option){
    b_renderCharacterSprite = option;
}

void setRenderTextBox(bool option){
    b_renderTextBox = option;
}

void setRenderText(bool option){
    b_renderText = option;
}

void setRenderNameBox(bool option){
    b_renderNameBox = option;
}

void setRenderOptions(bool option){
    b_renderOptions = option;
}

void setFadeBGIn(float BGFadeInRate){
    f_BGFadeInRate = BGFadeInRate;
    f_BGFadeIn = 0.0f;
    b_BGFadeIn = true;
}

void setFadeBGOut(float BGFadeOutRate){
    f_BGFadeOutRate = BGFadeOutRate;
    f_BGFadeOut = 1.0f;
    b_BGFadeOut = true;
}


// Check if we need to clear VRam and what kind of rendeing scene we're in
//This is bad, We should only be clearing the Character and BG as needed...
void reloadVRam(){
    gsKit_vram_clear(gsGlobal);
    printf("CLEARED VRAM\n");
    loadBGTextures();
    loadTextBoxTextures();
    loadCharacterTextures();
    b_reloadVRAM = false;
}

//Other Public Functions

void setBGTextureName(char spriteName[64]){
    if (!(spriteName == c_BGTextureName)){
        memcpy(c_BGTextureName,spriteName,64);
        b_reloadVRAM = true;
    }
}

void setCharacterSprite(char spriteName[64]){
    if (!(spriteName == c_CharacterTextureName)){
        memcpy(c_CharacterTextureName, spriteName,64);
        b_reloadVRAM = true;
    }
    
}

void setCharacterPosition(float XPos){
    f_CharacterX = XPos;
}

void setCharacterName(char name[64]){
    memcpy(c_CharacterName,name,64);
}

void setText(char text[512]){
    memcpy(c_text,text,512);
}

//Max 3 Options, this is arbitrary, but i dont wanna write the code to dynamically generage options boxes
void setOptions(int numOptions, char OptionsText[3][128]){
    for (int i = 0; i < numOptions; i++){
        memcpy(c_optionsText[i], OptionsText[i],128);
    }
    i_numberOptions = numOptions;
}

void setOptionSelection(int selected){
    i_selectedOption = selected;
}

//Internal
u64 isSelected(int number){
    if (i_selectedOption == number){
        return TexCol;
    } else {
        return TexColUnSelected;
    }
}


void initTextureManager(){
    gsGlobal = gsKit_init_global();
    /* As far as i can tell, you dont actually need to init textures, the memory can manage itself hehhehhehheh
    // ^ That is wrong and you are stupid for thinking that (probably)
    // Textures
    // Dynamic
    initTexture(BGTexture, GS_PSM_T8);
    initTexture(CharacterTexture, GS_PSM_CT16);
    // Static
    initTexture(TextBox, GS_PSM_CT16);
    initTexture(TextBoxEdge, GS_PSM_CT16);
    */

	gsGlobal->PSM = GS_PSM_CT32;
	gsGlobal->Interlace = GS_INTERLACED;
	gsGlobal->ZBuffering = GS_SETTING_OFF;
	gsGlobal->Aspect = GS_ASPECT_4_3;

	dmaKit_init(D_CTRL_RELE_OFF,D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
		    D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

    dmaKit_chan_init(DMA_CHANNEL_GIF);
	gsGlobal->PrimAlphaEnable = GS_SETTING_ON;
	gsKit_init_screen(gsGlobal);
    gsKit_clear(gsGlobal, White);

    gsKit_mode_switch(gsGlobal, GS_ONESHOT);

    textFont = gsKit_init_fontm();
    gsKit_fontm_upload(gsGlobal,textFont);
    // Default Spacing
    textFont->Spacing = 0.65f;

    // Set screen height and width
    i_ScreenHeight = gsGlobal->Height;
    i_ScreenWidth = gsGlobal->Width;

}

void printTexInfo(GSTEXTURE Texture){
	printf("Texture Info:\n\tH: %i,\n\tW: %i,\n\tSize: %i,\n\tPSM: %c\n\n",Texture.Height,Texture.Width,gsKit_texture_size(Texture.Width,Texture.Height,Texture.PSM),Texture.PSM);
    printf("Texture VRAM Range: 0x%X - 0x%X\n",Texture.Vram, Texture.Vram +gsKit_texture_size(Texture.Width, Texture.Height, Texture.PSM) - 1);
}

//Render a text box, takes X and Y, and lets you extend it relatively, as well as changing color offset
void renderTextBox(float startX, float startY, float Relwidth, float Relheight, u64 colorOffset){
        // Calculate Relative Position to the starting point
    float width = (Relwidth + startX);
    float height = (Relheight + startY);
                // Edges currently do not account for scaling, too bad!
                // Starting Edge
    gsKit_prim_sprite_texture(gsGlobal, &TextBoxEdge, startX,  // X1
					startY,  // Y1
					0.0f,  // U1
					0.0f,  // V1
					(startX + TextBoxEdge.Width), // X2
					height, // Y2
					TextBoxEdge.Width, // U2
					TextBoxEdge.Height, // V2
					2,
					colorOffset);

                // Ending Edge
    gsKit_prim_sprite_texture(gsGlobal, &TextBoxEdge, width,  // X1
					startY,  // Y1
					0.0f,  // U1
					0.0f,  // V1
					(width - TextBoxEdge.Width), // X2
					height, // Y2
					TextBoxEdge.Width, // U2
					TextBoxEdge.Height, // V2
					2,
					colorOffset);
                
                // Main Text Box Texture
    gsKit_prim_sprite_texture(gsGlobal, &TextBox, (startX + TextBoxEdge.Width),  // X1, Cut pixels for edge
					startY,  // Y1
					0.0f,  // U1
					0.0f,  // V1
					(width - TextBoxEdge.Width), // X2, Cut pixels for edge
					height, // Y2
					TextBox.Width, // U2
					TextBox.Height, // V2
					2,
					colorOffset);
}

void renderNameBox(){
    renderTextBox(75,317,(40 + (strlen(c_CharacterName) * 16)),30,TexCol);
	gsKit_fontm_print(gsGlobal, textFont, 87, 319, 2, TexCol, c_CharacterName);
}

void renderOptions(){
    // Middle - Option 1
    renderTextBox(80,160,(640 - (80*2)),35,isSelected(1));
	gsKit_fontm_print_scaled(gsGlobal,textFont,(320 - ((strlen(c_optionsText[0]) * 11) / 2)),168,2,0.65f,isSelected(1),c_optionsText[0]);
    // Bottom - Option 2
    renderTextBox(80,210,(640 - (80*2)),35,isSelected(2));
	gsKit_fontm_print_scaled(gsGlobal,textFont,(320 - ((strlen(c_optionsText[1]) * 11) / 2)),218,2,0.65f,isSelected(2),c_optionsText[1]);
    // Top - Option 3
    if (i_numberOptions >= 3){
        renderTextBox(80,110,(640 - (80*2)),35,isSelected(3));
		gsKit_fontm_print_scaled(gsGlobal,textFont,(320 - ((strlen(c_optionsText[2]) * 11) / 2)),118,2,0.65f,isSelected(3),c_optionsText[2]);
    }
}

void fadeHandler(){
    //fucjking floating point bullshit
    if ((f_BGFadeOut > 0.000001f) && b_BGFadeOut) {
        f_BGFadeOut -= f_BGFadeOutRate;
        BGTexCol = GS_SETREG_RGBAQ((0x80 * f_BGFadeOut),(0x80 * f_BGFadeOut),(0x80 * f_BGFadeOut),0x80,0x00);
    } else {
        b_BGFadeOut = false;
    }


    if ((f_BGFadeIn <= 1.0f) && b_BGFadeIn) {
        f_BGFadeIn += f_BGFadeInRate;
        BGTexCol = GS_SETREG_RGBAQ((0x80 * f_BGFadeIn),(0x80 * f_BGFadeIn),(0x80 * f_BGFadeIn),0x80,0x00);
    } else {
        b_BGFadeIn = false;
    }

}




void renderFuncLoop(){
    // Background
    fadeHandler();
    if (b_reloadVRAM) reloadVRam();
    gsKit_prim_sprite_texture(gsGlobal, &BGTexture,	0.0f,  // X1
							0.0f,  // Y1
							0.0f,  // U1
							0.0f,  // V1
							BGTexture.Width, // X2
							BGTexture.Height, // Y2
							BGTexture.Width, // U2
							BGTexture.Height, // V2
							2,
							BGTexCol);
    //Character
    if (b_renderCharacterSprite){
        gsKit_prim_sprite_texture(gsGlobal, &CharacterTexture,	(0.0f + f_CharacterX),  // X1
							(480 - CharacterTexture.Height),  // Y1 // Calculate offset so character is touching the bottom of the screen rather then the top
							0.0f,  // U1
							0.0f,  // V1
							(CharacterTexture.Width + f_CharacterX), // X2
							480, // Y2
							CharacterTexture.Width, // U2
							CharacterTexture.Height, // V2
							2,
							TexCol);
    }
    //Textbox
    if (b_renderTextBox) renderTextBox(40, 340, (i_ScreenWidth - 80), 100, TexCol);
    //Text (TEMP whilke i build a function)
    if (b_renderText) gsKit_fontm_print_scaled(gsGlobal, textFont, 50, 350, 2, 0.80f, TexCol, c_text);
    //Namebox
    if (b_renderNameBox) renderNameBox();
    //Options TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (b_renderOptions) renderOptions();
    
    

    gsKit_sync_flip(gsGlobal);
	gsKit_queue_exec(gsGlobal);
	gsKit_queue_reset(gsGlobal->Per_Queue);
}

