// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef TER_PATHFINDER_ABC__
#define TER_PATHFINDER_ABC__

#include <geometry/Types.h>

namespace pathfind
{
    class AStarManagementCallback_ABC;
}

namespace tools
{
namespace thread
{
    template< typename T > class Handler_ABC;
}
}

class TerrainRule_ABC;
class TerrainPathPoint;

// =============================================================================
/** @class  TER_PathFinder_ABC
    @brief  TER_Pathfinder_ABC interface
*/
// Created: BAX 2012-12-11
// =============================================================================
class TER_Pathfinder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    //! @name
    //@{
             TER_Pathfinder_ABC() {}
    virtual ~TER_Pathfinder_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SetId( size_t id ) = 0;
    virtual void SetChoiceRatio( float ratio ) = 0;
    virtual void SetConfiguration( unsigned nRefining, unsigned int nSubdivisions ) = 0;
    virtual bool ComputePath( const geometry::Point2f& from, const geometry::Point2f& to,
                              TerrainRule_ABC& rule,
                              pathfind::AStarManagementCallback_ABC* callback,
                              tools::thread::Handler_ABC< TerrainPathPoint >& handler ) = 0;
    //@}
};

#endif
