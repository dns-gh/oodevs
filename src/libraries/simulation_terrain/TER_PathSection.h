// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef SIMULATION_TERRAIN_PATHSECTION_H
#define SIMULATION_TERRAIN_PATHSECTION_H

#include "MT_Tools/MT_Vector2D.h"
#include <pathfind/TerrainPathPoint.h>
#include <pathfind/TerrainRule_ABC.h>

class TER_Pathfinder_ABC;

// =============================================================================
// @class  DEC_Tools
// Created: NLD 2005-02-22
// =============================================================================
class TER_PathSection : private boost::noncopyable
{
public:
             TER_PathSection(
                    std::unique_ptr< TerrainRule_ABC > rule,
                    const MT_Vector2D& vStartPoint, const MT_Vector2D& vEndPoint,
                    bool needRefine, bool useStrictClosest );
    virtual ~TER_PathSection();

    //! @name Operations
    //@{
    PathResultPtr Execute  ( TER_Pathfinder_ABC& pathfind,
            unsigned int deadlineSeconds );
    double GetLength() const;
    void   Cancel   ();
    //@}

    //! @name Accessors
    //@{
    const MT_Vector2D&  GetPosStart () const;
    const MT_Vector2D&  GetPosEnd   () const; // $$$$ AGE 2005-02-25: ...
    void                SetPosStart ( const MT_Vector2D& point );
    //@}

private:
    class Canceler;

private:
    std::unique_ptr< TerrainRule_ABC > rule_;
    std::unique_ptr< Canceler > canceler_;
    MT_Vector2D startPoint_;
    const MT_Vector2D endPoint_;
    const bool needRefine_;
    const bool useStrictClosest_;
    unsigned int nAddedPoints_;
};

#endif // SIMULATION_TERRAIN_PATHSECTION_H
