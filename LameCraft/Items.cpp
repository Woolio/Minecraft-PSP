#include "Items.h"

//items' id begin from 250

WoodenPickaxe::WoodenPickaxe():BaseItem()
{
	textureRow = 9;

	plane = 0;
	durabilityPoints = 60;
	stackable = false;

	name = "Wooden Pickaxe";
}

item_t WoodenPickaxe::getID(){ return 250;}

StonePickaxe::StonePickaxe():BaseItem()
{
	textureRow = 9;

	plane = 1;
	durabilityPoints = 132;
	stackable = false;

	name = "Stone Pickaxe";
}

item_t StonePickaxe::getID(){ return 251;}

IronPickaxe::IronPickaxe():BaseItem()
{
	textureRow = 9;

	plane = 2;
	durabilityPoints = 251;
	stackable = false;

	name = "Iron Pickaxe";
}

item_t IronPickaxe::getID(){ return 252;}

DiamondPickaxe::DiamondPickaxe():BaseItem()
{
	textureRow = 9;

	plane = 3;
	durabilityPoints = 1562;
	stackable = false;

	name = "Diamond Pickaxe";
}

item_t DiamondPickaxe::getID(){ return 253;}

GoldenPickaxe::GoldenPickaxe():BaseItem()
{
	textureRow = 9;

	plane = 4;
	durabilityPoints = 33;
	stackable = false;

	name = "Golden Pickaxe";
}

item_t GoldenPickaxe::getID(){ return 254;}

WoodenSword::WoodenSword():BaseItem()
{
	textureRow = 11;

	plane = 0;
	durabilityPoints = 60;
	stackable = false;

	name = "Wooden Sword";
}

item_t WoodenSword::getID(){ return 255;}

StoneSword::StoneSword():BaseItem()
{
	textureRow = 11;

	plane = 1;
	durabilityPoints = 132;
	stackable = false;

	name = "Stone Sword";
}

item_t StoneSword::getID(){ return 256;}

IronSword::IronSword():BaseItem()
{
	textureRow = 11;

	plane = 2;
	durabilityPoints = 251;
	stackable = false;

	name = "Iron Sword";
}

item_t IronSword::getID(){ return 257;}

DiamondSword::DiamondSword():BaseItem()
{
	textureRow = 11;

	plane = 3;
	durabilityPoints = 1562;
	stackable = false;

	name = "Diamond Sword";
}

item_t DiamondSword::getID(){ return 258;}

GoldenSword::GoldenSword():BaseItem()
{
	textureRow = 11;

	plane = 4;
	durabilityPoints = 33;
	stackable = false;

	name = "Golden Sword";
}

item_t GoldenSword::getID(){ return 259;}

WoodenShovel::WoodenShovel():BaseItem()
{
	textureRow = 10;

	plane = 0;
	durabilityPoints = 60;
	stackable = false;

	name = "Wooden Shovel";
}

item_t WoodenShovel::getID(){ return 260;}

StoneShovel::StoneShovel():BaseItem()
{
	textureRow = 10;

	plane = 1;
	durabilityPoints = 132;
	stackable = false;

	name = "Stone Shovel";
}

item_t StoneShovel::getID(){ return 261;}

IronShovel::IronShovel():BaseItem()
{
	textureRow = 10;

	plane = 2;
	durabilityPoints = 251;
	stackable = false;

	name = "Iron Shovel";
}

item_t IronShovel::getID(){ return 262;}

DiamondShovel::DiamondShovel():BaseItem()
{
	textureRow = 10;

	plane = 3;
	durabilityPoints = 1562;
	stackable = false;

	name = "Diamond Shovel";
}

item_t DiamondShovel::getID(){ return 263;}

GoldenShovel::GoldenShovel():BaseItem()
{
	textureRow = 10;

	plane = 4;
	durabilityPoints = 33;
	stackable = false;

	name = "Golden Shovel";
}

