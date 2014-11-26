// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __DEC_TerrainFunctions_h_
#define __DEC_TerrainFunctions_h_

namespace sword
{
    class Brain;
}

class DEC_Decision_ABC;
class TER_Localisation;
class MT_Vector2D;

// =============================================================================
/** @class  DEC_TerrainFunctions
    @brief  DEC_TerrainFunctions
*/
// Created: CMA 2011-08-30
// =============================================================================
class DEC_TerrainFunctions
{
public:
    static void Register( sword::Brain& brain );

    //! @name Functions
    //@{

    // Keypoint
    static std::vector< boost::shared_ptr< MT_Vector2D > > GetCrossroads( DEC_Decision_ABC* agent );
    static std::vector< boost::shared_ptr< MT_Vector2D > > FindSafetyPositionsWithinCircle( DEC_Decision_ABC* agent, float radius, float safetyDistance );
    static std::vector< boost::shared_ptr< MT_Vector2D > > GetRoadIntersectionsWithZone( const TER_Localisation* zone );
    static bool IsLinearRiverInBetween( const MT_Vector2D* from, const MT_Vector2D* to );
    static bool IsWaterInBetween( const MT_Vector2D* from, const MT_Vector2D* to );
    static std::pair< unsigned int, unsigned int > GetTerrainData( const MT_Vector2D* location );

    // Movement
    static bool CanMoveOn( const DEC_Decision_ABC* agent, boost::shared_ptr< MT_Vector2D > position );
    //@}
};

#endif // __DEC_TerrainFunctions_h_
