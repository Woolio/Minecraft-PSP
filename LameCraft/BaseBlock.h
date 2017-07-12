#ifndef BASEBLOCK_H
#define BASEBLOCK_H

#include <Aurora/Graphics/Vertex.h>
#include <Aurora/Math/BoundingBox.h>
#include <stdlib.h>
#include <string>
#include <Aurora/Graphics/RenderManager.h>
#include <pspmath.h>
#include <psptypes.h>

using namespace Aurora::Graphics;

typedef unsigned char block_t;

class BaseBlock
{
public:
	//информация о текстуре блока

	char upPlane_x, upPlane_y;
	char downPlane_x, downPlane_y;
	char leftPlane_x, leftPlane_y;
	char rightPlane_x, rightPlane_y;
    char botPlane_x, botPlane_y;
    char forPlane_x, forPlane_y;

	TexturesPSPVertex *verticesLeftRight;
	char pointsLeftRight;

    TexturesPSPVertex *verticesBackFront;
	char pointsBackFront;

    TexturesPSPVertex *verticesBottomTop;
	char pointsBottomTop;

	// другие параметры
	bool editable;              // разрушаемый блок? true false
	bool transparent;           // прозрачный блок? true false
	bool lightSource;           // светится? true false
	bool update;                // обновляемый блок? true false
	bool solid;                 // твердый блок? true false
	bool animated;
	bool allowLight;            // является ли блок препятствием для распространения света? true - нет ; false - да

    short furnItem;               // предмет, получаемый при переплавке блока (указывать id)
    short loot;                   // лут блока (указывать id)

    char material;              // материал блока (1 - дерево, 2 - камень, 3 - земля, 4 - листва, 5 - обсидиан, 6 - бедрок, 7 - руды, 8 - плотное дерево, 9 - плотный камень, 10 - металлы)
    //char points;              // количество точек у модели (36 - у блока т.к 6 сторон, на каждой по 2 треугольника = 12, в каждом треугольнике по 3 точки, 36 точек)
	char soundType;             // звук, когда наступаешь на блок (0 - звук травы, 1 - звук земли\гравия, 2 - звук камня, 3 - звук дерева, 5 - звук песка, 6 - звук снега)
    char blockModel;            // модель блока (0 - стандартный куб,
                                       //        1 - полублок,
                                       //        2 - забор,
                                       //        3 - крестовой объект(цветок, саженец),
                                       //        4 - модель пшеницы,
                                       //        5 - особый блок, модель которого вы можете найти в CraftWorld2.cpp,
                                       //        6 - ступенька z++
                                       //        7 - ступенька z--
                                       //        8 - ступенька x++
                                       //        9 - ступенька x--
                                       //        10 - панельный блок)

    char refraction;
    BoundingBox collideBox;

    std::string name;

    virtual bool MatchWithRecipe2x2(std::string recipe_string[2], int& return_id, int& return_am, bool& return_st); // may be in future
    virtual bool MatchWithRecipe3x3(std::string recipe_string[3], int& return_id, int& return_am, bool& return_st);

	BaseBlock();
	virtual ~BaseBlock();

	static block_t getBaseID();
};

#endif