item_t GoldenShovel::getID(){ return 264;}

WoodenAxe::WoodenAxe():BaseItem()
{
	textureRow = 8;

	plane = 0;
	durabilityPoints = 60;
	stackable = false;

	name = "Wooden Axe";
}

item_t WoodenAxe::getID(){ return 265;}

StoneAxe::StoneAxe():BaseItem()
{
	textureRow = 8;

	plane = 1;
	durabilityPoints = 132;
	stackable = false;

	name = "Stone Axe";
}

item_t StoneAxe::getID(){ return 266;}

IronAxe::IronAxe():BaseItem()
{
	textureRow = 8;

	plane = 2;
	durabilityPoints = 251;
	stackable = false;

	name = "Iron Axe";
}

item_t IronAxe::getID(){ return 267;}

DiamondAxe::DiamondAxe():BaseItem()
{
	textureRow = 8;

	plane = 3;
	durabilityPoints = 1562;
	stackable = false;

	name = "Diamond Axe";
}

item_t DiamondAxe::getID(){ return 268;}

GoldenAxe::GoldenAxe():BaseItem()
{
	textureRow = 8;

	plane = 4;
	durabilityPoints = 33;
	stackable = false;

	name = "Golden Axe";
}

item_t GoldenAxe::getID(){ return 269;}

WoodenHoe::WoodenHoe():BaseItem()
{
	textureRow = 7;

	plane = 0;
	durabilityPoints = 60;
	stackable = false;

	name = "Wooden Hoe";
}

item_t WoodenHoe::getID(){ return 270;}

StoneHoe::StoneHoe():BaseItem()
{
	textureRow = 7;

	plane = 1;
	durabilityPoints = 132;
	stackable = false;

	name = "Stone Hoe";
}

item_t StoneHoe::getID(){ return 271;}

IronHoe::IronHoe():BaseItem()
{
	textureRow = 7;

	plane = 2;
	durabilityPoints = 251;
	stackable = false;

	name = "Iron Hoe";
}

item_t IronHoe::getID(){ return 272;}

DiamondHoe::DiamondHoe():BaseItem()
{
	textureRow = 7;

	plane = 3;
	durabilityPoints = 1562;
	stackable = false;

	name = "Diamond Hoe";
}

item_t DiamondHoe::getID(){ return 273;}

GoldenHoe::GoldenHoe():BaseItem()
{
	textureRow = 7;

	plane = 4;
	durabilityPoints = 33;
	stackable = false;

	name = "Golden Hoe";
}

item_t GoldenHoe::getID(){ return 274;}

Scissors::Scissors():BaseItem()
{
	textureRow = 10;

	plane = 13;
	durabilityPoints = 239;
	stackable = false;

	name = "Shears";
}

item_t Scissors::getID(){ return 275;}

Stick::Stick():BaseItem()
{
	textureRow = 12;

	plane = 5;

	name = "Stick";
}

item_t Stick::getID(){ return 276;}

Coal::Coal():BaseItem()
{
	textureRow = 15;

	plane = 7;

	name = "Coal";
}

item_t Coal::getID(){ return 277;}

IronBar::IronBar():BaseItem()
{
	textureRow = 14;

	plane = 7;

	name = "Iron Ingot";
}

item_t IronBar::getID(){ return 278;}

DiamondItem::DiamondItem():BaseItem()
{
	textureRow = 12;

	plane = 7;

	name = "Diamond";
}

item_t DiamondItem::getID(){ return 279;}

GoldenBar::GoldenBar():BaseItem()
{
	textureRow = 13;

	plane = 7;

	name = "Gold Ingot";
}

item_t GoldenBar::getID(){ return 280;}

FireItem::FireItem():BaseItem()
{
	textureRow = 0;

	plane = 0;
}

item_t FireItem::getID(){ return 281;}

LigniteCoal::LigniteCoal():BaseItem()
{
	textureRow = 14;

	plane = 9;
}

