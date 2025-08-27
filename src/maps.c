#include  "common.h"

//----------------------------------------------------------------------------------
// Function Predefs
//----------------------------------------------------------------------------------

int checkInterference(Vector3 pos);


//----------------------------------------------------------------------------------
// Grid Shot
//----------------------------------------------------------------------------------
void Gridshot(void) 
{
    int n = targetEngine.targetCount;
    int maxFreq = 50;

    for (int i = 0; i < n; i++)
    {
        if (targets[i].health <= 0)
        {
            while (maxFreq > 0){
                //Attempt to create Target (maxFreq = 50)
                int x = (rand() % (targetEngine.xVar / 10)) - (int)(targetEngine.xVar / (2 * targetEngine.gap));
                int y = (rand() % (targetEngine.yVar / 10)) + 1;
                int z = targetEngine.maxZ / 10;

                Vector3 pos = {x,y,z};

                if (!checkInterference(pos)) {
                    Target newTarget = {pos, targetEngine.targetHealth};
                    targets[i] = newTarget;
                    break;
                }
                maxFreq -= 1;
                if (maxFreq == 0) {perror("Not Enough Space for Targets... Aborting");}
            }


        }
    }
}

//----------------------------------------------------------------------------------
// Track Shot
//----------------------------------------------------------------------------------
static int dir = 1;
void Track(void) 
{
    
    if (targets[0].health <= 0) 
    {
        Vector3 pos = {0, targetEngine.yVar/ 10 + 1, targetEngine.maxZ / 10};
        targets[0] = (Target){pos, 9999};
    }

    if ((gameEngine.timer > 0.00))
    {
        Target old = targets[0];
        float delta = GetFrameTime();
        float speed = targetEngine.targetSpeed / 10;

        targets[0].position.x += dir * delta * speed;

        if (dir == 1 && old.position.x > (targetEngine.xVar / 10))
        {
            old.position.x = targetEngine.xVar / 10;
            dir = -1;
        }

        else if (dir == -1 && old.position.x < -(targetEngine.xVar / 10))
        {
            old.position.x = -(targetEngine.xVar / 10);
            dir = 1;
        }

    
}
    } 

//----------------------------------------------------------------------------------
// FLOAT Shot
//----------------------------------------------------------------------------------

void Floatshot() {
    int n = targetEngine.targetCount;
    float speed = targetEngine.targetSpeed / 10;
    float xVar = targetEngine.xVar / 10;
    float yVar = targetEngine.yVar / 10;
    float delta = GetFrameTime();

    for (int i = 0; i < n; i++)
    {
        if (targets[i].health <= 0)
        {
                //Attempt to create Target (maxFreq = 50)
                int x = (rand() % (targetEngine.xVar / 10)) - (int)(targetEngine.xVar / (2 * targetEngine.gap));
                int y = (rand() % (targetEngine.yVar / 10)) + 1;
                int z = targetEngine.maxZ / 10;

                Vector3 pos = {x,y,z};

                if (!checkInterference(pos)) {
                    Target newTarget = {pos, targetEngine.targetHealth};
                    targets[i] = newTarget;
                    adjVelos[i] = (Vector3){0,0,0};
                    break;
                }
        }

        //If Velocity less than 0.1 (basically non-existent), assign a velocity
        if (Vector3Length(adjVelos[i]) < 0.1)
        {
            adjVelos[i] = (Vector3){(float)(rand() % 3), (float)(rand() % 3), 0.0 };

            if (rand() % 2 == 0) {
                adjVelos[i].x = -adjVelos[i].x;
            }
             if (rand() % 2 == 0) {
                adjVelos[i].y = -adjVelos[i].y;
            }

            adjVelos[i] = Vector3Normalize(adjVelos[i]);
            adjVelos[i] = Vector3Scale(adjVelos[i], 2.0);
        }

        //Bounce off of boundaries
        if ((targets[i].position.x > xVar) || (targets[i].position.x < -xVar))
        {
            adjVelos[i].x = -1 * adjVelos[i].x;
        }

         if ((targets[i].position.y > yVar) || (targets[i].position.y < -yVar))
        {
            adjVelos[i].y = -1 * adjVelos[i].y;
        }

        //MOVE TARGETS BY VELOCITY
        targets[i].position.x = targets[i].position.x + adjVelos[i].x * delta;
        targets[i].position.y = targets[i].position.y + adjVelos[i].y * delta;
        
    }
}


//----------------------------------------------------------------------------------
// Helpers
//----------------------------------------------------------------------------------
int checkInterference(Vector3 pos)
{
    int n = targetEngine.targetCount;
    for (int i = 0; i < n; i++)
    {
        if (Vector3Distance(pos, targets[i].position) < (targetEngine.gap / 10))
        {
            return 1;
        }
    }
    return 0;

}