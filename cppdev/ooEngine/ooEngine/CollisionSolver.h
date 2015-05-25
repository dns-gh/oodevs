#ifndef _COLLISIONSOLVER_H_
#define _COLLISIONSOLVER_H_

#include "EngineObject.h"
#include "Geometry2D.h"
#include "LogTools.h"

class CollisionSolver : public EngineObject
{
public:
    CollisionSolver( LogTools& logger );
    bool CollisionBetween( const Geometry2D::Circle& circle1, const Geometry2D::Circle& circle2 ) const;

private:
    LogTools& logger_;
};

#endif