item_t LigniteCoal::getID(){ return 282;}

Clay::Clay():BaseItem()
{
	textureRow = 12;

	plane = 9;

	furnItem = 289;

	name = "Clay";
}

item_t Clay::getID(){ return 283;}


Apple::Apple():BaseItem()
{
	textureRow = 15;

	plane = 10;

	name = "Apple";
}

item_t Apple::getID(){ return 284;}


GoldenApple::GoldenApple():BaseItem()
{
	textureRow = 15;

	plane = 11;

	name = "Golden Apple";
}

item_t GoldenApple::getID(){ return 285;}

WheatSeeds::WheatSeeds():BaseItem()
{
	textureRow = 15;

	plane = 9;

	name = "Seeds";
}

item_t WheatSeeds::getID(){ return 286;}

Wheat::Wheat():BaseItem()
{
	textureRow = 14;

	plane = 9;

	name = "Wheat";
}

item_t Wheat::getID(){ return 287;}


WhiteBread::WhiteBread():BaseItem()
{
	textureRow = 13;

	plane = 9;

	name = "Bread";
}

item_t WhiteBread::getID(){ return 288;}

Brick::Brick():BaseItem()
{
	textureRow = 14;

	plane = 6;

	name = "Brick";
}

item_t Brick::getID(){ return 289;}

Busket::Busket():BaseItem()
{
	textureRow = 11;

	plane = 10;
	stackable = false;

	name = "Bucket";
}

item_t Busket::getID(){ return 290;}

WaterBusket::WaterBusket():BaseItem()
{
	textureRow = 11;

	plane = 11;
	stackable = false;

	name = "Water Bucket";
}

item_t WaterBusket::getID(){ return 291;}

LavaBusket::LavaBusket():BaseItem()
{
	textureRow = 11;

	plane = 12;
	stackable = false;

	name = "Lava Bucket";
}

item_t LavaBusket::getID(){ return 292;}

CaneItem::CaneItem():BaseItem()
{
	textureRow = 14;

	plane = 11;

	name = "Sugar Canes";
}

item_t CaneItem::getID(){ return 293;}

Flower1Item::Flower1Item():BaseItem()
{
	textureRow = 15;

	plane = 12;

	name = "Rose";
	terrainTexture = true;
}

item_t Flower1Item::getID(){ return 294;}

Flower2Item::Flower2Item():BaseItem()
{
	textureRow = 15;

	plane = 13;

	name = "Dandelion";
	terrainTexture = true;
}

item_t Flower2Item::getID(){ return 295;}

OakSaplingItem::OakSaplingItem():BaseItem()
{
	textureRow = 15;

	plane = 15;

	name = "Oak Sapling";
	terrainTexture = true;
}

item_t OakSaplingItem::getID(){ return 296;}

Paper::Paper():BaseItem()
{
	textureRow = 12;

	plane = 10;

	name = "Paper";
}

item_t Paper::getID(){ return 297;}

Book::Book():BaseItem()
{
	textureRow = 12;

	plane = 11;

	name = "Book";
}

item_t Book::getID(){ return 298;}

SnowBallItem::SnowBallItem():BaseItem()
{
	textureRow = 15;

	plane = 14;

	name = "Snowball";
}

item_t SnowBallItem::getID(){ return 299;}

MooshroomItem1::MooshroomItem1():BaseItem()
{
	textureRow = 14;

	plane = 12;

	name = "Mushroom";
	terrainTexture = true;
}

item_t MooshroomItem1::getID(){ return 300;}


MooshroomItem2::MooshroomItem2():BaseItem()
{
	textureRow = 14;

	plane = 13;

	name = "Mushroom";
	terrainTexture = true;
}

item_t MooshroomItem2::getID(){ return 301;}

Bowl::Bowl():BaseItem()
{
	textureRow = 11;

	plane = 7;

	name = "Bowl";
}

item_t Bowl::getID(){ return 302;}

