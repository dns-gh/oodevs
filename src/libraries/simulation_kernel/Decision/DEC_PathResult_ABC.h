// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DEC_PathResult_ABC_h_
#define __DEC_PathResult_ABC_h_

#include <boost/optional.hpp>

class MT_Vector2D;
class TerrainData;

// =============================================================================
/** @class  DEC_PathResult_ABC
    @brief  Collects computed path points
*/
// Created: MCO 2014-05-14
// =============================================================================
class DEC_PathResult_ABC : boost::noncopyable
{
public:
             DEC_PathResult_ABC() {}
    virtual ~DEC_PathResult_ABC() {}

    virtual void AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint, bool beginPoint ) = 0;
};

#endif // __DEC_PathResult_ABC_h_
