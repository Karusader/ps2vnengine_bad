

#include <stdio.h>
#include <malloc.h>

#include <gsKit.h>
#include <pthread.h>

#include "soundManager.h"
#include "textureManager.h"
#include "input.h"
#include "game.h"
#include "main.h"

bool runGame = true;

void stopTheFuckingGame(){
    printf("stopping the fucking game");
    runGame = false;
}


int main(int argc, char *argv[]){


    bool debug = false;

    initTextureManager();
    initSoundManager();
    initGameSystem();
    initInput();

    setGameMode(gm_VISUALNOVEL);

    if (debug){
    setBGTextureName("host:images/kara_room_bg.png");

    setRenderTextBox(true);

    setText("We hit that #1 victory royale!");
    setRenderText(true);

    setCharacterName("test");

    setOptions(3, (char[3][128]){"images/kara/neutral.png", "images/kara/neutral2.png", "images/kara/sharkie.png"});
    setOptionSelection(1);
    setRenderOptions(true);

    setCharacterSprite("host:images/kara/sharkie.png");
    setCharacterPosition(POSITION_LEFT);
    setGameMode(gm_VISUALNOVELOPTION);
    setRenderCharacterSprite(true);

    setCharacterName("Kara");
    setRenderNameBox(true);



    playbgm("host:sound/Act1_nobark.wav",16,44100,2);
    setBGMEnabled(true);

    playSFX("host:sound/sfx/Kiss.wav",16,44100,1);
    setSFXEnabled(true);

    //setFadeBGOut(FADE_FAST);
    }
    //pthread_t soundThread;
    //int i_soundThread = pthread_create(&soundThread, NULL, soundFuncLoop, NULL);
    //int otherThing = pthread_detach(&soundThread);
    //int i_soundThread = pthread_create(&soundThread, NULL, soundFuncLoop, NULL);

    while (runGame)
    {
        // The amount of sound loop must match all the other functions, this is bad!
        //pthread_join( i_soundThread, NULL);
        inputLoop();
        gameLoop();
        
        
        
        
        //Render Last
        renderFuncLoop();
        //soundFuncLoop();
        //soundFuncLoop();
    }
    
    
    return 42069;
    // i have an immature sense of humor but thats ok
}