MooshroomBowl::MooshroomBowl():BaseItem()
{
	textureRow = 11;

	plane = 8;

	name = "Mushroom Stew";
}

item_t MooshroomBowl::getID(){ return 303;}

IronWateringCan::IronWateringCan():BaseItem()
{
	textureRow = 1;

	plane = 3;
}

item_t IronWateringCan::getID(){ return 304;}

DiamondWateringCan::DiamondWateringCan():BaseItem()
{
	textureRow = 1;

	plane = 4;
}

item_t DiamondWateringCan::getID(){ return 305;}

DoorItem::DoorItem():BaseItem()
{
	textureRow = 13;

	plane = 11;
	stackable = false;

	name = "Door";
}

item_t DoorItem::getID(){ return 306;}

LadderItem::LadderItem():BaseItem()
{
	textureRow = 10;

	plane = 3;

	name = "Ladder";
	terrainTexture = true;
}

item_t LadderItem::getID(){ return 307;}

TorchItem::TorchItem():BaseItem()
{
	textureRow = 10;

	plane = 0;

	name = "Torch";
	terrainTexture = true;
}

item_t TorchItem::getID(){ return 308;}

SpruceSaplingItem::SpruceSaplingItem():BaseItem()
{
	textureRow = 15;

	plane = 14;

	name = "Spruce Sapling";
	terrainTexture = true;
}

item_t SpruceSaplingItem::getID(){ return 309;}

BirchSaplingItem::BirchSaplingItem():BaseItem()
{
	textureRow = 14;

	plane = 14;

	name = "Birch Sapling";
	terrainTexture = true;
}

item_t BirchSaplingItem::getID(){ return 310;}

MelonItem::MelonItem():BaseItem()
{
	textureRow = 9;

	plane = 13;

	name = "Melon";
}

item_t MelonItem::getID(){ return 311;}

MelonSeeds::MelonSeeds():BaseItem()
{
	textureRow = 12;

	plane = 14;

	name = "Melon Seeds";
}

item_t MelonSeeds::getID(){ return 312;}

Sugar::Sugar():BaseItem()
{
	textureRow = 15;

	plane = 13;

	name = "Sugar";
}

item_t Sugar::getID(){ return 313;}

CocoaBeans::CocoaBeans():BaseItem()
{
	textureRow = 8;
	plane = 14;
	itemType = 'Y';

	name = "Cocoa Beans";
}

item_t CocoaBeans::getID(){ return 314;}

Cookie::Cookie():BaseItem()
{
	textureRow = 10;

	plane = 12;

	name = "Cookie";
}

item_t Cookie::getID(){ return 315;}

RawBeef::RawBeef():BaseItem()
{
	textureRow = 9;

	plane = 9;
	furnItem = 317;

	name = "Raw Beef";
}

item_t RawBeef::getID(){ return 316;}

Steak::Steak():BaseItem()
{
	textureRow = 9;

	plane = 10;

	name = "Steak";
}

item_t Steak::getID(){ return 317;}

LapisLazuli::LapisLazuli():BaseItem()
{
	textureRow = 7;
	plane = 14;
	itemType = 'Y';

	name = "Lapis Lazuli";
}

item_t LapisLazuli::getID(){ return 318;}

MilkBusket::MilkBusket():BaseItem()
{
	textureRow = 11;

	plane = 13;

	name = "Milk";
	stackable = false;
}

item_t MilkBusket::getID(){ return 319;}

CakeItem::CakeItem():BaseItem()
{
	textureRow = 14;

	plane = 13;

	name = "Cake";
	stackable = false;
}

item_t CakeItem::getID(){ return 320;}

Bone::Bone():BaseItem()
{
	textureRow = 14;

	plane = 12;

	name = "Bone";
}

item_t Bone::getID(){ return 321;}

BoneMeal::BoneMeal():BaseItem()
{
	textureRow = 4;
	plane = 15;
	itemType = 'Y';

	name = "Bone Meal";
}

