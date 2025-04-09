#include "PlayerHud.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"

void APlayerHud::DrawHUD()
{
	Super::DrawHUD();

	if (CrosshairTex)
	{
        // Coordonn�es du centre de l'�cran
        float CenterX = Canvas->ClipX / 2.0f;
        float CenterY = Canvas->ClipY / 2.0f;

        // Taille du r�ticule (ici 32x32 pixels)
        float SizeX = 32.0f;
        float SizeY = 32.0f;

        // Dimensions de la texture (en supposant que la texture est carr�e)
        float TextureU = 0.0f;  // Coordonn�e U de la texture (commence g�n�ralement � 0)
        float TextureV = 0.0f;  // Coordonn�e V de la texture (commence g�n�ralement � 0)
        float TextureUWidth = 1.0f;  // Largeur de la texture (en proportion, ici pleine texture)
        float TextureVHeight = 1.0f; // Hauteur de la texture (en proportion, ici pleine texture)

        // Dessiner la texture du r�ticule au centre de l'�cran
        DrawTexture(CrosshairTex, CenterX - SizeX / 2.0f, CenterY - SizeY / 2.0f, SizeX, SizeY, TextureU, TextureV, TextureUWidth, TextureVHeight, FLinearColor::White, BLEND_Translucent, 1.0f);

	}
}
