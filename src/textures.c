//  ____     ___ |    / _____ _____
// |  __    |    |___/    |     |
// |___| ___|    |    \ __|__   |     gsKit Open Source Project.
// ----------------------------------------------------------------------
// Copyright 2004 - Chris "Neovanglist" Gilbert <Neovanglist@LainOS.org>
// Licenced under Academic Free License version 2.0
// Review gsKit README & LICENSE files for further details.
//
// textures.c - Example demonstrating gsKit texture operation.
//

#include <stdio.h>
#include <malloc.h>

#include <gsKit.h>
#include <dmaKit.h>
#include <gsToolkit.h>


void printTexInfo(GSTEXTURE Texture){
	printf("Texture Info:\n\tH: %i,\n\tW: %i,\n\tSize: %i,\n\tPSM: %c\n\n",Texture.Height,Texture.Width,gsKit_texture_size(Texture.Width,Texture.Height,Texture.PSM),Texture.PSM);
}

//Render a text box, with 
void renderTextBox(float startX, float startY, float Relwidth, float Relheight, GSGLOBAL *gsGlobal, GSTEXTURE TextBox, GSTEXTURE TextBoxEdge);
void renderTextBox2(float startX, float startY, float Relwidth, float Relheight, GSGLOBAL *gsGlobal, GSTEXTURE TextBox, GSTEXTURE TextBoxEdge,u64 TextureColor);
void loadTextBoxTextures(GSGLOBAL *gsGlobal, GSTEXTURE *TextBox, GSTEXTURE *TextBoxEdge){
    gsKit_texture_png(gsGlobal, TextBox, "host:images/gui/textbox.png");
    gsKit_texture_png(gsGlobal, TextBoxEdge, "host:images/gui/textboxedge.png");
}
	u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
	u64 TexColSelected = GS_SETREG_RGBAQ(0x40,0x40,0x40,0x80,0x00);