item_t BoneMeal::getID(){ return 322;}

Leather::Leather():BaseItem()
{
	textureRow = 9;

	plane = 7;

	name = "Leather";
}

item_t Leather::getID(){ return 323;}

SampleHelmet::SampleHelmet():BaseItem()
{
	textureRow = 15;

	plane = 15;
	stackable = false;
}

item_t SampleHelmet::getID(){ return 324;}

SampleChestplate::SampleChestplate():BaseItem()
{
	textureRow = 14;

	plane = 15;
	stackable = false;
}

item_t SampleChestplate::getID(){ return 325;}

SampleLeggings::SampleLeggings():BaseItem()
{
	textureRow = 13;

	plane = 15;
	stackable = false;
}

item_t SampleLeggings::getID(){ return 326;}

SampleBoots::SampleBoots():BaseItem()
{
	textureRow = 12;

	plane = 15;
	stackable = false;
}

item_t SampleBoots::getID(){ return 327;}

LeatherHelmet::LeatherHelmet():BaseItem()
{
	textureRow = 15;

	plane = 0;
	itemType = 'H';
	durabilityPoints = 55;
	stackable = false;

	name = "Leather Helmet";
}

item_t LeatherHelmet::getID(){ return 328;}

LeatherChestplate::LeatherChestplate():BaseItem()
{
	textureRow = 14;

	plane = 0;
	itemType = 'C';
	durabilityPoints = 80;
	stackable = false;

	name = "Leather Chestplate";
}

item_t LeatherChestplate::getID(){ return 329;}

LeatherLeggings::LeatherLeggings():BaseItem()
{
	textureRow = 13;

	plane = 0;
	itemType = 'L';
	durabilityPoints = 75;
	stackable = false;

	name = "Leather Leggings";
}

item_t LeatherLeggings::getID(){ return 330;}

LeatherBoots::LeatherBoots():BaseItem()
{
	textureRow = 12;

	plane = 0;
	itemType = 'B';
	durabilityPoints = 65;
	stackable = false;

	name = "Leather Boots";
}

item_t LeatherBoots::getID(){ return 331;}

ChainmailHelmet::ChainmailHelmet():BaseItem()
{
	textureRow = 15;

	plane = 1;
	itemType = 'H';
	durabilityPoints = 165;
	stackable = false;

	name = "Chainmail Helmet";
}

item_t ChainmailHelmet::getID(){ return 332;}

ChainmailChestplate::ChainmailChestplate():BaseItem()
{
	textureRow = 14;

	plane = 1;
	itemType = 'C';
	durabilityPoints = 240;
	stackable = false;

	name = "Chainmail Chestplate";
}

item_t ChainmailChestplate::getID(){ return 333;}

ChainmailLeggings::ChainmailLeggings():BaseItem()
{
	textureRow = 13;

	plane = 1;
	itemType = 'L';
	durabilityPoints = 225;
	stackable = false;

	name = "Chainmail Leggings";
}

item_t ChainmailLeggings::getID(){ return 334;}

ChainmailBoots::ChainmailBoots():BaseItem()
{
	textureRow = 12;

	plane = 1;
	itemType = 'B';
	durabilityPoints = 195;
	stackable = false;

	name = "Chainmail Boots";
}

item_t ChainmailBoots::getID(){ return 335;}

IronHelmet::IronHelmet():BaseItem()
{
	textureRow = 15;

	plane = 2;
	itemType = 'H';
	durabilityPoints = 165;
	stackable = false;

	name = "Iron Helmet";
}

item_t IronHelmet::getID(){ return 336;}

IronChestplate::IronChestplate():BaseItem()
{
	textureRow = 14;

	plane = 2;
	itemType = 'C';
	durabilityPoints = 240;
	stackable = false;

	name = "Iron Chestplate";
}

item_t IronChestplate::getID(){ return 337;}

