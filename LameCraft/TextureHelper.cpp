#include "TextureHelper.h"
#include <Aurora/Graphics/RenderManager.h>

TextureHelper::TextureHelper()
{
	defaulPatch = "Assets/Textures/";
	defaultFolder = "Default/";
	texturePatch = defaulPatch+defaultFolder;
}

TextureHelper::~TextureHelper()
{

}

void TextureHelper::Init()
{
    LoadTexturePackPath();
    texturePatch = defaulPatch+defaultFolder;

    FastLoadTexture("title/genesis.png");
    FastLoadTexture("environment/moon.png");
    FastLoadTexture("hud/utils.png");
    FastLoadTexture("title/dirt.png");
    FastLoadTexture("terrain.png");
    FastLoadTexture("terrain_mips.png");
    FastLoadTexture("water_still.png");
    FastLoadTexture("lava_still.png");
    FastLoadTexture("items.png");
    FastLoadTexture("hud/blue.png");
    FastLoadTexture("environment/sun.png");
    FastLoadTexture("environment/moon.png");
    FastLoadTexture("environment/stars.png");
    FastLoadTexture("entity/snowball.png");
    FastLoadTexture("entity/zombie.png");
    FastLoadTexture("entity/steve.png");
    FastLoadTexture("title/logo.png");
    FastLoadTexture("title/choose_rectangles.png");
    FastLoadTexture("title/menu_elements.png");
    FastLoadTexture("title/load.png");
    FastLoadTexture("hud/red.png");
    FastLoadTexture("hud/black.png");
    FastLoadTexture("hud/pumpkinblur.png");
    FastLoadTexture("environment/clouds.png");
    FastLoadTexture("gui/inventory1.png");
    FastLoadTexture("gui/inventory_creative.png");
    FastLoadTexture("gui/crafting_table.png");
    FastLoadTexture("gui/chest.png");
    FastLoadTexture("gui/furnace.png");
    FastLoadTexture("gui/cursor.png");
    FastLoadTexture("gui/slider.png");
    FastLoadTexture("gui/fire.png");
	FastLoadTexture("gui/arrow.png");
	FastLoadTexture("gui/cell.png");
	FastLoadTexture("armor/cloth_1.png");
	FastLoadTexture("armor/cloth_2.png");
	FastLoadTexture("armor/chain_1.png");
	FastLoadTexture("armor/chain_2.png");
	FastLoadTexture("armor/iron_1.png");
	FastLoadTexture("armor/iron_2.png");
	FastLoadTexture("armor/diamond_1.png");
	FastLoadTexture("armor/diamond_2.png");
	FastLoadTexture("armor/gold_1.png");
	FastLoadTexture("armor/gold_2.png");
	FastLoadTexture("entity/cow.png");
    FastLoadTexture("particle/smoke.png");
	FastLoadTexture("particle/flame.png");
	FastLoadTexture("particle/note.png");
	FastLoadTexture("particle/bubble.png");
	FastLoadTexture("particle/explosion.png");
	FastLoadTexture("colormap/grass.png");
	FastLoadTexture("item/clock.png");
	FastLoadTexture("item/compass.png");
	FastLoadTexture("environment/sky_color.png");
	FastLoadTexture("environment/rain.png");
	FastLoadTexture("environment/snow.png");
	FastLoadTexture("entity/creeper.png");
	FastLoadTexture("entity/sheep.png");
	FastLoadTexture("entity/sheep_fur.png");
	FastLoadTexture("title/background.png");
}

