#include "LameMob.h"

LameMob::LameMob(float x, float y, float z)
{
    collideBox.x = 1;
    collideBox.y = 0.7;
    collideBox.z = 1;

    pos.x = x;
    pos.y = y;
    pos.z = z;

    hp = 10;
    triangles = 30;

    bent = 0;
    go_ = 0;
    way = 0;
    on_rails = 0;
    fall = 0;

}

void LameMob::Go(int block, float x, float y, float z)
{
if(go_ == 1 && on_rails == 1 && fall == 0)
{
switch(block)
{
    case 137:
    if (way == 0)
    {
        pos.x += spd;
    }
    case 138:
    if (way == 1)
    {
        pos.x -= spd;
    }
    case 139:
    if (way == 2)
    {
        pos.z += spd;
    }
    case 140:
    if (way == 3)
    {
        pos.z -= spd;
    }
    case 141:
    if (way == 0)
    {
        if(pos.x - x < 0.1 || pos.x - x > -0.1 )
        {
            pos.x = x;
        }

        if(pos.x == x)
        {
            way = 2;
        }
        else
        {
            pos.x += spd;

        }
    }



    if (way == 1)
    {
        pos.x -= spd;
    }

    if (way == 2)
    {
        pos.z += spd;
    }



    if (way == 3)
    {
        if(pos.z - z < 0.1 || pos.z - z > -0.1 )
        {
            pos.z = z;
        }

        if(pos.z == z)
        {
            way = 1;
        }
        else
        {
            pos.z -= spd;
        }
    }

                    /*    ====            */
                    /*        \\          */
                    /*        []          */
                    /*        []          */



}
}

}


LameMob::~LameMob()
{
}
