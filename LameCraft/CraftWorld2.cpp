#include "CraftWorld2.h"
#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Utils/Logger.h>
#include "TextureHelper.h"


#include <string.h>
#include <zlib.h>
#define PI 3.14159f

#define ENGLISH 1
#define RUSSIAN 2

CraftWorld::CraftWorld()
{
    WORLD_SIZE = 0;
    CHUNK_SIZE = 0;
    WORLD_HEIGHT = 0;

    // Инициализируем переменные статистики
    mainStatistics.blockPlaced = 0;
    mainStatistics.blockDestroyed = 0;
    mainStatistics.daysInGame = 0;
    mainStatistics.minutesPlayed = 0;
    mainStatistics.itemsCrafted = 0;
    mainStatistics.itemsSmelted = 0;
    mainStatistics.jumps = 0;
    mainStatistics.dies = 0;
    mainStatistics.foodEaten = 0;
    mainStatistics.badlyFalls = 0;
    mainStatistics.blocksWalked = 0;
    mainStatistics.treesGrowned = 0;
    mainStatistics.cropsGrowned = 0;
    mainStatistics.soilPlowed = 0;
    mainStatistics.timeInAir = 0;
    mainStatistics.timeInWater = 0;
    mainStatistics.damageRecieved = 0;
    // end

    // Инициализируем переменные опций
    mainOptions.detailedSky = 1;
    mainOptions.smoothLighting = true;
    mainOptions.sounds = 1;
    mainOptions.music = 1;
    mainOptions.fogRendering = 1;
    mainOptions.fastRendering = true;
    mainOptions.mipMapTexturing = false;
    mainOptions.autoJump = 0;
    mainOptions.fov = 65;
    mainOptions.freezeDayTime = 0;
    mainOptions.worldBlockAnimation = 1;
    mainOptions.particles = 0;
    mainOptions.difficult = 2; // normal
    mainOptions.horizontalViewDistance = 4;
    mainOptions.verticalViewDistance = 1;
    mainOptions.guiDrawing = 1;
    // end

    // Инициализируем переменные мира
    mainWorldVariables.worldCoalOre = 0;
    mainWorldVariables.worldDiamondOre = 0;
    mainWorldVariables.worldFlowers = 0;
    mainWorldVariables.worldGoldenOre = 0;
    mainWorldVariables.worldIronOre = 0;
    mainWorldVariables.worldLapisOre = 0;
    mainWorldVariables.worldRedstoneOre = 0;
    mainWorldVariables.worldTrees = 0;
    mainWorldVariables.worldClay = 0;
    // end

    blockXConstLight = 0.65f;
    blockZConstLight = 0.85f;

    worldWeather = new WeatherSystem();

    snowBiomePosition = Vector3(0,0,0);
    snowBiomeRadius = -1;
    desertBiomePosition = Vector3(0,0,0);
    desertBiomeRadius = -1;

    worldTime = 0.0f;
    skyTime = 112.0f;

    playerZoneSize = Vector3(14*mainOptions.horizontalViewDistance,15*mainOptions.verticalViewDistance,14*mainOptions.horizontalViewDistance); // Зона прорисовки чанков (каждый параметр должен быть кратен размеру чанка)
    playerSpawnPointPosition = Vector3(128.5f,64,128.5f);
    playerContactLadder = false;

    updateChunksSwitch = false;
    chunksWasSetToUpdate = false;

    updateChunkTimer = 0.0f;
    updateTransparentChunkTimer = 0.12f;
    updateFastChunkTimer = 0.0f;
    fetchNearestChunksTimer = 2.0f;

    chunksCreatedInFrameCount = 0;
    transparentchunksCreatedInFrameCount = 0;
    chunksCreatedInFrameCount2 = 0;
    transparentchunksCreatedInFrameCount2 = 0;

    mId = -1;
    mAm = -1;
    mSt = 0;

    worldSeed = 0;

    HP = 20;
    HG = 20;
    OS = 10;
    AP = 0;

    playerGrowth = 1.65f;
    playerCollisionSize = 0.2f;

    worldTime = 0.0f;
    worldDayTime = 12.0f;
    worldHour = 0.0f;
    allChunksUpdateTimer = 0.0f;

    westLight = 0.0f;
    eastLight = 0.0f;

    worldVersion = 150;

    mChestsize = 0;
    mFurnacesize = 0;
    mZombiesize = 0;
    mDirectsize = 0;
    mCowsize = 0;
    mCreepersize = 0;
    mSheepsize = 0;
    mTNTsize = 0;
    mNoteBlocksize = 0;
    mJukeboxsize = 0;
    mMonsterSpawnersize = 0;
    mItemFramesize = 0;

    zombieNum = 0;//14
    dropNum = 0;
    TNTnum = 0;

    spawnCowsTimer = 0.0f;
    spawnZombiesTimer = 0.0f;
    spawnCreeperTimer = 0.0f;
    spawnSheepTimer = 0.0f;

    updatingChunksTimer = 0.0f;
    updatingChunksTimerMax = 1.0f;
    updatingChunksPosition = 0;

    gameModeWorld = 0;
    gameLocked = false;

    for(int o = 0; o <= 35; o += 1) // Инициализация инвентаря
    {
        invAm[o] = -1; // Количество вещей в ячейки = -1 (0)
        invId[o] = -1; // Id вещей в ячейке = -1 (ячейка пуста)
        invSt[o] = false; // Вещь в ячейке не стакается (по умолчанию)
    }

    for(int o = 0; o <= 3; o += 1)
    {
        armorId[o] = 324+o;
        armorAm[o] = -1;
        armorSt[o] = false;
    }

    minuteTimer = 0.0f;
    lightShadowFactor = 0.29f;
    lightFactor = Vector3(1.0f,1.0f,1.0f);

    /*startSkyColor = Vector3(184,211,255);
	endSkyColor = Vector3(119,184,255);
	//startSkyColor = Vector3(176,198,255);
	//endSkyColor = Vector3(158,157,255);
	dawnSunsetSkyColor = Vector3(255,163,58);*/

	dawnSunsetClearSkyColor = Vector3(0,0,0);
	startClearSkyColor = Vector3(0,0,0);
	endClearSkyColor = Vector3(0,0,0);

	dawnSunsetRainySkyColor = Vector3(0,0,0);
	startRainySkyColor = Vector3(0,0,0);
	endRainySkyColor = Vector3(0,0,0);

    dawnSunsetSkyColor = Vector3(0,0,0);
	startSkyColor = Vector3(0,0,0);
	endSkyColor = Vector3(0,0,0);

	rainyColorAlpha = 0.0f;

    brightFactor = 1.0f;
    starsFactor = 0.0f;
    dawnSunsetFactor = 0.0f;

    hatVertices = NULL;
    bodyVertices = NULL;
    headVertices = NULL;
    legVertices = NULL;
    handVertices = NULL;

    creeperLegVertices = NULL;

    cowBodyVertices = NULL;
    cowHeadVertices = NULL;
    cowLegVertices = NULL;
    cowHornsVertices = NULL;

    sheepBodyVertices = NULL;
    sheepHeadVertices = NULL;
    sheepHeadFurVertices = NULL;
    sheepLegFurVertices = NULL;
    sheepLegOPTVertices = NULL;
    sheepHeadOPTVertices = NULL;

    helmetVertices = NULL;
    shoulderVertices = NULL;
    chestplateVertices = NULL;
    bellyVertices = NULL;
    leggingVertices = NULL;
    bootVertices = NULL;

    kickedStart = false;
    kickedBy = 0;
    kickedDamage = 0;
    kickedAngle = 0.0f;
    kickedTimer = 0.0f;

    iVertexLeftRight = 0;
    pointsLeftRight = 0;
    iVertexBackFront = 0;
    pointsBackFront = 0;
    iVertexBottomTop = 0;
    pointsBottomTop = 0;

    textureTerrainId = 0;
    textureItemsId = 0;
    textureWaterAnimationId = 0;
    textureLavaAnimationId = 0;
    textureLeatherArmor1Id = 0;
    textureLeatherArmor2Id = 0;
    textureChainArmor1Id = 0;
    textureChainArmor2Id = 0;
    textureIronArmor1Id = 0;
    textureIronArmor2Id = 0;
    textureDiamondArmor1Id = 0;
    textureDiamondArmor2Id = 0;
    textureGoldenArmor1Id = 0;
    textureGoldenArmor2Id = 0;
    textureVegetationColorMapId = 0;
    textureTerrainMipsId = 0;
    textureClockId = 0;
    textureCompassId = 0;

    animationTimer = 0.0f;
    animationTimerEnd = 0.085f;
    animationWaterFrame = 0;
    animationLavaFrame = 0;

    animationLavaStep = true;

    clockFrame = -1;
    lastClockFrame = -1;

    compassFrame = -1;
    lastCompassFrame = -1;

    InitBlockVector();
    InitItemVector();
    UpdateBlocksAndItemsName();

   // FetchNearestChunks();

    spawner_x = 0;
    spawner_z = 0;

    /*PutInInventory(Scissors::getID(),60,0);
    PutInInventory(BoneMeal::getID(),16,1);
    PutInInventory(LapisLazuli::getID(),16,1);
    PutInInventory(CocoaBeans::getID(),16,1);

    for(int i = 365; i <= 376; i++)
    {
        PutInInventory(i,16,1);
    }*/
    /*PutInInventory(ItemFrameItem::getID(),64,1);
    PutInInventory(IronBarsItem::getID(),64,1);
    PutInInventory(GlassPanelItem::getID(),64,1);
    PutInInventory(IronBar::getID(),64,1);
    PutInInventory(Redstone::getID(),64,1);
    PutInInventory(CobbleStone::getID(),64,1);
    PutInInventory(CobbleStone::getID(),64,1);
    PutInInventory(Diamond::getID(),64,1);
    PutInInventory(Gunpowder::getID(),64,1);
    PutInInventory(SandBlock::getID(),64,1);
    PutInInventory(DiamondPickaxe::getID(),1562,0);
    PutInInventory(DiamondAxe::getID(),1562,0);
    PutInInventory(BedItem::getID(),16,1);
    PutInInventory(WhiteWoolBlock::getID(),5,1);
    PutInInventory(RedWoolBlock::getID(),5,1);
    PutInInventory(RoseRed::getID(),5,1);
    PutInInventory(PinkDye::getID(),5,1);
    PutInInventory(LapisLazuli::getID(),5,1);
    PutInInventory(Wheat::getID(),64,1);
    PutInInventory(Carrot::getID(),64,1);
    PutInInventory(Potato::getID(),64,1);*/
   /* PutInInventory(LeatherHelmet::getID(),24,0);
    PutInInventory(LeatherLeggings::getID(),32,0);
    PutInInventory(IronChestplate::getID(),82,0);
    PutInInventory(IronBoots::getID(),43,0);
    PutInInventory(JukeBox::getID(),2,1);
    PutInInventory(FlintAndSteel::getID(),50,0);
    PutInInventory(TNTBlock::getID(),50,1);*/
    percent = 0.1f;
    percent2 = 0.1f;
    percent3 = 0.1f;
    texutreSize = 0;
    textureChunk = 0;
}

void CraftWorld::InitBlockVector()
{
    //Air
    blockTypes.push_back(BaseBlock());
    //Classic blocks
    blockTypes.push_back(GrassBlock());
    blockTypes.push_back(DirtBlock());
    blockTypes.push_back(RockBlock());
    blockTypes.push_back(WaterBlock());
    blockTypes.push_back(GoldBlock());
    blockTypes.push_back(LadderBlock());
    blockTypes.push_back(SandBlock());
    blockTypes.push_back(WoodBlock());
    blockTypes.push_back(LeavesBlock());

    blockTypes.push_back(BlackWoolBlock());
    blockTypes.push_back(GrayWoolBlock());
    blockTypes.push_back(RedWoolBlock());
    blockTypes.push_back(PinkWoolBlock());
    blockTypes.push_back(DarkGreenWoolBlock());
    blockTypes.push_back(GreenWoolBlock());
    blockTypes.push_back(BrownWoolBlock());
    blockTypes.push_back(YellowWoolBlock());
    blockTypes.push_back(BlueWoolBlock());

    blockTypes.push_back(LightBlueWoolBlock());
    blockTypes.push_back(VioletWoolBlock());
    blockTypes.push_back(PastelWoolBlock());
    blockTypes.push_back(OrangeWoolBlock());
    blockTypes.push_back(LightGrayWoolBlock());
    blockTypes.push_back(WhiteWoolBlock());
    blockTypes.push_back(CactusBlock());
    blockTypes.push_back(SandStone());
    blockTypes.push_back(BrickBlock());

    blockTypes.push_back(CageBlock());
    blockTypes.push_back(SnowSoil());
    blockTypes.push_back(DarkWoodBlock());
    blockTypes.push_back(WhiteWoodBlock());
    blockTypes.push_back(IceBlock());
    blockTypes.push_back(CaneBlock());
    blockTypes.push_back(OakPlanks());
    blockTypes.push_back(ShelfBlock());
    blockTypes.push_back(CobbleStone());

    blockTypes.push_back(SpruceLeaves());
    blockTypes.push_back(BirchLeaves());
    blockTypes.push_back(Sponge());
    blockTypes.push_back(GlassBlock());
    blockTypes.push_back(IronOre());
    blockTypes.push_back(CoalOre());
    blockTypes.push_back(DiamondOre());
    blockTypes.push_back(RedstoneOre());
    blockTypes.push_back(LapisOre());

    blockTypes.push_back(Obsidian());
    blockTypes.push_back(SnowBlock());
    blockTypes.push_back(Torch());
    blockTypes.push_back(Door1());
    blockTypes.push_back(Door2());
    blockTypes.push_back(Door3());
    blockTypes.push_back(Door4());
    blockTypes.push_back(Door5());
    blockTypes.push_back(Door6());

    blockTypes.push_back(Door7());
    blockTypes.push_back(Door8());
    blockTypes.push_back(Door9());
    blockTypes.push_back(Door10());
    blockTypes.push_back(WoodenFence());
    blockTypes.push_back(CobbleStair1());
    blockTypes.push_back(CobbleStair2());
    blockTypes.push_back(CobbleStair3());
    blockTypes.push_back(CobbleStair4());

    blockTypes.push_back(Diamond());
    blockTypes.push_back(Gold());
    blockTypes.push_back(Iron());
    blockTypes.push_back(BrickStair1());
    blockTypes.push_back(BrickStair2());
    blockTypes.push_back(BrickStair3());
    blockTypes.push_back(BrickStair4());
    blockTypes.push_back(NoteBlock());
    blockTypes.push_back(JukeBox());

    blockTypes.push_back(Pumpkin1());
    blockTypes.push_back(Pumpkin2());
    blockTypes.push_back(Pumpkin3());
    blockTypes.push_back(Pumpkin4());
    blockTypes.push_back(Mooshroom1());
    blockTypes.push_back(Mooshroom2());
    blockTypes.push_back(Soil());
    blockTypes.push_back(WateredSoil());
    blockTypes.push_back(MelonBlock());

    blockTypes.push_back(OakHalfBlock());
    blockTypes.push_back(CobbleHalfBlock());
    blockTypes.push_back(BrickHalfBlock());
    blockTypes.push_back(Slab());
    blockTypes.push_back(StoneSandHalfBlock());
    blockTypes.push_back(Ladder1());
    blockTypes.push_back(Ladder2());
    blockTypes.push_back(Ladder3());
    blockTypes.push_back(Ladder4());

    blockTypes.push_back(DoubleSlab());
    blockTypes.push_back(MossyCobblestone());
    blockTypes.push_back(Snow2());
    blockTypes.push_back(ChestBlock1());
    blockTypes.push_back(ChestBlock2());
    blockTypes.push_back(ChestBlock3());
    blockTypes.push_back(ChestBlock4());
    blockTypes.push_back(ChestLock());
    blockTypes.push_back(ClayBlock());

    blockTypes.push_back(Chest5());
    blockTypes.push_back(LapisBlock());
    blockTypes.push_back(StoneBrick());
    blockTypes.push_back(CrackedStoneBrick());
    blockTypes.push_back(CarvedStoneBrick());
    blockTypes.push_back(CraftingTable());
    blockTypes.push_back(FurnaceOff());
    blockTypes.push_back(FurnaceOn());
    blockTypes.push_back(HalfStoneBrick());

    blockTypes.push_back(Spawner());
    blockTypes.push_back(TNTBlock());
    blockTypes.push_back(Nuclear());
    blockTypes.push_back(Lava());
    blockTypes.push_back(Gravel());
    blockTypes.push_back(GlassPanel());
    blockTypes.push_back(IronBars());
    blockTypes.push_back(TallGrass());
    blockTypes.push_back(Fern());

    blockTypes.push_back(DrySapling());
    blockTypes.push_back(Flower1());
    blockTypes.push_back(Flower2());
    blockTypes.push_back(OakSapling());
    blockTypes.push_back(SpruceSapling());
    blockTypes.push_back(BirchSapling());
    blockTypes.push_back(FreeBlock5());
    blockTypes.push_back(WoodenStair1());
    blockTypes.push_back(WoodenStair2());

    blockTypes.push_back(WoodenStair3());
    blockTypes.push_back(WoodenStair4());
    blockTypes.push_back(MelonPlant4Reverse());
    blockTypes.push_back(MelonPlant1());
    blockTypes.push_back(MelonPlant2());
    blockTypes.push_back(MelonPlant3());
    blockTypes.push_back(MelonPlant4());

    blockTypes.push_back(WheatBlock1());
    blockTypes.push_back(WheatBlock2());
    blockTypes.push_back(WheatBlock3());
    blockTypes.push_back(WheatBlock4());

    blockTypes.push_back(WheatBlock5());
    blockTypes.push_back(WheatBlock6());
    blockTypes.push_back(WorldSky());
    blockTypes.push_back(Cloud());
    blockTypes.push_back(IronBlock());

    blockTypes.push_back(JackOLantern1());
    blockTypes.push_back(JackOLantern2());
    blockTypes.push_back(JackOLantern3());
    blockTypes.push_back(JackOLantern4());

    blockTypes.push_back(TrapdoorOpen1());
    blockTypes.push_back(TrapdoorOpen2());
    blockTypes.push_back(TrapdoorOpen3());
    blockTypes.push_back(TrapdoorOpen4());
    blockTypes.push_back(TrapdoorClosed1());
    blockTypes.push_back(TrapdoorClosed2());
    blockTypes.push_back(TrapdoorClosed3());
    blockTypes.push_back(TrapdoorClosed4());

    blockTypes.push_back(BirchPlanks());
    blockTypes.push_back(SprucePlanks());
    blockTypes.push_back(BirchHalfBlock());
    blockTypes.push_back(SpruceHalfBlock());
    blockTypes.push_back(BirchFence());
    blockTypes.push_back(SpruceFence());

    blockTypes.push_back(BirchStair1());
    blockTypes.push_back(BirchStair2());
    blockTypes.push_back(BirchStair3());
    blockTypes.push_back(BirchStair4());

    blockTypes.push_back(SpruceStair1());
    blockTypes.push_back(SpruceStair2());
    blockTypes.push_back(SpruceStair3());
    blockTypes.push_back(SpruceStair4());

    blockTypes.push_back(WaterBlock1());
    blockTypes.push_back(WaterBlock2());
    blockTypes.push_back(WaterBlock3());
    blockTypes.push_back(WaterBlock4());

    blockTypes.push_back(ItemFrame1());
    blockTypes.push_back(ItemFrame2());
    blockTypes.push_back(ItemFrame3());
    blockTypes.push_back(ItemFrame4());

    blockTypes.push_back(PumpkinPlant4Reverse());
    blockTypes.push_back(PumpkinPlant1());
    blockTypes.push_back(PumpkinPlant2());
    blockTypes.push_back(PumpkinPlant3());
    blockTypes.push_back(PumpkinPlant4());

    blockTypes.push_back(PotatoPlant1());
    blockTypes.push_back(PotatoPlant2());
    blockTypes.push_back(PotatoPlant3());
    blockTypes.push_back(PotatoPlant4());

    blockTypes.push_back(CarrotPlant1());
    blockTypes.push_back(CarrotPlant2());
    blockTypes.push_back(CarrotPlant3());
    blockTypes.push_back(CarrotPlant4());

    blockTypes.push_back(BedBlock1());
    blockTypes.push_back(BedBlock2());
    blockTypes.push_back(BedBlock3());
    blockTypes.push_back(BedBlock4());

    blockTypes.push_back(BedBlock5());
    blockTypes.push_back(BedBlock6());
    blockTypes.push_back(BedBlock7());
    blockTypes.push_back(BedBlock8());

    blockTypes.push_back(BlackWoolCarpet());
    blockTypes.push_back(GrayWoolCarpet());
    blockTypes.push_back(RedWoolCarpet());
    blockTypes.push_back(PinkWoolCarpet());
    blockTypes.push_back(DarkGreenWoolCarpet());
    blockTypes.push_back(GreenWoolCarpet());
    blockTypes.push_back(BrownWoolCarpet());
    blockTypes.push_back(YellowWoolCarpet());
    blockTypes.push_back(BlueWoolCarpet());
    blockTypes.push_back(LightBlueWoolCarpet());
    blockTypes.push_back(VioletWoolCarpet());
    blockTypes.push_back(PastelWoolCarpet());
    blockTypes.push_back(OrangeWoolCarpet());
    blockTypes.push_back(LightGrayWoolCarpet());
    blockTypes.push_back(WhiteWoolCarpet());

    blockTypes.push_back(HayBale());
    blockTypes.push_back(CoalBlock());
    blockTypes.push_back(RedstoneBlock());
}

void CraftWorld::InitItemVector()
{
    itemTypes.push_back(WoodenPickaxe());
    itemTypes.push_back(StonePickaxe());
    itemTypes.push_back(IronPickaxe());
    itemTypes.push_back(DiamondPickaxe());
    itemTypes.push_back(GoldenPickaxe());

    itemTypes.push_back(WoodenSword());
    itemTypes.push_back(StoneSword());
    itemTypes.push_back(IronSword());
    itemTypes.push_back(DiamondSword());
    itemTypes.push_back(GoldenSword());

    itemTypes.push_back(WoodenShovel());
    itemTypes.push_back(StoneShovel());
    itemTypes.push_back(IronShovel());
    itemTypes.push_back(DiamondShovel());
    itemTypes.push_back(GoldenShovel());

    itemTypes.push_back(WoodenAxe());
    itemTypes.push_back(StoneAxe());
    itemTypes.push_back(IronAxe());
    itemTypes.push_back(DiamondAxe());
    itemTypes.push_back(GoldenAxe());

    itemTypes.push_back(WoodenHoe());
    itemTypes.push_back(StoneHoe());
    itemTypes.push_back(IronHoe());
    itemTypes.push_back(DiamondHoe());
    itemTypes.push_back(GoldenHoe());

    itemTypes.push_back(Scissors());
    itemTypes.push_back(Stick());
    itemTypes.push_back(Coal());
    itemTypes.push_back(IronBar());
    itemTypes.push_back(DiamondItem());

    itemTypes.push_back(GoldenBar());
    itemTypes.push_back(FireItem());
    itemTypes.push_back(LigniteCoal());
    itemTypes.push_back(Clay());
    itemTypes.push_back(Apple());
    itemTypes.push_back(GoldenApple());

    itemTypes.push_back(WheatSeeds());
    itemTypes.push_back(Wheat());
    itemTypes.push_back(WhiteBread());
    itemTypes.push_back(Brick());
    itemTypes.push_back(Busket());
    itemTypes.push_back(WaterBusket());
    itemTypes.push_back(LavaBusket());

    itemTypes.push_back(CaneItem());
    itemTypes.push_back(Flower1Item());
    itemTypes.push_back(Flower2Item());
    itemTypes.push_back(OakSaplingItem());
    itemTypes.push_back(Paper());
    itemTypes.push_back(Book());
    itemTypes.push_back(SnowBallItem());

    itemTypes.push_back(MooshroomItem1());
    itemTypes.push_back(MooshroomItem2());
    itemTypes.push_back(Bowl());
    itemTypes.push_back(MooshroomBowl());

    itemTypes.push_back(IronWateringCan());
    itemTypes.push_back(DiamondWateringCan());
    itemTypes.push_back(DoorItem());
    itemTypes.push_back(LadderItem());
    itemTypes.push_back(TorchItem());
    itemTypes.push_back(SpruceSaplingItem());
    itemTypes.push_back(BirchSaplingItem());

    itemTypes.push_back(MelonItem());
    itemTypes.push_back(MelonSeeds());
    itemTypes.push_back(Sugar());
    itemTypes.push_back(CocoaBeans());
    itemTypes.push_back(Cookie());
    itemTypes.push_back(RawBeef());
    itemTypes.push_back(Steak());
    itemTypes.push_back(LapisLazuli());
    itemTypes.push_back(MilkBusket());
    itemTypes.push_back(CakeItem());
    itemTypes.push_back(Bone());
    itemTypes.push_back(BoneMeal());
    itemTypes.push_back(Leather());

    itemTypes.push_back(SampleHelmet());
    itemTypes.push_back(SampleChestplate());
    itemTypes.push_back(SampleLeggings());
    itemTypes.push_back(SampleBoots());

    itemTypes.push_back(LeatherHelmet());
    itemTypes.push_back(LeatherChestplate());
    itemTypes.push_back(LeatherLeggings());
    itemTypes.push_back(LeatherBoots());

    itemTypes.push_back(ChainmailHelmet());
    itemTypes.push_back(ChainmailChestplate());
    itemTypes.push_back(ChainmailLeggings());
    itemTypes.push_back(ChainmailBoots());

    itemTypes.push_back(IronHelmet());
    itemTypes.push_back(IronChestplate());
    itemTypes.push_back(IronLeggings());
    itemTypes.push_back(IronBoots());

    itemTypes.push_back(DiamondHelmet());
    itemTypes.push_back(DiamondChestplate());
    itemTypes.push_back(DiamondLeggings());
    itemTypes.push_back(DiamondBoots());

    itemTypes.push_back(GoldenHelmet());
    itemTypes.push_back(GoldenChestplate());
    itemTypes.push_back(GoldenLeggings());
    itemTypes.push_back(GoldenBoots());

    itemTypes.push_back(RottenFlesh());
    itemTypes.push_back(GlassPanelItem());
    itemTypes.push_back(IronBarsItem());

    itemTypes.push_back(Flint());
    itemTypes.push_back(Gunpowder());
    itemTypes.push_back(Redstone());
    itemTypes.push_back(Clock());
    itemTypes.push_back(Compass());
    itemTypes.push_back(Saddle());
    itemTypes.push_back(Sign());
    itemTypes.push_back(ItemFrameItem());
    itemTypes.push_back(FlowerPotItem());
    itemTypes.push_back(PaintningItem());
    itemTypes.push_back(BedItem());
    itemTypes.push_back(Disk1());
    itemTypes.push_back(Disk2());
    itemTypes.push_back(Disk3());

    itemTypes.push_back(LightGrayDye());
    itemTypes.push_back(GrayDye());
    itemTypes.push_back(InkSac());
    itemTypes.push_back(PinkDye());
    itemTypes.push_back(RoseRed());
    itemTypes.push_back(OrangeDye());
    itemTypes.push_back(DandelionYellow());
    itemTypes.push_back(LimeDye());
    itemTypes.push_back(CactusGreen());
    itemTypes.push_back(LightBlueDye());
    itemTypes.push_back(PurpleDye());
    itemTypes.push_back(MagnetaDye());

    itemTypes.push_back(PumpkinSeeds());
    itemTypes.push_back(FlintAndSteel());

    itemTypes.push_back(Potato());
    itemTypes.push_back(BakedPotato());
    itemTypes.push_back(PoisonedPotato());
    itemTypes.push_back(Carrot());

    itemTypes.push_back(Disk4());
    itemTypes.push_back(Disk5());
    itemTypes.push_back(Disk6());
    itemTypes.push_back(Disk7());
    itemTypes.push_back(Disk8());
    itemTypes.push_back(Disk9());
    itemTypes.push_back(Disk10());
    itemTypes.push_back(Disk11());
    itemTypes.push_back(Disk12());
}

void CraftWorld::UpdateBlocksAndItemsName()
{
    if(RenderManager::InstancePtr()->GetFontLanguage() == RUSSIAN)
    {
        blockTypes[1].name = "Dern";
        blockTypes[2].name = "Zeml^";
        blockTypes[3].name = "Kamen$";
        blockTypes[4].name = "Voda";
        blockTypes[5].name = "Zolota^ ruda";
        blockTypes[7].name = "Pesok";
        blockTypes[8].name = "Dub";
        blockTypes[9].name = "Dubov@e list$^";
        blockTypes[10].name = "Yerna^ werst$";
        blockTypes[11].name = "Sera^ werst$";
        blockTypes[12].name = "Krasna^ werst$";
        blockTypes[13].name = "Rozova^ werst$";
        blockTypes[14].name = "Zelena^ werst$";
        blockTypes[15].name = "La~mova^ werst$";
        blockTypes[16].name = "Koriyneva^ werst$";
        blockTypes[17].name = "Jelta^ werst$";
        blockTypes[18].name = "Sin^^ werst$";
        blockTypes[19].name = "Goluba^ werst$";
        blockTypes[20].name = "Fioletova^ werst$";
        blockTypes[21].name = "Purpurna^ werst$";
        blockTypes[22].name = "Orangeva^ werst$";
        blockTypes[23].name = "Svetlo-sera^ werst$";
        blockTypes[24].name = "Werst$";
        blockTypes[25].name = "Kaktus";
        blockTypes[26].name = "Pesyanik";
        blockTypes[27].name = "Kirpiyi";
        blockTypes[28].name = "Spauner";
        blockTypes[29].name = "Sneg";
        blockTypes[30].name = "El$";
        blockTypes[31].name = "Bereza";
        blockTypes[32].name = "Led";
        blockTypes[33].name = "Trostnik";
        blockTypes[34].name = "Dubov@e doski";
        blockTypes[35].name = "Knijna^ polka";
        blockTypes[36].name = "Bul@jnik";
        blockTypes[37].name = "Hvo^";
        blockTypes[38].name = "Berezov@e list$^";
        blockTypes[39].name = "Gubka";
        blockTypes[40].name = "Steklo";
        blockTypes[41].name = "Jelezna^ ruda";
        blockTypes[42].name = "Ugol$na^ ruda";
        blockTypes[43].name = "Almazna^ ruda";
        blockTypes[44].name = "Krasna^ ruda";
        blockTypes[45].name = "Lazuritova^ ruda";
        blockTypes[46].name = "Obsidian";
        blockTypes[47].name = "Snejn@~ blok";
        blockTypes[49].name = "Dverno~ blok";
        blockTypes[59].name = "Dubov@~ zabor";
        blockTypes[60].name = "Bul@jnikova^ stupen$ka";
        blockTypes[61].name = "Bul@jnikova^ stupen$ka";
        blockTypes[62].name = "Bul@jnikova^ stupen$ka";
        blockTypes[63].name = "Bul@jnikova^ stupen$ka";
        blockTypes[64].name = "Almazn@~ blok";
        blockTypes[65].name = "Zoloto~ blok";
        blockTypes[66].name = "Jelezn@~ blok";
        blockTypes[67].name = "Kirpiyna^ stupen$ka";
        blockTypes[68].name = "Kirpiyna^ stupen$ka";
        blockTypes[69].name = "Kirpiyna^ stupen$ka";
        blockTypes[70].name = "Kirpiyna^ stupen$ka";
        blockTypes[71].name = "Notn@~ blok";
        blockTypes[72].name = "Muz@kal$n@~ proigr@vatel$";
        blockTypes[73].name = "T@kva";
        blockTypes[74].name = "T@kva";
        blockTypes[75].name = "T@kva";
        blockTypes[76].name = "T@kva";
        blockTypes[81].name = "Arbuz";
        blockTypes[82].name = "Polublok iz dubov@h dosok";
        blockTypes[83].name = "Bul@jnikova^ plita";
        blockTypes[84].name = "Kirpiyna^ plita";
        blockTypes[85].name = "Kamenna^ plita";
        blockTypes[86].name = "Pesyanikova^ plita";
        blockTypes[91].name = "Tort";
        blockTypes[92].name = "Zamwel@~ bul@jnik";
        blockTypes[93].name = "Sneg";
        blockTypes[94].name = "Sunduk";
        blockTypes[95].name = "Sunduk";
        blockTypes[96].name = "Sunduk";
        blockTypes[97].name = "Sunduk";
        blockTypes[99].name = "Glin^n@~ blok";
        blockTypes[101].name = "Lazuritov@~ blok";
        blockTypes[102].name = "Kamenn@~ kirpiy";
        blockTypes[103].name = "Kamenn@~ kirpiy";
        blockTypes[104].name = "Rezno~ kamenn@~ kirpiy";
        blockTypes[105].name = "Verstak";
        blockTypes[106].name = "Pey$";
        blockTypes[107].name = "Pey$";
        blockTypes[108].name = "Plita iz kamennogo kirpiya";
        blockTypes[109].name = "Spauner mobov";
        blockTypes[110].name = "TNT";
        blockTypes[112].name = "Lava";
        blockTypes[113].name = "Gravi~";
        blockTypes[125].name = "Dubova^ stupen@ka";
        blockTypes[126].name = "Dubova^ stupen@ka";
        blockTypes[127].name = "Dubova^ stupen@ka";
        blockTypes[142].name = "Korenna^ poroda";
        blockTypes[143].name = "Svetil$nik Djeka";
        blockTypes[144].name = "Svetil$nik Djeka";
        blockTypes[145].name = "Svetil$nik Djeka";
        blockTypes[146].name = "Svetil$nik Djeka";
        blockTypes[151].name = "L#k";
        blockTypes[155].name = "Berezov@e doski";
        blockTypes[156].name = "Elov@e doski";
        blockTypes[157].name = "Polublok iz berezov@h dosok";
        blockTypes[158].name = "Polublok iz elov@h dosok";
        blockTypes[159].name = "Berezov@~ zabor";
        blockTypes[160].name = "Elov@~ zabor";
        blockTypes[161].name = "Berezova^ stupen$ka";
        blockTypes[165].name = "Elova^ stupen$ka";

        for(int i = 197; i <= 212; i++)
        {
            blockTypes[i].name = "Kover";
        }
        blockTypes[213].name = "Blok Sena";
        blockTypes[214].name = "Ugol$n@~ blok";
        blockTypes[215].name = "Blok Redstouna";

        itemTypes[0].name = "Derev^nna^ kirka";
        itemTypes[1].name = "Kamenna^ kirka";
        itemTypes[2].name = "Jelezna^ kirka";
        itemTypes[3].name = "Almazna^ kirka";
        itemTypes[4].name = "Zolota^ kirka";
        itemTypes[5].name = "Derev^nn@~ mey";
        itemTypes[6].name = "Kamenn@~ mey";
        itemTypes[7].name = "Jelezn@~ mey";
        itemTypes[8].name = "Almazn@~ mey";
        itemTypes[9].name = "Zoloto~ mey";
        itemTypes[10].name = "Derev^nna^ lopata";
        itemTypes[11].name = "Kamenna^ lopata";
        itemTypes[12].name = "Jelezna^ lopata";
        itemTypes[13].name = "Almazna^ lopata";
        itemTypes[14].name = "Zolota^ lopata";
        itemTypes[15].name = "Derev^nn@~ topor";
        itemTypes[16].name = "Kamenn@~ topor";
        itemTypes[17].name = "Jelezn@~ topor";
        itemTypes[18].name = "Almazn@~ topor";
        itemTypes[19].name = "Zoloto~ topor";
        itemTypes[20].name = "Derev^nna^ mot@ga";
        itemTypes[21].name = "Kamenna^ mot@ga";
        itemTypes[22].name = "Jelezna^ mot@ga";
        itemTypes[23].name = "Almazna^ mot@ga";
        itemTypes[24].name = "Zolota^ mot@ga";
        itemTypes[25].name = "Nojnic@";
        itemTypes[26].name = "Palka";
        itemTypes[27].name = "Ugol$";
        itemTypes[28].name = "Jelezn@~ slitok";
        itemTypes[29].name = "Almaz";
        itemTypes[30].name = "Zoloto~ slitok";
        itemTypes[33].name = "Glina";
        itemTypes[34].name = "]bloko";
        itemTypes[35].name = "Zolotoe ^bloko";
        itemTypes[36].name = "Semena pwenic@";
        itemTypes[37].name = "Pwenica";
        itemTypes[38].name = "Hleb";
        itemTypes[39].name = "Kirpiy";
        itemTypes[40].name = "Vedro";
        itemTypes[41].name = "Vedro vod@";
        itemTypes[42].name = "Vedro lav@";
        itemTypes[43].name = "Trostnik";
        itemTypes[44].name = "Roza";
        itemTypes[45].name = "Oduvanyik";
        itemTypes[46].name = "Sajenec duba";
        itemTypes[47].name = "Bumaga";
        itemTypes[48].name = "Kniga";
        itemTypes[49].name = "Snejok";
        itemTypes[50].name = "Grib";
        itemTypes[51].name = "Grib";
        itemTypes[52].name = "Miska";
        itemTypes[53].name = "Gribna^ pohlebka";
        itemTypes[56].name = "Dver$";
        itemTypes[57].name = "Lestnica";
        itemTypes[58].name = "Fakel";
        itemTypes[59].name = "Sajenec eli";
        itemTypes[60].name = "Sajenec berez@";
        itemTypes[61].name = "Lomtik arbuza";
        itemTypes[62].name = "Semeba arbuza";
        itemTypes[63].name = "Sahar";
        itemTypes[64].name = "Kako-bob@";
        itemTypes[65].name = "Peyen$e";
        itemTypes[66].name = "S@ra^ gov^dina";
        itemTypes[67].name = "Ste~k";
        itemTypes[68].name = "Lazurit";
        itemTypes[69].name = "Vedro moloka";
        itemTypes[70].name = "Tort";
        itemTypes[71].name = "Kost$";
        itemTypes[72].name = "Kostna^ muka";
        itemTypes[73].name = "Koja";
        itemTypes[78].name = "Kojana^ wapka";
        itemTypes[79].name = "Kojana^ kurtka";
        itemTypes[80].name = "Kojan@e wtan@";
        itemTypes[81].name = "Kojan@e sapogi";
        itemTypes[82].name = "Kolyujn@~ wlem";
        itemTypes[83].name = "Kolyujn@~ nagrudnik";
        itemTypes[84].name = "Kolyujn@e ponoji";
        itemTypes[85].name = "Kolyujn@e botinki";
        itemTypes[86].name = "Jelezn@~ wlem";
        itemTypes[87].name = "Jelezna^ kirasa";
        itemTypes[88].name = "Jelezn@e ponoji";
        itemTypes[89].name = "Jelezn@e botinki";
        itemTypes[90].name = "Almazn@~ wlem";
        itemTypes[91].name = "Almazna^ kirasa";
        itemTypes[92].name = "Almazn@e ponoji";
        itemTypes[93].name = "Almazn@e botinki";
        itemTypes[94].name = "Zoloto~ wlem";
        itemTypes[95].name = "Zolota^ kirasa";
        itemTypes[96].name = "Zolot@e ponoji";
        itemTypes[97].name = "Zolot@e botinki";
        itemTypes[98].name = "Gnila^ plot$";
        itemTypes[99].name = "Stekl^nna^ panel$";
        itemTypes[100].name = "Jelezna^ rewetka";

        itemTypes[101].name = "Kremen$";
        itemTypes[102].name = "Poroh";
        itemTypes[103].name = "Redstoun";
        itemTypes[104].name = "Yas@";
        itemTypes[105].name = "Kompas";
        itemTypes[106].name = "Sedlo";
        itemTypes[107].name = "Tabliyka";
        itemTypes[108].name = "Ramka";
        itemTypes[109].name = "Cvetoyn@~ gorwok";
        itemTypes[110].name = "Kartina";
        itemTypes[111].name = "Krovat$";
        itemTypes[112].name = "Muz@kal$na^ plastinka";
        itemTypes[113].name = "Muz@kal$na^ plastinka";
        itemTypes[114].name = "Muz@kal$na^ plastinka";
        itemTypes[115].name = "Svetlo-ser@~ krasitel$";
        itemTypes[116].name = "Ser@~ krasitel$";
        itemTypes[117].name = "Yernil$n@~ mewok";
        itemTypes[118].name = "Rozov@~ krasitel$";
        itemTypes[119].name = "Krasn@~ krasitel$";
        itemTypes[120].name = "Oranjev@~ krasitel$";
        itemTypes[121].name = "Jelt@~ krasitel$";
        itemTypes[122].name = "La~mov@~ krasitel$";
        itemTypes[123].name = "Zelen@~ krasitel$";
        itemTypes[124].name = "Svetlo-sini~ krasitel$";
        itemTypes[125].name = "Fioletov@~ krasitel$";
        itemTypes[126].name = "Sirenev@~ krasitel$";
        itemTypes[127].name = "Semena t@kv@";
        itemTypes[128].name = "Ognivo";
        itemTypes[129].name = "Kartofel$";
        itemTypes[130].name = "Peyenn@~ kartofel$";
        itemTypes[131].name = "]dovit@~ kartofel$";
        itemTypes[132].name = "Morkov$";

        for(int i = 133; i <= 141; i++)
        {
            itemTypes[i].name = "Muz@kal$na^ plastinka";
        }
    }
}

CraftWorld::~CraftWorld()
{
    free(m_Blocks);
    free(m_BlockSettings);

    //delete chests
    for(unsigned int i = 0; i < mChunks.size(); i++)
    {
        delete mChunks[i];
    }
    mChunks.clear();

    //water chunks
    for(unsigned int i = 0; i < mTransparentChunks.size(); i++)
    {
        delete mTransparentChunks[i];
    }
    mTransparentChunks.clear();

    for(unsigned int i = 0; i < mChests.size(); i++)
    {
        delete mChests[i];
    }
    mChests.clear();

    //delete furnace
    for(unsigned int i = 0; i < mFurnaces.size(); i++)
    {
        delete mFurnaces[i];
    }
    mFurnaces.clear();

    //delete note blocks
    for(unsigned int i = 0; i < mNoteBlocks.size(); i++)
    {
        delete mNoteBlocks[i];
    }
    mNoteBlocks.clear();

    //delete jukeboxes
    for(unsigned int i = 0; i < mJukeboxes.size(); i++)
    {
        delete mJukeboxes[i];
    }
    mJukeboxes.clear();

    //delete monster spawners
    for(unsigned int i = 0; i < mMonsterSpawners.size(); i++)
    {
        delete mMonsterSpawners[i];
    }
    mMonsterSpawners.clear();

    //delete item frames
    for(unsigned int i = 0; i < mItemFrames.size(); i++)
    {
        delete mItemFrames[i];
    }
    mItemFrames.clear();

    //delete all direction mind blocks
    for(unsigned int i = 0; i < mDirects.size(); i++)
    {
        delete mDirects[i];
    }
    mDirects.clear();

    //delete all zombies
    for(unsigned int i = 0; i < mZombies.size(); i++)
    {
        delete mZombies[i];
    }
    mZombies.clear();

    //delete all cows
    for(unsigned int i = 0; i < mCows.size(); i++)
    {
        delete mCows[i];
    }
    mCows.clear();

    //delete all creepers
    for(unsigned int i = 0; i < mCreepers.size(); i++)
    {
        delete mCreepers[i];
    }
    mCreepers.clear();

    //delete all sheeps
    for(unsigned int i = 0; i < mSheeps.size(); i++)
    {
        delete mSheeps[i];
    }
    mSheeps.clear();

    if(mDrops.empty() == false)
    {
        for(unsigned int i = 0; i < mDrops.size(); i++)
        {
            delete mDrops[i];
        }
        mDrops.clear();
    }

    if(mTNTs.empty() == false)
    {
        for(unsigned int i = 0; i < mTNTs.size(); i++)
        {
            delete mTNTs[i];
        }
        mTNTs.clear();
    }

    delete worldWeather;

    blockTypes.clear();
    itemTypes.clear();
    toUpdate.clear();
    updatingChunks.clear();

    free(sheepBodyVertices);
    free(sheepHeadVertices);
    free(sheepHeadFurVertices);
    free(sheepLegFurVertices);
    //free(sheepLegOPTVertices);
    //free(sheepHeadOPTVertices);

    free(creeperLegVertices);

    free(hatVertices);
    free(bodyVertices);
    free(headVertices);
    free(legVertices);
    free(handVertices);

    free(cowBodyVertices);
    free(cowHeadVertices);
    free(cowLegVertices);
    free(cowHornsVertices);

    free(helmetVertices);
    free(shoulderVertices);
    free(chestplateVertices);
    free(bellyVertices);
    free(leggingVertices);
    free(bootVertices);
}

void CraftWorld::initWorld(int worldSize, int worldHeight, int chunkSize)
{
    //inicjuj mape
    WORLD_SIZE = worldSize;
    CHUNK_SIZE = chunkSize;
    WORLD_HEIGHT = worldHeight;

    m_Blocks = new block_t[WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE];
    memset(m_Blocks, 0, sizeof(block_t) * WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE);

    m_BlockSettings = new block_t[WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE];
    memset(m_BlockSettings, 0, sizeof(block_t) * WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE);
}

void CraftWorld::initWorldBlocksLight() // Тень под блоками
{
    int x, y, z;
    int level = 0;
    for (z = 0; z < WORLD_SIZE; ++z)
    {
        for (x = 0; x < WORLD_SIZE; ++x)
        {
            level = 0;
            for (y = WORLD_HEIGHT - 1; y >= 0; --y)
            {
                SetShadowLevel(x,y,z,level);
                if(level < 15)
                {
                    if(GetBlock(x,y,z) == DirtBlock::getID())
                    {
                        if(level == 0)
                        {
                            GetBlock(x,y,z) = GrassBlock::getID();
                        }
                    }
                    if(GetBlock(x,y,z) != 0 && BlockRefraction(x,y,z) != 0 && BlockRefraction(x,y,z) != 2 || GetBlock(x,y,z) == WaterBlock::getID())
                    {
                        level += 1;
                    }
                    if(GetBlock(x,y,z) == WaterBlock::getID())
                    {
                        if(level != 15)
                        {
                            level += 1;
                        }
                    }
                    if(GetBlock(x,y,z) == GlassBlock::getID())
                    {
                        if(level == 0)
                        {
                            level += 1;
                        }
                    }

                    if(GetBlock(x,y,z) == LeavesBlock::getID() || GetBlock(x,y,z) == BirchLeaves::getID() || GetBlock(x,y,z) == SpruceLeaves::getID())
                    {
                        if(level == 0)
                        {
                            level += 2;
                        }
                    }
                }
            }
        }
    }
}

/*void CraftWorld::initWorldBlocksLight() //  // dont finished, not used
{
    block_t* m_HeightMap = new block_t[WORLD_SIZE * WORLD_SIZE];
    memset(m_HeightMap, 0, sizeof(block_t) * WORLD_SIZE * WORLD_SIZE);

    // first iteration
    int x, y, z;
    int level = 0;

    for (x = 0; x < WORLD_SIZE; ++x)
    {
        for (z = 0; z < WORLD_SIZE; ++z)
        {
            bool heightFound = false;
            level = 0;
            for (y = WORLD_HEIGHT - 1; y >= 0; --y)
            {
                block_t Block = GetBlockNoCheck(x,y,z);
                if(heightFound == false)
                {
                    if(Block != 0 && blockTypes[Block].refraction != 0)
                    {
                        heightFound = true;
                        m_HeightMap[x+z*WORLD_SIZE] = y;

                        level += 2;
                    }
                }
                else
                {
                    SetShadowLevelNoCheck(x,y,z,level);
                }
            }
        }
    }
    // second iteration
    for (x = 1; x < WORLD_SIZE-1; ++x)
    {
        for (z = 1; z < WORLD_SIZE-1; ++z)
        {
            level = 2;
            for (y = GetHeight(x,z); y >= 0; --y)
            {
                if(level == 4)
                {
                    SetShadowLevelNoCheck(x,y,z,level);
                }
                if(level == 2)
                {
                    if(GetHeight(x-1,z) > y &&
                       GetHeight(x+1,z) > y &&
                       GetHeight(x,z-1) > y &&
                       GetHeight(x,z+1) > y)
                    {
                        level = 4;
                    }
                }
            }
        }
    }
    // third iteration
    for (x = 2; x < WORLD_SIZE-2; ++x)
    {
        for (z = 2; z < WORLD_SIZE-2; ++z)
        {
            level = 4;
            for (y = GetHeight(x,z); y >= 0; --y)
            {
                if(level == 6)
                {
                    SetShadowLevelNoCheck(x,y,z,level);
                }
                if(level == 4)
                {
                    if(GetShadowLevelNoCheck(x,y,z) == 4)
                    {
                        if(GetHeight(x-2,z) > y &&
                           GetHeight(x+2,z) > y &&
                           GetHeight(x,z-2) > y &&
                           GetHeight(x,z+2) > y &&
                           GetHeight(x-1,z-1) > y &&
                           GetHeight(x-1,z+1) > y &&
                           GetHeight(x+1,z-1) > y &&
                           GetHeight(x+1,z+1) > y
                           )
                        {
                            level = 6;
                        }
                    }
                }
            }
        }
    }
    // fourth iteration
    for (x = 3; x < WORLD_SIZE-3; ++x)
    {
        for (z = 3; z < WORLD_SIZE-3; ++z)
        {
            level = 6;
            for (y = GetHeight(x,z); y >= 0; --y)
            {
                if(level == 8)
                {
                    SetShadowLevelNoCheck(x,y,z,level);
                }
                if(level == 6)
                {
                    if(GetShadowLevelNoCheck(x,y,z) == 6)
                    {
                        if(GetHeight(x-3,z) > y &&
                           GetHeight(x+3,z) > y &&
                           GetHeight(x,z-3) > y &&
                           GetHeight(x,z+3) > y &&
                           GetHeight(x-2,z-1) > y &&
                           GetHeight(x-1,z-2) > y &&
                           GetHeight(x-2,z+1) > y &&
                           GetHeight(x-1,z+2) > y &&
                           GetHeight(x+2,z-1) > y &&
                           GetHeight(x+1,z-2) > y &&
                           GetHeight(x+2,z+1) > y &&
                           GetHeight(x+1,z+2) > y
                           )
                        {
                            level = 8;
                        }
                    }
                }
            }
        }
    }

    free(m_HeightMap);
}*/


void CraftWorld::UpdateBlocksLightNearby(const int xx, const int yy, const int zz) // dont finished, not used
{
    int low_range_x, high_range_x;
    int low_range_z, high_range_z;

    xx - 4 < 0 ? low_range_x = 0 : low_range_x = xx - 4;
    zz - 4 < 0 ? low_range_z = 0 : low_range_z = zz - 4;

    xx + 4 >= WORLD_SIZE ? high_range_x = WORLD_SIZE - 1 : high_range_x = xx + 4;
    zz + 4 >= WORLD_SIZE ? high_range_z = WORLD_SIZE - 1 : high_range_z = zz + 4;

    for(int iteration = 1; iteration <= 4; iteration++)
    {
        for(int x = low_range_x; x <= high_range_x; x++)
        {
            for(int z = low_range_z; z <= high_range_z; z++)
            {
                if(TaxicabDistance2d(xx,zz,x,z) <= 4)
                {
                    CalculateBlocksLightAtPos(x,yy,z,iteration);
                }
            }
        }
    }
}

void CraftWorld::CalculateBlocksLightAtPos(const int xx, const int yy, const int zz, int iteration) // dont finished, not used
{
  /*  int level = 0;
    if(iteration == 1)
    {
        for (int y = yy; y >= 0; y--)
        {
            SetShadowLevel(xx,y,zz,level);

            block_t Block = GetBlockNoCheck(xx,y,zz);

            if(Block != 0 && blockTypes[Block].refraction != 0)
            {
                level = 2;
            }
        }
    }

    if(iteration == 2)
    {
        level = 2;
        if(GetShadowLevelNoCheck(xx,yy,zz) >= 4)
        {
            return;
        }
        for (int y = yy; y >= 0; y--)
        {
            int shadowLevel = GetShadowLevelNoCheck(xx,y,zz);
            if(shadowLevel > 2)
            {
                return;
            }

            SetShadowLevel(xx,y,zz,level);

            block_t Block = GetBlockNoCheck(xx,y,zz);

            if(GetShadowLevel(xx-1,y,zz) >= 2 &&
               GetShadowLevel(xx+1,y,zz) >= 2 &&
               GetShadowLevel(xx,y,zz-1) >= 2 &&
               GetShadowLevel(xx,y,zz+1) >= 2)
            {
                level = 4;
            }
        }
    }

    if(iteration == 3)
    {
        level = 4;
        if(GetShadowLevelNoCheck(xx,yy,zz) >= 6)
        {
            return;
        }
        for (int y = yy; y >= 0; y--)
        {
            int shadowLevel = GetShadowLevelNoCheck(xx,y,zz);
            if(shadowLevel > 4)
            {
                return;
            }

            SetShadowLevel(xx,y,zz,level);

            block_t Block = GetBlockNoCheck(xx,y,zz);

            if(GetShadowLevel(xx-2,y,zz) >= 4 &&
               GetShadowLevel(xx+2,y,zz) >= 4 &&
               GetShadowLevel(xx,y,zz-2) >= 4 &&
               GetShadowLevel(xx,y,zz+2) >= 4 &&
               GetShadowLevel(xx-1,y,zz-1) >= 4 &&
               GetShadowLevel(xx-1,y,zz+1) >= 4 &&
               GetShadowLevel(xx+1,y,zz-1) >= 4 &&
               GetShadowLevel(xx+1,y,zz+1) >= 4)
            {
                level = 6;
            }
        }
    }

    if(iteration == 4)
    {
        level = 6;
        if(GetShadowLevelNoCheck(xx,yy,zz) >= 8)
        {
            return;
        }
        for (int y = yy; y >= 0; y--)
        {
            int shadowLevel = GetShadowLevelNoCheck(xx,y,zz);
            if(shadowLevel > 6)
            {
                return;
            }

            SetShadowLevel(xx,y,zz,level);

            block_t Block = GetBlockNoCheck(xx,y,zz);

            if(GetShadowLevel(xx-3,y,zz) >= 6 &&
               GetShadowLevel(xx+3,y,zz) >= 6 &&
               GetShadowLevel(xx,y,zz-3) >= 6 &&
               GetShadowLevel(xx,y,zz+3) >= 6 &&
               GetShadowLevel(xx-2,y,zz-1) >= 6 &&
               GetShadowLevel(xx-1,y,zz-2) >= 6 &&
               GetShadowLevel(xx-2,y,zz+1) >= 6 &&
               GetShadowLevel(xx-1,y,zz+2) >= 6 &&
               GetShadowLevel(xx+2,y,zz-1) >= 6 &&
               GetShadowLevel(xx+1,y,zz-2) >= 6 &&
               GetShadowLevel(xx+2,y,zz+1) >= 6 &&
               GetShadowLevel(xx+1,y,zz+2) >= 6
               )
            {
                level = 8;
            }
        }
    } */
}

void CraftWorld::initPutBlocksLight(const int xx, const int zz) // Тень под блоками
{
    int x, y, z;
    int level = 0;

    x = xx;
    z = zz;

    if (x < 0 || z < 0 || x >= WORLD_SIZE || z >= WORLD_SIZE)
    {
        return;
    }

    for (y = WORLD_HEIGHT - 1; y >= 0; --y)
    {
        SetShadowLevel(x,y,z,level);
        if(level < 15)
        {
            int block = GetBlockNoCheck(x,y,z);
            if(block != 0 && BlockRefraction(x,y,z) != 0 && BlockRefraction(x,y,z) != 2 || block == WaterBlock::getID())
            {
                level += 1;
            }

            if(block == GlassBlock::getID())
            {
                if(level == 0)
                {
                    level += 1;
                }
            }

            if(block == LeavesBlock::getID() || block == BirchLeaves::getID() || block == SpruceLeaves::getID())
            {
                if(level == 0)
                {
                    level += 2;
                }
            }

            if(block == WaterBlock::getID())
            {
                if(level != 15)
                {
                    level += 1;
                }
            }
        }
        if(level == 15)
        {
            break;
        }
    }
}

void CraftWorld::initPutBlocksLight(const int xx, const int yy, const int zz) // Тень под блоками
{
    int x, y, z;
    int level = 0;

    x = xx;
    z = zz;
    y = yy;

    if (x < 0 || y < 0 || z < 0  || x >= WORLD_SIZE || y >= WORLD_HEIGHT || z >= WORLD_SIZE)
    {
        return;
    }

    level = GetShadowLevel(x,y,z);

    for (y = yy; y >= 0; --y)
    {
        SetShadowLevel(x,y,z,level);
        if(level < 15)
        {
            if(GetBlockNoCheck(x,y,z) != 0 && BlockRefraction(x,y,z) != 0 && BlockRefraction(x,y,z) != 2 || GetBlockNoCheck(x,y,z) == WaterBlock::getID())
            {
                level += 1;
            }
            if(GetBlockNoCheck(x,y,z) == WaterBlock::getID())
            {
                if(level != 15)
                {
                    level += 1;
                }
            }
        }
        if(level == 15)
        {
            break;
        }
    }
}

void CraftWorld::SaveCompressedWorld(std::string  filename) // Сохранение мира
{
    //first save version and name on normal file
    FILE * pFile;
    pFile = fopen(filename.c_str(),"wb");

    if(pFile == NULL)
        return;

    //version
    fwrite(&worldVersion,sizeof(int),1,pFile);

    //game mode
    fwrite(&gameModeWorld,sizeof(char),1,pFile);

    //game mode
    fwrite(&gameLocked,sizeof(bool),1,pFile);

    //name
    fwrite(worldName,sizeof(char),50,pFile);

    //close file
    fclose(pFile);

    //now save compressed map info
    filename = filename + "c";

    //open file
    gzFile saveFile;
    saveFile = gzopen(filename.c_str(),"wb");
    if(saveFile == 0)
        return;

    //player pos
    gzwrite(saveFile, &playerPos.x,sizeof(float));
    gzwrite(saveFile, &playerPos.y,sizeof(float));
    gzwrite(saveFile, &playerPos.z,sizeof(float));

    //player spawn pos
    gzwrite(saveFile, &playerSpawnPointPosition.x,sizeof(float));
    gzwrite(saveFile, &playerSpawnPointPosition.y,sizeof(float));
    gzwrite(saveFile, &playerSpawnPointPosition.z,sizeof(float));

    //snow biome pos
    gzwrite(saveFile, &snowBiomePosition.x,sizeof(float));
    gzwrite(saveFile, &snowBiomePosition.z,sizeof(float));
    gzwrite(saveFile, &snowBiomeRadius,sizeof(float));

    //desert biome pos
    gzwrite(saveFile, &desertBiomePosition.x,sizeof(float));
    gzwrite(saveFile, &desertBiomePosition.z,sizeof(float));
    gzwrite(saveFile, &desertBiomeRadius,sizeof(float));

    //size
    gzwrite(saveFile, &WORLD_SIZE,sizeof(int));

    //size
    gzwrite(saveFile, &WORLD_HEIGHT,sizeof(int));

    //chunksize
    gzwrite(saveFile, &CHUNK_SIZE,sizeof(int));

    //cubes
    gzwrite(saveFile, m_Blocks,sizeof(unsigned char)*(WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE));

    //settings
    gzwrite(saveFile, m_BlockSettings,sizeof(unsigned char)*(WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE));

    //world time
    gzwrite(saveFile, &worldDayTime,sizeof(float));
    //sun time
    //gzwrite(saveFile, &sunTime,sizeof(float));

    gzwrite(saveFile, &mainStatistics,sizeof(st));

    gzwrite(saveFile, &mainOptions,sizeof(opt));

    gzwrite(saveFile, &invId,sizeof(short)*(36));

    gzwrite(saveFile, &invAm,sizeof(short)*(36));

    gzwrite(saveFile, &invSt,sizeof(bool)*(36));

    gzwrite(saveFile, &armorId,sizeof(short)*(4));

    gzwrite(saveFile, &armorAm,sizeof(short)*(4));

    gzwrite(saveFile, &armorSt,sizeof(bool)*(4));

    gzwrite(saveFile, &HP,sizeof(unsigned int));

    gzwrite(saveFile, &HG,sizeof(unsigned int));

    gzwrite(saveFile, &OS,sizeof(unsigned int));

    gzwrite(saveFile, &brightFactor,sizeof(float));

    gzwrite(saveFile, &starsFactor,sizeof(float));

    gzwrite(saveFile, &worldSeed,sizeof(int));

    gzwrite(saveFile, &skyTime,sizeof(float));

    int savedWeatherType = worldWeather->GetWeatherType();
    gzwrite(saveFile, &savedWeatherType,sizeof(int));

    float savedWeatherDuration = worldWeather->GetWeatherDuration();
    gzwrite(saveFile, &savedWeatherDuration,sizeof(float));

    mChestsize = mChests.size();
    gzwrite(saveFile, &mChestsize,sizeof(int));

    if(mChestsize != 0)
    {
        int* data1 = new int[27*mChestsize];
        int* data2 = new int[27*mChestsize];
        bool* data3 = new bool[27*mChestsize];

        int* data4 = new int[mChestsize];

        for(int k = 0; k < mChestsize; k++)
        {
            Chest* SaveChest = mChests[k];
            for(int l = 0; l <= 26; l++)
            {
                data1[k*27+l] = SaveChest->chestSlotId[l];
                data2[k*27+l] = SaveChest->chestSlotAm[l];
                data3[k*27+l] = SaveChest->chestSlotSt[l];
            }

            data4[k] = SaveChest->chestAddress;
        }

        gzwrite(saveFile, data1,sizeof(int) * (mChestsize*27));
        gzwrite(saveFile, data2,sizeof(int) * (mChestsize*27));
        gzwrite(saveFile, data3,sizeof(bool) * (mChestsize*27));

        gzwrite(saveFile, data4,sizeof(int)*(mChestsize));

        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete[] data4;
    }

    mFurnacesize = mFurnaces.size();
    gzwrite(saveFile, &mFurnacesize,sizeof(int));

    if(mFurnacesize != 0)
    {
        int* data1 = new int[3*mFurnacesize];
        int* data2 = new int[3*mFurnacesize];
        bool* data3 = new bool[3*mFurnacesize];

        int* data4 = new int[mFurnacesize];
        int* data5 = new int[mFurnacesize];
        int* data6 = new int[mFurnacesize];

        float* data7 = new float[mFurnacesize];
        float* data8 = new float[mFurnacesize];
        int* data10 = new int[mFurnacesize];

        char* data11 = new char[mFurnacesize];

        for(int k = 0; k < mFurnacesize; k++)
        {
            Furnace* SaveFurnace = mFurnaces[k];
            for(int l = 0; l <= 2; l++)
            {
                data1[k*3+l] = SaveFurnace->furnaceSlotId[l];
                data2[k*3+l] = SaveFurnace->furnaceSlotAm[l];
                data3[k*3+l] = SaveFurnace->furnaceSlotSt[l];
            }
            data4[k] = SaveFurnace->furnaceX;
            data5[k] = SaveFurnace->furnaceY;
            data6[k] = SaveFurnace->furnaceZ;

            data7[k] = SaveFurnace->fuelTime;
            data8[k] = SaveFurnace->meltingTime;
            data10[k] = SaveFurnace->smeltItemTemp;
            data11[k] = SaveFurnace->GetSide();
        }

        gzwrite(saveFile, data1,sizeof(int) * (mFurnacesize*3));
        gzwrite(saveFile, data2,sizeof(int) * (mFurnacesize*3));
        gzwrite(saveFile, data3,sizeof(bool) *(mFurnacesize*3));

        gzwrite(saveFile, data4,sizeof(int)*(mFurnacesize));
        gzwrite(saveFile, data5,sizeof(int)*(mFurnacesize));
        gzwrite(saveFile, data6,sizeof(int)*(mFurnacesize));

        gzwrite(saveFile, data7,sizeof(float)*(mFurnacesize));
        gzwrite(saveFile, data8,sizeof(float)*(mFurnacesize));
        gzwrite(saveFile, data10,sizeof(int)*(mFurnacesize));
        gzwrite(saveFile, data11,sizeof(char)*(mFurnacesize));

        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete[] data4;
        delete[] data5;
        delete[] data6;
        delete[] data7;
        delete[] data8;
        delete[] data10;
        delete[] data11;
    }

    mZombiesize = mZombies.size();
    gzwrite(saveFile, &mZombiesize,sizeof(int));

    if(mZombiesize != 0)
    {
        float* data1 = new float[mZombiesize]; // x
        float* data2 = new float[mZombiesize]; // y
        float* data3 = new float[mZombiesize]; // z

        float* data4 = new float[mZombiesize]; // mainAngle
        float* data5 = new float[mZombiesize]; // health
        int* data6 = new int[mZombiesize]; // id

        int* data7 = new int[mZombiesize]; // helmet id
        int* data8 = new int[mZombiesize]; // chestplate id
        int* data9 = new int[mZombiesize]; // leggings id
        int* data10 = new int[mZombiesize]; // boots id

        for(int k = 0; k < mZombiesize; k++)
        {
            Zombie* SaveZombie = mZombies[k];

            data1[k] = SaveZombie->position.x;
            data2[k] = SaveZombie->position.y;
            data3[k] = SaveZombie->position.z;

            data4[k] = SaveZombie->mainAngle;
            data5[k] = SaveZombie->HP;
            data6[k] = SaveZombie->number;

            data7[k] = SaveZombie->helmetId;
            data8[k] = SaveZombie->chestplateId;
            data9[k] = SaveZombie->leggingsId;
            data10[k] = SaveZombie->bootsId;
        }

        gzwrite(saveFile, data1,sizeof(float) * (mZombiesize));
        gzwrite(saveFile, data2,sizeof(float) * (mZombiesize));
        gzwrite(saveFile, data3,sizeof(float) *(mZombiesize));

        gzwrite(saveFile, data4,sizeof(float)*(mZombiesize));
        gzwrite(saveFile, data5,sizeof(float)*(mZombiesize));
        gzwrite(saveFile, data6,sizeof(int)*(mZombiesize));

        gzwrite(saveFile, data7,sizeof(int)*(mZombiesize));
        gzwrite(saveFile, data8,sizeof(int)*(mZombiesize));
        gzwrite(saveFile, data9,sizeof(int)*(mZombiesize));
        gzwrite(saveFile, data10,sizeof(int)*(mZombiesize));

        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete[] data4;
        delete[] data5;
        delete[] data6;
        delete[] data7;
        delete[] data8;
        delete[] data9;
        delete[] data10;
    }


    mCowsize = mCows.size();
    gzwrite(saveFile, &mCowsize,sizeof(int));

    if(mCowsize != 0)
    {
        float* data1 = new float[mCowsize]; // x
        float* data2 = new float[mCowsize]; // y
        float* data3 = new float[mCowsize]; // z

        float* data4 = new float[mCowsize]; // mainAngle
        float* data5 = new float[mCowsize]; // health
        int* data6 = new int[mCowsize]; // id

        for(int k = 0; k < mCowsize; k++)
        {
            Cow* SaveCow = mCows[k];

            data1[k] = SaveCow->position.x;
            data2[k] = SaveCow->position.y;
            data3[k] = SaveCow->position.z;

            data4[k] = SaveCow->mainAngle;
            data5[k] = SaveCow->HP;
            data6[k] = SaveCow->number;
        }

        gzwrite(saveFile, data1,sizeof(float) * (mCowsize));
        gzwrite(saveFile, data2,sizeof(float) * (mCowsize));
        gzwrite(saveFile, data3,sizeof(float) *(mCowsize));

        gzwrite(saveFile, data4,sizeof(float)*(mCowsize));
        gzwrite(saveFile, data5,sizeof(float)*(mCowsize));
        gzwrite(saveFile, data6,sizeof(int)*(mCowsize));

        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete[] data4;
        delete[] data5;
        delete[] data6;
    }

    mSheepsize = mSheeps.size();
    gzwrite(saveFile, &mSheepsize,sizeof(int));

    if(mSheepsize != 0)
    {
        float* data1 = new float[mSheepsize]; // x
        float* data2 = new float[mSheepsize]; // y
        float* data3 = new float[mSheepsize]; // z

        float* data4 = new float[mSheepsize]; // mainAngle
        float* data5 = new float[mSheepsize]; // health
        int* data6 = new int[mSheepsize]; // id

        int* data7 = new int[mSheepsize]; // color
        bool* data8 = new bool[mSheepsize]; // sheared

        for(int k = 0; k < mSheepsize; k++)
        {
            Sheep* SaveSheep = mSheeps[k];

            data1[k] = SaveSheep->position.x;
            data2[k] = SaveSheep->position.y;
            data3[k] = SaveSheep->position.z;

            data4[k] = SaveSheep->mainAngle;
            data5[k] = SaveSheep->HP;
            data6[k] = SaveSheep->number;

            data7[k] = SaveSheep->GetFurColor();
            data8[k] = SaveSheep->sheared;
        }

        gzwrite(saveFile, data1,sizeof(float) * (mSheepsize));
        gzwrite(saveFile, data2,sizeof(float) * (mSheepsize));
        gzwrite(saveFile, data3,sizeof(float) *(mSheepsize));

        gzwrite(saveFile, data4,sizeof(float)*(mSheepsize));
        gzwrite(saveFile, data5,sizeof(float)*(mSheepsize));
        gzwrite(saveFile, data6,sizeof(int)*(mSheepsize));

        gzwrite(saveFile, data7,sizeof(int)*(mSheepsize));
        gzwrite(saveFile, data8,sizeof(bool)*(mSheepsize));

        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete[] data4;
        delete[] data5;
        delete[] data6;
        delete[] data7;
        delete[] data8;
    }


    mCreepersize = mCreepers.size();
    gzwrite(saveFile, &mCreepersize,sizeof(int));

    if(mCreepersize != 0)
    {
        float* data1 = new float[mCreepersize]; // x
        float* data2 = new float[mCreepersize]; // y
        float* data3 = new float[mCreepersize]; // z

        float* data4 = new float[mCreepersize]; // mainAngle
        float* data5 = new float[mCreepersize]; // health
        int* data6 = new int[mCreepersize]; // id

        float* data7 = new float[mCreepersize]; // freezedTimerMax

        for(int k = 0; k < mCreepersize; k++)
        {
            Creeper* SaveCreeper = mCreepers[k];

            data1[k] = SaveCreeper->position.x;
            data2[k] = SaveCreeper->position.y;
            data3[k] = SaveCreeper->position.z;

            data4[k] = SaveCreeper->mainAngle;
            data5[k] = SaveCreeper->HP;
            data6[k] = SaveCreeper->number;

            data7[k] = SaveCreeper->freezedTimerMax;
        }

        gzwrite(saveFile, data1,sizeof(float) * (mCreepersize));
        gzwrite(saveFile, data2,sizeof(float) * (mCreepersize));
        gzwrite(saveFile, data3,sizeof(float) *(mCreepersize));

        gzwrite(saveFile, data4,sizeof(float)*(mCreepersize));
        gzwrite(saveFile, data5,sizeof(float)*(mCreepersize));
        gzwrite(saveFile, data6,sizeof(int)*(mCreepersize));

        gzwrite(saveFile, data7,sizeof(float)*(mCreepersize));

        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete[] data4;
        delete[] data5;
        delete[] data6;
        delete[] data7;
    }

    gzwrite(saveFile, &zombieNum,sizeof(int));


    // TNT data saving
    mTNTsize = mTNTs.size();
    gzwrite(saveFile, &mTNTsize,sizeof(int));

    if(mTNTsize != 0)
    {
        float* data1 = new float[mTNTsize]; // x
        float* data2 = new float[mTNTsize]; // y
        float* data3 = new float[mTNTsize]; // z

        float* data4 = new float[mTNTsize]; // timeToExplode

        for(int k = 0; k < mTNTsize; k++)
        {
            TNT* SaveTNT = mTNTs[k];

            data1[k] = SaveTNT->position.x;
            data2[k] = SaveTNT->position.y;
            data3[k] = SaveTNT->position.z;

            data4[k] = SaveTNT->timeToExplode;
        }

        gzwrite(saveFile, data1,sizeof(float) * (mTNTsize));
        gzwrite(saveFile, data2,sizeof(float) * (mTNTsize));
        gzwrite(saveFile, data3,sizeof(float) *(mTNTsize));

        gzwrite(saveFile, data4,sizeof(float)*(mTNTsize));

        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete[] data4;
    }

    mNoteBlocksize = mNoteBlocks.size();
    gzwrite(saveFile, &mNoteBlocksize,sizeof(int));

    if(mNoteBlocksize != 0)
    {
        int* data1 = new int[mNoteBlocksize]; // x
        int* data2 = new int[mNoteBlocksize]; // y
        int* data3 = new int[mNoteBlocksize]; // z
        int* data4 = new int[mNoteBlocksize]; // note

        for(int k = 0; k < mNoteBlocksize; k++)
        {
            NoteBlockEntity* SaveNoteBlock = mNoteBlocks[k];

            data1[k] = SaveNoteBlock->GetX();
            data2[k] = SaveNoteBlock->GetY();
            data3[k] = SaveNoteBlock->GetZ();
            data4[k] = SaveNoteBlock->GetNote();
        }

        gzwrite(saveFile, data1,sizeof(int) * (mNoteBlocksize));
        gzwrite(saveFile, data2,sizeof(int) * (mNoteBlocksize));
        gzwrite(saveFile, data3,sizeof(int) * (mNoteBlocksize));

        gzwrite(saveFile, data4,sizeof(int)*(mNoteBlocksize));

        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete[] data4;
    }

    mJukeboxsize = mJukeboxes.size();
    gzwrite(saveFile, &mJukeboxsize,sizeof(int));

    if(mJukeboxsize != 0)
    {
        int* data1 = new int[mJukeboxsize]; // x
        int* data2 = new int[mJukeboxsize]; // y
        int* data3 = new int[mJukeboxsize]; // z
        int* data4 = new int[mJukeboxsize]; // record

        for(int k = 0; k < mJukeboxsize; k++)
        {
            JukeboxEntity* SaveJukebox = mJukeboxes[k];

            data1[k] = SaveJukebox->GetX();
            data2[k] = SaveJukebox->GetY();
            data3[k] = SaveJukebox->GetZ();
            data4[k] = SaveJukebox->GetRecord();
        }

        gzwrite(saveFile, data1,sizeof(int) * (mJukeboxsize));
        gzwrite(saveFile, data2,sizeof(int) * (mJukeboxsize));
        gzwrite(saveFile, data3,sizeof(int) * (mJukeboxsize));

        gzwrite(saveFile, data4,sizeof(int) * (mJukeboxsize));

        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete[] data4;
    }

    mMonsterSpawnersize = mMonsterSpawners.size();
    gzwrite(saveFile, &mMonsterSpawnersize,sizeof(int));

    if(mMonsterSpawnersize != 0)
    {
        int* data1 = new int[mMonsterSpawnersize]; // x
        int* data2 = new int[mMonsterSpawnersize]; // y
        int* data3 = new int[mMonsterSpawnersize]; // z
        int* data4 = new int[mMonsterSpawnersize]; // mob to spawn

        for(int k = 0; k < mMonsterSpawnersize; k++)
        {
            MonsterSpawnerEntity* SaveMS = mMonsterSpawners[k];

            data1[k] = SaveMS->GetX();
            data2[k] = SaveMS->GetY();
            data3[k] = SaveMS->GetZ();

            if(SaveMS->MobToSpawn() == "zombie")
            {
                data4[k] = 1;
            }
            else
            {
                data4[k] = 0;
            }
        }

        gzwrite(saveFile, data1,sizeof(int) * (mMonsterSpawnersize));
        gzwrite(saveFile, data2,sizeof(int) * (mMonsterSpawnersize));
        gzwrite(saveFile, data3,sizeof(int) * (mMonsterSpawnersize));

        gzwrite(saveFile, data4,sizeof(int)*(mMonsterSpawnersize));

        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete[] data4;
    }


    mItemFramesize = mItemFrames.size();
    gzwrite(saveFile, &mItemFramesize,sizeof(int));

    if(mItemFramesize != 0)
    {
        int* data1 = new int[mItemFramesize]; // x
        int* data2 = new int[mItemFramesize]; // y
        int* data3 = new int[mItemFramesize]; // z
        int* data4 = new int[mItemFramesize]; // facing
        int* data5 = new int[mItemFramesize]; // get item id
        int* data6 = new int[mItemFramesize]; // get item amount

        for(int k = 0; k < mItemFramesize; k++)
        {
            ItemFrameEntity* SaveIF = mItemFrames[k];

            data1[k] = SaveIF->GetX();
            data2[k] = SaveIF->GetY();
            data3[k] = SaveIF->GetZ();
            data4[k] = SaveIF->GetFacing();
            data5[k] = SaveIF->GetItem();
            data6[k] = SaveIF->GetAmount();
        }

        gzwrite(saveFile, data1,sizeof(int) * (mItemFramesize));
        gzwrite(saveFile, data2,sizeof(int) * (mItemFramesize));
        gzwrite(saveFile, data3,sizeof(int) * (mItemFramesize));
        gzwrite(saveFile, data4,sizeof(int) * (mItemFramesize));
        gzwrite(saveFile, data5,sizeof(int) * (mItemFramesize));
        gzwrite(saveFile, data6,sizeof(int) * (mItemFramesize));

        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete[] data4;
        delete[] data5;
        delete[] data6;
    }


    /*mDirectsize = mDirects.size();
    gzwrite(saveFile, &mDirectsize,sizeof(int));

    if(mDirectsize != 0)
    {
        int* data1 = new int[mDirectsize];
        int* data2 = new int[mDirectsize];
        int* data3 = new int[mDirectsize];

        char* data4 = new char[mDirectsize];

        for(int k = 0; k < mDirectsize; k++)
        {
            DirectionBlock* SaveDirectionBlock = mDirects[k];

            data1[k] = SaveDirectionBlock->X;
            data2[k] = SaveDirectionBlock->Y;
            data3[k] = SaveDirectionBlock->Z;
            data4[k] = SaveDirectionBlock->direction;
        }

        gzwrite(saveFile, data1,sizeof(int) * (mDirectsize));
        gzwrite(saveFile, data2,sizeof(int) * (mDirectsize));
        gzwrite(saveFile, data3,sizeof(int) * (mDirectsize));
        gzwrite(saveFile, data4,sizeof(char)* (mDirectsize));

        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete[] data4;
    }*/
    //close file
    gzclose(saveFile);
    saveFile=0;
}

void CraftWorld::LoadWorld(const char *filename)
{
    FILE * pFile;
    pFile = fopen(filename,"rb");

    if(pFile == NULL)
        return;

    //version
    fread(&worldVersion,sizeof(int),1,pFile);

    //name
    fread(worldName,sizeof(char),50,pFile);

    //size
    fread(&WORLD_SIZE,sizeof(int),1,pFile);

    //chunksize
    fread(&CHUNK_SIZE,sizeof(int),1,pFile);

    //cubes
    m_Blocks = new block_t[WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE];
    fread(m_Blocks,sizeof(unsigned char),(WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE),pFile);

    m_BlockSettings = new block_t[WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE];
    memset(m_BlockSettings, 0, sizeof(block_t) * WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE);

    //close file
    fclose(pFile);
}

void CraftWorld::LoadCompressedWorld(std::string  filename) // Загрузка мира
{
    //load normal save info
    FILE * pFile;
    pFile = fopen(filename.c_str(),"rb");

    if(pFile == NULL)
        return;

    //version
    fread(&worldVersion,sizeof(int),1,pFile);

    fread(&gameModeWorld,sizeof(char),1,pFile);

    fread(&gameLocked,sizeof(bool),1,pFile);
    //name
    fread(worldName,sizeof(char),50,pFile);
    //close file
    fclose(pFile);

    //load compressed map data
    filename = filename + "c";

    //open file
    gzFile saveFile;
    saveFile = gzopen(filename.c_str(),"rb");
    if(saveFile == 0)
        return;

    //player pos
    gzread(saveFile, &playerPos.x,sizeof(float));
    gzread(saveFile, &playerPos.y,sizeof(float));
    gzread(saveFile, &playerPos.z,sizeof(float));

    //player spawn pos
    gzread(saveFile, &playerSpawnPointPosition.x,sizeof(float));
    gzread(saveFile, &playerSpawnPointPosition.y,sizeof(float));
    gzread(saveFile, &playerSpawnPointPosition.z,sizeof(float));

    //snow biome data
    gzread(saveFile, &snowBiomePosition.x,sizeof(float));
    gzread(saveFile, &snowBiomePosition.z,sizeof(float));
    gzread(saveFile, &snowBiomeRadius,sizeof(float));

    //desert biome data
    gzread(saveFile, &desertBiomePosition.x,sizeof(float));
    gzread(saveFile, &desertBiomePosition.z,sizeof(float));
    gzread(saveFile, &desertBiomeRadius,sizeof(float));

    //size
    gzread(saveFile, &WORLD_SIZE,sizeof(int));

    //size
    gzread(saveFile, &WORLD_HEIGHT,sizeof(int));

    //chunksize
    gzread(saveFile, &CHUNK_SIZE,sizeof(int));

    //cubes
    m_Blocks = new block_t[WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE];
    gzread(saveFile, m_Blocks,sizeof(unsigned char)*(WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE));

    //settings
    m_BlockSettings = new block_t[WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE];
    if(worldVersion >= 3)
    {
        //settings
        gzread(saveFile, m_BlockSettings,sizeof(unsigned char)*(WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE));

        //world time
        gzread(saveFile, &worldDayTime,sizeof(float));

        //sun time
        //gzread(saveFile, &sunTime,sizeof(float));

        gzread(saveFile, &mainStatistics,sizeof(st));

        gzread(saveFile, &mainOptions,sizeof(opt));

        gzread(saveFile, &invId,sizeof(short)*(36));

        gzread(saveFile, &invAm,sizeof(short)*(36));

        gzread(saveFile, &invSt,sizeof(bool)*(36));

        gzread(saveFile, &armorId,sizeof(short)*(4));

        gzread(saveFile, &armorAm,sizeof(short)*(4));

        gzread(saveFile, &armorSt,sizeof(bool)*(4));

        gzread(saveFile, &HP,sizeof(unsigned int));

        gzread(saveFile, &HG,sizeof(unsigned int));

        gzread(saveFile, &OS,sizeof(unsigned int));

        gzread(saveFile, &brightFactor,sizeof(float));

        gzread(saveFile, &starsFactor,sizeof(float));

        gzread(saveFile, &worldSeed,sizeof(int));

        gzread(saveFile, &skyTime,sizeof(float));

        int loadedWeatherType = 0;
        gzread(saveFile, &loadedWeatherType,sizeof(int));
        worldWeather->SetWeatherType(loadedWeatherType);

        float loadedWeatherDuration = 0.0f;
        gzread(saveFile, &loadedWeatherDuration,sizeof(float));
        worldWeather->SetWeatherDuration(loadedWeatherDuration);

        gzread(saveFile, &mChestsize,sizeof(int));

        mChests.clear();

        if(mChestsize != 0)
        {
            int* data1 = new int[27*mChestsize];
            int* data2 = new int[27*mChestsize];
            bool* data3 = new bool[27*mChestsize];
            int* data4 = new int[mChestsize];


            gzread(saveFile, data1,sizeof(int)*(mChestsize*27));
            gzread(saveFile, data2,sizeof(int)*(mChestsize*27));
            gzread(saveFile, data3,sizeof(bool)*(mChestsize*27));

            gzread(saveFile, data4,sizeof(int)*(mChestsize));

            for(int k = 0; k < mChestsize; k++)
            {
                Chest* SaveChest = new Chest(-1,-1,-1);

                for(int l = 0; l <= 26; l++)
                {
                    SaveChest->chestSlotId[l] = data1[k * 27 + l];
                    SaveChest->chestSlotAm[l] = data2[k * 27 + l];
                    SaveChest->chestSlotSt[l] = data3[k * 27 + l];
                }
                SaveChest->chestAddress = data4[k];
                mChests.push_back(SaveChest);

                //delete SaveChest;
            }
            delete[] data1;
            delete[] data2;
            delete[] data3;
            delete[] data4;
        }


        gzread(saveFile, &mFurnacesize,sizeof(int));
        mFurnaces.clear();

        if(mFurnacesize != 0)
        {
            int* data1 = new int[3*mFurnacesize];
            int* data2 = new int[3*mFurnacesize];
            bool* data3 = new bool[3*mFurnacesize];

            int* data4 = new int[mFurnacesize];
            int* data5 = new int[mFurnacesize];
            int* data6 = new int[mFurnacesize];

            float* data7 = new float[mFurnacesize];
            float* data8 = new float[mFurnacesize];
            int* data10 = new int[mFurnacesize];

            char* data11 = new char[mFurnacesize];

            gzread(saveFile, data1,sizeof(int)*(mFurnacesize*3));
            gzread(saveFile, data2,sizeof(int)*(mFurnacesize*3));
            gzread(saveFile, data3,sizeof(bool)*(mFurnacesize*3));

            gzread(saveFile, data4,sizeof(int)*(mFurnacesize));
            gzread(saveFile, data5,sizeof(int)*(mFurnacesize));
            gzread(saveFile, data6,sizeof(int)*(mFurnacesize));

            gzread(saveFile, data7,sizeof(float)*(mFurnacesize));
            gzread(saveFile, data8,sizeof(float)*(mFurnacesize));
            gzread(saveFile, data10,sizeof(int)*(mFurnacesize));

            gzread(saveFile, data11,sizeof(char)*(mFurnacesize));

            for(int k = 0; k < mFurnacesize; k++)
            {
                Furnace* SaveFurnace = new Furnace(-1,-1,-1);

                for(int l = 0; l <= 2; l++)
                {
                    SaveFurnace->furnaceSlotId[l] = data1[k * 3 + l];
                    SaveFurnace->furnaceSlotAm[l] = data2[k * 3 + l];
                    SaveFurnace->furnaceSlotSt[l] = data3[k * 3 + l];
                }
                SaveFurnace->furnaceX = data4[k];
                SaveFurnace->furnaceY = data5[k];
                SaveFurnace->furnaceZ = data6[k];
                SaveFurnace->fuelTime = data7[k];
                SaveFurnace->meltingTime = data8[k];
                SaveFurnace->smeltItemTemp = data10[k];
                SaveFurnace->SetSide(data11[k]);
                mFurnaces.push_back(SaveFurnace);
            }
            delete[] data1;
            delete[] data2;
            delete[] data3;
            delete[] data4;
            delete[] data5;
            delete[] data6;
            delete[] data7;
            delete[] data8;
            delete[] data10;
            delete[] data11;
        }

        mZombiesize = 0;
        gzread(saveFile, &mZombiesize,sizeof(int));

        mZombies.clear();

        if(mZombiesize != 0)
        {
            float* data1 = new float[mZombiesize];
            float* data2 = new float[mZombiesize];
            float* data3 = new float[mZombiesize];

            float* data4 = new float[mZombiesize];
            float* data5 = new float[mZombiesize];
            int* data6 = new int[mZombiesize];

            int* data7 = new int[mZombiesize];
            int* data8 = new int[mZombiesize];
            int* data9 = new int[mZombiesize];
            int* data10 = new int[mZombiesize];

            gzread(saveFile, data1,sizeof(float)*(mZombiesize));
            gzread(saveFile, data2,sizeof(float)*(mZombiesize));
            gzread(saveFile, data3,sizeof(float)*(mZombiesize));

            gzread(saveFile, data4,sizeof(float)*(mZombiesize));
            gzread(saveFile, data5,sizeof(float)*(mZombiesize));
            gzread(saveFile, data6,sizeof(int)*(mZombiesize));

            gzread(saveFile, data7,sizeof(int)*(mZombiesize));
            gzread(saveFile, data8,sizeof(int)*(mZombiesize));
            gzread(saveFile, data9,sizeof(int)*(mZombiesize));
            gzread(saveFile, data10,sizeof(int)*(mZombiesize));

            for(int g = 0; g < mZombiesize; g++)
            {
                Zombie* SaveZombie = new Zombie(Vector3(data1[g],data2[g],data3[g]), data6[g]);

                SaveZombie->mainAngle = data4[g];
                SaveZombie->HP = data5[g];
                SaveZombie->helmetId = data7[g];
                SaveZombie->chestplateId = data8[g];
                SaveZombie->leggingsId = data9[g];
                SaveZombie->bootsId = data10[g];

                mZombies.push_back(SaveZombie);
            }
            delete[] data1;
            delete[] data2;
            delete[] data3;
            delete[] data4;
            delete[] data5;
            delete[] data6;
            delete[] data7;
            delete[] data8;
            delete[] data9;
            delete[] data10;
        }

        mCowsize = 0;
        gzread(saveFile, &mCowsize,sizeof(int));

        mCows.clear();

        if(mCowsize != 0)
        {
            float* data1 = new float[mCowsize];
            float* data2 = new float[mCowsize];
            float* data3 = new float[mCowsize];

            float* data4 = new float[mCowsize];
            float* data5 = new float[mCowsize];
            int* data6 = new int[mCowsize];


            gzread(saveFile, data1,sizeof(float)*(mCowsize));
            gzread(saveFile, data2,sizeof(float)*(mCowsize));
            gzread(saveFile, data3,sizeof(float)*(mCowsize));

            gzread(saveFile, data4,sizeof(float)*(mCowsize));
            gzread(saveFile, data5,sizeof(float)*(mCowsize));
            gzread(saveFile, data6,sizeof(int)*(mCowsize));


            for(int g = 0; g < mCowsize; g++)
            {
                Cow* SaveCow = new Cow(Vector3(data1[g],data2[g],data3[g]), data6[g]);

                SaveCow->mainAngle = data4[g];
                SaveCow->HP = data5[g];

                mCows.push_back(SaveCow);
            }
            delete[] data1;
            delete[] data2;
            delete[] data3;
            delete[] data4;
            delete[] data5;
            delete[] data6;
        }


        mSheepsize = 0;
        gzread(saveFile, &mSheepsize,sizeof(int));

        mSheeps.clear();

        if(mSheepsize != 0)
        {
            float* data1 = new float[mSheepsize];
            float* data2 = new float[mSheepsize];
            float* data3 = new float[mSheepsize];

            float* data4 = new float[mSheepsize];
            float* data5 = new float[mSheepsize];
            int* data6 = new int[mSheepsize];

            int* data7 = new int[mSheepsize];
            bool* data8 = new bool[mSheepsize];

            gzread(saveFile, data1,sizeof(float)*(mSheepsize));
            gzread(saveFile, data2,sizeof(float)*(mSheepsize));
            gzread(saveFile, data3,sizeof(float)*(mSheepsize));

            gzread(saveFile, data4,sizeof(float)*(mSheepsize));
            gzread(saveFile, data5,sizeof(float)*(mSheepsize));
            gzread(saveFile, data6,sizeof(int)*(mSheepsize));

            gzread(saveFile, data7,sizeof(int)*(mSheepsize));
            gzread(saveFile, data8,sizeof(bool)*(mSheepsize));


            for(int g = 0; g < mSheepsize; g++)
            {
                Sheep* SaveSheep = new Sheep(Vector3(data1[g],data2[g],data3[g]), data6[g]);

                SaveSheep->mainAngle = data4[g];
                SaveSheep->HP = data5[g];
                SaveSheep->SetFurColor(data7[g]);
                SaveSheep->sheared = data8[g];

                mSheeps.push_back(SaveSheep);
            }
            delete[] data1;
            delete[] data2;
            delete[] data3;
            delete[] data4;
            delete[] data5;
            delete[] data6;
            delete[] data7;
            delete[] data8;
        }



        mCreepersize = 0;
        gzread(saveFile, &mCreepersize,sizeof(int));

        mCreepers.clear();

        if(mCreepersize != 0)
        {
            float* data1 = new float[mCreepersize];
            float* data2 = new float[mCreepersize];
            float* data3 = new float[mCreepersize];

            float* data4 = new float[mCreepersize];
            float* data5 = new float[mCreepersize];
            int* data6 = new int[mCreepersize];

            float* data7 = new float[mCreepersize];

            gzread(saveFile, data1,sizeof(float)*(mCreepersize));
            gzread(saveFile, data2,sizeof(float)*(mCreepersize));
            gzread(saveFile, data3,sizeof(float)*(mCreepersize));

            gzread(saveFile, data4,sizeof(float)*(mCreepersize));
            gzread(saveFile, data5,sizeof(float)*(mCreepersize));
            gzread(saveFile, data6,sizeof(int)*(mCreepersize));

            gzread(saveFile, data7,sizeof(float)*(mCreepersize));


            for(int g = 0; g < mCreepersize; g++)
            {
                Creeper* SaveCreeper = new Creeper(Vector3(data1[g],data2[g],data3[g]), data6[g]);

                SaveCreeper->mainAngle = data4[g];
                SaveCreeper->HP = data5[g];
                SaveCreeper->freezedTimerMax = data7[g];

                mCreepers.push_back(SaveCreeper);
            }
            delete[] data1;
            delete[] data2;
            delete[] data3;
            delete[] data4;
            delete[] data5;
            delete[] data6;
            delete[] data7;
        }

        gzread(saveFile, &zombieNum,sizeof(int));

        // loading TNT data
        mTNTsize = 0;
        gzread(saveFile, &mTNTsize,sizeof(int));

        mTNTs.clear();

        if(mTNTsize != 0)
        {
            float* data1 = new float[mTNTsize];
            float* data2 = new float[mTNTsize];
            float* data3 = new float[mTNTsize];

            float* data4 = new float[mTNTsize];


            gzread(saveFile, data1,sizeof(float)*(mTNTsize));
            gzread(saveFile, data2,sizeof(float)*(mTNTsize));
            gzread(saveFile, data3,sizeof(float)*(mTNTsize));

            gzread(saveFile, data4,sizeof(float)*(mTNTsize));

            for(int g = 0; g < mTNTsize; g++)
            {
                TNT* SaveTNT = new TNT(this, Vector3(data1[g],data2[g],data3[g]), Vector3(0,0,0));

                SaveTNT->timeToExplode = data4[g];

                mTNTs.push_back(SaveTNT);
            }
            delete[] data1;
            delete[] data2;
            delete[] data3;
            delete[] data4;
        }

        // loading NoteBlocks data
        mNoteBlocksize = 0;
        gzread(saveFile, &mNoteBlocksize,sizeof(int));

        mNoteBlocks.clear();

        if(mNoteBlocksize != 0)
        {
            int* data1 = new int[mNoteBlocksize];
            int* data2 = new int[mNoteBlocksize];
            int* data3 = new int[mNoteBlocksize];
            int* data4 = new int[mNoteBlocksize];

            gzread(saveFile, data1,sizeof(int)*(mNoteBlocksize));
            gzread(saveFile, data2,sizeof(int)*(mNoteBlocksize));
            gzread(saveFile, data3,sizeof(int)*(mNoteBlocksize));
            gzread(saveFile, data4,sizeof(int)*(mNoteBlocksize));

            for(int k = 0; k < mNoteBlocksize; k++)
            {
                NoteBlockEntity* SaveNoteBlock = new NoteBlockEntity(data1[k],data2[k],data3[k],data4[k]);
                mNoteBlocks.push_back(SaveNoteBlock);
            }

            delete[] data1;
            delete[] data2;
            delete[] data3;
            delete[] data4;
        }

        // loading Jukebox data
        mJukeboxsize = 0;
        gzread(saveFile, &mJukeboxsize,sizeof(int));

        mJukeboxes.clear();

        if(mJukeboxsize != 0)
        {
            int* data1 = new int[mJukeboxsize];
            int* data2 = new int[mJukeboxsize];
            int* data3 = new int[mJukeboxsize];
            int* data4 = new int[mJukeboxsize];

            gzread(saveFile, data1,sizeof(int)*(mJukeboxsize));
            gzread(saveFile, data2,sizeof(int)*(mJukeboxsize));
            gzread(saveFile, data3,sizeof(int)*(mJukeboxsize));
            gzread(saveFile, data4,sizeof(int)*(mJukeboxsize));

            for(int k = 0; k < mJukeboxsize; k++)
            {
                JukeboxEntity* SaveJukebox = new JukeboxEntity(data1[k],data2[k],data3[k],data4[k]);
                mJukeboxes.push_back(SaveJukebox);
            }

            delete[] data1;
            delete[] data2;
            delete[] data3;
            delete[] data4;
        }


        // loading monstaer Spawners data
        mMonsterSpawnersize = 0;
        gzread(saveFile, &mMonsterSpawnersize,sizeof(int));

        mMonsterSpawners.clear();

        if(mMonsterSpawnersize != 0)
        {
            int* data1 = new int[mMonsterSpawnersize];
            int* data2 = new int[mMonsterSpawnersize];
            int* data3 = new int[mMonsterSpawnersize];
            int* data4 = new int[mMonsterSpawnersize];

            gzread(saveFile, data1,sizeof(int)*(mMonsterSpawnersize));
            gzread(saveFile, data2,sizeof(int)*(mMonsterSpawnersize));
            gzread(saveFile, data3,sizeof(int)*(mMonsterSpawnersize));
            gzread(saveFile, data4,sizeof(int)*(mMonsterSpawnersize));

            for(int k = 0; k < mMonsterSpawnersize; k++)
            {
                MonsterSpawnerEntity* SaveMS;

                if(data4[k] == 1)
                {
                    SaveMS = new MonsterSpawnerEntity(data1[k],data2[k],data3[k],"zombie");
                }
                else
                {
                    SaveMS = new MonsterSpawnerEntity(data1[k],data2[k],data3[k]," ");
                }

                mMonsterSpawners.push_back(SaveMS);
            }

            delete[] data1;
            delete[] data2;
            delete[] data3;
            delete[] data4;
        }


        // loading item frames data
        mItemFramesize = 0;
        gzread(saveFile, &mItemFramesize,sizeof(int));

        mItemFrames.clear();

        if(mItemFramesize != 0)
        {
            int* data1 = new int[mItemFramesize];
            int* data2 = new int[mItemFramesize];
            int* data3 = new int[mItemFramesize];
            int* data4 = new int[mItemFramesize];
            int* data5 = new int[mItemFramesize];
            int* data6 = new int[mItemFramesize];

            gzread(saveFile, data1,sizeof(int)*(mItemFramesize));
            gzread(saveFile, data2,sizeof(int)*(mItemFramesize));
            gzread(saveFile, data3,sizeof(int)*(mItemFramesize));
            gzread(saveFile, data4,sizeof(int)*(mItemFramesize));
            gzread(saveFile, data5,sizeof(int)*(mItemFramesize));
            gzread(saveFile, data6,sizeof(int)*(mItemFramesize));

            for(int k = 0; k < mItemFramesize; k++)
            {
                ItemFrameEntity* SaveIF;
                SaveIF = new ItemFrameEntity(data1[k],data2[k],data3[k],data4[k]);
                SaveIF->SetItem(data5[k]);
                SaveIF->SetAmount(data6[k]);

                mItemFrames.push_back(SaveIF);
            }

            delete[] data1;
            delete[] data2;
            delete[] data3;
            delete[] data4;
            delete[] data5;
            delete[] data6;
        }

        /*gzread(saveFile, &mDirectsize,sizeof(int));
        mDirects.clear();

        if(mDirectsize != 0)
        {
        int* data1 = new int[mDirectsize];
        int* data2 = new int[mDirectsize];
        int* data3 = new int[mDirectsize];
        char* data4 = new char[mDirectsize];

        gzread(saveFile, data1,sizeof(int)*(mDirectsize));
        gzread(saveFile, data2,sizeof(int)*(mDirectsize));
        gzread(saveFile, data3,sizeof(int)*(mDirectsize));
        gzread(saveFile, data4,sizeof(char)*(mDirectsize));

        for(int k = 0; k < mDirectsize; k++)
        {
            DirectionBlock* SaveDirectionBlock = new DirectionBlock(-1,-1,-1,-1);

            SaveDirectionBlock->X = data1[k];
            SaveDirectionBlock->Y = data2[k];
            SaveDirectionBlock->Z = data3[k];
            SaveDirectionBlock->direction = data4[k];
            mDirects.push_back(SaveDirectionBlock);

            //delete SaveChest;
        }
        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete[] data4;
        }*/


    }
    else
    {
        memset(m_BlockSettings, 0, sizeof(block_t) * WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE);
    }

    //close file
    gzclose(saveFile);
    saveFile=0;
}

void CraftWorld::buildMap()
{
    createWorldChunkss();
}

void CraftWorld::destroyMap()
{
    for(unsigned int i = 0; i < mChunks.size(); i++)
    {
        mChunks[i]->reset();
    }

    for(unsigned int i = 0; i < mTransparentChunks.size(); i++)
    {
        mTransparentChunks[i]->reset();
    }
}

int CraftWorld::FindChestID(Vector3 pos)
{
    Vector3 intPos = Vector3((int)pos.x,(int)pos.y,(int)pos.z);

    int id;
    int address;
    id = -1;
    address = intPos.x + intPos.y * WORLD_SIZE + intPos.z * WORLD_SIZE * WORLD_HEIGHT;

    for(unsigned int i = 0; i < mChests.size(); i++)
    {
        Chest* NewChest = mChests[i];
        if(NewChest->chestAddress == address)
        {
            id = i;
            break;
        }
    }

    return id;
}

int CraftWorld::FindFurnaceID(Vector3 pos)
{
    Vector3 intPos = Vector3((int)pos.x,(int)pos.y,(int)pos.z);

    int id;
    id = -1;
    for(unsigned int i = 0; i < mFurnaces.size(); i++)
    {
        Furnace* NewFurnace = mFurnaces[i];
        if(NewFurnace->furnaceX == intPos.x && NewFurnace->furnaceY == intPos.y && NewFurnace->furnaceZ == intPos.z)
        {
            id = i;
            break;
        }
    }

    return id;
}

int CraftWorld::FindNoteBlockID(Vector3 pos)
{
    Vector3 intPos = Vector3((int)pos.x,(int)pos.y,(int)pos.z);

    int id;
    id = -1;
    for(unsigned int i = 0; i < mNoteBlocks.size(); i++)
    {
        NoteBlockEntity* TestNoteBlock = mNoteBlocks[i];
        if(TestNoteBlock->GetPosition() == intPos)
        {
            id = i;
            break;
        }
    }

    return id;
}

int CraftWorld::FindJukeboxID(Vector3 pos)
{
    Vector3 intPos = Vector3((int)pos.x,(int)pos.y,(int)pos.z);

    int id;
    id = -1;
    for(unsigned int i = 0; i < mJukeboxes.size(); i++)
    {
        JukeboxEntity* TestJukebox = mJukeboxes[i];
        if(TestJukebox->GetPosition() == intPos)
        {
            id = i;
            break;
        }
    }

    return id;
}

int CraftWorld::FindMonsterSpawnerID(Vector3 pos)
{
    Vector3 intPos = Vector3((int)pos.x,(int)pos.y,(int)pos.z);

    int id;
    id = -1;
    for(unsigned int i = 0; i < mMonsterSpawners.size(); i++)
    {
        MonsterSpawnerEntity* TestMS = mMonsterSpawners[i];
        if(TestMS->GetPosition() == intPos)
        {
            id = i;
            break;
        }
    }

    return id;
}

int CraftWorld::FindItemFrameID(Vector3 pos)
{
    Vector3 intPos = Vector3((int)pos.x,(int)pos.y,(int)pos.z);

    int id;
    id = -1;
    for(unsigned int i = 0; i < mItemFrames.size(); i++)
    {
        ItemFrameEntity* TestIF = mItemFrames[i];
        if(TestIF->GetPosition() == intPos)
        {
            id = i;
            break;
        }
    }

    return id;
}

void CraftWorld::setTextureSize(int texture,int chunk,int animTexture, int animChunk, int itemTexture, int itemChunk)
{
    texutreSize = texture;
    textureChunk = chunk;

    percent = (float)textureChunk / (float)texutreSize;
    percent2 = (float)animChunk / (float)animTexture;
    percent3 = (float)itemChunk / (float)itemTexture;
}

void CraftWorld::InitLightValues()
{
    lightColor = Vector3(1, 1, 1);
    fogColor = lightColor * 0.8f;
}

void CraftWorld::GetTexturesIds()
{
    textureTerrainId = TextureHelper::Instance()->GetTexture(TextureHelper::Terrain1);
    textureTerrainMipsId = TextureHelper::Instance()->GetTexture(TextureHelper::Terrain2);
    textureItemsId = TextureHelper::Instance()->GetTexture(TextureHelper::Items1);
    textureWaterAnimationId = TextureHelper::Instance()->GetTexture(TextureHelper::WaterAnimation);
    textureLavaAnimationId = TextureHelper::Instance()->GetTexture(TextureHelper::LavaAnimation);
    textureLeatherArmor1Id = TextureHelper::Instance()->GetTexture(TextureHelper::leatherArmor1);
    textureLeatherArmor2Id = TextureHelper::Instance()->GetTexture(TextureHelper::leatherArmor2);
    textureChainArmor1Id = TextureHelper::Instance()->GetTexture(TextureHelper::chainmailArmor1);
    textureChainArmor2Id = TextureHelper::Instance()->GetTexture(TextureHelper::chainmailArmor2);
    textureIronArmor1Id = TextureHelper::Instance()->GetTexture(TextureHelper::ironArmor1);
    textureIronArmor2Id = TextureHelper::Instance()->GetTexture(TextureHelper::ironArmor2);
    textureDiamondArmor1Id = TextureHelper::Instance()->GetTexture(TextureHelper::diamondArmor1);
    textureDiamondArmor2Id = TextureHelper::Instance()->GetTexture(TextureHelper::diamondArmor2);
    textureGoldenArmor1Id = TextureHelper::Instance()->GetTexture(TextureHelper::goldenArmor1);
    textureGoldenArmor2Id = TextureHelper::Instance()->GetTexture(TextureHelper::goldenArmor2);
    textureVegetationColorMapId = TextureHelper::Instance()->GetTexture(TextureHelper::vegetationColorMap);
    textureClockId = TextureHelper::Instance()->GetTexture(TextureHelper::clockTexture);
    textureCompassId = TextureHelper::Instance()->GetTexture(TextureHelper::compassTexture);
    textureSkyColorId = TextureHelper::Instance()->GetTexture(TextureHelper::skyColorTexture);

    GetSkyColor();
}

void CraftWorld::GetSkyColor()
{
    unsigned int r,g,b,r2,g2,b2;

    //clear sky colors
    r2 = TextureManager::Instance()->getRed(0,2,textureSkyColorId) << 24; // there is some shit
    g2 = TextureManager::Instance()->getGreen(0,2,textureSkyColorId);
    b2 = TextureManager::Instance()->getBlue(0,2,textureSkyColorId) << 24;

    r = r2 >> 24;
    g = g2;
    b = b2 >> 24;

    dawnSunsetClearSkyColor = Vector3(r,g,b);

    r2 = TextureManager::Instance()->getRed(0,0,textureSkyColorId) << 24; // there is some shit
    g2 = TextureManager::Instance()->getGreen(0,0,textureSkyColorId);
    b2 = TextureManager::Instance()->getBlue(0,0,textureSkyColorId) << 24;

    r = r2 >> 24;
    g = g2;
    b = b2 >> 24;

    startClearSkyColor = Vector3(r,g,b);

    r2 = TextureManager::Instance()->getRed(0,1,textureSkyColorId) << 24; // there is some shit
    g2 = TextureManager::Instance()->getGreen(0,1,textureSkyColorId);
    b2 = TextureManager::Instance()->getBlue(0,1,textureSkyColorId) << 24;

    r = r2 >> 24;
    g = g2;
    b = b2 >> 24;

    endClearSkyColor = Vector3(r,g,b);

    // rainy sky colors
    r2 = TextureManager::Instance()->getRed(0,5,textureSkyColorId) << 24; // there is some shit
    g2 = TextureManager::Instance()->getGreen(0,5,textureSkyColorId);
    b2 = TextureManager::Instance()->getBlue(0,5,textureSkyColorId) << 24;

    r = r2 >> 24;
    g = g2;
    b = b2 >> 24;

    dawnSunsetRainySkyColor = Vector3(r,g,b);

    r2 = TextureManager::Instance()->getRed(0,3,textureSkyColorId) << 24; // there is some shit
    g2 = TextureManager::Instance()->getGreen(0,3,textureSkyColorId);
    b2 = TextureManager::Instance()->getBlue(0,3,textureSkyColorId) << 24;

    r = r2 >> 24;
    g = g2;
    b = b2 >> 24;

    startRainySkyColor = Vector3(r,g,b);

    r2 = TextureManager::Instance()->getRed(0,4,textureSkyColorId) << 24; // there is some shit
    g2 = TextureManager::Instance()->getGreen(0,4,textureSkyColorId);
    b2 = TextureManager::Instance()->getBlue(0,4,textureSkyColorId) << 24;

    r = r2 >> 24;
    g = g2;
    b = b2 >> 24;

    endRainySkyColor = Vector3(r,g,b);

    startClearSkyColor *= 1.0f/255.0f;
	endClearSkyColor *= 1.0f/255.0f;
	dawnSunsetClearSkyColor *= 1.0f/255.0f;

    startRainySkyColor *= 1.0f/255.0f;
	endRainySkyColor *= 1.0f/255.0f;
	dawnSunsetRainySkyColor *= 1.0f/255.0f;

	startSkyColor = startClearSkyColor;
	endSkyColor = endClearSkyColor;
	dawnSunsetSkyColor = dawnSunsetClearSkyColor;
}

void CraftWorld::SetWolrdTime(float time)
{
    if(time >= 22)
    {
        skyTime = 280+22.5*(time-22);
    }
    if(time < 6)
    {
        skyTime = 325+22.5*time;
    }
    if(time < 22 && time >= 6)
    {
        skyTime = 100+11.25f*(time-6);
    }
    worldDayTime = time;//std::fmod(time, 24);

    lightAngle = worldDayTime / 24.0f * 360.0f - 90.0f;
    if (lightAngle < 0)lightAngle += 360.0f;

    if (worldDayTime <= 4 || worldDayTime >= 23)
    {
        westLight = 0.0f;
        lightColor = Vector3(0.15f, 0.15f, 0.15f);
    }
    else if (worldDayTime == 5)
    {
        lightColor = Vector3(0.15, 0.15, 0.15);
        eastLight = 0.05f;
    }
    else if (worldDayTime == 6)
    {
        lightColor = Vector3(0.2, 0.2, 0.2);
        eastLight = 0.15f;
    }
    else if (worldDayTime == 7)
    {
        lightColor = Vector3(0.35, 0.35, 0.35);
        eastLight = 0.25f;
    }
    else if (worldDayTime == 8)
    {
        lightColor = Vector3(0.55, 0.55, 0.55);
        eastLight = 0.15f;
    }
    else if (worldDayTime == 9)
    {
        lightColor = Vector3(0.8, 0.8, 0.8);
        eastLight = 0.05f;
    }
    else if (worldDayTime == 10)
    {
        lightColor = Vector3(1.0, 1.0, 1.0);
        eastLight = 0.0f;
    }
    else if (worldDayTime >= 11 && worldDayTime <= 18)
    {
        lightColor = Vector3(1.0, 1.0, 1.0);
    }
    else if (worldDayTime == 19)
    {
        lightColor = Vector3(0.9, 0.9, 0.9);
    }
    else if (worldDayTime == 20)
    {
        lightColor = Vector3(0.75, 0.75, 0.75);
        westLight = 0.15f;
    }
    else if (worldDayTime == 21)
    {
        lightColor = Vector3(0.4, 0.4, 0.4);
        westLight = 0.20f;
    }
    else if (worldDayTime == 22)
    {
        lightColor = Vector3(0.2, 0.2, 0.2);
        westLight = 0.20f;
    }
    else if (worldDayTime == 23)
    {
        lightColor = Vector3(0.15, 0.15, 0.15);
        westLight = 0.0f;
    }
    else	//Shouldn't get here
    {
        lightColor = Vector3(1, 1, 1);
    }

    //ambientColor = lightColor / 3.0f;
    ambientColor = Vector3(0,0,0);
    //light face factors
}

void CraftWorld::DropThis(int id, Vector3 position)
{
    if(id != -1)
    {
        dropNum ++;
        Drop *NewDrop = new Drop(Vector3((int)position.x+0.5f,(int)position.y+0.51f,(int)position.z+0.5f), id, 1, true, dropNum);
        NewDrop->myCrft = this;
        mDrops.push_back(NewDrop);
    }
}

void CraftWorld::DropThis(int id, int am, Vector3 position)
{
    if(id != -1 && am > 0)
    {
        dropNum ++;
        Drop *NewDrop = new Drop(Vector3((int)position.x+0.5f,(int)position.y+0.51f,(int)position.z+0.5f), id, am, true, dropNum);
        NewDrop->myCrft = this;
        mDrops.push_back(NewDrop);
    }
}

void CraftWorld::DropThis(int id, int am, bool st, Vector3 position)
{
    if(id != -1 && am > 0)
    {
        dropNum ++;
        Drop *NewDrop = new Drop(Vector3((int)position.x+0.5f,(int)position.y+0.51f,(int)position.z+0.5f), id, am, st, dropNum);
        NewDrop->myCrft = this;
        mDrops.push_back(NewDrop);
    }
}

void CraftWorld::DropThisNoAlign(int id, int am, bool st, Vector3 position, Vector3 velocity)
{
    if(id != -1 && am > 0)
    {
        dropNum ++;
        Drop *NewDrop = new Drop(Vector3(position.x,position.y,position.z), id, am, st, dropNum);
        NewDrop->myCrft = this;
        NewDrop->SetVelociy(velocity);
        mDrops.push_back(NewDrop);
    }
}


void CraftWorld::DestroyDrop(int id)
{
    if(id < mDrops.size() && id >= 0)
    {
        delete mDrops[id];
        mDrops.erase(mDrops.begin()+id);
    }
}


void CraftWorld::SpawnZombie(float xx, float yy, float zz)
{
    zombieNum += 1;

    Zombie *NewZombie = new Zombie(Vector3(xx,yy,zz), zombieNum);
    NewZombie->world = this;
   // NewZombie->Update(this,Vector3(0,0,0),0.0f);
    mZombies.push_back(NewZombie);
}

void CraftWorld::DestroyZombie(int id)
{
    if(id < mZombies.size() && id >= 0)
    {
        delete mZombies[id];
        mZombies.erase(mZombies.begin()+id);
    }
}

void CraftWorld::DestroyAllZombies()
{
    for(int ii = 0; ii < mZombies.size(); ii++)
    {
        delete mZombies[0];
        mZombies.erase(mZombies.begin());
    }
    mZombies.clear();
}

void CraftWorld::SpawnCreeper(float xx, float yy, float zz)
{
    zombieNum += 1;

    Creeper *NewCreeper = new Creeper(Vector3(xx,yy,zz), zombieNum);
    NewCreeper->world = this;
   // NewZombie->Update(this,Vector3(0,0,0),0.0f);
    mCreepers.push_back(NewCreeper);
}

void CraftWorld::SpawnCreeper(float xx, float yy, float zz, float freezedTimerMax)
{
    zombieNum += 1;

    Creeper *NewCreeper = new Creeper(Vector3(xx,yy,zz), zombieNum);
    NewCreeper->world = this;
    NewCreeper->freezedTimerMax = freezedTimerMax;
   // NewZombie->Update(this,Vector3(0,0,0),0.0f);
    mCreepers.push_back(NewCreeper);
}


void CraftWorld::DestroyCreeper(int id)
{
    if(id < mCreepers.size() && id >= 0)
    {
        delete mCreepers[id];
        mCreepers.erase(mCreepers.begin()+id);
    }
}

void CraftWorld::DestroyAllCreepers()
{
    for(int ii = 0; ii < mCreepers.size(); ii++)
    {
        delete mCreepers[0];
        mCreepers.erase(mCreepers.begin());
    }
    mCreepers.clear();
}

void CraftWorld::SpawnCow(float xx, float yy, float zz)
{
    zombieNum += 1;

    Cow *NewCow = new Cow(Vector3(xx,yy,zz), zombieNum);
   // NewZombie->Update(this,Vector3(0,0,0),0.0f);
    mCows.push_back(NewCow);
}

void CraftWorld::DestroyCow(int id)
{
    if(id < mCows.size() && id >= 0)
    {
        delete mCows[id];
        mCows.erase(mCows.begin()+id);
    }
}

void CraftWorld::SpawnSheep(float xx, float yy, float zz)
{
    zombieNum += 1;

    Sheep *NewSheep = new Sheep(Vector3(xx,yy,zz), zombieNum);
   // NewZombie->Update(this,Vector3(0,0,0),0.0f);
    mSheeps.push_back(NewSheep);
}

void CraftWorld::DestroySheep(int id)
{
    if(id < mSheeps.size() && id >= 0)
    {
        delete mSheeps[id];
        mSheeps.erase(mSheeps.begin()+id);
    }
}

void CraftWorld::DestroyItemFrameEntity(Vector3 position)
{
    int id = FindItemFrameID(position);
    if(id != -1)
    {
        ItemFrameEntity* DestroyIF = mItemFrames[id];

        if(DestroyIF->GetAmount() != -1)
        {
            DropThis(DestroyIF->GetItem(),DestroyIF->GetAmount(),StackableItem(DestroyIF->GetItem()),Vector3((int)position.x+0.5f,(int)position.y+0.5f,(int)position.z+0.5f));
        }
        else
        {
            if(DestroyIF->GetItem() >= 250)
            {
                DropThis(DestroyIF->GetItem(),1,StackableItem(DestroyIF->GetItem()),Vector3((int)position.x+0.5f,(int)position.y+0.5f,(int)position.z+0.5f));
            }
            else
            {
                DropThis(DestroyIF->GetItem(),1,true,Vector3((int)position.x+0.5f,(int)position.y+0.5f,(int)position.z+0.5f));
            }
        }
        DropThis(ItemFrameItem::getID(),1,true,Vector3((int)position.x+0.5f,(int)position.y+0.5f,(int)position.z+0.5f));

        delete mItemFrames[id];
        mItemFrames.erase(mItemFrames.begin()+id);
    }
}

void CraftWorld::SpawnTNTentity(float xx, float yy, float zz)
{
    zombieNum += 1;
    TNT *NewTNT = new TNT(this, Vector3(xx,yy,zz), Vector3(0,0,0));
    NewTNT->num = zombieNum;
    NewTNT->ignitedByPlayer = true;
    mTNTs.push_back(NewTNT);
}

void CraftWorld::SpawnTNTentity(float xx, float yy, float zz, float time)
{
    zombieNum += 1;
    TNT *NewTNT = new TNT(this, Vector3(xx,yy,zz), Vector3(0,0,0));
    NewTNT->num = zombieNum;
    NewTNT->timeToExplode = time;
    mTNTs.push_back(NewTNT);
}

void CraftWorld::DestroyTNTentity(int id)
{
    if(id < mTNTs.size() && id >= 0)
    {
        mTNTs.erase(mTNTs.begin()+id);
    }
}

void CraftWorld::CollisionWithOtherZombies(Zombie *Zombie1)
{

}

block_t CraftWorld::_GetBlock (const int x, const int y, const int z) // id блока в данной координате
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        return m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT];
    }
    return 1;
}

block_t CraftWorld::_GetBlockNoCheck(const int x, const int y, const int z) // id блока в данной координате
{
    return m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT];
}


block_t& CraftWorld::GetBlock (const int x, const int y, const int z) // id блока в данной координате
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        return m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT];
    }
    return m_Blocks[1];
}

block_t& CraftWorld::GetBlockNoCheck(const int x, const int y, const int z) // id блока в данной координате
{
    return m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT];
}

block_t& CraftWorld::GetBlockSettings(const int x, const int y, const int z)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        return m_BlockSettings[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT];
    }
    return m_BlockSettings[0];
}

block_t& CraftWorld::GetBlockSettingsNoCheck(const int x, const int y, const int z)
{
    return m_BlockSettings[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT];
}

int CraftWorld::GetShadowLevel(const int x, const int y, const int z)
{
    int value = 0;
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        if((GetBlockSettingsNoCheck(x,y,z) & OpFirstShadowBit) != 0)
        {
            value += 8;
        }
        if((GetBlockSettingsNoCheck(x,y,z) & OpSecondShadowBit) != 0)
        {
            value += 4;
        }
        if((GetBlockSettingsNoCheck(x,y,z) & OpThirdShadowBit) != 0)
        {
            value += 2;
        }
        if((GetBlockSettingsNoCheck(x,y,z) & OpFourthShadowBit) != 0)
        {
            value += 1;
        }
        return value;
    }
    return 0;
}

int CraftWorld::GetShadowLevelNoCheck(const int x, const int y, const int z)
{
    int value = 0;

    if((GetBlockSettingsNoCheck(x,y,z) & OpFirstShadowBit) != 0)
    {
        value += 8;
    }
    if((GetBlockSettingsNoCheck(x,y,z) & OpSecondShadowBit) != 0)
    {
        value += 4;
    }
    if((GetBlockSettingsNoCheck(x,y,z) & OpThirdShadowBit) != 0)
    {
        value += 2;
    }
    if((GetBlockSettingsNoCheck(x,y,z) & OpFourthShadowBit) != 0)
    {
        value += 1;
    }
    return value;
}

void CraftWorld::SetShadowLevel (const int x, const int y, const int z, int level)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        if((GetBlockSettingsNoCheck(x,y,z) & OpFirstShadowBit) != 0)
        {
            GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
        }
        if((GetBlockSettingsNoCheck(x,y,z) & OpSecondShadowBit) != 0)
        {
            GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
        }
        if((GetBlockSettingsNoCheck(x,y,z) & OpThirdShadowBit) != 0)
        {
            GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
        }
        if((GetBlockSettingsNoCheck(x,y,z) & OpFourthShadowBit) != 0)
        {
            GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
        }

        switch(level)
        {
            case 1:
            GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
            break;
            case 2:
            GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
            break;
            case 3:
            GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
            break;
            case 4:
            GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
            break;
            case 5:
            GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
            break;
            case 6:
            GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
            break;
            case 7:
            GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
            break;

            case 8:
            GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
            break;
            case 9:
            GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
            break;
            case 10:
            GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
            break;
            case 11:
            GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
            break;
            case 12:
            GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
            break;
            case 13:
            GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
            break;
            case 14:
            GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
            break;
            case 15:
            GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
            GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
            break;
        }
    }
}


void CraftWorld::SetShadowLevelNoCheck (const int x, const int y, const int z, int level)
{
    if((GetBlockSettingsNoCheck(x,y,z) & OpFirstShadowBit) != 0)
    {
        GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
    }
    if((GetBlockSettingsNoCheck(x,y,z) & OpSecondShadowBit) != 0)
    {
        GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
    }
    if((GetBlockSettingsNoCheck(x,y,z) & OpThirdShadowBit) != 0)
    {
        GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
    }
    if((GetBlockSettingsNoCheck(x,y,z) & OpFourthShadowBit) != 0)
    {
        GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
    }

    switch(level)
    {
        case 1:
        GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
        break;
        case 2:
        GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
        break;
        case 3:
        GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
        break;
        case 4:
        GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
        break;
        case 5:
        GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
        break;
        case 6:
        GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
        break;
        case 7:
        GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
        break;

        case 8:
        GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
        break;
        case 9:
        GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
        break;
        case 10:
        GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
        break;
        case 11:
        GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
        break;
        case 12:
        GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
        break;
        case 13:
        GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
        break;
        case 14:
        GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
        break;
        case 15:
        GetBlockSettingsNoCheck(x,y,z) ^= OpFirstShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpSecondShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpThirdShadowBit;
        GetBlockSettingsNoCheck(x,y,z) ^= OpFourthShadowBit;
        break;
    }
}


void CraftWorld::SetLightLevel(const int x, const int y, const int z, unsigned int level)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        block_t& blockSettings = GetBlockSettingsNoCheck(x,y,z);
        int current = blockSettings & 15;
        //clear this value
        blockSettings ^= current & 15;
        //set new value
        blockSettings ^= level & 15;
    }
}

void CraftWorld::SetLightLevelNoCheck(const int x, const int y, const int z, unsigned int level)
{
    block_t& blockSettings = GetBlockSettingsNoCheck(x,y,z);
    int current = blockSettings & 15;
    //clear this value
    blockSettings ^= current & 15;
    //set new value
    blockSettings ^= level & 15;
}

int CraftWorld::GetLightLevel(const int x, const int y, const int z)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        block_t blockSettings = GetBlockSettingsNoCheck(x,y,z);
        int current = blockSettings & 15;
        return current;
    }
    return 0;
}

int CraftWorld::GetLightLevelNoCheck(const int x, const int y, const int z)
{
    block_t blockSettings = GetBlockSettingsNoCheck(x,y,z);
    int current = blockSettings & 15;
    return current;
}

bool CraftWorld::CheckForTorchSupport(const int x, const int y, const int z,int blockID)
{
    bool haveTorch = false;
    std::vector<int> toRebuild;

    for(int zz = z-1; zz <= z+1; zz++)
    {
        for(int xx = x-1; xx <= x+1; xx++)
        {
            for(int yy = y-1; yy <= y+1; yy++)
            {
                if(GetBlock(xx,yy,zz) == Torch::getID())
                {
                    if(CanPutBlockHere(xx,yy,zz,TorchItem::getID()) == false)
                    {
                        GetBlock(xx,yy,zz) = 0;
                        RemoveLigtSourceAtPosition(xx,yy,zz,Torch::getID());
                        DropThis(TorchItem::getID(), Vector3(xx,yy,zz));

                        haveTorch = true;

                        int id = getChunkId(Vector3(xx,yy,zz));
                        if(toRebuild.empty() == true)
                        {
                            toRebuild.push_back(id);
                        }
                        else
                        {
                            bool alreadyHave = false;
                            for(int i = 0; i < toRebuild.size(); i++)
                            {
                                if(toRebuild[i] == id)
                                {
                                    alreadyHave = true;
                                }
                            }
                            if(alreadyHave == false)
                            {
                                toRebuild.push_back(id);
                            }
                        }

                    }
                }
            }
        }
    }
    if(haveTorch)
    {
        UpdateLightAreaIn(Vector3(x,y,z));
        for(int i = 0; i < toRebuild.size(); i++)
        {
            RebuildTransparentMeshChunk(toRebuild[i]);
        }
    }
    return haveTorch;
}

bool CraftWorld::CheckForLadderSupport(const int x, const int y, const int z)
{
    bool haveLadder = false;
    std::vector<int> toRebuild;

    for(int zz = z-1; zz <= z+1; zz++)
    {
        for(int xx = x-1; xx <= x+1; xx++)
        {
            if(GetBlock(xx,y,zz) == Ladder1::getID() && GetBlock(xx-1,y,zz) == 0)
            {
                GetBlock(xx,y,zz) = 0;
                DropThis(LadderItem::getID(), Vector3(xx,y,zz));

                haveLadder = true;
            }
            if(GetBlock(xx,y,zz) == Ladder2::getID() && GetBlock(xx+1,y,zz) == 0)
            {
                GetBlock(xx,y,zz) = 0;
                DropThis(LadderItem::getID(), Vector3(xx,y,zz));

                haveLadder = true;
            }
            if(GetBlock(xx,y,zz) == Ladder3::getID() && GetBlock(xx,y,zz-1) == 0)
            {
                GetBlock(xx,y,zz) = 0;
                DropThis(LadderItem::getID(), Vector3(xx,y,zz));

                haveLadder = true;
            }
            if(GetBlock(xx,y,zz) == Ladder4::getID() && GetBlock(xx,y,zz+1) == 0)
            {
                GetBlock(xx,y,zz) = 0;
                DropThis(LadderItem::getID(), Vector3(xx,y,zz));

                haveLadder = true;
            }

            if(haveLadder)
            {
                int id = getChunkId(Vector3(xx,y,zz));
                if(toRebuild.empty() == true)
                {
                    toRebuild.push_back(id);
                }
                else
                {
                    bool alreadyHave = false;
                    for(int i = 0; i < toRebuild.size(); i++)
                    {
                        if(toRebuild[i] == id)
                        {
                            alreadyHave = true;
                        }
                    }
                    if(alreadyHave == false)
                    {
                        toRebuild.push_back(id);
                    }
                }
            }
        }
    }
    if(haveLadder)
    {
        for(int i = 0; i < toRebuild.size(); i++)
        {
            RebuildTransparentMeshChunk(toRebuild[i]);
        }
    }
    return haveLadder;
}


bool CraftWorld::DestroyAroundTrapdoors(const int x, const int y, const int z)
{
    std::vector<int> toRebuild;

    block_t& z_plus_block = GetBlock(x,y,z+1);
    block_t& z_minus_block = GetBlock(x,y,z-1);
    block_t& x_plus_block = GetBlock(x+1,y,z);
    block_t& x_minus_block = GetBlock(x-1,y,z);

    if(z_plus_block == 153 || z_plus_block == 149)
    {
        z_plus_block = 0;
        DropThis(TrapdoorClosed1::getID(), Vector3(x,y,z+1));

        if(!ChunksEqual(Vector3(x,y,z+1),Vector3(x,y,z)))
        {
            int id = getChunkId(Vector3(x,y,z+1));
            if(toRebuild.empty() == true)
            {

                toRebuild.push_back(id);
            }
            else
            {
                bool alreadyHave = false;
                for(int i = 0; i < toRebuild.size(); i++)
                {
                    if(toRebuild[i] == id)
                    {
                        alreadyHave = true;
                    }
                }
                if(alreadyHave == false)
                {
                    toRebuild.push_back(id);
                }
            }
        }
    }

    if(z_minus_block == 154 || z_minus_block == 150)
    {
        z_minus_block = 0;
        DropThis(TrapdoorClosed1::getID(), Vector3(x,y,z-1));

        if(!ChunksEqual(Vector3(x,y,z-1),Vector3(x,y,z)))
        {
            int id = getChunkId(Vector3(x,y,z-1));
            if(toRebuild.empty() == true)
            {

                toRebuild.push_back(id);
            }
            else
            {
                bool alreadyHave = false;
                for(int i = 0; i < toRebuild.size(); i++)
                {
                    if(toRebuild[i] == id)
                    {
                        alreadyHave = true;
                    }
                }
                if(alreadyHave == false)
                {
                    toRebuild.push_back(id);
                }
            }
        }
    }

    if(x_plus_block == 151 || x_plus_block == 147)
    {
        x_plus_block = 0;
        DropThis(TrapdoorClosed1::getID(), Vector3(x+1,y,z));

        if(!ChunksEqual(Vector3(x+1,y,z),Vector3(x,y,z)))
        {
            int id = getChunkId(Vector3(x+1,y,z));
            if(toRebuild.empty() == true)
            {

                toRebuild.push_back(id);
            }
            else
            {
                bool alreadyHave = false;
                for(int i = 0; i < toRebuild.size(); i++)
                {
                    if(toRebuild[i] == id)
                    {
                        alreadyHave = true;
                    }
                }
                if(alreadyHave == false)
                {
                    toRebuild.push_back(id);
                }
            }
        }
    }

    if(x_minus_block == 152 || x_minus_block == 148)
    {
        x_minus_block = 0;
        DropThis(TrapdoorClosed1::getID(), Vector3(x-1,y,z));

        if(!ChunksEqual(Vector3(x-1,y,z),Vector3(x,y,z)))
        {
            int id = getChunkId(Vector3(x-1,y,z));
            if(toRebuild.empty() == true)
            {

                toRebuild.push_back(id);
            }
            else
            {
                bool alreadyHave = false;
                for(int i = 0; i < toRebuild.size(); i++)
                {
                    if(toRebuild[i] == id)
                    {
                        alreadyHave = true;
                    }
                }
                if(alreadyHave == false)
                {
                    toRebuild.push_back(id);
                }
            }
        }
    }

    if(toRebuild.empty() == false)
    {
        for(int i = 0; i < toRebuild.size(); i++)
        {
            RebuildTransparentMeshChunk(toRebuild[i]);
        }
    }
    //RebuildTransparentMeshChunk(getChunkId(Vector3(x,y,z)));
}

bool CraftWorld::DestroyAroundItemFrames(const int x, const int y, const int z)
{
    std::vector<int> toRebuild;

    block_t& z_plus_block = GetBlock(x,y,z+1);
    block_t& z_minus_block = GetBlock(x,y,z-1);
    block_t& x_plus_block = GetBlock(x+1,y,z);
    block_t& x_minus_block = GetBlock(x-1,y,z);

    if(z_plus_block == ItemFrame3::getID())
    {
        z_plus_block = 0;
        DestroyItemFrameEntity(Vector3(x,y,z+1));

        if(!ChunksEqual(Vector3(x,y,z+1),Vector3(x,y,z)))
        {
            int id = getChunkId(Vector3(x,y,z+1));
            if(toRebuild.empty() == true)
            {

                toRebuild.push_back(id);
            }
            else
            {
                bool alreadyHave = false;
                for(int i = 0; i < toRebuild.size(); i++)
                {
                    if(toRebuild[i] == id)
                    {
                        alreadyHave = true;
                    }
                }
                if(alreadyHave == false)
                {
                    toRebuild.push_back(id);
                }
            }
        }
    }

    if(z_minus_block == ItemFrame4::getID())
    {
        z_minus_block = 0;
        DestroyItemFrameEntity(Vector3(x,y,z-1));

        if(!ChunksEqual(Vector3(x,y,z-1),Vector3(x,y,z)))
        {
            int id = getChunkId(Vector3(x,y,z-1));
            if(toRebuild.empty() == true)
            {

                toRebuild.push_back(id);
            }
            else
            {
                bool alreadyHave = false;
                for(int i = 0; i < toRebuild.size(); i++)
                {
                    if(toRebuild[i] == id)
                    {
                        alreadyHave = true;
                    }
                }
                if(alreadyHave == false)
                {
                    toRebuild.push_back(id);
                }
            }
        }
    }

    if(x_plus_block == ItemFrame1::getID())
    {
        x_plus_block = 0;
        DestroyItemFrameEntity(Vector3(x+1,y,z));

        if(!ChunksEqual(Vector3(x+1,y,z),Vector3(x,y,z)))
        {
            int id = getChunkId(Vector3(x+1,y,z));
            if(toRebuild.empty() == true)
            {

                toRebuild.push_back(id);
            }
            else
            {
                bool alreadyHave = false;
                for(int i = 0; i < toRebuild.size(); i++)
                {
                    if(toRebuild[i] == id)
                    {
                        alreadyHave = true;
                    }
                }
                if(alreadyHave == false)
                {
                    toRebuild.push_back(id);
                }
            }
        }
    }

    if(x_minus_block == ItemFrame2::getID())
    {
        x_minus_block = 0;
        DestroyItemFrameEntity(Vector3(x-1,y,z));

        if(!ChunksEqual(Vector3(x-1,y,z),Vector3(x,y,z)))
        {
            int id = getChunkId(Vector3(x-1,y,z));
            if(toRebuild.empty() == true)
            {

                toRebuild.push_back(id);
            }
            else
            {
                bool alreadyHave = false;
                for(int i = 0; i < toRebuild.size(); i++)
                {
                    if(toRebuild[i] == id)
                    {
                        alreadyHave = true;
                    }
                }
                if(alreadyHave == false)
                {
                    toRebuild.push_back(id);
                }
            }
        }
    }

    if(toRebuild.empty() == false)
    {
        for(int i = 0; i < toRebuild.size(); i++)
        {
            RebuildTransparentMeshChunk(toRebuild[i]);
        }
    }
    //RebuildTransparentMeshChunk(getChunkId(Vector3(x,y,z)));
}


void CraftWorld::SetLigtSourcePosition(const int x, const int y, const int z,int blockID) // Установить источник света
{
    LightTravel(x,y,z,10,11);
}

void CraftWorld::RemoveLigtSourceAtPosition(const int x, const int y, const int z,int blockID) // Выпилить источник света
{
    for(int zz = z-9; zz <= z+9; zz++)
    {
        int area_size = 9-abs(zz-z);
        for(int xx = x-area_size; xx <= x+area_size; xx++)
        {
            for(int yy = y-area_size; yy <= y+area_size; yy++)
            {
                if (xx >= 0 && yy >= 0 && zz >= 0 && xx < WORLD_SIZE && yy < WORLD_HEIGHT && zz < WORLD_SIZE)
                {
                    int current = GetLightLevel(xx,yy,zz);
                    int shouldBe = 11-(TaxicabDistance2d(xx,zz,x,z)+abs(yy-y));

                    if(current <= shouldBe)
                    {
                        SetLightLevel(xx,yy,zz,0);
                    }
                }
            }
        }
    }
}

void CraftWorld::LightTravel(int x,int y,int z,int steps,int lightLevel) // Алгоритм распространения света
{
    if(steps > 0 && lightLevel > 0)
    {
        if (x >= 0 && y >= 0 && z >= 0  && x < WORLD_SIZE && y < WORLD_HEIGHT && z < WORLD_SIZE)
        {
            if(lightLevel < 11)
            {
                if(LightSourceBlock(GetBlockNoCheck(x,y,z)))
                {
                    return;
                }
            }

            bool canGofuther = false;
            int currentLightLevel = GetLightLevelNoCheck(x,y,z);

            if(currentLightLevel > 0)//lightened
            {
                //set new only if it's brighter
                if(currentLightLevel < lightLevel)
                {
                    SetLightLevelNoCheck(x,y,z,lightLevel);
                    canGofuther = true;
                }
            }
            else
            {
                SetLightLevelNoCheck(x,y,z,lightLevel);
                canGofuther = true;
            }

            //go futher
            if(canGofuther == true)
            {
                if(BlockAllowLight(x+1,y,z) == true)
                {
                    LightTravel(x+1,y,z,steps-1,lightLevel-1);
                }
                if(BlockAllowLight(x-1,y,z) == true)
                {
                    LightTravel(x-1,y,z,steps-1,lightLevel-1);
                }
                if(BlockAllowLight(x,y+1,z) == true)
                {
                    LightTravel(x,y+1,z,steps-1,lightLevel-1);
                }
                if(BlockAllowLight(x,y-1,z) == true)
                {
                    LightTravel(x,y-1,z,steps-1,lightLevel-1);
                }
                if(BlockAllowLight(x,y,z+1) == true)
                {
                    LightTravel(x,y,z+1,steps-1,lightLevel-1);
                }
                if(BlockAllowLight(x,y,z-1) == true)
                {
                    LightTravel(x,y,z-1,steps-1,lightLevel-1);
                }
            }
        }
    }
}

bool CraftWorld::CanPutBlockHere(const int x, const int y, const int z,int blockID)
{
    if (BlockSolid(x-1,y,z) == true || BlockSolid(x+1,y,z) == true || BlockSolid(x,y,z-1) == true || BlockSolid(x,y,z+1) == true || BlockSolid(x,y-1,z) == true || (BlockSolid(x,y+1,z) == true && blockID != TorchItem::getID()))
    {
        return true;
    }
    return false;
}

bool CraftWorld::BlockTransparentOrSpecial(const int x, const int y, const int z)
{
    if (x < 0 || y < 0 || z < 0  || x >= WORLD_SIZE || y >= WORLD_HEIGHT || z >= WORLD_SIZE) return true;

    if(blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].transparent || blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].blockModel > 0)
        return true;

    return false;
}

bool CraftWorld::BlockTransparentOrLightSource(const int x, const int y, const int z)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        if(blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].transparent || blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].lightSource || blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].refraction == 2)
            return true;

        return false;
    }
}

bool CraftWorld::BlockTransparentOrLightSourceNoCheck(const int x, const int y, const int z)
{
    if(blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].transparent || blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].lightSource || blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].refraction == 2)
    {
        return true;
    }

    return false;
}

bool CraftWorld::BlockTransparent(const int x, const int y, const int z)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        return blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].transparent;
    }
}

bool CraftWorld::BlockTransparentNoCheck(const int x, const int y, const int z)
{
    return blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].transparent;
}

bool CraftWorld::BlockAnimated(const int x, const int y, const int z)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        return blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].animated;
    }
}

char CraftWorld::BlockSpecial(const int x, const int y, const int z)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        return blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].blockModel;
    }
}

char CraftWorld::BlockSpecialNoCheck(const int x, const int y, const int z)
{
    return blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].blockModel;
}

bool CraftWorld::BlockUpdate2(const int x, const int y, const int z)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        return blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].update;
    }
}

bool CraftWorld::BlockSolid(const int x, const int y, const int z)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        return blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].solid;
    }
}

short CraftWorld::BlockLoot(const int x, const int y, const int z)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        return blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].loot;
    }
}

short CraftWorld::BlockMaterial(const int x, const int y, const int z)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        return blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].material;
    }
}

bool CraftWorld::BlockEditable(const int x, const int y, const int z)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        return blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].editable;
    }
    return 0;
}


bool CraftWorld::BlockAllowLight(const int x, const int y, const int z)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        return blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].allowLight;
    }
    return 0;
}

char CraftWorld::BlockRefraction(const int x, const int y, const int z)
{
    if (x >= 0 && x < WORLD_SIZE && z >= 0 && z < WORLD_SIZE && y >= 0 && y < WORLD_HEIGHT)
    {
        return blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_HEIGHT]].refraction;
    }
    return 0;
}

float CraftWorld::BlockFinalLight(int x, int y, int z)
{
    float Light = 0.0f;

    int shadowLevel = GetShadowLevel(x,y,z);
    int lightLevel = GetLightLevel(x,y,z);

    bool inShadow = false;

    if(shadowLevel > 0)
    {
        inShadow = true;
    }

    if(lightLevel > 0)//block is lightened
    {
        float lightened = (lightLevel+1) / 15.0f;
        float normal = 0.0f;

        inShadow == true ? normal = lightColor.x*(1.0f-((float)shadowLevel/15.0f)) : normal = lightColor.x;

        if(lightened > normal)
        {
            Light = lightened;
        }
        else
        {
            Light = normal;
        }
    }
    else
    {
        inShadow == true ? Light = lightColor.x*(1.0-(float)(shadowLevel/15.0f)) : Light = lightColor.x;
    }

    if(Light < 0.1)
    {
        return 0.1;
    }

    return Light;

    return 0.1;
}

float CraftWorld::BlockFinalLightNoCheck(int x, int y, int z)
{
    float Light = 0.0f;

    int shadowLevel = GetShadowLevelNoCheck(x,y,z);
    int lightLevel = GetLightLevelNoCheck(x,y,z);

    bool inShadow = false;

    if(shadowLevel > 0)
    {
        inShadow = true;
    }

    if(lightLevel > 0)//block is lightened
    {
        float lightened = (lightLevel+1) / 15.0f;
        float normal = 0.0f;

        inShadow == true ? normal = lightColor.x*(1.0f-((float)shadowLevel/15.0f)) : normal = lightColor.x;

        if(lightened > normal)
        {
            Light = lightened;
        }
        else
        {
            Light = normal;
        }
    }
    else
    {
        inShadow == true ? Light = lightColor.x*(1.0-(float)(shadowLevel/15.0f)) : Light = lightColor.x;
    }

    if(Light < 0.1)
    {
        return 0.1;
    }

    return Light;

    return 0.1;
}

string CraftWorld::NameBlock(int id)
{
    if(id < 250)
    {
        if(id >= 0 && id < blockTypes.size())
        {
            return blockTypes[id].name;
        }
    }
    else
    {
        id -= 250;
        if(id >= 0 && id < itemTypes.size())
        {
            return itemTypes[id].name;
        }
    }
    return "null";
}

bool CraftWorld::LightSourceBlock(int id)
{
    if(id < blockTypes.size() && id >= 0)
    {
        return blockTypes[id].lightSource;
    }
    else
    {
        return false;
    }
    return false;
}


int CraftWorld::LootBlock(int id)
{
    if(id < blockTypes.size() && id >= 0)
    {
        return blockTypes[id].loot;
    }
    else
    {
        return false;
    }
    return 0;
}

int CraftWorld::DurabilityPointsItem(int id)
{
    id -= 250; // it is item
    if(id < itemTypes.size() && id >= 0)
    {
        return itemTypes[id].durabilityPoints;
    }
    return -1;
}

bool CraftWorld::StackableItem(int id)
{
    id -= 250; // it is item
    if(id < itemTypes.size() && id >= 0)
    {
        return itemTypes[id].stackable;
    }
    return false;
}


bool CraftWorld::ItemHaveTerrainTexture(int id)
{
    id -= 250; // it is item
    if(id < itemTypes.size() && id >= 0)
    {
        return itemTypes[id].terrainTexture;
    }
    return false;
}

char CraftWorld::ItemType(int id)
{
    id -= 250; // it is item
    if(id < itemTypes.size() && id >= 0)
    {
        return itemTypes[id].itemType;
    }
    return '0';
}

void CraftWorld::GetSpecialBlockVerts(int i,BaseBlock* blockType) // Строим меш инвентарных блоков, имеющих особую модель
{
    iVertexLeftRight = 0;
    pointsLeftRight = 0;
    iVertexBackFront = 0;
    pointsBackFront = 0;
    iVertexBottomTop = 0;
    pointsBottomTop = 0;

    if(blockType->blockModel == 5) //not common model
    {
        if(i == 6 || i == 134 || i == 135 || i == 136 || (i >= 87 && i <= 90))
        {
            BuildGUIBlockPlane(blockType,1,Vector3(0,0,16),Vector3(0,16,16),Vector3(0,16,0),Vector3(0,0,0));
            BuildGUIBlockPlane(blockType,2,Vector3(16,0,0),Vector3(16,16,0),Vector3(16,16,16),Vector3(16,0,16));
            BuildGUIBlockPlane(blockType,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
            BuildGUIBlockPlane(blockType,4,Vector3(0,16,16),Vector3(16,16,16),Vector3(16,16,0),Vector3(0,16,0));
            BuildGUIBlockPlane(blockType,5,Vector3(0,16,0),Vector3(16,16,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildGUIBlockPlane(blockType,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,16,16),Vector3(0,16,16));
        }

        if(i >= BlackWoolCarpet::getID() && i <= WhiteWoolCarpet::getID())
        {
            BuildGUIBlockPlane(blockType,1,Vector3(0,0,16),Vector3(0,1,16),Vector3(0,1,0),Vector3(0,0,0));
            BuildGUIBlockPlane(blockType,2,Vector3(16,0,0),Vector3(16,1,0),Vector3(16,1,16),Vector3(16,0,16));
            BuildGUIBlockPlane(blockType,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
            BuildGUIBlockPlane(blockType,4,Vector3(0,1,16),Vector3(16,1,16),Vector3(16,1,0),Vector3(0,1,0));
            BuildGUIBlockPlane(blockType,5,Vector3(0,1,0),Vector3(16,1,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildGUIBlockPlane(blockType,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,1,16),Vector3(0,1,16));
        }

       /* if(i == Cake::getID())
        {
            BuildGUIBlockPlane(blockType,1,Vector3(0,0,16),Vector3(0,8,16),Vector3(0,8,0),Vector3(0,0,0));
            BuildGUIBlockPlane(blockType,2,Vector3(16,0,0),Vector3(16,8,0),Vector3(16,8,16),Vector3(16,0,16));
            BuildGUIBlockPlane(blockType,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
            BuildGUIBlockPlane(blockType,4,Vector3(0,8,16),Vector3(16,8,16),Vector3(16,8,0),Vector3(0,8,0));
            BuildGUIBlockPlane(blockType,5,Vector3(0,8,0),Vector3(16,8,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildGUIBlockPlane(blockType,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,8,16),Vector3(0,8,16));
        }*/

        if(i == TrapdoorClosed1::getID())
        {
            BuildGUIBlockPlane(blockType,1,Vector3(0,0,16),Vector3(0,3,16),Vector3(0,3,0),Vector3(0,0,0));
            BuildGUIBlockPlane(blockType,2,Vector3(16,0,0),Vector3(16,3,0),Vector3(16,3,16),Vector3(16,0,16));
            BuildGUIBlockPlane(blockType,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
            BuildGUIBlockPlane(blockType,4,Vector3(0,3,16),Vector3(16,3,16),Vector3(16,3,0),Vector3(0,3,0));
            BuildGUIBlockPlane(blockType,5,Vector3(0,3,0),Vector3(16,3,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildGUIBlockPlane(blockType,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,3,16),Vector3(0,3,16));
        }

        if(i == Snow2::getID())
        {
            BuildGUIBlockPlane(blockType,1,Vector3(0,0,16),Vector3(0,2,16),Vector3(0,2,0),Vector3(0,0,0));
            BuildGUIBlockPlane(blockType,2,Vector3(16,0,0),Vector3(16,2,0),Vector3(16,2,16),Vector3(16,0,16));
            BuildGUIBlockPlane(blockType,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
            BuildGUIBlockPlane(blockType,4,Vector3(0,2,16),Vector3(16,2,16),Vector3(16,2,0),Vector3(0,2,0));
            BuildGUIBlockPlane(blockType,5,Vector3(0,2,0),Vector3(16,2,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildGUIBlockPlane(blockType,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,2,16),Vector3(0,2,16));
        }

        if(i == Torch::getID())
        {
            BuildGUIBlockPlane(blockType,1,Vector3(7,0,9),Vector3(7,10,9),Vector3(7,10,7),Vector3(7,0,7));
            BuildGUIBlockPlane(blockType,2,Vector3(10,0,7),Vector3(9,10,7),Vector3(9,10,9),Vector3(9,0,9));
            BuildGUIBlockPlane(blockType,3,Vector3(7,0,7),Vector3(9,0,7),Vector3(9,0,9),Vector3(7,0,10));
            BuildGUIBlockPlane(blockType,4,Vector3(7,10,9),Vector3(9,10,9),Vector3(9,10,7),Vector3(7,10,7));
            BuildGUIBlockPlane(blockType,5,Vector3(7,10,7),Vector3(9,10,7),Vector3(9,0,7),Vector3(7,0,7));
            BuildGUIBlockPlane(blockType,6,Vector3(7,0,9),Vector3(9,0,9),Vector3(9,10,9),Vector3(7,10,9));
        }

        if(i == 112 || i == 9 || i == 4 || i == 37 || i == 38)
        {
            BuildGUIBlockPlane(blockType,1,Vector3(0,0,16),Vector3(0,16,16),Vector3(0,16,0),Vector3(0,0,0));
            BuildGUIBlockPlane(blockType,2,Vector3(16,0,0),Vector3(16,16,0),Vector3(16,16,16),Vector3(16,0,16));
            BuildGUIBlockPlane(blockType,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
            BuildGUIBlockPlane(blockType,4,Vector3(0,16,16),Vector3(16,16,16),Vector3(16,16,0),Vector3(0,16,0));
            BuildGUIBlockPlane(blockType,5,Vector3(0,16,0),Vector3(16,16,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildGUIBlockPlane(blockType,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,16,16),Vector3(0,16,16));
        }

        if(i == CactusBlock::getID())
        {
            BuildGUIBlockPlane(blockType,1,Vector3(1,0,16),Vector3(1,16,16),Vector3(1,16,0),Vector3(1,0,0));
            BuildGUIBlockPlane(blockType,2,Vector3(15,0,0),Vector3(15,16,0),Vector3(15,16,16),Vector3(15,0,16));
            BuildGUIBlockPlane(blockType,3,Vector3(1,0,1),Vector3(15,0,1),Vector3(15,0,15),Vector3(1,0,15));
            BuildGUIBlockPlane(blockType,4,Vector3(0,16,15),Vector3(15,16,15),Vector3(15,16,1),Vector3(1,16,1));
            BuildGUIBlockPlane(blockType,5,Vector3(0,16,1),Vector3(16,16,1),Vector3(16,0,1),Vector3(0,0,1));
            BuildGUIBlockPlane(blockType,6,Vector3(0,0,15),Vector3(16,0,15),Vector3(16,16,15),Vector3(0,16,15));
        }

        if(i == ChestBlock3::getID())
        {
            BuildGUIBlockPlane(blockType,1,Vector3(1,0,15),Vector3(1,14,15),Vector3(1,14,1),Vector3(1,0,1));
            BuildGUIBlockPlane(blockType,2,Vector3(15,0,1),Vector3(15,14,1),Vector3(15,14,15),Vector3(15,0,15));
            BuildGUIBlockPlane(blockType,3,Vector3(1,0,1),Vector3(15,0,1),Vector3(15,0,15),Vector3(1,0,15));
            BuildGUIBlockPlane(blockType,4,Vector3(1,14,15),Vector3(15,14,15),Vector3(15,14,1),Vector3(1,14,1));
            BuildGUIBlockPlane(blockType,5,Vector3(1,14,1),Vector3(15,14,1),Vector3(15,0,1),Vector3(1,0,1));
            BuildGUIBlockPlane(blockType,6,Vector3(1,0,15),Vector3(15,0,15),Vector3(15,14,15),Vector3(1,14,15));

            blockType = &blockTypes[98];

            BuildGUIBlockPlane(blockType,1,Vector3(7,7,1),Vector3(7,11,1),Vector3(7,11,0),Vector3(7,7,0));
            BuildGUIBlockPlane(blockType,2,Vector3(9,7,0),Vector3(9,11,0),Vector3(9,11,1),Vector3(9,7,1));
            BuildGUIBlockPlane(blockType,4,Vector3(7,11,1),Vector3(9,11,1),Vector3(9,11,0),Vector3(7,11,0));
            BuildGUIBlockPlane(blockType,5,Vector3(7,11,0),Vector3(9,11,0),Vector3(9,7,0),Vector3(7,7,0));
        }
    }

    if(blockType->blockModel == 6)
    {
        BuildGUIBlockPlane(blockType,1,Vector3(0,0,16),Vector3(0,8,16),Vector3(0,8,0),Vector3(0,0,0));
        BuildGUIBlockPlane(blockType,2,Vector3(16,0,0),Vector3(16,8,0),Vector3(16,8,16),Vector3(16,0,16));
        BuildGUIBlockPlane(blockType,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
        BuildGUIBlockPlane(blockType,4,Vector3(0,8,8),Vector3(16,8,8),Vector3(16,8,0),Vector3(0,8,0));
        BuildGUIBlockPlane(blockType,5,Vector3(0,8,0),Vector3(16,8,0),Vector3(16,0,0),Vector3(0,0,0));
        BuildGUIBlockPlane(blockType,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,8,16),Vector3(0,8,16));

        BuildGUIBlockPlane(blockType,1,Vector3(0,8,16),Vector3(0,16,16),Vector3(0,16,8),Vector3(0,8,8));
        BuildGUIBlockPlane(blockType,2,Vector3(16,8,8),Vector3(16,16,8),Vector3(16,16,16),Vector3(16,8,16));
        BuildGUIBlockPlane(blockType,4,Vector3(0,16,16),Vector3(16,16,16),Vector3(16,16,8),Vector3(0,16,8));
        BuildGUIBlockPlane(blockType,5,Vector3(0,16,8),Vector3(16,16,8),Vector3(16,8,8),Vector3(0,8,8));
        BuildGUIBlockPlane(blockType,6,Vector3(0,8,16),Vector3(16,8,16),Vector3(16,16,16),Vector3(0,16,16));
    }


    if(blockType->blockModel == 2)
    {
        //
        BuildGUIBlockPlane(blockType,1,Vector3(6,0,16),Vector3(6,16,16),Vector3(6,16,12),Vector3(6,0,12));
        BuildGUIBlockPlane(blockType,2,Vector3(10,0,12),Vector3(10,16,12),Vector3(10,16,16),Vector3(10,0,16));
        BuildGUIBlockPlane(blockType,3,Vector3(6,0,12),Vector3(10,0,12),Vector3(10,0,16),Vector3(6,0,16));
        BuildGUIBlockPlane(blockType,4,Vector3(6,16,16),Vector3(10,16,16),Vector3(10,16,12),Vector3(6,16,12));
        BuildGUIBlockPlane(blockType,5,Vector3(6,16,12),Vector3(10,16,12),Vector3(10,0,12),Vector3(6,0,12));
        BuildGUIBlockPlane(blockType,6,Vector3(6,0,16),Vector3(10,0,16),Vector3(10,16,16),Vector3(6,16,16));

        BuildGUIBlockPlane(blockType,1,Vector3(7,12,12),Vector3(7,15,12),Vector3(7,15,4),Vector3(7,12,4));
        BuildGUIBlockPlane(blockType,2,Vector3(9,12,4),Vector3(9,15,4),Vector3(9,15,12),Vector3(9,12,12));
        BuildGUIBlockPlane(blockType,3,Vector3(7,12,4),Vector3(9,12,4),Vector3(9,12,12),Vector3(7,12,12));
        BuildGUIBlockPlane(blockType,4,Vector3(7,15,12),Vector3(9,15,12),Vector3(9,15,4),Vector3(7,15,4));

        BuildGUIBlockPlane(blockType,1,Vector3(7,5,12),Vector3(7,8,12),Vector3(7,8,4),Vector3(7,5,4));
        BuildGUIBlockPlane(blockType,2,Vector3(9,5,4),Vector3(9,8,4),Vector3(9,8,12),Vector3(9,5,12));
        BuildGUIBlockPlane(blockType,3,Vector3(7,5,4),Vector3(9,5,4),Vector3(9,5,12),Vector3(7,5,12));
        BuildGUIBlockPlane(blockType,4,Vector3(7,8,12),Vector3(9,8,12),Vector3(9,8,4),Vector3(7,8,4));

        BuildGUIBlockPlane(blockType,1,Vector3(6,0,4),Vector3(6,16,4),Vector3(6,16,0),Vector3(6,0,0));
        BuildGUIBlockPlane(blockType,2,Vector3(10,0,0),Vector3(10,16,0),Vector3(10,16,4),Vector3(10,0,4));
        BuildGUIBlockPlane(blockType,3,Vector3(6,0,0),Vector3(10,0,0),Vector3(10,0,4),Vector3(6,0,4));
        BuildGUIBlockPlane(blockType,4,Vector3(6,16,4),Vector3(10,16,4),Vector3(10,16,0),Vector3(6,16,0));
        BuildGUIBlockPlane(blockType,5,Vector3(6,16,0),Vector3(10,16,0),Vector3(10,0,0),Vector3(6,0,0));
        BuildGUIBlockPlane(blockType,6,Vector3(6,0,4),Vector3(10,0,4),Vector3(10,16,4),Vector3(6,16,4));
    }

    if(blockType->blockModel == 1)
    {
        BuildGUIBlockPlane(blockType,1,Vector3(0,0,16),Vector3(0,8,16),Vector3(0,8,0),Vector3(0,0,0));
        BuildGUIBlockPlane(blockType,2,Vector3(16,0,0),Vector3(16,8,0),Vector3(16,8,16),Vector3(16,0,16));
        BuildGUIBlockPlane(blockType,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
        BuildGUIBlockPlane(blockType,4,Vector3(0,8,16),Vector3(16,8,16),Vector3(16,8,0),Vector3(0,8,0));
        BuildGUIBlockPlane(blockType,5,Vector3(0,8,0),Vector3(16,8,0),Vector3(16,0,0),Vector3(0,0,0));
        BuildGUIBlockPlane(blockType,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,8,16),Vector3(0,8,16));
    }

    blockTypes[i].verticesLeftRight = (TexturesPSPVertex*)memalign(16,( mTriangleLeftRight.size() * 3) * sizeof(TexturesPSPVertex));
    blockTypes[i].pointsLeftRight = pointsLeftRight;
    //build verts
    //vertices
    int vert = 0;
    unsigned int size =  mTriangleLeftRight.size();
    for(unsigned int j = 0; j < size; j++)
    {
        blockTypes[i].verticesLeftRight[vert].u = mtexturesLeftRight[mTriangleLeftRight[j]->x]->x;
        blockTypes[i].verticesLeftRight[vert].v = mtexturesLeftRight[mTriangleLeftRight[j]->x]->y;
        blockTypes[i].verticesLeftRight[vert].x = mPositionLeftRight[mTriangleLeftRight[j]->x]->x;
        blockTypes[i].verticesLeftRight[vert].y = mPositionLeftRight[mTriangleLeftRight[j]->x]->y;
        blockTypes[i].verticesLeftRight[vert].z = mPositionLeftRight[mTriangleLeftRight[j]->x]->z;
        vert++;

        blockTypes[i].verticesLeftRight[vert].u = mtexturesLeftRight[mTriangleLeftRight[j]->y]->x;
        blockTypes[i].verticesLeftRight[vert].v = mtexturesLeftRight[mTriangleLeftRight[j]->y]->y;
        blockTypes[i].verticesLeftRight[vert].x = mPositionLeftRight[mTriangleLeftRight[j]->y]->x;
        blockTypes[i].verticesLeftRight[vert].y = mPositionLeftRight[mTriangleLeftRight[j]->y]->y;
        blockTypes[i].verticesLeftRight[vert].z = mPositionLeftRight[mTriangleLeftRight[j]->y]->z;
        vert++;

        blockTypes[i].verticesLeftRight[vert].u = mtexturesLeftRight[mTriangleLeftRight[j]->z]->x;
        blockTypes[i].verticesLeftRight[vert].v = mtexturesLeftRight[mTriangleLeftRight[j]->z]->y;
        blockTypes[i].verticesLeftRight[vert].x = mPositionLeftRight[mTriangleLeftRight[j]->z]->x;
        blockTypes[i].verticesLeftRight[vert].y = mPositionLeftRight[mTriangleLeftRight[j]->z]->y;
        blockTypes[i].verticesLeftRight[vert].z = mPositionLeftRight[mTriangleLeftRight[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(blockTypes[i].verticesLeftRight,(mTriangleLeftRight.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPositionLeftRight.size(); aa++)
    {
        delete mPositionLeftRight[aa];
        delete mtexturesLeftRight[aa];
    }
    mPositionLeftRight.clear();
    mtexturesLeftRight.clear();

    for(unsigned int aa = 0; aa < mTriangleLeftRight.size(); aa++)
    {
        delete mTriangleLeftRight[aa];
    }
    mTriangleLeftRight.clear();



    blockTypes[i].verticesBackFront = (TexturesPSPVertex*)memalign(16,(mTriangleBackFront.size() * 3) * sizeof(TexturesPSPVertex));
    blockTypes[i].pointsBackFront = pointsBackFront;
    //build verts
    //vertices
    vert = 0;
    size =  mTriangleBackFront.size();
    for(unsigned int j = 0; j < size; j++)
    {
        blockTypes[i].verticesBackFront[vert].u = mtexturesBackFront[mTriangleBackFront[j]->x]->x;
        blockTypes[i].verticesBackFront[vert].v = mtexturesBackFront[mTriangleBackFront[j]->x]->y;
        blockTypes[i].verticesBackFront[vert].x = mPositionBackFront[mTriangleBackFront[j]->x]->x;
        blockTypes[i].verticesBackFront[vert].y = mPositionBackFront[mTriangleBackFront[j]->x]->y;
        blockTypes[i].verticesBackFront[vert].z = mPositionBackFront[mTriangleBackFront[j]->x]->z;
        vert++;

        blockTypes[i].verticesBackFront[vert].u = mtexturesBackFront[mTriangleBackFront[j]->y]->x;
        blockTypes[i].verticesBackFront[vert].v = mtexturesBackFront[mTriangleBackFront[j]->y]->y;
        blockTypes[i].verticesBackFront[vert].x = mPositionBackFront[mTriangleBackFront[j]->y]->x;
        blockTypes[i].verticesBackFront[vert].y = mPositionBackFront[mTriangleBackFront[j]->y]->y;
        blockTypes[i].verticesBackFront[vert].z = mPositionBackFront[mTriangleBackFront[j]->y]->z;
        vert++;

        blockTypes[i].verticesBackFront[vert].u = mtexturesBackFront[mTriangleBackFront[j]->z]->x;
        blockTypes[i].verticesBackFront[vert].v = mtexturesBackFront[mTriangleBackFront[j]->z]->y;
        blockTypes[i].verticesBackFront[vert].x = mPositionBackFront[mTriangleBackFront[j]->z]->x;
        blockTypes[i].verticesBackFront[vert].y = mPositionBackFront[mTriangleBackFront[j]->z]->y;
        blockTypes[i].verticesBackFront[vert].z = mPositionBackFront[mTriangleBackFront[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(blockTypes[i].verticesBackFront,(mTriangleBackFront.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPositionBackFront.size(); aa++)
    {
        delete mPositionBackFront[aa];
        delete mtexturesBackFront[aa];
    }
    mPositionBackFront.clear();
    mtexturesBackFront.clear();

    for(unsigned int aa = 0; aa < mTriangleBackFront.size(); aa++)
    {
        delete mTriangleBackFront[aa];
    }
    mTriangleBackFront.clear();



    blockTypes[i].verticesBottomTop = (TexturesPSPVertex*)memalign(16,(mTriangleBottomTop.size() * 3) * sizeof(TexturesPSPVertex));
    blockTypes[i].pointsBottomTop = pointsBottomTop;
    //build verts
    //vertices
    vert = 0;
    size =  mTriangleBottomTop.size();
    for(unsigned int j = 0; j < size; j++)
    {
        blockTypes[i].verticesBottomTop[vert].u = mtexturesBottomTop[mTriangleBottomTop[j]->x]->x;
        blockTypes[i].verticesBottomTop[vert].v = mtexturesBottomTop[mTriangleBottomTop[j]->x]->y;
        blockTypes[i].verticesBottomTop[vert].x = mPositionBottomTop[mTriangleBottomTop[j]->x]->x;
        blockTypes[i].verticesBottomTop[vert].y = mPositionBottomTop[mTriangleBottomTop[j]->x]->y;
        blockTypes[i].verticesBottomTop[vert].z = mPositionBottomTop[mTriangleBottomTop[j]->x]->z;
        vert++;

        blockTypes[i].verticesBottomTop[vert].u = mtexturesBottomTop[mTriangleBottomTop[j]->y]->x;
        blockTypes[i].verticesBottomTop[vert].v = mtexturesBottomTop[mTriangleBottomTop[j]->y]->y;
        blockTypes[i].verticesBottomTop[vert].x = mPositionBottomTop[mTriangleBottomTop[j]->y]->x;
        blockTypes[i].verticesBottomTop[vert].y = mPositionBottomTop[mTriangleBottomTop[j]->y]->y;
        blockTypes[i].verticesBottomTop[vert].z = mPositionBottomTop[mTriangleBottomTop[j]->y]->z;
        vert++;

        blockTypes[i].verticesBottomTop[vert].u = mtexturesBottomTop[mTriangleBottomTop[j]->z]->x;
        blockTypes[i].verticesBottomTop[vert].v = mtexturesBottomTop[mTriangleBottomTop[j]->z]->y;
        blockTypes[i].verticesBottomTop[vert].x = mPositionBottomTop[mTriangleBottomTop[j]->z]->x;
        blockTypes[i].verticesBottomTop[vert].y = mPositionBottomTop[mTriangleBottomTop[j]->z]->y;
        blockTypes[i].verticesBottomTop[vert].z = mPositionBottomTop[mTriangleBottomTop[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(blockTypes[i].verticesBottomTop,(mTriangleBottomTop.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPositionBottomTop.size(); aa++)
    {
        delete mPositionBottomTop[aa];
        delete mtexturesBottomTop[aa];
    }
    mPositionBottomTop.clear();
    mtexturesBottomTop.clear();

    for(unsigned int aa = 0; aa < mTriangleBottomTop.size(); aa++)
    {
        delete mTriangleBottomTop[aa];
    }
    mTriangleBottomTop.clear();
}

void CraftWorld::GetCloudsVerts(BaseBlock *blockType)
{
    std::vector<Vector3*> mPosition;
    std::vector<Vector2*> mtextures;
    std::vector<Vector3*> mTriangle;

    int iVertex = 0;
    int points = 0;

    float x = 0;
    float y = 0;
    float z = 0;

    iVertex = 0;
    points = 0;

    for(float l = 1; l <= 10; l += 1)
    {
        for(float j = 1; j <= 10; j += 1)//0-|-
        {
            mPosition.push_back(new Vector3(x+((l-1)*48),   y, z+((j-1)*48)));
            mtextures.push_back(new Vector2(0+(l-1)/40, 0+(j-1)/10));
            mPosition.push_back(new Vector3(x+(l*48), y, z+((j-1)*48)));
            mtextures.push_back(new Vector2(l/40, 0+(j-1)/10));
            mPosition.push_back(new Vector3(x+(l*48), y, z+(j*48)));
            mtextures.push_back(new Vector2(l/40, j/10));
            mPosition.push_back(new Vector3(x+((l-1)*48),   y, z+(j*48)));
            mtextures.push_back(new Vector2(0+(l-1)/40, j/10));

            mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
            mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

            iVertex += 4;
            points += 6;
        }
    }

    blockTypes[141].verticesBottomTop = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    blockTypes[141].pointsBottomTop = points;
    //build verts
    //vertices
    int vert = 0;
    unsigned int size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        blockTypes[141].verticesBottomTop[vert].u = mtextures[mTriangle[j]->x]->x;
        blockTypes[141].verticesBottomTop[vert].v = mtextures[mTriangle[j]->x]->y;
        blockTypes[141].verticesBottomTop[vert].x = mPosition[mTriangle[j]->x]->x;
        blockTypes[141].verticesBottomTop[vert].y = mPosition[mTriangle[j]->x]->y;
        blockTypes[141].verticesBottomTop[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        blockTypes[141].verticesBottomTop[vert].u = mtextures[mTriangle[j]->y]->x;
        blockTypes[141].verticesBottomTop[vert].v = mtextures[mTriangle[j]->y]->y;
        blockTypes[141].verticesBottomTop[vert].x = mPosition[mTriangle[j]->y]->x;
        blockTypes[141].verticesBottomTop[vert].y = mPosition[mTriangle[j]->y]->y;
        blockTypes[141].verticesBottomTop[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        blockTypes[141].verticesBottomTop[vert].u = mtextures[mTriangle[j]->z]->x;
        blockTypes[141].verticesBottomTop[vert].v = mtextures[mTriangle[j]->z]->y;
        blockTypes[141].verticesBottomTop[vert].x = mPosition[mTriangle[j]->z]->x;
        blockTypes[141].verticesBottomTop[vert].y = mPosition[mTriangle[j]->z]->y;
        blockTypes[141].verticesBottomTop[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(blockTypes[141].verticesBottomTop,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
        mPosition.clear();
        mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();
}


void CraftWorld::GetNormalBlockVerts(int i,BaseBlock *blockType)
{
    iVertexLeftRight = 0;
    pointsLeftRight = 0;
    iVertexBackFront = 0;
    pointsBackFront = 0;
    iVertexBottomTop = 0;
    pointsBottomTop = 0;

    BuildGUIBlockPlane(blockType,1,Vector3(0,0,16),Vector3(0,16,16),Vector3(0,16,0),Vector3(0,0,0));
    BuildGUIBlockPlane(blockType,2,Vector3(16,0,0),Vector3(16,16,0),Vector3(16,16,16),Vector3(16,0,16));
    BuildGUIBlockPlane(blockType,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
    BuildGUIBlockPlane(blockType,4,Vector3(0,16,16),Vector3(16,16,16),Vector3(16,16,0),Vector3(0,16,0));
    BuildGUIBlockPlane(blockType,5,Vector3(0,16,0),Vector3(16,16,0),Vector3(16,0,0),Vector3(0,0,0));
    BuildGUIBlockPlane(blockType,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,16,16),Vector3(0,16,16));

    blockTypes[i].verticesLeftRight = (TexturesPSPVertex*)memalign(16,( mTriangleLeftRight.size() * 3) * sizeof(TexturesPSPVertex));
    blockTypes[i].pointsLeftRight = pointsLeftRight;
    //build verts
    //vertices
    int vert = 0;
    unsigned int size =  mTriangleLeftRight.size();
    for(unsigned int j = 0; j < size; j++)
    {
        blockTypes[i].verticesLeftRight[vert].u = mtexturesLeftRight[mTriangleLeftRight[j]->x]->x;
        blockTypes[i].verticesLeftRight[vert].v = mtexturesLeftRight[mTriangleLeftRight[j]->x]->y;
        blockTypes[i].verticesLeftRight[vert].x = mPositionLeftRight[mTriangleLeftRight[j]->x]->x;
        blockTypes[i].verticesLeftRight[vert].y = mPositionLeftRight[mTriangleLeftRight[j]->x]->y;
        blockTypes[i].verticesLeftRight[vert].z = mPositionLeftRight[mTriangleLeftRight[j]->x]->z;
        vert++;

        blockTypes[i].verticesLeftRight[vert].u = mtexturesLeftRight[mTriangleLeftRight[j]->y]->x;
        blockTypes[i].verticesLeftRight[vert].v = mtexturesLeftRight[mTriangleLeftRight[j]->y]->y;
        blockTypes[i].verticesLeftRight[vert].x = mPositionLeftRight[mTriangleLeftRight[j]->y]->x;
        blockTypes[i].verticesLeftRight[vert].y = mPositionLeftRight[mTriangleLeftRight[j]->y]->y;
        blockTypes[i].verticesLeftRight[vert].z = mPositionLeftRight[mTriangleLeftRight[j]->y]->z;
        vert++;

        blockTypes[i].verticesLeftRight[vert].u = mtexturesLeftRight[mTriangleLeftRight[j]->z]->x;
        blockTypes[i].verticesLeftRight[vert].v = mtexturesLeftRight[mTriangleLeftRight[j]->z]->y;
        blockTypes[i].verticesLeftRight[vert].x = mPositionLeftRight[mTriangleLeftRight[j]->z]->x;
        blockTypes[i].verticesLeftRight[vert].y = mPositionLeftRight[mTriangleLeftRight[j]->z]->y;
        blockTypes[i].verticesLeftRight[vert].z = mPositionLeftRight[mTriangleLeftRight[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(blockTypes[i].verticesLeftRight,(mTriangleLeftRight.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPositionLeftRight.size(); aa++)
    {
        delete mPositionLeftRight[aa];
        delete mtexturesLeftRight[aa];
    }
    mPositionLeftRight.clear();
    mtexturesLeftRight.clear();

    for(unsigned int aa = 0; aa < mTriangleLeftRight.size(); aa++)
    {
        delete mTriangleLeftRight[aa];
    }
    mTriangleLeftRight.clear();



    blockTypes[i].verticesBackFront = (TexturesPSPVertex*)memalign(16,(mTriangleBackFront.size() * 3) * sizeof(TexturesPSPVertex));
    blockTypes[i].pointsBackFront = pointsBackFront;
    //build verts
    //vertices
    vert = 0;
    size =  mTriangleBackFront.size();
    for(unsigned int j = 0; j < size; j++)
    {
        blockTypes[i].verticesBackFront[vert].u = mtexturesBackFront[mTriangleBackFront[j]->x]->x;
        blockTypes[i].verticesBackFront[vert].v = mtexturesBackFront[mTriangleBackFront[j]->x]->y;
        blockTypes[i].verticesBackFront[vert].x = mPositionBackFront[mTriangleBackFront[j]->x]->x;
        blockTypes[i].verticesBackFront[vert].y = mPositionBackFront[mTriangleBackFront[j]->x]->y;
        blockTypes[i].verticesBackFront[vert].z = mPositionBackFront[mTriangleBackFront[j]->x]->z;
        vert++;

        blockTypes[i].verticesBackFront[vert].u = mtexturesBackFront[mTriangleBackFront[j]->y]->x;
        blockTypes[i].verticesBackFront[vert].v = mtexturesBackFront[mTriangleBackFront[j]->y]->y;
        blockTypes[i].verticesBackFront[vert].x = mPositionBackFront[mTriangleBackFront[j]->y]->x;
        blockTypes[i].verticesBackFront[vert].y = mPositionBackFront[mTriangleBackFront[j]->y]->y;
        blockTypes[i].verticesBackFront[vert].z = mPositionBackFront[mTriangleBackFront[j]->y]->z;
        vert++;

        blockTypes[i].verticesBackFront[vert].u = mtexturesBackFront[mTriangleBackFront[j]->z]->x;
        blockTypes[i].verticesBackFront[vert].v = mtexturesBackFront[mTriangleBackFront[j]->z]->y;
        blockTypes[i].verticesBackFront[vert].x = mPositionBackFront[mTriangleBackFront[j]->z]->x;
        blockTypes[i].verticesBackFront[vert].y = mPositionBackFront[mTriangleBackFront[j]->z]->y;
        blockTypes[i].verticesBackFront[vert].z = mPositionBackFront[mTriangleBackFront[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(blockTypes[i].verticesBackFront,(mTriangleBackFront.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPositionBackFront.size(); aa++)
    {
        delete mPositionBackFront[aa];
        delete mtexturesBackFront[aa];
    }
    mPositionBackFront.clear();
    mtexturesBackFront.clear();

    for(unsigned int aa = 0; aa < mTriangleBackFront.size(); aa++)
    {
        delete mTriangleBackFront[aa];
    }
    mTriangleBackFront.clear();



    blockTypes[i].verticesBottomTop = (TexturesPSPVertex*)memalign(16,(mTriangleBottomTop.size() * 3) * sizeof(TexturesPSPVertex));
    blockTypes[i].pointsBottomTop = pointsBottomTop;
    //build verts
    //vertices
    vert = 0;
    size =  mTriangleBottomTop.size();
    for(unsigned int j = 0; j < size; j++)
    {
        blockTypes[i].verticesBottomTop[vert].u = mtexturesBottomTop[mTriangleBottomTop[j]->x]->x;
        blockTypes[i].verticesBottomTop[vert].v = mtexturesBottomTop[mTriangleBottomTop[j]->x]->y;
        blockTypes[i].verticesBottomTop[vert].x = mPositionBottomTop[mTriangleBottomTop[j]->x]->x;
        blockTypes[i].verticesBottomTop[vert].y = mPositionBottomTop[mTriangleBottomTop[j]->x]->y;
        blockTypes[i].verticesBottomTop[vert].z = mPositionBottomTop[mTriangleBottomTop[j]->x]->z;
        vert++;

        blockTypes[i].verticesBottomTop[vert].u = mtexturesBottomTop[mTriangleBottomTop[j]->y]->x;
        blockTypes[i].verticesBottomTop[vert].v = mtexturesBottomTop[mTriangleBottomTop[j]->y]->y;
        blockTypes[i].verticesBottomTop[vert].x = mPositionBottomTop[mTriangleBottomTop[j]->y]->x;
        blockTypes[i].verticesBottomTop[vert].y = mPositionBottomTop[mTriangleBottomTop[j]->y]->y;
        blockTypes[i].verticesBottomTop[vert].z = mPositionBottomTop[mTriangleBottomTop[j]->y]->z;
        vert++;

        blockTypes[i].verticesBottomTop[vert].u = mtexturesBottomTop[mTriangleBottomTop[j]->z]->x;
        blockTypes[i].verticesBottomTop[vert].v = mtexturesBottomTop[mTriangleBottomTop[j]->z]->y;
        blockTypes[i].verticesBottomTop[vert].x = mPositionBottomTop[mTriangleBottomTop[j]->z]->x;
        blockTypes[i].verticesBottomTop[vert].y = mPositionBottomTop[mTriangleBottomTop[j]->z]->y;
        blockTypes[i].verticesBottomTop[vert].z = mPositionBottomTop[mTriangleBottomTop[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(blockTypes[i].verticesBottomTop,(mTriangleBottomTop.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPositionBottomTop.size(); aa++)
    {
        delete mPositionBottomTop[aa];
        delete mtexturesBottomTop[aa];
    }
    mPositionBottomTop.clear();
    mtexturesBottomTop.clear();

    for(unsigned int aa = 0; aa < mTriangleBottomTop.size(); aa++)
    {
        delete mTriangleBottomTop[aa];
    }
    mTriangleBottomTop.clear();
}

void CraftWorld::GetItemVerts(int i,BaseItem *itemType)
{
    std::vector<Vector3*> mPosition;
    std::vector<Vector2*> mtextures;
    std::vector<Vector3*> mTriangle;

    std::vector<Vector3*> mPosition2;
    std::vector<Vector2*> mtextures2;
    std::vector<Vector3*> mTriangle2;

    int iVertex = 0;
    int points = 0;

    int iVertex2 = 0;
    int points2 = 0;

    float x = -0.5f;
    float y = -0.5f;
    float z = -1.0f;

    float pixel = 1.0f / 16.0f;
    float texturepixel = percent3 / 16.0f;

    float down = 1.0f - percent3 * (itemType->textureRow + 1);
    float up = down + percent3;

    float left = percent3 * itemType->plane;
    float right = left + percent3;

    int startPixelsX = itemType->plane*16;
    int startPixelsY = 256 - ((itemType->textureRow + 1)*16);

    iVertex = 0;
    //light
    float BlockLight  = 1.0f;  //For the two x faces

    mPosition.push_back(new Vector3(x,   y,   z+1));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+1, y,   z+1));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+1, y+1, z+1));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+1, z+1));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    itemTypes[i].vertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    itemTypes[i].points = points;
    //build verts
    //vertices
    int vert = 0;
    unsigned int size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        itemTypes[i].vertices[vert].u = mtextures[mTriangle[j]->x]->x;
        itemTypes[i].vertices[vert].v = mtextures[mTriangle[j]->x]->y;
        itemTypes[i].vertices[vert].x = mPosition[mTriangle[j]->x]->x;
        itemTypes[i].vertices[vert].y = mPosition[mTriangle[j]->x]->y;
        itemTypes[i].vertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        itemTypes[i].vertices[vert].u = mtextures[mTriangle[j]->y]->x;
        itemTypes[i].vertices[vert].v = mtextures[mTriangle[j]->y]->y;
        itemTypes[i].vertices[vert].x = mPosition[mTriangle[j]->y]->x;
        itemTypes[i].vertices[vert].y = mPosition[mTriangle[j]->y]->y;
        itemTypes[i].vertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        itemTypes[i].vertices[vert].u = mtextures[mTriangle[j]->z]->x;
        itemTypes[i].vertices[vert].v = mtextures[mTriangle[j]->z]->y;
        itemTypes[i].vertices[vert].x = mPosition[mTriangle[j]->z]->x;
        itemTypes[i].vertices[vert].y = mPosition[mTriangle[j]->z]->y;
        itemTypes[i].vertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(itemTypes[i].vertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete mTriangle[aa];
    }
    mTriangle.clear();

    // drop vertices
    float size2 = 0.25f;
    iVertex = 0;

    mPosition.push_back(new Vector3(0,   -size2,   size2));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(0, size2,   size2));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(0, size2, -size2));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(0,   -size2, -size2));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;

    itemTypes[i].dropVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    vert = 0;
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        itemTypes[i].dropVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        itemTypes[i].dropVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        itemTypes[i].dropVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        itemTypes[i].dropVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        itemTypes[i].dropVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        itemTypes[i].dropVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        itemTypes[i].dropVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        itemTypes[i].dropVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        itemTypes[i].dropVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        itemTypes[i].dropVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        itemTypes[i].dropVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        itemTypes[i].dropVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        itemTypes[i].dropVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        itemTypes[i].dropVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        itemTypes[i].dropVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(itemTypes[i].dropVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete mTriangle[aa];
    }
    mTriangle.clear();

    // add vertices
    points = 0;
    iVertex = 0;

    points2 = 0;
    iVertex2 = 0;

    int texture;
    itemTypes[i].terrainTexture == true ? texture = textureTerrainId : texture = textureItemsId;

    if(TextureManager::Instance()->Images[texture]->Width == 256)
    {
        for(int i = 15; i >= 0; i--)
        {
            for(int j = 15; j >= 0; j--)
            {
                int xx = startPixelsX+i;
                int yy = startPixelsY+j;
                if(TextureManager::Instance()->getAlpha(xx,yy,texture) == 255)
                {
                   /* if( (i == 0) || (i != 0 && TextureManager::Instance()->getAlpha(xx-1,yy,texture) == 0))
                    {
                        mPosition.push_back(new Vector3(x+i*pixel,y+j*pixel,   z+pixel*17));
                        mtextures.push_back(new Vector2(left+texturepixel*(i+1), down+texturepixel*(j+1)));

                        mPosition.push_back(new Vector3(x+i*pixel, y+(j+1)*pixel,   z+pixel*17));
                        mtextures.push_back(new Vector2(left+texturepixel*(i+1), down+texturepixel*j));

                        mPosition.push_back(new Vector3(x+i*pixel, y+(j+1)*pixel, z+pixel*16));
                        mtextures.push_back(new Vector2(left+texturepixel*i, down+texturepixel*j));

                        mPosition.push_back(new Vector3(x+i*pixel, y+j*pixel, z+pixel*16));
                        mtextures.push_back(new Vector2(left+texturepixel*i, down+texturepixel*(j+1)));

                        mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
                        mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

                        iVertex += 4;
                        points += 6;
                    } */

                    if( (j == 255) || (j != 255 && TextureManager::Instance()->getAlpha(xx,yy+1,texture) == 0) )
                    {
                        mPosition.push_back(new Vector3(x+i*pixel,y+(j+1)*pixel,   z+pixel*17));
                        mtextures.push_back(new Vector2(left+texturepixel*i, down+texturepixel*j));

                        mPosition.push_back(new Vector3(x+(i+1)*pixel, y+(j+1)*pixel,   z+pixel*17));
                        mtextures.push_back(new Vector2(left+texturepixel*(i+1), down+texturepixel*j));

                        mPosition.push_back(new Vector3(x+(i+1)*pixel, y+(j+1)*pixel, z+pixel*16));
                        mtextures.push_back(new Vector2(left+texturepixel*(i+1), down+texturepixel*(j+1)));

                        mPosition.push_back(new Vector3(x+i*pixel, y+(j+1)*pixel, z+pixel*16));
                        mtextures.push_back(new Vector2(left+texturepixel*i, down+texturepixel*(j+1)));

                        mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
                        mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

                        iVertex += 4;
                        points += 6;
                    }

                    if( (i == 255) || (i != 255 && TextureManager::Instance()->getAlpha(xx+1,yy,texture) == 0))
                    {
                        mPosition2.push_back(new Vector3(x+(i+1)*pixel,y+j*pixel,   z+pixel*17));
                        mtextures2.push_back(new Vector2(left+texturepixel*(i+1), down+texturepixel*(j+1)));

                        mPosition2.push_back(new Vector3(x+(i+1)*pixel, y+(j+1)*pixel,   z+pixel*17));
                        mtextures2.push_back(new Vector2(left+texturepixel*(i+1), down+texturepixel*j));

                        mPosition2.push_back(new Vector3(x+(i+1)*pixel, y+(j+1)*pixel, z+pixel*16));
                        mtextures2.push_back(new Vector2(left+texturepixel*i, down+texturepixel*j));

                        mPosition2.push_back(new Vector3(x+(i+1)*pixel, y+j*pixel, z+pixel*16));
                        mtextures2.push_back(new Vector2(left+texturepixel*i, down+texturepixel*(j+1)));

                        mTriangle2.push_back(new Vector3(iVertex2, iVertex2+1, iVertex2+2));
                        mTriangle2.push_back(new Vector3(iVertex2+2, iVertex2+3, iVertex2));

                        iVertex2 += 4;
                        points2 += 6;
                    }

                    if( (j == 0) || (j != 0 && TextureManager::Instance()->getAlpha(xx,yy-1,texture) == 0) )
                    {
                        mPosition2.push_back(new Vector3(x+i*pixel,y+j*pixel,   z+pixel*17));
                        mtextures2.push_back(new Vector2(left+texturepixel*i, down+texturepixel*j));

                        mPosition2.push_back(new Vector3(x+(i+1)*pixel, y+j*pixel,   z+pixel*17));
                        mtextures2.push_back(new Vector2(left+texturepixel*(i+1), down+texturepixel*j));

                        mPosition2.push_back(new Vector3(x+(i+1)*pixel, y+j*pixel, z+pixel*16));
                        mtextures2.push_back(new Vector2(left+texturepixel*(i+1), down+texturepixel*(j+1)));

                        mPosition2.push_back(new Vector3(x+i*pixel, y+j*pixel, z+pixel*16));
                        mtextures2.push_back(new Vector2(left+texturepixel*i, down+texturepixel*(j+1)));

                        mTriangle2.push_back(new Vector3(iVertex2, iVertex2+1, iVertex2+2));
                        mTriangle2.push_back(new Vector3(iVertex2+2, iVertex2+3, iVertex2));

                        iVertex2 += 4;
                        points2 += 6;
                    }
                }
            }
        }
    }

    itemTypes[i].addVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    itemTypes[i].addPoints = points;


    //build verts for addVertices
    //vertices
    vert = 0;
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        itemTypes[i].addVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        itemTypes[i].addVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        itemTypes[i].addVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        itemTypes[i].addVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        itemTypes[i].addVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        itemTypes[i].addVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        itemTypes[i].addVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        itemTypes[i].addVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        itemTypes[i].addVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        itemTypes[i].addVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        itemTypes[i].addVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        itemTypes[i].addVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        itemTypes[i].addVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        itemTypes[i].addVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        itemTypes[i].addVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(itemTypes[i].addVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete mTriangle[aa];
    }
    mTriangle.clear();

    itemTypes[i].add2Vertices = (TexturesPSPVertex*)memalign(16,( mTriangle2.size() * 3) * sizeof(TexturesPSPVertex));
    itemTypes[i].add2Points = points2;

    //build verts for add2Vertices
    //vertices
    vert = 0;
    size =  mTriangle2.size();
    for(unsigned int j = 0; j < size; j++)
    {
        itemTypes[i].add2Vertices[vert].u = mtextures2[mTriangle2[j]->x]->x;
        itemTypes[i].add2Vertices[vert].v = mtextures2[mTriangle2[j]->x]->y;
        itemTypes[i].add2Vertices[vert].x = mPosition2[mTriangle2[j]->x]->x;
        itemTypes[i].add2Vertices[vert].y = mPosition2[mTriangle2[j]->x]->y;
        itemTypes[i].add2Vertices[vert].z = mPosition2[mTriangle2[j]->x]->z;
        vert++;

        itemTypes[i].add2Vertices[vert].u = mtextures2[mTriangle2[j]->y]->x;
        itemTypes[i].add2Vertices[vert].v = mtextures2[mTriangle2[j]->y]->y;
        itemTypes[i].add2Vertices[vert].x = mPosition2[mTriangle2[j]->y]->x;
        itemTypes[i].add2Vertices[vert].y = mPosition2[mTriangle2[j]->y]->y;
        itemTypes[i].add2Vertices[vert].z = mPosition2[mTriangle2[j]->y]->z;
        vert++;

        itemTypes[i].add2Vertices[vert].u = mtextures2[mTriangle2[j]->z]->x;
        itemTypes[i].add2Vertices[vert].v = mtextures2[mTriangle2[j]->z]->y;
        itemTypes[i].add2Vertices[vert].x = mPosition2[mTriangle2[j]->z]->x;
        itemTypes[i].add2Vertices[vert].y = mPosition2[mTriangle2[j]->z]->y;
        itemTypes[i].add2Vertices[vert].z = mPosition2[mTriangle2[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(itemTypes[i].add2Vertices,( mTriangle2.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition2.size(); aa++)
    {
        delete mPosition2[aa];
        delete mtextures2[aa];
    }
    mPosition2.clear();
    mtextures2.clear();

    for(unsigned int aa = 0; aa < mTriangle2.size(); aa++)
    {
        delete mTriangle2[aa];
    }
    mTriangle2.clear();
}

void CraftWorld::buildblocksVerts()
{

    for(unsigned int i = 0; i < itemTypes.size(); i++)
    {
        BaseItem *itemType = &itemTypes[i];
        GetItemVerts(i,itemType);
    }
    //create vertices for each block type
    for(unsigned int i = 1; i < blockTypes.size(); i++)
    {
        BaseBlock *blockType = &blockTypes[i];
        if(blockType->blockModel > 0)
            GetSpecialBlockVerts(i,blockType);
        else
            GetNormalBlockVerts(i,blockType);
    }

}

void CraftWorld::buildcloudsVerts()
{
    BaseBlock *blockType = &blockTypes[141];
    GetCloudsVerts(blockType);
}

void CraftWorld::buildZombieVerts()
{

}

void CraftWorld::buildmobVerts()
{
    std::vector<Vector3*> mPosition;
    std::vector<Vector2*> mtextures;
    std::vector<Vector3*> mTriangle;

    int iVertex = 0;
    int points = 0;
    unsigned int size = 0;

    float pixel = 1 / 16.0f;
    float textureYpixel;
    float textureXpixel;

    float x = pixel * -2;
    float y = pixel * -6;
    float z = pixel * -4;

    float down = 0.0f;
    float up = 0.0f;

    float left = 0.0f;
    float right = 0.0f;

    int vert = 0;

    /// * --- BODY --- *

    left = 0.5f;
    right = left + 0.125f;

    down = 0.625f;
    up = down + 0.375f;

    mPosition.push_back(new Vector3(x, y, z+pixel*8));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*12, z+pixel*8));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*12, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.3125f;
    right = left + 0.125f;

    mPosition.push_back(new Vector3(x+pixel*4, y,   z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*8));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z+pixel*8));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.4375f;
    right = left + 0.125f;

    down = 0.5f;
    up = down + 0.125f;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*4, y, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y, z+pixel*8));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y, z+pixel*8));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.3125f;
    right = left + 0.125f;

    down = 0.5f;
    up = down + 0.125f;

    mPosition.push_back(new Vector3(x,   y+pixel*12, z+pixel*8));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*8));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*12, z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.25f;
    right = left + 0.0625f;

    down = 0.625f;
    up = down + 0.375f;

    mPosition.push_back(new Vector3(x,   y+pixel*12, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.4375f;
    right = left + 0.0625f;

    down = 0.625f;
    up = down + 0.375f;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*8));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z+pixel*8));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*8));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*12, z+pixel*8));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    bodyVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices

    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        bodyVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        bodyVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        bodyVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        bodyVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        bodyVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        bodyVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        bodyVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        bodyVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        bodyVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        bodyVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        bodyVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        bodyVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        bodyVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        bodyVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        bodyVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(bodyVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();

    /// * --- HEAD --- *

    x = pixel * -4;
    y = pixel * -4;
    z = pixel * -4;

    vert = 0;
    iVertex = 0;
    points = 0;
    size = 0;

    left = 0.375f;
    right = left + 0.125f;

    down = 0.25;
    up = down + 0.25;

    mPosition.push_back(new Vector3(x, y, z+pixel*8));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*8, z+pixel*8));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*8, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.125f;
    right = left + 0.125f;

    mPosition.push_back(new Vector3(x+pixel*8, y,   z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*8, y+pixel*8, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*8, y+pixel*8, z+pixel*8));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*8, y,   z+pixel*8));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.25f;
    right = left + 0.125f;

    down = 0;
    up = down + 0.25f;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*8, y, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*8, y, z+pixel*8));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y, z+pixel*8));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.125f;
    right = left + 0.125f;

    down = 0;
    up = down + 0.25f;

    mPosition.push_back(new Vector3(x,   y+pixel*8, z+pixel*8));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*8, y+pixel*8, z+pixel*8));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*8, y+pixel*8, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*8, z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.125;
    right = 0;

    down = 0.25;
    up = down + 0.25;

    mPosition.push_back(new Vector3(x,   y+pixel*8, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*8, y+pixel*8, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*8, y,   z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.375;
    right = 0.25;

    down = 0.25;
    up = down + 0.25;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*8));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*8, y,   z+pixel*8));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*8, y+pixel*8, z+pixel*8));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*8, z+pixel*8));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    headVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        headVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        headVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        headVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        headVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        headVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        headVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        headVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        headVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        headVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        headVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        headVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        headVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        headVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        headVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        headVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(headVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();



    /// * --- HAT --- *

    x = pixel * -4.5f;
    y = pixel * -4.5f;
    z = pixel * -4.5f;

    vert = 0;
    iVertex = 0;
    points = 0;
    size = 0;

    left = 0.875f;
    right = left + 0.125f;

    down = 0.25;
    up = down + 0.25;

    mPosition.push_back(new Vector3(x, y, z+pixel*9));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*9, z+pixel*9));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*9, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.625f;
    right = left + 0.125f;

    mPosition.push_back(new Vector3(x+pixel*9, y,   z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*9, y+pixel*9, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*9, y+pixel*9, z+pixel*9));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*9, y,   z+pixel*9));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.75f;
    right = left + 0.125f;

    down = 0;
    up = down + 0.25f;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*9, y, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*9, y, z+pixel*9));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y, z+pixel*9));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.625f;
    right = left + 0.125f;

    down = 0;
    up = down + 0.25f;

    mPosition.push_back(new Vector3(x,   y+pixel*9, z+pixel*9));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*9, y+pixel*9, z+pixel*9));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*9, y+pixel*9, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*9, z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.625;
    right = 0.5;

    down = 0.25;
    up = down + 0.25;

    mPosition.push_back(new Vector3(x,   y+pixel*9, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*9, y+pixel*9, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*9, y,   z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.875;
    right = 0.75;

    down = 0.25;
    up = down + 0.25;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*9));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*9, y,   z+pixel*9));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*9, y+pixel*9, z+pixel*9));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*9, z+pixel*9));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    hatVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        hatVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        hatVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        hatVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        hatVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        hatVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        hatVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        hatVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        hatVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        hatVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        hatVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        hatVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        hatVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        hatVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        hatVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        hatVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(hatVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();



    /// * --- HAND --- *

    x = pixel * -2;
    y = pixel * -2;
    z = pixel * -2;

    iVertex = 0;
    points = 0;
    vert = 0;
    size = 0;

    left = 0.8125f;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x, y, z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*12, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.6875f;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x+pixel*4, y,   z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z+pixel*4));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.6875f;
    right = left + 0.0625f;

    down = 0.5;
    up = down + 0.125;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y, z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y, z+pixel*4));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.75f;
    right = left + 0.0625f;

    down = 0.5;
    up = down + 0.125;

    mPosition.push_back(new Vector3(x,   y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*12, z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.625f;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x,   y+pixel*12, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.75f;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*4));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    handVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        handVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        handVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        handVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        handVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        handVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        handVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        handVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        handVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        handVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        handVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        handVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        handVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        handVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        handVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        handVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(handVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();




    /// * --- LEG --- *

    x = pixel * -2;
    y = pixel * -2;
    z = pixel * -2;

    iVertex = 0;
    points = 0;
    vert = 0;
    size = 0;

    left = 0.8125f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x, y, z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*12, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.6875 - 0.625f;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x+pixel*4, y,   z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z+pixel*4));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.6875f - 0.625;
    right = left + 0.0625f;

    down = 0.5;
    up = down + 0.125;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y, z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y, z+pixel*4));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.75f - 0.625;
    right = left + 0.0625f;

    down = 0.5;
    up = down + 0.125;

    mPosition.push_back(new Vector3(x,   y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*12, z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.625f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x,   y+pixel*12, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.75f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*4));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    legVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        legVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        legVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        legVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        legVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        legVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        legVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        legVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        legVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        legVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        legVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        legVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        legVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        legVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        legVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        legVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(legVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();



    /// * --- cowBodyVertices --- *

    x = pixel * -9;
    y = pixel * -5;
    z = pixel * -6;

    vert = 0;
    iVertex = 0;
    points = 0;
    size = 0;

    left = 40.0f/64.0f;
    right = left + 12.0f/64.0f;

    down = 4.0f/32.0f;
    up = down + 10.0f/32.0f;

    mPosition.push_back(new Vector3(x, y, z+pixel*12));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*10, z+pixel*12));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*10, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    //face
    left = 28.0f/64.0f;
    right = left + 12.0f/64.0f;

    down = 4.0f/32.0f;
    up = down + 10.0f/32.0f;

    mPosition.push_back(new Vector3(x+pixel*18, y,   z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*18, y+pixel*10, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*18, y+pixel*10, z+pixel*12));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*18, y,   z+pixel*12));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    // bottom
    left = 28.0f/64.0f;
    right = left + 12.0f/64.0f;

    down = 14.0f/32.0f;
    up = down + 18.0f/32.0f;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*18, y, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*18, y, z+pixel*12));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y, z+pixel*12));
    mtextures.push_back(new Vector2(right, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    // top
    left = 50.0f/64.0f;
    right = left + 12.0f/64.0f;

    down = 14.0f/32.0f;
    up = down + 18.0f/32.0f;

    mPosition.push_back(new Vector3(x,   y+pixel*10, z+pixel*12));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*18, y+pixel*10, z+pixel*12));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*18, y+pixel*10, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x,   y+pixel*10, z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    // left
    left = 18.0f/64.0f;
    right = left + 10.0f/64.0f;

    down = 14.0f/32.0f;
    up = down + 18.0f/32.0f;

    mPosition.push_back(new Vector3(x,   y+pixel*10, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*18, y+pixel*10, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*18, y,   z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    // right
    left = 40.0f/64.0f;
    right = left + 10.0f/64.0f;

    down = 14.0f/32.0f;
    up = down + 18.0f/32.0f;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*12));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*18, y,   z+pixel*12));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*18, y+pixel*10, z+pixel*12));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*10, z+pixel*12));
    mtextures.push_back(new Vector2(right, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    cowBodyVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices

    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        cowBodyVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        cowBodyVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        cowBodyVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        cowBodyVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        cowBodyVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        cowBodyVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        cowBodyVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        cowBodyVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        cowBodyVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        cowBodyVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        cowBodyVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        cowBodyVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        cowBodyVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        cowBodyVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        cowBodyVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(cowBodyVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();



    /// * --- cowHeadVertices --- *

    x = pixel * -3;
    y = pixel * -4;
    z = pixel * -4;

    vert = 0;
    iVertex = 0;
    points = 0;
    size = 0;

    left = 20.0f/64.0f;
    right = left + 8.0f/64.0f;

    down = 6.0f/32.0f;
    up = down + 8.0f/32.0f;

    mPosition.push_back(new Vector3(x, y, z+pixel*8));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*8, z+pixel*8));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*8, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    //face
    left = 6.0f/64.0f;
    right = left + 8.0f/64.0f;

    mPosition.push_back(new Vector3(x+pixel*6, y,   z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*8, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*8, z+pixel*8));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z+pixel*8));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 14.0f/64.0f;
    right = left + 8.0f/64.0f;

    down = 0;
    up = down + 6.0f/32.0f;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*6, y, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*6, y, z+pixel*8));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y, z+pixel*8));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 6.0f/64.0f;
    right = left + 8.0f/64.0f;

    down = 0;
    up = down + 6.0f/32.0f;

    mPosition.push_back(new Vector3(x,   y+pixel*8, z+pixel*8));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*8, z+pixel*8));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*8, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*8, z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    left = 6.0f/64.0f;
    right = 0;

    down = 6.0f/32.0f;
    up = down + 8.0f/32.0f;

    mPosition.push_back(new Vector3(x,   y+pixel*8, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*8, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    left = 14.0f/64.0f;
    right = 20.0f/64.0f;

    down = 6.0f/32.0f;
    up = down + 8.0f/32.0f;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*8));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z+pixel*8));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*8, z+pixel*8));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*8, z+pixel*8));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    cowHeadVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        cowHeadVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        cowHeadVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        cowHeadVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        cowHeadVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        cowHeadVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        cowHeadVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        cowHeadVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        cowHeadVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        cowHeadVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        cowHeadVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        cowHeadVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        cowHeadVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        cowHeadVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        cowHeadVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        cowHeadVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(cowHeadVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();



    /// * --- cowHornsVertices --- *

    x = pixel * -0.5f;
    y = pixel * -2.0f;
    z = pixel * -0.5f;

    vert = 0;
    iVertex = 0;
    points = 0;
    size = 0;

    textureXpixel = 1/64.0f;
    textureYpixel = 1/32.0f;

    left = textureXpixel*22;
    right = left + textureXpixel;

    down = textureYpixel;
    up = down + textureYpixel*3;

    mPosition.push_back(new Vector3(x, y, z+pixel*1));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*3, z+pixel*1));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*3, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*23;
    right = left + textureXpixel;

    mPosition.push_back(new Vector3(x+pixel*1, y,   z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*1, y+pixel*3, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*1, y+pixel*3, z+pixel*1));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*1, y,   z+pixel*1));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*24;
    right = left + textureXpixel;

    down = 0;
    up = down + textureYpixel;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*1, y, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*1, y, z+pixel*1));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y, z+pixel*1));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*23;
    right = left + textureXpixel;

    down = 0;
    up = down + textureYpixel;

    mPosition.push_back(new Vector3(x,   y+pixel*3, z+pixel*1));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*1, y+pixel*3, z+pixel*1));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*1, y+pixel*3, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*3, z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    left = textureXpixel*24;
    right = left + textureXpixel;

    down = textureYpixel;
    up = down + textureYpixel*3;

    mPosition.push_back(new Vector3(x,   y+pixel*3, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*1, y+pixel*3, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*1, y,   z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    left = textureXpixel*25;
    right = left + textureXpixel;

    down = textureYpixel;
    up = down + textureYpixel*3;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*1));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*1, y,   z+pixel*1));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*1, y+pixel*3, z+pixel*1));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*3, z+pixel*1));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    cowHornsVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        cowHornsVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        cowHornsVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        cowHornsVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        cowHornsVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        cowHornsVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        cowHornsVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        cowHornsVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        cowHornsVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        cowHornsVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        cowHornsVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        cowHornsVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        cowHornsVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        cowHornsVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        cowHornsVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        cowHornsVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(cowHornsVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();


    /// * --- cowLegVertices --- *


    x = pixel * -2;
    y = pixel * -2;
    z = pixel * -2;

    iVertex = 0;
    points = 0;
    vert = 0;
    size = 0;

    left = 0.8125f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x, y, z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*12, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.6875 - 0.625f;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x+pixel*4, y,   z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z+pixel*4));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.75f - 0.625;
    right = left + 0.0625f;

    down = 0.5;
    up = down + 0.125;

    mPosition.push_back(new Vector3(x,   y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*12, z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.625f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x,   y+pixel*12, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.75f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*4));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    left = 0.6875f - 0.625;
    right = left + 0.0625f;

    down = 0.5;
    up = down + 0.125;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y, z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y, z+pixel*4));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    cowLegVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        cowLegVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        cowLegVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        cowLegVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        cowLegVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        cowLegVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        cowLegVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        cowLegVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        cowLegVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        cowLegVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        cowLegVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        cowLegVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        cowLegVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        cowLegVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        cowLegVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        cowLegVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(cowLegVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();

    /*
    /// * --- sheepLegOPTVertices --- *


    x = pixel * -2;
    y = pixel * -2;
    z = pixel * -2;

    iVertex = 0;
    points = 0;
    vert = 0;
    size = 0;

    left = 0.8125f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x, y, z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*12, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.6875 - 0.625f;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x+pixel*4, y,   z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z+pixel*4));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    left = 0.75f - 0.625;
    right = left + 0.0625f;

    down = 0.5;
    up = down + 0.125;

    mPosition.push_back(new Vector3(x,   y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*12, z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.625f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x,   y+pixel*12, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.75f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*4));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*12, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    sheepLegOPTVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        sheepLegOPTVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        sheepLegOPTVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        sheepLegOPTVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        sheepLegOPTVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        sheepLegOPTVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        sheepLegOPTVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        sheepLegOPTVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        sheepLegOPTVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        sheepLegOPTVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        sheepLegOPTVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        sheepLegOPTVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        sheepLegOPTVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        sheepLegOPTVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        sheepLegOPTVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        sheepLegOPTVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(sheepLegOPTVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();

    */

    /// * --- creeperLegVertices --- *

    textureXpixel = 1/64.0f;
    textureYpixel = 1/32.0f;

    x = pixel * -2;
    y = pixel * 0;
    z = pixel * -2;

    iVertex = 0;
    points = 0;
    vert = 0;
    size = 0;

    left = textureXpixel*12;
    right = left + textureXpixel*4;

    down = textureYpixel*20;
    up = down + textureYpixel*6;

    mPosition.push_back(new Vector3(x, y, z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*6, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*6, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*4;
    right = left + textureXpixel*4;

    down = textureYpixel*20;
    up = down + textureYpixel*6;

    mPosition.push_back(new Vector3(x+pixel*4, y,   z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*6, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*6, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z+pixel*4));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*4;
    right = left + textureXpixel*4;

    down = textureYpixel*16;
    up = down + textureYpixel*4;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y, z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y, z+pixel*4));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*8;
    right = left + textureXpixel*4;

    down = textureYpixel*16;
    up = down + textureYpixel*4;

    mPosition.push_back(new Vector3(x,   y+pixel*6, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*6, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*6, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*6, z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0;
    right = left + textureXpixel*4;

    down = textureYpixel*20;
    up = down + textureYpixel*6;

    mPosition.push_back(new Vector3(x,   y+pixel*6, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*6, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*8;
    right = left + textureXpixel*4;

    down = textureYpixel*20;
    up = down + textureYpixel*6;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*4));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*4, y,   z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*6, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*6, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    creeperLegVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        creeperLegVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        creeperLegVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        creeperLegVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        creeperLegVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        creeperLegVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        creeperLegVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        creeperLegVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        creeperLegVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        creeperLegVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        creeperLegVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        creeperLegVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        creeperLegVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        creeperLegVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        creeperLegVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        creeperLegVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(creeperLegVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();




    /// * --- sheepBodyVertices --- *

    textureXpixel = 1/64.0f;
    textureYpixel = 1/32.0f;

    x = pixel * -8;
    y = pixel * -3;
    z = pixel * -4;

    iVertex = 0;
    points = 0;
    vert = 0;
    size = 0;

    left = textureXpixel*34;
    right = left + textureXpixel*8;

    down = textureYpixel*8;
    up = down + textureYpixel*6;

    mPosition.push_back(new Vector3(x, y, z+pixel*8));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*6, z+pixel*8));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*6, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*42;
    right = left + textureXpixel*8;

    down = textureYpixel*8;
    up = down + textureYpixel*6;

    mPosition.push_back(new Vector3(x+pixel*16, y,   z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*16, y+pixel*6, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*16, y+pixel*6, z+pixel*8));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*16, y,   z+pixel*8));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*28;
    right = left + textureXpixel*8;

    down = textureYpixel*14;
    up = down + textureYpixel*16;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*16, y, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*16, y, z+pixel*8));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y, z+pixel*8));
    mtextures.push_back(new Vector2(right, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*42;
    right = left + textureXpixel*8;

    down = textureYpixel*14;
    up = down + textureYpixel*16;

    mPosition.push_back(new Vector3(x,   y+pixel*6, z+pixel*8));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*16, y+pixel*6, z+pixel*8));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*16, y+pixel*6, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*6, z));
    mtextures.push_back(new Vector2(right, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*36;
    right = left + textureXpixel*6;

    down = textureYpixel*14;
    up = down + textureYpixel*16;

    mPosition.push_back(new Vector3(x,   y+pixel*6, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*16, y+pixel*6, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*16, y,   z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*50;
    right = left + textureXpixel*6;

    down = textureYpixel*14;
    up = down + textureYpixel*16;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*8));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*16, y,   z+pixel*8));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*16, y+pixel*6, z+pixel*8));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*6, z+pixel*8));
    mtextures.push_back(new Vector2(right, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    sheepBodyVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        sheepBodyVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        sheepBodyVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        sheepBodyVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        sheepBodyVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        sheepBodyVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        sheepBodyVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        sheepBodyVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        sheepBodyVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        sheepBodyVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        sheepBodyVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        sheepBodyVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        sheepBodyVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        sheepBodyVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        sheepBodyVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        sheepBodyVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(sheepBodyVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();




    /// * --- sheepHeadFurVertices --- *

    textureXpixel = 1/64.0f;
    textureYpixel = 1/32.0f;

    x = pixel * -3;
    y = pixel * -3;
    z = pixel * -3;

    iVertex = 0;
    points = 0;
    vert = 0;
    size = 0;

    left = textureXpixel*18;
    right = left + textureXpixel*6;

    down = textureYpixel*12;
    up = down - textureYpixel*6;

    mPosition.push_back(new Vector3(x, y, z+pixel*6));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*6, z+pixel*6));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*6, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*6;
    right = left + textureXpixel*6;

    down = textureYpixel*12;
    up = down - textureYpixel*6;

    mPosition.push_back(new Vector3(x+pixel*6, y,   z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*6, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*6, z+pixel*6));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z+pixel*6));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*12;
    right = left + textureXpixel*6;

    down = textureYpixel*0;
    up = down + textureYpixel*6;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*6, y, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*6, y, z+pixel*6));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y, z+pixel*6));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*6;
    right = left + textureXpixel*6;

    down = textureYpixel*0;
    up = down + textureYpixel*6;

    mPosition.push_back(new Vector3(x,   y+pixel*6, z+pixel*6));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*6, z+pixel*6));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*6, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*6, z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*6;
    right = 0;

    down = textureYpixel*6;
    up = down + textureYpixel*6;

    mPosition.push_back(new Vector3(x,   y+pixel*6, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*6, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*18;
    right = left - textureXpixel*6;

    down = textureYpixel*6;
    up = down + textureYpixel*6;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*6));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z+pixel*6));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*6, z+pixel*6));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*6, z+pixel*6));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    sheepHeadFurVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        sheepHeadFurVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        sheepHeadFurVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        sheepHeadFurVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        sheepHeadFurVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        sheepHeadFurVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        sheepHeadFurVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        sheepHeadFurVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        sheepHeadFurVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        sheepHeadFurVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        sheepHeadFurVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        sheepHeadFurVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        sheepHeadFurVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        sheepHeadFurVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        sheepHeadFurVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        sheepHeadFurVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(sheepHeadFurVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();




    /// * --- sheepHeadVertices --- *

    textureXpixel = 1/64.0f;
    textureYpixel = 1/32.0f;

    x = pixel * -4;
    y = pixel * -3;
    z = pixel * -3;

    iVertex = 0;
    points = 0;
    vert = 0;
    size = 0;

    left = textureXpixel*8;
    right = left + textureXpixel*6;

    down = textureYpixel*14;
    up = down - textureYpixel*6;

    mPosition.push_back(new Vector3(x+pixel*8, y,   z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*8, y+pixel*6, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*8, y+pixel*6, z+pixel*6));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*8, y,   z+pixel*6));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*14;
    right = left + textureXpixel*6;

    down = textureYpixel*0;
    up = down + textureYpixel*8;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*8, y, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*8, y, z+pixel*6));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y, z+pixel*6));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*8;
    right = left + textureXpixel*6;

    down = textureYpixel*0;
    up = down + textureYpixel*8;

    mPosition.push_back(new Vector3(x,   y+pixel*6, z+pixel*6));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*8, y+pixel*6, z+pixel*6));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*8, y+pixel*6, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*6, z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*8;
    right = 0;

    down = textureYpixel*8;
    up = down + textureYpixel*6;

    mPosition.push_back(new Vector3(x,   y+pixel*6, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*8, y+pixel*6, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*8, y,   z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*22;
    right = left - textureXpixel*8;

    down = textureYpixel*8;
    up = down + textureYpixel*6;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*6));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*8, y,   z+pixel*6));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*8, y+pixel*6, z+pixel*6));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*6, z+pixel*6));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = textureXpixel*22;
    right = left + textureXpixel*6;

    down = textureYpixel*14;
    up = down - textureYpixel*6;

    mPosition.push_back(new Vector3(x, y, z+pixel*6));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*6, z+pixel*6));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*6, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    sheepHeadVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        sheepHeadVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        sheepHeadVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        sheepHeadVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        sheepHeadVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        sheepHeadVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        sheepHeadVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        sheepHeadVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        sheepHeadVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        sheepHeadVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        sheepHeadVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        sheepHeadVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        sheepHeadVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        sheepHeadVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        sheepHeadVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        sheepHeadVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(sheepHeadVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();



    /// * --- sheepLegFurVertices --- *


    x = pixel * -2;
    y = pixel * -2;
    z = pixel * -2;

    iVertex = 0;
    points = 0;
    vert = 0;
    size = 0;

    left = 0.8125f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.1875f;
    down += 0.03125*0.4f;

    mPosition.push_back(new Vector3(x, y+pixel*0.4f, z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*6, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*6, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x, y+pixel*0.4f,   z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.6875 - 0.625f;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.1875f;
    down += 0.03125*0.4f;

    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*0.4f,   z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*6, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*6, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*0.4f,   z+pixel*4));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    /*left = 0.6875f - 0.625;
    right = left + 0.0625f;

    down = 0.5;
    up = down + 0.125;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y, z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y, z+pixel*4));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;*/

    left = 0.75f - 0.625;
    right = left + 0.0625f;

    down = 0.5;
    up = down + 0.125;

    mPosition.push_back(new Vector3(x,   y+pixel*6, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*6, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*6, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*6, z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.625f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.1875f;
    down += 0.03125*0.4f;

    mPosition.push_back(new Vector3(x,   y+pixel*6, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*6, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*0.4f,   z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x,   y+pixel*0.4f,   z));
    mtextures.push_back(new Vector2(right, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.75f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.1875f;
    down += 0.03125*0.4f;

    mPosition.push_back(new Vector3(x,   y+pixel*0.4f,   z+pixel*4));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*0.4f,   z+pixel*4));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*4, y+pixel*6, z+pixel*4));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*6, z+pixel*4));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    sheepLegFurVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        sheepLegFurVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        sheepLegFurVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        sheepLegFurVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        sheepLegFurVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        sheepLegFurVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        sheepLegFurVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        sheepLegFurVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        sheepLegFurVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        sheepLegFurVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        sheepLegFurVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        sheepLegFurVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        sheepLegFurVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        sheepLegFurVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        sheepLegFurVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        sheepLegFurVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(sheepLegFurVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();



    /// * --- helmetVertices --- *

    x = pixel * -5;
    y = pixel * -5;
    z = pixel * -5;

    vert = 0;
    iVertex = 0;
    points = 0;
    size = 0;

    left = 0.375f;
    right = left + 0.125f;

    down = 0.25;
    up = down + 0.25;

    mPosition.push_back(new Vector3(x, y, z+pixel*10));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*10, z+pixel*10));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*10, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.125f;
    right = left + 0.125f;

    mPosition.push_back(new Vector3(x+pixel*10, y,   z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*10, y+pixel*10, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*10, y+pixel*10, z+pixel*10));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*10, y,   z+pixel*10));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;



    left = 0.125f;
    right = left + 0.125f;

    down = 0;
    up = down + 0.25f;

    mPosition.push_back(new Vector3(x,   y+pixel*10, z+pixel*10));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*10, y+pixel*10, z+pixel*10));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*10, y+pixel*10, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*10, z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;



    left = 0.125;
    right = 0;

    down = 0.25;
    up = down + 0.25;

    mPosition.push_back(new Vector3(x,   y+pixel*10, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*10, y+pixel*10, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*10, y,   z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;



    left = 0.375;
    right = 0.25;

    down = 0.25;
    up = down + 0.25;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*10));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*10, y,   z+pixel*10));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*10, y+pixel*10, z+pixel*10));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*10, z+pixel*10));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    helmetVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        helmetVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        helmetVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        helmetVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        helmetVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        helmetVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        helmetVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        helmetVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        helmetVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        helmetVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        helmetVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        helmetVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        helmetVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        helmetVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        helmetVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        helmetVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(helmetVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();





    /// * --- shoulderVertices --- *

    x = pixel * -3;
    y = pixel * -3;
    z = pixel * -3;

    iVertex = 0;
    points = 0;
    vert = 0;
    size = 0;

    left = 0.8125f;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x, y, z+pixel*6));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*14, z+pixel*6));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*14, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.6875f;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x+pixel*6, y,   z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*14, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*14, z+pixel*6));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z+pixel*6));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.6875f;
    right = left + 0.0625f;

    down = 0.5;
    up = down + 0.125;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*6, y, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*6, y, z+pixel*6));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y, z+pixel*6));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    left = 0.625f;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x,   y+pixel*14, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*14, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.75f;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*6));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z+pixel*6));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*14, z+pixel*6));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*14, z+pixel*6));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    shoulderVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        shoulderVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        shoulderVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        shoulderVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        shoulderVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        shoulderVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        shoulderVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        shoulderVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        shoulderVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        shoulderVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        shoulderVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        shoulderVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        shoulderVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        shoulderVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        shoulderVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        shoulderVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(shoulderVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();






    /// * --- chestplateVertices --- *

    x = pixel * -3;
    y = pixel * -7;
    z = pixel * -5;

    iVertex = 0;
    points = 0;
    vert = 0;
    size = 0;

    left = 0.5f;
    right = left + 0.125f;

    down = 0.625f;
    up = down + 0.375f;

    mPosition.push_back(new Vector3(x, y, z+pixel*10));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*14, z+pixel*10));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*14, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.3125f;
    right = left + 0.125f;

    mPosition.push_back(new Vector3(x+pixel*6, y,   z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*14, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*14, z+pixel*10));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z+pixel*10));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    left = 0.25f;
    right = left + 0.0625f;

    down = 0.625f;
    up = down + 0.375f;

    mPosition.push_back(new Vector3(x,   y+pixel*14, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*14, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.4375f;
    right = left + 0.0625f;

    down = 0.625f;
    up = down + 0.375f;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*10));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z+pixel*10));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*14, z+pixel*10));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*14, z+pixel*10));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    chestplateVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices

    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        chestplateVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        chestplateVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        chestplateVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        chestplateVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        chestplateVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        chestplateVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        chestplateVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        chestplateVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        chestplateVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        chestplateVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        chestplateVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        chestplateVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        chestplateVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        chestplateVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        chestplateVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(chestplateVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();




    /// * --- bellyVertices --- *

    x = pixel * -2.5f;
    y = pixel * -6;
    z = pixel * -4.5f;

    iVertex = 0;
    points = 0;
    vert = 0;
    size = 0;

    left = 0.5f;
    right = left + 0.125f;

    down = 0.625f;
    up = down + 0.375f;

    mPosition.push_back(new Vector3(x, y, z+pixel*9));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*12, z+pixel*9));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*12, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.3125f;
    right = left + 0.125f;

    mPosition.push_back(new Vector3(x+pixel*5, y,   z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*5, y+pixel*12, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*5, y+pixel*12, z+pixel*9));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*5, y,   z+pixel*9));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.25f;
    right = left + 0.0625f;

    down = 0.625f;
    up = down + 0.375f;

    mPosition.push_back(new Vector3(x,   y+pixel*12, z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*5, y+pixel*12, z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*5, y,   z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.4375f;
    right = left + 0.0625f;

    down = 0.625f;
    up = down + 0.375f;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*9));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*5, y,   z+pixel*9));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*5, y+pixel*12, z+pixel*9));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y+pixel*12, z+pixel*9));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    bellyVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices

    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        bellyVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        bellyVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        bellyVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        bellyVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        bellyVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        bellyVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        bellyVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        bellyVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        bellyVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        bellyVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        bellyVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        bellyVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        bellyVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        bellyVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        bellyVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(bellyVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();



    /// * --- leggingVertices --- *

    x = pixel * -2.5f;
    y = pixel * -2.5f;
    z = pixel * -2.5f;

    iVertex = 0;
    points = 0;
    vert = 0;
    size = 0;

    left = 0.8125f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x, y, z+pixel*5));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*13, z+pixel*5));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*13, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.6875 - 0.625f;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x+pixel*5, y,   z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*5, y+pixel*13, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*5, y+pixel*13, z+pixel*5));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*5, y,   z+pixel*5));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    left = 0.625f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x,   y+pixel*13, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*5, y+pixel*13, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*5, y,   z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.75f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*5));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*5, y,   z+pixel*5));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*5, y+pixel*13, z+pixel*5));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*13, z+pixel*5));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;



    leggingVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        leggingVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        leggingVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        leggingVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        leggingVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        leggingVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        leggingVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        leggingVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        leggingVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        leggingVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        leggingVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        leggingVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        leggingVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        leggingVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        leggingVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        leggingVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(leggingVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();




    /// * --- bootVertices --- *

    x = pixel * -3;
    y = pixel * -3;
    z = pixel * -3;

    iVertex = 0;
    points = 0;
    vert = 0;
    size = 0;

    left = 0.8125f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x, y, z+pixel*6));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x, y+pixel*14, z+pixel*6));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x, y+pixel*14, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x, y,   z));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;



    left = 0.6875 - 0.625f;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x+pixel*6, y,   z));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*14, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*14, z+pixel*6));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z+pixel*6));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;



    left = 0.6875f - 0.625;
    right = left + 0.0625f;

    down = 0.5;
    up = down + 0.125;

    mPosition.push_back(new Vector3(x,   y, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*6, y, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*6, y, z+pixel*6));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x,   y, z+pixel*6));
    mtextures.push_back(new Vector2(left, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;



    left = 0.625f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x,   y+pixel*14, z));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*14, z));
    mtextures.push_back(new Vector2(left, up));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x,   y,   z));
    mtextures.push_back(new Vector2(right, down));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;

    left = 0.75f - 0.625;
    right = left + 0.0625f;

    down = 0.625;
    up = down + 0.375;

    mPosition.push_back(new Vector3(x,   y,   z+pixel*6));
    mtextures.push_back(new Vector2(left, down));
    mPosition.push_back(new Vector3(x+pixel*6, y,   z+pixel*6));
    mtextures.push_back(new Vector2(right, down));
    mPosition.push_back(new Vector3(x+pixel*6, y+pixel*14, z+pixel*6));
    mtextures.push_back(new Vector2(right, up));
    mPosition.push_back(new Vector3(x,   y+pixel*14, z+pixel*6));
    mtextures.push_back(new Vector2(left, up));

    mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
    mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

    iVertex += 4;
    points += 6;


    bootVertices = (TexturesPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //build verts
    //vertices
    size =  mTriangle.size();
    for(unsigned int j = 0; j < size; j++)
    {
        bootVertices[vert].u = mtextures[mTriangle[j]->x]->x;
        bootVertices[vert].v = mtextures[mTriangle[j]->x]->y;
        bootVertices[vert].x = mPosition[mTriangle[j]->x]->x;
        bootVertices[vert].y = mPosition[mTriangle[j]->x]->y;
        bootVertices[vert].z = mPosition[mTriangle[j]->x]->z;
        vert++;

        bootVertices[vert].u = mtextures[mTriangle[j]->y]->x;
        bootVertices[vert].v = mtextures[mTriangle[j]->y]->y;
        bootVertices[vert].x = mPosition[mTriangle[j]->y]->x;
        bootVertices[vert].y = mPosition[mTriangle[j]->y]->y;
        bootVertices[vert].z = mPosition[mTriangle[j]->y]->z;
        vert++;

        bootVertices[vert].u = mtextures[mTriangle[j]->z]->x;
        bootVertices[vert].v = mtextures[mTriangle[j]->z]->y;
        bootVertices[vert].x = mPosition[mTriangle[j]->z]->x;
        bootVertices[vert].y = mPosition[mTriangle[j]->z]->y;
        bootVertices[vert].z = mPosition[mTriangle[j]->z]->z;
        vert++;
    }

    //clear the cache or there will be some errors
    sceKernelDcacheWritebackInvalidateRange(bootVertices,( mTriangle.size() * 3) * sizeof(TexturesPSPVertex));
    //sceKernelDcacheWritebackInvalidateAll();

    for(unsigned int aa = 0; aa < mPosition.size(); aa++)
    {
        delete mPosition[aa];
        delete mtextures[aa];
    }
    mPosition.clear();
    mtextures.clear();

    for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
    {
        delete 	mTriangle[aa];
    }
    mTriangle.clear();
}

void CraftWorld::CreateFullMeshChunk(const int StartX, const int StartY, const int StartZ)
{
    SimpleMeshChunk* OpaqueMeshChunk = new SimpleMeshChunk();
    SimpleMeshChunk* TransparentMeshChunk = new SimpleMeshChunk();

    int iOpaqueVertex = 0;
    int iTransparentVertex = 0;
    block_t Block;

    /* Only create visible faces of each chunk */
    OpaqueMeshChunk->chunkStartZ = StartZ;
    OpaqueMeshChunk->chunkStartY = StartY;
    OpaqueMeshChunk->chunkStartX = StartX;

    TransparentMeshChunk->chunkStartZ = StartZ;
    TransparentMeshChunk->chunkStartY = StartY;
    TransparentMeshChunk->chunkStartX = StartX;

    OpaqueMeshChunk->bBox = BoundingBox(Vector3(StartX,StartY,StartZ),Vector3(StartX + CHUNK_SIZE,StartY + CHUNK_SIZE,StartZ + CHUNK_SIZE));
    TransparentMeshChunk->bBox = BoundingBox(Vector3(StartX,StartY,StartZ),Vector3(StartX + CHUNK_SIZE,StartY + CHUNK_SIZE,StartZ + CHUNK_SIZE));

    //if we are in player zone on the start then build map
    if(playerZoneBB.intersect(OpaqueMeshChunk->bBox))
    {
        createdChunksCount++;
        //light
        for (int z = StartZ; z < CHUNK_SIZE + StartZ; ++z)
        {
            for (int y = StartY; y < CHUNK_SIZE + StartY; ++y)
            {
                for (int x = StartX; x < CHUNK_SIZE + StartX; ++x)
                {
                    Block = GetBlockNoCheck(x,y,z);
                    if(Block == 0) continue;

                    if(blockTypes[Block].transparent == true)
                    {
                        if(blockTypes[Block].blockModel == 0)
                        {
                            GetNormalBlock(x,y,z,iTransparentVertex,TransparentMeshChunk,Block,true);
                        }
                        else
                        {
                            GetSpecialBlock(x,y,z,iTransparentVertex,TransparentMeshChunk,Block,true);
                        }
                    }
                    else
                    {
                        if(blockTypes[Block].blockModel == 0)
                        {
                            GetNormalBlock(x,y,z,iOpaqueVertex,OpaqueMeshChunk,Block,false);
                        }
                        else
                        {
                            GetSpecialBlock(x,y,z,iOpaqueVertex,OpaqueMeshChunk,Block,false);
                        }
                    }
                }
            }
        }

        OpaqueMeshChunk->end();
        TransparentMeshChunk->end();
    }
    else
    {
        OpaqueMeshChunk->end();
        OpaqueMeshChunk->created = false;

        TransparentMeshChunk->end();
        TransparentMeshChunk->created = false;
    }

   // MeshChunk->id = mChunks.size();
    mChunks.push_back(OpaqueMeshChunk);
    mTransparentChunks.push_back(TransparentMeshChunk);
}

void CraftWorld::RebuildFullMeshChunk(int id)
{
    if(id < 0 || id >= mChunks.size())
    {
        return;
    }

    SimpleMeshChunk* OpaqueMeshChunk = mChunks[id];
    SimpleMeshChunk* TransparentMeshChunk = mTransparentChunks[id];

    int iOpaqueVertex = 0;
    int iTransparentVertex = 0;
    block_t Block;

    int StartZ = OpaqueMeshChunk->chunkStartZ;
    int StartY = OpaqueMeshChunk->chunkStartY;
    int StartX = OpaqueMeshChunk->chunkStartX;

    OpaqueMeshChunk->reset();
    TransparentMeshChunk->reset();

    for (int z = StartZ; z < CHUNK_SIZE + StartZ; ++z)
    {
        for (int y = StartY; y < CHUNK_SIZE + StartY; ++y)
        {
            for (int x = StartX; x < CHUNK_SIZE + StartX; ++x)
            {
                Block = GetBlockNoCheck(x,y,z);
                if(Block == 0)
                {
                    continue;
                }

                if(OpaqueMeshChunk->periodicallyUpadted == false)
                {
                    if((Block >= MelonPlant1::getID() && Block <= MelonPlant4::getID()) ||
                       (Block >= WheatBlock1::getID() && Block <= WheatBlock4::getID()) ||
                       (Block >= OakSapling::getID() && Block <= BirchSapling::getID()) ||
                       (Block >= PotatoPlant1::getID() && Block <= PotatoPlant4::getID()) ||
                       (Block >= CarrotPlant1::getID() && Block <= CarrotPlant4::getID()) ||
                       (Block >= PumpkinPlant1::getID() && Block <= PumpkinPlant4::getID()))
                    {
                        updatingChunks.push_back(id);
                        OpaqueMeshChunk->periodicallyUpadted = true;
                    }
                }

                if(blockTypes[Block].transparent == true)
                {
                    if(blockTypes[Block].blockModel == 0)
                    {
                        GetNormalBlock(x,y,z,iTransparentVertex,TransparentMeshChunk,Block,true);
                    }
                    else
                    {
                        GetSpecialBlock(x,y,z,iTransparentVertex,TransparentMeshChunk,Block,true);
                    }
                }
                else
                {
                    if(blockTypes[Block].blockModel == 0)
                    {
                        GetNormalBlock(x,y,z,iOpaqueVertex,OpaqueMeshChunk,Block,false);
                    }
                    else
                    {
                        GetSpecialBlock(x,y,z,iOpaqueVertex,OpaqueMeshChunk,Block,false);
                    }
                }
            }
        }
    }

    OpaqueMeshChunk->end();
    TransparentMeshChunk->end();
}

void CraftWorld::RebuildOpaqueMeshChunk(int id)
{
    if(id < 0 || id >= mChunks.size())
    {
        return;
    }

    SimpleMeshChunk* OpaqueMeshChunk = mChunks[id];

    int iOpaqueVertex = 0;
    block_t Block;

    int StartZ = OpaqueMeshChunk->chunkStartZ;
    int StartY = OpaqueMeshChunk->chunkStartY;
    int StartX = OpaqueMeshChunk->chunkStartX;

    OpaqueMeshChunk->reset();

    for (int z = StartZ; z < CHUNK_SIZE + StartZ; ++z)
    {
        for (int y = StartY; y < CHUNK_SIZE + StartY; ++y)
        {
            for (int x = StartX; x < CHUNK_SIZE + StartX; ++x)
            {
                Block = GetBlockNoCheck(x,y,z);
                if(Block == 0)continue;
                if(blockTypes[Block].transparent == true) continue;

                if(blockTypes[Block].blockModel == 0)
                {
                    GetNormalBlock(x,y,z,iOpaqueVertex,OpaqueMeshChunk,Block,false);
                }
                else
                {
                    GetSpecialBlock(x,y,z,iOpaqueVertex,OpaqueMeshChunk,Block,false);
                }
            }
        }
    }

    OpaqueMeshChunk->end();
}

void CraftWorld::RebuildTransparentMeshChunk(int id)
{
    if(id < 0 || id >= mChunks.size())
    {
        return;
    }

    SimpleMeshChunk* TransparentMeshChunk = mTransparentChunks[id];

    int iTransparentVertex = 0;
    block_t Block;

    int StartZ = TransparentMeshChunk->chunkStartZ;
    int StartY = TransparentMeshChunk->chunkStartY;
    int StartX = TransparentMeshChunk->chunkStartX;

    TransparentMeshChunk->reset();

    for (int z = StartZ; z < CHUNK_SIZE + StartZ; ++z)
    {
        for (int y = StartY; y < CHUNK_SIZE + StartY; ++y)
        {
            for (int x = StartX; x < CHUNK_SIZE + StartX; ++x)
            {
                Block = GetBlockNoCheck(x,y,z);
                if(Block == 0)continue;
                if(blockTypes[Block].transparent == false) continue;

                if(blockTypes[Block].blockModel == 0)
                {
                    GetNormalBlock(x,y,z,iTransparentVertex,TransparentMeshChunk,Block,true);
                }
                else
                {
                    GetSpecialBlock(x,y,z,iTransparentVertex,TransparentMeshChunk,Block,true);
                }
            }
        }
    }

    TransparentMeshChunk->end();
}

void CraftWorld::GetSpecialBlock(int x,int y, int z,int &iVertex,SimpleMeshChunk* MeshChunk,block_t Block,bool transparent)
{
    BaseBlock *blockType = &blockTypes[Block];
    float pixel, texturePixel;

    pixel = (float)1/16.0f;
    texturePixel = (float)1/256.0f;

    bool canCreate = false;
    bool DefaultBlock = false;
    bool transparentBlock = false;

    float up = 0.0f;
    float down = 0.0f;
    float left = 0.0f;
    float right = 0.0f;

    char Block1;

    Vector3 light1, light2, light3, light4;
    Vector3 BlockColorx1, BlockColorx2, BlockColory1, BlockColory2, BlockColorz1, BlockColorz2;

    ///left     right
    // x y z    x y z
    // 0 0 1    1 0 0
    // 0 1 1    1 1 0
    // 0 1 0    1 1 1
    // 0 0 0    1 0 1

    ///down     top
    // x y z    x y z
    // 0 0 0    0 1 1
    // 1 0 0    1 1 1
    // 1 0 1    1 1 0
    // 0 0 1    0 1 0

    ///back     forward
    // x y z    x y z
    // 0 1 0    0 0 1
    // 1 1 0    1 0 1
    // 1 0 0    1 1 1
    // 0 0 0    0 1 1

    if(blockType->blockModel == 5)
    {
        if(Block == WaterBlock::getID())
        {
            int upper_block = GetBlock(x,y+1,z);
            if(upper_block == Lava::getID() || upper_block == WaterBlock::getID() || upper_block == IceBlock::getID())
            {

                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,16,16),Vector3(0,16,0),Vector3(0,0,0));
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,16,0),Vector3(16,16,16),Vector3(16,0,16));

                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));

                /*if(GetBlock(x,y+1,z) != IceBlock::getID())
                {
                   BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,16,16),Vector3(16,16,16),Vector3(16,16,0),Vector3(0,16,0));
                }*/

                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,0),Vector3(16,16,0),Vector3(16,0,0),Vector3(0,0,0));
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,16,16),Vector3(0,16,16));
            }
            else
            {
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,14,16),Vector3(0,14,0),Vector3(0,0,0));
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,14,0),Vector3(16,14,16),Vector3(16,0,16));

                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,14,16),Vector3(16,14,16),Vector3(16,14,0),Vector3(0,14,0),1,0);

                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,14,0),Vector3(16,14,0),Vector3(16,0,0),Vector3(0,0,0));
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,14,16),Vector3(0,14,16));
            }
            return;
        }

        /// LEAVES
        if(Block == 9 || Block == 37 || Block == 38)
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,16,16),Vector3(0,16,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,16,0),Vector3(16,16,16),Vector3(16,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,16,16),Vector3(16,16,16),Vector3(16,16,0),Vector3(0,16,0));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,0),Vector3(16,16,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,16,16),Vector3(0,16,16));
            return;
        }


        /// SNOW LAYER
        if(Block == Snow2::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,2,16),Vector3(0,2,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,2,0),Vector3(16,2,16),Vector3(16,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,2,16),Vector3(16,2,16),Vector3(16,2,0),Vector3(0,2,0),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,2,0),Vector3(16,2,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,2,16),Vector3(0,2,16));

            return;
        }


        /// CACTI
        if(Block == 25)
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(1,0,16),Vector3(1,16,16),Vector3(1,16,0),Vector3(1,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(15,0,0),Vector3(15,16,0),Vector3(15,16,16),Vector3(15,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(1,0,1),Vector3(15,0,1),Vector3(15,0,15),Vector3(1,0,15));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(1,16,15),Vector3(15,16,15),Vector3(15,16,1),Vector3(1,16,1));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,1),Vector3(16,16,1),Vector3(16,0,1),Vector3(0,0,1));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,15),Vector3(16,0,15),Vector3(16,16,15),Vector3(0,16,15));
            return;
        }



        /// CARPETS
        if(Block >= BlackWoolCarpet::getID() && Block <= WhiteWoolCarpet::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,1,16),Vector3(0,1,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,1,0),Vector3(16,1,16),Vector3(16,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,1,16),Vector3(16,1,16),Vector3(16,1,0),Vector3(0,1,0),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,1,0),Vector3(16,1,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,1,16),Vector3(0,1,16));

            return;
        }

        /*if(Block == Cake::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(1,0,15),Vector3(1,8,15),Vector3(1,8,1),Vector3(1,0,1));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(15,0,1),Vector3(15,8,1),Vector3(15,8,15),Vector3(15,0,15));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(1,0,1),Vector3(15,0,1),Vector3(15,0,15),Vector3(1,0,15));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(1,8,15),Vector3(15,8,15),Vector3(15,8,1),Vector3(1,8,1));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(1,8,1),Vector3(15,8,1),Vector3(15,0,1),Vector3(1,0,1));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(1,0,15),Vector3(15,0,15),Vector3(15,8,15),Vector3(1,8,15));
        }*/

        if(Block == ItemFrame1::getID())
        {
            //BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,16,16),Vector3(0,16,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(1,2,2),Vector3(1,14,2),Vector3(1,14,14),Vector3(1,2,14),true,false,3);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(0.5f,2,2),Vector3(0.5f,14,2),Vector3(0.5f,14,14),Vector3(0.5f,2,14),true,false,2);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,2,2),Vector3(1,2,2),Vector3(1,2,14),Vector3(0,2,14),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,13,3),Vector3(1,13,3),Vector3(1,13,13),Vector3(0,13,13),true,false,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,14,14),Vector3(1,14,14),Vector3(1,14,2),Vector3(0,14,2),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,3,13),Vector3(1,3,13),Vector3(1,3,3),Vector3(0,3,3),true,false,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,14,2),Vector3(1,14,2),Vector3(1,2,2),Vector3(0,2,2),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,13,13),Vector3(1,13,13),Vector3(1,3,13),Vector3(0,3,13),true,false,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,2,14),Vector3(1,2,14),Vector3(1,14,14),Vector3(0,14,14),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,3,3),Vector3(1,3,3),Vector3(1,13,3),Vector3(0,13,3),true,false,0);

            return;
        }

        if(Block == ItemFrame2::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(15,2,14),Vector3(15,14,14),Vector3(15,14,2),Vector3(15,2,2),true,false,3);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(15.5f,2,14),Vector3(15.5f,14,14),Vector3(15.5f,14,2),Vector3(15.5f,2,2),true,false,2);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(15,2,2),Vector3(16,2,2),Vector3(15,2,14),Vector3(16,2,14),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(15,13,3),Vector3(16,13,3),Vector3(16,13,13),Vector3(15,13,13),true,false,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(15,14,14),Vector3(16,14,14),Vector3(16,14,2),Vector3(15,14,2),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(15,3,13),Vector3(16,3,13),Vector3(16,3,3),Vector3(15,3,3),true,false,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(15,14,2),Vector3(16,14,2),Vector3(16,2,2),Vector3(15,2,2),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(15,13,13),Vector3(16,13,13),Vector3(16,3,13),Vector3(15,3,13),true,false,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(15,2,14),Vector3(16,2,14),Vector3(16,14,14),Vector3(15,14,14),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(15,3,3),Vector3(16,3,3),Vector3(16,13,3),Vector3(15,13,3),true,false,0);

            return;
        }

        if(Block == ItemFrame3::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(2,2,1),Vector3(2,14,1),Vector3(2,14,0),Vector3(2,2,0),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(13,3,1),Vector3(13,13,1),Vector3(13,13,0),Vector3(13,3,0),true,false,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(14,2,0),Vector3(14,14,0),Vector3(14,14,1),Vector3(14,2,1),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(3,3,0),Vector3(3,13,0),Vector3(3,13,1),Vector3(3,3,1),true,false,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(2,2,0),Vector3(14,2,0),Vector3(14,2,1),Vector3(2,2,1),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(3,13,0),Vector3(13,13,0),Vector3(13,13,1),Vector3(3,13,1),true,false,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(2,14,1),Vector3(14,14,1),Vector3(14,14,0),Vector3(2,14,0),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(3,3,1),Vector3(13,3,1),Vector3(13,3,0),Vector3(3,3,0),true,false,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(2,2,1),Vector3(14,2,1),Vector3(14,14,1),Vector3(2,14,1),true,false,3);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(2,2,0.5f),Vector3(14,2,0.5f),Vector3(14,14,0.5f),Vector3(2,14,0.5f),true,false,2);

            return;
        }

        if(Block == ItemFrame4::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(2,2,16),Vector3(2,14,16),Vector3(2,14,15),Vector3(2,2,15),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(13,3,16),Vector3(13,13,16),Vector3(13,13,15),Vector3(13,3,15),true,false,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(14,2,15),Vector3(14,14,15),Vector3(14,14,16),Vector3(14,2,16),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(3,3,15),Vector3(3,13,15),Vector3(3,13,16),Vector3(3,3,16),true,false,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(2,2,15),Vector3(14,2,15),Vector3(14,2,16),Vector3(2,2,16),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(3,13,15),Vector3(13,13,15),Vector3(13,13,16),Vector3(3,13,16),true,false,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(2,14,16),Vector3(14,14,16),Vector3(14,14,15),Vector3(2,14,15),true,false,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(3,3,16),Vector3(13,3,16),Vector3(13,3,15),Vector3(3,3,15),true,false,0);


            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(2,14,15),Vector3(14,14,15),Vector3(14,2,15),Vector3(2,2,15),true,false,3);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(2,14,15.5f),Vector3(14,14,15.5f),Vector3(14,2,15.5f),Vector3(2,2,15.5f),true,false,2);

            return;
        }

        if(Block >= TrapdoorClosed1::getID() && Block <= TrapdoorClosed4::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,3,16),Vector3(0,3,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,3,0),Vector3(16,3,16),Vector3(16,0,16));

            if(BlockTransparentOrSpecial(x,y-1,z))
            {
                BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
            }
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,3,16),Vector3(16,3,16),Vector3(16,3,0),Vector3(0,3,0));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,3,0),Vector3(16,3,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,3,16),Vector3(0,3,16));
            return;
        }

        if(Block == TrapdoorOpen1::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,16,16),Vector3(0,16,0),Vector3(0,0,0));
            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(3,0,0),Vector3(3,16,0),Vector3(3,16,16),Vector3(3,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(3,0,0),Vector3(3,0,16),Vector3(0,0,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,16,16),Vector3(3,16,16),Vector3(3,16,0),Vector3(0,16,0));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,0),Vector3(3,16,0),Vector3(3,0,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(3,0,16),Vector3(3,16,16),Vector3(0,16,16));
            return;
        }

        if(Block == TrapdoorOpen2::getID())
        {
            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(13,0,16),Vector3(13,16,16),Vector3(13,16,0),Vector3(13,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,16,0),Vector3(16,16,16),Vector3(16,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(13,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(13,0,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(13,16,16),Vector3(16,16,16),Vector3(16,16,0),Vector3(13,16,0));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(13,16,0),Vector3(16,16,0),Vector3(16,0,0),Vector3(13,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(13,0,16),Vector3(16,0,16),Vector3(16,16,16),Vector3(13,16,16));
            return;
        }

        if(Block == TrapdoorOpen3::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,3),Vector3(0,16,3),Vector3(0,16,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,16,0),Vector3(16,16,3),Vector3(16,0,3));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,3),Vector3(0,0,3));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,16,3),Vector3(16,16,3),Vector3(16,16,0),Vector3(0,16,0));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,0),Vector3(16,16,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,3),Vector3(16,0,3),Vector3(16,16,3),Vector3(0,16,3));
            return;
        }

        if(Block == TrapdoorOpen4::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,16,16),Vector3(0,16,13),Vector3(0,0,13));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,13),Vector3(16,16,13),Vector3(16,16,16),Vector3(16,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,13),Vector3(16,0,13),Vector3(16,0,16),Vector3(0,0,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,16,16),Vector3(16,16,16),Vector3(16,16,13),Vector3(0,16,13));

            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,13),Vector3(16,16,13),Vector3(16,0,13),Vector3(0,0,13));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,16,16),Vector3(0,16,16));
            return;
        }

        if(Block == Ladder1::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(0.5f,0,0),Vector3(0.5f,16,0),Vector3(0.5f,16,16),Vector3(0.5f,0,16),1,1);
            return;
        }

        if(Block == Ladder2::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(15.5f,0,16),Vector3(15.5f,16,16),Vector3(15.5f,16,0),Vector3(15.5f,0,0),1,1);
            return;
        }

        if(Block == Ladder3::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,0.5f),Vector3(16,0,0.5f),Vector3(16,16,0.5f),Vector3(0,16,0.5f),1,1);
            return;
        }

        if(Block == Ladder4::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,15.5f),Vector3(16,16,15.5f),Vector3(16,0,15.5f),Vector3(0,0,15.5f),1,1);
            return;
        }

        if(Block == 112)
        {
            int upper_block = GetBlock(x,y+1,z);
            if(upper_block == WaterBlock::getID() || upper_block == Lava::getID())
            {
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,16,16),Vector3(0,16,0),Vector3(0,0,0));
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,16,0),Vector3(16,16,16),Vector3(16,0,16));

                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
               // BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,16,16),Vector3(16,16,16),Vector3(16,16,0),Vector3(0,16,0));

                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,0),Vector3(16,16,0),Vector3(16,0,0),Vector3(0,0,0));
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,16,16),Vector3(0,16,16));
            }
            else
            {
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,14,16),Vector3(0,14,0),Vector3(0,0,0));
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,14,0),Vector3(16,14,16),Vector3(16,0,16));

                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,14,16),Vector3(16,14,16),Vector3(16,14,0),Vector3(0,14,0));

                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,14,0),Vector3(16,14,0),Vector3(16,0,0),Vector3(0,0,0));
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,14,16),Vector3(0,14,16));
            }
            return;
        }

        /// MELON PLANTS
       /* if(Block >= MelonPlant1::getID() && Block <= MelonPlant3::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(8,0,16),Vector3(8,16,16),Vector3(8,16,0),Vector3(8,0,0),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(8,0,0),Vector3(8,16,0),Vector3(8,16,16),Vector3(8,0,16),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,8),Vector3(16,16,8),Vector3(16,0,8),Vector3(0,0,8),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,8),Vector3(16,0,8),Vector3(16,16,8),Vector3(0,16,8),1,0);
        } */
        if(Block == MelonPlant4::getID())
        {
            bool haveMelon = false;
            if(GetBlock(x+1,y,z) == MelonBlock::getID())
            {
                blockType = &blockTypes[Block];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,8),Vector3(16,16,8),Vector3(16,0,8),Vector3(0,0,8),1,1);
                blockType = &blockTypes[MelonPlant4Reverse::getID()];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,MelonPlant4Reverse::getID(),transparent,6,Vector3(0,0,8),Vector3(16,0,8),Vector3(16,16,8),Vector3(0,16,8),1,1);
                haveMelon = true;
            }
            if(GetBlock(x-1,y,z) == MelonBlock::getID() && haveMelon == false)
            {
                blockType = &blockTypes[MelonPlant4Reverse::getID()];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,MelonPlant4Reverse::getID(),transparent,5,Vector3(0,16,8),Vector3(16,16,8),Vector3(16,0,8),Vector3(0,0,8),1,1);
                blockType = &blockTypes[Block];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,8),Vector3(16,0,8),Vector3(16,16,8),Vector3(0,16,8),1,1);
                haveMelon = true;
            }
            if(GetBlock(x,y,z+1) == MelonBlock::getID() && haveMelon == false)
            {
                blockType = &blockTypes[MelonPlant4Reverse::getID()];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,MelonPlant4Reverse::getID(),transparent,1,Vector3(8,0,16),Vector3(8,16,16),Vector3(8,16,0),Vector3(8,0,0),1,1);
                blockType = &blockTypes[Block];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(8,0,0),Vector3(8,16,0),Vector3(8,16,16),Vector3(8,0,16),1,1);
                haveMelon = true;
            }
            if(GetBlock(x,y,z-1) == MelonBlock::getID() && haveMelon == false)
            {
                blockType = &blockTypes[Block];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(8,0,16),Vector3(8,16,16),Vector3(8,16,0),Vector3(8,0,0),1,1);
                blockType = &blockTypes[MelonPlant4Reverse::getID()];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,MelonPlant4Reverse::getID(),transparent,2,Vector3(8,0,0),Vector3(8,16,0),Vector3(8,16,16),Vector3(8,0,16),1,1);
                haveMelon = true;
            }

            if(haveMelon == false)
            {
                GetSpecialBlock(x,y,z,iVertex,MeshChunk,MelonPlant3::getID(),true);
            }
            else
            {
                GetSpecialBlock(x,y,z,iVertex,MeshChunk,MelonPlant2::getID(),true);
            }
        }

        if(Block == PumpkinPlant4::getID())
        {
            bool havePumpkin = false;
            if(GetBlock(x+1,y,z) == Pumpkin1::getID())
            {
                blockType = &blockTypes[Block];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,8),Vector3(16,16,8),Vector3(16,0,8),Vector3(0,0,8),1,1);
                blockType = &blockTypes[PumpkinPlant4Reverse::getID()];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,PumpkinPlant4Reverse::getID(),transparent,6,Vector3(0,0,8),Vector3(16,0,8),Vector3(16,16,8),Vector3(0,16,8),1,1);
                havePumpkin = true;
            }
            if(GetBlock(x-1,y,z) == Pumpkin1::getID() && havePumpkin == false)
            {
                blockType = &blockTypes[PumpkinPlant4Reverse::getID()];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,PumpkinPlant4Reverse::getID(),transparent,5,Vector3(0,16,8),Vector3(16,16,8),Vector3(16,0,8),Vector3(0,0,8),1,1);
                blockType = &blockTypes[Block];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,8),Vector3(16,0,8),Vector3(16,16,8),Vector3(0,16,8),1,1);
                havePumpkin = true;
            }
            if(GetBlock(x,y,z+1) == Pumpkin1::getID() && havePumpkin == false)
            {
                blockType = &blockTypes[PumpkinPlant4Reverse::getID()];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,PumpkinPlant4Reverse::getID(),transparent,1,Vector3(8,0,16),Vector3(8,16,16),Vector3(8,16,0),Vector3(8,0,0),1,1);
                blockType = &blockTypes[Block];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(8,0,0),Vector3(8,16,0),Vector3(8,16,16),Vector3(8,0,16),1,1);
                havePumpkin = true;
            }
            if(GetBlock(x,y,z-1) == Pumpkin1::getID() && havePumpkin == false)
            {
                blockType = &blockTypes[Block];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(8,0,16),Vector3(8,16,16),Vector3(8,16,0),Vector3(8,0,0),1,1);
                blockType = &blockTypes[PumpkinPlant4Reverse::getID()];
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,PumpkinPlant4Reverse::getID(),transparent,2,Vector3(8,0,0),Vector3(8,16,0),Vector3(8,16,16),Vector3(8,0,16),1,1);
                havePumpkin = true;
            }

            if(havePumpkin == false)
            {
                GetSpecialBlock(x,y,z,iVertex,MeshChunk,PumpkinPlant3::getID(),true);
            }
            else
            {
                GetSpecialBlock(x,y,z,iVertex,MeshChunk,PumpkinPlant2::getID(),true);
            }
        }

        /// DOOR 1 Z--
        if  (
            Block == 49 ||
            (Block == 56 && GetBlock(x,y,z-1) != 52 && GetBlock(x,y,z-1) != 56 && (GetBlock(x,y,z+1) == 56 || GetBlock(x,y,z+1) == 52)) ||
            (Block == 56 && GetBlock(x,y,z-1) != 52 && GetBlock(x,y,z-1) != 56 && GetBlock(x,y,z+1) != 52 && GetBlock(x,y,z+1) != 56) ||
            (Block == 55 && GetBlock(x,y,z-1) != 51 && GetBlock(x,y,z-1) != 55 && (GetBlock(x,y,z+1) == 55 || GetBlock(x,y,z+1) == 51))
            )
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,3),Vector3(0,32,3),Vector3(0,32,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,32,0),Vector3(16,32,3),Vector3(16,0,3));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,3),Vector3(0,0,3));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,32,3),Vector3(16,32,3),Vector3(16,32,0),Vector3(0,32,0));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,32,0),Vector3(16,32,0),Vector3(16,0,0),Vector3(0,0,0),0,1);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,3),Vector3(16,0,3),Vector3(16,32,3),Vector3(0,32,3),1,1);
            return;
        }

        /// DOOR Z++
        if  (
            Block == 50 ||
            (Block == 56 && GetBlock(x,y,z+1) != 52 && GetBlock(x,y,z+1) != 56 && (GetBlock(x,y,z-1) == 56 || GetBlock(x,y,z-1) == 52)) ||
            (Block == 55 && GetBlock(x,y,z+1) != 51 && GetBlock(x,y,z+1) != 55 && (GetBlock(x,y,z-1) == 55 || GetBlock(x,y,z-1) == 51)) ||
            (Block == 55 && GetBlock(x,y,z+1) != 51 && GetBlock(x,y,z+1) != 55 && GetBlock(x,y,z-1) != 51 && GetBlock(x,y,z-1) != 55)
            )
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,32,16),Vector3(0,32,13),Vector3(0,0,13));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,13),Vector3(16,32,13),Vector3(16,32,16),Vector3(16,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,13),Vector3(16,0,13),Vector3(16,0,16),Vector3(0,0,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,32,16),Vector3(16,32,16),Vector3(16,32,13),Vector3(0,32,13));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,32,13),Vector3(16,32,13),Vector3(16,0,13),Vector3(0,0,13),1,1);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,32,16),Vector3(0,32,16),0,1);
            return;
        }

        /// DOOR X--
        if  (
            Block == 51 ||
            (Block == 53 && GetBlock(x-1,y,z) != 49 && GetBlock(x-1,y,z) != 53 && (GetBlock(x+1,y,z) == 53 || GetBlock(x+1,y,z) == 49)) ||
            (Block == 53 && GetBlock(x-1,y,z) != 49 && GetBlock(x-1,y,z) != 53 && GetBlock(x+1,y,z) != 49 && GetBlock(x+1,y,z) != 53) ||
            (Block == 54 && GetBlock(x-1,y,z) != 50 && GetBlock(x-1,y,z) != 54 && (GetBlock(x+1,y,z) == 54 || GetBlock(x+1,y,z) == 50))
            )
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,32,16),Vector3(0,32,0),Vector3(0,0,0),0,1);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(3,0,0),Vector3(3,32,0),Vector3(3,32,16),Vector3(3,0,16),1,1);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(3,0,0),Vector3(3,0,16),Vector3(0,0,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,32,16),Vector3(3,32,16),Vector3(3,32,0),Vector3(0,32,0));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,32,0),Vector3(3,32,0),Vector3(3,0,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(3,0,16),Vector3(3,32,16),Vector3(0,32,16));
            return;
        }

        /// DOOR X++
        if  (
            Block == 52 ||
            (Block == 53 && GetBlock(x+1,y,z) != 49 && GetBlock(x+1,y,z) != 53 && (GetBlock(x-1,y,z) == 53 || GetBlock(x-1,y,z) == 49)) ||
            (Block == 54 && GetBlock(x+1,y,z) != 50 && GetBlock(x+1,y,z) != 54 && (GetBlock(x-1,y,z) == 54 || GetBlock(x-1,y,z) == 50)) ||
            (Block == 54 && GetBlock(x+1,y,z) != 50 && GetBlock(x+1,y,z) != 54 && GetBlock(x-1,y,z) != 50 && GetBlock(x-1,y,z) != 54)
            )
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(13,0,16),Vector3(13,32,16),Vector3(13,32,0),Vector3(13,0,0),1,1);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,32,0),Vector3(16,32,16),Vector3(16,0,16),0,1);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(13,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(13,0,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(13,32,16),Vector3(16,32,16),Vector3(16,32,0),Vector3(13,32,0));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(13,32,0),Vector3(16,32,0),Vector3(16,0,0),Vector3(13,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(13,0,16),Vector3(16,0,16),Vector3(16,32,16),Vector3(13,32,16));
            return;
        }

        if(Block == 55 || Block == 56)
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,3),Vector3(0,32,3),Vector3(0,32,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,32,0),Vector3(16,32,3),Vector3(16,0,3));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,3),Vector3(0,0,3));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,32,3),Vector3(16,32,3),Vector3(16,32,0),Vector3(0,32,0));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,32,0),Vector3(16,32,0),Vector3(16,0,0),Vector3(0,0,0),0,1);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,3),Vector3(16,0,3),Vector3(16,32,3),Vector3(0,32,3),1,1);
            return;
        }

        if(Block == 53 || Block == 54)
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,32,16),Vector3(0,32,0),Vector3(0,0,0),0,1);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(3,0,0),Vector3(3,32,0),Vector3(3,32,16),Vector3(3,0,16),1,1);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(3,0,0),Vector3(3,0,16),Vector3(0,0,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,32,16),Vector3(3,32,16),Vector3(3,32,0),Vector3(0,32,0));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,32,0),Vector3(3,32,0),Vector3(3,0,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(3,0,16),Vector3(3,32,16),Vector3(0,32,16));
            return;
        }

        /// CHEST
        if(Block == 94)
        {
            // chest
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(1,0,15),Vector3(1,14,15),Vector3(1,14,1),Vector3(1,0,1),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(15,0,1),Vector3(15,14,1),Vector3(15,14,15),Vector3(15,0,15),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(1,0,1),Vector3(15,0,1),Vector3(15,0,15),Vector3(1,0,15),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(1,14,15),Vector3(15,14,15),Vector3(15,14,1),Vector3(1,14,1),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(1,14,1),Vector3(15,14,1),Vector3(15,0,1),Vector3(1,0,1),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(1,0,15),Vector3(15,0,15),Vector3(15,14,15),Vector3(1,14,15),1,0);

            // chest lock
            blockType = &blockTypes[98];
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,1,Vector3(0,7,9),Vector3(0,11,9),Vector3(0,11,7),Vector3(0,7,7),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,3,Vector3(0,7,7),Vector3(1,7,7),Vector3(1,7,9),Vector3(0,7,9),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,4,Vector3(0,11,9),Vector3(1,11,9),Vector3(1,11,7),Vector3(0,11,7),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,5,Vector3(0,11,7),Vector3(1,11,7),Vector3(1,7,7),Vector3(0,7,7),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,6,Vector3(0,7,9),Vector3(1,7,9),Vector3(1,11,9),Vector3(0,11,9),1,0);
            return;
        }

        if(Block == 95)
        {
            // chest
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(1,0,15),Vector3(1,14,15),Vector3(1,14,1),Vector3(1,0,1),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(15,0,1),Vector3(15,14,1),Vector3(15,14,15),Vector3(15,0,15),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(1,0,1),Vector3(15,0,1),Vector3(15,0,15),Vector3(1,0,15),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(1,14,15),Vector3(15,14,15),Vector3(15,14,1),Vector3(1,14,1),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(1,14,1),Vector3(15,14,1),Vector3(15,0,1),Vector3(1,0,1),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(1,0,15),Vector3(15,0,15),Vector3(15,14,15),Vector3(1,14,15),1,0);

            // chest lock
            blockType = &blockTypes[98];
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,2,Vector3(16,7,7),Vector3(16,11,7),Vector3(16,11,9),Vector3(16,7,9),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,3,Vector3(15,7,7),Vector3(16,7,7),Vector3(16,7,9),Vector3(15,7,9),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,4,Vector3(15,11,9),Vector3(16,11,9),Vector3(16,11,7),Vector3(15,11,7),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,5,Vector3(15,11,7),Vector3(16,11,7),Vector3(16,7,7),Vector3(15,7,7),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,6,Vector3(15,7,9),Vector3(16,7,9),Vector3(16,11,9),Vector3(15,11,9),1,0);
            return;
        }

        if(Block == 96)
        {
            // chest
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(1,0,15),Vector3(1,14,15),Vector3(1,14,1),Vector3(1,0,1),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(15,0,1),Vector3(15,14,1),Vector3(15,14,15),Vector3(15,0,15),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(1,0,1),Vector3(15,0,1),Vector3(15,0,15),Vector3(1,0,15),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(1,14,15),Vector3(15,14,15),Vector3(15,14,1),Vector3(1,14,1),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(1,14,1),Vector3(15,14,1),Vector3(15,0,1),Vector3(1,0,1),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(1,0,15),Vector3(15,0,15),Vector3(15,14,15),Vector3(1,14,15),1,0);

            // chest lock
            blockType = &blockTypes[98];
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,1,Vector3(7,7,1),Vector3(7,11,1),Vector3(7,11,0),Vector3(7,7,0),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,2,Vector3(9,7,0),Vector3(9,11,0),Vector3(9,11,1),Vector3(9,7,1),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,3,Vector3(7,7,0),Vector3(9,7,0),Vector3(9,7,1),Vector3(7,7,1),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,4,Vector3(7,11,1),Vector3(9,11,1),Vector3(9,11,0),Vector3(7,11,0),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,5,Vector3(7,11,0),Vector3(9,11,0),Vector3(9,7,0),Vector3(7,7,0),1,0);
            return;
        }

        if(Block == 97)
        {
            // chest
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(1,0,15),Vector3(1,14,15),Vector3(1,14,1),Vector3(1,0,1),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(15,0,1),Vector3(15,14,1),Vector3(15,14,15),Vector3(15,0,15),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(1,0,1),Vector3(15,0,1),Vector3(15,0,15),Vector3(1,0,15),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(1,14,15),Vector3(15,14,15),Vector3(15,14,1),Vector3(1,14,1),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(1,14,1),Vector3(15,14,1),Vector3(15,0,1),Vector3(1,0,1),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(1,0,15),Vector3(15,0,15),Vector3(15,14,15),Vector3(1,14,15),1,0);

            // chest lock
            blockType = &blockTypes[98];
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,1,Vector3(7,7,16),Vector3(7,11,16),Vector3(7,11,15),Vector3(7,7,15),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,2,Vector3(9,7,15),Vector3(9,11,15),Vector3(9,11,16),Vector3(9,7,16),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,3,Vector3(7,7,15),Vector3(9,7,15),Vector3(9,7,16),Vector3(7,7,16),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,4,Vector3(7,11,16),Vector3(9,11,16),Vector3(9,11,15),Vector3(7,11,15),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,66,transparent,6,Vector3(7,7,16),Vector3(9,7,16),Vector3(9,11,16),Vector3(7,11,16),1,0);
            return;
        }

        /// TORCH
        if(Block == Torch::getID())
        {
            //texture stuff
            float up = percent*blockType->leftPlane_y + texturePixel*5;
            float down = percent*blockType->leftPlane_y + percent;

            float left = percent * blockType->leftPlane_x + texturePixel*6;
            float right = left + texturePixel*4;

            //standing torch
            block_t Block1 = GetBlock(x,y-1,z);
            pixel = 1.0f/16.0f;

            if(BlockSolid(x,y-1,z)) /////
            {
                //x-1
                MeshChunk->vert(x + 0.4375f,y			,z + 0.4375f + 0.0625*2+pixel	,right	, down,1,1,1);
                MeshChunk->vert(x + 0.4375f,y + 0.0625*11,z + 0.4375f + 0.0625*2+pixel	,right	, up	,1,1,1);
                MeshChunk->vert(x + 0.4375f,y + 0.0625*11,z + 0.4375f-pixel			,left	, up	,1,1,1);
                MeshChunk->vert(x + 0.4375f,y			,z + 0.4375f-pixel			,left	, down	,1,1,1);

                MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                iVertex += 4;

                //x+1
                MeshChunk->vert(x + 0.4375f + pixel*2,y		        ,z + 0.4375f-pixel            ,right, down,1,1,1);
                MeshChunk->vert(x + 0.4375f + pixel*2,y + pixel*11  ,z + 0.4375f-pixel            ,right, up,1,1,1);
                MeshChunk->vert(x + 0.4375f + pixel*2,y + pixel*11  ,z + 0.4375f + pixel*2+pixel  ,left, up,1,1,1);
                MeshChunk->vert(x + 0.4375f + pixel*2,y		        ,z + 0.4375f + pixel*2+pixel  ,left, down,1,1,1);

                MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                iVertex += 4;

                //z-1
                MeshChunk->vert(x + 0.4375f-pixel		        ,y + pixel*11   ,z + 0.4375f,right, up,1,1,1);
                MeshChunk->vert(x + 0.4375f + pixel*2+pixel   ,y + pixel*11   ,z + 0.4375f,left, up,1,1,1);
                MeshChunk->vert(x + 0.4375f + pixel*2+pixel   ,y			    ,z + 0.4375f,left, down,1,1,1);
                MeshChunk->vert(x + 0.4375f-pixel		        ,y			    ,z + 0.4375f,right, down,1,1,1);

                MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                iVertex += 4;

                //z+1
                MeshChunk->vert(x + 0.4375f-pixel		        ,y			    ,z + 0.4375f + pixel*2,left, down,1,1,1);
                MeshChunk->vert(x + 0.4375f + pixel*2+pixel   ,y			    ,z + 0.4375f + pixel*2,right, down,1,1,1);
                MeshChunk->vert(x + 0.4375f + pixel*2+pixel   ,y + pixel*11	    ,z + 0.4375f + pixel*2,right, up,1,1,1);
                MeshChunk->vert(x + 0.4375f-pixel		        ,y + pixel*11	,z + 0.4375f + pixel*2,left, up,1,1,1);

                MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                iVertex += 4;

                //y+1
                {
                    up = percent*blockType->leftPlane_y + texturePixel*6;
                    down = up + texturePixel*2;

                    left = percent * blockType->leftPlane_x + texturePixel*7;
                    right = left + texturePixel*2;

                    MeshChunk->vert(x + 0.4375f		        ,y + pixel*10   ,z + 0.4375f + pixel*2  ,left, up,1,1,1);
                    MeshChunk->vert(x + 0.4375f + pixel*2   ,y + pixel*10   ,z + 0.4375f + pixel*2  ,right, up,1,1,1);
                    MeshChunk->vert(x + 0.4375f + pixel*2   ,y + pixel*10   ,z + 0.4375f            ,right, down,1,1,1);
                    MeshChunk->vert(x + 0.4375f		        ,y + pixel*10   ,z + 0.4375f            ,left, down,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;
                }
            }
            else
            {

                bool canPlaseTorch = true;
                if(BlockSolid(x-1,y,z) && canPlaseTorch)
                {

                    //x-1
                    MeshChunk->vert(x - pixel   ,y + pixel*3    ,z + 0.4375f + pixel*2+pixel	,right	, down	,1,1,1);
                    MeshChunk->vert(x + pixel*4 ,y + pixel*14   ,z + 0.4375f + pixel*2+pixel	,right	, up	,1,1,1);
                    MeshChunk->vert(x + pixel*4 ,y + pixel*14   ,z + 0.4375f-pixel			,left	, up	,1,1,1);
                    MeshChunk->vert(x - pixel   ,y + pixel*3    ,z + 0.4375f-pixel			,left	, down	,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    //x+1
                    MeshChunk->vert(x + pixel    ,y	+ pixel*3	,z + 0.4375f-pixel            ,right, down,1,1,1);
                    MeshChunk->vert(x + pixel*6  ,y + pixel*14  ,z + 0.4375f-pixel            ,right, up  ,1,1,1);
                    MeshChunk->vert(x + pixel*6  ,y + pixel*14  ,z + 0.4375f + pixel*2+pixel  ,left, up   ,1,1,1);
                    MeshChunk->vert(x + pixel    ,y + pixel*3   ,z + 0.4375f + pixel*2+pixel  ,left, down ,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    //z-1
                    MeshChunk->vert(x + pixel*4-pixel   ,y + pixel*14   ,z + pixel*7,right, up  ,1,1,1);
                    MeshChunk->vert(x + pixel*6+pixel   ,y + pixel*14   ,z + pixel*7,left, up   ,1,1,1);
                    MeshChunk->vert(x + pixel+pixel     ,y + pixel*3	,z + pixel*7,left, down ,1,1,1);
                    MeshChunk->vert(x - pixel-pixel	    ,y + pixel*3	,z + pixel*7,right, down,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    //z+1
                    MeshChunk->vert(x - pixel-pixel	        ,y + pixel*3    ,z + pixel*9,left, down,1,1,1);
                    MeshChunk->vert(x + pixel+pixel           ,y + pixel*3	,z + pixel*9,right, down,1,1,1);
                    MeshChunk->vert(x + pixel*6+pixel         ,y + pixel*14	,z + pixel*9,right, up,1,1,1);
                    MeshChunk->vert(x + pixel*4-pixel		    ,y + pixel*14	,z + pixel*9,left, up,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;


                    //y+1
                    up = percent*blockType->leftPlane_y + texturePixel*6;
                    down = up + texturePixel*2;

                    left = percent * blockType->leftPlane_x + texturePixel*7;
                    right = left + texturePixel*2;

                    MeshChunk->vert(x + pixel*4 - pixel/2.0f		    ,y + pixel*13   ,z + pixel*7 + pixel*2  ,left, up,1,1,1);
                    MeshChunk->vert(x + pixel*4 + pixel*2 - pixel/2.0f  ,y + pixel*13   ,z + pixel*7 + pixel*2  ,right, up,1,1,1);
                    MeshChunk->vert(x + pixel*4 + pixel*2 - pixel/2.0f  ,y + pixel*13   ,z + pixel*7            ,right, down,1,1,1);
                    MeshChunk->vert(x + pixel*4	- pixel/2.0f	        ,y + pixel*13   ,z + pixel*7            ,left, down,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;


                    //y-1
                    up = percent*blockType->leftPlane_y + texturePixel*14;
                    down = up + texturePixel*2;

                    left = percent * blockType->leftPlane_x + texturePixel*7;
                    right = left + texturePixel*2;

                    MeshChunk->vert(x - pixel   ,y + pixel*3   ,z + pixel*7            ,left, up,1,1,1);
                    MeshChunk->vert(x + pixel   ,y + pixel*3   ,z + pixel*7            ,right, up,1,1,1);
                    MeshChunk->vert(x + pixel   ,y + pixel*3   ,z + pixel*7 + pixel*2  ,right, down,1,1,1);
                    MeshChunk->vert(x - pixel	,y + pixel*3   ,z + pixel*7 + pixel*2  ,left, down,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    canPlaseTorch = false;
                }

                if(BlockSolid(x+1,y,z) && canPlaseTorch)
                {
                    //x-1
                    MeshChunk->vert(x + pixel*15   ,y + pixel*3    ,z + 0.4375f + pixel*2+pixel	,right	, down	,1,1,1);
                    MeshChunk->vert(x + pixel*10 ,y + pixel*14   ,z + 0.4375f + pixel*2+pixel	,right	, up	,1,1,1);
                    MeshChunk->vert(x + pixel*10 ,y + pixel*14   ,z + 0.4375f-pixel			,left	, up	,1,1,1);
                    MeshChunk->vert(x + pixel*15  ,y + pixel*3    ,z + 0.4375f-pixel			,left	, down	,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    //x+1
                    MeshChunk->vert(x + pixel*17    ,y	+ pixel*3	,z + 0.4375f-pixel            ,right, down,1,1,1);
                    MeshChunk->vert(x + pixel*12  ,y + pixel*14  ,z + 0.4375f-pixel            ,right, up  ,1,1,1);
                    MeshChunk->vert(x + pixel*12  ,y + pixel*14  ,z + 0.4375f + pixel*2+pixel  ,left, up   ,1,1,1);
                    MeshChunk->vert(x + pixel*17    ,y + pixel*3   ,z + 0.4375f + pixel*2+pixel  ,left, down ,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    //z-1
                    MeshChunk->vert(x + pixel*12+pixel		,y + pixel*14  ,z + pixel*9,right, up  ,1,1,1);
                    MeshChunk->vert(x + pixel*10-pixel     ,y + pixel*14   ,z + pixel*9,left, up   ,1,1,1);
                    MeshChunk->vert(x + pixel*15-pixel       ,y + pixel*3	,z + pixel*9,left, down ,1,1,1);
                    MeshChunk->vert(x + pixel*17+pixel	    ,y + pixel*3	,z + pixel*9,right, down,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    //z+1
                    MeshChunk->vert(x + pixel*17+pixel	        ,y + pixel*3    ,z + pixel*7,left, down,1,1,1);
                    MeshChunk->vert(x + pixel*15-pixel           ,y + pixel*3	,z + pixel*7,right, down,1,1,1);
                    MeshChunk->vert(x + pixel*10-pixel         ,y + pixel*14	,z + pixel*7,right, up,1,1,1);
                    MeshChunk->vert(x + pixel*12+pixel		    ,y + pixel*14	,z + pixel*7,left, up,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    {
                        up = percent*blockType->leftPlane_y + texturePixel*6;
                        down = up + texturePixel*2;

                        left = percent * blockType->leftPlane_x + texturePixel*7;
                        right = left + texturePixel*2;

                        MeshChunk->vert(x + pixel*10 + pixel/2.0f	,y + pixel*13   ,z + pixel*7 + pixel*2  ,left, up,1,1,1);
                        MeshChunk->vert(x + pixel*12 + pixel/2.0f   ,y + pixel*13   ,z + pixel*7 + pixel*2  ,right, up,1,1,1);
                        MeshChunk->vert(x + pixel*12 + pixel/2.0f   ,y + pixel*13   ,z + pixel*7            ,right, down,1,1,1);
                        MeshChunk->vert(x + pixel*10 + pixel/2.0f   ,y + pixel*13   ,z + pixel*7            ,left, down,1,1,1);

                        MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                        MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                        iVertex += 4;
                    }

                    {
                        up = percent*blockType->leftPlane_y + texturePixel*14;
                        down = up + texturePixel*2;

                        left = percent * blockType->leftPlane_x + texturePixel*7;
                        right = left + texturePixel*2;

                        MeshChunk->vert(x + pixel * 15  ,y + pixel*3   ,z + pixel*7            ,left, up,1,1,1);
                        MeshChunk->vert(x + pixel * 17  ,y + pixel*3   ,z + pixel*7            ,right, up,1,1,1);
                        MeshChunk->vert(x + pixel * 17  ,y + pixel*3   ,z + pixel*7 + pixel*2  ,right, down,1,1,1);
                        MeshChunk->vert(x + pixel * 15 	,y + pixel*3   ,z + pixel*7 + pixel*2  ,left, down,1,1,1);

                        MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                        MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                        iVertex += 4;
                    }
                    canPlaseTorch = false;
                }

                if(BlockSolid(x,y,z-1) && canPlaseTorch)
                {
                    //x-1
                    MeshChunk->vert(x + pixel*9,y + pixel*3		,z - pixel-pixel	    ,right	, down	,1,1,1);
                    MeshChunk->vert(x + pixel*9,y + pixel*14    ,z + pixel*4-pixel	,right	, up	,1,1,1);
                    MeshChunk->vert(x + pixel*9,y + pixel*14    ,z + pixel*6+pixel	,left	, up	,1,1,1);
                    MeshChunk->vert(x + pixel*9,y + pixel*3		,z + pixel+pixel	    ,left	, down	,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    //x+1
                    MeshChunk->vert(x + pixel*7,y + pixel*3     ,z + pixel+pixel          ,right, down,1,1,1);
                    MeshChunk->vert(x + pixel*7,y + pixel*14    ,z + pixel*6+pixel           ,right, up,1,1,1);
                    MeshChunk->vert(x + pixel*7,y + pixel*14    ,z + pixel*4-pixel  ,left, up,1,1,1);
                    MeshChunk->vert(x + pixel*7,y + pixel*3	    ,z - pixel-pixel ,left, down,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    //z-1
                    MeshChunk->vert(x + pixel*7	- pixel	        ,y + pixel*14   ,z + pixel*4,right, up,1,1,1);
                    MeshChunk->vert(x + pixel*7 + pixel*2 + pixel    ,y + pixel*14   ,z + pixel*4,left, up,1,1,1);
                    MeshChunk->vert(x + pixel*7 + pixel*2 + pixel   ,y + pixel*3    ,z - pixel,left, down,1,1,1);
                    MeshChunk->vert(x + pixel*7	- pixel	        ,y + pixel*3    ,z - pixel,right, down,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    //z+1
                    MeshChunk->vert(x + pixel*7	-pixel	        ,y + pixel*3    ,z + pixel,left, down,1,1,1);
                    MeshChunk->vert(x + pixel*7 + pixel*2+pixel   ,y + pixel*3	,z + pixel,right, down,1,1,1);
                    MeshChunk->vert(x + pixel*7 + pixel*2+pixel  ,y + pixel*14	,z + pixel*6,right, up,1,1,1);
                    MeshChunk->vert(x + pixel*7	-pixel	        ,y + pixel*14	,z + pixel*6,left, up,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    //y+1
                    {
                        up = percent*blockType->leftPlane_y + texturePixel*6;
                        down = up + texturePixel*2;

                        left = percent * blockType->leftPlane_x + texturePixel*7;
                        right = left + texturePixel*2;

                        MeshChunk->vert(x + pixel*7		        ,y + pixel*13   ,z + pixel*6 - pixel/2.0f  ,left, up,1,1,1);
                        MeshChunk->vert(x + pixel*7 + pixel*2   ,y + pixel*13   ,z + pixel*6 - pixel/2.0f ,right, up,1,1,1);
                        MeshChunk->vert(x + pixel*7 + pixel*2   ,y + pixel*13   ,z + pixel*4 - pixel/2.0f           ,right, down,1,1,1);
                        MeshChunk->vert(x + pixel*7		        ,y + pixel*13   ,z + pixel*4 - pixel/2.0f         ,left, down,1,1,1);

                        MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                        MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                        iVertex += 4;
                    }

                    //y-1
                    {
                        up = percent*blockType->leftPlane_y + texturePixel*14;
                        down = up + texturePixel*2;

                        left = percent * blockType->leftPlane_x + texturePixel*7;
                        right = left + texturePixel*2;

                        MeshChunk->vert(x + pixel*7		        ,y + pixel*3   ,z + pixel  ,left, up,1,1,1);
                        MeshChunk->vert(x + pixel*7             ,y + pixel*3   ,z - pixel  ,right, up,1,1,1);
                        MeshChunk->vert(x + pixel*9             ,y + pixel*3   ,z - pixel  ,right, down,1,1,1);
                        MeshChunk->vert(x + pixel*9		        ,y + pixel*3   ,z + pixel  ,left, down,1,1,1);

                        MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                        MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                        iVertex += 4;
                    }
                    canPlaseTorch = false;
                }

                if(BlockSolid(x,y,z+1) && canPlaseTorch)
                {

                    //x-1
                    MeshChunk->vert(x + pixel*9,y + pixel*3		,z + pixel*15-pixel	    ,right	, down	,1,1,1);
                    MeshChunk->vert(x + pixel*9,y + pixel*14    ,z + pixel*10-pixel	,right	, up	,1,1,1);
                    MeshChunk->vert(x + pixel*9,y + pixel*14    ,z + pixel*12+pixel	,left	, up	,1,1,1);
                    MeshChunk->vert(x + pixel*9,y + pixel*3		,z + pixel*17+pixel	    ,left	, down	,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    //x+1
                    MeshChunk->vert(x + pixel*7,y + pixel*3     ,z + pixel*17+pixel         ,right, down,1,1,1);
                    MeshChunk->vert(x + pixel*7,y + pixel*14    ,z + pixel*12+pixel           ,right, up,1,1,1);
                    MeshChunk->vert(x + pixel*7,y + pixel*14    ,z + pixel*10-pixel  ,left, up,1,1,1);
                    MeshChunk->vert(x + pixel*7,y + pixel*3	    ,z + pixel*15-pixel ,left, down,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    //z-1
                    MeshChunk->vert(x + pixel*7-pixel		        ,y + pixel*14   ,z + pixel*10,right, up,1,1,1);
                    MeshChunk->vert(x + pixel*7 + pixel*2+pixel  ,y + pixel*14   ,z + pixel*10,left, up,1,1,1);
                    MeshChunk->vert(x + pixel*7 + pixel*2+pixel   ,y + pixel*3    ,z + pixel*15,left, down,1,1,1);
                    MeshChunk->vert(x + pixel*7-pixel		        ,y + pixel*3    ,z + pixel*15,right, down,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    //z+1
                    MeshChunk->vert(x + pixel*7-pixel		        ,y + pixel*3    ,z + pixel*17,left, down,1,1,1);
                    MeshChunk->vert(x + pixel*7 + pixel*2+pixel   ,y + pixel*3	,z + pixel*17,right, down,1,1,1);
                    MeshChunk->vert(x + pixel*7 + pixel*2+pixel   ,y + pixel*14	,z + pixel*12,right, up,1,1,1);
                    MeshChunk->vert(x + pixel*7-pixel		        ,y + pixel*14	,z + pixel*12,left, up,1,1,1);

                    MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                    MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                    iVertex += 4;

                    //y+1
                    {
                        up = percent*blockType->leftPlane_y + texturePixel*6;
                        down = up + texturePixel*2;

                        left = percent * blockType->leftPlane_x + texturePixel*7;
                        right = left + texturePixel*2;

                        MeshChunk->vert(x + pixel*7		        ,y + pixel*13   ,z + pixel*12 + pixel/2.0f ,left, up,1,1,1);
                        MeshChunk->vert(x + pixel*7 + pixel*2   ,y + pixel*13   ,z + pixel*12 + pixel/2.0f ,right, up,1,1,1);
                        MeshChunk->vert(x + pixel*7 + pixel*2   ,y + pixel*13   ,z + pixel*10 + pixel/2.0f ,right, down,1,1,1);
                        MeshChunk->vert(x + pixel*7		        ,y + pixel*13   ,z + pixel*10 + pixel/2.0f ,left, down,1,1,1);

                        MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                        MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                        iVertex += 4;
                    }

                            //y+1
                    {
                        up = percent*blockType->leftPlane_y + texturePixel*14;
                        down = up + texturePixel*2;

                        left = percent * blockType->leftPlane_x + texturePixel*7;
                        right = left + texturePixel*2;

                        MeshChunk->vert(x + pixel*7		        ,y + pixel*3   ,z + pixel *17  ,left, up,1,1,1);
                        MeshChunk->vert(x + pixel*7             ,y + pixel*3   ,z + pixel *15 ,right, up,1,1,1);
                        MeshChunk->vert(x + pixel*9             ,y + pixel*3   ,z + pixel *15 ,right, down,1,1,1);
                        MeshChunk->vert(x + pixel*9		        ,y + pixel*3   ,z + pixel *17 ,left, down,1,1,1);

                        MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
                        MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

                        iVertex += 4;
                    }
                }
            }
            return;
        }
    }

    /// HALF BLOCKS
    if(blockType->blockModel == 1)
    {
        // shitty part
        if(Block == BedBlock1::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,9,16),Vector3(0,9,0),Vector3(0,0,0));
            //BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,8,0),Vector3(16,8,16),Vector3(16,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,3,0),Vector3(16,3,0),Vector3(16,3,16),Vector3(0,3,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,9,16),Vector3(16,9,16),Vector3(16,9,0),Vector3(0,9,0),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,9,0),Vector3(16,9,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,9,16),Vector3(0,9,16));
            return;
        }
        if(Block == BedBlock5::getID())
        {
            //BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,9,16),Vector3(0,9,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,9,0),Vector3(16,9,16),Vector3(16,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,3,0),Vector3(16,3,0),Vector3(16,3,16),Vector3(0,3,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,9,16),Vector3(16,9,16),Vector3(16,9,0),Vector3(0,9,0),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,9,0),Vector3(16,9,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,9,16),Vector3(0,9,16));
            return;
        }

        if(Block == BedBlock2::getID())
        {
            //BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,9,16),Vector3(0,9,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,9,0),Vector3(16,9,16),Vector3(16,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,3,0),Vector3(16,3,0),Vector3(16,3,16),Vector3(0,3,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,9,16),Vector3(16,9,16),Vector3(16,9,0),Vector3(0,9,0),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,9,0),Vector3(16,9,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,9,16),Vector3(0,9,16));
            return;
        }
        if(Block == BedBlock6::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,9,16),Vector3(0,9,0),Vector3(0,0,0));
            //BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,9,0),Vector3(16,9,16),Vector3(16,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,3,0),Vector3(16,3,0),Vector3(16,3,16),Vector3(0,3,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,9,16),Vector3(16,9,16),Vector3(16,9,0),Vector3(0,9,0),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,9,0),Vector3(16,9,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,9,16),Vector3(0,9,16));
            return;
        }

        if(Block == BedBlock3::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,9,16),Vector3(0,9,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,9,0),Vector3(16,9,16),Vector3(16,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,3,0),Vector3(16,3,0),Vector3(16,3,16),Vector3(0,3,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,9,16),Vector3(16,9,16),Vector3(16,9,0),Vector3(0,9,0),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,9,0),Vector3(16,9,0),Vector3(16,0,0),Vector3(0,0,0));
            //BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,9,16),Vector3(0,9,16));
            return;
        }
        if(Block == BedBlock7::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,9,16),Vector3(0,9,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,9,0),Vector3(16,9,16),Vector3(16,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,3,0),Vector3(16,3,0),Vector3(16,3,16),Vector3(0,3,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,9,16),Vector3(16,9,16),Vector3(16,9,0),Vector3(0,9,0),1,0);

            //BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,9,0),Vector3(16,9,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,9,16),Vector3(0,9,16));
            return;
        }

        if(Block == BedBlock4::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,9,16),Vector3(0,9,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,9,0),Vector3(16,9,16),Vector3(16,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,3,0),Vector3(16,3,0),Vector3(16,3,16),Vector3(0,3,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,9,16),Vector3(16,9,16),Vector3(16,9,0),Vector3(0,9,0),1,0);

            //BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,9,0),Vector3(16,9,0),Vector3(16,0,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,9,16),Vector3(0,9,16));
            return;
        }
        if(Block == BedBlock8::getID())
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,9,16),Vector3(0,9,0),Vector3(0,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,9,0),Vector3(16,9,16),Vector3(16,0,16));

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,3,0),Vector3(16,3,0),Vector3(16,3,16),Vector3(0,3,16));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,9,16),Vector3(16,9,16),Vector3(16,9,0),Vector3(0,9,0),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,9,0),Vector3(16,9,0),Vector3(16,0,0),Vector3(0,0,0));
            //BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,9,16),Vector3(0,9,16));
            return;
        }

        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,8,16),Vector3(0,8,0),Vector3(0,0,0));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,8,0),Vector3(16,8,16),Vector3(16,0,16));

        if(BlockTransparentOrSpecial(x,y-1,z))
        {
            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
        }
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,8,16),Vector3(16,8,16),Vector3(16,8,0),Vector3(0,8,0),1,0);

        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,8,0),Vector3(16,8,0),Vector3(16,0,0),Vector3(0,0,0));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,8,16),Vector3(0,8,16));

        return;
    }


    /// FENCE
    if(blockType->blockModel == 2)
    {
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(6,0,10),Vector3(6,16,10),Vector3(6,16,6),Vector3(6,0,6),1,0);
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(10,0,6),Vector3(10,16,6),Vector3(10,16,10),Vector3(10,0,10),1,0);

        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(6,0,6),Vector3(10,0,6),Vector3(10,0,10),Vector3(6,0,10));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(6,16,10),Vector3(10,16,10),Vector3(10,16,6),Vector3(6,16,6));

        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(6,16,6),Vector3(10,16,6),Vector3(10,0,6),Vector3(6,0,6),1,0);
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(6,0,10),Vector3(10,0,10),Vector3(10,16,10),Vector3(6,16,10),1,0);

        if(BlockSolid(x,y,z-1) == true)
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(7,12,8),Vector3(7,15,8),Vector3(7,15,0),Vector3(7,12,0),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(7,6,8),Vector3(7,9,8),Vector3(7,9,0),Vector3(7,6,0),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(9,12,0),Vector3(9,15,0),Vector3(9,15,8),Vector3(9,12,8),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(9,6,0),Vector3(9,9,0),Vector3(9,9,8),Vector3(9,6,8),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(7,12,0),Vector3(9,12,0),Vector3(9,12,8),Vector3(7,12,8),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(7,6,0),Vector3(9,6,0),Vector3(9,6,8),Vector3(7,6,8),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(7,15,8),Vector3(9,15,8),Vector3(9,15,0),Vector3(7,15,0),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(7,9,8),Vector3(9,9,8),Vector3(9,9,0),Vector3(7,9,0),1,0);
        }

        if(BlockSolid(x,y,z+1) == true)
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(7,12,16),Vector3(7,15,16),Vector3(7,15,8),Vector3(7,12,8),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(7,6,16),Vector3(7,9,16),Vector3(7,9,8),Vector3(7,6,8),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(9,12,8),Vector3(9,15,8),Vector3(9,15,16),Vector3(9,12,16),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(9,6,8),Vector3(9,9,8),Vector3(9,9,16),Vector3(9,6,16),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(7,12,8),Vector3(9,12,8),Vector3(9,12,16),Vector3(7,12,16),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(7,6,8),Vector3(9,6,8),Vector3(9,6,16),Vector3(7,6,16),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(7,15,16),Vector3(9,15,16),Vector3(9,15,8),Vector3(7,15,8),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(7,9,16),Vector3(9,9,16),Vector3(9,9,8),Vector3(7,9,8),1,0);
        }

        if(BlockSolid(x-1,y,z) == true)
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,15,7),Vector3(8,15,7),Vector3(8,12,7),Vector3(0,12,7),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,9,7),Vector3(8,9,7),Vector3(8,6,7),Vector3(0,6,7),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,12,9),Vector3(8,12,9),Vector3(8,15,9),Vector3(0,15,9),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,6,9),Vector3(8,6,9),Vector3(8,9,9),Vector3(0,9,9),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,12,7),Vector3(8,12,7),Vector3(8,12,9),Vector3(0,12,9),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,6,7),Vector3(8,6,7),Vector3(8,6,9),Vector3(0,6,9),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,15,9),Vector3(8,15,9),Vector3(8,15,7),Vector3(0,15,7),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,9,9),Vector3(8,9,9),Vector3(8,9,7),Vector3(0,9,7),1,0);
        }

        if(BlockSolid(x+1,y,z) == true)
        {
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(8,15,7),Vector3(16,15,7),Vector3(16,12,7),Vector3(8,12,7),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(8,9,7),Vector3(16,9,7),Vector3(16,6,7),Vector3(8,6,7),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(8,12,9),Vector3(16,12,9),Vector3(16,15,9),Vector3(8,15,9),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(8,6,9),Vector3(16,6,9),Vector3(16,9,9),Vector3(8,9,9),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(8,12,7),Vector3(16,12,7),Vector3(16,12,9),Vector3(8,12,9),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(8,6,7),Vector3(16,6,7),Vector3(16,6,9),Vector3(8,6,9),1,0);

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(8,15,9),Vector3(16,15,9),Vector3(16,15,7),Vector3(8,15,7),1,0);
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(8,9,9),Vector3(16,9,9),Vector3(16,9,7),Vector3(8,9,7),1,0);
        }

        return;
    }

    /// FLOWERS & CANES & MOOSHROOMS
    if(blockType->blockModel == 3)
    {
        BlockColorx1 = Vector3(BlockFinalLight(x,y,z),BlockFinalLight(x,y,z),BlockFinalLight(x,y,z));
        BlockColorx2 = BlockColorx1*0.9f;

        up = blockType->leftPlane_y*percent;
        down = up+percent;

        left = blockType->leftPlane_x*percent;
        right = left+percent;

        transparentBlock = DefaultBlock;
        canCreate = true;

        if (canCreate)
        {
            light1 = Vector3(1,1,1);
            light2 = Vector3(1,1,1);
            light3 = Vector3(1,1,1);
            light4 = Vector3(1,1,1);

            light1 = light2 = light3 = light4 = BlockColorx1;

            MeshChunk->vert(x, y, z+1, right, down, light1.x,light1.y,light1.z);
            MeshChunk->vert(x, y+1, z+1, right, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x+1, y+1, z, left, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x+1, y, z, left, down, light3.x,light3.y,light3.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }

        //x+1
        transparentBlock = DefaultBlock;
        canCreate = true;

        if (canCreate)
        {
            light1 = Vector3(1,1,1);
            light2 = Vector3(1,1,1);
            light3 = Vector3(1,1,1);
            light4 = Vector3(1,1,1);

            light1 = light2 = light3 = light4 = BlockColorx2;

            MeshChunk->vert(x, y, z, right, down, light3.x,light3.y,light3.z);
            MeshChunk->vert(x, y+1, z, right, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x+1, y+1, z+1, left, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x+1, y, z+1, left, down, light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }

        transparentBlock = DefaultBlock;
        canCreate = true;

        if (canCreate)
        {
            light1 = Vector3(1,1,1);
            light2 = Vector3(1,1,1);
            light3 = Vector3(1,1,1);
            light4 = Vector3(1,1,1);

            light1 = light2 = light3 = light4 = BlockColorx2;

            MeshChunk->vert(x+1, y, z+1, right, down, light3.x,light3.y,light3.z);
            MeshChunk->vert(x+1, y+1, z+1, right, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x, y+1, z, left, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x, y, z, left, down, light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }

        transparentBlock = DefaultBlock;
        canCreate = true;

        if (canCreate)
        {
            light1 = Vector3(1,1,1);
            light2 = Vector3(1,1,1);
            light3 = Vector3(1,1,1);
            light4 = Vector3(1,1,1);

            light1 = light2 = light3 = light4 = BlockColorx1;

            MeshChunk->vert(x+1, y, z, right, down, light1.x,light1.y,light1.z);
            MeshChunk->vert(x+1, y+1, z, right, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x, y+1, z+1, left, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x, y, z+1, left, down, light3.x,light3.y,light3.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }

        return;
    }

    /// WHEAT
    if(blockType->blockModel == 4)
    {
        //x-1
        transparentBlock = DefaultBlock;
        canCreate = true;

        BlockColorx2 = Vector3(BlockFinalLight(x,y,z),BlockFinalLight(x,y,z),BlockFinalLight(x,y,z));
        BlockColorz2 = BlockColorx2;

        up = blockType->leftPlane_y*percent;
        down = up+percent;

        left = blockType->leftPlane_x*percent;
        right = left+percent;

        if (canCreate)
        {
            light1 = light2 = light3 = light4 = BlockColorx2;

            MeshChunk->vert(x+pixel*4, y, z+1, right, down, light1.x,light1.y,light1.z);
            MeshChunk->vert(x+pixel*4, y+1, z+1, right, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x+pixel*4, y+1, z, left, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x+pixel*4, y, z, left, down, light3.x,light3.y,light3.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }

        if (canCreate)
        {
            light1 = light2 = light3 = light4 = BlockColorx2;

            MeshChunk->vert(x+pixel*4, y, z, right, down, light3.x,light3.y,light3.z);
            MeshChunk->vert(x+pixel*4, y+1, z, right, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x+pixel*4, y+1, z+1, left, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x+pixel*4, y, z+1, left, down, light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }

        if (canCreate)
        {
            light1 = light2 = light3 = light4 = BlockColorx2;

            MeshChunk->vert(x+pixel*12, y, z, right, down, light3.x,light3.y,light3.z);
            MeshChunk->vert(x+pixel*12, y+1, z, right, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x+pixel*12, y+1, z+1, left, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x+pixel*12, y, z+1, left, down, light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }

        if (canCreate)
        {
            light1 = light2 = light3 = light4 = BlockColorx2;

            MeshChunk->vert(x+pixel*12, y, z+1, right, down, light1.x,light1.y,light1.z);
            MeshChunk->vert(x+pixel*12, y+1, z+1, right, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x+pixel*12, y+1, z, left, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x+pixel*12, y, z, left, down, light3.x,light3.y,light3.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }

        if (canCreate)
        {
            light1 = light2 = light3 = light4 = BlockColorz2;

            MeshChunk->vert(x, y+1, z+pixel*4, right, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x+1, y+1, z+pixel*4, left, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x+1, y, z+pixel*4, left, down, light3.x,light3.y,light3.z);
            MeshChunk->vert(x, y, z+pixel*4, right, down, light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }

        if (canCreate == true)
        {
            light1 = light2 = light3 = light4 = BlockColorz2;

            MeshChunk->vert(x, y, z+pixel*4, left, down, light1.x,light1.y,light1.z);
            MeshChunk->vert(x+1, y, z+pixel*4, right, down, light3.x,light3.y,light3.z);
            MeshChunk->vert(x+1, y+1, z+pixel*4, right, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x, y+1, z+pixel*4, left, up, light2.x,light2.y,light2.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }

        if (canCreate == true)
        {
            light1 = light2 = light3 = light4 = BlockColorz2;

            MeshChunk->vert(x, y, z+pixel*12, left, down, light1.x,light1.y,light1.z);
            MeshChunk->vert(x+1, y, z+pixel*12, right, down, light3.x,light3.y,light3.z);
            MeshChunk->vert(x+1, y+1, z+pixel*12, right, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x, y+1, z+pixel*12, left, up, light2.x,light2.y,light2.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }

        if (canCreate)
        {
            light1 = light2 = light3 = light4 = BlockColorz2;

            MeshChunk->vert(x, y+1, z+pixel*12, right, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x+1, y+1, z+pixel*12, left, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x+1, y, z+pixel*12, left, down, light3.x,light3.y,light3.z);
            MeshChunk->vert(x, y, z+pixel*12, right, down, light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
        return;
    }

    /// STAIRS
    if(blockType->blockModel == 6)
    {
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,8,16),Vector3(0,8,0),Vector3(0,0,0));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,8,16),Vector3(0,16,16),Vector3(0,16,8),Vector3(0,8,8));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,8,0),Vector3(16,8,16),Vector3(16,0,16));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,8,8),Vector3(16,16,8),Vector3(16,16,16),Vector3(16,8,16));

        if(BlockTransparentOrSpecial(x,y-1,z) == true)
            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));

        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,8,16),Vector3(16,8,16),Vector3(16,8,0),Vector3(0,8,0),1,0);
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,16,16),Vector3(16,16,16),Vector3(16,16,8),Vector3(0,16,8));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,8,0),Vector3(16,8,0),Vector3(16,0,0),Vector3(0,0,0));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,8),Vector3(16,16,8),Vector3(16,8,8),Vector3(0,8,8),1,0);

        if(BlockTransparentOrSpecial(x,y,z+1) == true)
            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,16,16),Vector3(0,16,16));

        return;
    }

    if(blockType->blockModel == 7)
    {
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,8,16),Vector3(0,8,0),Vector3(0,0,0));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,8,8),Vector3(0,16,8),Vector3(0,16,0),Vector3(0,8,0));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,8,0),Vector3(16,8,16),Vector3(16,0,16));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,8,0),Vector3(16,16,0),Vector3(16,16,8),Vector3(16,8,8));

        if(BlockTransparentOrSpecial(x,y-1,z) == true)
            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));

        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,8,16),Vector3(16,8,16),Vector3(16,8,0),Vector3(0,8,0),1,0);
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,16,8),Vector3(16,16,8),Vector3(16,16,0),Vector3(0,16,0));

        if(BlockTransparentOrSpecial(x,y,z-1) == true)
            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,0),Vector3(16,16,0),Vector3(16,0,0),Vector3(0,0,0));

        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,8,16),Vector3(0,8,16));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,8,8),Vector3(16,8,8),Vector3(16,16,8),Vector3(0,16,8),1,0);

        return;
    }

    if(blockType->blockModel == 8)
    {
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,8,16),Vector3(0,8,0),Vector3(0,0,0));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(8,8,16),Vector3(8,16,16),Vector3(8,16,0),Vector3(8,8,0),1,0);

        if(BlockTransparentOrSpecial(x+1,y,z) == true)
            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,16,0),Vector3(16,16,16),Vector3(16,0,16));


        if(BlockTransparentOrSpecial(x,y-1,z) == true)
            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));

        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,8,16),Vector3(16,8,16),Vector3(16,8,0),Vector3(0,8,0),1,0);
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(8,16,16),Vector3(16,16,16),Vector3(16,16,0),Vector3(8,16,0));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,8,0),Vector3(16,8,0),Vector3(16,0,0),Vector3(0,0,0));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(8,16,0),Vector3(16,16,0),Vector3(16,8,0),Vector3(8,8,0));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,8,16),Vector3(0,8,16));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(8,8,16),Vector3(16,8,16),Vector3(16,16,16),Vector3(8,16,16));

        return;
    }

    if(blockType->blockModel == 9)
    {
        if(BlockTransparentOrSpecial(x-1,y,z) == true)
            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,16,16),Vector3(0,16,0),Vector3(0,0,0));

        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,8,0),Vector3(16,8,16),Vector3(16,0,16));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(8,8,0),Vector3(8,16,0),Vector3(8,16,16),Vector3(8,8,16),1,0);

        if(BlockTransparentOrSpecial(x,y-1,z) == true)
            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));

        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,8,16),Vector3(16,8,16),Vector3(16,8,0),Vector3(0,8,0),1,0);
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,16,16),Vector3(8,16,16),Vector3(8,16,0),Vector3(0,16,0));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,8,0),Vector3(16,8,0),Vector3(16,0,0),Vector3(0,0,0));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,0),Vector3(8,16,0),Vector3(8,8,0),Vector3(0,8,0));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,8,16),Vector3(0,8,16));
        BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,8,16),Vector3(8,8,16),Vector3(8,16,16),Vector3(0,16,16));

        return;
    }

    /// PANELS
    if(blockType->blockModel == 10)
    {
        bool connectLeft, connectRight, connectFront, connectBack;
        connectLeft = (BlockSolid(x-1,y,z) && (!BlockTransparentOrSpecial(x-1,y,z) || GetBlock(x-1,y,z) == Block));
        connectRight = (BlockSolid(x+1,y,z) && (!BlockTransparentOrSpecial(x+1,y,z) || GetBlock(x+1,y,z) == Block));
        connectBack = (BlockSolid(x,y,z-1) && (!BlockTransparentOrSpecial(x,y,z-1) || GetBlock(x,y,z-1) == Block));
        connectFront = (BlockSolid(x,y,z+1) && (!BlockTransparentOrSpecial(x,y,z+1) || GetBlock(x,y,z+1) == Block));
        int connectsNum = 0;

        if(!connectFront && !connectBack && !connectLeft && !connectRight)
        {
            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(8,0,16),Vector3(8,16,16),Vector3(8,16,0),Vector3(8,0,0));
            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(8,0,0),Vector3(8,16,0),Vector3(8,16,16),Vector3(8,0,16),true,false,0);

            if(BlockSolid(x,y-1,z) == false)
            {
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,7),Vector3(16,0,7),Vector3(16,0,9),Vector3(0,0,9));
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(7,0,0),Vector3(9,0,0),Vector3(9,0,16),Vector3(7,0,16));

                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(7,0,16),Vector3(9,0,16),Vector3(9,0,0),Vector3(7,0,0),1,0);
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,0,9),Vector3(16,0,9),Vector3(16,0,7),Vector3(0,0,7),1,0);
            }
            if(BlockSolid(x,y+1,z) == false)
            {
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(7,16,16),Vector3(9,16,16),Vector3(9,16,0),Vector3(7,16,0),1,0);
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,16,9),Vector3(16,16,9),Vector3(16,16,7),Vector3(0,16,7),1,0);

                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,16,7),Vector3(16,16,7),Vector3(16,16,9),Vector3(0,16,9));
                BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(7,16,0),Vector3(9,16,0),Vector3(9,16,16),Vector3(7,16,16));
            }

            BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,8),Vector3(16,16,8),Vector3(16,0,8),Vector3(0,0,8),true,false,5);
            BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,8),Vector3(16,0,8),Vector3(16,16,8),Vector3(0,16,8));
        }
        else
        {
            if(connectLeft)
            {
                BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,8),Vector3(8,16,8),Vector3(8,0,8),Vector3(0,0,8));
                BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,8),Vector3(8,0,8),Vector3(8,16,8),Vector3(0,16,8));

                if(BlockSolid(x,y-1,z) == false)
                {
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,7),Vector3(8,0,7),Vector3(8,0,9),Vector3(0,0,9));
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,0,9),Vector3(8,0,9),Vector3(8,0,7),Vector3(0,0,7),1,0);
                }
                if(BlockSolid(x,y+1,z) == false)
                {
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,16,9),Vector3(8,16,9),Vector3(8,16,7),Vector3(0,16,7),1,0);
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,16,7),Vector3(8,16,7),Vector3(8,16,9),Vector3(0,16,9));
                }
                connectsNum ++;
            }
            if(connectRight)
            {
                BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(8,16,8),Vector3(16,16,8),Vector3(16,0,8),Vector3(8,0,8));
                BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(8,0,8),Vector3(16,0,8),Vector3(16,16,8),Vector3(8,16,8));

                if(BlockSolid(x,y-1,z) == false)
                {
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(8,0,7),Vector3(16,0,7),Vector3(16,0,9),Vector3(8,0,9));
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(8,0,9),Vector3(16,0,9),Vector3(16,0,7),Vector3(8,0,7),1,0);
                }
                if(BlockSolid(x,y+1,z) == false)
                {
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(8,16,9),Vector3(16,16,9),Vector3(16,16,7),Vector3(8,16,7),1,0);
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(8,16,7),Vector3(16,16,7),Vector3(16,16,9),Vector3(8,16,9));
                }
                connectsNum ++;
            }
            if(connectBack)
            {
                BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(8,0,8),Vector3(8,16,8),Vector3(8,16,0),Vector3(8,0,0));
                BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(8,0,0),Vector3(8,16,0),Vector3(8,16,8),Vector3(8,0,8));

                if(BlockSolid(x,y-1,z) == false)
                {
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(7,0,0),Vector3(9,0,0),Vector3(9,0,8),Vector3(7,0,8));
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(7,0,8),Vector3(9,0,8),Vector3(9,0,0),Vector3(7,0,0));
                }
                if(BlockSolid(x,y+1,z) == false)
                {
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(7,16,8),Vector3(9,16,8),Vector3(9,16,0),Vector3(7,16,0));
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(7,16,0),Vector3(9,16,0),Vector3(9,16,8),Vector3(7,16,8));
                }
                connectsNum ++;
            }
            if(connectFront)
            {
                BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(8,0,16),Vector3(8,16,16),Vector3(8,16,8),Vector3(8,0,8));
                BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(8,0,8),Vector3(8,16,8),Vector3(8,16,16),Vector3(8,0,16));

                if(BlockSolid(x,y-1,z) == false)
                {
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(7,0,8),Vector3(9,0,8),Vector3(9,0,16),Vector3(7,0,16));
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(7,0,16),Vector3(9,0,16),Vector3(9,0,8),Vector3(7,0,8));
                }
                if(BlockSolid(x,y+1,z) == false)
                {
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(7,16,16),Vector3(9,16,16),Vector3(9,16,8),Vector3(7,16,8));
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(7,16,8),Vector3(9,16,8),Vector3(9,16,16),Vector3(7,16,16));
                }
                connectsNum ++;
            }

            if(connectsNum == 1)
            {
                if(connectLeft || connectRight)
                {
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(8,0,9),Vector3(8,16,9),Vector3(8,16,7),Vector3(8,0,7),true,false,0);
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(8,0,7),Vector3(8,16,7),Vector3(8,16,9),Vector3(8,0,9),true,false,0);
                }
                if(connectBack || connectFront)
                {
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(7,16,8),Vector3(9,16,8),Vector3(9,0,8),Vector3(7,0,8),true,false,0);
                    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(7,0,8),Vector3(9,0,8),Vector3(9,16,8),Vector3(7,16,8),true,false,0);
                }
            }
        }
    }

    /*
    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,1,Vector3(0,0,16),Vector3(0,16,16),Vector3(0,16,0),Vector3(0,0,0));
    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,2,Vector3(16,0,0),Vector3(16,16,0),Vector3(16,16,16),Vector3(16,0,16));

    BuildWorldBlockPlaneNoCheck(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,3,Vector3(0,0,0),Vector3(16,0,0),Vector3(16,0,16),Vector3(0,0,16));
    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,4,Vector3(0,16,16),Vector3(16,16,16),Vector3(16,16,0),Vector3(0,16,0),1,0);

    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,5,Vector3(0,16,0),Vector3(16,16,0),Vector3(16,0,0),Vector3(0,0,0));
    BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,6,Vector3(0,0,16),Vector3(16,0,16),Vector3(16,16,16),Vector3(0,16,16));
    */
}

void CraftWorld::GetNormalBlock(int x,int y, int z, int &iVertex, SimpleMeshChunk* MeshChunk, block_t Block, bool transparent)
{
    ///left     right
    // x y z    x y z
    // 0 0 1    1 0 0
    // 0 1 1    1 1 0
    // 0 1 0    1 1 1
    // 0 0 0    1 0 1

    ///down     top
    // x y z    x y z
    // 0 0 0    0 1 1
    // 1 0 0    1 1 1
    // 1 0 1    1 1 0
    // 0 0 1    0 1 0

    ///back     forward
    // x y z    x y z
    // 0 1 0    0 0 1
    // 1 1 0    1 0 1
    // 1 0 0    1 1 1
    // 0 0 0    0 1 1

    Block = _GetBlockNoCheck(x,y,z);
    BaseBlock *blockType = &blockTypes[Block];

    int xx = x;
    int yy = y;
    int zz = z;

    bool anotherLight = false;
    char side = -1;

    if(Block == FurnaceOff::getID() || Block == FurnaceOn::getID())
    {
        int o;
        o = -1;
        for(unsigned int i = 0; i < mFurnaces.size(); i++)
        {
            Furnace* NewFurnace = mFurnaces[i];
            if(NewFurnace->furnaceX == x && NewFurnace->furnaceY == y && NewFurnace->furnaceZ == z)
            {
                side = NewFurnace->GetSide();
                break;
            }
        }
    }

    bool canCreate = false;
    bool DefaultBlock = false;
    bool transparentBlock = false;
    char specialBlock = 0;

    float up = 0.0f;
    float down = 0.0f;
    float left = 0.0f;
    float right = 0.0f;

    block_t Block1 = 0;
    BaseBlock *blockType1;

    float light[20];
    for(int o = 0; o <= 19; o++)
    {
        light[o] = -1;
    }
    bool onCorners = false;
    if(x == 0 || y == 0 || z == 0 || x == WORLD_SIZE-1 || y == WORLD_HEIGHT-1 || z == WORLD_SIZE-1)
    {
        onCorners = true;
    }

    Vector3 light1, light2, light3, light4;

    Vector3 BlockColorx1;
    onCorners == true ? Block1 = _GetBlock(x-1,y,z) : Block1 = _GetBlockNoCheck(x-1,y,z);
    blockType1 = &blockTypes[Block1];

    transparentBlock = blockType1->transparent;
    specialBlock = blockType1->blockModel;
    if(transparent == true)
    {
        if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (specialBlock > 0 && Block1 != 4 && Block1 != 112))
        {
            canCreate = true;
        }
        if (transparentBlock == false && specialBlock == 0)
        {
            canCreate = false;
        }
    }
    else
    {
        if (specialBlock > 0 && Block1 != Block)
        {
            canCreate = true;
        }
        if (transparentBlock == true || Block1 == 0)
        {
            canCreate = true;
        }
    }

    if(mainOptions.fastRendering)
    {
        if(Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID())
        {
            canCreate = false;
        }
    }

    if(canCreate == true)
    {
        float mainLight = BlockFinalLight(x-1,y,z);
        BlockColorx1 = Vector3(mainLight,mainLight,mainLight) * blockXConstLight;
        if(westLight != 0)
        {
            BlockColorx1.x += 0.3f*westLight;
            BlockColorx1.y += 0.2f*westLight;
        }

        if (BlockColorx1.x > 1.0f)
        {
            BlockColorx1.x = 1.0f;
        }
        if (BlockColorx1.y > 1.0f)
        {
            BlockColorx1.y = 1.0f;
        }
        light1 = light2 = light3 = light4 = BlockColorx1;

        if(mainOptions.smoothLighting == true)
        {
            if(onCorners)
            {
                light[0] = BlockFinalLight(x-1,y+1,z) ;
                light[1] = BlockFinalLight(x-1,y,z+1) ;
                light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                light[3] = BlockFinalLight(x-1,y,z-1) ;
                light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                light[5] = BlockFinalLight(x-1,y-1,z) ;
                light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                light[7] = BlockFinalLight(x-1,y-1,z-1) ;
            }
            else
            {
                light[0] = BlockFinalLightNoCheck(x-1,y+1,z) ;
                light[1] = BlockFinalLightNoCheck(x-1,y,z+1) ;
                light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                light[3] = BlockFinalLightNoCheck(x-1,y,z-1) ;
                light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                light[5] = BlockFinalLightNoCheck(x-1,y-1,z) ;
                light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
            }

            float light1noV = (light[0]+light[1]+light[2]+mainLight)/4.0f;
            float light2noV = (light[0]+light[3]+light[4]+mainLight)/4.0f;
            float light3noV = (light[5]+light[1]+light[6]+mainLight)/4.0f;
            float light4noV = (light[5]+light[3]+light[7]+mainLight)/4.0f;

            float lightFactor1 = light1noV * lightShadowFactor;
            float lightFactor2 = light2noV * lightShadowFactor;
            float lightFactor3 = light3noV * lightShadowFactor;
            float lightFactor4 = light4noV * lightShadowFactor;

            if(onCorners)
            {
                if(!BlockTransparentOrLightSource(x-1,y+1,z) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1))
                {
                    light1noV-=lightFactor1;
                }

                if(!BlockTransparentOrLightSource(x-1,y+1,z) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1))
                {
                    light2noV-=lightFactor2;
                }
                if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1))
                {
                    light3noV-=lightFactor3;
                }

                if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y-1,z-1))
                {
                    light4noV-=lightFactor4;
                }
            }
            else
            {
                if(!BlockTransparentOrLightSourceNoCheck(x-1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1))
                {
                    light1noV-=lightFactor1;
                }

                if(!BlockTransparentOrLightSourceNoCheck(x-1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1))
                {
                    light2noV-=lightFactor2;
                }
                if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1))
                {
                    light3noV-=lightFactor3;
                }

                if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1))
                {
                    light4noV-=lightFactor4;
                }
            }

            light1 = Vector3(light3noV,light3noV,light3noV)*blockXConstLight;
            light2 = Vector3(light1noV,light1noV,light1noV)*blockXConstLight;
            light3 = Vector3(light4noV,light4noV,light4noV)*blockXConstLight;
            light4 = Vector3(light2noV,light2noV,light2noV)*blockXConstLight;
        }

        left = percent*blockType->leftPlane_x;
        right = percent*blockType->leftPlane_x+percent;

        up = percent*blockType->leftPlane_y;
        down = percent*blockType->leftPlane_y+percent;

        if(side != -1)
        {
            int dif = Block - FurnaceOff::getID();

            if(side == 2)
            {
                left = percent*(12+dif);
                right = percent*(12+dif)+percent;

                up = percent*(2+dif);
                down = percent*(2+dif)+percent;
            }
        }

        MeshChunk->vert(x, y, z+1, right, down, light1.x,light1.y,light1.z);
        MeshChunk->vert(x, y+1, z+1, right, up, light2.x,light2.y,light2.z);
        MeshChunk->vert(x, y+1, z, left, up, light4.x,light4.y,light4.z);
        MeshChunk->vert(x, y, z, left, down,light3.x,light3.y,light3.z);

        MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
        MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

        iVertex += 4;
    }


    Vector3 BlockColorx2;
    canCreate = false;
    onCorners == true ? Block1 = _GetBlock(x+1,y,z) : Block1 = _GetBlockNoCheck(x+1,y,z);
    blockType1 = &blockTypes[Block1];

    transparentBlock = blockType1->transparent;
    specialBlock = blockType1->blockModel;
    if(transparent == true)
    {
        if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (specialBlock > 0 && Block1 != 4 && Block1 != 112))
        {
            canCreate = true;
        }
        if (transparentBlock == false && specialBlock == 0)
        {
            canCreate = false;
        }
    }
    else
    {
        if (specialBlock > 0 && Block1 != Block)
        {
            canCreate = true;
        }
        if (transparentBlock == true || Block1 == 0)
        {
            canCreate = true;
        }
    }

    if(mainOptions.fastRendering)
    {
        if(Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID())
        {
            canCreate = false;
        }
    }

    if (canCreate)
    {

        float mainLight = BlockFinalLight(x+1,y,z);
        BlockColorx2 = Vector3(mainLight,mainLight,mainLight) * blockXConstLight;

        if(eastLight != 0)
        {
            BlockColorx2.x += 0.3f*eastLight;
            BlockColorx2.y += 0.2f*eastLight;
        }

        if (BlockColorx2.x > 1.0f)
        {
            BlockColorx2.x = 1.0f;
        }
        if (BlockColorx2.y > 1.0f)
        {
            BlockColorx2.y = 1.0f;
        }

        light1 = light2 = light3 = light4 = BlockColorx2;

        if(mainOptions.smoothLighting == true)
        {
            if(onCorners)
            {
                light[8] = BlockFinalLight(x+1,y+1,z) ;
                light[9] = BlockFinalLight(x+1,y,z+1) ;
                light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                light[11] = BlockFinalLight(x+1,y,z-1) ;
                light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                light[13] = BlockFinalLight(x+1,y-1,z) ;
                light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                light[15] = BlockFinalLight(x+1,y-1,z-1) ;
            }
            else
            {
                light[8] = BlockFinalLightNoCheck(x+1,y+1,z) ;
                light[9] = BlockFinalLightNoCheck(x+1,y,z+1) ;
                light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                light[11] = BlockFinalLightNoCheck(x+1,y,z-1) ;
                light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                light[13] = BlockFinalLightNoCheck(x+1,y-1,z) ;
                light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
            }

            float light1noV = (light[8]+light[9]+light[10]+mainLight)/4.0f;
            float light2noV = (light[8]+light[11]+light[12]+mainLight)/4.0f;
            float light3noV = (light[13]+light[9]+light[14]+mainLight)/4.0f;
            float light4noV = (light[13]+light[11]+light[15]+mainLight)/4.0f;

            float lightFactor1 = light1noV * lightShadowFactor;
            float lightFactor2 = light2noV * lightShadowFactor;
            float lightFactor3 = light3noV * lightShadowFactor;
            float lightFactor4 = light4noV * lightShadowFactor;

            if(onCorners)
            {
                if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1))
                {
                    light1noV-=lightFactor1;
                }

                if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y+1,z-1))
                {
                    light2noV-=lightFactor2;
                }
                if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y-1,z+1))
                {
                    light3noV-=lightFactor3;
                }

                if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1))
                {
                    light4noV-=lightFactor4;
                }
            }
            else
            {
                if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1))
                {
                    light1noV-=lightFactor1;
                }

                if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1))
                {
                    light2noV-=lightFactor2;
                }
                if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1))
                {
                    light3noV-=lightFactor3;
                }

                if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1))
                {
                    light4noV-=lightFactor4;
                }
            }

            light1 = Vector3(light3noV,light3noV,light3noV)*blockXConstLight;
            light2 = Vector3(light1noV,light1noV,light1noV)*blockXConstLight;
            light3 = Vector3(light4noV,light4noV,light4noV)*blockXConstLight;
            light4 = Vector3(light2noV,light2noV,light2noV)*blockXConstLight;
        }

        left = percent*blockType->rightPlane_x;
        right = percent*blockType->rightPlane_x+percent;

        up = percent*blockType->rightPlane_y;
        down = percent*blockType->rightPlane_y+percent;

        if(side != -1)
        {
            int dif = Block - FurnaceOff::getID();

            if(side == 1)
            {
                left = percent*(12+dif);
                right = percent*(12+dif)+percent;

                up = percent*(2+dif);
                down = percent*(2+dif)+percent;
            }
        }

        MeshChunk->vert(x+1, y, z, right, down, light3.x,light3.y,light3.z);
        MeshChunk->vert(x+1, y+1, z, right, up, light4.x,light4.y,light4.z);
        MeshChunk->vert(x+1, y+1, z+1, left, up, light2.x,light2.y,light2.z);
        MeshChunk->vert(x+1, y, z+1, left, down, light1.x,light1.y,light1.z);

        MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
        MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

        iVertex += 4;
    }



    canCreate = false;
    Vector3 BlockColory2;
    onCorners == true ? Block1 = _GetBlock(x,y-1,z) : Block1 = _GetBlockNoCheck(x,y-1,z);
    blockType1 = &blockTypes[Block1];

    transparentBlock = blockType1->transparent;
    specialBlock = blockType1->blockModel;
    if(transparent == true)
    {
        if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (specialBlock > 0 && Block1 != 4 && Block1 != 112))
        {
            canCreate = true;
        }
        if (transparentBlock == false && specialBlock == 0)
        {
            canCreate = false;
        }
    }
    else
    {
        if (specialBlock > 0 && Block1 != Block)
        {
            canCreate = true;
        }
        if (transparentBlock == true || Block1 == 0)
        {
            canCreate = true;
        }
    }

    if(mainOptions.fastRendering)
    {
        if(Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID())
        {
            canCreate = false;
        }
    }

    if (canCreate)
    {
        float mainLight = BlockFinalLight(x,y-1,z);
        BlockColory2 = Vector3(mainLight,mainLight,mainLight) * 0.6f;
        light1 = light2 = light3 = light4 = BlockColory2;

        if(mainOptions.smoothLighting == true)
        {
            if(onCorners)
            {
                if(light[5] == -1)
                {
                    light[5] = BlockFinalLight(x-1,y-1,z) ;
                }

                if(light[7] == -1)
                {
                    light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                }

                light[16] = BlockFinalLight(x,y-1,z-1) ;

                if(light[15] == -1)
                {
                    light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                }

                if(light[13] == -1)
                {
                    light[13] = BlockFinalLight(x+1,y-1,z) ;
                }

                if(light[14] == -1)
                {
                    light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                }

                if(light[6] == -1)
                {
                    light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                }

                light[17] = BlockFinalLight(x,y-1,z+1) ;
            }
            else
            {
                if(light[5] == -1)
                {
                    light[5] = BlockFinalLightNoCheck(x-1,y-1,z) ;
                }

                if(light[7] == -1)
                {
                    light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                }

                light[16] = BlockFinalLightNoCheck(x,y-1,z-1) ;

                if(light[15] == -1)
                {
                    light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                }

                if(light[13] == -1)
                {
                    light[13] = BlockFinalLightNoCheck(x+1,y-1,z) ;
                }

                if(light[14] == -1)
                {
                    light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                }

                if(light[6] == -1)
                {
                    light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                }

                light[17] = BlockFinalLightNoCheck(x,y-1,z+1) ;
            }

            float light1noV = (light[5]+light[7]+light[16]+mainLight)/4.0f;
            float light2noV = (light[16]+light[15]+light[13]+mainLight)/4.0f;
            float light3noV = (light[13]+light[14]+light[17]+mainLight)/4.0f;
            float light4noV = (light[17]+light[6]+light[5]+mainLight)/4.0f;

            float lightFactor1 = light1noV * lightShadowFactor;
            float lightFactor2 = light2noV * lightShadowFactor;
            float lightFactor3 = light3noV * lightShadowFactor;
            float lightFactor4 = light4noV * lightShadowFactor;

            if(onCorners)
            {
                if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y-1,z-1) || !BlockTransparentOrLightSource(x,y-1,z-1))
                {
                    light1noV-=lightFactor1;
                }

                if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z))
                {
                    light2noV-=lightFactor2;
                }

                if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y-1,z+1) || !BlockTransparentOrLightSource(x,y-1,z+1))
                {
                    light3noV-=lightFactor3;
                }

                if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z))
                {
                    light4noV-=lightFactor4;
                }
            }
            else
            {
                if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x,y-1,z-1))
                {
                    light1noV-=lightFactor1;
                }

                if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z))
                {
                    light2noV-=lightFactor2;
                }

                if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x,y-1,z+1))
                {
                    light3noV-=lightFactor3;
                }

                if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z))
                {
                    light4noV-=lightFactor4;
                }
            }

            light1 = Vector3(light1noV,light1noV,light1noV)*0.6f;
            light2 = Vector3(light2noV,light2noV,light2noV)*0.6f;
            light3 = Vector3(light3noV,light3noV,light3noV)*0.6f;
            light4 = Vector3(light4noV,light4noV,light4noV)*0.6f;
        }

        left = percent*blockType->downPlane_x;
        right = percent*blockType->downPlane_x+percent;

        up = percent*blockType->downPlane_y;
        down = percent*blockType->downPlane_y+percent;

        MeshChunk->vert(x, y, z, left, up, light1.x,light1.y,light1.z);
        MeshChunk->vert(x+1, y, z, right, up, light2.x,light2.y,light2.z);
        MeshChunk->vert(x+1, y, z+1, right, down, light3.x,light3.y,light3.z);
        MeshChunk->vert(x, y, z+1, left, down, light4.x,light4.y,light4.z);

        MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
        MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

        iVertex += 4;
    }



    canCreate = false;
    Vector3 BlockColory1;
    onCorners == true ? Block1 = _GetBlock(x,y+1,z) : Block1 = _GetBlockNoCheck(x,y+1,z);
    blockType1 = &blockTypes[Block1];

    transparentBlock = blockType1->transparent;
    specialBlock = blockType1->blockModel;
    if(transparent == true)
    {
        if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (specialBlock > 0 && Block1 != 4 && Block1 != 112))
        {
            canCreate = true;
        }
        if (transparentBlock == false && specialBlock == 0)
        {
            canCreate = false;
        }
    }
    else
    {
        if (specialBlock > 0 && Block1 != Block)
        {
            canCreate = true;
        }
        if (transparentBlock == true || Block1 == 0)
        {
            canCreate = true;
        }
    }

    if(Block1 == 93)
    {
        canCreate = false;
    }

    if(mainOptions.fastRendering)
    {
        if(Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID())
        {
            canCreate = false;
        }
    }

    if(canCreate)
    {
        float mainLight = BlockFinalLight(x,y+1,z);
        BlockColory1 = Vector3(mainLight,mainLight,mainLight);

        light1 = Vector3(1,1,1);
        light2 = Vector3(1,1,1);
        light3 = Vector3(1,1,1);
        light4 = Vector3(1,1,1);

        light1 *= BlockColory1;
        light2 *= BlockColory1;
        light3 *= BlockColory1;
        light4 *= BlockColory1;

        if(mainOptions.smoothLighting == true)
        {
            if(onCorners)
            {
                light[18] = BlockFinalLight(x,y+1,z+1) ;

                if(light[2] == -1)
                {
                    light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                }

                if(light[0] == -1)
                {
                    light[0] = BlockFinalLight(x-1,y+1,z) ;
                }

                if(light[10] == -1)
                {
                    light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                }

                if(light[8] == -1)
                {
                    light[8] = BlockFinalLight(x+1,y+1,z) ;
                }

                if(light[12] == -1)
                {
                    light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                }

                light[19] = BlockFinalLight(x,y+1,z-1) ;

                if(light[4] == -1)
                {
                    light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                }
            }
            else
            {
                light[18] = BlockFinalLightNoCheck(x,y+1,z+1) ;

                if(light[2] == -1)
                {
                    light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                }

                if(light[0] == -1)
                {
                    light[0] = BlockFinalLightNoCheck(x-1,y+1,z) ;
                }

                if(light[10] == -1)
                {
                    light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                }

                if(light[8] == -1)
                {
                    light[8] = BlockFinalLightNoCheck(x+1,y+1,z) ;
                }

                if(light[12] == -1)
                {
                    light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                }

                light[19] = BlockFinalLightNoCheck(x,y+1,z-1) ;

                if(light[4] == -1)
                {
                    light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                }
            }

            float light1noV = (light[18]+light[2]+light[0]+mainLight)/4.0f;
            float light2noV = (light[18]+light[10]+light[8]+mainLight)/4.0f;
            float light3noV = (light[8]+light[12]+light[19]+mainLight)/4.0f;
            float light4noV = (light[19]+light[4]+light[0]+mainLight)/4.0f;

            float lightFactor1 = light1noV * lightShadowFactor;
            float lightFactor2 = light2noV * lightShadowFactor;
            float lightFactor3 = light3noV * lightShadowFactor;
            float lightFactor4 = light4noV * lightShadowFactor;

            if(onCorners)
            {
                if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z))
                {
                    light1noV-=lightFactor1;
                }

                if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z))
                {
                    light2noV-=lightFactor2;
                }

                if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y+1,z-1) || !BlockTransparentOrLightSource(x,y+1,z-1))
                {
                    light3noV-=lightFactor3;
                }

                if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z))
                {
                    light4noV-=lightFactor4;
                }
            }
            else
            {
                if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z))
                {
                    light1noV-=lightFactor1;
                }

                if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z))
                {
                    light2noV-=lightFactor2;
                }

                if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x,y+1,z-1))
                {
                    light3noV-=lightFactor3;
                }

                if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z))
                {
                    light4noV-=lightFactor4;
                }
            }

            light1 = Vector3(light1noV,light1noV,light1noV);
            light2 = Vector3(light2noV,light2noV,light2noV);
            light3 = Vector3(light3noV,light3noV,light3noV);
            light4 = Vector3(light4noV,light4noV,light4noV);
        }

        left = percent*blockType->upPlane_x;
        right = percent*blockType->upPlane_x+percent;

        up = percent*blockType->upPlane_y;
        down = percent*blockType->upPlane_y+percent;

        MeshChunk->vert(x, y+1, z+1, left, up, light1.x,light1.y,light1.z);
        MeshChunk->vert(x+1, y+1, z+1, right, up, light2.x,light2.y,light2.z);
        MeshChunk->vert(x+1, y+1, z, right, down, light3.x,light3.y,light3.z);
        MeshChunk->vert(x, y+1, z, left, down, light4.x,light4.y,light4.z);

        MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
        MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

        iVertex += 4;
    }


    Vector3 BlockColorz1;
    canCreate = false;
    onCorners == true ? Block1 = _GetBlock(x,y,z-1) : Block1 = _GetBlockNoCheck(x,y,z-1);
    blockType1 = &blockTypes[Block1];

    transparentBlock = blockType1->transparent;
    specialBlock = blockType1->blockModel;
    if(transparent == true)
    {
        if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (specialBlock > 0 && Block1 != 4 && Block1 != 112))
        {
            canCreate = true;
        }
        if (transparentBlock == false && specialBlock == 0)
        {
            canCreate = false;
        }
    }
    else
    {
        if (specialBlock > 0 && Block1 != Block)
        {
            canCreate = true;
        }
        if (transparentBlock == true || Block1 == 0)
        {
            canCreate = true;
        }
    }

    if(mainOptions.fastRendering)
    {
        if(Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID())
        {
            canCreate = false;
        }
    }

    if (canCreate)
    {
        float mainLight = BlockFinalLight(x,y,z-1);
        BlockColorz1 = Vector3(mainLight,mainLight,mainLight) * blockZConstLight;
        light1 = light2 = light3 = light4 = BlockColorz1;

        if(mainOptions.smoothLighting == true)
        {
            if(onCorners)
            {
                if(light[19] == -1)
                {
                    light[19] = BlockFinalLight(x,y+1,z-1) ;
                }

                if(light[3] == -1)
                {
                    light[3] = BlockFinalLight(x-1,y,z-1) ;
                }

                if(light[4] == -1)
                {
                    light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                }

                if(light[11] == -1)
                {
                    light[11] = BlockFinalLight(x+1,y,z-1) ;
                }

                if(light[12] == -1)
                {
                    light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                }

                if(light[16] == -1)
                {
                    light[16] = BlockFinalLight(x,y-1,z-1) ;
                }

                if(light[7] == -1)
                {
                    light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                }

                if(light[15] == -1)
                {
                    light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                }
            }
            else
            {
                if(light[19] == -1)
                {
                    light[19] = BlockFinalLightNoCheck(x,y+1,z-1) ;
                }

                if(light[3] == -1)
                {
                    light[3] = BlockFinalLightNoCheck(x-1,y,z-1) ;
                }

                if(light[4] == -1)
                {
                    light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                }

                if(light[11] == -1)
                {
                    light[11] = BlockFinalLightNoCheck(x+1,y,z-1) ;
                }

                if(light[12] == -1)
                {
                    light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                }

                if(light[16] == -1)
                {
                    light[16] = BlockFinalLightNoCheck(x,y-1,z-1) ;
                }

                if(light[7] == -1)
                {
                    light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                }

                if(light[15] == -1)
                {
                    light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                }
            }

            float light1noV = (light[19]+light[3]+light[4]+mainLight)/4.0f;
            float light2noV = (light[19]+light[11]+light[12]+mainLight)/4.0f;
            float light3noV = (light[16]+light[3]+light[7]+mainLight)/4.0f;
            float light4noV = (light[16]+light[11]+light[15]+mainLight)/4.0f;

            float lightFactor1 = light1noV * lightShadowFactor;
            float lightFactor2 = light2noV * lightShadowFactor;
            float lightFactor3 = light3noV * lightShadowFactor;
            float lightFactor4 = light4noV * lightShadowFactor;

            if(onCorners)
            {
                if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1))
                {
                    light1noV-=lightFactor1;
                }

                if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y+1,z-1))
                {
                    light2noV-=lightFactor2;
                }
                if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y-1,z-1))
                {
                    light3noV-=lightFactor3;
                }

                if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1))
                {
                    light4noV-=lightFactor4;
                }
            }
            else
            {
                if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1))
                {
                    light1noV-=lightFactor1;
                }

                if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1))
                {
                    light2noV-=lightFactor2;
                }
                if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1))
                {
                    light3noV-=lightFactor3;
                }

                if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1))
                {
                    light4noV-=lightFactor4;
                }
            }

            light1 = Vector3(light3noV,light3noV,light3noV)*blockZConstLight;
            light2 = Vector3(light1noV,light1noV,light1noV)*blockZConstLight;
            light3 = Vector3(light4noV,light4noV,light4noV)*blockZConstLight;
            light4 = Vector3(light2noV,light2noV,light2noV)*blockZConstLight;
        }

        left = percent*blockType->botPlane_x;
        right = percent*blockType->botPlane_x+percent;

        up = percent*blockType->botPlane_y;
        down = percent*blockType->botPlane_y+percent;

        if(side != -1)
        {
            int dif = Block - FurnaceOff::getID();

            if(side == 3)
            {
                left = percent*(12+dif);
                right = percent*(12+dif)+percent;

                up = percent*(2+dif);
                down = percent*(2+dif)+percent;
            }
        }

        MeshChunk->vert(x, y+1, z, right, up, light2.x,light2.y,light2.z);
        MeshChunk->vert(x+1, y+1, z, left, up, light4.x,light4.y,light4.z);
        MeshChunk->vert(x+1, y, z, left, down, light3.x,light3.y,light3.z);
        MeshChunk->vert(x, y, z, right, down, light1.x,light1.y,light1.z);

        MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
        MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

        iVertex += 4;
    }


    Vector3 BlockColorz2;
    canCreate = false;
    onCorners == true ? Block1 = _GetBlock(x,y,z+1) : Block1 = _GetBlockNoCheck(x,y,z+1);
    blockType1 = &blockTypes[Block1];

    transparentBlock = blockType1->transparent;
    specialBlock = blockType1->blockModel;
    if(transparent == true)
    {
        if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (specialBlock > 0 && Block1 != 4 && Block1 != 112))
        {
            canCreate = true;
        }
        if (transparentBlock == false && specialBlock == 0)
        {
            canCreate = false;
        }
    }
    else
    {
        if (specialBlock > 0 && Block1 != Block)
        {
            canCreate = true;
        }
        if (transparentBlock == true || Block1 == 0)
        {
            canCreate = true;
        }
    }

    if(mainOptions.fastRendering)
    {
        if(Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID())
        {
            canCreate = false;
        }
    }

    if (canCreate == true)
    {
        float mainLight = BlockFinalLight(x,y,z+1);
        BlockColorz2 = Vector3(mainLight,mainLight,mainLight) * blockZConstLight;
        light1 = light2 = light3 = light4 = BlockColorz2;

        if(mainOptions.smoothLighting == true)
        {
            if(onCorners)
            {
                if(light[18] == -1)
                {
                    light[18] = BlockFinalLight(x,y+1,z+1) ;
                }

                if(light[1] == -1)
                {
                    light[1] = BlockFinalLight(x-1,y,z+1) ;
                }

                if(light[2] == -1)
                {
                    light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                }

                if(light[9] == -1)
                {
                    light[9] = BlockFinalLight(x+1,y,z+1) ;
                }

                if(light[10] == -1)
                {
                    light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                }

                if(light[17] == -1)
                {
                    light[17] = BlockFinalLight(x,y-1,z+1) ;
                }

                if(light[6] == -1)
                {
                    light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                }

                if(light[14] == -1)
                {
                    light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                }
            }
            else
            {
                if(light[18] == -1)
                {
                    light[18] = BlockFinalLightNoCheck(x,y+1,z+1) ;
                }

                if(light[1] == -1)
                {
                    light[1] = BlockFinalLightNoCheck(x-1,y,z+1) ;
                }

                if(light[2] == -1)
                {
                    light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                }

                if(light[9] == -1)
                {
                    light[9] = BlockFinalLightNoCheck(x+1,y,z+1) ;
                }

                if(light[10] == -1)
                {
                    light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                }

                if(light[17] == -1)
                {
                    light[17] = BlockFinalLightNoCheck(x,y-1,z+1) ;
                }

                if(light[6] == -1)
                {
                    light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                }

                if(light[14] == -1)
                {
                    light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                }
            }

            float light1noV = (light[18]+light[1]+light[2]+mainLight)/4.0f;
            float light2noV = (light[18]+light[9]+light[10]+mainLight)/4.0f;
            float light3noV = (light[17]+light[1]+light[6]+mainLight)/4.0f;
            float light4noV = (light[17]+light[9]+light[14]+mainLight)/4.0f;

            float lightFactor1 = light1noV * lightShadowFactor;
            float lightFactor2 = light2noV * lightShadowFactor;
            float lightFactor3 = light3noV * lightShadowFactor;
            float lightFactor4 = light4noV * lightShadowFactor;

            if(onCorners)
            {
                if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1))
                {
                    light1noV-=lightFactor1;
                }
                if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1))
                {
                    light2noV-=lightFactor2;
                }
                if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1))
                {
                    light3noV-=lightFactor3;
                }

                if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y-1,z+1))
                {
                    light4noV-=lightFactor4;
                }
            }
            else
            {
                if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1))
                {
                    light1noV-=lightFactor1;
                }
                if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1))
                {
                    light2noV-=lightFactor2;
                }
                if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1))
                {
                    light3noV-=lightFactor3;
                }

                if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1))
                {
                    light4noV-=lightFactor4;
                }
            }

            light1 = Vector3(light3noV,light3noV,light3noV)*blockZConstLight;
            light2 = Vector3(light1noV,light1noV,light1noV)*blockZConstLight;
            light3 = Vector3(light4noV,light4noV,light4noV)*blockZConstLight;
            light4 = Vector3(light2noV,light2noV,light2noV)*blockZConstLight;
        }

        left = percent*blockType->forPlane_x;
        right = percent*blockType->forPlane_x+percent;

        up = percent*blockType->forPlane_y;
        down = percent*blockType->forPlane_y+percent;

        if(side != -1)
        {
            int dif = Block - FurnaceOff::getID();

            if(side == 0)
            {
                left = percent*(12+dif);
                right = percent*(12+dif)+percent;

                up = percent*(2+dif);
                down = percent*(2+dif)+percent;
            }
        }

        MeshChunk->vert(x, y, z+1, left, down, light1.x,light1.y,light1.z);
        MeshChunk->vert(x+1, y, z+1, right, down, light3.x,light3.y,light3.z);
        MeshChunk->vert(x+1, y+1, z+1, right, up, light4.x,light4.y,light4.z);
        MeshChunk->vert(x, y+1, z+1, left, up, light2.x,light2.y,light2.z);

        MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
        MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

        iVertex += 4;
    }
}

void CraftWorld::UpdateLightAreaIn(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    int current = 0;

    int z_start, z_end, x_start, x_end, y_start, y_end;
    z-9 >= 0 ? z_start = z-9 : z_start = 0;
    z+9 < WORLD_SIZE ? z_end = z+9 : z_end = WORLD_SIZE-1;
    x-9 >= 0 ? x_start = x-9 : x_start = 0;
    x+9 < WORLD_SIZE ? x_end = x+9 : x_end = WORLD_SIZE-1;
    y-9 >= 0 ? y_start = y-9 : y_start = 0;
    y+9 < WORLD_HEIGHT ? y_end = y+9 : y_end = WORLD_HEIGHT-1;

    for(int zz = z_start; zz <= z_end; zz++)
    {
        for(int xx = x_start; xx <= x_end; xx++)
        {
            for(int yy = y_start; yy <= y_end; yy++)
            {
                if(LightSourceBlock(GetBlock(xx,yy,zz)) == true)
				{
                    RemoveLigtSourceAtPosition(xx,yy,zz,Torch::getID());
				    SetLigtSourcePosition(xx,yy,zz,Torch::getID());
				}
            }
        }
    }

    /*z-20 >= 0 ? z_start = z-20 : z_start = 0;
    z+20 < WORLD_SIZE ? z_end = z+20 : z_end = WORLD_SIZE-1;
    x-20 >= 0 ? x_start = x-20 : x_start = 0;
    x+20 < WORLD_SIZE ? x_end = x+20 : x_end = WORLD_SIZE-1;
    y-20 >= 0 ? y_start = y-20 : y_start = 0;
    y+20 < WORLD_HEIGHT ? y_end = y+20 : y_end = WORLD_HEIGHT-1;

    for(int zz = z_start; zz <= z_end; zz++)
    {
        for(int xx = x_start; xx <= x_end; xx++)
        {
            for(int yy = y_start; yy <= y_end; yy++)
            {
                if(LightSourceBlock(GetBlock(xx,yy,zz)) == true)
				{

				}
            }
        }
    }*/
}

void CraftWorld::AddChunkToUpdate(int id)
{
    if(id > 0 && id < mChunks.size())
    {
        bool push = true;
        if(toUpdate.empty() == false)
        {
            for(int i = toUpdate.size()-1; i >= 0; i--)
            {
                if(toUpdate[i] == id)
                {
                    push = false;
                    break;
                }
            }
        }
        if(push)
        {
            toUpdate.push_back(id);
        }
    }
}

void CraftWorld::AddChunkToFastUpdate(int id)
{
    if(id >= 0 && id < mChunks.size())
    {
        bool push = true;
        if(toFastUpdate.empty() == false)
        {
            for(std::vector<int>::size_type i = 0; i < toFastUpdate.size(); i++)
            {
                if(toFastUpdate[i] == id)
                {
                    push = false;
                    break;
                }
            }
        }

        if(push)
        {
            toFastUpdate.push_back(id);
        }
    }
}


void CraftWorld::rebuildNearestChunksForLight(int id,Vector3 pos)
{
    Vector3 test = mChunks[id]->bBox.onBorder(pos);
    Vector3 temp = pos;
    Vector3 posInChunk = pos-Vector3(mChunks[id]->chunkStartX,mChunks[id]->chunkStartY,mChunks[id]->chunkStartZ);

    int chunkNumber = -1;
    ///X
    if(posInChunk.x < 10)
    {
        chunkNumber = getChunkId(Vector3(temp.x-CHUNK_SIZE/2,temp.y,temp.z));
        if(chunkNumber != -1)
        {
            AddChunkToUpdate(chunkNumber);
        }

        // diagonale
        if(posInChunk.z < 10)
        {
            chunkNumber = getChunkId(Vector3(temp.x-CHUNK_SIZE/2,temp.y,temp.z-CHUNK_SIZE/2));
            if(chunkNumber != -1)
            {
                AddChunkToUpdate(chunkNumber);
            }

            if(posInChunk.y < 10)
            {
                chunkNumber = getChunkId(Vector3(temp.x-CHUNK_SIZE/2,temp.y-CHUNK_SIZE/2,temp.z-CHUNK_SIZE/2));
                if(chunkNumber != -1)
                {
                    AddChunkToUpdate(chunkNumber);
                }
            }
            if(posInChunk.y > CHUNK_SIZE-10)
            {
                chunkNumber = getChunkId(Vector3(temp.x-CHUNK_SIZE/2,temp.y+CHUNK_SIZE/2,temp.z-CHUNK_SIZE/2));
                if(chunkNumber != -1)
                {
                    AddChunkToUpdate(chunkNumber);
                }
            }
        }
        if(posInChunk.z > CHUNK_SIZE-10)
        {
            chunkNumber = getChunkId(Vector3(temp.x-CHUNK_SIZE/2,temp.y,temp.z+CHUNK_SIZE/2));
            if(chunkNumber != -1)
            {
                AddChunkToUpdate(chunkNumber);
            }

            if(posInChunk.y < 10)
            {
                chunkNumber = getChunkId(Vector3(temp.x-CHUNK_SIZE/2,temp.y-CHUNK_SIZE/2,temp.z+CHUNK_SIZE/2));
                if(chunkNumber != -1)
                {
                    AddChunkToUpdate(chunkNumber);
                }
            }
            if(posInChunk.y > CHUNK_SIZE-10)
            {
                chunkNumber = getChunkId(Vector3(temp.x-CHUNK_SIZE/2,temp.y+CHUNK_SIZE/2,temp.z+CHUNK_SIZE/2));
                if(chunkNumber != -1)
                {
                    AddChunkToUpdate(chunkNumber);
                }
            }
        }
    }
    if(posInChunk.x > CHUNK_SIZE-10)
    {
        chunkNumber = getChunkId(Vector3(temp.x+CHUNK_SIZE/2,temp.y,temp.z));
        if(chunkNumber != -1)
        {
            AddChunkToUpdate(chunkNumber);
        }

        // diagonale
        if(posInChunk.z < 10)
        {
            chunkNumber = getChunkId(Vector3(temp.x+CHUNK_SIZE/2,temp.y,temp.z-CHUNK_SIZE/2));
            if(chunkNumber != -1)
            {
                AddChunkToUpdate(chunkNumber);
            }

            if(posInChunk.y < 10)
            {
                chunkNumber = getChunkId(Vector3(temp.x+CHUNK_SIZE/2,temp.y-CHUNK_SIZE/2,temp.z-CHUNK_SIZE/2));
                if(chunkNumber != -1)
                {
                    AddChunkToUpdate(chunkNumber);
                }
            }
            if(posInChunk.y > CHUNK_SIZE-10)
            {
                chunkNumber = getChunkId(Vector3(temp.x+CHUNK_SIZE/2,temp.y+CHUNK_SIZE/2,temp.z-CHUNK_SIZE/2));
                if(chunkNumber != -1)
                {
                    AddChunkToUpdate(chunkNumber);
                }
            }
        }
        if(posInChunk.z > CHUNK_SIZE-10)
        {
            chunkNumber = getChunkId(Vector3(temp.x+CHUNK_SIZE/2,temp.y,temp.z+CHUNK_SIZE/2));
            if(chunkNumber != -1)
            {
                AddChunkToUpdate(chunkNumber);
            }

            if(posInChunk.y < 10)
            {
                chunkNumber = getChunkId(Vector3(temp.x+CHUNK_SIZE/2,temp.y-CHUNK_SIZE/2,temp.z+CHUNK_SIZE/2));
                if(chunkNumber != -1)
                {
                    AddChunkToUpdate(chunkNumber);
                }
            }
            if(posInChunk.y > CHUNK_SIZE-10)
            {
                chunkNumber = getChunkId(Vector3(temp.x+CHUNK_SIZE/2,temp.y+CHUNK_SIZE/2,temp.z+CHUNK_SIZE/2));
                if(chunkNumber != -1)
                {
                    AddChunkToUpdate(chunkNumber);
                }
            }
        }
    }

    ///Y
    if(posInChunk.y < 10)
    {
        chunkNumber = getChunkId(Vector3(temp.x,temp.y-CHUNK_SIZE/2,temp.z));
        if(chunkNumber != -1)
        {
            AddChunkToUpdate(chunkNumber);
        }
    }
    if(posInChunk.y > CHUNK_SIZE-10)
    {
        chunkNumber = getChunkId(Vector3(temp.x,temp.y+CHUNK_SIZE/2,temp.z));
        if(chunkNumber != -1)
        {
            AddChunkToUpdate(chunkNumber);
        }
    }

    ///Z
    if(posInChunk.z < 10)
    {
        chunkNumber = getChunkId(Vector3(temp.x,temp.y,temp.z-CHUNK_SIZE/2));
        if(chunkNumber != -1)
        {
            AddChunkToUpdate(chunkNumber);
        }
    }
    if(posInChunk.z > CHUNK_SIZE-10)
    {
        chunkNumber = getChunkId(Vector3(temp.x,temp.y,temp.z+CHUNK_SIZE/2));
        if(chunkNumber != -1)
        {
            AddChunkToUpdate(chunkNumber);
        }
    }
}

void CraftWorld::rebuildNearestTransparentChunks(int id,Vector3 pos)
{
    Vector3 test = mChunks[id]->bBox.onBorder(pos);
    Vector3 temp = pos;
    int chunkNumber = -1;

    if(test.x != 0)
    {
        temp.x +=test.x;

        chunkNumber = getChunkId(temp);
        if(chunkNumber != -1)
        {
            RebuildTransparentMeshChunk(chunkNumber);
        }

        //now check for other with this... //needed by shadows
        if(test.y != 0)
        {
            temp.y +=test.y;
            chunkNumber = getChunkId(temp);
            if(chunkNumber != -1)
            {
                RebuildTransparentMeshChunk(chunkNumber);
            }
        }

        if(test.z != 0)
        {
            temp.z +=test.z;
            chunkNumber = getChunkId(temp);
            if(chunkNumber != -1)
            {
                RebuildTransparentMeshChunk(chunkNumber);
            }
        }
    }


    if(test.y != 0)
    {
        temp = pos;
        temp.y +=test.y;

        chunkNumber = getChunkId(temp);
        if(chunkNumber != -1)
        {
            RebuildTransparentMeshChunk(chunkNumber);
        }

        if(test.z != 0)
        {
            temp.z +=test.z;
            chunkNumber = getChunkId(temp);
            if(chunkNumber != -1)
            {
                RebuildTransparentMeshChunk(chunkNumber);
            }
        }
    }

    if(test.z != 0)
    {
        temp = pos;
        temp.z +=test.z;

        chunkNumber = getChunkId(temp);
        if(chunkNumber != -1)
        {
            RebuildTransparentMeshChunk(chunkNumber);
        }

        if(test.x != 0)
        {
            temp.x +=test.x;

            chunkNumber = getChunkId(temp);
            if(chunkNumber != -1)
            {
                RebuildTransparentMeshChunk(chunkNumber);
            }
        }
    }
}

void CraftWorld::rebuildNearestChunks(int id,Vector3 pos)
{
    Vector3 test = mChunks[id]->bBox.onBorder(pos);
    Vector3 temp = pos;
    int chunkNumber = -1;

    if(test.x != 0)
    {
        temp.x +=test.x;

        chunkNumber = getChunkId(temp);
        if(chunkNumber != -1)
        {
            RebuildFullMeshChunk(chunkNumber);
        }


        //now check for other with this... //needed by shadows
        if(test.y != 0)
        {
            temp.y +=test.y;
            chunkNumber = getChunkId(temp);
            if(chunkNumber != -1)
            {
                RebuildFullMeshChunk(chunkNumber);
            }
        }

        if(test.z != 0)
        {
            temp.z +=test.z;
            chunkNumber = getChunkId(temp);
            if(chunkNumber != -1)
            {
                RebuildFullMeshChunk(chunkNumber);
            }
        }
    }


    if(test.y != 0)
    {
        temp = pos;
        temp.y +=test.y;

        chunkNumber = getChunkId(temp);
        if(chunkNumber != -1)
        {
            RebuildFullMeshChunk(chunkNumber);
        }

        if(test.z != 0)
        {
            temp.z +=test.z;
            chunkNumber = getChunkId(temp);
            if(chunkNumber != -1)
            {
                RebuildFullMeshChunk(chunkNumber);
            }
        }
    }

    if(test.z != 0)
    {
        temp = pos;
        temp.z +=test.z;

        chunkNumber = getChunkId(temp);
        if(chunkNumber != -1)
        {
            RebuildFullMeshChunk(chunkNumber);
        }

        if(test.x != 0)
        {
            temp.x +=test.x;

            chunkNumber = getChunkId(temp);
            if(chunkNumber != -1)
            {
                RebuildFullMeshChunk(chunkNumber);
            }
        }
    }
}

void CraftWorld::createWorldChunkss()
{
    createdChunksCount = 0;

    for (int z = 0; z < WORLD_SIZE; z += CHUNK_SIZE)
    {
        for (int y = 0; y < WORLD_HEIGHT; y += CHUNK_SIZE)
        {
            for (int x = 0; x < WORLD_SIZE; x += CHUNK_SIZE)
            {
                CreateFullMeshChunk(x,y,z);
            }
        }
    }
}

void CraftWorld::UpdateChunkBlocks(int id)
{
    SimpleMeshChunk* MeshChunk = mChunks[id];

    int StartZ = MeshChunk->chunkStartZ;
    int StartY = MeshChunk->chunkStartY;
    int StartX = MeshChunk->chunkStartX;

    for (int z = StartZ; z < CHUNK_SIZE + StartZ; ++z)
    {
        for (int y = StartY; y < CHUNK_SIZE + StartY; ++y)
        {
            for (int x = StartX; x < CHUNK_SIZE + StartX; ++x)
            {
                block_t& block = GetBlock(x,y,z);
                /*if (BlockUpdate2(x,y,z) == 0)
                {
                    continue;
                } */
                if(block == 0)
                {
                    continue;
                }

                if(worldWeather->itIsSnowing && worldWeather->GetWeatherType() == 1 && y != CHUNK_SIZE + StartY-1)
                {
                    if(rand() % 20 < 3)
                    {
                        if(blockTypes[block].blockModel == 0) // snow can be laid only on normal blocks
                        {
                            if(GetBlockNoCheck(x,y+1,z) == 0)
                            {
                                if(GetShadowLevel(x,y+1,z) == 0)
                                {
                                   GetBlock(x,y+1,z) = Snow2::getID();
                                }
                            }
                        }
                    }
                }

                if(block == DirtBlock::getID() && GetShadowLevel(x,y,z) == 0)
                {
                    if(rand() % 6 == 0)
                    {
                        if(BlockTransparentOrSpecial(x,y+1,z) == true)
                        {
                            block = GrassBlock::getID();
                            if(block == Snow2::getID())
                            {
                                block = SnowSoil::getID();
                            }
                            continue;
                        }
                    }
                }
                if(block == GrassBlock::getID() && (GetShadowLevel(x,y,z) > 6 || BlockTransparentOrSpecial(x,y+1,z) == false))
                {
                    block = DirtBlock::getID();
                    continue;
                }

                /*if(block == Snow2::getID() && y != StartY)
                {
                    if(GetLightLevel(x,y,z) >= 9)
                    {
                        block = 0;
                    }
                }*/

                if(block == CaneBlock::getID() && GetBlock(x,y+1,z) == 0 && GetBlock(x,y-3,z) != 33 && GetBlock(x,y-3,z) != 0)
                {
                    if(rand() % 100 > 92)
                    {
                        GetBlock(x,y+1,z) = 33;
                        continue;
                    }
                }

                int upperBlock = GetBlock(x,y+1,z);
                if(block == Soil::getID())
                {
                    if(BlockTransparent(x,y+1,z) == false)
                    {
                        if(rand() % 5 == 0)
                        {
                            block = DirtBlock::getID();
                        }
                    }
                    continue;
                }
                if(block == WateredSoil::getID())
                {
                    if(BlockTransparent(x,y+1,z) == false)
                    {
                        if(rand() % 5 == 0)
                        {
                            block = DirtBlock::getID();
                        }
                    }
                    continue;
                }

                if(block == Mooshroom1::getID())
                {
                    if(BlockFinalLight(x,y,z) > 0.5f) // Mooshrooms can grow only in dark places
                        return;

                    if(rand() % 30 == 0)
                    {
                        if(rand() % 3 == 0)
                        {
                            if(BlockSolid(x+1,y-1,z) == true && GetBlock(x+1,y,z) == 0)
                            {
                                GetBlock(x+1,y,z) = Mooshroom1::getID();
                            }
                        }
                        if(rand() % 3 == 0)
                        {
                            if(BlockSolid(x-1,y-1,z) == true && GetBlock(x-1,y,z) == 0)
                            {
                                GetBlock(x-1,y,z) = Mooshroom1::getID();
                            }
                        }
                        if(rand() % 3 == 0)
                        {
                            if(BlockSolid(x,y-1,z-1) == true && GetBlock(x,y,z-1) == 0)
                            {
                                GetBlock(x,y,z-1) = Mooshroom1::getID();
                            }
                        }
                        if(rand() % 3 == 0)
                        {
                            if(BlockSolid(x,y-1,z+1) == true && GetBlock(x,y,z+1) == 0)
                            {
                                GetBlock(x,y,z+1) = Mooshroom1::getID();
                            }
                        }
                    }
                    continue;
                }
                if(block == Mooshroom2::getID())
                {
                    if(BlockFinalLight(x,y,z) > 0.5f) // Mooshrooms can grow only in dark places
                        return;

                    if(rand() % 30 == 0)
                    {
                        if(rand() % 3 == 0)
                        {
                            if(BlockSolid(x+1,y-1,z) == true && GetBlock(x+1,y,z) == 0)
                            {
                                GetBlock(x+1,y,z) = Mooshroom2::getID();
                            }
                        }
                        if(rand() % 3 == 0)
                        {
                            if(BlockSolid(x-1,y-1,z) == true && GetBlock(x-1,y,z) == 0)
                            {
                                GetBlock(x-1,y,z) = Mooshroom2::getID();
                            }
                        }
                        if(rand() % 3 == 0)
                        {
                            if(BlockSolid(x,y-1,z-1) == true && GetBlock(x,y,z-1) == 0)
                            {
                                GetBlock(x,y,z-1) = Mooshroom2::getID();
                            }
                        }
                        if(rand() % 3 == 0)
                        {
                            if(BlockSolid(x,y-1,z+1) == true && GetBlock(x,y,z+1) == 0)
                            {
                                GetBlock(x,y,z+1) = Mooshroom2::getID();
                            }
                        }
                    }
                    continue;
                }

                if(block == Lava::getID() && GetBlock(x+2,y,z) == WaterBlock::getID() && GetBlock(x+1,y,z) == 0)
                {
                    GetBlock(x+1,y,z) = CobbleStone::getID(); //make coblestone
                    continue;
                }
                if(block == Lava::getID() && GetBlock(x-2,y,z) == WaterBlock::getID() && GetBlock(x-1,y,z) == 0)
                {
                    GetBlock(x-1,y,z) = CobbleStone::getID();
                    continue;
                }

                if(block == Lava::getID() && GetBlock(x,y,z+2) == WaterBlock::getID() && GetBlock(x,y,z+1) == 0)
                {
                    GetBlock(x,y,z+1) = Obsidian::getID(); //make obsidian
                    continue;
                }
                if(block == Lava::getID() && GetBlock(x,y,z-2) == WaterBlock::getID() && GetBlock(x,y,z-1) == 0)
                {
                    GetBlock(x,y,z-1) = Obsidian::getID();
                    continue;
                }
            }
        }
    }
}


void CraftWorld::UpdateChunkGrowingBlocks(int id, int vectorId)
{
    SimpleMeshChunk* MeshChunk = mChunks[id];

    int StartZ = MeshChunk->chunkStartZ;
    int StartY = MeshChunk->chunkStartY;
    int StartX = MeshChunk->chunkStartX;

    bool somethingWasUpdated = false;
    bool waterFoundNearBy = false;

    int water_x, water_y, water_z;
    water_x = -1;
    water_y = -1;
    water_z = -1;

    for (int z = StartZ; z < CHUNK_SIZE + StartZ; ++z)
    {
        for (int y = StartY; y < CHUNK_SIZE + StartY; ++y)
        {
            for (int x = StartX; x < CHUNK_SIZE + StartX; ++x)
            {
                if (BlockUpdate2(x,y,z) == 0)
                {
                    continue;
                }

                int block = GetBlock(x,y,z);
                float block_light = BlockFinalLight(x,y,z);

                if(block == 0)
                {
                    continue;
                }
                if(x > 0 && x < WORLD_SIZE && y > 0 && y < WORLD_SIZE && z > 0 && z < WORLD_SIZE)
                {
                    if(block == Soil::getID() || block == WateredSoil::getID())
                    {
                        if(water_x != -1 && water_z != -1)
                        {
                            BoundingBox hitBox = BoundingBox(Vector3(water_x - 2,water_y - 1,water_z - 2),Vector3(water_x + 2,water_y + 1,water_z + 2));
                            if(hitBox.contains(Vector3(x,y,z)) == false)
                            {
                                waterFoundNearBy = false;
                            }
                        }

                        if(waterFoundNearBy == false)
                        {
                            for(int x2 = x - 2; x2 <= x+2; x2++)
                            {
                                if(waterFoundNearBy)
                                {
                                    break;
                                }
                                for(int z2 = z - 2; z2 <= z+2; z2++)
                                {
                                    if(GetBlock(x2,y,z2) == WaterBlock::getID())
                                    {
                                        waterFoundNearBy = true;
                                        water_x = x2;
                                        water_y = y;
                                        water_z = z2;
                                    }
                                    if(waterFoundNearBy)
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if(block == Soil::getID())
                    {
                        somethingWasUpdated = true;
                        if(!worldWeather->itIsRaining) // if it is clear weather
                        {
                            if(waterFoundNearBy)
                            {
                                if(rand() % 10 > 3)
                                {
                                    GetBlock(x,y,z) = WateredSoil::getID();
                                }
                            }
                            continue;
                        }
                        else
                        {
                            if(rand() % 10 > 3)
                            {
                                GetBlock(x,y,z) = WateredSoil::getID();
                            }
                        }
                    }
                    if(block == WateredSoil::getID())
                    {
                        somethingWasUpdated = true;

                        if(!waterFoundNearBy)
                        {
                            if(rand() % 10 > 3)
                            {
                                GetBlock(x,y,z) = Soil::getID();
                            }
                        }
                        continue;
                    }

                    int upperBlock = GetBlock(x,y+1,z);
                    if(block == Soil::getID())
                    {
                        if(BlockTransparent(x,y+1,z) == false)
                        {
                            if(rand() % 5 == 0)
                            {
                                GetBlock(x,y,z) = DirtBlock::getID();
                            }
                        }
                        continue;
                    }
                    if(block == WateredSoil::getID())
                    {
                        if(BlockTransparent(x,y+1,z) == false)
                        {
                            if(rand() % 5 == 0)
                            {
                                GetBlock(x,y,z) = DirtBlock::getID();
                            }
                        }
                        continue;
                    }

                    // pumpkin plants
                    if(block >= PumpkinPlant1::getID() && block <= PumpkinPlant3::getID())
                    {
                        somethingWasUpdated = true;
                        if(GetBlock(x,y-1,z) == Soil::getID())
                        {
                            if(rand() % 100 <= 10)
                            {
                                GetBlock(x,y,z) = block+1;
                                continue;
                            }
                        }
                        if(GetBlock(x,y-1,z) == WateredSoil::getID())
                        {
                            if(rand() % 100 <= 14)
                            {
                                GetBlock(x,y,z) = block+1;
                                continue;
                            }
                        }
                    }

                    if(block == PumpkinPlant4::getID())
                    {
                        somethingWasUpdated = true;
                        bool canGrow = true;
                        if(GetBlock(x-1,y,z) == Pumpkin1::getID() || GetBlock(x+1,y,z) == Pumpkin1::getID() || GetBlock(x,y,z-1) == Pumpkin1::getID() || GetBlock(x,y,z+1) == Pumpkin1::getID())
                        {
                            canGrow = false;
                        }
                        if(rand() % 7 == 0 && canGrow)
                        {
                            switch(rand() % 4)
                            {
                            case 0:
                                if(GetBlock(x-1,y,z) == 0)
                                {
                                    GetBlock(x-1,y,z) = Pumpkin1::getID();
                                }
                            break;
                            case 1:
                                if(GetBlock(x+1,y,z) == 0)
                                {
                                    GetBlock(x+1,y,z) = Pumpkin1::getID();
                                }
                            break;
                            case 2:
                                if(GetBlock(x,y,z-1) == 0)
                                {
                                    GetBlock(x,y,z-1) = Pumpkin1::getID();
                                }
                            break;
                            case 3:
                                if(GetBlock(x,y,z+1) == 0)
                                {
                                    GetBlock(x,y,z+1) = Pumpkin1::getID();
                                }
                            break;
                            }
                        }
                        continue;
                    }


                    // melon plants
                    if(block == MelonPlant1::getID() && block <= MelonPlant3::getID())
                    {
                        somethingWasUpdated = true;
                        if(GetBlock(x,y-1,z) == Soil::getID())
                        {
                            if(rand() % 100 <= 10)
                            {
                                GetBlock(x,y,z) = block+1;
                                continue;
                            }
                        }
                        if(GetBlock(x,y-1,z) == WateredSoil::getID())
                        {
                            if(rand() % 100 <= 14)
                            {
                                GetBlock(x,y,z) = block+1;
                                continue;
                            }
                        }
                    }

                    if(block == MelonPlant4::getID())
                    {
                        somethingWasUpdated = true;
                        bool canGrow = true;
                        if(GetBlock(x-1,y,z) == MelonBlock::getID() || GetBlock(x+1,y,z) == MelonBlock::getID() || GetBlock(x,y,z-1) == MelonBlock::getID() || GetBlock(x,y,z+1) == MelonBlock::getID())
                        {
                            canGrow = false;
                        }
                        if(rand() % 7 == 0 && canGrow)
                        {
                            switch(rand() % 4)
                            {
                            case 0:
                                if(GetBlock(x-1,y,z) == 0)
                                {
                                    GetBlock(x-1,y,z) = MelonBlock::getID();
                                }
                            break;
                            case 1:
                                if(GetBlock(x+1,y,z) == 0)
                                {
                                    GetBlock(x+1,y,z) = MelonBlock::getID();
                                }
                            break;
                            case 2:
                                if(GetBlock(x,y,z-1) == 0)
                                {
                                    GetBlock(x,y,z-1) = MelonBlock::getID();
                                }
                            break;
                            case 3:
                                if(GetBlock(x,y,z+1) == 0)
                                {
                                    GetBlock(x,y,z+1) = MelonBlock::getID();
                                }
                            break;
                            }
                        }
                        continue;
                    }

                    // wheat
                    if(block >= WheatBlock1::getID() && block <= WheatBlock5::getID())
                    {
                        somethingWasUpdated = true;
                        if(block_light >= 0.33f)
                        {
                            if(GetBlock(x,y-1,z) == Soil::getID())
                            {
                                if(rand() % 100 > 86)
                                {
                                    if(block == WheatBlock5::getID())
                                    {
                                        mainStatistics.cropsGrowned += 1;
                                    }
                                    GetBlock(x,y,z) = block+1;
                                    continue;
                                }
                            }
                            if(GetBlock(x,y-1,z) == WateredSoil::getID())
                            {
                                if(rand() % 100 > 78)
                                {
                                    if(block == WheatBlock5::getID())
                                    {
                                        mainStatistics.cropsGrowned += 1;
                                    }
                                    GetBlock(x,y,z) = block+1;
                                    continue;
                                }
                            }
                        }
                        continue;
                    }

                    // potato
                    if(block >= PotatoPlant1::getID() && block <= PotatoPlant3::getID())
                    {
                        somethingWasUpdated = true;
                        if(block_light >= 0.33f)
                        {
                            if(GetBlock(x,y-1,z) == Soil::getID())
                            {
                                if(rand() % 100 > 93)
                                {
                                    if(block == PotatoPlant3::getID())
                                    {
                                        mainStatistics.cropsGrowned += 1;
                                    }
                                    GetBlock(x,y,z) = block+1;
                                    continue;
                                }
                            }
                            if(GetBlock(x,y-1,z) == WateredSoil::getID())
                            {
                                if(rand() % 100 > 87)
                                {
                                    if(block == PotatoPlant3::getID())
                                    {
                                        mainStatistics.cropsGrowned += 1;
                                    }
                                    GetBlock(x,y,z) = block+1;
                                    continue;
                                }
                            }
                        }
                        continue;
                    }

                    // carrot
                    if(block >= CarrotPlant1::getID() && block <= CarrotPlant3::getID())
                    {
                        somethingWasUpdated = true;
                        if(block_light >= 0.33f)
                        {
                            if(GetBlock(x,y-1,z) == Soil::getID())
                            {
                                if(rand() % 100 > 92)
                                {
                                    if(block == CarrotPlant3::getID())
                                    {
                                        mainStatistics.cropsGrowned += 1;
                                    }
                                    GetBlock(x,y,z) = block+1;
                                    continue;
                                }
                            }
                            if(GetBlock(x,y-1,z) == WateredSoil::getID())
                            {
                                if(rand() % 100 > 86)
                                {
                                    if(block == CarrotPlant3::getID())
                                    {
                                        mainStatistics.cropsGrowned += 1;
                                    }
                                    GetBlock(x,y,z) = block+1;
                                    continue;
                                }
                            }
                        }
                        continue;
                    }

                    if(block == Mooshroom1::getID())
                    {
                        if(block_light > 0.5f) // Mooshrooms can grow only in dark places
                            return;

                        if(rand() % 30 == 0)
                        {
                            if(rand() % 3 == 0)
                            {
                                if(BlockSolid(x+1,y-1,z) == true && GetBlock(x+1,y,z) == 0)
                                {
                                    GetBlock(x+1,y,z) = Mooshroom1::getID();
                                }
                            }
                            if(rand() % 3 == 0)
                            {
                                if(BlockSolid(x-1,y-1,z) == true && GetBlock(x-1,y,z) == 0)
                                {
                                    GetBlock(x-1,y,z) = Mooshroom1::getID();
                                }
                            }
                            if(rand() % 3 == 0)
                            {
                                if(BlockSolid(x,y-1,z-1) == true && GetBlock(x,y,z-1) == 0)
                                {
                                    GetBlock(x,y,z-1) = Mooshroom1::getID();
                                }
                            }
                            if(rand() % 3 == 0)
                            {
                                if(BlockSolid(x,y-1,z+1) == true && GetBlock(x,y,z+1) == 0)
                                {
                                    GetBlock(x,y,z+1) = Mooshroom1::getID();
                                }
                            }
                        }
                        continue;
                    }
                    if(block == Mooshroom2::getID())
                    {
                        if(block_light > 0.5f) // Mooshrooms can grow only in dark places
                            return;

                        if(rand() % 30 == 0)
                        {
                            if(rand() % 3 == 0)
                            {
                                if(BlockSolid(x+1,y-1,z) == true && GetBlock(x+1,y,z) == 0)
                                {
                                    GetBlock(x+1,y,z) = Mooshroom2::getID();
                                }
                            }
                            if(rand() % 3 == 0)
                            {
                                if(BlockSolid(x-1,y-1,z) == true && GetBlock(x-1,y,z) == 0)
                                {
                                    GetBlock(x-1,y,z) = Mooshroom2::getID();
                                }
                            }
                            if(rand() % 3 == 0)
                            {
                                if(BlockSolid(x,y-1,z-1) == true && GetBlock(x,y,z-1) == 0)
                                {
                                    GetBlock(x,y,z-1) = Mooshroom2::getID();
                                }
                            }
                            if(rand() % 3 == 0)
                            {
                                if(BlockSolid(x,y-1,z+1) == true && GetBlock(x,y,z+1) == 0)
                                {
                                    GetBlock(x,y,z+1) = Mooshroom2::getID();
                                }
                            }
                        }
                        continue;
                    }

                    if(block == CaneBlock::getID() && GetBlock(x,y+1,z) == 0 && GetBlock(x,y-3,z) != 33 && GetBlock(x,y-3,z) != 0)
                    {
                        somethingWasUpdated = true;
                        if(rand() % 20 < 3)
                        {
                            GetBlock(x,y+1,z) = 33;
                        }
                        continue;
                    }

                    /// oak tree growing
                    if(block == 121 && (GetBlock(x,y-1,z) == 1 || GetBlock(x,y-1,z) == 2))
                    {
                        somethingWasUpdated = true;
                        if((rand() % 20 == 0 && (GetBlock(x,y-1,z) == 1 || GetBlock(x,y-1,z) == 2)) || (rand() % 32 == 0  && GetBlock(x,y-1,z) == 29))
                        {
                            mainStatistics.treesGrowned += 1;
                            int height = 1 + rand() % 2;

                            for(int x2 = x - 2; x2 <= x + 2; x2 ++)
                            {
                                for(int z2 = z - 2; z2 <= z + 2; z2 ++)
                                {
                                    for(int y2 = y+height; y2 <= y+height+1; y2 ++)
                                    {
                                        if(GetBlock(x2,y2,z2) == 0)
                                        {
                                            GetBlock(x2,y2,z2) = 9;
                                        }
                                    }
                                }
                            }

                            for(int x2 = x - 1; x2 <= x + 1; x2 ++)
                            {
                                for(int z2 = z - 1; z2 <= z + 1; z2 ++)
                                {
                                    int y2 = y+height+2;
                                    if(rand() % 4 <= 1)
                                    {
                                        if(GetBlock(x2,y2,z2) == 0)
                                        {
                                            GetBlock(x2,y2,z2) = 9;
                                        }
                                    }
                                }
                            }

                            for(int x2 = x - 1; x2 <= x + 1; x2 ++)
                            {
                                for(int y2 = y+height+2; y2 <= y+height+3; y2 ++)
                                {
                                    if(GetBlock(x2,y2,z) == 0)
                                    {
                                        GetBlock(x2,y2,z) = 9;
                                    }
                                }
                            }

                            for(int z2 = z - 1; z2 <= z + 1; z2 ++)
                            {
                                for(int y2 = y+height+2; y2 <= y+height+3; y2 ++)
                                {
                                    if(GetBlock(x,y2,z2) == 0)
                                    {
                                        GetBlock(x,y2,z2) = 9;
                                    }
                                }
                            }

                            for(int yy1 = y; yy1 <= y+height+2; yy1++)
                            {
                               GetBlock(x,yy1,z) = 8;
                            }
                        }
                    }


                    /// birch tree growing
                    if(block == 123 && (GetBlock(x,y-1,z) == 1 || GetBlock(x,y-1,z) == 2))
                    {
                        somethingWasUpdated = true;
                        if((rand() % 22 == 0 && (GetBlock(x,y-1,z) == 1 || GetBlock(x,y-1,z) == 2)) || (rand() % 40 == 0 && GetBlock(x,y-1,z) == 29))
                        {
                            mainStatistics.treesGrowned += 1;
                            int height = 2 + rand() % 2;

                            for(int x2 = x - 2; x2 <= x + 2; x2 ++)
                            {
                                for(int z2 = z - 2; z2 <= z + 2; z2 ++)
                                {
                                    for(int y2 = y+height; y2 <= y+height+1; y2 ++)
                                    {
                                        if(GetBlock(x2,y2,z2) == 0)
                                        {
                                            GetBlock(x2,y2,z2) = 38;
                                        }
                                    }
                                }
                            }

                            for(int x2 = x - 1; x2 <= x + 1; x2 ++)
                            {
                                for(int z2 = z - 1; z2 <= z + 1; z2 ++)
                                {
                                    int y2 = y+height+2;
                                    if(rand() % 4 <= 1)
                                    {
                                        if(GetBlock(x2,y2,z2) == 0)
                                        {
                                            GetBlock(x2,y2,z2) = 38;
                                        }
                                    }
                                }
                            }

                            for(int x2 = x - 1; x2 <= x + 1; x2 ++)
                            {
                                for(int y2 = y+height+2; y2 <= y+height+3; y2 ++)
                                {
                                    if(GetBlock(x2,y2,z) == 0)
                                    {
                                        GetBlock(x2,y2,z) = 38;
                                    }
                                }
                            }

                            for(int z2 = z - 1; z2 <= z + 1; z2 ++)
                            {
                                for(int y2 = y+height+2; y2 <= y+height+3; y2 ++)
                                {
                                    GetBlock(x,y2,z2) = 38;
                                }
                            }

                            for(int yy1 = y; yy1 <= y+height+2; yy1++)
                            {
                               GetBlock(x,yy1,z) = 31;
                            }
                        }
                    }


                    /// spruce tree growing
                    if(block == 122 && (GetBlock(x,y-1,z) == 1 || GetBlock(x,y-1,z) == 2 || GetBlock(x,y-1,z) == 29))
                    {
                        somethingWasUpdated = true;
                        if((rand() % 22 < 2 && (GetBlock(x,y-1,z) == 1 || GetBlock(x,y-1,z) == 2)) || (rand() % 19 == 0  && GetBlock(x,y-1,z) == 29))
                        {
                            mainStatistics.treesGrowned += 1;

                            y = groundHeight(x,z);
                            int TrunkHeight = 6;
                            int type = 0;

                            //check if there is an water on there or another tree
                            bool flag = 1;

                            for (int y1 = y+1; y1 <= y + TrunkHeight ; y1++)
                            {
                                if(GetBlock(x, y1, z) != 0)
                                {
                                    flag = 0;
                                    break;
                                }
                            }

                            if(flag == 0)continue;

                            if(rand() % 7 == 1)
                            {
                                type = 1;
                                TrunkHeight = 6 + rand() % 3;
                            }
                            else
                            {
                                type = 0;
                            }

                            //Create the tree trunk
                            for (int y1 = y; y1 <= y + TrunkHeight ; y1++)
                            {
                                block_t& Block = GetBlock(x, y1, z);
                                Block = 30;
                            }

                            if(type == 0)
                            {
                                for(int xx = -1; xx <= 1; xx++)
                                {
                                    for(int zz = -2; zz <= 2; zz++)
                                    {
                                        int x1 = xx + x;
                                        int y1 = y + 1;
                                        int z1 = zz + z;

                                        block_t& Block = GetBlock(x1, y1, z1);
                                        if (Block == 0) Block = 37;
                                    }
                                }

                                for(int xx = -2; xx <= 2; xx++)
                                {
                                    for(int zz = -1; zz <= 1; zz++)
                                    {
                                        int x1 = xx + x;
                                        int y1 = y + 1;
                                        int z1 = zz + z;

                                        block_t& Block = GetBlock(x1, y1, z1);
                                        if (Block == 0) Block = 37;
                                    }
                                }

                                for(int xx = -1; xx <= 1; xx++)
                                {
                                    for(int zz = -2; zz <= 2; zz++)
                                    {
                                        int x1 = xx + x;
                                        int y1 = y + 3;
                                        int z1 = zz + z;

                                        block_t& Block = GetBlock(x1, y1, z1);
                                        if (Block == 0) Block = 37;
                                    }
                                }

                                for(int xx = -2; xx <= 2; xx++)
                                {
                                    for(int zz = -1; zz <= 1; zz++)
                                    {
                                        int x1 = xx + x;
                                        int y1 = y + 3;
                                        int z1 = zz + z;

                                        block_t& Block = GetBlock(x1, y1, z1);
                                        if (Block == 0) Block = 37;
                                    }
                                }

                                for(int xx = -1; xx <= 1; xx++)
                                {
                                        int x1 = xx + x;
                                        int y1 = y + 2;
                                        int z1 = z;

                                        block_t& Block = GetBlock(x1, y1, z1);
                                        if (Block == 0) Block = 37;
                                }


                                for(int zz = -1; zz <= 1; zz++)
                                {
                                    int x1 = x;
                                    int y1 = y + 2;
                                    int z1 = zz + z;

                                    block_t& Block = GetBlock(x1, y1, z1);
                                    if (Block == 0) Block = 37;
                                }

                                for(int xx = -1; xx <= 1; xx++)
                                {
                                        int x1 = xx + x;
                                        int y1 = y + 4;
                                        int z1 = z;

                                        block_t& Block = GetBlock(x1, y1, z1);
                                        if (Block == 0) Block = 37;
                                }


                                for(int zz = -1; zz <= 1; zz++)
                                {
                                    int x1 = x;// :D - what a shitty code
                                    int y1 = y + 4;
                                    int z1 = zz + z;

                                    block_t& Block = GetBlock(x1, y1, z1);
                                    if (Block == 0) Block = 37;
                                }

                                for(int xx = -1; xx <= 1; xx++)
                                {
                                    int x1 = xx + x;// :D - what a shitty code
                                    int y1 = y + 6;
                                    int z1 = z;

                                    block_t& Block = GetBlock(x1, y1, z1);
                                    if (Block == 0) Block = 37;
                                }

                                for(int zz = -1; zz <= 1; zz++)
                                {
                                    int x1 = x;// :D - what a shitty code
                                    int y1 = y + 6;
                                    int z1 = zz + z;

                                    block_t& Block = GetBlock(x1, y1, z1);
                                    if (Block == 0) Block = 37;
                                }

                                block_t& Block = GetBlock(x, y+7, z);
                                if (Block == 0) Block = 37;
                            }
                            else // type == 1
                            {
                                block_t& Block = GetBlock(x, y+TrunkHeight+1, z);
                                if (Block == 0) Block = 37;

                                for(int xx = -1; xx <= 1; xx++)
                                {
                                        int x1 = xx + x;// :D - what a shitty code
                                        int y1 = y + TrunkHeight;
                                        int z1 = z;

                                        block_t& Block = GetBlock(x1, y1, z1);
                                        if (Block == 0) Block = 37;
                                }

                                for(int zz = -1; zz <= 1; zz++)
                                {
                                    int x1 = x;
                                    int y1 = y + TrunkHeight;
                                    int z1 = zz + z;

                                    block_t& Block = GetBlock(x1, y1, z1);
                                    if (Block == 0) Block = 37;
                                }

                                for(int xx = -1; xx <= 1; xx++)
                                {
                                        int x1 = xx + x;// :D - what a shitty code
                                        int y1 = y + TrunkHeight-1;
                                        int z1 = z;

                                        block_t& Block = GetBlock(x1, y1, z1);
                                        if (Block == 0) Block = 37;
                                }

                                for(int zz = -1; zz <= 1; zz++)
                                {
                                    int x1 = x;// :D - what a shitty code
                                    int y1 = y + TrunkHeight-1;
                                    int z1 = zz + z;

                                    block_t& Block = GetBlock(x1, y1, z1);
                                    if (Block == 0) Block = 37;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if(!somethingWasUpdated)
    {
        updatingChunks.erase(updatingChunks.begin()+vectorId);
        MeshChunk->periodicallyUpadted = false;
    }
}

void CraftWorld::UpdateFlood(Vector3 pos) // not used
{
    /*
    int x, y, z;
    x = pos.x;
    y = pos.y;
    z = pos.z;

    int id = GetBlock(x,y,z);

    int bot_block = GetBlock(x,y-1,z);
    int left_block = GetBlock(x-1,y,z);
    int right_block = GetBlock(x+1,y,z);
    int back_block = GetBlock(x,y,z-1);
    int front_block = GetBlock(x,y,z+1);

    if(id == WaterBlock::getID() || (id >= WaterBlock2::getID() && id <= WaterBlock4::getID()))
    {
        if(blockTypes[bot_block].solid == false && bot_block != Torch::getID()) // move down
        {
            bool moved = false;
            if((bot_block == WaterBlock2::getID() && id == WaterBlock2::getID()) ||
               (bot_block == WaterBlock3::getID() && id == WaterBlock1::getID()) ||
               (bot_block == WaterBlock1::getID() && id == WaterBlock3::getID()))
            {
                GetBlock(x,y,z) = 0;
                GetBlock(x,y-1,z) = WaterBlock4::getID();

                AddToFloodPos(Vector3(x,y-1,z));

                moved = true;

                AddChunkToFloodUpdate(getChunkId(Vector3(x,y-1,z)));
                AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                return;
            }
            else if((bot_block == WaterBlock3::getID() && id == WaterBlock2::getID()) ||
                    (bot_block == WaterBlock1::getID() && id == WaterBlock4::getID()) ||
                    (bot_block == WaterBlock2::getID() && id == WaterBlock3::getID()))
            {
                GetBlock(x,y,z) = WaterBlock1::getID();
                GetBlock(x,y-1,z) = WaterBlock4::getID();

                AddToFloodPos(Vector3(x,y-1,z));
                AddToFloodPos(Vector3(x,y,z));

                moved = true;

                AddChunkToFloodUpdate(getChunkId(Vector3(x,y-1,z)));
                AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                return;
            }
            else if((bot_block == WaterBlock2::getID() && id == WaterBlock4::getID()) ||
                    (bot_block == WaterBlock3::getID() && id == WaterBlock3::getID()))
            {
                GetBlock(x,y,z) = WaterBlock2::getID();
                GetBlock(x,y-1,z) = WaterBlock4::getID();

                AddToFloodPos(Vector3(x,y-1,z));
                AddToFloodPos(Vector3(x,y,z));

                moved = true;

                AddChunkToFloodUpdate(getChunkId(Vector3(x,y-1,z)));
                AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                return;
            }
            else if((bot_block == WaterBlock3::getID() && id == WaterBlock4::getID()))
            {
                GetBlock(x,y,z) = WaterBlock3::getID();
                GetBlock(x,y-1,z) = WaterBlock4::getID();

                AddToFloodPos(Vector3(x,y-1,z));
                AddToFloodPos(Vector3(x,y,z));

                moved = true;

                AddChunkToFloodUpdate(getChunkId(Vector3(x,y-1,z)));
                AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                return;
            }
            else if(!(bot_block >= WaterBlock1::getID() && bot_block <= WaterBlock4::getID()))
            {
                GetBlock(x,y,z) = 0;
                GetBlock(x,y-1,z) = WaterBlock4::getID();

                AddToFloodPos(Vector3(x,y-1,z));

                moved = true;

                AddChunkToFloodUpdate(getChunkId(Vector3(x,y-1,z)));
                AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                return;
            }
        }

        if(blockTypes[left_block].solid == false && left_block != Torch::getID()) // move left
        {
            bool moved = false;
            bool infiniteSource = false;
            if(id == WaterBlock4::getID())
            {
                if(right_block == WaterBlock4::getID() || front_block == WaterBlock4::getID() || back_block == WaterBlock4::getID()) // infinite source
                {
                    infiniteSource = true;
                    if(left_block == WaterBlock3::getID())
                    {
                        GetBlock(x-1,y,z) = WaterBlock4::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x-1,y,z));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x-1,y,z)));
                        return;
                    }
                    else if(left_block == WaterBlock2::getID())
                    {
                        GetBlock(x-1,y,z) = WaterBlock3::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x-1,y,z));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x-1,y,z)));
                        return;
                    }
                    else if(left_block == WaterBlock1::getID())
                    {
                        GetBlock(x-1,y,z) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x-1,y,z));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x-1,y,z)));
                        return;
                    }
                    else if(left_block != WaterBlock4::getID())
                    {
                        GetBlock(x-1,y,z) = WaterBlock1::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x-1,y,z));
                        AddToFloodPos(Vector3(x,y,z));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x-1,y,z)));
                        return;
                    }
                }
            }

            if(!infiniteSource)
            {
                if(id == WaterBlock4::getID())
                {
                    if(left_block == WaterBlock2::getID())
                    {
                        GetBlock(x-1,y,z) = WaterBlock3::getID();
                        GetBlock(x,y,z) = WaterBlock3::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x-1,y,z));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x-1,y,z)));

                        return;
                    }
                    else if(left_block == WaterBlock1::getID())
                    {
                        GetBlock(x-1,y,z) = WaterBlock2::getID();
                        GetBlock(x,y,z) = WaterBlock3::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x-1,y,z));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x-1,y,z)));

                        return;
                    }
                    else if(left_block != WaterBlock4::getID() && left_block != WaterBlock3::getID())
                    {
                        GetBlock(x-1,y,z) = WaterBlock2::getID();
                        GetBlock(x,y,z) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x-1,y,z));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x-1,y,z)));

                        return;
                    }
                }
                if(id == WaterBlock3::getID())
                {
                    if(left_block == WaterBlock1::getID())
                    {
                        GetBlock(x-1,y,z) = WaterBlock2::getID();
                        GetBlock(x,y,z) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x-1,y,z));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x-1,y,z)));

                        return;
                    }
                    else if(!(left_block >= WaterBlock2::getID() && left_block <= WaterBlock4::getID()))
                    {

                        GetBlock(x-1,y,z) = WaterBlock1::getID();
                        GetBlock(x,y,z) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x-1,y,z));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x-1,y,z)));

                        return;
                    }
                }
                if(id == WaterBlock2::getID())
                {
                    if(!(left_block >= WaterBlock1::getID() && left_block <= WaterBlock4::getID()))
                    {
                        GetBlock(x-1,y,z) = WaterBlock1::getID();
                        GetBlock(x,y,z) = WaterBlock1::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x-1,y,z));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x-1,y,z)));

                        return;
                    }
                }
            }
        }




        if(blockTypes[right_block].solid == false && right_block != Torch::getID()) // move right
        {
            bool moved = false;
            bool infiniteSource = false;
            if(id == WaterBlock4::getID())
            {
                if(left_block == WaterBlock4::getID() || front_block == WaterBlock4::getID() || back_block == WaterBlock4::getID()) // infinite source
                {
                    infiniteSource = true;
                    if(right_block == WaterBlock3::getID())
                    {
                        GetBlock(x+1,y,z) = WaterBlock4::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x+1,y,z));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x+1,y,z)));
                        return;
                    }
                    else if(right_block == WaterBlock2::getID())
                    {
                        GetBlock(x+1,y,z) = WaterBlock3::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x+1,y,z));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x+1,y,z)));
                        return;
                    }
                    else if(right_block == WaterBlock1::getID())
                    {
                        GetBlock(x+1,y,z) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x+1,y,z));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x+1,y,z)));
                        return;
                    }
                    else if(right_block != WaterBlock4::getID())
                    {
                        GetBlock(x+1,y,z) = WaterBlock1::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x+1,y,z));
                        AddToFloodPos(Vector3(x,y,z));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x+1,y,z)));
                        return;
                    }
                }
            }

            if(!infiniteSource)
            {
                if(id == WaterBlock4::getID())
                {
                    if(right_block == WaterBlock2::getID())
                    {
                        GetBlock(x+1,y,z) = WaterBlock3::getID();
                        GetBlock(x,y,z) = WaterBlock3::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x+1,y,z));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x+1,y,z)));

                        return;
                    }
                    else if(right_block == WaterBlock1::getID())
                    {
                        GetBlock(x+1,y,z) = WaterBlock2::getID();
                        GetBlock(x,y,z) = WaterBlock3::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x+1,y,z));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x+1,y,z)));

                        return;
                    }
                    else if(right_block != WaterBlock4::getID() && right_block != WaterBlock3::getID())
                    {
                        GetBlock(x+1,y,z) = WaterBlock2::getID();
                        GetBlock(x,y,z) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x+1,y,z));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x+1,y,z)));

                        return;
                    }
                }
                if(id == WaterBlock3::getID())
                {
                    if(right_block == WaterBlock1::getID())
                    {
                        GetBlock(x+1,y,z) = WaterBlock2::getID();
                        GetBlock(x,y,z) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x+1,y,z));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x+1,y,z)));

                        return;
                    }
                    else if(!(right_block >= WaterBlock2::getID() && right_block <= WaterBlock4::getID()))
                    {

                        GetBlock(x+1,y,z) = WaterBlock1::getID();
                        GetBlock(x,y,z) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x+1,y,z));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x+1,y,z)));

                        return;
                    }
                }
                if(id == WaterBlock2::getID())
                {
                    if(!(right_block >= WaterBlock1::getID() && right_block <= WaterBlock4::getID()))
                    {
                        GetBlock(x+1,y,z) = WaterBlock1::getID();
                        GetBlock(x,y,z) = WaterBlock1::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x+1,y,z));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x+1,y,z)));

                        return;
                    }
                }
            }
        }





        if(blockTypes[back_block].solid == false && back_block != Torch::getID()) // move back
        {
            bool moved = false;
            bool infiniteSource = false;
            if(id == WaterBlock4::getID())
            {
                if(left_block == WaterBlock4::getID() || front_block == WaterBlock4::getID() || right_block == WaterBlock4::getID()) // infinite source
                {
                    infiniteSource = true;
                    if(back_block == WaterBlock3::getID())
                    {
                        GetBlock(x,y,z-1) = WaterBlock4::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z-1));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z-1)));
                        return;
                    }
                    else if(back_block == WaterBlock2::getID())
                    {
                        GetBlock(x,y,z-1) = WaterBlock3::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z-1));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z-1)));
                        return;
                    }
                    else if(back_block == WaterBlock1::getID())
                    {
                        GetBlock(x,y,z-1) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z-1));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z-1)));
                        return;
                    }
                    else if(back_block != WaterBlock4::getID())
                    {
                        GetBlock(x,y,z-1) = WaterBlock1::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z-1));
                        AddToFloodPos(Vector3(x,y,z));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z-1)));
                        return;
                    }
                }
            }

            if(!infiniteSource)
            {
                if(id == WaterBlock4::getID())
                {
                    if(back_block == WaterBlock2::getID())
                    {
                        GetBlock(x,y,z-1) = WaterBlock3::getID();
                        GetBlock(x,y,z) = WaterBlock3::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z-1));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z-1)));

                        return;
                    }
                    else if(back_block == WaterBlock1::getID())
                    {
                        GetBlock(x,y,z-1) = WaterBlock2::getID();
                        GetBlock(x,y,z) = WaterBlock3::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z-1));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z-1)));

                        return;
                    }
                    else if(back_block != WaterBlock4::getID() && back_block != WaterBlock3::getID())
                    {
                        GetBlock(x,y,z-1) = WaterBlock2::getID();
                        GetBlock(x,y,z) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z-1));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z-1)));

                        return;
                    }
                }
                if(id == WaterBlock3::getID())
                {
                    if(back_block == WaterBlock1::getID())
                    {
                        GetBlock(x,y,z-1) = WaterBlock2::getID();
                        GetBlock(x,y,z) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z-1));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z-1)));

                        return;
                    }
                    else if(!(back_block >= WaterBlock2::getID() && back_block <= WaterBlock4::getID()))
                    {

                        GetBlock(x,y,z-1) = WaterBlock1::getID();
                        GetBlock(x,y,z) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z-1));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z-1)));

                        return;
                    }
                }
                if(id == WaterBlock2::getID())
                {
                    if(!(back_block >= WaterBlock1::getID() && back_block <= WaterBlock4::getID()))
                    {
                        GetBlock(x,y,z-1) = WaterBlock1::getID();
                        GetBlock(x,y,z) = WaterBlock1::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z-1));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z-1)));

                        return;
                    }
                }
            }
        }



        if(blockTypes[front_block].solid == false && front_block != Torch::getID()) // move front
        {
            bool moved = false;
            bool infiniteSource = false;
            if(id == WaterBlock4::getID())
            {
                if(left_block == WaterBlock4::getID() || back_block == WaterBlock4::getID() || right_block == WaterBlock4::getID()) // infinite source
                {
                    infiniteSource = true;
                    if(front_block == WaterBlock3::getID())
                    {
                        GetBlock(x,y,z+1) = WaterBlock4::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z+1));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z+1)));
                        return;
                    }
                    else if(front_block == WaterBlock2::getID())
                    {
                        GetBlock(x,y,z+1) = WaterBlock3::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z+1));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z+1)));
                        return;
                    }
                    else if(front_block == WaterBlock1::getID())
                    {
                        GetBlock(x,y,z+1) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z+1));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z+1)));
                        return;
                    }
                    else if(front_block != WaterBlock4::getID())
                    {
                        GetBlock(x,y,z+1) = WaterBlock1::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z+1));
                        AddToFloodPos(Vector3(x,y,z));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z+1)));
                        return;
                    }
                }
            }

            if(!infiniteSource)
            {
                if(id == WaterBlock4::getID())
                {
                    if(front_block == WaterBlock2::getID())
                    {
                        GetBlock(x,y,z+1) = WaterBlock3::getID();
                        GetBlock(x,y,z) = WaterBlock3::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z+1));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z+1)));

                        return;
                    }
                    else if(front_block == WaterBlock1::getID())
                    {
                        GetBlock(x,y,z+1) = WaterBlock2::getID();
                        GetBlock(x,y,z) = WaterBlock3::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z+1));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z+1)));

                        return;
                    }
                    else if(front_block != WaterBlock4::getID() && front_block != WaterBlock3::getID())
                    {
                        GetBlock(x,y,z+1) = WaterBlock2::getID();
                        GetBlock(x,y,z) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z+1));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z+1)));

                        return;
                    }
                }
                if(id == WaterBlock3::getID())
                {
                    if(front_block == WaterBlock1::getID())
                    {
                        GetBlock(x,y,z+1) = WaterBlock2::getID();
                        GetBlock(x,y,z) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z+1));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z+1)));

                        return;
                    }
                    else if(!(front_block >= WaterBlock2::getID() && front_block <= WaterBlock4::getID()))
                    {

                        GetBlock(x,y,z+1) = WaterBlock1::getID();
                        GetBlock(x,y,z) = WaterBlock2::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z+1));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z+1)));

                        return;
                    }
                }
                if(id == WaterBlock2::getID())
                {
                    if(!(front_block >= WaterBlock1::getID() && front_block <= WaterBlock4::getID()))
                    {
                        GetBlock(x,y,z+1) = WaterBlock1::getID();
                        GetBlock(x,y,z) = WaterBlock1::getID();

                        moved = true;

                        AddToFloodPos(Vector3(x,y,z));
                        AddToFloodPos(Vector3(x,y,z+1));

                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z)));
                        AddChunkToFloodUpdate(getChunkId(Vector3(x,y,z+1)));

                        return;
                    }
                }
            }
        }




    }*/
}

void CraftWorld::AddToFloodPos(Vector3 pos) // not used
{
    /*
    bool push = true;
    if(floodPos.empty() == false)
    {
        for(std::vector<Vector3>::size_type i = 0; i < floodPos.size(); i++)
        {
            if(floodPos[i] == pos)
            {
                push = false;
                break;
            }
        }
    }

    if(push)
    {
        floodPos.push_back(pos);
    }*/
}


void CraftWorld::AddChunkToFloodUpdate(int id)
{
    if(id > 0 && id < mChunks.size())
    {
        bool push = true;
        if(floodChunks.empty() == false)
        {
            for(std::vector<int>::size_type i = 0; i < floodChunks.size(); i++)
            {
                if(floodChunks[i] == id)
                {
                    push = false;
                    break;
                }
            }
        }

        if(push)
        {
            floodChunks.push_back(id);
        }
    }
}

void CraftWorld::SetAllChunksToUpdate()
{
    for(unsigned int i = 0; i < mChunks.size(); i++)
    {
        mChunks[i]->needUpdate = true;
        mTransparentChunks[i]->needUpdate = true;
    }
}

int CraftWorld::getChunkId(Vector3 pos)
{
    if(pos.x >= WORLD_SIZE)
    {
        pos.x = WORLD_SIZE-2;
    }
    if(pos.z >= WORLD_SIZE)
    {
        pos.z = WORLD_SIZE-2;
    }
    if(pos.y >= WORLD_HEIGHT)
    {
        pos.y = WORLD_HEIGHT-2;
    }
    if(pos.x < 0)
    {
        pos.x = 1;
    }
    if(pos.y < 0)
    {
        pos.y = 1;
    }
    if(pos.z < 0)
    {
        pos.z = 1;
    }

    int id = floorf(pos.x/CHUNK_SIZE) + floorf(pos.y/CHUNK_SIZE) * (WORLD_SIZE/CHUNK_SIZE) + floorf(pos.z/CHUNK_SIZE) * (WORLD_SIZE/CHUNK_SIZE) * (WORLD_HEIGHT/CHUNK_SIZE);
    return id;
}

bool CraftWorld::ChunksEqual(Vector3 pos1, Vector3 pos2)
{
    return (getChunkId(pos1) == getChunkId(pos2));
}

void CraftWorld::drawCubes(int i, float light)
{
    DrawPlaceTexture(true);
    sceGuEnable(GU_BLEND);

    sceGuColor(GU_COLOR(light*0.51,light*0.51,light*0.51,1));
    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, blockTypes[i].pointsLeftRight, 0, blockTypes[i].verticesLeftRight);

    sceGuColor(GU_COLOR(light*0.71,light*0.71,light*0.71,1));
    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, blockTypes[i].pointsBackFront, 0, blockTypes[i].verticesBackFront);

    sceGuColor(GU_COLOR(light,light,light,1));
    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, blockTypes[i].pointsBottomTop, 0, blockTypes[i].verticesBottomTop);

    sceGuDisable(GU_BLEND);
    DrawPlaceTexture(false);
}


void CraftWorld::drawHudCubes(int i)
{
    sceGuDisable(GU_DEPTH_TEST);
    DrawSetDepthMask(true);

    MatrixScale(Vector3(18,18,18));
    MatrixAngle(Vector3(150,44.6f,0));

    DrawSetCulling(true);
    drawCubes(i,1.0f);
    DrawSetCulling(false);

    DrawSetDepthMask(false);
    sceGuEnable(GU_DEPTH_TEST);
}


void CraftWorld::drawWhiteCube(int i, float light)
{
    DrawPlaceTexture(false);

    sceGuColor(GU_COLOR(light,light,light,1));

    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, blockTypes[i].pointsLeftRight, 0, blockTypes[i].verticesLeftRight);
    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, blockTypes[i].pointsBackFront, 0, blockTypes[i].verticesBackFront);
    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, blockTypes[i].pointsBottomTop, 0, blockTypes[i].verticesBottomTop);

    DrawPlaceTexture(true);
}

void CraftWorld::drawHand()
{
    DrawPlaceTexture(true);
    DrawSetBlend(true);

    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 36, 0, handVertices);

    DrawSetBlend(false);
    DrawPlaceTexture(false);
}

void CraftWorld::drawClouds()
{
    DrawPlaceTexture(true);
    DrawSetBlend(true);

    DrawSetCulling(true);
    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 600, 0, blockTypes[141].verticesBottomTop);
    DrawSetCulling(false);
    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 600, 0, blockTypes[141].verticesBottomTop);

    DrawSetBlend(false);
    DrawPlaceTexture(false);
}

void CraftWorld::drawItems(int i)
{
    DrawPlaceTexture(true);
    sceGuDisable(GU_DEPTH_TEST);
    DrawSetDepthMask(true);
    DrawSetBlend(true);

    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, itemTypes[i-250].points, 0, itemTypes[i-250].vertices);

    DrawSetBlend(false);
    DrawSetDepthMask(false);
    sceGuEnable(GU_DEPTH_TEST);
    DrawPlaceTexture(false);
}

void CraftWorld::drawHudItems(int i)
{
    MatrixAngle(Vector3(0,0,0));
    MatrixScale(Vector3(32,32,32));

    DrawSetCulling(true);
    drawItems(i);
    DrawSetCulling(false);
}

void CraftWorld::drawDropItems(int i)
{
    DrawPlaceTexture(true);
    DrawSetBlend(true);

    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, itemTypes[i-250].points, 0, itemTypes[i-250].dropVertices);

    DrawSetBlend(false);
    DrawPlaceTexture(false);
}

void CraftWorld::drawHandItems(int i, float light)
{
    DrawPlaceTexture(true);
    DrawSetBlend(true);

    sceGuColor(GU_COLOR(light*0.75,light*0.75,light*0.75,1));
    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, itemTypes[i-250].addPoints, 0, itemTypes[i-250].addVertices);

    DrawSetCulling(true);
    sceGuColor(GU_COLOR(light*0.75,light*0.75,light*0.75,1));
    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, itemTypes[i-250].add2Points, 0, itemTypes[i-250].add2Vertices);
    sceGuColor(GU_COLOR(light,light,light,1));
    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, itemTypes[i-250].points, 0, itemTypes[i-250].vertices);
    DrawSetCulling(false);

    DrawSetBlend(false);
    DrawPlaceTexture(false);
}

void CraftWorld::drawFull3DItem(int i, float light)
{
    DrawPlaceTexture(true);
    DrawSetBlend(true);

    DrawSetCulling(false);

    sceGuColor(GU_COLOR(light*0.75,light*0.75,light*0.75,1));
    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, itemTypes[i-250].add2Points, 0, itemTypes[i-250].add2Vertices);

    DrawSetCulling(true);
    sceGuColor(GU_COLOR(light*0.75,light*0.75,light*0.75,1));
    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, itemTypes[i-250].addPoints, 0, itemTypes[i-250].addVertices);

    sceGuColor(GU_COLOR(light,light,light,1));
    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, itemTypes[i-250].points, 0, itemTypes[i-250].vertices);
    DrawSetCulling(false);
    sceGuColor(GU_COLOR(light,light,light,1));
    sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, itemTypes[i-250].points, 0, itemTypes[i-250].vertices);

    DrawSetCulling(true);

    DrawSetBlend(false);
    DrawPlaceTexture(false);
}

void CraftWorld::drawArmor(int i, float light)
{
    if(i >= 328 && i <= 331) // leather
    {
        TextureManager::Instance()->SetTextureModeulate(textureLeatherArmor1Id);
        if(i == 330)
        {
            TextureManager::Instance()->SetTextureModeulate(textureLeatherArmor2Id);
        }
    }

    if(i >= 332 && i <= 335) // chain
    {
        TextureManager::Instance()->SetTextureModeulate(textureChainArmor1Id);
        if(i == 334)
        {
            TextureManager::Instance()->SetTextureModeulate(textureChainArmor2Id);
        }
    }

    if(i >= 336 && i <= 339) // iron
    {
        TextureManager::Instance()->SetTextureModeulate(textureIronArmor1Id);
        if(i == 338)
        {
            TextureManager::Instance()->SetTextureModeulate(textureIronArmor2Id);
        }
    }

    if(i >= 340 && i <= 343) // diamond
    {
        TextureManager::Instance()->SetTextureModeulate(textureDiamondArmor1Id);
        if(i == 342)
        {
            TextureManager::Instance()->SetTextureModeulate(textureDiamondArmor2Id);
        }
    }

    if(i >= 344 && i <= 347) // diamond
    {
        TextureManager::Instance()->SetTextureModeulate(textureGoldenArmor1Id);
        if(i == 346)
        {
            TextureManager::Instance()->SetTextureModeulate(textureGoldenArmor2Id);
        }
    }

    float points = 0;

    DrawSetBlend(true);
    sceGuEnable(GU_DEPTH_TEST);
    sceGuEnable(GU_ALPHA_TEST);

    sceGuColor(GU_COLOR(light,light,light,1));
    switch(itemTypes[i-250].itemType)
    {
    case 'H':
        points = 30;
        sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, points, 0, helmetVertices);
    break;
    case 'C':
        points = 24;
        sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, points, 0, chestplateVertices);
    break;
    case 'L':
        points = 24;
        sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, points, 0, leggingVertices);
    break;
    case 'B':
        points = 30;
        sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, points, 0, bootVertices);
    break;
    }

    sceGuDisable(GU_ALPHA_TEST);
    sceGuDisable(GU_DEPTH_TEST);
    DrawSetBlend(false);
}

void CraftWorld::drawShoulder(int chestplateId, float light)
{
    if(chestplateId >= 328 && chestplateId <= 331) // leather
    {
        TextureManager::Instance()->SetTextureModeulate(textureLeatherArmor1Id);
    }

    if(chestplateId >= 332 && chestplateId <= 335) // chain
    {
        TextureManager::Instance()->SetTextureModeulate(textureChainArmor1Id);
    }

    if(chestplateId >= 336 && chestplateId <= 339) // iron
    {
        TextureManager::Instance()->SetTextureModeulate(textureIronArmor1Id);
    }

    if(chestplateId >= 340 && chestplateId <= 343) // diamond
    {
        TextureManager::Instance()->SetTextureModeulate(textureDiamondArmor1Id);
    }

    if(chestplateId >= 344 && chestplateId <= 347) // diamond
    {
        TextureManager::Instance()->SetTextureModeulate(textureGoldenArmor1Id);
    }

    float points = 0;

    DrawSetBlend(true);
    sceGuEnable(GU_DEPTH_TEST);
    sceGuEnable(GU_ALPHA_TEST);

    sceGuColor(GU_COLOR(light,light,light,1));
    switch(itemTypes[chestplateId-250].itemType)
    {
    case 'C':
        points = 30;
        DrawSetCulling(false);
        sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, points, 0, shoulderVertices);
        DrawSetCulling(true);
        sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, points, 0, shoulderVertices);
    break;
    }

    sceGuDisable(GU_ALPHA_TEST);
    sceGuDisable(GU_DEPTH_TEST);
    DrawSetBlend(false);
    DrawSetCulling(true);
}

void CraftWorld::drawBelly(int leggingsId, float light)
{
    if(leggingsId >= 328 && leggingsId <= 331) // leather
    {
        TextureManager::Instance()->SetTextureModeulate(textureLeatherArmor2Id);
    }

    if(leggingsId >= 332 && leggingsId <= 335) // chain
    {
        TextureManager::Instance()->SetTextureModeulate(textureChainArmor2Id);
    }

    if(leggingsId >= 336 && leggingsId <= 339) // iron
    {
        TextureManager::Instance()->SetTextureModeulate(textureIronArmor2Id);
    }

    if(leggingsId >= 340 && leggingsId <= 343) // diamond
    {
        TextureManager::Instance()->SetTextureModeulate(textureDiamondArmor2Id);
    }

    if(leggingsId >= 344 && leggingsId <= 347) // diamond
    {
        TextureManager::Instance()->SetTextureModeulate(textureGoldenArmor2Id);
    }

    float points = 0;

    DrawSetBlend(true);
    sceGuEnable(GU_DEPTH_TEST);
    sceGuEnable(GU_ALPHA_TEST);

    sceGuColor(GU_COLOR(light,light,light,1));
    switch(itemTypes[leggingsId-250].itemType)
    {
    case 'L':
        points = 24;
        DrawSetCulling(true);
        sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, points, 0, bellyVertices);
    break;
    }

    sceGuDisable(GU_ALPHA_TEST);
    sceGuDisable(GU_DEPTH_TEST);
    DrawSetBlend(false);
    DrawSetCulling(true);
}

void CraftWorld::AddNearestChunk(int id)
{
    bool alreadyHadThisChunk = false;
    for(unsigned int i = 0; i < mNearestChunksIds.size(); i++)
    {
        if(mNearestChunksIds[i] == id)
        {
            alreadyHadThisChunk = true;
            break;
        }
    }
    if(!alreadyHadThisChunk)
    {
        mNearestChunksIds.push_back(id);
    }
}

double CraftWorld::TestWriteChunk(float dt)
{
    if(dt < 0.03f)
    {

        char *testChunkData;
        testChunkData = new  char[14 * 98 * 14];
        for(int i = 0; i < 14*98*14; i++)
        {
            testChunkData[i] = rand() % 122;
        }

        struct timeval  tv;
        gettimeofday(&tv, NULL);
        double time_begin = ((double)tv.tv_sec) * 1000 + ((double)tv.tv_usec) / 1000;

        SceUID fd = sceIoOpen("ms0:/test.txt",  PSP_O_WRONLY|PSP_O_CREAT, 0777);
        sceIoChangeAsyncPriority(fd, 0x8);
        sceIoWriteAsync(fd, testChunkData, sizeof(char)*14*98*14);
        SceInt64 res;
        sceIoWaitAsync(fd, &res);
        sceIoClose(fd);

        gettimeofday(&tv, NULL);
        double time_end = ((double)tv.tv_sec) * 1000 + ((double)tv.tv_usec) / 1000 ;
        double total_time_ms = time_end - time_begin;

        /*std::ofstream outfile ("test2.txt",std::ofstream::binary);

        struct timeval  tv;
        gettimeofday(&tv, NULL);
        double time_begin = ((double)tv.tv_sec) * 1000 + ((double)tv.tv_usec) / 1000;

        outfile << "edsdfsergredsfgsdgfdfdsgsrsge";
        outfile.write (testChunkData,sizeof(char)*14*98*14);

        gettimeofday(&tv, NULL);
        double time_end = ((double)tv.tv_sec) * 1000 + ((double)tv.tv_usec) / 1000 ;
        double total_time_ms = time_end - time_begin;

        outfile.close(); */


        /*FILE * pFile;
        pFile = fopen ( "myfile.bin" , "w+b" );
        fwrite (testChunkData, sizeof(unsigned char), 14*14*14, pFile);
        fclose (pFile); */
       /* SceUID filein, fileout;

        fileout = sceIoOpen("test.txt", PSP_O_WRONLY , 0777);

        //Write pBuf to the new file/

        sceIoWrite(fileout, testChunkData, sizeof(unsigned char)*14*98*14);


        //Close the file.
        sceIoClose(fileout); */


        free(testChunkData);
        return total_time_ms;
    }
    return 0;
}

void CraftWorld::FetchNearestChunks()
{
    // code profiled successful. -0.4 ms
    /* struct timeval  tv;
    gettimeofday(&tv, NULL);

    double time_begin = ((double)tv.tv_sec) * 1000 + ((double)tv.tv_usec) / 1000;


    gettimeofday(&tv, NULL);
    double time_end = ((double)tv.tv_sec) * 1000 + ((double)tv.tv_usec) / 1000 ;
    double total_time_ms = time_end - time_begin;

    Utils::Logger::Instance()->LogMessage("(CraftWorld::FetchNearestChunks : %f ms)\n",total_time_ms); */


    int playerChunkId = getChunkId(playerPos);
    SimpleMeshChunk* TestChunk = mChunks[playerChunkId];

    int playerChunkX = TestChunk->chunkStartX + CHUNK_SIZE/2;
    int playerChunkY = TestChunk->chunkStartY + CHUNK_SIZE/2;
    int playerChunkZ = TestChunk->chunkStartZ + CHUNK_SIZE/2;

    int lower_range_x = playerChunkX-(mainOptions.horizontalViewDistance)*CHUNK_SIZE;
    int lower_range_y = playerChunkY-(mainOptions.verticalViewDistance)*CHUNK_SIZE;
    int lower_range_z = playerChunkZ-(mainOptions.horizontalViewDistance)*CHUNK_SIZE;

    int higher_range_x = playerChunkX+(mainOptions.horizontalViewDistance)*CHUNK_SIZE;
    int higher_range_y = playerChunkY+(mainOptions.verticalViewDistance)*CHUNK_SIZE;
    int higher_range_z = playerChunkZ+(mainOptions.horizontalViewDistance)*CHUNK_SIZE;

    if(lower_range_x < 0)
    {
        lower_range_x = CHUNK_SIZE/2;
    }
    else if(higher_range_x >= WORLD_SIZE)
    {
        higher_range_x = WORLD_SIZE-CHUNK_SIZE/2;
    }

    if(lower_range_y < 0)
    {
        lower_range_y = CHUNK_SIZE/2;
    }
    else if(higher_range_y >= WORLD_HEIGHT)
    {
        higher_range_y = WORLD_HEIGHT-CHUNK_SIZE/2;
    }

    if(lower_range_z < 0)
    {
        lower_range_z = CHUNK_SIZE/2;
    }
    else if(higher_range_z >= WORLD_SIZE)
    {
        higher_range_z = WORLD_SIZE-CHUNK_SIZE/2;
    }

    for(int x = lower_range_x; x <= higher_range_x; x+=CHUNK_SIZE)
    {
        for(int y = lower_range_y; y <= higher_range_y; y+=CHUNK_SIZE)
        {
            for(int z = lower_range_z; z <= higher_range_z; z+=CHUNK_SIZE)
            {
                int id = getChunkId(Vector3(x,y,z));
                if(playerZoneBB.intersect(mChunks[id]->bBox))
                {
                    AddNearestChunk(id);
                }
            }
        }
    }


    /*for(unsigned int i = 0; i < mChunks.size(); i++)
    {
        if(playerZoneBB.intersect(mChunks[i]->bBox))
        {
            AddNearestChunk(i);
        }
    }*/
}

void CraftWorld::drawWorld(Frustum &camFrustum, bool camUpdate)
{
    //DrawSetCulling(true);
    drawnTriangles = 0;
    //normal not transparent chunks
    sceGuColor(0xFFFFFFFF);
    sceGuDisable(GU_BLEND);
    sceGuDisable(GU_ALPHA_TEST);

    int lowestDistanse = 999;
    int toUpdateId = -1;

    if(mNearestChunksIds.empty() == false)
    {
        for(unsigned int i2 = 0; i2 != mNearestChunksIds.size(); i2++)
        {
            bool found = false;
            int i = mNearestChunksIds[i2];

            mTransparentChunks[i]->inFrustum = false;

            if(playerZoneBB.intersect(mChunks[i]->bBox))
            {
                if(mChunks[i]->trienglesCount > 0 || mTransparentChunks[i]->trienglesCount > 0 || !mChunks[i]->created)
                {
                    if(camFrustum.BoxInFrustum(mChunks[i]->bBox) == Frustum::Intersects)
                    {
                        mTransparentChunks[i]->inFrustum = true;

                        //check if we need to build vertices of this chunk
                        if((!mChunks[i]->created || mChunks[i]->needUpdate) && chunksCreatedInFrameCount == 0 && !found)
                        {
                            int distance = FastDistance2d(abs(playerPos.x-(mChunks[i]->chunkStartX+7)),abs(playerPos.z-(mChunks[i]->chunkStartZ+7)))+abs(playerPos.y-(mChunks[i]->chunkStartY+7))*0.85f;
                            //int distance = abs(playerPos.x-(mChunks[i]->chunkStartX+8))+abs(playerPos.y-(mChunks[i]->chunkStartY+8))+abs(playerPos.z-(mChunks[i]->chunkStartZ+8));
                            if(distance < lowestDistanse)
                            {
                                lowestDistanse = distance;
                                toUpdateId = i;
                            }
                            if(distance < CHUNK_SIZE)
                            {
                                found = true;
                            }
                        }

                        if(mChunks[i]->trienglesCount > 0)
                        {
                            mChunks[i]->drawChunk();
                            drawnTriangles += mChunks[i]->trienglesCount;
                        }
                    }
                }
            }
            else
            {
                if(mChunks[i]->created)
                {
                    mChunks[i]->reset();
                }
                if(mTransparentChunks[i]->created)
                {
                    mTransparentChunks[i]->reset();
                }

                mNearestChunksIds.erase(mNearestChunksIds.begin()+i2);
                i2 --;

                if(mNearestChunksIds.empty() == true)
                {
                    break;
                }
            }
        }
    }

    if(toUpdateId != -1)
    {
        chunksCreatedInFrameCount++;
        RebuildFullMeshChunk(toUpdateId);
    }

    //transparent chunks
    sceGuEnable(GU_DEPTH_TEST);
    sceGuEnable(GU_ALPHA_TEST);
    sceGuEnable(GU_BLEND);

    if(mNearestChunksIds.empty() == false)
    {
        unsigned int length = mNearestChunksIds.size();
        for(unsigned int i2 = 0; i2 != length; i2++)
        {
            int i = mNearestChunksIds[i2];

            if(mTransparentChunks[i]->trienglesCount > 0 || !mTransparentChunks[i]->created)
            {
                if(mTransparentChunks[i]->inFrustum == true)
                {
                    mTransparentChunks[i]->drawChunk();
                    drawnTriangles += mTransparentChunks[i]->trienglesCount;
                }
            }
        }
    }

    sceGuDisable(GU_BLEND);
    sceGuDisable(GU_ALPHA_TEST);
    sceGuDisable(GU_DEPTH_TEST);
}

void CraftWorld::UpdatePlayerZoneBB(Vector3 playerPosition)
{
    playerPos = playerPosition;
    playerZoneBB = BoundingBox(Vector3(playerPosition.x - playerZoneSize.x,playerPosition.y - playerZoneSize.y,playerPosition.z - playerZoneSize.z),Vector3(playerPosition.x + playerZoneSize.x,playerPosition.y + playerZoneSize.y,playerPosition.z + playerZoneSize.z));
}

Vector3 CraftWorld::GetPlayerPos()
{
    return playerPos;
}

void CraftWorld::UpdateClockTexture()
{
    // 102.5f = noon
    // 0.715277 21.458333
    float realSkyTime = skyTime-100.0f;

    if(realSkyTime >= 102.5f && realSkyTime < 360.0f)
    {
        clockFrame = (int)(((realSkyTime-102.5f)/257.5f)*(257.5f/360.0f*30));
    }
    if(realSkyTime >= 0.0f && realSkyTime < 102.5f)
    {
        clockFrame = (int)((257.5f/360.0f*30)+((realSkyTime)/102.5f)*(102.5f/360.0f*30));
    }

    if(lastClockFrame != clockFrame)
    {
        lastClockFrame = clockFrame;
        int frameSize = TextureManager::Instance()->getWidth(textureItemsId)/16;

        for(int i = 0; i < frameSize; i++)
        {
            for(int j = 0; j < frameSize; j++)
            {
                TextureManager::Instance()->setColour(frameSize*6+i,
                                                      frameSize*4+j,
                                                      TextureManager::Instance()->getRed(i,lastClockFrame*frameSize+j,textureClockId),
                                                      TextureManager::Instance()->getGreen(i,lastClockFrame*frameSize+j,textureClockId),
                                                      TextureManager::Instance()->getBlue(i,lastClockFrame*frameSize+j,textureClockId),
                                                      TextureManager::Instance()->getAlpha(i,lastClockFrame*frameSize+j,textureClockId),
                                                      textureItemsId);
            }
        }
    }
}

void CraftWorld::UpdateCompassTexture(float cameraAngle)
{
    float angle = ATAN2(playerPos.z-playerSpawnPointPosition.z,playerPos.x-playerSpawnPointPosition.x)-(cameraAngle-PI);
    if(angle < -PI)
    {
        angle = angle+2*PI;
    }
    if(angle > PI)
    {
        angle = angle-2*PI;
    }
    //atan2f(playerPos.x-playerSpawnPointPosition.x,playerPos.z-playerSpawnPointPosition.z) +

    compassFrame = roundf((angle+PI)/PI*8);
    if(compassFrame >= 16)
    {
        compassFrame = 0;
    }

    if(lastCompassFrame != compassFrame)
    {
        lastCompassFrame = (int)compassFrame;

        int frameSize = TextureManager::Instance()->getWidth(textureItemsId)/16;

        for(int i = 0; i < frameSize; i++)
        {
            for(int j = 0; j < frameSize; j++)
            {
                TextureManager::Instance()->setColour(frameSize*6+i,
                                                      frameSize*3+j,
                                                      TextureManager::Instance()->getRed(i,lastCompassFrame*frameSize+j,textureCompassId),
                                                      TextureManager::Instance()->getGreen(i,lastCompassFrame*frameSize+j,textureCompassId),
                                                      TextureManager::Instance()->getBlue(i,lastCompassFrame*frameSize+j,textureCompassId),
                                                      TextureManager::Instance()->getAlpha(i,lastCompassFrame*frameSize+j,textureCompassId),
                                                      textureItemsId);
            }
        }
    }
}

void CraftWorld::UpdateWorldAnimation(float dt)
{
    if(mainOptions.worldBlockAnimation == true)
    {
        animationTimer += dt;

        if(animationTimer >= animationTimerEnd) // every 0.075 secs
        {
            int waterFrameSize = TextureManager::Instance()->getWidth(textureWaterAnimationId);
            if(waterFrameSize > 16)
                return;

            animationTimer = 0.0f; // zeroize timer
            animationWaterFrame += 1; // go to the next water frame
            if(animationWaterFrame >= TextureManager::Instance()->getHeight(textureWaterAnimationId)/TextureManager::Instance()->getWidth(textureWaterAnimationId)) // if all frames passed
            {
                animationWaterFrame = 0; // zeroize frames
            }

            if(animationLavaFrame >= TextureManager::Instance()->getHeight(textureLavaAnimationId)/TextureManager::Instance()->getWidth(textureLavaAnimationId)-1)
            {
                animationLavaStep = false;
            }
            if(animationLavaFrame == 0)
            {
                animationLavaStep = true;
            }

            if(animationLavaStep == true)
            {
                animationLavaFrame += 1;
            }
            else
            {
                animationLavaFrame -= 1;
            }

            for(int i = 0; i < waterFrameSize; i++)
            {
                for(int j = 0; j < waterFrameSize; j++)
                {
                    // copy each water pixel from water_still.png to terrain.png
                    TextureManager::Instance()->setColour(waterFrameSize*14+i,
                                                          waterFrameSize*15+j,
                                                          TextureManager::Instance()->getRed(i,animationWaterFrame*waterFrameSize+j,textureWaterAnimationId),
                                                          TextureManager::Instance()->getGreen(i,animationWaterFrame*waterFrameSize+j,textureWaterAnimationId),
                                                          TextureManager::Instance()->getBlue(i,animationWaterFrame*waterFrameSize+j,textureWaterAnimationId),
                                                          184,
                                                          textureTerrainId);
                    // same to lava
                    TextureManager::Instance()->setColour(waterFrameSize*15+i,
                                                          waterFrameSize*15+j,
                                                          TextureManager::Instance()->getRed(i,animationLavaFrame*waterFrameSize+j,textureLavaAnimationId),
                                                          TextureManager::Instance()->getGreen(i,animationLavaFrame*waterFrameSize+j,textureLavaAnimationId),
                                                          TextureManager::Instance()->getBlue(i,animationLavaFrame*waterFrameSize+j,textureLavaAnimationId),
                                                          255,
                                                          textureTerrainId);


                    // for mips texture
                    if(i < 8 && j < 8)
                    {
                        TextureManager::Instance()->setColour(8*14+i,
                                                              8*15+j,
                                                              TextureManager::Instance()->getRed(i,animationWaterFrame*waterFrameSize+j,textureWaterAnimationId),
                                                              TextureManager::Instance()->getGreen(i,animationWaterFrame*waterFrameSize+j,textureWaterAnimationId),
                                                              TextureManager::Instance()->getBlue(i,animationWaterFrame*waterFrameSize+j,textureWaterAnimationId),
                                                              184,
                                                              textureTerrainMipsId);

                        TextureManager::Instance()->setColour(8*15+i,
                                                              8*15+j,
                                                              TextureManager::Instance()->getRed(i,animationLavaFrame*waterFrameSize+j,textureLavaAnimationId),
                                                              TextureManager::Instance()->getGreen(i,animationLavaFrame*waterFrameSize+j,textureLavaAnimationId),
                                                              TextureManager::Instance()->getBlue(i,animationLavaFrame*waterFrameSize+j,textureLavaAnimationId),
                                                              255,
                                                              textureTerrainMipsId);
                    }
                }
            }
        }
    }
}

void CraftWorld::SpawnMobs(float dt)
{
    spawnZombiesTimer += dt;
    if(spawnZombiesTimer > 15.0f)
    {
        bool wasSpawned = false;
        if((worldDayTime > 21 || worldDayTime < 6) && mainOptions.difficult != 0 && playerPos.y > 55.0f) // spawn zombs
        {
            if(rand() % 50 <= 26+(24*mZombies.empty()) && mZombies.size() < 8)
            {
                float spawnX = 0;
                float spawnY = 0;
                float spawnZ = 0;

                rand() % 2 == 0 ? spawnX = playerPos.x-8-rand() % 18 : spawnX = playerPos.x+8+rand() % 18;
                rand() % 2 == 0 ? spawnZ = playerPos.z-8-rand() % 18 : spawnZ = playerPos.z+8+rand() % 18;

                spawnY = groundHeightWater(spawnX,spawnZ);

                if(BlockFinalLight(spawnX,spawnY,spawnZ) < 0.25f) // spawn only at black areas
                {
                    if(spawnY != -1 && spawnY < WORLD_HEIGHT && spawnX < WORLD_SIZE && spawnZ < WORLD_SIZE && spawnY > 0 && spawnX > 0 && spawnZ > 0)
                    {
                        SpawnZombie(spawnX+0.5f,spawnY+1.8f,spawnZ+0.5f);
                        SpawnZombie(spawnX+2.5f,groundHeightWater(spawnX+2.0f,spawnZ+2.0f)+1.8f,spawnZ+2.5f);
                        wasSpawned = true;
                    }
                }
            }
            spawnZombiesTimer = -10.0f+rand()%11;
        }

        if(mainOptions.difficult != 0 && playerPos.y <= 55.0f) // spawn zombs underground
        {
            if(rand() % 50 <= 30+(24*mZombies.empty()) && mZombies.size() < 12)
            {
                float spawnX = 0;
                float spawnY = 0;
                float spawnZ = 0;

                int side = rand() % 4;
                if(side == 0)
                {
                    spawnZ = playerPos.z;
                    for(spawnX = playerPos.x-26; spawnX <= playerPos.x-16; spawnX++)
                    {
                        spawnZ = playerPos.z-1+rand()%3;
                        spawnY = groundHeightFromBottom(spawnX,playerPos.y-5,spawnZ);
                        if(spawnY != -1)
                        {
                            if(GetBlock(spawnX,spawnY+1,spawnZ) == 0)
                            {
                                if(BlockFinalLight(spawnX,spawnY,spawnZ) <= 0.15f)
                                {
                                    if(spawnY != -1 && spawnY < WORLD_HEIGHT && spawnX < WORLD_SIZE && spawnZ < WORLD_SIZE && spawnY > 0 && spawnX > 0 && spawnZ > 0)
                                    {
                                        SpawnZombie(spawnX+0.5f,spawnY+1.8f,spawnZ+0.5f);
                                        if(rand() % 5 == 0)
                                        {
                                            SpawnZombie(spawnX+0.5f,spawnY+1.8f,spawnZ+0.5f);
                                        }
                                        wasSpawned = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

                if(side == 1)
                {
                    spawnZ = playerPos.z;
                    for(spawnX = playerPos.x+16; spawnX <= playerPos.x+26; spawnX++)
                    {
                        spawnZ = playerPos.z-1+rand()%3;
                        spawnY = groundHeightFromBottom(spawnX,playerPos.y-5,spawnZ);
                        if(spawnY != -1)
                        {
                            if(GetBlock(spawnX,spawnY+1,spawnZ) == 0)
                            {
                                if(BlockFinalLight(spawnX,spawnY,spawnZ) <= 0.15f)
                                {
                                    if(spawnY != -1 && spawnY < WORLD_HEIGHT && spawnX < WORLD_SIZE && spawnZ < WORLD_SIZE && spawnY > 0 && spawnX > 0 && spawnZ > 0)
                                    {
                                        SpawnZombie(spawnX+0.5f,spawnY+1.8f,spawnZ+0.5f);
                                        if(rand() % 5 == 0)
                                        {
                                            SpawnZombie(spawnX+0.5f,spawnY+1.8f,spawnZ+0.5f);
                                        }
                                        wasSpawned = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

                if(side == 2)
                {
                    spawnX = playerPos.z;
                    for(spawnZ = playerPos.z+16; spawnZ <= playerPos.z+26; spawnZ++)
                    {
                        spawnX = playerPos.x-1+rand()%3;
                        spawnY = groundHeightFromBottom(spawnX,playerPos.y-5,spawnZ);
                        if(spawnY != -1)
                        {
                            if(GetBlock(spawnX,spawnY+1,spawnZ) == 0)
                            {
                                if(BlockFinalLight(spawnX,spawnY,spawnZ) <= 0.15f)
                                {
                                    if(spawnY != -1 && spawnY < WORLD_HEIGHT && spawnX < WORLD_SIZE && spawnZ < WORLD_SIZE && spawnY > 0 && spawnX > 0 && spawnZ > 0)
                                    {
                                        SpawnZombie(spawnX+0.5f,spawnY+1.8f,spawnZ+0.5f);
                                        if(rand() % 5 == 0)
                                        {
                                            SpawnZombie(spawnX+0.5f,spawnY+1.8f,spawnZ+0.5f);
                                        }
                                        wasSpawned = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

                if(side == 3)
                {
                    spawnX = playerPos.z;
                    for(spawnZ = playerPos.z-26; spawnZ <= playerPos.z-16; spawnZ++)
                    {
                        spawnX = playerPos.x-1+rand()%3;
                        spawnY = groundHeightFromBottom(spawnX,playerPos.y-5,spawnZ);
                        if(spawnY != -1)
                        {
                            if(GetBlock(spawnX,spawnY+1,spawnZ) == 0)
                            {
                                if(BlockFinalLight(spawnX,spawnY,spawnZ) <= 0.15f)
                                {
                                    if(spawnY != -1 && spawnY < WORLD_HEIGHT && spawnX < WORLD_SIZE && spawnZ < WORLD_SIZE && spawnY > 0 && spawnX > 0 && spawnZ > 0)
                                    {
                                        SpawnZombie(spawnX+0.5f,spawnY+1.8f,spawnZ+0.5f);
                                        if(rand() % 5 == 0)
                                        {
                                            SpawnZombie(spawnX+0.5f,spawnY+1.8f,spawnZ+0.5f);
                                        }
                                        wasSpawned = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(wasSpawned)
            {
                spawnZombiesTimer = -45+rand()%15;
            }
            else
            {
                spawnZombiesTimer = -15.0f;
            }
        }
    }

    spawnCreeperTimer -= dt;
    if(spawnCreeperTimer < 0.0f)
    {
        if((worldDayTime > 21 || worldDayTime < 6) && mainOptions.difficult != 0) // spawn creepers
        {
            if(rand() % 50 <= 27-mCreepers.size()*1.5f && mCreepers.size() < 4)
            {
                float spawnX = 0;
                float spawnY = 0;
                float spawnZ = 0;

                if(rand() % 4 != 0)
                {
                    rand() % 2 == 0 ? spawnX = playerPos.x-20-rand() % 14 : spawnX = playerPos.x+20+rand() % 14;
                    rand() % 2 == 0 ? spawnZ = playerPos.z-20-rand() % 14 : spawnZ = playerPos.z+20+rand() % 14;
                }
                else
                {
                    rand() % 2 == 0 ? spawnX = playerPos.x-8-rand() % 16 : spawnX = playerPos.x+8+rand() % 16;
                    rand() % 2 == 0 ? spawnZ = playerPos.z-8-rand() % 16 : spawnZ = playerPos.z+8+rand() % 16;
                }

                spawnY = groundHeightWater(spawnX,spawnZ);

                if(BlockFinalLight(spawnX,spawnY,spawnZ) < 0.25f) // spawn only at black areas
                {
                    if(spawnY != -1 && spawnY < WORLD_HEIGHT && spawnX < WORLD_SIZE && spawnZ < WORLD_SIZE && spawnY > 0 && spawnX > 0 && spawnZ > 0)
                    {
                        SpawnCreeper(spawnX+0.5f,spawnY+1.8f,spawnZ+0.5f);
                        if(rand() % 4 == 0)
                        {
                            SpawnCreeper(spawnX+2.5f,groundHeightWater(spawnX+2.0f,spawnZ+2.0f)+1.8f,spawnZ+2.5f);
                        }
                    }
                }
            }
            if(rand() % 10 < 7 && mCreepers.size() < 6)
            {
                float spawnX = 0;
                float spawnY = 0;
                float spawnZ = 0;

                spawnX = 30 + rand() % (WORLD_SIZE-32);
                spawnZ = 30 + rand() % (WORLD_SIZE-32);

                spawnY = groundHeightWater(spawnX,spawnZ);

                if(BlockFinalLight(spawnX,spawnY,spawnZ) < 0.25f) // spawn only at black areas
                {
                    if(spawnY != -1 && spawnY < WORLD_HEIGHT && spawnX < WORLD_SIZE && spawnZ < WORLD_SIZE && spawnY > 0 && spawnX > 0 && spawnZ > 0)
                    {
                        SpawnCreeper(spawnX+0.5f,spawnY+1.8f,spawnZ+0.5f,800);
                    }
                }
            }
        }
        spawnCreeperTimer = 30+rand()%30;
    }

    spawnCowsTimer -= dt;
    if(spawnCowsTimer < 0.0f)
    {
        if(worldDayTime > 6 && worldDayTime < 21)
        {
            if(rand() % 2 == 0 && mCows.size()+mSheeps.size() < 12)
            {
                float spawnX = 0;
                float spawnY = 0;
                float spawnZ = 0;

                spawnX = 25 + rand() % (WORLD_SIZE-27);
                spawnZ = 25 + rand() % (WORLD_SIZE-27);
                spawnY = groundHeightWater(spawnX,spawnZ);

                if(spawnY != -1 && spawnY < WORLD_HEIGHT && spawnX < WORLD_SIZE && spawnZ < WORLD_SIZE && spawnY > 0 && spawnX > 0 && spawnZ > 0)
                {
                    SpawnCow(spawnX+0.5f,spawnY+2.0f,spawnZ+0.5f);
                    SpawnCow(spawnX+2.5f,groundHeightWater(spawnX+2.0f,spawnZ+2.0f)+2.0f,spawnZ+2.5f);
                }
            }
        }
        spawnCowsTimer = 80+(mCows.size()+mSheeps.size())*10+rand()%60;
    }

    spawnSheepTimer -= dt;
    if(spawnSheepTimer < 0.0f)
    {
        if(worldDayTime > 6 && worldDayTime < 21)
        {
            if(rand() % 2 == 0 && mCows.size()+mSheeps.size() < 10)
            {
                float spawnX = 0;
                float spawnY = 0;
                float spawnZ = 0;

                spawnX = 30 + rand() % (WORLD_SIZE-32);
                spawnZ = 30 + rand() % (WORLD_SIZE-32);
                spawnY = groundHeightWater(spawnX,spawnZ);

                if(spawnY != -1 && spawnY < WORLD_HEIGHT && spawnX < WORLD_SIZE && spawnZ < WORLD_SIZE && spawnY > 0 && spawnX > 0 && spawnZ > 0)
                {
                    SpawnSheep(spawnX+0.5f,spawnY+2.0f,spawnZ+0.5f);
                    SpawnSheep(spawnX+2.5f,groundHeightWater(spawnX+2.0f,spawnZ+2.0f)+2.0f,spawnZ+2.5f);
                }
            }
        }
        spawnSheepTimer = 100+(mCows.size()+mSheeps.size())*10+rand()%60;
    }
}

void CraftWorld::UpdateWorldProcesses(float dt)
{
    UpdateClockTexture();
    UpdateWorldAnimation(dt);
    SpawnMobs(dt);
    //stuff that goes on tick
    //1 tick = 1 second

    minuteTimer += dt;
    if(minuteTimer > 60.0f)
    {
        mainStatistics.minutesPlayed += 1;
        minuteTimer -= 60.0f;
    }

    if(updatingChunks.empty() == false)
    {
        updatingChunksTimer += dt;
        updatingChunksTimerMax = 30.0f/updatingChunks.size();
        if(updatingChunksTimer > updatingChunksTimerMax)
        {
            UpdateChunkGrowingBlocks(updatingChunks[updatingChunksPosition],updatingChunksPosition);

            if(playerZoneBB.intersect(mChunks[updatingChunks[updatingChunksPosition]]->bBox))
            {
                RebuildFullMeshChunk(updatingChunks[updatingChunksPosition]);
            }

            updatingChunksPosition += 1;
            if(updatingChunksPosition >= updatingChunks.size())
            {
                updatingChunksPosition = 0;
            }
            updatingChunksTimer = 0.0f;
        }
    }

    updateFastChunkTimer += dt;
    if(updateFastChunkTimer >= 0.05f)
    {
        updateFastChunkTimer -= 0.05f;

        if(toFastUpdate.empty() == false)
        {
            unsigned int o = 0;
            RebuildFullMeshChunk(toFastUpdate[o]);

            toFastUpdate.erase(toFastUpdate.begin());
        }
    }

    fetchNearestChunksTimer += dt;
    if(fetchNearestChunksTimer >= 1.65f)
    {
        FetchNearestChunks();
        fetchNearestChunksTimer = 0.0f;
    }

    float cooldown = 0.21f;
    if(mainOptions.horizontalViewDistance == 4)
    {
        cooldown = 0.18f;
    }

    updateChunkTimer += dt;
    if(updateChunkTimer >= cooldown)
    {
        chunksCreatedInFrameCount = 0;
        updateChunkTimer -= cooldown;

        if(toUpdate.empty() == false)
        {
            RebuildFullMeshChunk(toUpdate[0]);
            toUpdate.erase(toUpdate.begin());
        }
    }
}

void CraftWorld::UpdateWorldTime(float dt)
{
    worldTime += dt;
    if(worldTime >= 0.05f)
    {
        if(worldDayTime >= 22 || worldDayTime < 6)
        {
            skyTime += 180.0f/8.0f/50.0f/20.0f;
        }
        if(worldDayTime >= 6 && worldDayTime < 22)
        {
            skyTime += 180.0f/16.0f/50.0f/20.0f;
        }
        worldTime -= 0.05f;
    }

    //world day/night time
    worldHour += dt;
    if(worldHour >= 50.0f)//one game hour = 50 sec
    {
        worldHour -= 50.0f;
        worldDayTime += 1.0f;
        if(worldDayTime > 23.0f)
        {
            worldDayTime = 0.0f;
            mainStatistics.daysInGame += 1;
        }

        Vector3 oldLightColor = lightColor;
        SetWolrdTime(worldDayTime);

        if(oldLightColor != lightColor)
        {
            SetAllChunksToUpdate();
            chunksWasSetToUpdate = true;
        }
        else
        {
            chunksWasSetToUpdate = false;
        }
    }
}

int CraftWorld::GetBlockTypesCount()
{
    return blockTypes.size();
}

int CraftWorld::GetItemTypesCount()
{
    return itemTypes.size();
}

int CraftWorld::groundHeight(const int x, const int z)
{
    if (x < 0 || z < 0 || x >= WORLD_SIZE || z >= WORLD_SIZE) return -1;

    for (int y = WORLD_HEIGHT - 1; y >= 0; --y)
    {
        block_t Block = GetBlock(x, y, z);
        if (Block != 0 && Block != 4)
        {
            return y;
        }
    }

    return -1;
}

int CraftWorld::groundHeight(const int x, const int y, const int z)
{
    if (x < 0 || z < 0 || x >= WORLD_SIZE || z >= WORLD_SIZE) return -1;

    for (int yy = y; yy >= 0; --yy)
    {
        block_t Block = GetBlock(x, yy, z);
        if (Block != 0 && Block != WaterBlock::getID())
        {
            return yy;
        }
    }

    return -1;
}

int CraftWorld::groundHeightFromBottom(const int x, const int y, const int z)
{
    if (x < 0 || z < 0 || x >= WORLD_SIZE || z >= WORLD_SIZE) return -1;


    for (int yy = y; yy < 56; yy++)
    {
        if(yy < 3)
        {
            yy = 3;
        }
        block_t Block = GetBlock(x, yy, z);
        if (Block == 0)
        {
            return yy;
        }
    }

    return -1;
}

int CraftWorld::groundHeightExcept(const int x, const int z, int bl)
{
    if (x < 0 || z < 0 || x >= WORLD_SIZE || z >= WORLD_SIZE) return -1;


    for (int y = WORLD_HEIGHT - 1; y >= 0; --y)
    {
        block_t Block = GetBlock(x, y, z);
        if(Block == bl)
        {
            continue;
        }
        if (Block != 0) return (y);
    }

    return -1;
}

int CraftWorld::groundHeightWater(const int x, const int z)
{
    if (x < 0 || z < 0 || x >= WORLD_SIZE || z >= WORLD_SIZE) return -1;

    for (int y = WORLD_HEIGHT - 1; y >= 0; --y)
    {
        block_t Block = GetBlock(x, y, z);
        if (Block != 0)
        {
            return y;
        }
    }

    return -1;
}

int CraftWorld::BlockSoundAtPos(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    //outside the world so collision
    if (x <= 0 || y <= 0 || z <= 0  || x >= WORLD_SIZE-1 || y >= WORLD_HEIGHT-1 || z >= WORLD_SIZE-1) return -1;

    return blockTypes[GetBlockNoCheck(x,y,z)].soundType;
}


bool CraftWorld::SolidAtPoint(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    float relative_x = (float)pos.x-x;
    float relative_y = (float)pos.y-y;
    float relative_z = (float)pos.z-z;

    //outside the world so collision
    if (x <= 0 || y <= 0 || z <= 0  || x >= WORLD_SIZE-1 || y >= WORLD_HEIGHT-1 || z >= WORLD_SIZE-1) return true;

    //don't collide with water and air
    if(BlockSolid(x,y,z) == 0)
    {
        return false;
    }

    int posBlockID = GetBlockNoCheck(x,y,z);
    if(blockTypes[posBlockID].blockModel == 1) // if it is half block
    {
        if(relative_y < 0.5f)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    if(blockTypes[posBlockID].blockModel == 9)
    {
        if(relative_x < 0.5f)
        {
            return true;
        }
        else
        {
            if(relative_y < 0.5f)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    if(blockTypes[posBlockID].blockModel == 8)
    {
        if(relative_x >= 0.5f)
        {
            return true;
        }
        else
        {
            if(relative_y < 0.5f)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    if(blockTypes[posBlockID].blockModel == 7)
    {
        if(relative_z < 0.5f)
        {
            return true;
        }
        else
        {
            if(relative_y < 0.5f)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    if(blockTypes[posBlockID].blockModel == 6)
    {
        if(relative_z >= 0.5f)
        {
            return true;
        }
        else
        {
            if(relative_y < 0.5f)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

bool CraftWorld::SolidAtPointForPlayerOnGround(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    //outside the world so collision
    if (y <= 0 || y >= WORLD_HEIGHT-1) return false;
    if (x <= 0 || z <= 0  || x >= WORLD_SIZE-1 || z >= WORLD_SIZE-1) return true;

    //4 corners
    if(SolidAtPoint(Vector3(pos.x-playerCollisionSize,pos.y,pos.z-playerCollisionSize))) return true;
    if(SolidAtPoint(Vector3(pos.x+playerCollisionSize,pos.y,pos.z+playerCollisionSize))) return true;
    if(SolidAtPoint(Vector3(pos.x+playerCollisionSize,pos.y,pos.z-playerCollisionSize))) return true;
    if(SolidAtPoint(Vector3(pos.x-playerCollisionSize,pos.y,pos.z+playerCollisionSize))) return true;
    //
    return false;
}


bool CraftWorld::SolidAtPointForPlayer(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    //outside the world so collision
    if (y <= 0 || y >= WORLD_HEIGHT-1) return false;
    if (x <= 0 || z <= 0  || x >= WORLD_SIZE-1 || z >= WORLD_SIZE-1) return true;

    //4 corners
    if(SolidAtPoint(Vector3(pos.x-playerCollisionSize,pos.y,pos.z-playerCollisionSize))) return true;
    if(SolidAtPoint(Vector3(pos.x+playerCollisionSize,pos.y,pos.z+playerCollisionSize))) return true;
    if(SolidAtPoint(Vector3(pos.x+playerCollisionSize,pos.y,pos.z-playerCollisionSize))) return true;
    if(SolidAtPoint(Vector3(pos.x-playerCollisionSize,pos.y,pos.z+playerCollisionSize))) return true;
    //
    return false;
}

bool CraftWorld::SolidAtPointForMovingPlayer(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    //outside the world so collision
    if (y <= 0 || y >= WORLD_HEIGHT-1) return false;
    if (x <= 0 || z <= 0  || x >= WORLD_SIZE-1 || z >= WORLD_SIZE-1) return true;

    //4 corners
    if(SolidAtPoint(Vector3(pos.x-playerCollisionSize,pos.y,pos.z-playerCollisionSize))) return true;
    if(SolidAtPoint(Vector3(pos.x+playerCollisionSize,pos.y,pos.z+playerCollisionSize))) return true;
    if(SolidAtPoint(Vector3(pos.x+playerCollisionSize,pos.y,pos.z-playerCollisionSize))) return true;
    if(SolidAtPoint(Vector3(pos.x-playerCollisionSize,pos.y,pos.z+playerCollisionSize))) return true;
    //
    return false;
}

bool CraftWorld::SolidAtPointForPlayerHead(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    //outside the world so collision
    if (y <= 0 || y >= WORLD_HEIGHT-1) return false;
    if (x <= 0 || z <= 0  || x >= WORLD_SIZE-1 || z >= WORLD_SIZE-1) return true;

    float collisionSize = playerCollisionSize-0.125f;
    //4 corners
    if(SolidAtPoint(Vector3(pos.x-collisionSize,pos.y,pos.z-collisionSize))) return true;
    if(SolidAtPoint(Vector3(pos.x+collisionSize,pos.y,pos.z+collisionSize))) return true;
    if(SolidAtPoint(Vector3(pos.x+collisionSize,pos.y,pos.z-collisionSize))) return true;
    if(SolidAtPoint(Vector3(pos.x-collisionSize,pos.y,pos.z+collisionSize))) return true;
    //
    return false;
}

bool CraftWorld::SolidAtPointForEntity(Vector3 pos, Vector3 collider1)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    float xSize = collider1.x;
    float zSize = collider1.z;

    //outside the world so collision
    if (y <= 0 || y >= WORLD_HEIGHT-1) return false;
    if (x <= 0 || z <= 0  || x >= WORLD_SIZE-1 || z >= WORLD_SIZE-1) return true;
    //4 corners
    if(SolidAtPoint(Vector3(pos.x-xSize,pos.y,pos.z-zSize))) return true;
    if(SolidAtPoint(Vector3(pos.x+xSize,pos.y,pos.z+zSize))) return true;
    if(SolidAtPoint(Vector3(pos.x+xSize,pos.y,pos.z-zSize))) return true;
    if(SolidAtPoint(Vector3(pos.x-xSize,pos.y,pos.z+zSize))) return true;
    //
    return false;
}

bool CraftWorld::HighStairAtPoint(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    float relativePlayer_x = pos.x-(float)x;
    //float relativePlayer_y = pos.y-(float)y;
    float relativePlayer_z = pos.z-(float)z;

    bool highPoint = false;

    //outside the world so collision
    if (x <= 0 || y <= 0 || z <= 0  || x >= WORLD_SIZE-1 || y >= WORLD_HEIGHT-1 || z >= WORLD_SIZE-1) return true;

    int posBlockID = GetBlock(x,y,z);
    bool stairsFound = false;

    if(blockTypes[posBlockID].blockModel == 9)
    {
        stairsFound = true;
        if(relativePlayer_x < 0.5f)
        {
            highPoint = true;
        }
    }
    if(blockTypes[posBlockID].blockModel == 8)
    {
        stairsFound = true;
        if(relativePlayer_x >= 0.5f)
        {
            highPoint = true;
        }
    }
    if(blockTypes[posBlockID].blockModel == 7)
    {
        stairsFound = true;
        if(relativePlayer_z < 0.5f)
        {
            highPoint = true;
        }
    }
    if(blockTypes[posBlockID].blockModel == 6)
    {
        stairsFound = true;
        if(relativePlayer_z >= 0.5f)
        {
            highPoint = true;
        }
    }

    if(stairsFound == false)
    {
        if(SolidAtPoint(Vector3(pos.x,pos.y+0.5f,pos.z)))
        {
            highPoint = true;
        }
    }

    return highPoint;
}

bool CraftWorld::PlayerInWater(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    if (x < 0 || y < 0 || z < 0  || x >= WORLD_SIZE-1 || y >= WORLD_HEIGHT-1 || z >= WORLD_SIZE-1) return false;

    if(GetBlockNoCheck(x,y,z) == 4 || GetBlockNoCheck(x,y,z) == 32)
        return true;

    return false;
}

bool CraftWorld::PlayerOnHalfBlock(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;
    int points = 0;
    int badPoints = 0;
    if (x < 0 || y < 0 || z < 0  || x >= WORLD_SIZE-1 || y >= WORLD_HEIGHT-1 || z >= WORLD_SIZE-1) return false;

    int posBlockID = BlockAtPoint(Vector3(pos.x-playerCollisionSize,pos.y,pos.z-playerCollisionSize));
    if(blockTypes[posBlockID].blockModel == 1 ||
       blockTypes[posBlockID].solid == false  ||
       (blockTypes[posBlockID].blockModel >= 6 && blockTypes[posBlockID].blockModel <= 9 && !HighStairAtPoint(Vector3(pos.x-playerCollisionSize,pos.y,pos.z-playerCollisionSize)))
       )
    {
        if(blockTypes[posBlockID].solid == false)
        {
            badPoints++;
        }
        points ++;
    }


    posBlockID = BlockAtPoint(Vector3(pos.x+playerCollisionSize,pos.y,pos.z-playerCollisionSize));
    if(blockTypes[posBlockID].blockModel == 1 ||
       blockTypes[posBlockID].solid == false  ||
       (blockTypes[posBlockID].blockModel >= 6 && blockTypes[posBlockID].blockModel <= 9 && !HighStairAtPoint(Vector3(pos.x+playerCollisionSize,pos.y,pos.z-playerCollisionSize)))
       )
    {
        if(blockTypes[posBlockID].solid == false)
        {
            badPoints++;
        }
        points ++;
    }


    posBlockID = BlockAtPoint(Vector3(pos.x-playerCollisionSize,pos.y,pos.z+playerCollisionSize));
    if(blockTypes[posBlockID].blockModel == 1 ||
       blockTypes[posBlockID].solid == false  ||
       (blockTypes[posBlockID].blockModel >= 6 && blockTypes[posBlockID].blockModel <= 9 && !HighStairAtPoint(Vector3(pos.x-playerCollisionSize,pos.y,pos.z+playerCollisionSize)))
       )
    {
        if(blockTypes[posBlockID].solid == false)
        {
            badPoints++;
        }
        points ++;
    }

    posBlockID = BlockAtPoint(Vector3(pos.x+playerCollisionSize,pos.y,pos.z+playerCollisionSize));
    if(blockTypes[posBlockID].blockModel == 1 ||
       blockTypes[posBlockID].solid == false  ||
       (blockTypes[posBlockID].blockModel >= 6 && blockTypes[posBlockID].blockModel <= 9 && !HighStairAtPoint(Vector3(pos.x+playerCollisionSize,pos.y,pos.z+playerCollisionSize)))
       )
    {
        if(blockTypes[posBlockID].solid == false)
        {
            badPoints++;
        }
        points ++;
    }

    if(points == 4 && badPoints != 4)
    {
        return true;
    }
    return false;
}

bool CraftWorld::PlayerOnStairsBlock(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    if (x < 0 || y < 0 || z < 0  || x >= WORLD_SIZE-1 || y >= WORLD_HEIGHT-1 || z >= WORLD_SIZE-1) return false;

    int posBlockID = BlockAtPoint(Vector3(pos.x-playerCollisionSize,pos.y,pos.z-playerCollisionSize));
    if(blockTypes[posBlockID].blockModel >= 6 && blockTypes[posBlockID].blockModel <= 9)
    {
        return true;
    }
    posBlockID = BlockAtPoint(Vector3(pos.x+playerCollisionSize,pos.y,pos.z-playerCollisionSize));
    if(blockTypes[posBlockID].blockModel >= 6 && blockTypes[posBlockID].blockModel <= 9)
    {
        return true;
    }

    posBlockID = BlockAtPoint(Vector3(pos.x-playerCollisionSize,pos.y,pos.z+playerCollisionSize));
    if(blockTypes[posBlockID].blockModel >= 6 && blockTypes[posBlockID].blockModel <= 9)
    {
        return true;
    }

    posBlockID = BlockAtPoint(Vector3(pos.x+playerCollisionSize,pos.y,pos.z+playerCollisionSize));
    if(blockTypes[posBlockID].blockModel >= 6 && blockTypes[posBlockID].blockModel <= 9)
    {
        return true;
    }
    return false;
}

bool CraftWorld::PlayerOnHighStair(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    bool onHighStair = false;

    if (x < 0 || y < 0 || z < 0  || x >= WORLD_SIZE-1 || y >= WORLD_HEIGHT-1 || z >= WORLD_SIZE-1) return false;

    if(HighStairAtPoint(Vector3(pos.x-playerCollisionSize,pos.y,pos.z-playerCollisionSize)) ||
       HighStairAtPoint(Vector3(pos.x+playerCollisionSize,pos.y,pos.z-playerCollisionSize)) ||
       HighStairAtPoint(Vector3(pos.x-playerCollisionSize,pos.y,pos.z+playerCollisionSize)) ||
       HighStairAtPoint(Vector3(pos.x+playerCollisionSize,pos.y,pos.z+playerCollisionSize)))
    {
        onHighStair = true;
    }
    else
    {
        onHighStair = false;
    }


    return onHighStair;
}

bool CraftWorld::PlayerInLava(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    if (x < 0 || y < 0 || z < 0  || x >= WORLD_SIZE-1 || y >= WORLD_HEIGHT-1 || z >= WORLD_SIZE-1) return false;

    if(GetBlock(x,y,z) == Lava::getID())
        return true;

    return false;
}

bool CraftWorld::PlayerInBlock(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    if (x < 0 || y < 0 || z < 0  || x >= WORLD_SIZE-1 || y >= WORLD_HEIGHT-1 || z >= WORLD_SIZE-1) return false;

    if(GetBlock(x,y,z) != Lava::getID() || GetBlock(x,y,z) != 4 || GetBlock(x,y,z) != 0)
        return true;

    return false;
}

bool CraftWorld::PlayerCollisionWithCactus(Vector3 pos)
{
    float x = pos.x;
    float y = pos.y;
    float z = pos.z;

    float collision_distance = 1.05f;

    if(GetBlock(x-1,y,z) == CactusBlock::getID())
    {
        float cactus_x = (int)(x-1) + 0.5f;

        if(abs(x-cactus_x) <= collision_distance)
        {
            return true;
        }
    }
    if(GetBlock(x+1,y,z) == CactusBlock::getID())
    {
        float cactus_x = (int)(x+1) + 0.5f;

        if(abs(x-cactus_x) <= collision_distance)
        {
            return true;
        }
    }

    if(GetBlock(x,y,z-1) == CactusBlock::getID())
    {
        float cactus_z = (int)(z-1) + 0.5f;

        if(abs(z-cactus_z) <= collision_distance)
        {
            return true;
        }
    }
    if(GetBlock(x,y,z+1) == CactusBlock::getID())
    {
        float cactus_z = (int)(z+1) + 0.5f;

        if(abs(z-cactus_z) <= collision_distance)
        {
            return true;
        }
    }

    return false;
}

bool CraftWorld::PlayerCollisionWithLadder(Vector3 pos)
{
    float x = pos.x;
    float y = pos.y;
    float z = pos.z;

    float pixel = 10.0f/16.0f;

    if (x < 0 || y < 0 || z < 0  || x >= WORLD_SIZE-1 || y >= WORLD_HEIGHT-1 || z >= WORLD_SIZE-1) return false;

    if(GetBlock(x,y,z) == Ladder1::getID())
    {
        if(BlockSolid(x-pixel,y,z) || BlockSolid(x-pixel,y-0.7f,z) || BlockSolid(x-pixel,y-1.55f,z))
        {
            return true;
        }
    }
    if(GetBlock(x,y,z) == Ladder2::getID())
    {
        if(BlockSolid(x+pixel,y,z) || BlockSolid(x+pixel,y-0.7f,z) || BlockSolid(x+pixel,y-1.55f,z))
        {
            return true;
        }
    }
    if(GetBlock(x,y,z) == Ladder3::getID())
    {
        if(BlockSolid(x,y,z-pixel) || BlockSolid(x,y-0.7f,z-pixel) || BlockSolid(x,y-1.55f,z-pixel))
        {
            return true;
        }
    }
    if(GetBlock(x,y,z) == Ladder4::getID())
    {
        if(BlockSolid(x,y,z+pixel) || BlockSolid(x,y-0.7f,z+pixel) || BlockSolid(x,y-1.55f,z+pixel))
        {
            return true;
        }
    }

    return false;
}

block_t CraftWorld::BlockAtPoint(Vector3 pos)
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    float relative_x = (float)pos.x-x;
    float relative_y = (float)pos.y-y;
    float relative_z = (float)pos.z-z;

    if (x < 0 || y < 0 || z < 0  || x >= WORLD_SIZE || y >= WORLD_HEIGHT || z >= WORLD_SIZE) return -1;

    block_t block = GetBlock(x,y,z);

    if(blockTypes[block].blockModel == 1)
    {
        if(relative_y < 0.5f)
        {
            return block;
        }
        else
        {
            return 0;
        }
    }

    if(blockTypes[block].blockModel == 9)
    {
        if(relative_x < 0.5f)
        {
            return block;
        }
        else
        {
            if(relative_y < 0.5f)
            {
                return block;
            }
            else
            {
                return 0;
            }
        }
    }
    if(blockTypes[block].blockModel == 8)
    {
        if(relative_x >= 0.5f)
        {
            return block;
        }
        else
        {
            if(relative_y < 0.5f)
            {
                return block;
            }
            else
            {
                return 0;
            }
        }
    }
    if(blockTypes[block].blockModel == 7)
    {
        if(relative_z < 0.5f)
        {
            return block;
        }
        else
        {
            if(relative_y < 0.5f)
            {
                return block;
            }
            else
            {
                return 0;
            }
        }
    }
    if(blockTypes[block].blockModel == 6)
    {
        if(relative_z >= 0.5f)
        {
            return block;
        }
        else
        {
            if(relative_y < 0.5f)
            {
                return block;
            }
            else
            {
                return 0;
            }
        }
    }

    return block;
}

float CraftWorld::LengthDirX(float horAngle, float verAngle)
{
    return -sinf(horAngle) * cosf(verAngle);
}

float CraftWorld::LengthDirY(float horAngle, float verAngle)
{
    return sinf(verAngle);
}

float CraftWorld::LengthDirZ(float horAngle, float verAngle)
{
    return cosf(horAngle) * cosf(verAngle);
}

float CraftWorld::Distance3d(float x1, float y1, float z1, float x2, float y2, float z2)
{
    return sqrt(pow((x1-x2),2)+pow((y1-y2),2)+pow((z1-z2),2));
}

float CraftWorld::Distance2d(float x1, float y1, float x2, float y2)
{
    return sqrt(pow((x1-x2),2)+pow((y1-y2),2));
}

unsigned int CraftWorld::FastDistance2d(unsigned int dx, unsigned int dy)
{
    unsigned int w;
    if ( dy < dx ) {
        w = dy >> 2;
        return (dx + w + (w >> 1));
    }
    else {
        w = dx >> 2;
        return (dy + w + (w >> 1));
    }
}


float CraftWorld::TaxicabDistance2d(float x1, float y1, float x2, float y2)
{
    return abs(x1-x2)+abs(y1-y2);
}


void CraftWorld::BuildExplodeSphere(float radius, int X, int Y, int Z)
{
    std::vector<int> toUpdate2;
    std::vector<int> dropIDs;

    float reference = 125 / 20.0f;

    float latitudes = (float)(radius * reference);
    float longitudes = (float)(latitudes/2);

    float latitude_increment = PI*2 / latitudes;
    float longitude_increment = PI / longitudes;

    bool wasLight = false;

    for (float u = 0; u < PI*2; u += latitude_increment)
    {
        for (float t = 0; t < PI; t += longitude_increment)
        {

            float rad = radius;

            int x = X + (rad * sinf(t) * sinf(u));
            int y = Y + (rad * cosf(t));
            int z = Z + (rad * sinf(t) * cosf(u));

            if(x < 0 || x >= WORLD_SIZE || y < 0 || y >= WORLD_HEIGHT || z < 0 || z >= WORLD_SIZE)
            {
                continue;
            }

            if(x > X)
            {
                for(int x2 = x; x2 >= X - (x2-X); x2 --)
                {
                    block_t& block = GetBlock(x2,y,z);
                    if(block == IronBlock::getID() || block == WaterBlock::getID() || block == 0)
                    {
                        continue;
                    }

                    if(block == TNTBlock::getID())
                    {
                        Vector3 testPos1 = Vector3(x2,y,z);

                        SpawnTNTentity(((int)testPos1.x)+0.5f,((int)testPos1.y)+0.5f,((int)testPos1.z)+0.5f,0.25f+(rand()%11)*0.25f);

                        bool pushToUpdate = true;
                        int id = getChunkId(testPos1);
                        for(unsigned int i = 0; i < toUpdate2.size(); i++)
                        {
                            if(toUpdate2[i] == id)
                            {
                                pushToUpdate = false;
                            }
                        }

                        if(pushToUpdate == true)
                        {
                            toUpdate2.push_back(id);
                        }
                        block = 0;
                        continue;
                    }
                    if(LightSourceBlock(block))
                    {
                        RemoveLigtSourceAtPosition(x2,y,z,0);
                        wasLight = true;
                    }
                    if(block >= ItemFrame1::getID() && block <= ItemFrame4::getID())
                    {
                        DestroyItemFrameEntity(Vector3(x2,y,z));
                    }

                    switch(blockTypes[block].material)
                    {
                        case 1:
                        if(rand() % 12 < 11)
                        {
                            if(rand() % 10 == 0)
                            {
                                dropIDs.push_back(blockTypes[block].loot);
                                //DropThis(blockTypes[block].loot, Vector3(x2,y,z));
                            }
                            block = 0;

                        }
                        break;
                        case 2:
                        if(rand() % 12 < 11)
                        {
                            if(rand() % 10 == 0)
                            {
                                dropIDs.push_back(blockTypes[block].loot);
                            }
                            block = 0;
                        }
                        break;
                        case 3:
                        if(rand() % 10 == 0)
                        {
                            dropIDs.push_back(blockTypes[block].loot);
                        }
                        block = 0;
                        break;
                        case 4:
                        block = 0;
                        break;
                        case 7:
                        if(rand() % 4 > 0)
                        {
                            dropIDs.push_back(blockTypes[block].loot);
                        }
                        block = 0;
                        break;
                        case 0:
                        block = 0;
                        break;
                    }
                }
            }

            if(x < X)
            {
                for(int x2 = x; x2 <= X + (X-x2); x2 ++)
                {
                    block_t& block = GetBlock(x2,y,z);
                    if(block == IronBlock::getID() || block == WaterBlock::getID() || block == 0)
                    {
                        continue;
                    }

                    if(block == TNTBlock::getID())
                    {
                        Vector3 testPos1 = Vector3(x2,y,z);

                        SpawnTNTentity(((int)testPos1.x)+0.5f,((int)testPos1.y)+0.5f,((int)testPos1.z)+0.5f,0.25f+(rand()%11)*0.25f);
                        //RebuildOpaqueMeshChunk(getChunkId(testPos1));

                        bool pushToUpdate = true;
                        int id = getChunkId(testPos1);
                        for(unsigned int i = 0; i < toUpdate2.size(); i++)
                        {
                            if(toUpdate2[i] == id)
                            {
                                pushToUpdate = false;
                            }
                        }

                        if(pushToUpdate == true)
                        {
                            toUpdate2.push_back(id);
                        }
                        block = 0;
                        continue;
                    }
                    if(LightSourceBlock(block))
                    {
                        RemoveLigtSourceAtPosition(x2,y,z,0);
                        wasLight = true;
                    }
                    if(block >= ItemFrame1::getID() && block <= ItemFrame4::getID())
                    {
                        DestroyItemFrameEntity(Vector3(x2,y,z));
                    }
                    switch(blockTypes[block].material)
                    {
                        case 0:
                        block = 0;
                        break;
                        case 1:
                        if(rand() % 12 < 11)
                        {
                            if(rand() % 10 == 0)
                            {
                                dropIDs.push_back(blockTypes[block].loot);
                            }
                            block = 0;

                        }
                        break;
                        case 2:
                        if(rand() % 12 < 11)
                        {
                            if(rand() % 10 == 0)
                            {
                                dropIDs.push_back(blockTypes[block].loot);
                            }
                            block = 0;
                        }
                        break;
                        case 3:
                        if(rand() % 10 == 0)
                        {
                            dropIDs.push_back(blockTypes[block].loot);
                        }
                        block = 0;
                        break;
                        case 4:
                        block = 0;
                        break;
                        case 7:
                        if(rand() % 4 > 0)
                        {
                            dropIDs.push_back(blockTypes[block].loot);
                        }
                        block = 0;
                        break;
                    }
                }
            }
        }
    }

    for(int x2 = X-radius; x2 <= X+radius; x2++)
    {
        for(int z2 = Z-radius; z2 <= Z+radius; z2++)
        {
            initPutBlocksLight(x2,Y+radius+1,z2);
        }
    }

    //clever drop
    if(dropIDs.size() > 1)
    {
        std::sort(dropIDs.begin(), dropIDs.end());
        int temp = dropIDs[0];
        int num = 0;
        for(unsigned int i = 0; i < dropIDs.size(); i++)
        {
            if(dropIDs[i] != temp)
            {
                DropThis(temp,num,Vector3(X-1.5f+(rand()%7)*0.5f,Y-radius+1.5f,Z-1.5f+(rand()%7)*0.5));
                num = 0;
            }

            num ++;
            temp = dropIDs[i];

            if(i == dropIDs.size()-1)
            {
                DropThis(temp,num,Vector3(X-1.5f+(rand()%7)*0.5f,Y-radius+1.5f,Z-1.5f+(rand()%7)*0.5));
                break;
            }
        }
    }
    dropIDs.clear();

    if(wasLight)
    {
        UpdateLightAreaIn(Vector3(X,Y,Z));
    }

    for(unsigned int i = 0; i < toUpdate2.size(); i++)
    {
        RebuildFullMeshChunk(toUpdate2[i]);
    }
    toUpdate2.clear();
}

void CraftWorld::BuildBlockSphere(float radius, block_t block, int X, int Y, int Z)
{
    float reference = 125 / 20.0f;

    float latitudes = (float)(radius * reference);
    float longitudes = (float)(latitudes/2);

    float latitude_increment = PI*2 / latitudes;
    float longitude_increment = PI / longitudes;

    for (float u = 0; u < PI*2; u += latitude_increment)
    {
        for (float t = 0; t < PI; t += longitude_increment)
        {

            float rad = radius;

            int x = X + (rad * sinf(t) * sinf(u));
            int y = Y + (rad * cosf(t));
            int z = Z + (rad * sinf(t) * cosf(u));

            if(x > X)
            {
                for(int x2 = x; x2 >= X - (x2-X); x2 --)
                {
                    if(GetBlock(x2,y,z) == IronBlock::getID() || GetBlock(x2,y,z) == WaterBlock::getID())
                    {
                        continue;
                    }
                    GetBlock(x2,y,z) = block;
                }
            }

            if(x < X)
            {
                for(int x2 = x; x2 <= X + (X-x2); x2 ++)
                {
                    if(GetBlock(x2,y,z) == IronBlock::getID() || GetBlock(x2,y,z) == WaterBlock::getID())
                    {
                        continue;
                    }
                    GetBlock(x2,y,z) = block;
                }
            }
        }
    }
}

void CraftWorld::BuildBlockEllipsoid(float w_radius, float h_radius, block_t block, int X, int Y, int Z)
{
    float reference = 125 / 20.0f;

    float latitudes = (float)(w_radius * reference);
    float longitudes = (float)((h_radius * reference) /2.0f);

    float latitude_increment = PI*2 / latitudes;
    float longitude_increment = PI / longitudes;

    for (float u = 0; u < PI*2; u += latitude_increment)
    {
        for (float t = 0; t < PI; t += longitude_increment)
        {

           // float rad = radius;

            int x = X + (w_radius * sinf(t) * sinf(u));
            int y = Y + (h_radius * cosf(t));
            int z = Z + (w_radius * sinf(t) * cosf(u));

            if(x > X)
            {
                for(int x2 = x; x2 >= X - (x2-X); x2 --)
                {
                    if(GetBlock(x2,y,z) == IronBlock::getID() || GetBlock(x2,y,z) == WaterBlock::getID())
                    {
                        continue;
                    }
                    GetBlock(x2,y,z) = block;
                }
            }

            if(x < X)
            {
                for(int x2 = x; x2 <= X + (X-x2); x2 ++)
                {
                    if(GetBlock(x2,y,z) == IronBlock::getID() || GetBlock(x2,y,z) == WaterBlock::getID())
                    {
                        continue;
                    }
                    GetBlock(x2,y,z) = block;
                }
            }
        }
    }
}

void CraftWorld::BuildBlockSphere(float radius, block_t block, int X, int Y, int Z, block_t blockToChange)
{
    float reference = 125 / 20.0f;

    float latitudes = (float)(radius * reference);
    float longitudes = (float)(latitudes/2);

    float latitude_increment = PI*2 / latitudes;
    float longitude_increment = PI / longitudes;

    for (float u = 0; u < PI*2; u += latitude_increment)
    {
        for (float t = 0; t < PI; t += longitude_increment)
        {

            float rad = radius;

            int x = X + (rad * sinf(t) * sinf(u));
            int y = Y + (rad * cosf(t));
            int z = Z + (rad * sinf(t) * cosf(u));

            if(x > X)
            {
                for(int x2 = x; x2 >= X - (x2-X); x2 --)
                {
                    if (GetBlock(x2,y,z) == blockToChange)
                    {
                        if(GetBlock(x2,y,z) == IronBlock::getID() || GetBlock(x2,y,z) == WaterBlock::getID())
                        {
                            continue;
                        }
                        GetBlock(x2,y,z) = block;
                    }
                }
            }

            if(x < X)
            {
                for(int x2 = x; x2 <= X + (X-x2); x2 ++)
                {
                    if (GetBlock(x2,y,z) == blockToChange)
                    {
                        if(GetBlock(x2,y,z) == IronBlock::getID() || GetBlock(x2,y,z) == WaterBlock::getID())
                        {
                            continue;
                        }
                        GetBlock(x2,y,z) = block;
                    }
                }
            }
        }
    }
}

void CraftWorld::SetPlayerSpawnPosition(Vector3 newPosition)
{
    playerSpawnPointPosition = newPosition;
}

int CraftWorld::FetchArmorPoints(int bootsId, int leggingsId, int chestId, int helmetId)
{
    int armorPoints = 0;
    switch(helmetId)
    {
       case 328 : armorPoints += 1.0f; break;
       case 332 : armorPoints += 2.0f; break;
       case 336 : armorPoints += 2.0f; break;
       case 340 : armorPoints += 3.0f; break;
       case 344 : armorPoints += 2.0f; break;
    }
    switch(chestId)
    {
       case 329 : armorPoints += 3.0f; break;
       case 333 : armorPoints += 5.0f; break;
       case 337 : armorPoints += 6.0f; break;
       case 341 : armorPoints += 8.0f; break;
       case 345 : armorPoints += 5.0f; break;
    }
    switch(leggingsId)
    {
       case 330 : armorPoints += 2.0f; break;
       case 334 : armorPoints += 4.0f; break;
       case 338 : armorPoints += 5.0f; break;
       case 342 : armorPoints += 6.0f; break;
       case 346 : armorPoints += 3.0f; break;
    }
    switch(bootsId)
    {
       case 331 : armorPoints += 1.0f; break;
       case 335 : armorPoints += 1.0f; break;
       case 339 : armorPoints += 2.0f; break;
       case 343 : armorPoints += 3.0f; break;
       case 347 : armorPoints += 1.0f; break;
    }
    return armorPoints;
}

float CraftWorld::FetchDamagePoints(int weaponId, bool &weapon)
{
    int damagePoints = 1.0f;
    switch(weaponId)
    {
        //picks
        case 250 : damagePoints = 4.0f; weapon = true; break;
        case 251 : damagePoints = 5.0f; weapon = true; break;
        case 252 : damagePoints = 6.0f; weapon = true; break;
        case 253 : damagePoints = 7.0f; weapon = true; break;
        case 254 : damagePoints = 4.0f; weapon = true; break;

        //swords
        case 255 : damagePoints = 2.0f; weapon = true; break;
        case 256 : damagePoints = 3.0f; weapon = true; break;
        case 257 : damagePoints = 4.0f; weapon = true; break;
        case 258 : damagePoints = 5.0f; weapon = true; break;
        case 259 : damagePoints = 2.0f; weapon = true; break;

        //shovels
        case 260 : damagePoints = 1.0f; weapon = true; break;
        case 261 : damagePoints = 2.0f; weapon = true; break;
        case 262 : damagePoints = 3.0f; weapon = true; break;
        case 263 : damagePoints = 4.0f; weapon = true; break;
        case 264 : damagePoints = 1.0f; weapon = true; break;

        //axes
        case 265 : damagePoints = 3.0f; weapon = true; break;
        case 266 : damagePoints = 4.0f; weapon = true; break;
        case 267 : damagePoints = 5.0f; weapon = true; break;
        case 268 : damagePoints = 6.0f; weapon = true; break;
        case 269 : damagePoints = 3.0f; weapon = true; break;
    }
    return damagePoints;
}

bool CraftWorld::HaveItemInBarSlots(int id)
{
    for(int j = 27; j <= 35; j++)
    {
        if(invId[j] == id)
        {
            return true;
        }
    }
    return false;
}

void CraftWorld::PutInInventory(int id, int num, bool st)
{
    bool ok;
    ok = false;
    for(int j = 27; j <= 35; j++)
    {
        if (invSt[j]==1)
        {
            if (invId[j]== id)
            {
                if(invAm[j] + num <= 64)
                {
                    invAm[j] += num;
                    ok = true;
                    break;
                }
                else
                {
                    continue;
                }
            }
        }

        if (invId[j]== -1)
        {
            invSt[j] = st;
            invId[j] = id;
            invAm[j] = num;
            ok = true;
            break;
        }
    }

    if(ok == false)
    {
        for(int j = 0; j <= 26; j++)
        {
            if (invSt[j]==1)
            {
                if (invId[j]== id)
                {
                    if(invAm[j] + num <= 64)
                    {
                        invAm[j] += num;
                        break;
                        return;
                    }
                    else
                    {
                        continue;
                    }
                }
            }

            if (invId[j]== -1)
            {
                invSt[j] = st;
                invId[j] = id;
                invAm[j] = num;
                break;
                return;
            }
        }
    }
}


void CraftWorld::PutInInventory(int id, int num, bool st, int& slot)
{
    slot = -1; // some animation stuff


    bool ok;
    ok = false;
    for(int j = 27; j <= 35; j++)
    {
        if (invSt[j]==1)
        {
            if (invId[j]== id)
            {
                if(invAm[j] + num <= 64)
                {
                    invAm[j] += num;
                    ok = true;
                    slot = j-27;
                    break;
                }
                else
                {
                    continue;
                }
            }
        }

        if (invId[j]== -1)
        {
            invSt[j] = st;
            invId[j] = id;
            invAm[j] = num;
            ok = true;
            slot = j-27;
            break;
        }
    }

    if(ok == false)
    {
        for(int j = 0; j <= 26; j++)
        {
            if (invSt[j]==1)
            {
                if (invId[j]== id)
                {
                    if(invAm[j] + num <= 64)
                    {
                        invAm[j] += num;
                        break;
                        return;
                    }
                    else
                    {
                        continue;
                    }
                }
            }

            if (invId[j]== -1)
            {
                invSt[j] = st;
                invId[j] = id;
                invAm[j] = num;
                break;
                return;
            }
        }
    }
}

bool CraftWorld::InventoryIsFull(int id, int num, bool st)
{
    bool ok;
    ok = false;
    for(int j = 27; j <= 35; j++)
    {
        if (invSt[j]==1)
        {
            if (invId[j]== id)
            {
                if(invAm[j] + num <= 64)
                {
                    ok = true;
                    return false;
                }
                else
                {
                    continue;
                }
            }
        }

        if (invId[j]== -1)
        {
            ok = true;
            return false;
            break;

        }
    }

    if(ok == false)
    {
        for(int j = 0; j <= 26; j++)
        {
            if (invSt[j]==1)
            {
                if (invId[j]== id)
                {
                    if(invAm[j] + num <= 64)
                    {
                        return false;
                        break;

                    }
                    else
                    {
                        continue;
                    }
                }
            }

            if (invId[j]== -1)
            {
                return false;
                break;
            }
        }
    }
    return true;
}

int CraftWorld::FindDirId(int x, int y, int z)
{
    int o;
    o = -1;
    for(unsigned int i = 0; i < mDirects.size(); i++)
    {
        DirectionBlock* NewDirectionBlock = mDirects[i];
        if(NewDirectionBlock->X == x && NewDirectionBlock->Y == y && NewDirectionBlock->Z == z)
        {
            o = i;
            break;
        }
    }

    return o;
}

void CraftWorld::RebuildVisibleChunks()
{
    for(unsigned int i = 0; i < mChunks.size(); i++)
    {
        if(playerZoneBB.intersect(mChunks[i]->bBox))
        {
            RebuildFullMeshChunk(i);
        }
    }
}

void CraftWorld::BuildWorldBlockPlaneNoCheck(BaseBlock *blockType, int x,int y, int z, int &iVertex, SimpleMeshChunk* MeshChunk, block_t Block, bool transparent, char side, Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4)
{
    float pixel, texturePixel;

    pixel = (float)1/16.0f;
    texturePixel = (float)1/256.0f;

    bool canCreate = false;
    //bool DefaultBlock = false;
    //bool transparentBlock = false;

    float up = 0.0f;
    float down = 0.0f;
    float left = 0.0f;
    float right = 0.0f;

    char Block1 = 0;

    float light[20];
    for(int o = 0; o <= 19; o++)
    {
        light[o] = -1;
    }
    bool onCorners = false;
    if(x == 0 || y == 0 || z == 0 || x == WORLD_SIZE-1 || y == WORLD_HEIGHT-1 || z == WORLD_SIZE-1)
    {
        onCorners = true;
    }

    Vector3 light1, light2, light3, light4;

    if(side == 1)
    {
        // x y z
        // 0 0 1
        // 0 1 1
        // 0 1 0
        // 0 0 0
        Vector3 BlockColorx1;
        canCreate = true;

        if(canCreate == true)
        {
            float mainLight = BlockFinalLight(x-1,y,z);
            BlockColorx1 = Vector3(mainLight,mainLight,mainLight) * blockXConstLight;

            if(westLight != 0)
            {
                BlockColorx1.x += 0.3*westLight;
                BlockColorx1.y += 0.2*westLight;
            }

            light1 = light2 = light3 = light4 = BlockColorx1;

            if(mainOptions.smoothLighting == true)
            {
                if(onCorners)
                {
                    light[0] = BlockFinalLight(x-1,y+1,z) ;
                    light[1] = BlockFinalLight(x-1,y,z+1) ;
                    light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    light[3] = BlockFinalLight(x-1,y,z-1) ;
                    light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    light[5] = BlockFinalLight(x-1,y-1,z) ;
                    light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                }
                else
                {
                    light[0] = BlockFinalLightNoCheck(x-1,y+1,z) ;
                    light[1] = BlockFinalLightNoCheck(x-1,y,z+1) ;
                    light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    light[3] = BlockFinalLightNoCheck(x-1,y,z-1) ;
                    light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    light[5] = BlockFinalLightNoCheck(x-1,y-1,z) ;
                    light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                }

                float light1noV = (light[0]+light[1]+light[2]+mainLight)/4.0f;
                float light2noV = (light[0]+light[3]+light[4]+mainLight)/4.0f;
                float light3noV = (light[5]+light[1]+light[6]+mainLight)/4.0f;
                float light4noV = (light[5]+light[3]+light[7]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x-1,y+1,z) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x-1,y+1,z) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockXConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockXConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockXConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockXConstLight;
            }

            left = percent*blockType->leftPlane_x + point4.z*texturePixel;
            right = percent*blockType->leftPlane_x+percent - (16-point2.z)*texturePixel;

            up = percent*blockType->leftPlane_y + (16-point2.y)*texturePixel;
            down = percent*blockType->leftPlane_y + (16-point4.y)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light3.x,light3.y,light3.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 2)
    {
        // x y z
        // 1 0 0
        // 1 1 0
        // 1 1 1
        // 1 0 1

        Vector3 BlockColorx2;
        canCreate = true;

        if (canCreate)
        {
            float mainLight = BlockFinalLight(x+1,y,z);
            BlockColorx2 = Vector3(mainLight,mainLight,mainLight) * blockXConstLight;

            if(eastLight != 0)
            {
                BlockColorx2.x += 0.3*eastLight;
                BlockColorx2.y += 0.2*eastLight;
            }

            light1 = light2 = light3 = light4 = BlockColorx2;

            if(mainOptions.smoothLighting == true)
            {
                if(onCorners)
                {
                    light[8] = BlockFinalLight(x+1,y+1,z) ;
                    light[9] = BlockFinalLight(x+1,y,z+1) ;
                    light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    light[11] = BlockFinalLight(x+1,y,z-1) ;
                    light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    light[13] = BlockFinalLight(x+1,y-1,z) ;
                    light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                }
                else
                {
                    light[8] = BlockFinalLightNoCheck(x+1,y+1,z) ;
                    light[9] = BlockFinalLightNoCheck(x+1,y,z+1) ;
                    light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    light[11] = BlockFinalLightNoCheck(x+1,y,z-1) ;
                    light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    light[13] = BlockFinalLightNoCheck(x+1,y-1,z) ;
                    light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                }

                float light1noV = (light[8]+light[9]+light[10]+mainLight)/4.0f;
                float light2noV = (light[8]+light[11]+light[12]+mainLight)/4.0f;
                float light3noV = (light[13]+light[9]+light[14]+mainLight)/4.0f;
                float light4noV = (light[13]+light[11]+light[15]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockXConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockXConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockXConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockXConstLight;
            }

            left = percent*blockType->rightPlane_x + point1.z*texturePixel;
            right = percent*blockType->rightPlane_x+percent - (16-point3.z)*texturePixel;

            up = percent*blockType->rightPlane_y + (16-point3.y)*texturePixel;
            down = percent*blockType->rightPlane_y + (16-point1.y)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 3)
    {
        // x z y
        // 0 0 0
        // 1 0 0
        // 1 1 0
        // 0 1 0

        Vector3 BlockColory2;
        canCreate = true;

        if (canCreate)
        {
            float mainLight = BlockFinalLight(x,y-1,z);
            BlockColory2 = Vector3(mainLight,mainLight,mainLight) * 0.6f;
            light1 = light2 = light3 = light4 = BlockColory2;

            if(mainOptions.smoothLighting == true)
            {
                if(onCorners)
                {
                    if(light[5] == -1)
                    {
                        light[5] = BlockFinalLight(x-1,y-1,z) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                    }

                    light[16] = BlockFinalLight(x,y-1,z-1) ;

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                    }

                    if(light[13] == -1)
                    {
                        light[13] = BlockFinalLight(x+1,y-1,z) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    }

                    light[17] = BlockFinalLight(x,y-1,z+1) ;
                }
                else
                {
                    if(light[5] == -1)
                    {
                        light[5] = BlockFinalLightNoCheck(x-1,y-1,z) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                    }

                    light[16] = BlockFinalLightNoCheck(x,y-1,z-1) ;

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                    }

                    if(light[13] == -1)
                    {
                        light[13] = BlockFinalLightNoCheck(x+1,y-1,z) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    }

                    light[17] = BlockFinalLightNoCheck(x,y-1,z+1) ;
                }

                float light1noV = (light[5]+light[7]+light[16]+mainLight)/4.0f;
                float light2noV = (light[16]+light[15]+light[13]+mainLight)/4.0f;
                float light3noV = (light[13]+light[14]+light[17]+mainLight)/4.0f;
                float light4noV = (light[17]+light[6]+light[5]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y-1,z-1) || !BlockTransparentOrLightSource(x,y-1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y-1,z+1) || !BlockTransparentOrLightSource(x,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x,y-1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light1noV,light1noV,light1noV)*0.6f;
                light2 = Vector3(light2noV,light2noV,light2noV)*0.6f;
                light3 = Vector3(light3noV,light3noV,light3noV)*0.6f;
                light4 = Vector3(light4noV,light4noV,light4noV)*0.6f;
            }

            left = percent*blockType->downPlane_x + point1.x*texturePixel;
            right = percent*blockType->downPlane_x+percent - (16-point3.x)*texturePixel;

            up = percent*blockType->downPlane_y + (16-point1.z)*texturePixel;
            down = percent*blockType->downPlane_y + (16-point3.z)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light4.x,light4.y,light4.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 4)
    {
        // x z y
        // 0 1 1
        // 1 1 1
        // 1 0 1
        // 0 0 1

        Vector3 BlockColory1;
        canCreate = true;

        if(canCreate)
        {
            float mainLight = BlockFinalLight(x,y+1,z);
            BlockColory1 = Vector3(mainLight,mainLight,mainLight);
            light1 = light2 = light3 = light4 = BlockColory1;

            if(mainOptions.smoothLighting == true)
            {
                if(onCorners)
                {
                    light[18] = BlockFinalLight(x,y+1,z+1) ;

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    }

                    if(light[0] == -1)
                    {
                        light[0] = BlockFinalLight(x-1,y+1,z) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    }

                    if(light[8] == -1)
                    {
                        light[8] = BlockFinalLight(x+1,y+1,z) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    }

                    light[19] = BlockFinalLight(x,y+1,z-1) ;

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    }
                }
                else
                {
                    light[18] = BlockFinalLightNoCheck(x,y+1,z+1) ;

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    }

                    if(light[0] == -1)
                    {
                        light[0] = BlockFinalLightNoCheck(x-1,y+1,z) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    }

                    if(light[8] == -1)
                    {
                        light[8] = BlockFinalLightNoCheck(x+1,y+1,z) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    }

                    light[19] = BlockFinalLightNoCheck(x,y+1,z-1) ;

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    }
                }

                float light1noV = (light[18]+light[2]+light[0]+mainLight)/4.0f;
                float light2noV = (light[18]+light[10]+light[8]+mainLight)/4.0f;
                float light3noV = (light[8]+light[12]+light[19]+mainLight)/4.0f;
                float light4noV = (light[19]+light[4]+light[0]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y+1,z-1) || !BlockTransparentOrLightSource(x,y+1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x,y+1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light1noV,light1noV,light1noV);
                light2 = Vector3(light2noV,light2noV,light2noV);
                light3 = Vector3(light3noV,light3noV,light3noV);
                light4 = Vector3(light4noV,light4noV,light4noV);
            }

            left = percent*blockType->upPlane_x + point4.x*texturePixel;
            right = percent*blockType->upPlane_x+percent - (16-point2.x)*texturePixel;

            up = percent*blockType->upPlane_y + (16-point2.z)*texturePixel;
            down = percent*blockType->upPlane_y + (16-point4.z)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light4.x,light4.y,light4.z);


            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 5)
    {
        // x y z
        // 0 1 0
        // 1 1 0
        // 1 0 0
        // 0 0 0

        Vector3 BlockColorz1;
        canCreate = true;

        if (canCreate)
        {
            float mainLight = BlockFinalLight(x,y,z-1);
            BlockColorz1 = Vector3(mainLight,mainLight,mainLight) * blockZConstLight;
            light1 = light2 = light3 = light4 = BlockColorz1;

            if(mainOptions.smoothLighting == true)
            {
                if(onCorners)
                {
                    if(light[19] == -1)
                    {
                        light[19] = BlockFinalLight(x,y+1,z-1) ;
                    }

                    if(light[3] == -1)
                    {
                        light[3] = BlockFinalLight(x-1,y,z-1) ;
                    }

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    }

                    if(light[11] == -1)
                    {
                        light[11] = BlockFinalLight(x+1,y,z-1) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    }

                    if(light[16] == -1)
                    {
                        light[16] = BlockFinalLight(x,y-1,z-1) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                    }

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                    }
                }
                else
                {
                    if(light[19] == -1)
                    {
                        light[19] = BlockFinalLightNoCheck(x,y+1,z-1) ;
                    }

                    if(light[3] == -1)
                    {
                        light[3] = BlockFinalLightNoCheck(x-1,y,z-1) ;
                    }

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    }

                    if(light[11] == -1)
                    {
                        light[11] = BlockFinalLightNoCheck(x+1,y,z-1) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    }

                    if(light[16] == -1)
                    {
                        light[16] = BlockFinalLightNoCheck(x,y-1,z-1) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                    }

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                    }
                }

                float light1noV = (light[19]+light[3]+light[4]+mainLight)/4.0f;
                float light2noV = (light[19]+light[11]+light[12]+mainLight)/4.0f;
                float light3noV = (light[16]+light[3]+light[7]+mainLight)/4.0f;
                float light4noV = (light[16]+light[11]+light[15]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y-1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockZConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockZConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockZConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockZConstLight;
            }

            left = percent*blockType->botPlane_x + point4.x*texturePixel;
            right = percent*blockType->botPlane_x+percent - (16-point2.x)*texturePixel;

            up = percent*blockType->botPlane_y + (16-point2.y)*texturePixel;
            down = percent*blockType->botPlane_y + (16-point4.y)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 6)
    {
        // x y z
        // 0 0 1
        // 1 0 1
        // 1 1 1
        // 0 1 1

        Vector3 BlockColorz2;
        canCreate = true;


        if (canCreate == true)
        {
            float mainLight = BlockFinalLight(x,y,z+1);
            BlockColorz2 = Vector3(mainLight,mainLight,mainLight) * blockZConstLight;
            light1 = light2 = light3 = light4 = BlockColorz2;

            if(mainOptions.smoothLighting == true)
            {
                if(onCorners)
                {
                    if(light[18] == -1)
                    {
                        light[18] = BlockFinalLight(x,y+1,z+1) ;
                    }

                    if(light[1] == -1)
                    {
                        light[1] = BlockFinalLight(x-1,y,z+1) ;
                    }

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    }

                    if(light[9] == -1)
                    {
                        light[9] = BlockFinalLight(x+1,y,z+1) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    }

                    if(light[17] == -1)
                    {
                        light[17] = BlockFinalLight(x,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    }
                }
                else
                {
                    if(light[18] == -1)
                    {
                        light[18] = BlockFinalLightNoCheck(x,y+1,z+1) ;
                    }

                    if(light[1] == -1)
                    {
                        light[1] = BlockFinalLightNoCheck(x-1,y,z+1) ;
                    }

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    }

                    if(light[9] == -1)
                    {
                        light[9] = BlockFinalLightNoCheck(x+1,y,z+1) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    }

                    if(light[17] == -1)
                    {
                        light[17] = BlockFinalLightNoCheck(x,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    }
                }

                float light1noV = (light[18]+light[1]+light[2]+mainLight)/4.0f;
                float light2noV = (light[18]+light[9]+light[10]+mainLight)/4.0f;
                float light3noV = (light[17]+light[1]+light[6]+mainLight)/4.0f;
                float light4noV = (light[17]+light[9]+light[14]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y-1,z+1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockZConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockZConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockZConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockZConstLight;
            }

            left = percent*blockType->forPlane_x + point1.x*texturePixel;
            right = percent*blockType->forPlane_x+percent - (16-point3.x)*texturePixel;

            up = percent*blockType->forPlane_y + (16-point3.y)*texturePixel;
            down = percent*blockType->forPlane_y + (16-point1.y)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light2.x,light2.y,light2.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }
}


void CraftWorld::BuildWorldBlockPlane(BaseBlock *blockType, int x,int y, int z, int &iVertex, SimpleMeshChunk* MeshChunk, block_t Block, bool transparent, char side, Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4)
{
    float pixel, texturePixel;

    pixel = (float)1/16.0f;
    texturePixel = (float)1/256.0f;

    bool canCreate = false;
    bool DefaultBlock = false;
    bool transparentBlock = false;
    bool specialBlock;

    float up = 0.0f;
    float down = 0.0f;
    float left = 0.0f;
    float right = 0.0f;

    char Block1 = 0;

    Vector3 light1, light2, light3, light4;
    Block = GetBlock(x,y,z);

    float light[20];
    for(int o = 0; o <= 19; o++)
    {
        light[o] = -1;
    }
    bool onCorners = false;
    if(x == 0 || y == 0 || z == 0 || x == WORLD_SIZE-1 || y == WORLD_HEIGHT-1 || z == WORLD_SIZE-1)
    {
        onCorners = true;
    }

    bool leavesBlock = false;
    if(Block == LeavesBlock::getID() || Block == BirchLeaves::getID() || Block == SpruceLeaves::getID())
    {
        leavesBlock = true;
    }

    BaseBlock *blockType1;

    if(side == 1)
    {
        // x y z
        // 0 0 1
        // 0 1 1
        // 0 1 0
        // 0 0 0
        Vector3 BlockColorx1;

        onCorners == true ? Block1 = _GetBlock(x-1,y,z) : Block1 = _GetBlockNoCheck(x-1,y,z);
        BaseBlock *blockType1;
        blockType1 = &blockTypes[Block1];

        transparentBlock = blockType1->transparent;
        specialBlock = blockType1->blockModel;
        if(transparent == true)
        {
            if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (specialBlock > 0 && Block1 != 4 && Block1 != 112 && Block1 != 9 && Block1 != 37 && Block1 != 38))
            {
                canCreate = true;
            }
            if (transparentBlock == false && specialBlock == 0)
            {
                canCreate = false;
            }
        }
        else
        {
            if (specialBlock > 0 && Block1 != Block)
            {
                canCreate = true;
            }
            if (transparentBlock == true || Block1 == 0)
            {
                canCreate = true;
            }
        }

        if(Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID())
        {
            if(mainOptions.fastRendering == true)
            {
                canCreate = false;
            }
        }

        if(canCreate == true)
        {
            float mainLight = BlockFinalLight(x-1,y,z);
            BlockColorx1 = Vector3(mainLight,mainLight,mainLight) * blockXConstLight;

            if(westLight != 0)
            {
                BlockColorx1.x += 0.3*westLight;
                BlockColorx1.y += 0.2*westLight;
            }

            light1 = Vector3(1,1,1);
            light2 = Vector3(1,1,1);
            light3 = Vector3(1,1,1);
            light4 = Vector3(1,1,1);

            light1 = light2 = light3 = light4 = BlockColorx1;

            if(mainOptions.smoothLighting == true)
            {
                if(onCorners)
                {
                    light[0] = BlockFinalLight(x-1,y+1,z) ;
                    light[1] = BlockFinalLight(x-1,y,z+1) ;
                    light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    light[3] = BlockFinalLight(x-1,y,z-1) ;
                    light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    light[5] = BlockFinalLight(x-1,y-1,z) ;
                    light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                }
                else
                {
                    light[0] = BlockFinalLightNoCheck(x-1,y+1,z) ;
                    light[1] = BlockFinalLightNoCheck(x-1,y,z+1) ;
                    light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    light[3] = BlockFinalLightNoCheck(x-1,y,z-1) ;
                    light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    light[5] = BlockFinalLightNoCheck(x-1,y-1,z) ;
                    light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                }

                float light1noV = (light[0]+light[1]+light[2]+mainLight)/4.0f;
                float light2noV = (light[0]+light[3]+light[4]+mainLight)/4.0f;
                float light3noV = (light[5]+light[1]+light[6]+mainLight)/4.0f;
                float light4noV = (light[5]+light[3]+light[7]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x-1,y+1,z) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x-1,y+1,z) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockXConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockXConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockXConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockXConstLight;
            }

            left = percent*blockType->leftPlane_x + point4.z*texturePixel;
            right = percent*blockType->leftPlane_x+percent - (16-point2.z)*texturePixel;

            up = percent*blockType->leftPlane_y + (16-point2.y)*texturePixel;
            down = percent*blockType->leftPlane_y + (16-point4.y)*texturePixel;

            if(leavesBlock && mainOptions.fastRendering == true)
            {
                left += percent;
                right += percent;
            }

            MeshChunk->vert(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel, right, down, light1.x,light1.y,light1.z);
            MeshChunk->vert(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel, right, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel, left, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel, left, down, light3.x,light3.y,light3.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 2)
    {
        // x y z
        // 1 0 0
        // 1 1 0
        // 1 1 1
        // 1 0 1

        Vector3 BlockColorx2;

        onCorners == true ? Block1 = _GetBlock(x+1,y,z) : Block1 = _GetBlockNoCheck(x+1,y,z);
        BaseBlock *blockType1;
        blockType1 = &blockTypes[Block1];

        transparentBlock = blockType1->transparent;
        specialBlock = blockType1->blockModel;
        if(transparent == true)
        {
            if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (specialBlock > 0 && Block1 != 4 && Block1 != 112 && Block1 != 9 && Block1 != 37 && Block1 != 38))
            {
                canCreate = true;
            }
            if (transparentBlock == false && specialBlock == 0)
            {
                canCreate = false;
            }
        }
        else
        {
            if (specialBlock > 0 && Block1 != Block)
            {
                canCreate = true;
            }
            if (transparentBlock == true || Block1 == 0)
            {
                canCreate = true;
            }
        }

        if(Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID())
        {
            if(mainOptions.fastRendering == true)
            {
                canCreate = false;
            }
        }

        if (canCreate)
        {
            float mainLight = BlockFinalLight(x+1,y,z);
            BlockColorx2 = Vector3(mainLight,mainLight,mainLight) * blockXConstLight;

            if(eastLight != 0)
            {
                BlockColorx2.x += 0.3*eastLight;
                BlockColorx2.y += 0.2*eastLight;
            }

            light1 = Vector3(1,1,1);
            light2 = Vector3(1,1,1);
            light3 = Vector3(1,1,1);
            light4 = Vector3(1,1,1);

            light1 = light2 = light3 = light4 = BlockColorx2;

            if(mainOptions.smoothLighting == true)
            {
                if(onCorners)
                {
                    light[8] = BlockFinalLight(x+1,y+1,z) ;
                    light[9] = BlockFinalLight(x+1,y,z+1) ;
                    light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    light[11] = BlockFinalLight(x+1,y,z-1) ;
                    light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    light[13] = BlockFinalLight(x+1,y-1,z) ;
                    light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                }
                else
                {
                    light[8] = BlockFinalLightNoCheck(x+1,y+1,z) ;
                    light[9] = BlockFinalLightNoCheck(x+1,y,z+1) ;
                    light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    light[11] = BlockFinalLightNoCheck(x+1,y,z-1) ;
                    light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    light[13] = BlockFinalLightNoCheck(x+1,y-1,z) ;
                    light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                }

                float light1noV = (light[8]+light[9]+light[10]+mainLight)/4.0f;
                float light2noV = (light[8]+light[11]+light[12]+mainLight)/4.0f;
                float light3noV = (light[13]+light[9]+light[14]+mainLight)/4.0f;
                float light4noV = (light[13]+light[11]+light[15]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockXConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockXConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockXConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockXConstLight;
            }

            left = percent*blockType->rightPlane_x + point1.z*texturePixel;
            right = percent*blockType->rightPlane_x+percent - (16-point3.z)*texturePixel;

            up = percent*blockType->rightPlane_y + (16-point3.y)*texturePixel;
            down = percent*blockType->rightPlane_y + (16-point1.y)*texturePixel;

            if(leavesBlock && mainOptions.fastRendering == true)
            {
                left += percent;
                right += percent;
            }

            MeshChunk->vert(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel, right, down, light3.x,light3.y,light3.z);
            MeshChunk->vert(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel, right, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel, left, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel, left, down, light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 3)
    {
        // x z y
        // 0 0 0
        // 1 0 0
        // 1 1 0
        // 0 1 0

        Vector3 BlockColory2;

        onCorners == true ? Block1 = _GetBlock(x,y-1,z) : Block1 = _GetBlockNoCheck(x,y-1,z);
        BaseBlock *blockType1;
        blockType1 = &blockTypes[Block1];

        transparentBlock = blockType1->transparent;
        specialBlock = blockType1->blockModel;
        if(transparent == true)
        {
            if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (specialBlock > 0 && Block1 != 4 && Block1 != 112 && Block1 != 9 && Block1 != 37 && Block1 != 38))
            {
                canCreate = true;
            }
            if (transparentBlock == false && specialBlock == 0)
            {
                canCreate = false;
            }
        }
        else
        {
            if (specialBlock > 0 && Block1 != Block)
            {
                canCreate = true;
            }
            if (transparentBlock == true || Block1 == 0)
            {
                canCreate = true;
            }
        }

        if(Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID())
        {
            if(mainOptions.fastRendering == true)
            {
                canCreate = false;
            }
        }

        if(Block == 25 && Block1 == 25)
        {
            canCreate = false;
        }

        if (canCreate)
        {
            float mainLight = BlockFinalLight(x,y-1,z);
            BlockColory2 = Vector3(mainLight,mainLight,mainLight) * 0.6f;

            light1 = Vector3(1,1,1);
            light2 = Vector3(1,1,1);
            light3 = Vector3(1,1,1);
            light4 = Vector3(1,1,1);

            light1 = light2 = light3 = light4 = BlockColory2;

            if(mainOptions.smoothLighting == true)
            {
                if(onCorners)
                {
                    if(light[5] == -1)
                    {
                        light[5] = BlockFinalLight(x-1,y-1,z) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                    }

                    light[16] = BlockFinalLight(x,y-1,z-1) ;

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                    }

                    if(light[13] == -1)
                    {
                        light[13] = BlockFinalLight(x+1,y-1,z) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    }

                    light[17] = BlockFinalLight(x,y-1,z+1) ;
                }
                else
                {
                    if(light[5] == -1)
                    {
                        light[5] = BlockFinalLightNoCheck(x-1,y-1,z) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                    }

                    light[16] = BlockFinalLightNoCheck(x,y-1,z-1) ;

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                    }

                    if(light[13] == -1)
                    {
                        light[13] = BlockFinalLightNoCheck(x+1,y-1,z) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    }

                    light[17] = BlockFinalLightNoCheck(x,y-1,z+1) ;
                }

                float light1noV = (light[5]+light[7]+light[16]+mainLight)/4.0f;
                float light2noV = (light[16]+light[15]+light[13]+mainLight)/4.0f;
                float light3noV = (light[13]+light[14]+light[17]+mainLight)/4.0f;
                float light4noV = (light[17]+light[6]+light[5]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y-1,z-1) || !BlockTransparentOrLightSource(x,y-1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y-1,z+1) || !BlockTransparentOrLightSource(x,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x,y-1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light1noV,light1noV,light1noV)*0.6f;
                light2 = Vector3(light2noV,light2noV,light2noV)*0.6f;
                light3 = Vector3(light3noV,light3noV,light3noV)*0.6f;
                light4 = Vector3(light4noV,light4noV,light4noV)*0.6f;
            }

            left = percent*blockType->downPlane_x + point1.x*texturePixel;
            right = percent*blockType->downPlane_x+percent - (16-point3.x)*texturePixel;

            up = percent*blockType->downPlane_y + (16-point1.z)*texturePixel;
            down = percent*blockType->downPlane_y + (16-point3.z)*texturePixel;

            if(leavesBlock && mainOptions.fastRendering == true)
            {
                left += percent;
                right += percent;
            }

            MeshChunk->vert(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel, left, up, light1.x,light1.y,light1.z);
            MeshChunk->vert(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel, right, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel, right, down, light3.x,light3.y,light3.z);
            MeshChunk->vert(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel, left, down, light4.x,light4.y,light4.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 4)
    {
        // x z y
        // 0 1 1
        // 1 1 1
        // 1 0 1
        // 0 0 1

        Vector3 BlockColory1;

        onCorners == true ? Block1 = _GetBlock(x,y+1,z) : Block1 = _GetBlockNoCheck(x,y+1,z);
        BaseBlock *blockType1;
        blockType1 = &blockTypes[Block1];

        transparentBlock = blockType1->transparent;
        specialBlock = blockType1->blockModel;
        if(transparent == true)
        {
            if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (specialBlock > 0 && Block1 != 4 && Block1 != 112 && Block1 != 9 && Block1 != 37 && Block1 != 38))
            {
                canCreate = true;
            }
            if (transparentBlock == false && specialBlock == 0)
            {
                canCreate = false;
            }
        }
        else
        {
            if (specialBlock > 0 && Block1 != Block)
            {
                canCreate = true;
            }
            if (transparentBlock == true || Block1 == 0)
            {
                canCreate = true;
            }
        }

        if(Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID())
        {
            if(mainOptions.fastRendering == true)
            {
                canCreate = false;
            }
        }

        if(Block1 == 93)
        {
            canCreate = false;
        }

        if(Block == 25 && Block1 == 25)
        {
            canCreate = false;
        }

        if(canCreate)
        {
            float mainLight = BlockFinalLight(x,y+1,z);
            BlockColory1 = Vector3(mainLight,mainLight,mainLight);

            light1 = Vector3(1,1,1);
            light2 = Vector3(1,1,1);
            light3 = Vector3(1,1,1);
            light4 = Vector3(1,1,1);

            light1 = light2 = light3 = light4 = BlockColory1;

            if(mainOptions.smoothLighting == true)
            {
                if(onCorners)
                {
                    light[18] = BlockFinalLight(x,y+1,z+1) ;

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    }

                    if(light[0] == -1)
                    {
                        light[0] = BlockFinalLight(x-1,y+1,z) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    }

                    if(light[8] == -1)
                    {
                        light[8] = BlockFinalLight(x+1,y+1,z) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    }

                    light[19] = BlockFinalLight(x,y+1,z-1) ;

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    }
                }
                else
                {
                    light[18] = BlockFinalLightNoCheck(x,y+1,z+1) ;

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    }

                    if(light[0] == -1)
                    {
                        light[0] = BlockFinalLightNoCheck(x-1,y+1,z) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    }

                    if(light[8] == -1)
                    {
                        light[8] = BlockFinalLightNoCheck(x+1,y+1,z) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    }

                    light[19] = BlockFinalLightNoCheck(x,y+1,z-1) ;

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    }
                }

                float light1noV = (light[18]+light[2]+light[0]+mainLight)/4.0f;
                float light2noV = (light[18]+light[10]+light[8]+mainLight)/4.0f;
                float light3noV = (light[8]+light[12]+light[19]+mainLight)/4.0f;
                float light4noV = (light[19]+light[4]+light[0]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y+1,z-1) || !BlockTransparentOrLightSource(x,y+1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x,y+1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light1noV,light1noV,light1noV);
                light2 = Vector3(light2noV,light2noV,light2noV);
                light3 = Vector3(light3noV,light3noV,light3noV);
                light4 = Vector3(light4noV,light4noV,light4noV);
            }

            left = percent*blockType->upPlane_x + point4.x*texturePixel;
            right = percent*blockType->upPlane_x+percent - (16-point2.x)*texturePixel;

            up = percent*blockType->upPlane_y + (16-point2.z)*texturePixel;
            down = percent*blockType->upPlane_y + (16-point4.z)*texturePixel;

            if(leavesBlock && mainOptions.fastRendering == true)
            {
                left += percent;
                right += percent;
            }

            MeshChunk->vert(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel, left, up, light1.x,light1.y,light1.z);
            MeshChunk->vert(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel, right, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel, right, down, light3.x,light3.y,light3.z);
            MeshChunk->vert(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel, left, down, light4.x,light4.y,light4.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 5)
    {
        // x y z
        // 0 1 0
        // 1 1 0
        // 1 0 0
        // 0 0 0

        Vector3 BlockColorz1;

        onCorners == true ? Block1 = _GetBlock(x,y,z-1) : Block1 = _GetBlockNoCheck(x,y,z-1);
        BaseBlock *blockType1;
        blockType1 = &blockTypes[Block1];

        transparentBlock = blockType1->transparent;
        specialBlock = blockType1->blockModel;
        if(transparent == true)
        {
            if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (specialBlock > 0 && Block1 != 4 && Block1 != 112 && Block1 != 9 && Block1 != 37 && Block1 != 38))
            {
                canCreate = true;
            }
            if (transparentBlock == false && specialBlock == 0)
            {
                canCreate = false;
            }
        }
        else
        {
            if (specialBlock > 0 && Block1 != Block)
            {
                canCreate = true;
            }
            if (transparentBlock == true || Block1 == 0)
            {
                canCreate = true;
            }
        }

        if(Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID())
        {
            if(mainOptions.fastRendering == true)
            {
                canCreate = false;
            }
        }

        if (canCreate)
        {
            float mainLight = BlockFinalLight(x,y,z-1);
            BlockColorz1 = Vector3(mainLight,mainLight,mainLight) * blockZConstLight;

            light1 = Vector3(1,1,1);
            light2 = Vector3(1,1,1);
            light3 = Vector3(1,1,1);
            light4 = Vector3(1,1,1);

            light1 = light2 = light3 = light4 = BlockColorz1;

            if(mainOptions.smoothLighting == true)
            {
                if(onCorners)
                {
                    if(light[19] == -1)
                    {
                        light[19] = BlockFinalLight(x,y+1,z-1) ;
                    }

                    if(light[3] == -1)
                    {
                        light[3] = BlockFinalLight(x-1,y,z-1) ;
                    }

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    }

                    if(light[11] == -1)
                    {
                        light[11] = BlockFinalLight(x+1,y,z-1) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    }

                    if(light[16] == -1)
                    {
                        light[16] = BlockFinalLight(x,y-1,z-1) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                    }

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                    }
                }
                else
                {
                    if(light[19] == -1)
                    {
                        light[19] = BlockFinalLightNoCheck(x,y+1,z-1) ;
                    }

                    if(light[3] == -1)
                    {
                        light[3] = BlockFinalLightNoCheck(x-1,y,z-1) ;
                    }

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    }

                    if(light[11] == -1)
                    {
                        light[11] = BlockFinalLightNoCheck(x+1,y,z-1) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    }

                    if(light[16] == -1)
                    {
                        light[16] = BlockFinalLightNoCheck(x,y-1,z-1) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                    }

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                    }
                }

                float light1noV = (light[19]+light[3]+light[4]+mainLight)/4.0f;
                float light2noV = (light[19]+light[11]+light[12]+mainLight)/4.0f;
                float light3noV = (light[16]+light[3]+light[7]+mainLight)/4.0f;
                float light4noV = (light[16]+light[11]+light[15]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y-1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockZConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockZConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockZConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockZConstLight;
            }

            left = percent*blockType->botPlane_x + point4.x*texturePixel;
            right = percent*blockType->botPlane_x+percent - (16-point2.x)*texturePixel;

            up = percent*blockType->botPlane_y + (16-point2.y)*texturePixel;
            down = percent*blockType->botPlane_y + (16-point4.y)*texturePixel;

            if(leavesBlock && mainOptions.fastRendering == true)
            {
                left += percent;
                right += percent;
            }

            MeshChunk->vert(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel, right, up, light2.x,light2.y,light2.z);
            MeshChunk->vert(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel, left, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel, left, down, light3.x,light3.y,light3.z);
            MeshChunk->vert(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel, right, down, light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 6)
    {
        // x y z
        // 0 0 1
        // 1 0 1
        // 1 1 1
        // 0 1 1

        Vector3 BlockColorz2;

        onCorners == true ? Block1 = _GetBlock(x,y,z+1) : Block1 = _GetBlockNoCheck(x,y,z+1);
        BaseBlock *blockType1;
        blockType1 = &blockTypes[Block1];

        transparentBlock = blockType1->transparent;
        specialBlock = blockType1->blockModel;
        if(transparent == true)
        {
            if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (specialBlock > 0 && Block1 != 4 && Block1 != 112 && Block1 != 9 && Block1 != 37 && Block1 != 38))
            {
                canCreate = true;
            }
            if (transparentBlock == false && specialBlock == 0)
            {
                canCreate = false;
            }
        }
        else
        {
            if (specialBlock > 0 && Block1 != Block)
            {
                canCreate = true;
            }
            if (transparentBlock == true || Block1 == 0)
            {
                canCreate = true;
            }
        }

        if(Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID())
        {
            if(mainOptions.fastRendering == true)
            {
                canCreate = false;
            }
        }

        if (canCreate == true)
        {
            float mainLight = BlockFinalLight(x,y,z+1);
            BlockColorz2 = Vector3(mainLight,mainLight,mainLight) * blockZConstLight;

            light1 = Vector3(1,1,1);
            light2 = Vector3(1,1,1);
            light3 = Vector3(1,1,1);
            light4 = Vector3(1,1,1);

            light1 = light2 = light3 = light4 = BlockColorz2;

            if(mainOptions.smoothLighting == true)
            {
                if(onCorners)
                {
                    if(light[18] == -1)
                    {
                        light[18] = BlockFinalLight(x,y+1,z+1) ;
                    }

                    if(light[1] == -1)
                    {
                        light[1] = BlockFinalLight(x-1,y,z+1) ;
                    }

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    }

                    if(light[9] == -1)
                    {
                        light[9] = BlockFinalLight(x+1,y,z+1) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    }

                    if(light[17] == -1)
                    {
                        light[17] = BlockFinalLight(x,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    }
                }
                else
                {
                    if(light[18] == -1)
                    {
                        light[18] = BlockFinalLightNoCheck(x,y+1,z+1) ;
                    }

                    if(light[1] == -1)
                    {
                        light[1] = BlockFinalLightNoCheck(x-1,y,z+1) ;
                    }

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    }

                    if(light[9] == -1)
                    {
                        light[9] = BlockFinalLightNoCheck(x+1,y,z+1) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    }

                    if(light[17] == -1)
                    {
                        light[17] = BlockFinalLightNoCheck(x,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    }
                }

                float light1noV = (light[18]+light[1]+light[2]+mainLight)/4.0f;
                float light2noV = (light[18]+light[9]+light[10]+mainLight)/4.0f;
                float light3noV = (light[17]+light[1]+light[6]+mainLight)/4.0f;
                float light4noV = (light[17]+light[9]+light[14]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y-1,z+1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockZConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockZConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockZConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockZConstLight;
            }

            left = percent*blockType->forPlane_x + point1.x*texturePixel;
            right = percent*blockType->forPlane_x+percent - (16-point3.x)*texturePixel;

            up = percent*blockType->forPlane_y + (16-point3.y)*texturePixel;
            down = percent*blockType->forPlane_y + (16-point1.y)*texturePixel;

            if(leavesBlock && mainOptions.fastRendering == true)
            {
                left += percent;
                right += percent;
            }

            MeshChunk->vert(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel, left, down, light1.x,light1.y,light1.z);
            MeshChunk->vert(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel, right, down, light3.x,light3.y,light3.z);
            MeshChunk->vert(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel, right, up, light4.x,light4.y,light4.z);
            MeshChunk->vert(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel, left, up, light2.x,light2.y,light2.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }
}


void CraftWorld::BuildWorldBlockPlane(BaseBlock *blockType, int x,int y, int z, int &iVertex, SimpleMeshChunk* MeshChunk, block_t Block, bool transparent, char side, Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4, bool alwaysBuild, bool constantColor)
{
    float pixel, texturePixel;

    pixel = (float)1/16.0f;
    texturePixel = (float)1/256.0f;

    bool canCreate = false;
    bool DefaultBlock = false;
    bool transparentBlock = false;

    float up = 0.0f;
    float down = 0.0f;
    float left = 0.0f;
    float right = 0.0f;

    char Block1;

    float light[20];
    for(int o = 0; o <= 19; o++)
    {
        light[o] = -1;
    }
    bool onCorners = false;
    if(x == 0 || y == 0 || z == 0 || x == WORLD_SIZE-1 || y == WORLD_HEIGHT-1 || z == WORLD_SIZE-1)
    {
        onCorners = true;
    }

    Vector3 light1, light2, light3, light4;

    if(side == 1)
    {
        // x y z
        // 0 0 1
        // 0 1 1
        // 0 1 0
        // 0 0 0
        Vector3 BlockColorx1;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x-1,y,z);

            transparentBlock = DefaultBlock;
            canCreate = false;
            if(transparent)
            {
                Block1 = 1;
                if (x > 0)
                {
                    transparentBlock = BlockTransparent(x-1,y,z);
                    Block1 = GetBlock(x-1,y,z);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x-1,y,z) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (x > 0)
                {
                    Block1 = GetBlock(x-1,y,z);
                    transparentBlock = BlockTransparentOrSpecial(x-1,y,z);
                    if (transparentBlock == true)
                        canCreate = true;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }

        if (canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x-1,y,z);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColorx1 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColorx1 = Vector3(mainLight,mainLight,mainLight) * blockXConstLight;
            }

            if(westLight != 0)
            {
                BlockColorx1.x += 0.3*westLight;
                BlockColorx1.y += 0.2*westLight;
            }

            light1 = light2 = light3 = light4 = BlockColorx1;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    light[0] = BlockFinalLight(x-1,y+1,z) ;
                    light[1] = BlockFinalLight(x-1,y,z+1) ;
                    light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    light[3] = BlockFinalLight(x-1,y,z-1) ;
                    light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    light[5] = BlockFinalLight(x-1,y-1,z) ;
                    light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                }
                else
                {
                    light[0] = BlockFinalLightNoCheck(x-1,y+1,z) ;
                    light[1] = BlockFinalLightNoCheck(x-1,y,z+1) ;
                    light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    light[3] = BlockFinalLightNoCheck(x-1,y,z-1) ;
                    light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    light[5] = BlockFinalLightNoCheck(x-1,y-1,z) ;
                    light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                }

                float light1noV = (light[0]+light[1]+light[2]+mainLight)/4.0f;
                float light2noV = (light[0]+light[3]+light[4]+mainLight)/4.0f;
                float light3noV = (light[5]+light[1]+light[6]+mainLight)/4.0f;
                float light4noV = (light[5]+light[3]+light[7]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x-1,y+1,z) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x-1,y+1,z) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockXConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockXConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockXConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockXConstLight;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*blockType->leftPlane_x + point4.z*texturePixel;
            right = percent*blockType->leftPlane_x+percent - (16-point2.z)*texturePixel;

            up = percent*blockType->leftPlane_y + (16-point2.y)*texturePixel;
            down = percent*blockType->leftPlane_y + (16-point4.y)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light3.x,light3.y,light3.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 2)
    {
        // x y z
        // 1 0 0
        // 1 1 0
        // 1 1 1
        // 1 0 1

        Vector3 BlockColorx2;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x+1,y,z);

            transparentBlock = DefaultBlock;
            canCreate = false;
            if(transparent)
            {
                Block1 = 1;
                if (x < WORLD_SIZE - 1)
                {
                    transparentBlock = BlockTransparent(x+1,y,z);
                    Block1 = GetBlock(x+1,y,z);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x+1,y,z) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (x < WORLD_SIZE - 1)
                {
                    transparentBlock = BlockTransparentOrSpecial(x+1,y,z);
                    Block1 = GetBlock(x+1,y,z);
                    if (transparentBlock == true)
                        canCreate = true;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if (canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x+1,y,z);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColorx2 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColorx2 = Vector3(mainLight,mainLight,mainLight) * blockXConstLight;
            }

            if(eastLight != 0)
            {
                BlockColorx2.x += 0.3*eastLight;
                BlockColorx2.y += 0.2*eastLight;
            }

            light1 = light2 = light3 = light4 = BlockColorx2;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    light[8] = BlockFinalLight(x+1,y+1,z) ;
                    light[9] = BlockFinalLight(x+1,y,z+1) ;
                    light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    light[11] = BlockFinalLight(x+1,y,z-1) ;
                    light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    light[13] = BlockFinalLight(x+1,y-1,z) ;
                    light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                }
                else
                {
                    light[8] = BlockFinalLightNoCheck(x+1,y+1,z) ;
                    light[9] = BlockFinalLightNoCheck(x+1,y,z+1) ;
                    light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    light[11] = BlockFinalLightNoCheck(x+1,y,z-1) ;
                    light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    light[13] = BlockFinalLightNoCheck(x+1,y-1,z) ;
                    light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                }

                float light1noV = (light[8]+light[9]+light[10]+mainLight)/4.0f;
                float light2noV = (light[8]+light[11]+light[12]+mainLight)/4.0f;
                float light3noV = (light[13]+light[9]+light[14]+mainLight)/4.0f;
                float light4noV = (light[13]+light[11]+light[15]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockXConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockXConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockXConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockXConstLight;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*blockType->rightPlane_x + point1.z*texturePixel;
            right = percent*blockType->rightPlane_x+percent - (16-point3.z)*texturePixel;

            up = percent*blockType->rightPlane_y + (16-point3.y)*texturePixel;
            down = percent*blockType->rightPlane_y + (16-point1.y)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 3)
    {
        // x z y
        // 0 0 0
        // 1 0 0
        // 1 1 0
        // 0 1 0

        Vector3 BlockColory2;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x,y-1,z);

            transparentBlock = DefaultBlock;
            canCreate = false;
            if(transparent)
            {
                Block1 = 1;
                if (y > 0)
                {
                    transparentBlock = BlockTransparent(x,y-1,z);
                    Block1 = GetBlock(x,y-1,z);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x,y-1,z) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (y > 0)
                {
                    Block1 = GetBlock(x,y-1,z);
                    transparentBlock = BlockTransparentOrSpecial(x,y-1,z);
                    if (transparentBlock == true)
                        canCreate = true;
                }
                else
                {
                    canCreate = false;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if (canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x,y-1,z);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColory2 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColory2 = Vector3(mainLight,mainLight,mainLight) * 0.6f;
            }
            light1 = light2 = light3 = light4 = BlockColory2;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    if(light[5] == -1)
                    {
                        light[5] = BlockFinalLight(x-1,y-1,z) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                    }

                    light[16] = BlockFinalLight(x,y-1,z-1) ;

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                    }

                    if(light[13] == -1)
                    {
                        light[13] = BlockFinalLight(x+1,y-1,z) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    }

                    light[17] = BlockFinalLight(x,y-1,z+1) ;
                }
                else
                {
                    if(light[5] == -1)
                    {
                        light[5] = BlockFinalLightNoCheck(x-1,y-1,z) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                    }

                    light[16] = BlockFinalLightNoCheck(x,y-1,z-1) ;

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                    }

                    if(light[13] == -1)
                    {
                        light[13] = BlockFinalLightNoCheck(x+1,y-1,z) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    }

                    light[17] = BlockFinalLightNoCheck(x,y-1,z+1) ;
                }

                float light1noV = (light[5]+light[7]+light[16]+mainLight)/4.0f;
                float light2noV = (light[16]+light[15]+light[13]+mainLight)/4.0f;
                float light3noV = (light[13]+light[14]+light[17]+mainLight)/4.0f;
                float light4noV = (light[17]+light[6]+light[5]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y-1,z-1) || !BlockTransparentOrLightSource(x,y-1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y-1,z+1) || !BlockTransparentOrLightSource(x,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x,y-1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light1noV,light1noV,light1noV)*0.6f;
                light2 = Vector3(light2noV,light2noV,light2noV)*0.6f;
                light3 = Vector3(light3noV,light3noV,light3noV)*0.6f;
                light4 = Vector3(light4noV,light4noV,light4noV)*0.6f;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*blockType->downPlane_x + point1.x*texturePixel;
            right = percent*blockType->downPlane_x+percent - (16-point3.x)*texturePixel;

            up = percent*blockType->downPlane_y + (16-point1.z)*texturePixel;
            down = percent*blockType->downPlane_y + (16-point3.z)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 4)
    {
        // x z y
        // 0 1 1
        // 1 1 1
        // 1 0 1
        // 0 0 1

        Vector3 BlockColory1;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x,y+1,z);

            if(transparent)
            {
                Block1 = 1;
                if (y < WORLD_HEIGHT - 1)
                {
                    transparentBlock = BlockTransparent(x,y+1,z);
                    Block1 = GetBlock(x,y+1,z);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x,y+1,z) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                    {
                        canCreate = true;
                    }
                }
            }
            else
            {
                if (y < WORLD_HEIGHT - 1)
                {
                    Block1 = GetBlock(x,y+1,z);
                    if (BlockTransparentOrSpecial(x,y+1,z) == true && GetBlock(x,y+1,z) != 93)
                    {
                        canCreate = true;
                    }
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if(canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x,y+1,z);
            BlockColory1 = Vector3(mainLight,mainLight,mainLight);
            light1 = light2 = light3 = light4 = BlockColory1;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    light[18] = BlockFinalLight(x,y+1,z+1) ;

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    }

                    if(light[0] == -1)
                    {
                        light[0] = BlockFinalLight(x-1,y+1,z) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    }

                    if(light[8] == -1)
                    {
                        light[8] = BlockFinalLight(x+1,y+1,z) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    }

                    light[19] = BlockFinalLight(x,y+1,z-1) ;

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    }
                }
                else
                {
                    light[18] = BlockFinalLightNoCheck(x,y+1,z+1) ;

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    }

                    if(light[0] == -1)
                    {
                        light[0] = BlockFinalLightNoCheck(x-1,y+1,z) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    }

                    if(light[8] == -1)
                    {
                        light[8] = BlockFinalLightNoCheck(x+1,y+1,z) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    }

                    light[19] = BlockFinalLightNoCheck(x,y+1,z-1) ;

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    }
                }

                float light1noV = (light[18]+light[2]+light[0]+mainLight)/4.0f;
                float light2noV = (light[18]+light[10]+light[8]+mainLight)/4.0f;
                float light3noV = (light[8]+light[12]+light[19]+mainLight)/4.0f;
                float light4noV = (light[19]+light[4]+light[0]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y+1,z-1) || !BlockTransparentOrLightSource(x,y+1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x,y+1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light1noV,light1noV,light1noV);
                light2 = Vector3(light2noV,light2noV,light2noV);
                light3 = Vector3(light3noV,light3noV,light3noV);
                light4 = Vector3(light4noV,light4noV,light4noV);
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }
            //BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,"top",Vector3(6,16,10),Vector3(10,16,10),Vector3(10,16,6),Vector3(6,16,6));
            left = percent*blockType->upPlane_x + point4.x*texturePixel;
            right = percent*blockType->upPlane_x + percent - ((16-point2.x)*texturePixel);

            up = percent*blockType->upPlane_y + (16-point2.z)*texturePixel;
            down = percent*blockType->upPlane_y + (16-point4.z)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);


            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 5)
    {
        // x y z
        // 0 1 0
        // 1 1 0
        // 1 0 0
        // 0 0 0

        Vector3 BlockColorz1;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x,y,z-1);

            if(transparent)
            {
                Block1 = 1;
                if (z > 0)
                {
                    transparentBlock = BlockTransparent(x,y,z-1);
                    Block1 = GetBlock(x,y,z-1);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x,y,z-1) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (z > 0)
                {
                    Block1 = GetBlock(x,y,z-1);
                    transparentBlock = BlockTransparentOrSpecial(x,y,z-1);
                    if (transparentBlock == true)
                        canCreate = true;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if (canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x,y,z-1);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColorz1 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColorz1 = Vector3(mainLight,mainLight,mainLight) * blockZConstLight;
            }
            light1 = light2 = light3 = light4 = BlockColorz1;

            if(mainOptions.smoothLighting == true && constantColor)
            {
                if(onCorners)
                {
                    if(light[19] == -1)
                    {
                        light[19] = BlockFinalLight(x,y+1,z-1) ;
                    }

                    if(light[3] == -1)
                    {
                        light[3] = BlockFinalLight(x-1,y,z-1) ;
                    }

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    }

                    if(light[11] == -1)
                    {
                        light[11] = BlockFinalLight(x+1,y,z-1) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    }

                    if(light[16] == -1)
                    {
                        light[16] = BlockFinalLight(x,y-1,z-1) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                    }

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                    }
                }
                else
                {
                    if(light[19] == -1)
                    {
                        light[19] = BlockFinalLightNoCheck(x,y+1,z-1) ;
                    }

                    if(light[3] == -1)
                    {
                        light[3] = BlockFinalLightNoCheck(x-1,y,z-1) ;
                    }

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    }

                    if(light[11] == -1)
                    {
                        light[11] = BlockFinalLightNoCheck(x+1,y,z-1) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    }

                    if(light[16] == -1)
                    {
                        light[16] = BlockFinalLightNoCheck(x,y-1,z-1) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                    }

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                    }
                }

                float light1noV = (light[19]+light[3]+light[4]+mainLight)/4.0f;
                float light2noV = (light[19]+light[11]+light[12]+mainLight)/4.0f;
                float light3noV = (light[16]+light[3]+light[7]+mainLight)/4.0f;
                float light4noV = (light[16]+light[11]+light[15]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y-1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockZConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockZConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockZConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockZConstLight;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*blockType->botPlane_x + point4.x*texturePixel;
            right = percent*blockType->botPlane_x+percent - (16-point2.x)*texturePixel;

            up = percent*blockType->botPlane_y + (16-point2.y)*texturePixel;
            down = percent*blockType->botPlane_y + (16-point4.y)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 6)
    {
        // x y z
        // 0 0 1
        // 1 0 1
        // 1 1 1
        // 0 1 1

        Vector3 BlockColorz2;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x,y,z+1);

            if(transparent)
            {
                Block1 = 1;
                if (z < WORLD_SIZE - 1)
                {
                    transparentBlock = BlockTransparent(x,y,z+1);
                    Block1 = GetBlock(x,y,z+1);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x,y,z+1) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (z < WORLD_SIZE - 1)
                {
                    Block1 = GetBlock(x,y,z+1);
                    transparentBlock = BlockTransparentOrSpecial(x,y,z+1);
                    if (transparentBlock == true)
                        canCreate = true;
                }
                else
                {
                    canCreate = false;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if (canCreate == true || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x,y,z+1);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColorz2 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColorz2 = Vector3(mainLight,mainLight,mainLight) * blockZConstLight;
            }

            light1 = light2 = light3 = light4 = BlockColorz2;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    if(light[18] == -1)
                    {
                        light[18] = BlockFinalLight(x,y+1,z+1) ;
                    }

                    if(light[1] == -1)
                    {
                        light[1] = BlockFinalLight(x-1,y,z+1) ;
                    }

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    }

                    if(light[9] == -1)
                    {
                        light[9] = BlockFinalLight(x+1,y,z+1) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    }

                    if(light[17] == -1)
                    {
                        light[17] = BlockFinalLight(x,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    }
                }
                else
                {
                    if(light[18] == -1)
                    {
                        light[18] = BlockFinalLightNoCheck(x,y+1,z+1) ;
                    }

                    if(light[1] == -1)
                    {
                        light[1] = BlockFinalLightNoCheck(x-1,y,z+1) ;
                    }

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    }

                    if(light[9] == -1)
                    {
                        light[9] = BlockFinalLightNoCheck(x+1,y,z+1) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    }

                    if(light[17] == -1)
                    {
                        light[17] = BlockFinalLightNoCheck(x,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    }
                }

                float light1noV = (light[18]+light[1]+light[2]+mainLight)/4.0f;
                float light2noV = (light[18]+light[9]+light[10]+mainLight)/4.0f;
                float light3noV = (light[17]+light[1]+light[6]+mainLight)/4.0f;
                float light4noV = (light[17]+light[9]+light[14]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y-1,z+1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockZConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockZConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockZConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockZConstLight;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*blockType->forPlane_x + point1.x*texturePixel;
            right = percent*blockType->forPlane_x+percent - (16-point3.x)*texturePixel;

            up = percent*blockType->forPlane_y + (16-point3.y)*texturePixel;
            down = percent*blockType->forPlane_y + (16-point1.y)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light2.x,light2.y,light2.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }
}

void CraftWorld::BuildWorldBlockPlane(BaseBlock *blockType, int x,int y, int z, int &iVertex, SimpleMeshChunk* MeshChunk, block_t Block, bool transparent, char side, Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4, bool alwaysBuild, bool constantColor, int texturePlane)
{
    float pixel, texturePixel;

    pixel = (float)1/16.0f;
    texturePixel = (float)1/256.0f;

    bool canCreate = false;
    bool DefaultBlock = false;
    bool transparentBlock = false;

    float up = 0.0f;
    float down = 0.0f;
    float left = 0.0f;
    float right = 0.0f;

    char Block1;

    int plane_x, plane_y;
    switch(texturePlane)
    {
        case 0 : plane_x = blockType->leftPlane_x; plane_y = blockType->leftPlane_y;
        break;
        case 1 : plane_x = blockType->rightPlane_x; plane_y = blockType->rightPlane_y;
        break;
        case 2 : plane_x = blockType->downPlane_x; plane_y = blockType->downPlane_y;
        break;
        case 3 : plane_x = blockType->upPlane_x; plane_y = blockType->upPlane_y;
        break;
        case 4 : plane_x = blockType->botPlane_x; plane_y = blockType->botPlane_y;
        break;
        case 5 : plane_x = blockType->forPlane_x; plane_y = blockType->forPlane_y;
        break;
    }

    float light[20];
    for(int o = 0; o <= 19; o++)
    {
        light[o] = -1;
    }
    bool onCorners = false;
    if(x == 0 || y == 0 || z == 0 || x == WORLD_SIZE-1 || y == WORLD_HEIGHT-1 || z == WORLD_SIZE-1)
    {
        onCorners = true;
    }

    Vector3 light1, light2, light3, light4;

    if(side == 1)
    {
        // x y z
        // 0 0 1
        // 0 1 1
        // 0 1 0
        // 0 0 0
        Vector3 BlockColorx1;

        if(!alwaysBuild)
        {
            Block1 = GetBlock(x-1,y,z);

            transparentBlock = DefaultBlock;
            canCreate = false;
            if(transparent)
            {
                Block1 = 1;
                if (x > 0)
                {
                    transparentBlock = BlockTransparent(x-1,y,z);
                    Block1 = GetBlock(x-1,y,z);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x-1,y,z) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (x > 0)
                {
                    Block1 = GetBlock(x-1,y,z);
                    transparentBlock = BlockTransparentOrSpecial(x-1,y,z);
                    if (transparentBlock == true)
                        canCreate = true;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if (canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x-1,y,z);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColorx1 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColorx1 = Vector3(mainLight,mainLight,mainLight) * blockXConstLight;
            }

            if(westLight != 0)
            {
                BlockColorx1.x += 0.3*westLight;
                BlockColorx1.y += 0.2*westLight;
            }

            light1 = light2 = light3 = light4 = BlockColorx1;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    light[0] = BlockFinalLight(x-1,y+1,z) ;
                    light[1] = BlockFinalLight(x-1,y,z+1) ;
                    light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    light[3] = BlockFinalLight(x-1,y,z-1) ;
                    light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    light[5] = BlockFinalLight(x-1,y-1,z) ;
                    light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                }
                else
                {
                    light[0] = BlockFinalLightNoCheck(x-1,y+1,z) ;
                    light[1] = BlockFinalLightNoCheck(x-1,y,z+1) ;
                    light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    light[3] = BlockFinalLightNoCheck(x-1,y,z-1) ;
                    light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    light[5] = BlockFinalLightNoCheck(x-1,y-1,z) ;
                    light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                }

                float light1noV = (light[0]+light[1]+light[2]+mainLight)/4.0f;
                float light2noV = (light[0]+light[3]+light[4]+mainLight)/4.0f;
                float light3noV = (light[5]+light[1]+light[6]+mainLight)/4.0f;
                float light4noV = (light[5]+light[3]+light[7]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x-1,y+1,z) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x-1,y+1,z) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockXConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockXConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockXConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockXConstLight;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*plane_x + point4.z*texturePixel;
            right = percent*plane_x+percent - (16-point2.z)*texturePixel;

            up = percent*plane_y + (16-point2.y)*texturePixel;
            down = percent*plane_y + (16-point4.y)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light3.x,light3.y,light3.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 2)
    {
        // x y z
        // 1 0 0
        // 1 1 0
        // 1 1 1
        // 1 0 1

        Vector3 BlockColorx2;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x+1,y,z);

            transparentBlock = DefaultBlock;
            canCreate = false;
            if(transparent)
            {
                Block1 = 1;
                if (x < WORLD_SIZE - 1)
                {
                    transparentBlock = BlockTransparent(x+1,y,z);
                    Block1 = GetBlock(x+1,y,z);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x+1,y,z) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (x < WORLD_SIZE - 1)
                {
                    transparentBlock = BlockTransparentOrSpecial(x+1,y,z);
                    Block1 = GetBlock(x+1,y,z);
                    if (transparentBlock == true)
                        canCreate = true;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if (canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x+1,y,z);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColorx2 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColorx2 = Vector3(mainLight,mainLight,mainLight) * blockXConstLight;
            }

            if(eastLight != 0)
            {
                BlockColorx2.x += 0.3*eastLight;
                BlockColorx2.y += 0.2*eastLight;
            }

            light1 = light2 = light3 = light4 = BlockColorx2;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    light[8] = BlockFinalLight(x+1,y+1,z) ;
                    light[9] = BlockFinalLight(x+1,y,z+1) ;
                    light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    light[11] = BlockFinalLight(x+1,y,z-1) ;
                    light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    light[13] = BlockFinalLight(x+1,y-1,z) ;
                    light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                }
                else
                {
                    light[8] = BlockFinalLightNoCheck(x+1,y+1,z) ;
                    light[9] = BlockFinalLightNoCheck(x+1,y,z+1) ;
                    light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    light[11] = BlockFinalLightNoCheck(x+1,y,z-1) ;
                    light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    light[13] = BlockFinalLightNoCheck(x+1,y-1,z) ;
                    light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                }

                float light1noV = (light[8]+light[9]+light[10]+mainLight)/4.0f;
                float light2noV = (light[8]+light[11]+light[12]+mainLight)/4.0f;
                float light3noV = (light[13]+light[9]+light[14]+mainLight)/4.0f;
                float light4noV = (light[13]+light[11]+light[15]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockXConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockXConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockXConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockXConstLight;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*plane_x + point1.z*texturePixel;
            right = percent*plane_x+percent - (16-point3.z)*texturePixel;

            up = percent*plane_y + (16-point3.y)*texturePixel;
            down = percent*plane_y + (16-point1.y)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 3)
    {
        // x z y
        // 0 0 0
        // 1 0 0
        // 1 1 0
        // 0 1 0

        Vector3 BlockColory2;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x,y-1,z);

            transparentBlock = DefaultBlock;
            canCreate = false;
            if(transparent)
            {
                Block1 = 1;
                if (y > 0)
                {
                    transparentBlock = BlockTransparent(x,y-1,z);
                    Block1 = GetBlock(x,y-1,z);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x,y-1,z) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (y > 0)
                {
                    Block1 = GetBlock(x,y-1,z);
                    transparentBlock = BlockTransparentOrSpecial(x,y-1,z);
                    if (transparentBlock == true)
                        canCreate = true;
                }
                else
                {
                    canCreate = false;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if (canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x,y-1,z);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColory2 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColory2 = Vector3(mainLight,mainLight,mainLight) * 0.6f;
            }
            light1 = light2 = light3 = light4 = BlockColory2;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    if(light[5] == -1)
                    {
                        light[5] = BlockFinalLight(x-1,y-1,z) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                    }

                    light[16] = BlockFinalLight(x,y-1,z-1) ;

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                    }

                    if(light[13] == -1)
                    {
                        light[13] = BlockFinalLight(x+1,y-1,z) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    }

                    light[17] = BlockFinalLight(x,y-1,z+1) ;
                }
                else
                {
                    if(light[5] == -1)
                    {
                        light[5] = BlockFinalLightNoCheck(x-1,y-1,z) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                    }

                    light[16] = BlockFinalLightNoCheck(x,y-1,z-1) ;

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                    }

                    if(light[13] == -1)
                    {
                        light[13] = BlockFinalLightNoCheck(x+1,y-1,z) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    }

                    light[17] = BlockFinalLightNoCheck(x,y-1,z+1) ;
                }

                float light1noV = (light[5]+light[7]+light[16]+mainLight)/4.0f;
                float light2noV = (light[16]+light[15]+light[13]+mainLight)/4.0f;
                float light3noV = (light[13]+light[14]+light[17]+mainLight)/4.0f;
                float light4noV = (light[17]+light[6]+light[5]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y-1,z-1) || !BlockTransparentOrLightSource(x,y-1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y-1,z+1) || !BlockTransparentOrLightSource(x,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x,y-1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light1noV,light1noV,light1noV)*0.6f;
                light2 = Vector3(light2noV,light2noV,light2noV)*0.6f;
                light3 = Vector3(light3noV,light3noV,light3noV)*0.6f;
                light4 = Vector3(light4noV,light4noV,light4noV)*0.6f;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*plane_x + point1.x*texturePixel;
            right = percent*plane_x+percent - (16-point3.x)*texturePixel;

            up = percent*plane_y + (16-point1.z)*texturePixel;
            down = percent*plane_y + (16-point3.z)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 4)
    {
        // x z y
        // 0 1 1
        // 1 1 1
        // 1 0 1
        // 0 0 1

        Vector3 BlockColory1;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x,y+1,z);

            if(transparent)
            {
                Block1 = 1;
                if (y < WORLD_HEIGHT - 1)
                {
                    transparentBlock = BlockTransparent(x,y+1,z);
                    Block1 = GetBlock(x,y+1,z);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x,y+1,z) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                    {
                        canCreate = true;
                    }
                }
            }
            else
            {
                if (y < WORLD_HEIGHT - 1)
                {
                    Block1 = GetBlock(x,y+1,z);
                    if (BlockTransparentOrSpecial(x,y+1,z) == true && GetBlock(x,y+1,z) != 93)
                    {
                        canCreate = true;
                    }
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if(canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x,y+1,z);
            BlockColory1 = Vector3(mainLight,mainLight,mainLight);
            light1 = light2 = light3 = light4 = BlockColory1;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    light[18] = BlockFinalLight(x,y+1,z+1) ;

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    }

                    if(light[0] == -1)
                    {
                        light[0] = BlockFinalLight(x-1,y+1,z) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    }

                    if(light[8] == -1)
                    {
                        light[8] = BlockFinalLight(x+1,y+1,z) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    }

                    light[19] = BlockFinalLight(x,y+1,z-1) ;

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    }
                }
                else
                {
                    light[18] = BlockFinalLightNoCheck(x,y+1,z+1) ;

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    }

                    if(light[0] == -1)
                    {
                        light[0] = BlockFinalLightNoCheck(x-1,y+1,z) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    }

                    if(light[8] == -1)
                    {
                        light[8] = BlockFinalLightNoCheck(x+1,y+1,z) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    }

                    light[19] = BlockFinalLightNoCheck(x,y+1,z-1) ;

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    }
                }

                float light1noV = (light[18]+light[2]+light[0]+mainLight)/4.0f;
                float light2noV = (light[18]+light[10]+light[8]+mainLight)/4.0f;
                float light3noV = (light[8]+light[12]+light[19]+mainLight)/4.0f;
                float light4noV = (light[19]+light[4]+light[0]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y+1,z-1) || !BlockTransparentOrLightSource(x,y+1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x,y+1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light1noV,light1noV,light1noV);
                light2 = Vector3(light2noV,light2noV,light2noV);
                light3 = Vector3(light3noV,light3noV,light3noV);
                light4 = Vector3(light4noV,light4noV,light4noV);
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }
            //BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,"top",Vector3(6,16,10),Vector3(10,16,10),Vector3(10,16,6),Vector3(6,16,6));
            left = percent*plane_x + point4.x*texturePixel;
            right = percent*plane_x + percent - ((16-point2.x)*texturePixel);

            up = percent*plane_y + (16-point2.z)*texturePixel;
            down = percent*plane_y + (16-point4.z)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);


            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 5)
    {
        // x y z
        // 0 1 0
        // 1 1 0
        // 1 0 0
        // 0 0 0

        Vector3 BlockColorz1;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x,y,z-1);

            if(transparent)
            {
                Block1 = 1;
                if (z > 0)
                {
                    transparentBlock = BlockTransparent(x,y,z-1);
                    Block1 = GetBlock(x,y,z-1);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x,y,z-1) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (z > 0)
                {
                    Block1 = GetBlock(x,y,z-1);
                    transparentBlock = BlockTransparentOrSpecial(x,y,z-1);
                    if (transparentBlock == true)
                        canCreate = true;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if (canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x,y,z-1);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColorz1 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColorz1 = Vector3(mainLight,mainLight,mainLight) * blockZConstLight;
            }
            light1 = light2 = light3 = light4 = BlockColorz1;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    if(light[19] == -1)
                    {
                        light[19] = BlockFinalLight(x,y+1,z-1) ;
                    }

                    if(light[3] == -1)
                    {
                        light[3] = BlockFinalLight(x-1,y,z-1) ;
                    }

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    }

                    if(light[11] == -1)
                    {
                        light[11] = BlockFinalLight(x+1,y,z-1) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    }

                    if(light[16] == -1)
                    {
                        light[16] = BlockFinalLight(x,y-1,z-1) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                    }

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                    }
                }
                else
                {
                    if(light[19] == -1)
                    {
                        light[19] = BlockFinalLightNoCheck(x,y+1,z-1) ;
                    }

                    if(light[3] == -1)
                    {
                        light[3] = BlockFinalLightNoCheck(x-1,y,z-1) ;
                    }

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    }

                    if(light[11] == -1)
                    {
                        light[11] = BlockFinalLightNoCheck(x+1,y,z-1) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    }

                    if(light[16] == -1)
                    {
                        light[16] = BlockFinalLightNoCheck(x,y-1,z-1) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                    }

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                    }
                }

                float light1noV = (light[19]+light[3]+light[4]+mainLight)/4.0f;
                float light2noV = (light[19]+light[11]+light[12]+mainLight)/4.0f;
                float light3noV = (light[16]+light[3]+light[7]+mainLight)/4.0f;
                float light4noV = (light[16]+light[11]+light[15]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y-1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockZConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockZConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockZConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockZConstLight;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*plane_x + point4.x*texturePixel;
            right = percent*plane_x+percent - (16-point2.x)*texturePixel;

            up = percent*plane_y + (16-point2.y)*texturePixel;
            down = percent*plane_y + (16-point4.y)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 6)
    {
        // x y z
        // 0 0 1
        // 1 0 1
        // 1 1 1
        // 0 1 1

        Vector3 BlockColorz2;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x,y,z+1);

            if(transparent)
            {
                Block1 = 1;
                if (z < WORLD_SIZE - 1)
                {
                    transparentBlock = BlockTransparent(x,y,z+1);
                    Block1 = GetBlock(x,y,z+1);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x,y,z+1) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (z < WORLD_SIZE - 1)
                {
                    Block1 = GetBlock(x,y,z+1);
                    transparentBlock = BlockTransparentOrSpecial(x,y,z+1);
                    if (transparentBlock == true)
                        canCreate = true;
                }
                else
                {
                    canCreate = false;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if (canCreate == true || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x,y,z+1);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColorz2 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColorz2 = Vector3(mainLight,mainLight,mainLight) * blockZConstLight;
            }

            light1 = light2 = light3 = light4 = BlockColorz2;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    if(light[18] == -1)
                    {
                        light[18] = BlockFinalLight(x,y+1,z+1) ;
                    }

                    if(light[1] == -1)
                    {
                        light[1] = BlockFinalLight(x-1,y,z+1) ;
                    }

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    }

                    if(light[9] == -1)
                    {
                        light[9] = BlockFinalLight(x+1,y,z+1) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    }

                    if(light[17] == -1)
                    {
                        light[17] = BlockFinalLight(x,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    }
                }
                else
                {
                    if(light[18] == -1)
                    {
                        light[18] = BlockFinalLightNoCheck(x,y+1,z+1) ;
                    }

                    if(light[1] == -1)
                    {
                        light[1] = BlockFinalLightNoCheck(x-1,y,z+1) ;
                    }

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    }

                    if(light[9] == -1)
                    {
                        light[9] = BlockFinalLightNoCheck(x+1,y,z+1) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    }

                    if(light[17] == -1)
                    {
                        light[17] = BlockFinalLightNoCheck(x,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    }
                }

                float light1noV = (light[18]+light[1]+light[2]+mainLight)/4.0f;
                float light2noV = (light[18]+light[9]+light[10]+mainLight)/4.0f;
                float light3noV = (light[17]+light[1]+light[6]+mainLight)/4.0f;
                float light4noV = (light[17]+light[9]+light[14]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y-1,z+1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockZConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockZConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockZConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockZConstLight;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*plane_x + point1.x*texturePixel;
            right = percent*plane_x+percent - (16-point3.x)*texturePixel;

            up = percent*plane_y + (16-point3.y)*texturePixel;
            down = percent*plane_y + (16-point1.y)*texturePixel;

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(left, down);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(right, down);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(right, up);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(left, up);
            MeshChunk->colour(light2.x,light2.y,light2.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }
}

void CraftWorld::BuildWorldBlockPlane(BaseBlock *blockType, int x,int y, int z, int &iVertex, SimpleMeshChunk* MeshChunk, block_t Block, bool transparent, char side, Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4, bool alwaysBuild, bool constantColor, int texturePlane, int textureRotation)
{
    float pixel, texturePixel;

    pixel = (float)1/16.0f;
    texturePixel = (float)1/256.0f;

    bool canCreate = false;
    bool DefaultBlock = false;
    bool transparentBlock = false;

    float up = 0.0f;
    float down = 0.0f;
    float left = 0.0f;
    float right = 0.0f;

    char Block1;

    int plane_x, plane_y;
    switch(texturePlane)
    {
        case 0 : plane_x = blockType->leftPlane_x; plane_y = blockType->leftPlane_y;
        break;
        case 1 : plane_x = blockType->rightPlane_x; plane_y = blockType->rightPlane_y;
        break;
        case 2 : plane_x = blockType->downPlane_x; plane_y = blockType->downPlane_y;
        break;
        case 3 : plane_x = blockType->upPlane_x; plane_y = blockType->upPlane_y;
        break;
        case 4 : plane_x = blockType->botPlane_x; plane_y = blockType->botPlane_y;
        break;
        case 5 : plane_x = blockType->forPlane_x; plane_y = blockType->forPlane_y;
        break;
    }

    float light[20];
    for(int o = 0; o <= 19; o++)
    {
        light[o] = -1;
    }
    bool onCorners = false;
    if(x == 0 || y == 0 || z == 0 || x == WORLD_SIZE-1 || y == WORLD_HEIGHT-1 || z == WORLD_SIZE-1)
    {
        onCorners = true;
    }

    Vector3 light1, light2, light3, light4;

    if(side == 1)
    {
        // x y z
        // 0 0 1
        // 0 1 1
        // 0 1 0
        // 0 0 0
        Vector3 BlockColorx1;

        if(!alwaysBuild)
        {
            Block1 = GetBlock(x-1,y,z);

            transparentBlock = DefaultBlock;
            canCreate = false;
            if(transparent)
            {
                Block1 = 1;
                if (x > 0)
                {
                    transparentBlock = BlockTransparent(x-1,y,z);
                    Block1 = GetBlock(x-1,y,z);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x-1,y,z) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (x > 0)
                {
                    Block1 = GetBlock(x-1,y,z);
                    transparentBlock = BlockTransparentOrSpecial(x-1,y,z);
                    if (transparentBlock == true)
                        canCreate = true;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if (canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x-1,y,z);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColorx1 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColorx1 = Vector3(mainLight,mainLight,mainLight) * blockXConstLight;
            }

            if(westLight != 0)
            {
                BlockColorx1.x += 0.3*westLight;
                BlockColorx1.y += 0.2*westLight;
            }

            light1 = light2 = light3 = light4 = BlockColorx1;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    light[0] = BlockFinalLight(x-1,y+1,z) ;
                    light[1] = BlockFinalLight(x-1,y,z+1) ;
                    light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    light[3] = BlockFinalLight(x-1,y,z-1) ;
                    light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    light[5] = BlockFinalLight(x-1,y-1,z) ;
                    light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                }
                else
                {
                    light[0] = BlockFinalLightNoCheck(x-1,y+1,z) ;
                    light[1] = BlockFinalLightNoCheck(x-1,y,z+1) ;
                    light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    light[3] = BlockFinalLightNoCheck(x-1,y,z-1) ;
                    light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    light[5] = BlockFinalLightNoCheck(x-1,y-1,z) ;
                    light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                }

                float light1noV = (light[0]+light[1]+light[2]+mainLight)/4.0f;
                float light2noV = (light[0]+light[3]+light[4]+mainLight)/4.0f;
                float light3noV = (light[5]+light[1]+light[6]+mainLight)/4.0f;
                float light4noV = (light[5]+light[3]+light[7]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x-1,y+1,z) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x-1,y+1,z) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockXConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockXConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockXConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockXConstLight;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*plane_x + point4.z*texturePixel;
            right = percent*plane_x+percent - (16-point2.z)*texturePixel;

            up = percent*plane_y + (16-point2.y)*texturePixel;
            down = percent*plane_y + (16-point4.y)*texturePixel;

            float x1,x2,x3,x4,y1,y2,y3,y4;
            float x1t,x2t,x3t,x4t,y1t,y2t,y3t,y4t;
            x1 = x1t = right;
            y1 = y1t = down;

            x2 = x2t = right;
            y2 = y2t = up;

            x3 = x3t = left;
            y3 = y3t = up;

            x4 = x4t = left;
            y4 = y4t = down;

            switch(textureRotation)
            {
                case 1:
                x1 = x2t;
                y1 = y2t;

                x2 = x1t;
                y2 = y1t;

                x3 = x4t;
                y3 = y4t;

                x4 = x3t;
                y4 = y3t;
                break;

                case 2:
                x1 = x3t;
                y1 = y3t;

                x2 = x4t;
                y2 = y4t;

                x3 = x1t;
                y3 = y1t;

                x4 = x2t;
                y4 = y2t;
                break;

                case 3:
                x1 = x4t;
                y1 = y4t;

                x2 = x1t;
                y2 = y1t;

                x3 = x2t;
                y3 = y2t;

                x4 = x3t;
                y4 = y3t;
                break;
            }

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(x1, y1);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(x2, y2);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(x3, y3);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(x4, y4);
            MeshChunk->colour(light3.x,light3.y,light3.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 2)
    {
        // x y z
        // 1 0 0
        // 1 1 0
        // 1 1 1
        // 1 0 1

        Vector3 BlockColorx2;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x+1,y,z);

            transparentBlock = DefaultBlock;
            canCreate = false;
            if(transparent)
            {
                Block1 = 1;
                if (x < WORLD_SIZE - 1)
                {
                    transparentBlock = BlockTransparent(x+1,y,z);
                    Block1 = GetBlock(x+1,y,z);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x+1,y,z) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (x < WORLD_SIZE - 1)
                {
                    transparentBlock = BlockTransparentOrSpecial(x+1,y,z);
                    Block1 = GetBlock(x+1,y,z);
                    if (transparentBlock == true)
                        canCreate = true;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if (canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x+1,y,z);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColorx2 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColorx2 = Vector3(mainLight,mainLight,mainLight) * blockXConstLight;
            }

            if(eastLight != 0)
            {
                BlockColorx2.x += 0.3*eastLight;
                BlockColorx2.y += 0.2*eastLight;
            }

            light1 = light2 = light3 = light4 = BlockColorx2;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    light[8] = BlockFinalLight(x+1,y+1,z) ;
                    light[9] = BlockFinalLight(x+1,y,z+1) ;
                    light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    light[11] = BlockFinalLight(x+1,y,z-1) ;
                    light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    light[13] = BlockFinalLight(x+1,y-1,z) ;
                    light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                }
                else
                {
                    light[8] = BlockFinalLightNoCheck(x+1,y+1,z) ;
                    light[9] = BlockFinalLightNoCheck(x+1,y,z+1) ;
                    light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    light[11] = BlockFinalLightNoCheck(x+1,y,z-1) ;
                    light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    light[13] = BlockFinalLightNoCheck(x+1,y-1,z) ;
                    light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                }

                float light1noV = (light[8]+light[9]+light[10]+mainLight)/4.0f;
                float light2noV = (light[8]+light[11]+light[12]+mainLight)/4.0f;
                float light3noV = (light[13]+light[9]+light[14]+mainLight)/4.0f;
                float light4noV = (light[13]+light[11]+light[15]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockXConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockXConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockXConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockXConstLight;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*plane_x + point1.z*texturePixel;
            right = percent*plane_x+percent - (16-point3.z)*texturePixel;

            up = percent*plane_y + (16-point3.y)*texturePixel;
            down = percent*plane_y + (16-point1.y)*texturePixel;

            float x1,x2,x3,x4,y1,y2,y3,y4;
            float x1t,x2t,x3t,x4t,y1t,y2t,y3t,y4t;
            x1 = x1t = right;
            y1 = y1t = down ;

            x2 = x2t = right;
            y2 = y2t = up;

            x3 = x3t = left;
            y3 = y3t = up;

            x4 = x4t = left;
            y4 = y4t = down;

            switch(textureRotation)
            {
                case 1:
                x1 = x2t;
                y1 = y2t;

                x2 = x3t;
                y2 = y3t;

                x3 = x3t;
                y3 = y3t;

                x4 = x1t;
                y4 = y1t;
                break;

                case 2:
                x1 = x3t;
                y1 = y3t;

                x2 = x4t;
                y2 = y4t;

                x3 = x1t;
                y3 = y1t;

                x4 = x2t;
                y4 = y2t;
                break;

                case 3:
                x1 = x4t;
                y1 = y4t;

                x2 = x1t;
                y2 = y1t;

                x3 = x2t;
                y3 = y2t;

                x4 = x3t;
                y4 = y3t;
                break;
            }

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(x1, y1);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(x2, y2);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(x3, y3);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(x4, y4);
            MeshChunk->colour(light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 3)
    {
        // x z y
        // 0 0 0
        // 1 0 0
        // 1 1 0
        // 0 1 0

        Vector3 BlockColory2;


        if(!alwaysBuild)
        {
            Block1 = GetBlock(x,y-1,z);
            transparentBlock = DefaultBlock;
            canCreate = false;
            if(transparent)
            {
                Block1 = 1;
                if (y > 0)
                {
                    transparentBlock = BlockTransparent(x,y-1,z);
                    Block1 = GetBlock(x,y-1,z);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x,y-1,z) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (y > 0)
                {
                    Block1 = GetBlock(x,y-1,z);
                    transparentBlock = BlockTransparentOrSpecial(x,y-1,z);
                    if (transparentBlock == true)
                        canCreate = true;
                }
                else
                {
                    canCreate = false;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if (canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x,y-1,z);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColory2 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColory2 = Vector3(mainLight,mainLight,mainLight) * 0.6f;
            }
            light1 = light2 = light3 = light4 = BlockColory2;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    if(light[5] == -1)
                    {
                        light[5] = BlockFinalLight(x-1,y-1,z) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                    }

                    light[16] = BlockFinalLight(x,y-1,z-1) ;

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                    }

                    if(light[13] == -1)
                    {
                        light[13] = BlockFinalLight(x+1,y-1,z) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    }

                    light[17] = BlockFinalLight(x,y-1,z+1) ;
                }
                else
                {
                    if(light[5] == -1)
                    {
                        light[5] = BlockFinalLightNoCheck(x-1,y-1,z) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                    }

                    light[16] = BlockFinalLightNoCheck(x,y-1,z-1) ;

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                    }

                    if(light[13] == -1)
                    {
                        light[13] = BlockFinalLightNoCheck(x+1,y-1,z) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    }

                    light[17] = BlockFinalLightNoCheck(x,y-1,z+1) ;
                }

                float light1noV = (light[5]+light[7]+light[16]+mainLight)/4.0f;
                float light2noV = (light[16]+light[15]+light[13]+mainLight)/4.0f;
                float light3noV = (light[13]+light[14]+light[17]+mainLight)/4.0f;
                float light4noV = (light[17]+light[6]+light[5]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x-1,y-1,z) || !BlockTransparentOrLightSource(x-1,y-1,z-1) || !BlockTransparentOrLightSource(x,y-1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y-1,z) || !BlockTransparentOrLightSource(x+1,y-1,z+1) || !BlockTransparentOrLightSource(x,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x-1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x,y-1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y-1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light1noV,light1noV,light1noV)*0.6f;
                light2 = Vector3(light2noV,light2noV,light2noV)*0.6f;
                light3 = Vector3(light3noV,light3noV,light3noV)*0.6f;
                light4 = Vector3(light4noV,light4noV,light4noV)*0.6f;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*plane_x + point1.x*texturePixel;
            right = percent*plane_x+percent - (16-point3.x)*texturePixel;

            up = percent*plane_y + (16-point1.z)*texturePixel;
            down = percent*plane_y + (16-point3.z)*texturePixel;

            float x1,x2,x3,x4,y1,y2,y3,y4;
            float x1t,x2t,x3t,x4t,y1t,y2t,y3t,y4t;
            x1 = x1t = left;
            y1 = y1t = down;

            x2 = x2t = right;
            y2 = y2t = down;

            x3 = x3t = right;
            y3 = y3t = up;

            x4 = x4t = left;
            y4 = y4t = up;

            switch(textureRotation)
            {
                case 1:
                x1 = x2t;
                y1 = y2t;

                x2 = x3t;
                y2 = y3t;

                x3 = x3t;
                y3 = y3t;

                x4 = x1t;
                y4 = y1t;
                break;

                case 2:
                x1 = x3t;
                y1 = y3t;

                x2 = x4t;
                y2 = y4t;

                x3 = x1t;
                y3 = y1t;

                x4 = x2t;
                y4 = y2t;
                break;

                case 3:
                x1 = x4t;
                y1 = y4t;

                x2 = x1t;
                y2 = y1t;

                x3 = x2t;
                y3 = y2t;

                x4 = x3t;
                y4 = y3t;
                break;
            }

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(x1, y1);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(x2, y2);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(x3, y3);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(x4, y4);
            MeshChunk->colour(light4.x,light4.y,light4.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 4)
    {
        // x z y
        // 0 1 1
        // 1 1 1
        // 1 0 1
        // 0 0 1

        Vector3 BlockColory1;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x,y+1,z);

            if(transparent)
            {
                Block1 = 1;
                if (y < WORLD_HEIGHT - 1)
                {
                    transparentBlock = BlockTransparent(x,y+1,z);
                    Block1 = GetBlock(x,y+1,z);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x,y+1,z) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                    {
                        canCreate = true;
                    }
                }
            }
            else
            {
                if (y < WORLD_HEIGHT - 1)
                {
                    Block1 = GetBlock(x,y+1,z);
                    if (BlockTransparentOrSpecial(x,y+1,z) == true && GetBlock(x,y+1,z) != 93)
                    {
                        canCreate = true;
                    }
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if(canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x,y+1,z);
            BlockColory1 = Vector3(mainLight,mainLight,mainLight);
            light1 = light2 = light3 = light4 = BlockColory1;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    light[18] = BlockFinalLight(x,y+1,z+1) ;

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    }

                    if(light[0] == -1)
                    {
                        light[0] = BlockFinalLight(x-1,y+1,z) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    }

                    if(light[8] == -1)
                    {
                        light[8] = BlockFinalLight(x+1,y+1,z) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    }

                    light[19] = BlockFinalLight(x,y+1,z-1) ;

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    }
                }
                else
                {
                    light[18] = BlockFinalLightNoCheck(x,y+1,z+1) ;

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    }

                    if(light[0] == -1)
                    {
                        light[0] = BlockFinalLightNoCheck(x-1,y+1,z) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    }

                    if(light[8] == -1)
                    {
                        light[8] = BlockFinalLightNoCheck(x+1,y+1,z) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    }

                    light[19] = BlockFinalLightNoCheck(x,y+1,z-1) ;

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    }
                }

                float light1noV = (light[18]+light[2]+light[0]+mainLight)/4.0f;
                float light2noV = (light[18]+light[10]+light[8]+mainLight)/4.0f;
                float light3noV = (light[8]+light[12]+light[19]+mainLight)/4.0f;
                float light4noV = (light[19]+light[4]+light[0]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSource(x+1,y+1,z) || !BlockTransparentOrLightSource(x+1,y+1,z-1) || !BlockTransparentOrLightSource(x,y+1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z))
                    {
                        light2noV-=lightFactor2;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x+1,y+1,z) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x,y+1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light1noV,light1noV,light1noV);
                light2 = Vector3(light2noV,light2noV,light2noV);
                light3 = Vector3(light3noV,light3noV,light3noV);
                light4 = Vector3(light4noV,light4noV,light4noV);
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }
            //BuildWorldBlockPlane(blockType,x,y,z,iVertex,MeshChunk,Block,transparent,"top",Vector3(6,16,10),Vector3(10,16,10),Vector3(10,16,6),Vector3(6,16,6));
            left = percent*plane_x + point4.x*texturePixel;
            right = percent*plane_x + percent - ((16-point2.x)*texturePixel);

            up = percent*plane_y + (16-point2.z)*texturePixel;
            down = percent*plane_y + (16-point4.z)*texturePixel;

            float x1,x2,x3,x4,y1,y2,y3,y4;
            float x1t,x2t,x3t,x4t,y1t,y2t,y3t,y4t;
            x1 = x1t = left;
            y1 = y1t = down;

            x2 = x2t = right;
            y2 = y2t = down ;

            x3 = x3t = right;
            y3 = y3t = up;

            x4 = x4t = left;
            y4 = y4t = up;

            switch(textureRotation)
            {
                case 1:
                x1 = x2t;
                y1 = y2t;

                x2 = x1t;
                y2 = y1t;

                x3 = x4t;
                y3 = y4t;

                x4 = x3t;
                y4 = y3t;
                break;

                case 2:
                x1 = x3t;
                y1 = y3t;

                x2 = x4t;
                y2 = y4t;

                x3 = x1t;
                y3 = y1t;

                x4 = x2t;
                y4 = y2t;
                break;

                case 3:
                x1 = x4t;
                y1 = y4t;

                x2 = x1t;
                y2 = y1t;

                x3 = x2t;
                y3 = y2t;

                x4 = x3t;
                y4 = y3t;
                break;
            }

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(x1, y1);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(x2, y2);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(x3, y3);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(x4, y4);
            MeshChunk->colour(light4.x,light4.y,light4.z);


            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 5)
    {
        // x y z
        // 0 1 0
        // 1 1 0
        // 1 0 0
        // 0 0 0

        Vector3 BlockColorz1;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x,y,z-1);

            if(transparent)
            {
                Block1 = 1;
                if (z > 0)
                {
                    transparentBlock = BlockTransparent(x,y,z-1);
                    Block1 = GetBlock(x,y,z-1);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x,y,z-1) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (z > 0)
                {
                    Block1 = GetBlock(x,y,z-1);
                    transparentBlock = BlockTransparentOrSpecial(x,y,z-1);
                    if (transparentBlock == true)
                        canCreate = true;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }

        if (canCreate || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x,y,z-1);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColorz1 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColorz1 = Vector3(mainLight,mainLight,mainLight) * blockZConstLight;
            }
            light1 = light2 = light3 = light4 = BlockColorz1;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    if(light[19] == -1)
                    {
                        light[19] = BlockFinalLight(x,y+1,z-1) ;
                    }

                    if(light[3] == -1)
                    {
                        light[3] = BlockFinalLight(x-1,y,z-1) ;
                    }

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLight(x-1,y+1,z-1) ;
                    }

                    if(light[11] == -1)
                    {
                        light[11] = BlockFinalLight(x+1,y,z-1) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLight(x+1,y+1,z-1) ;
                    }

                    if(light[16] == -1)
                    {
                        light[16] = BlockFinalLight(x,y-1,z-1) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLight(x-1,y-1,z-1) ;
                    }

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLight(x+1,y-1,z-1) ;
                    }
                }
                else
                {
                    if(light[19] == -1)
                    {
                        light[19] = BlockFinalLightNoCheck(x,y+1,z-1) ;
                    }

                    if(light[3] == -1)
                    {
                        light[3] = BlockFinalLightNoCheck(x-1,y,z-1) ;
                    }

                    if(light[4] == -1)
                    {
                        light[4] = BlockFinalLightNoCheck(x-1,y+1,z-1) ;
                    }

                    if(light[11] == -1)
                    {
                        light[11] = BlockFinalLightNoCheck(x+1,y,z-1) ;
                    }

                    if(light[12] == -1)
                    {
                        light[12] = BlockFinalLightNoCheck(x+1,y+1,z-1) ;
                    }

                    if(light[16] == -1)
                    {
                        light[16] = BlockFinalLightNoCheck(x,y-1,z-1) ;
                    }

                    if(light[7] == -1)
                    {
                        light[7] = BlockFinalLightNoCheck(x-1,y-1,z-1) ;
                    }

                    if(light[15] == -1)
                    {
                        light[15] = BlockFinalLightNoCheck(x+1,y-1,z-1) ;
                    }
                }

                float light1noV = (light[19]+light[3]+light[4]+mainLight)/4.0f;
                float light2noV = (light[19]+light[11]+light[12]+mainLight)/4.0f;
                float light3noV = (light[16]+light[3]+light[7]+mainLight)/4.0f;
                float light4noV = (light[16]+light[11]+light[15]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y+1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSource(x,y+1,z-1) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x-1,y,z-1) || !BlockTransparentOrLightSource(x-1,y-1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z-1) || !BlockTransparentOrLightSource(x+1,y,z-1) || !BlockTransparentOrLightSource(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z-1))
                    {
                        light1noV-=lightFactor1;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z-1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z-1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z-1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z-1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockZConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockZConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockZConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockZConstLight;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*plane_x + point4.x*texturePixel;
            right = percent*plane_x+percent - (16-point2.x)*texturePixel;

            up = percent*plane_y + (16-point2.y)*texturePixel;
            down = percent*plane_y + (16-point4.y)*texturePixel;

            float x1,x2,x3,x4,y1,y2,y3,y4;
            float x1t,x2t,x3t,x4t,y1t,y2t,y3t,y4t;
            x1 = x1t = right;
            y1 = y1t = up;

            x2 = x2t = left;
            y2 = y2t = up;

            x3 = x3t = left;
            y3 = y3t = down;

            x4 = x4t = right;
            y4 = y4t = down;

            switch(textureRotation)
            {
                case 1:
                x1 = x2t;
                y1 = y2t;

                x2 = x3t;
                y2 = y3t;

                x3 = x3t;
                y3 = y3t;

                x4 = x1t;
                y4 = y1t;
                break;

                case 2:
                x1 = x3t;
                y1 = y3t;

                x2 = x4t;
                y2 = y4t;

                x3 = x1t;
                y3 = y1t;

                x4 = x2t;
                y4 = y2t;
                break;

                case 3:
                x1 = x4t;
                y1 = y4t;

                x2 = x1t;
                y2 = y1t;

                x3 = x2t;
                y3 = y2t;

                x4 = x3t;
                y4 = y3t;
                break;
            }

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(x1, y1);
            MeshChunk->colour(light2.x,light2.y,light2.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(x2, y2);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(x3, y3);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(x4, y4);
            MeshChunk->colour(light1.x,light1.y,light1.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }

    if(side == 6)
    {
        // x y z
        // 0 0 1
        // 1 0 1
        // 1 1 1
        // 0 1 1

        Vector3 BlockColorz2;
        if(!alwaysBuild)
        {
            Block1 = GetBlock(x,y,z+1);

            if(transparent)
            {
                Block1 = 1;
                if (z < WORLD_SIZE - 1)
                {
                    transparentBlock = BlockTransparent(x,y,z+1);
                    Block1 = GetBlock(x,y,z+1);
                    if ((transparentBlock == true && Block1 != Block) || Block1 == 0 || (BlockSpecial(x,y,z+1) && Block1 != 9 && Block1 != 4 && Block1 != 112 && Block1 != 38))
                        canCreate = true;
                }
            }
            else
            {
                if (z < WORLD_SIZE - 1)
                {
                    Block1 = GetBlock(x,y,z+1);
                    transparentBlock = BlockTransparentOrSpecial(x,y,z+1);
                    if (transparentBlock == true)
                        canCreate = true;
                }
                else
                {
                    canCreate = false;
                }
            }
            if((Block1 == LeavesBlock::getID() || Block1 == BirchLeaves::getID() || Block1 == SpruceLeaves::getID()) && mainOptions.fastRendering == 1)
            {
                canCreate = false;
            }
        }
        if (canCreate == true || alwaysBuild)
        {
            float mainLight = BlockFinalLight(x,y,z+1);
            if(constantColor == true)
            {
                mainLight = BlockFinalLight(x,y,z);
                BlockColorz2 = Vector3(mainLight,mainLight,mainLight);
            }
            else
            {
                BlockColorz2 = Vector3(mainLight,mainLight,mainLight) * blockZConstLight;
            }

            light1 = light2 = light3 = light4 = BlockColorz2;

            if(mainOptions.smoothLighting == true && !constantColor)
            {
                if(onCorners)
                {
                    if(light[18] == -1)
                    {
                        light[18] = BlockFinalLight(x,y+1,z+1) ;
                    }

                    if(light[1] == -1)
                    {
                        light[1] = BlockFinalLight(x-1,y,z+1) ;
                    }

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLight(x-1,y+1,z+1) ;
                    }

                    if(light[9] == -1)
                    {
                        light[9] = BlockFinalLight(x+1,y,z+1) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLight(x+1,y+1,z+1) ;
                    }

                    if(light[17] == -1)
                    {
                        light[17] = BlockFinalLight(x,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLight(x-1,y-1,z+1) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLight(x+1,y-1,z+1) ;
                    }
                }
                else
                {
                    if(light[18] == -1)
                    {
                        light[18] = BlockFinalLightNoCheck(x,y+1,z+1) ;
                    }

                    if(light[1] == -1)
                    {
                        light[1] = BlockFinalLightNoCheck(x-1,y,z+1) ;
                    }

                    if(light[2] == -1)
                    {
                        light[2] = BlockFinalLightNoCheck(x-1,y+1,z+1) ;
                    }

                    if(light[9] == -1)
                    {
                        light[9] = BlockFinalLightNoCheck(x+1,y,z+1) ;
                    }

                    if(light[10] == -1)
                    {
                        light[10] = BlockFinalLightNoCheck(x+1,y+1,z+1) ;
                    }

                    if(light[17] == -1)
                    {
                        light[17] = BlockFinalLightNoCheck(x,y-1,z+1) ;
                    }

                    if(light[6] == -1)
                    {
                        light[6] = BlockFinalLightNoCheck(x-1,y-1,z+1) ;
                    }

                    if(light[14] == -1)
                    {
                        light[14] = BlockFinalLightNoCheck(x+1,y-1,z+1) ;
                    }
                }

                float light1noV = (light[18]+light[1]+light[2]+mainLight)/4.0f;
                float light2noV = (light[18]+light[9]+light[10]+mainLight)/4.0f;
                float light3noV = (light[17]+light[1]+light[6]+mainLight)/4.0f;
                float light4noV = (light[17]+light[9]+light[14]+mainLight)/4.0f;

                float lightFactor1 = light1noV * lightShadowFactor;
                float lightFactor2 = light2noV * lightShadowFactor;
                float lightFactor3 = light3noV * lightShadowFactor;
                float lightFactor4 = light4noV * lightShadowFactor;

                if(onCorners)
                {
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }
                    if(!BlockTransparentOrLightSource(x,y+1,z+1) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y+1,z+1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x-1,y,z+1) || !BlockTransparentOrLightSource(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSource(x,y-1,z+1) || !BlockTransparentOrLightSource(x+1,y,z+1) || !BlockTransparentOrLightSource(x+1,y-1,z+1))
                    {
                        light4noV-=lightFactor4;
                    }
                }
                else
                {
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y+1,z+1))
                    {
                        light1noV-=lightFactor1;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y+1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y+1,z+1))
                    {
                        light2noV-=lightFactor2;
                    }
                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x-1,y-1,z+1))
                    {
                        light3noV-=lightFactor3;
                    }

                    if(!BlockTransparentOrLightSourceNoCheck(x,y-1,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y,z+1) || !BlockTransparentOrLightSourceNoCheck(x+1,y-1,z+1))
                    {
                        light4noV-=lightFactor4;
                    }
                }

                light1 = Vector3(light3noV,light3noV,light3noV)*blockZConstLight;
                light2 = Vector3(light1noV,light1noV,light1noV)*blockZConstLight;
                light3 = Vector3(light4noV,light4noV,light4noV)*blockZConstLight;
                light4 = Vector3(light2noV,light2noV,light2noV)*blockZConstLight;
            }
            if(constantColor)
            {
                mainLight = BlockFinalLight(x,y,z);
                light1 = light2 = light3 = light4 = Vector3(mainLight,mainLight,mainLight);
            }

            left = percent*plane_x + point1.x*texturePixel;
            right = percent*plane_x+percent - (16-point3.x)*texturePixel;

            up = percent*plane_y + (16-point3.y)*texturePixel;
            down = percent*plane_y + (16-point1.y)*texturePixel;

            float x1,x2,x3,x4,y1,y2,y3,y4;
            float x1t,x2t,x3t,x4t,y1t,y2t,y3t,y4t;
            x1 = x1t = left;
            y1 = y1t = down;

            x2 = x2t = right;
            y2 = y2t = down;

            x3 = x3t = right;
            y3 = y3t = up;

            x4 = x4t = left;
            y4 = y4t = up;

            switch(textureRotation)
            {
                case 1:
                x1 = x2t;
                y1 = y2t;

                x2 = x3t;
                y2 = y3t;

                x3 = x3t;
                y3 = y3t;

                x4 = x1t;
                y4 = y1t;
                break;

                case 2:
                x1 = x3t;
                y1 = y3t;

                x2 = x4t;
                y2 = y4t;

                x3 = x1t;
                y3 = y1t;

                x4 = x2t;
                y4 = y2t;
                break;

                case 3:
                x1 = x4t;
                y1 = y4t;

                x2 = x1t;
                y2 = y1t;

                x3 = x2t;
                y3 = y2t;

                x4 = x3t;
                y4 = y3t;
                break;
            }

            MeshChunk->position(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel);
            MeshChunk->textureCoord(x1, y1);
            MeshChunk->colour(light1.x,light1.y,light1.z);
            MeshChunk->position(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel);
            MeshChunk->textureCoord(x2, y2);
            MeshChunk->colour(light3.x,light3.y,light3.z);
            MeshChunk->position(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel);
            MeshChunk->textureCoord(x3, y3);
            MeshChunk->colour(light4.x,light4.y,light4.z);
            MeshChunk->position(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel);
            MeshChunk->textureCoord(x4, y4);
            MeshChunk->colour(light2.x,light2.y,light2.z);

            MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
            MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

            iVertex += 4;
        }
    }
}

void CraftWorld::BuildGUIBlockPlane(BaseBlock* blockType, char side, Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4)
{
    float pixel, texturePixel;

    pixel = (float)1/16.0f;
    texturePixel = (float)1/256.0f;

    float x = -0.5f;
    float y = -0.5f;
    float z = -0.5f;

    float down = 0.0f;
    float up = 0.0f;

    float left = 0.0f;
    float right = 0.0f;

    if(side == 1)
    {
        /// x  y  z
        //  0  0  1
        //  0  1  1
        //  0  1  0
        //  0  0  0

        left = blockType->leftPlane_x * percent + point4.z*texturePixel;
        right = blockType->leftPlane_x * percent + percent - (16-point2.z)*texturePixel;

        up = percent*blockType->leftPlane_y + (16-point2.y)*texturePixel;
        down = percent*blockType->leftPlane_y + (16-point4.y)*texturePixel;

        mPositionLeftRight.push_back(new Vector3(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel));
        mtexturesLeftRight.push_back(new Vector2(right, down));
        mPositionLeftRight.push_back(new Vector3(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel));
        mtexturesLeftRight.push_back(new Vector2(right, up));
        mPositionLeftRight.push_back(new Vector3(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel));
        mtexturesLeftRight.push_back(new Vector2(left, up));
        mPositionLeftRight.push_back(new Vector3(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel));
        mtexturesLeftRight.push_back(new Vector2(left, down));

        mTriangleLeftRight.push_back(new Vector3(iVertexLeftRight, iVertexLeftRight+1, iVertexLeftRight+2));
        mTriangleLeftRight.push_back(new Vector3(iVertexLeftRight+2, iVertexLeftRight+3, iVertexLeftRight));

        iVertexLeftRight += 4;
        pointsLeftRight += 6;
    }

    if(side == 2)
    {
        /// x  y  z
        //  1  0  0
        //  1  1  0
        //  1  1  1
        //  1  0  1

        left = blockType->rightPlane_x*percent + point1.z*texturePixel;
        right = blockType->rightPlane_x*percent + percent - (16-point3.z)*texturePixel;

        up = percent*blockType->rightPlane_y + (16-point3.y)*texturePixel;
        down = percent*blockType->rightPlane_y + (16-point1.y)*texturePixel;

        mPositionLeftRight.push_back(new Vector3(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel));
        mtexturesLeftRight.push_back(new Vector2(right, down));
        mPositionLeftRight.push_back(new Vector3(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel));
        mtexturesLeftRight.push_back(new Vector2(right, up));
        mPositionLeftRight.push_back(new Vector3(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel));
        mtexturesLeftRight.push_back(new Vector2(left, up));
        mPositionLeftRight.push_back(new Vector3(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel));
        mtexturesLeftRight.push_back(new Vector2(left, down));

        mTriangleLeftRight.push_back(new Vector3(iVertexLeftRight, iVertexLeftRight+1, iVertexLeftRight+2));
        mTriangleLeftRight.push_back(new Vector3(iVertexLeftRight+2, iVertexLeftRight+3, iVertexLeftRight));

        iVertexLeftRight += 4;
        pointsLeftRight += 6;
    }

    if(side == 3)
    {
        /// x  y  z
        //  0  0  0
        //  1  0  0
        //  1  0  1
        //  0  0  1

        left = blockType->downPlane_x*percent + point1.x*texturePixel;
        right = blockType->downPlane_x*percent + percent - (16-point3.x)*texturePixel;

        up = percent*blockType->downPlane_y + (16-point1.z)*texturePixel;
        down = percent*blockType->downPlane_y + (16-point3.z)*texturePixel;

        mPositionBottomTop.push_back(new Vector3(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel));
        mtexturesBottomTop.push_back(new Vector2(left, up));
        mPositionBottomTop.push_back(new Vector3(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel));
        mtexturesBottomTop.push_back(new Vector2(right, up));
        mPositionBottomTop.push_back(new Vector3(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel));
        mtexturesBottomTop.push_back(new Vector2(right, down));
        mPositionBottomTop.push_back(new Vector3(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel));
        mtexturesBottomTop.push_back(new Vector2(left, down));

        mTriangleBottomTop.push_back(new Vector3(iVertexBottomTop, iVertexBottomTop+1, iVertexBottomTop+2));
        mTriangleBottomTop.push_back(new Vector3(iVertexBottomTop+2, iVertexBottomTop+3, iVertexBottomTop));

        iVertexBottomTop += 4;
        pointsBottomTop += 6;
    }

    if(side == 4)
    {
        /// x  y  z
        //  0  1  1
        //  1  1  1
        //  1  1  0
        //  0  1  0

        left = blockType->upPlane_x*percent + point4.x*texturePixel;
        right = blockType->upPlane_x*percent + percent - (16-point2.x)*texturePixel;

        up = percent*blockType->upPlane_y + (16-point2.z)*texturePixel;
        down = percent*blockType->upPlane_y + (16-point4.z)*texturePixel;

        mPositionBottomTop.push_back(new Vector3(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel));
        mtexturesBottomTop.push_back(new Vector2(left, up));
        mPositionBottomTop.push_back(new Vector3(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel));
        mtexturesBottomTop.push_back(new Vector2(right, up));
        mPositionBottomTop.push_back(new Vector3(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel));
        mtexturesBottomTop.push_back(new Vector2(right, down));
        mPositionBottomTop.push_back(new Vector3(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel));
        mtexturesBottomTop.push_back(new Vector2(left, down));

        mTriangleBottomTop.push_back(new Vector3(iVertexBottomTop, iVertexBottomTop+1, iVertexBottomTop+2));
        mTriangleBottomTop.push_back(new Vector3(iVertexBottomTop+2, iVertexBottomTop+3, iVertexBottomTop));

        iVertexBottomTop += 4;
        pointsBottomTop += 6;
    }

    if(side == 5)
    {
        /// x  y  z
        //  0  1  0
        //  1  1  0
        //  1  0  0
        //  0  0  0

        left = blockType->botPlane_x*percent + point4.x*texturePixel;
        right = blockType->botPlane_x*percent + percent - (16-point2.x)*texturePixel;

        up = percent*blockType->botPlane_y + (16-point2.y)*texturePixel;
        down = percent*blockType->botPlane_y + (16-point4.y)*texturePixel;

        if(blockType->name == "Furnace")
        {
            left = 12*percent;
            right = left+percent;

            up = 2*percent;
            down = up+percent;
        }

        mPositionBackFront.push_back(new Vector3(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel));
        mtexturesBackFront.push_back(new Vector2(right, up));
        mPositionBackFront.push_back(new Vector3(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel));
        mtexturesBackFront.push_back(new Vector2(left, up));
        mPositionBackFront.push_back(new Vector3(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel));
        mtexturesBackFront.push_back(new Vector2(left, down));
        mPositionBackFront.push_back(new Vector3(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel));
        mtexturesBackFront.push_back(new Vector2(right, down));

        mTriangleBackFront.push_back(new Vector3(iVertexBackFront, iVertexBackFront+1, iVertexBackFront+2));
        mTriangleBackFront.push_back(new Vector3(iVertexBackFront+2, iVertexBackFront+3, iVertexBackFront));

        iVertexBackFront += 4;
        pointsBackFront += 6;
    }

    if(side == 6)
    {
        /// x  y  z
        //  0  0  1
        //  1  0  1
        //  1  1  1
        //  0  1  1

        left = blockType->forPlane_x*percent + point1.x*texturePixel;
        right = blockType->forPlane_x*percent + percent - (16-point3.x)*texturePixel;

        up = percent*blockType->forPlane_y + (16-point3.y)*texturePixel;
        down = percent*blockType->forPlane_y + (16-point1.y)*texturePixel;

        mPositionBackFront.push_back(new Vector3(x+point1.x*pixel, y+point1.y*pixel, z+point1.z*pixel));
        mtexturesBackFront.push_back(new Vector2(left, down));
        mPositionBackFront.push_back(new Vector3(x+point2.x*pixel, y+point2.y*pixel, z+point2.z*pixel));
        mtexturesBackFront.push_back(new Vector2(right, down));
        mPositionBackFront.push_back(new Vector3(x+point3.x*pixel, y+point3.y*pixel, z+point3.z*pixel));
        mtexturesBackFront.push_back(new Vector2(right, up));
        mPositionBackFront.push_back(new Vector3(x+point4.x*pixel, y+point4.y*pixel, z+point4.z*pixel));
        mtexturesBackFront.push_back(new Vector2(left, up));

        mTriangleBackFront.push_back(new Vector3(iVertexBackFront, iVertexBackFront+1, iVertexBackFront+2));
        mTriangleBackFront.push_back(new Vector3(iVertexBackFront+2, iVertexBackFront+3, iVertexBackFront));

        iVertexBackFront += 4;
        pointsBackFront += 6;
    }
}


