#ifndef DROP_H_
#define DROP_H_

#include <Aurora/Graphics/Vertex.h>
#include <Aurora/Graphics/RenderManager.h>
#include <pspmath.h>
#include <Aurora/Math/Frustum.h>
#include <LameCraft/CraftWorld2.h>
#include <Aurora/Math/BoundingBox.h>

class CraftWorld;

class Drop
{
public:
    Drop(Vector3 position2, int id2, int amount2, bool stackable2, int num2);
    ~Drop();

    void CheckOtherDrops();
    void BuildVerticesForItem();
    void Update(float dt);
    void Render(Frustum &camFrustum, Vector3 playerPos, float camAngle);
    void SetVelociy(Vector3 velocity2);

    int GetId();
    int GetAmount();
    bool GetStackable();

    Vector3 position;
    int amount;
    bool toDestroy;
    bool getMe;

    CraftWorld* myCrft;

private:

    Vector3 velocity;

    float size2;
    float timeOut;
    float levit;
    float existingTime;

    int id;
    int num;
    bool stackable;

    bool onGround;
    bool checkForOtherDrops;
    bool active;

    float angleBlock;
    BoundingBox bBox;

};

#endif

