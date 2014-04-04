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

class DEC_Decision_ABC;
class MIL_AgentPion;
class TER_Localisation;
class MT_Vector2D;

namespace sword
{
    class Brain;
}

namespace directia
{
namespace tools
{
namespace binders
{
    class ScriptRef;
}
}
}

// =============================================================================
/** @class  DEC_TerrainFunctions
    @brief  DEC_TerrainFunctions
*/
// Created: CMA 2011-08-30
// =============================================================================
class DEC_TerrainFunctions
{
public:
    //! @name Functions
    //@{

    // Keypoint
    static void GetCrossroads( sword::Brain& brain, MIL_AgentPion& pion, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& table );
    static std::vector< boost::shared_ptr< MT_Vector2D > > FindSafetyPositionsWithinCircle( MIL_AgentPion& pion, float radius, float safetyDistance );
    static std::vector< boost::shared_ptr< MT_Vector2D > > GetRoadIntersectionsWithZone( const TER_Localisation* zone );
    static bool IsLinearRiverInBetween( const MT_Vector2D* from, const MT_Vector2D* to );
    static bool IsWaterInBetween( const MT_Vector2D* from, const MT_Vector2D* to );
    static std::pair< unsigned int, unsigned int > GetTerrainData( const MT_Vector2D* location );

    // Movement
    static bool CanMoveOn( const DEC_Decision_ABC* agent, boost::shared_ptr< MT_Vector2D > position );
    //@}
};

#endif // __DEC_TerrainFunctions_h_