int main(int argc, char *argv[])
{
	GSGLOBAL *gsGlobal;

	GSTEXTURE Tex1;
	GSTEXTURE Tex2;

	GSTEXTURE TextBox;
	GSTEXTURE TextBoxEdge;

	GSFONTM *fontThing;

	u64 White = GS_SETREG_RGBAQ(0xFF,0xFF,0xFF,0x00,0x00);
	
	gsGlobal = gsKit_init_global();


	fontThing = gsKit_init_fontm();

	Tex1.Width = 0;
	Tex1.Height = 0;
	Tex2.Width = 0;
	Tex2.Height = 0;
	Tex1.PSM = GS_PSM_CT16;
	Tex2.PSM = GS_PSM_CT16;

	gsGlobal->PSM = GS_PSM_CT32;
	gsGlobal->Interlace = GS_INTERLACED;
	gsGlobal->ZBuffering = GS_SETTING_OFF;
	gsGlobal->Aspect = GS_ASPECT_4_3;

	dmaKit_init(D_CTRL_RELE_OFF,D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
		    D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

	// Initialize the DMAC
	dmaKit_chan_init(DMA_CHANNEL_GIF);
	printf("alpha\n");
	gsGlobal->PrimAlphaEnable = GS_SETTING_ON;
	gsKit_init_screen(gsGlobal);
	//gsKit_TexManager_init(gsGlobal);

	//Load PNG into GSTEXTURE
	gsKit_clear(gsGlobal, White);
	gsKit_texture_png(gsGlobal, &Tex1, "host:test.png");
	printTexInfo(Tex1);
	printf("Texture 1 VRAM Range: 0x%X - 0x%X\n",Tex1.Vram, Tex1.Vram +gsKit_texture_size(Tex1.Width, Tex1.Height, Tex1.PSM) - 1);
	//printf("Texture 1 VRAM Size: %ikb\n",(gsKit_texture_size(Tex1.Width,Tex1.Height,Tex1.PSM)));

	gsKit_texture_png(gsGlobal, &Tex2, "host:test2.png");
	printTexInfo(Tex2);
	printf("Texture 2 VRAM Range = 0x%X - 0x%X\n",Tex2.Vram, Tex2.Vram +gsKit_texture_size(Tex2.Width, Tex2.Height, Tex2.PSM) - 1);

	loadTextBoxTextures(gsGlobal, &TextBox, &TextBoxEdge);

	printf("Double check loading textr  box thing ::::\n");
	printTexInfo(TextBox);

	gsKit_fontm_upload(gsGlobal,fontThing);
	fontThing->Spacing = 0.65f;

	

	gsKit_mode_switch(gsGlobal, GS_ONESHOT);

	float moveTestX;
	bool flipTest = true;

	char c_iFuckingSwear[3][5] = { "test", "test2"};
	char c_iFuckingSwear2[3][5];

	//c_iFuckingSwear2[1] = c_iFuckingSwear[1];
	memcpy(c_iFuckingSwear2[1], c_iFuckingSwear[1],5);
	memcpy(c_iFuckingSwear2[0], c_iFuckingSwear[0],5);

	printf("==========================the length of the thing is %i : %s\n", strlen(c_iFuckingSwear2[0]), c_iFuckingSwear2[0]);
	
	

	while(1)
	{
		gsKit_prim_sprite_texture(gsGlobal, &Tex1,	0.0f,  // X1
							0.0f,  // Y2
							0.0f,  // U1
							0.0f,  // V1
							Tex1.Width, // X2
							Tex1.Height, // Y2
							Tex1.Width, // U2
							Tex1.Height, // V2
							2,
							TexCol);

		gsKit_prim_sprite_texture(gsGlobal, &Tex2,	moveTestX,  // X1
							(480 - Tex2.Height),  // Y2
							0.0f,  // U1
							0.0f,  // V1
							Tex2.Width + moveTestX, // X2
							480, // Y2
							Tex2.Width, // U2
							Tex2.Height + moveTestX, // V2
							2,
							TexCol);
		
		
		//Don't render past screen
		moveTestX++;
		if (moveTestX >= 640){ 
			gsKit_vram_clear(gsGlobal);
			gsKit_texture_png(gsGlobal, &Tex1, "host:test.png");
			if (flipTest){
				flipTest = !flipTest;
				gsKit_texture_png(gsGlobal, &Tex2, "host:test3.png");
			} else {
				flipTest = !flipTest;
				gsKit_texture_png(gsGlobal, &Tex2, "host:test2.png");
			}
			loadTextBoxTextures(gsGlobal, &TextBox, &TextBoxEdge);
			printf("Texture 1 VRAM Range: 0x%X - 0x%X\n",Tex1.Vram, Tex1.Vram +gsKit_texture_size(Tex1.Width, Tex1.Height, Tex1.PSM) - 1);
			printf("Texture 2 VRAM Range: 0x%X - 0x%X\n",Tex2.Vram, Tex2.Vram +gsKit_texture_size(Tex2.Width, Tex2.Height, Tex2.PSM) - 1);
			moveTestX = 0;
		}
		//Main
		renderTextBox(40,340,(640 - 80),100,gsGlobal,TextBox,TextBoxEdge);
		//Sub
		renderTextBox(75,320,(40 + (strlen("Kara")) * 16),30,gsGlobal,TextBox,TextBoxEdge);
		gsKit_fontm_print(gsGlobal,fontThing,87,323,2,TexCol,"Kara");
		//Dom ;)
		gsKit_fontm_print_scaled(gsGlobal,fontThing,50,350,2,0.80f,TexCol,"I have my own fridge in the other room ,\nbut ...my toaster oven is broken right\nnow .");

		//Options
		//Middle
		renderTextBox(80,160,(640 - (80*2)),35,gsGlobal,TextBox,TextBoxEdge);
		gsKit_fontm_print_scaled(gsGlobal,fontThing,(320 - 224),165,2,0.65f,TexCol,"Do you like being a huggable plush shark?");
		//Bottom
		renderTextBox2(80,210,(640 - (80*2)),35,gsGlobal,TextBox,TextBoxEdge,TexColSelected);
		gsKit_fontm_print_scaled(gsGlobal,fontThing,(320 - 32),215,2,0.65f,TexColSelected,"Test");
		//Top
		renderTextBox2(80,110,(640 - (80*2)),35,gsGlobal,TextBox,TextBoxEdge,TexColSelected);
		gsKit_fontm_print(gsGlobal,fontThing,(320 - 32),115,2,TexColSelected,c_iFuckingSwear2[1]);

		printf(c_iFuckingSwear[1][0]);
        
		gsKit_sync_flip(gsGlobal);
		gsKit_queue_exec(gsGlobal);
		gsKit_queue_reset(gsGlobal->Per_Queue);
	}

	return 0;
}


void renderTextBox(float startX, float startY, float Relwidth, float Relheight, GSGLOBAL *gsGlobal, GSTEXTURE TextBox, GSTEXTURE TextBoxEdge){
        // Calculate Relative Position to the starting point
    float width = (Relwidth + startX);
    float height = (Relheight + startY);

                // Starting Edge
				//printf("edge: %f\n", (Relheight / TextBoxEdge.Height));
	
    gsKit_prim_sprite_texture(gsGlobal, &TextBoxEdge, startX,  // X1
					startY,  // Y1
					0.0f,  // U1
					0.0f,  // V1
					(startX + TextBoxEdge.Width), // X2
					height, // Y2
					TextBoxEdge.Width, // U2
					TextBoxEdge.Height, // V2
					2,
					TexCol);

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
					TexCol);
                
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
					TexCol);
}

void renderTextBox2(float startX, float startY, float Relwidth, float Relheight, GSGLOBAL *gsGlobal, GSTEXTURE TextBox, GSTEXTURE TextBoxEdge,u64 TextureColor){
        // Calculate Relative Position to the starting point
    float width = (Relwidth + startX);
    float height = (Relheight + startY);

                // Starting Edge
				//printf("edge: %f\n", (Relheight / TextBoxEdge.Height));
	
    gsKit_prim_sprite_texture(gsGlobal, &TextBoxEdge, startX,  // X1
					startY,  // Y1
					0.0f,  // U1
					0.0f,  // V1
					(startX + TextBoxEdge.Width), // X2
					height, // Y2
					TextBoxEdge.Width, // U2
					TextBoxEdge.Height, // V2
					2,
					TextureColor);

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
					TextureColor);
                
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
					TextureColor);
}
