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
void renderTextBox(float startX, float startY, float Relwidth, float Relheight, GSGLOBAL gsGlobal);
void loadTextBoxTextures(GSGLOBAL gsGlobal);


int main(int argc, char *argv[])
{
	GSGLOBAL *gsGlobal;

	GSTEXTURE Tex1;
	GSTEXTURE Tex2;

	u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
	u64 TexColAlpha = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x00,0x00);
	u64 White = GS_SETREG_RGBAQ(0xFF,0xFF,0xFF,0x00,0x00);
	
	gsGlobal = gsKit_init_global();

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

	

	gsKit_mode_switch(gsGlobal, GS_ONESHOT);

	float moveTestX;
	bool flipTest = true;

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
							0.0f,  // Y2
							0.0f,  // U1
							0.0f,  // V1
							Tex2.Width + moveTestX, // X2
							Tex2.Height, // Y2
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
			printf("Texture 1 VRAM Range: 0x%X - 0x%X\n",Tex1.Vram, Tex1.Vram +gsKit_texture_size(Tex1.Width, Tex1.Height, Tex1.PSM) - 1);
			printf("Texture 2 VRAM Range: 0x%X - 0x%X\n",Tex2.Vram, Tex2.Vram +gsKit_texture_size(Tex2.Width, Tex2.Height, Tex2.PSM) - 1);
			moveTestX = 0;
		}
        
		gsKit_sync_flip(gsGlobal);
		gsKit_queue_exec(gsGlobal);
		gsKit_queue_reset(gsGlobal->Per_Queue);
	}

	return 0;
}


void renderTextBox(float startX, float startY, float Relwidth, float Relheight, GSGLOBAL gsGlobal){
        // Calculate Relative Position to the starting point
    float width = (Relwidth + startX);
    float height = (Relheight + startY);

                // Starting Edge
    gsKit_prim_sprite_texture(gsGlobal, &TextBoxEdge, startX,  // X1
					startY,  // Y1
					0.0f,  // U1
					0.0f,  // V1
					width, // X2
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
					TextBoxEdge.Width, // U2
					TextBoxEdge.Height, // V2
					2,
					TexCol);
}

void loadTextBoxTextures(GSGLOBAL gsGlobal){
    gsKit_texture_png(gsGlobal, &TextBox, "host:images/gui/textbox.png");
    gsKit_texture_png(gsGlobal, &TextBoxEdge, "host:images/gui/textboxedge.png");
}