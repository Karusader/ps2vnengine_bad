
/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2005, ps2dev - http://www.ps2dev.org
# Licenced under GNU Library General Public License version 2
# Review ps2sdk README & LICENSE files for further details.
#
# audsrv sample
*/

#include <stdio.h>
#include <string.h>

#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <tamtypes.h>
#include <stdbool.h>



#include "soundManager.h"
#include "ngdn_defines.h"

#include <audsrv.h>



void initSoundManager(){
    int ret;
	SifInitRpc(0);

	printf("sample: kicking IRXs\n");
	ret = SifLoadModule("rom0:LIBSD", 0, NULL);
	printf("libsd loadmodule %d\n", ret);

	printf("sample: loading audsrv\n");
	ret = SifLoadModule(NGDN_STORAGE_TYPE "audsrv.irx", 0, NULL);
	printf("audsrv loadmodule %d\n", ret);

	ret = audsrv_init();
	if (ret != 0)
	{
		printf("sample: failed to initialize audsrv\n");
		printf("audsrv returned error string: %s\n", audsrv_get_error_string());
		return;
	}

    audsrv_set_volume(MAX_VOLUME);

}
//BGM
FILE *BGMWav;
bool b_BGMEnabled = false;
struct audsrv_fmt_t BGMformat;
int i_bgmRet;
char BGMchunk[2048];
//SFX
FILE *SFXWav;
bool b_SFXEnabled = false;
struct audsrv_fmt_t SFXformat;
int i_SFXRet;
char SFXchunk[2048];



void setBGMEnabled(bool enabled){
    b_BGMEnabled = enabled;
}

void setSFXEnabled(bool enabled){
    b_SFXEnabled = enabled;
}


void playbgm(char bgmFile[64], int bgmBits, int bgmFreq, int bgmChannels)
{
    fclose(BGMWav);
	int err;

	BGMformat.bits = bgmBits;
	BGMformat.freq = bgmFreq;
	BGMformat.channels = bgmChannels;

	err = audsrv_set_format(&BGMformat);
	printf("set format returned %d\n", err);
	printf("audsrv returned error string: %s\n", audsrv_get_error_string());

	BGMWav = fopen(bgmFile, "rb");
	if (BGMWav == NULL)
	{
		printf("BGM: failed to open wav file\n");
		return;
	}
	return;
}


void playSFX(char sfxFile[64], int sfxBits, int sfxFreq, int sfxChannels){

    fclose(SFXWav);
	int err;

	SFXformat.bits = sfxBits;
	SFXformat.freq = sfxFreq * 2;
	SFXformat.channels = sfxChannels;

	audsrv_set_format(&SFXformat);
	//printf("set format returned %d\n", err);
	//printf("audsrv returned error string: %s\n", audsrv_get_error_string());

	SFXWav = fopen(sfxFile, "rb");
	if (SFXWav == NULL)
	{
		printf("SFX: failed to open wav file\n");
		return;
	}
	return;
}



void soundFuncLoop(){

	// Disable the BGM when you play a sound effect, this shouldnt need to happen!
	// TODO: Figure out why the fuck audio breaks when you try to play 2 tracks at once
	// Im also 83% sure that we are streaming audio from the disc... Thats probably ok...
    if (b_BGMEnabled && !b_SFXEnabled){
    i_bgmRet = fread(BGMchunk, 1, sizeof(BGMchunk), BGMWav);
		if (i_bgmRet > 0)
		{
			audsrv_wait_audio(i_bgmRet);
			audsrv_play_audio(BGMchunk, i_bgmRet);
		}
		if (i_bgmRet < sizeof(BGMchunk))
		{
			/* no more data */
			printf("Attemping to loop playback...\n");
			fseek(BGMWav, 0, SEEK_SET);
			//break;
		}
    }
    if (b_SFXEnabled) {
		i_SFXRet = fread(SFXchunk, 1, sizeof(SFXchunk), SFXWav);
		if (i_SFXRet > 0)
		{
			audsrv_wait_audio(i_SFXRet);
			audsrv_play_audio(SFXchunk, i_SFXRet);
		}

		if (i_SFXRet < sizeof(SFXchunk))
		{
			/* no more data */
			audsrv_set_format(&BGMformat);
			printf("SFX Finished! Resetting Audio Format...\n");
            fclose(SFXWav);
            b_SFXEnabled = false;
		}
	}
    
}