int TextureHelper::GetTexture(Textures texture)
{
	switch(texture)
	{
		case Genesis:
		{
			fileName = texturePatch + "title/genesis.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Logo:
		{
			fileName = texturePatch + "environment/moon.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Utils:
		{
			fileName = texturePatch + "hud/utils.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Dirt:
		{
			fileName = texturePatch + "title/dirt.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Terrain1:
		{
			fileName = texturePatch + "terrain.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case Terrain2:
		{
			fileName = texturePatch + "terrain_mips.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case WaterAnimation:
		{
			fileName = texturePatch + "water_still.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case LavaAnimation:
		{
			fileName = texturePatch + "lava_still.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case Items1:
		{
			fileName = texturePatch + "items.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Blue:
		{
			fileName = texturePatch + "hud/blue.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Sun:
		{
			fileName = texturePatch + "environment/sun.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case Moon:
		{
			fileName = texturePatch + "environment/moon.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case Stars:
		{
			fileName = texturePatch + "environment/stars.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case SnowBall3:
		{
			fileName = texturePatch + "entity/snowball.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case zombieTexture:
		{
			fileName = texturePatch + "entity/zombie.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case Steve:
		{
			fileName = texturePatch + "entity/steve.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case lameCraft:
		{
			fileName = texturePatch + "title/logo.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Buttons:
		{
			fileName = texturePatch + "title/menu_elements.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case Rectangles:
		{
			fileName = texturePatch + "title/choose_rectangles.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Load:
		{
			fileName = texturePatch + "title/load.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Red:
		{
			fileName = texturePatch + "hud/red.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Black:
		{
			fileName = texturePatch + "hud/black.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case PumpkinMask:
		{
			fileName = texturePatch + "hud/pumpkinblur.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case clouds1:
		{
			fileName = texturePatch + "environment/clouds.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case inv:
		{
			fileName = texturePatch + "gui/inventory1.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case invCreative:
		{
			fileName = texturePatch + "gui/inventory_creative.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case crt:
		{
			fileName = texturePatch + "gui/crafting_table.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case cht:
		{
			fileName = texturePatch + "gui/chest.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case fur:
		{
			fileName = texturePatch + "gui/furnace.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case cursor:
		{
			fileName = texturePatch + "gui/cursor.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case invSlider:
		{
			fileName = texturePatch + "gui/slider.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case furFire:
		{
			fileName = texturePatch + "gui/fire.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case furArrow:
		{
			fileName = texturePatch + "gui/arrow.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case invCell:
		{
			fileName = texturePatch + "gui/cell.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case leatherArmor1:
		{
			fileName = texturePatch + "armor/cloth_1.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case leatherArmor2:
		{
			fileName = texturePatch + "armor/cloth_2.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case chainmailArmor1:
		{
			fileName = texturePatch + "armor/chain_1.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case chainmailArmor2:
		{
			fileName = texturePatch + "armor/chain_2.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case ironArmor1:
		{
			fileName = texturePatch + "armor/iron_1.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case ironArmor2:
		{
			fileName = texturePatch + "armor/iron_2.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case diamondArmor1:
		{
			fileName = texturePatch + "armor/diamond_1.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case diamondArmor2:
		{
			fileName = texturePatch + "armor/diamond_2.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case goldenArmor1:
		{
			fileName = texturePatch + "armor/gold_1.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case goldenArmor2:
		{
			fileName = texturePatch + "armor/gold_2.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case cowTexture:
		{
			fileName = texturePatch + "entity/cow.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case smokeTexture:
		{
			fileName = texturePatch + "particle/smoke.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case flameTexture:
		{
			fileName = texturePatch + "particle/flame.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case noteTexture:
		{
			fileName = texturePatch + "particle/note.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case bubbleTexture:
		{
			fileName = texturePatch + "particle/flame.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case explosionTexture:
		{
			fileName = texturePatch + "particle/explosion.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case vegetationColorMap:
		{
			fileName = texturePatch + "colormap/grass.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case clockTexture:
		{
			fileName = texturePatch + "item/clock.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case compassTexture:
		{
			fileName = texturePatch + "item/compass.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case skyColorTexture:
		{
			fileName = texturePatch + "environment/sky_color.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case rainTexture:
		{
			fileName = texturePatch + "environment/rain.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case snowTexture:
		{
			fileName = texturePatch + "environment/snow.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case creeperTexture:
		{
			fileName = texturePatch + "entity/creeper.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case sheepTexture:
		{
			fileName = texturePatch + "entity/sheep.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case sheepFurTexture:
		{
			fileName = texturePatch + "entity/sheep_fur.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case background:
		{
			fileName = texturePatch + "title/background.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
	}
	return 0;
}

void TextureHelper::SetTexturePack(std::string name)
{
    TextureManager::Instance()->RemoveTextures();

	SetTextureFolderName(name+"/");

    FastLoadTexture("environment/moon.png");
    FastLoadTexture("hud/utils.png");
    FastLoadTexture("title/dirt.png");
    FastLoadTexture("terrain.png");
    FastLoadTexture("terrain_mips.png");
    FastLoadTexture("water_still.png");
    FastLoadTexture("lava_still.png");
    FastLoadTexture("items.png");
    FastLoadTexture("hud/blue.png");
    FastLoadTexture("environment/sun.png");
    FastLoadTexture("environment/moon.png");
    FastLoadTexture("environment/stars.png");
    FastLoadTexture("entity/snowball.png");
    FastLoadTexture("entity/zombie.png");
    FastLoadTexture("entity/steve.png");
    FastLoadTexture("title/logo.png");
    FastLoadTexture("title/choose_rectangles.png");
    FastLoadTexture("title/menu_elements.png");
    FastLoadTexture("title/load.png");
    FastLoadTexture("hud/red.png");
    FastLoadTexture("hud/black.png");
    FastLoadTexture("hud/pumpkinblur.png");
    FastLoadTexture("environment/clouds.png");
    FastLoadTexture("gui/inventory1.png");
    FastLoadTexture("gui/inventory_creative.png");
    FastLoadTexture("gui/crafting_table.png");
    FastLoadTexture("gui/chest.png");
    FastLoadTexture("gui/furnace.png");
    FastLoadTexture("gui/cursor.png");
    FastLoadTexture("gui/slider.png");
    FastLoadTexture("gui/fire.png");
	FastLoadTexture("gui/arrow.png");
	FastLoadTexture("gui/cell.png");
	FastLoadTexture("armor/cloth_1.png");
	FastLoadTexture("armor/cloth_2.png");
	FastLoadTexture("armor/chain_1.png");
	FastLoadTexture("armor/chain_2.png");
	FastLoadTexture("armor/iron_1.png");
	FastLoadTexture("armor/iron_2.png");
	FastLoadTexture("armor/diamond_1.png");
	FastLoadTexture("armor/diamond_2.png");
	FastLoadTexture("armor/gold_1.png");
	FastLoadTexture("armor/gold_2.png");
	FastLoadTexture("entity/cow.png");
	FastLoadTexture("particle/smoke.png");
	FastLoadTexture("particle/flame.png");
	FastLoadTexture("particle/note.png");
	FastLoadTexture("particle/bubble.png");
	FastLoadTexture("particle/explosion.png");
	FastLoadTexture("colormap/grass.png");
	FastLoadTexture("item/clock.png");
	FastLoadTexture("item/compass.png");
    FastLoadTexture("environment/sky_color.png");
    FastLoadTexture("environment/rain.png");
    FastLoadTexture("environment/snow.png");
    FastLoadTexture("entity/creeper.png");
    FastLoadTexture("entity/sheep.png");
    FastLoadTexture("entity/sheep_fur.png");
    FastLoadTexture("title/background.png");
}

void TextureHelper::FastRemoveTexture(std::string name)
{

}

void TextureHelper::RemoveConstTextures()
{
    TextureManager::Instance()->RemoveConstTextures();
}

void TextureHelper::FastLoadTexture(std::string name)
{
    std::string endName = texturePatch+name;
    TextureManager::Instance()->LoadTexture(endName);
}

void TextureHelper::SetTextureFolderName(std::string name)
{
	defaultFolder = name;
	texturePatch = defaulPatch + defaultFolder;
	SaveTexturePackPath();
}

void TextureHelper::SaveTexturePackPath()
{
	unsigned int pathLength = defaultFolder.size();

	FILE * pFile;
	pFile = fopen("Config/texturepack.cf","wb");

	if(pFile == NULL)
		return;

    fwrite(&pathLength,sizeof(unsigned int),1,pFile);
	fwrite(defaultFolder.c_str(),1,pathLength,pFile);

	//close file
	fclose(pFile);
}

void TextureHelper::LoadTexturePackPath()
{
	unsigned int pathLength = 0;

	FILE * pFile;
	pFile = fopen("Config/texturepack.cf","rb");

    if(pFile == NULL)
    {
        defaultFolder = "Default/";
        return;
    }


    fread(&pathLength,sizeof(unsigned int),1,pFile);
	char* c = new char[pathLength+1];
    fread(c,1,pathLength,pFile);
    c[pathLength] = '\0';
    defaultFolder = c;
    delete[] c;

	//close file
	fclose(pFile);

	if(fileExists(defaulPatch+defaultFolder+"terrain.png") == false) // if this texture pack doesn't exist at the moment
    {
        defaultFolder = "Default/"; // return to default one
    }
}

inline bool TextureHelper::fileExists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}


TextureHelper TextureHelper::m_TextureHelper;

TextureHelper* TextureHelper::Instance()
{
	return &m_TextureHelper;
}
