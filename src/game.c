


#include <stdio.h>
#include <malloc.h>

#include <gsKit.h>
#include <input.h>
#include <stdbool.h>
#include <kernel.h>


#include <libpad.h>

#include "textureManager.h"
#include "external/CJSON.h"
#include "game.h"
#include "soundManager.h"
#include "main.h"
#include "ngdn_defines.h"




#define MaxFileSizeThing 256000

cJSON *root;
cJSON *script;

char c_jsonChar[MaxFileSizeThing + 1];

bool b_inOption;


int i_currentOption = 1;
int i_numberOptionsGame = 3;
int i_currentID = 0;
int i_gameMode;


int i_OptionJump[3] = {1,2,3};

struct gameData{
    int nextID;
    
    int NumberOptions;
    struct options{
        const char OptionText;
        const int OptionJump
    };

    const char SpritePath;
    const char Name;

    const char *Text;
    const char *NextScript
};



void readNewScript(char newScript[64]){
    //Clear varibles
    printf("Reading New Script...============================\n");
    //cJSON_Delete(script);
    //cJSON_Delete(root);
    //free(c_jsonChar);
    // stackoverflow my beloved
    // https://stackoverflow.com/questions/2029103/correct-way-to-read-a-text-file-into-a-buffer-in-c
    FILE *scriptFile = fopen(newScript, "r");
    if (scriptFile != NULL) {
        size_t newLen = fread(c_jsonChar,sizeof(char),MaxFileSizeThing, scriptFile);
        if ( ferror( scriptFile ) != 0 ) {
        fputs("Error reading file", stderr);
        } else {
        c_jsonChar[newLen++] = '\0'; /* Just to be safe. */
        }
        fclose(scriptFile);
    } else {
        printf("Could not find script : %s\n", newScript);
        return;
    }
    printf("Finished Reading Script...\n");
    
    root = cJSON_Parse(c_jsonChar);
    printf("Finished Parsing Script...\n");
    script = cJSON_GetObjectItem(root, "script");
    printf("Finished Processing Script...\n");

}


void loadID(int ID){
    cJSON *item = cJSON_GetArrayItem(script, ID);
    //printf(cJSON_Print(item));
    if (cJSON_GetObjectItem(item, "NextScript") != NULL){
        readNewScript(cJSON_GetObjectItem(item,"NextScript")->valuestring);
        i_currentID = 0;
        loadID(0);
        return;
    }

    if (cJSON_GetObjectItem(item, "Text") != NULL){
        printf("Found Text");
        char textInfo = cJSON_GetObjectItem(item,"Text")->valuestring;
        if (textInfo == "notext"){
            setRenderText(false);
            setRenderTextBox(false);
        } else {
            setText(cJSON_GetObjectItem(item, "Text")->valuestring);
            setRenderText(true);
            setRenderTextBox(true);
        }
    }

    if (cJSON_GetObjectItem(item, "Name") != NULL){
        setCharacterName(cJSON_GetObjectItem(item,"Name")->valuestring);
        setRenderNameBox(true);
    } else {
        setRenderNameBox(false);
    }
    
    if (cJSON_GetObjectItem(item, "Sprite") != NULL){
        if (cJSON_GetObjectItem(item,"Sprite")->valuestring == "nosprite") {
            setRenderCharacterSprite(false);
        } else {
            setCharacterSprite(cJSON_GetObjectItem(item,"Sprite")->valuestring);
            setRenderCharacterSprite(true);
        }
    }
    if (cJSON_GetObjectItem(item, "SpritePosition") != NULL){
        setCharacterPosition(cJSON_GetObjectItem(item,"SpritePosition")->valueint);
    }

    if (cJSON_GetObjectItem(item, "Background") != NULL){
        setBGTextureName(cJSON_GetObjectItem(item,"Background")->valuestring);
    }

    if (cJSON_GetObjectItem(item,"NumberOptions") != NULL){
        //Reset Current option in case the player left off on option 3 and the next box has no option 3
        i_currentOption = 1;
        //Placeholder thing in case its mad you dont have something in the third option
        
        char OptionsText[3][128] = {"","",""};
        cJSON *optionArray = cJSON_GetObjectItem(item, "Options");

        i_numberOptionsGame = cJSON_GetObjectItem(item,"NumberOptions")->valueint;

        for (int i = 0; i < i_numberOptionsGame; i++){
            cJSON *Option = cJSON_GetArrayItem(optionArray, i);
            memcpy(OptionsText[i], cJSON_GetObjectItem(Option,"OptionText")->valuestring ,128);
            i_OptionJump[i] = cJSON_GetObjectItem(Option,"OptionJump")->valueint;
            printf("Option %i jump set to %i\n",i,i_OptionJump[i]);
        }
        setOptions(i_numberOptionsGame, OptionsText);
        i_gameMode = gm_VISUALNOVELOPTION;
        setOptionSelection(1);
        setRenderOptions(true);
    }
    
    if (cJSON_GetObjectItem(item,"BGM") != NULL){
        playbgm(cJSON_GetObjectItem(item,"BGM")->valuestring, 16, 44100, 2);
        setBGMEnabled(true);
    }
    if (cJSON_GetObjectItem(item,"DisableBGM") != NULL){
        setBGMEnabled(false);
    }
    if (cJSON_GetObjectItem(item,"EnableBGM") != NULL){
        setBGMEnabled(true);
    }
    if (cJSON_GetObjectItem(item, "Jump") != NULL){
        loadID(cJSON_GetObjectItem(item,"Jump")->valueint);
        i_currentID = cJSON_GetObjectItem(item,"Jump")->valueint;
    }
    if (cJSON_GetObjectItem(item, "StopTheFuckingGame") != NULL){
        stopTheFuckingGame();
    }
    
    if (i_gameMode == gm_VISUALNOVEL){
        i_currentID += 1; // Queue Next Item
    }
    
    
}

