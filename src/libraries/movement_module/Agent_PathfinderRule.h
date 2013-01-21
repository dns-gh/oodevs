// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef sword_movement_Agent_PathfinderRule_h_
#define sword_movement_Agent_PathfinderRule_h_

#include <pathfind/TerrainRule_ABC.h>
#include "MT_Tools/MT_Vector2D.h"
#include "wrapper/View.h"

namespace sword
{
namespace movement
{
    class Agent_Path;

// =============================================================================
/** @class  Agent_PathfinderRule
    @brief  Agent_PathfinderRule
*/
// Created: AGE 2005-03-08
// =============================================================================
class Agent_PathfinderRule : public TerrainRule_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent_PathfinderRule( const wrapper::View& entity, Agent_Path& path, const MT_Vector2D& from, const MT_Vector2D& to );
    virtual ~Agent_PathfinderRule();
    //@}

    //! @name Operations
    //@{
    virtual float EvaluateCost( const geometry::Point2f& from, const geometry::Point2f& to );
    virtual float GetCost( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Agent_PathfinderRule( const Agent_PathfinderRule& );            //!< Copy constructor
    Agent_PathfinderRule& operator=( const Agent_PathfinderRule& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    double GetCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType );
    double GetTerrainCost( const TerrainData& data ) const;
    double GetDangerDirectionCost( const MT_Vector2D& to ) const;
    //@}

private:
    const wrapper::View entity_;
    Agent_Path& path_;
    const void* handler_;
    // Misc
    const float rMaxSpeed_;
    const bool bShort_;
    // Terrain
    const TerrainData& avoidedTerrain_;
    const double rAvoidedTerrainCost_;
    const TerrainData& preferedTerrain_;
    const double rPreferedTerrainCost_;
    // Altitude
    const double rAltitudeCostPerMeter_;
    // Fuseau
    const double rComfortFuseauDistance_;
    const double rFuseauCostPerMeterOut_;
    const double rFuseauCostPerMeterIn_;
    // Fuseau automate
    double rAutomataFuseauCostPerMeterOut_;
    // Danger direction
    const MT_Vector2D dangerDirection_;
    const double rDangerDirectionBaseCost_;
    const double rDangerDirectionLinearCost_;
    const MT_Vector2D dangerPoint_;
    // Enemies
    const double rEnemyMaximumCost_;
    // Populations
    const double rPopulationMaximumCost_;
};

}
}

#endif // sword_movement_Agent_PathfinderRule_h_
