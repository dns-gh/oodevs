//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/Rep_PathPoint.cpp $
// $Author: Nld $
// $Modtime: 7/06/05 16:39 $
// $Revision: 6 $
// $Workfile: Rep_PathPoint.cpp $
//
//*****************************************************************************

#include "Rep_PathPoint.h"
#include "wrapper/Hook.h"

using namespace sword;
using namespace sword::movement;

DECLARE_HOOK( AddPathPoint, void, ( unsigned int entity, const boost::shared_ptr< sword::movement::PathPoint >& point ) )
DECLARE_HOOK( RemovePathPoint, void, ( unsigned int entity, const boost::shared_ptr< sword::movement::PathPoint >& point ) )

//-----------------------------------------------------------------------------
// Name: Rep_PathPoint constructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
Rep_PathPoint::Rep_PathPoint( const MT_Vector2D& vPos, E_TypePoint nTypePoint, const TerrainData& nTypeTerrain, const char* szDIARepType )
    : PathPoint( vPos, eTypePointSpecial, nTypePoint, szDIARepType )
    , nTypeTerrain_( nTypeTerrain )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Rep_PathPoint destructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
Rep_PathPoint::~Rep_PathPoint()
{
}

//-----------------------------------------------------------------------------
// Name: Rep_PathPoint::SendToDIA
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
void Rep_PathPoint::SendToDIA( unsigned int entity, boost::shared_ptr< PathPoint > point ) const
{
    if( entity_ )
        return;
    // ATTENTION, si cette fonction est appelée, alors l'agent physique s'est automatiquement arrêté sur la position du point...
    GET_HOOK( AddPathPoint )( entity, point );
    entity_ = entity;
}

//-----------------------------------------------------------------------------
// Name: Rep_PathPoint::RemoveFromDIA
// Created: MGD 10-03-11
//-----------------------------------------------------------------------------
void Rep_PathPoint::RemoveFromDIA( boost::shared_ptr< PathPoint > self )
{
    if( entity_ )
        GET_HOOK( RemovePathPoint )( *entity_, self );
}

//-----------------------------------------------------------------------------
// Name: Rep_PathPoint::GetTypeTerrain
// Created: AGN 03-03-25
//-----------------------------------------------------------------------------
const TerrainData& Rep_PathPoint::GetTypeTerrain() const
{
    return nTypeTerrain_;
}