IronLeggings::IronLeggings():BaseItem()
{
	textureRow = 13;

	plane = 2;
	itemType = 'L';
	durabilityPoints = 225;
	stackable = false;

	name = "Iron Leggings";
}

item_t IronLeggings::getID(){ return 338;}

IronBoots::IronBoots():BaseItem()
{
	textureRow = 12;

	plane = 2;
	itemType = 'B';
	durabilityPoints = 195;
	stackable = false;

	name = "Iron Boots";
}

item_t IronBoots::getID(){ return 339;}

DiamondHelmet::DiamondHelmet():BaseItem()
{
	textureRow = 15;

	plane = 3;
	itemType = 'H';
	durabilityPoints = 363;
	stackable = false;

	name = "Diamond Helmet";
}

item_t DiamondHelmet::getID(){ return 340;}

DiamondChestplate::DiamondChestplate():BaseItem()
{
	textureRow = 14;

	plane = 3;
	itemType = 'C';
	durabilityPoints = 528;
	stackable = false;

	name = "Diamond Chestplate";
}

item_t DiamondChestplate::getID(){ return 341;}

DiamondLeggings::DiamondLeggings():BaseItem()
{
	textureRow = 13;

	plane = 3;
	itemType = 'L';
	durabilityPoints = 495;
	stackable = false;

	name = "Diamond Leggings";
}

item_t DiamondLeggings::getID(){ return 342;}

DiamondBoots::DiamondBoots():BaseItem()
{
	textureRow = 12;

	plane = 3;
	itemType = 'B';
	durabilityPoints = 429;
	stackable = false;

	name = "Diamond Boots";
}

item_t DiamondBoots::getID(){ return 343;}

GoldenHelmet::GoldenHelmet():BaseItem()
{
	textureRow = 15;

	plane = 4;
	itemType = 'H';
	durabilityPoints = 77;
	stackable = false;

	name = "Golden Helmet";
}

item_t GoldenHelmet::getID(){ return 344;}

GoldenChestplate::GoldenChestplate():BaseItem()
{
	textureRow = 14;

	plane = 4;
	itemType = 'C';
	durabilityPoints = 112;
	stackable = false;

	name = "Golden Chestplate";
}

item_t GoldenChestplate::getID(){ return 345;}

GoldenLeggings::GoldenLeggings():BaseItem()
{
	textureRow = 13;

	plane = 4;
	itemType = 'L';
	durabilityPoints = 105;
	stackable = false;

	name = "Golden Leggings";
}

item_t GoldenLeggings::getID(){ return 346;}

GoldenBoots::GoldenBoots():BaseItem()
{
	textureRow = 12;

	plane = 4;
	itemType = 'B';
	durabilityPoints = 91;
	stackable = false;

	name = "Golden Boots";
}

item_t GoldenBoots::getID(){ return 347;}

RottenFlesh::RottenFlesh():BaseItem()
{
	textureRow = 10;

	plane = 11;

	name = "Rotten Flesh";
}

item_t RottenFlesh::getID(){ return 348;}

GlassPanelItem::GlassPanelItem():BaseItem()
{
	textureRow = 12;
	plane = 1;

	terrainTexture = true;

	name = "Glass Panel";
}

item_t GlassPanelItem::getID(){ return 349;}

IronBarsItem::IronBarsItem():BaseItem()
{
	textureRow = 10;
	plane = 5;

	terrainTexture = true;

	name = "Iron Bars";
}

item_t IronBarsItem::getID(){ return 350;}

Flint::Flint():BaseItem()
{
	textureRow = 15;
	plane = 6;

	name = "Flint";
}

item_t Flint::getID(){ return 351;}

Gunpowder::Gunpowder():BaseItem()
{
	textureRow = 13;
	plane = 8;

	name = "Gunpowder";
}

item_t Gunpowder::getID(){ return 352;}

Redstone::Redstone():BaseItem()
{
	textureRow = 12;
	plane = 8;

	name = "Redstone";
}

