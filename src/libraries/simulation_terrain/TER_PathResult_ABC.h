// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef SIMULATION_TERRAIN_PATHRESULT_ABC
#define SIMULATION_TERRAIN_PATHRESULT_ABC

#include <boost/optional.hpp>

class MT_Vector2D;
class TerrainData;

// =============================================================================
/** @class  TER_PathResult_ABC
    @brief  Collects computed path points
*/
// Created: MCO 2014-05-14
// =============================================================================
class TER_PathResult_ABC : boost::noncopyable
{
public:
             TER_PathResult_ABC() {}
    virtual ~TER_PathResult_ABC() {}

    virtual void AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes,
            const TerrainData& nObjectTypesToNextPoint, bool beginPoint ) = 0;
};

#endif // SIMULATION_TERRAIN_PATHRESULT_ABC
