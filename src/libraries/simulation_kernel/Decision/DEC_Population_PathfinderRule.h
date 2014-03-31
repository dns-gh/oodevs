// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Population_PathfinderRule_h_
#define __DEC_Population_PathfinderRule_h_

#include <pathfind/TerrainRule_ABC.h>

class DEC_Population_Path;
class MT_Vector2D;
class Terrain_Data;
class TER_World;

// =============================================================================
// Created: AGE 2005-03-08
// =============================================================================
class DEC_Population_PathfinderRule : public TerrainRule_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_Population_PathfinderRule( const DEC_Population_Path& path );
    virtual ~DEC_Population_PathfinderRule();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DEC_Population_PathfinderRule( const DEC_Population_PathfinderRule& );            //!< Copy constructor
    DEC_Population_PathfinderRule& operator=( const DEC_Population_PathfinderRule& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual float EvaluateCost( const geometry::Point2f& from, const geometry::Point2f& to );
    virtual float GetCost( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween, std::ostream* reason );
    //@}

    //! @name Helpers
    //@{
    float GetChannelingCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    float GetTerrainCost( const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    float GetObjectsCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    //@}

private:
    //! @name Member data
    //@{
    const DEC_Population_Path& path_;
    const TER_World& world_;
    //@}
};

#endif // __DEC_Population_PathfinderRule_h_