item_t Redstone::getID(){ return 353;}

Clock::Clock():BaseItem()
{
	textureRow = 11;
	plane = 6;
	stackable = false;

	name = "Clock";
}

item_t Clock::getID(){ return 354;}

Compass::Compass():BaseItem()
{
	textureRow = 12;
	plane = 6;
	stackable = false;

	name = "Compass";
}

item_t Compass::getID(){ return 355;}

Saddle::Saddle():BaseItem()
{
	textureRow = 9;
	plane = 8;
	stackable = false;

	name = "Saddle";
}

item_t Saddle::getID(){ return 356;}

Sign::Sign():BaseItem()
{
	textureRow = 13;
	plane = 10;
	stackable = false;

	name = "Sign";
}

item_t Sign::getID(){ return 357;}

ItemFrameItem::ItemFrameItem():BaseItem()
{
	textureRow = 3;
	plane = 14;

	name = "Item Frame";
}

item_t ItemFrameItem::getID(){ return 358;}

FlowerPotItem::FlowerPotItem():BaseItem()
{
	textureRow = 3;
	plane = 15;

	name = "Flower Pot";
}

item_t FlowerPotItem::getID(){ return 359;}

PaintningItem::PaintningItem():BaseItem()
{
	textureRow = 14;
	plane = 10;

	name = "Paintning";
}

item_t PaintningItem::getID(){ return 360;}

BedItem::BedItem():BaseItem()
{
	textureRow = 13;
	plane = 13;
	stackable = false;

	name = "Bed";
}

item_t BedItem::getID(){ return 361;}


Disk1::Disk1():BaseItem()
{
	textureRow = 0;
	plane = 0;
	stackable = false;

	name = "Music Disk";
}

item_t Disk1::getID(){ return 362;}

Disk2::Disk2():BaseItem()
{
	textureRow = 0;
	plane = 1;
	stackable = false;

	name = "Music Disk";
}

item_t Disk2::getID(){ return 363;}

Disk3::Disk3():BaseItem()
{
	textureRow = 0;
	plane = 2;
	stackable = false;

	name = "Music Disk";
}

item_t Disk3::getID(){ return 364;}


LightGrayDye::LightGrayDye():BaseItem()
{
	textureRow = 4;
	plane = 14;
	itemType = 'Y';

	name = "Light Gray Dye";
}

item_t LightGrayDye::getID(){ return 365;}

GrayDye::GrayDye():BaseItem()
{
	textureRow = 11;
	plane = 15;
	itemType = 'Y';

	name = "Gray Dye";
}

item_t GrayDye::getID(){ return 366;}

InkSac::InkSac():BaseItem()
{
	textureRow = 11;
	plane = 14;
	itemType = 'Y';

	name = "Ink Sac";
}

item_t InkSac::getID(){ return 367;}

PinkDye::PinkDye():BaseItem()
{
	textureRow = 10;
	plane = 15;
	itemType = 'Y';

	name = "Pink Dye";
}

item_t PinkDye::getID(){ return 368;}

RoseRed::RoseRed():BaseItem()
{
	textureRow = 10;
	plane = 14;
	itemType = 'Y';

	name = "Rose Red";
}

item_t RoseRed::getID(){ return 369;}

OrangeDye::OrangeDye():BaseItem()
{
	textureRow = 5;
	plane = 15;
	itemType = 'Y';

	name = "Orange Dye";
}

item_t OrangeDye::getID(){ return 370;}

DandelionYellow::DandelionYellow():BaseItem()
{
	textureRow = 8;
	plane = 15;
	itemType = 'Y';

	name = "Dandelion Yellow";
}

item_t DandelionYellow::getID(){ return 371;}

LimeDye::LimeDye():BaseItem()
{
	textureRow = 9;
	plane = 15;
	itemType = 'Y';

	name = "Lime Dye";
}

item_t LimeDye::getID(){ return 372;}