void setGameMode(int gamemode){
    i_gameMode = gamemode;
}


//0 = 1, 1 = 2, null = 3...
void optionUp(){
    if (i_currentOption == OPTION_MIDDLE){
        if (i_numberOptionsGame == 2){
            i_currentOption = OPTION_LOWER;
            setOptionSelection(2);
        } else {
            i_currentOption = OPTION_UPPER;
            setOptionSelection(3);
        }
    } else 
    if (i_currentOption == OPTION_LOWER){
        i_currentOption = OPTION_MIDDLE;
        setOptionSelection(1);
    } else 
    if (i_currentOption == OPTION_UPPER){
        i_currentOption = OPTION_LOWER;
        setOptionSelection(2);
    }
}

void optionDown(){
    if (i_currentOption == OPTION_LOWER) {
        if (i_numberOptionsGame == 2) {
            i_currentOption = OPTION_MIDDLE;
            setOptionSelection(1);
        } else {
            i_currentOption = OPTION_UPPER;
            setOptionSelection(3);
        }
    } else
    if (i_currentOption == OPTION_MIDDLE){
        i_currentOption = OPTION_LOWER;
        setOptionSelection(2);
    } else
    if (i_currentOption == OPTION_UPPER){
        i_currentOption = OPTION_MIDDLE;
        setOptionSelection(1);
    }
}


//This function should not be for debugging, but  it is for now
void selectOption(){
    //setRenderOptions(false);
    if (i_currentOption == OPTION_MIDDLE){
        //setCharacterSprite("images/kara/neutral.png");
        //printf("selection test : %i\n", i_OptionJump[0]);
        i_currentID = i_OptionJump[0];
    } else
    if (i_currentOption == OPTION_LOWER){
        //printf("selection test : %i\n", i_OptionJump[1]);
        //setCharacterSprite("images/kara/neutral2.png");
        i_currentID = i_OptionJump[1];
    } else 
    if (i_currentOption == OPTION_UPPER){
        //printf("selection test : %i\n", i_OptionJump[2]);
        //setCharacterSprite("images/kara/sharkie.png");
        i_currentID = i_OptionJump[2];
    }
    i_gameMode = gm_VISUALNOVEL;
    setRenderOptions(false);
    loadID(i_currentID);
}

void initGameSystem(){

    printf("hi pukko!!!\n");
    readNewScript("mass:scripts/init.json");
    loadID(i_currentID);
    i_currentID += 1;
    

    


}

void gameLoop(){
    if (i_gameMode == gm_VISUALNOVELOPTION){
        if (getPadState() & PAD_UP) {
            optionUp();
        }
        if (getPadState() & PAD_DOWN) {
            optionDown();
        }
        if (getPadState() & PAD_CROSS) {
            selectOption();
        }
    }
    if (i_gameMode == gm_VISUALNOVEL){
        if (getPadState() & PAD_CROSS) {
            loadID(i_currentID);
        }
        
    }
}