CactusGreen::CactusGreen():BaseItem()
{
	textureRow = 9;
	plane = 14;
	itemType = 'Y';

	name = "Cactus Green";
}

item_t CactusGreen::getID(){ return 373;}

LightBlueDye::LightBlueDye():BaseItem()
{
	textureRow = 7;
	plane = 15;
	itemType = 'Y';

	name = "Light Blue Dye";
}

item_t LightBlueDye::getID(){ return 374;}

PurpleDye::PurpleDye():BaseItem()
{
	textureRow = 6;
	plane = 14;
	itemType = 'Y';

	name = "Purple Dye";
}

item_t PurpleDye::getID(){ return 375;}

MagnetaDye::MagnetaDye():BaseItem()
{
	textureRow = 6;
	plane = 15;
	itemType = 'Y';

	name = "Magneta Dye";
}

item_t MagnetaDye::getID(){ return 376;}


PumpkinSeeds::PumpkinSeeds():BaseItem()
{
	textureRow = 12;
	plane = 13;

	name = "Pumpkin Seeds";
}

item_t PumpkinSeeds::getID(){ return 377;}

FlintAndSteel::FlintAndSteel():BaseItem()
{
	textureRow = 15;
	plane = 5;
	stackable = false;

	durabilityPoints = 65;

	name = "Flint and Steel";
}

item_t FlintAndSteel::getID(){ return 378;}

Potato::Potato():BaseItem()
{
	textureRow = 8;
	plane = 7;
	furnItem = 380;

	name = "Potato";
}

item_t Potato::getID(){ return 379;}

BakedPotato::BakedPotato():BaseItem()
{
	textureRow = 8;
	plane = 6;

	name = "Baked Potato";
}

item_t BakedPotato::getID(){ return 380;}

PoisonedPotato::PoisonedPotato():BaseItem()
{
	textureRow = 7;
	plane = 6;

	name = "Poisioned Potato";
}

item_t PoisonedPotato::getID(){ return 381;}

Carrot::Carrot():BaseItem()
{
	textureRow = 8;
	plane = 8;

	name = "Carrot";
}

item_t Carrot::getID(){ return 382;}

Disk4::Disk4():BaseItem()
{
	textureRow = 0;
	plane = 3;
	stackable = false;

	name = "Music Disk";
}

item_t Disk4::getID(){ return 383;}

Disk5::Disk5():BaseItem()
{
	textureRow = 0;
	plane = 4;
	stackable = false;

	name = "Music Disk";
}

item_t Disk5::getID(){ return 384;}

Disk6::Disk6():BaseItem()
{
	textureRow = 0;
	plane = 5;
	stackable = false;

	name = "Music Disk";
}

item_t Disk6::getID(){ return 385;}

Disk7::Disk7():BaseItem()
{
	textureRow = 0;
	plane = 6;
	stackable = false;

	name = "Music Disk";
}

item_t Disk7::getID(){ return 386;}

Disk8::Disk8():BaseItem()
{
	textureRow = 0;
	plane = 7;
	stackable = false;

	name = "Music Disk";
}

item_t Disk8::getID(){ return 387;}

Disk9::Disk9():BaseItem()
{
	textureRow = 0;
	plane = 8;
	stackable = false;

	name = "Music Disk";
}

item_t Disk9::getID(){ return 388;}

Disk10::Disk10():BaseItem()
{
	textureRow = 0;
	plane = 9;
	stackable = false;

	name = "Music Disk";
}

item_t Disk10::getID(){ return 389;}

Disk11::Disk11():BaseItem()
{
	textureRow = 0;
	plane = 10;
	stackable = false;

	name = "Music Disk";
}

item_t Disk11::getID(){ return 390;}

Disk12::Disk12():BaseItem()
{
	textureRow = 0;
	plane = 11;
	stackable = false;

	name = "Music Disk";
}

item_t Disk12::getID(){ return 391;}
















