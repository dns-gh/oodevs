// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Agent_PathfinderRule_h_
#define __DEC_Agent_PathfinderRule_h_

#include <pathfind/TerrainRule_ABC.h>
#include "MT_Tools/MT_Vector2D.h"

class MIL_Fuseau;
class DEC_Agent_Path;
class PHY_RawVisionData;
class TER_World;

// =============================================================================
/** @class  DEC_Agent_PathfinderRule
    @brief  DEC_Agent_PathfinderRule
*/
// Created: AGE 2005-03-08
// =============================================================================
class DEC_Agent_PathfinderRule : public TerrainRule_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Agent_PathfinderRule( const DEC_Agent_Path& path, const MT_Vector2D& from, const MT_Vector2D& to );
    virtual ~DEC_Agent_PathfinderRule();
    //@}

    //! @name Operations
    //@{
    virtual float EvaluateCost( const geometry::Point2f& from, const geometry::Point2f& to );
    virtual float GetCost( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween, std::ostream* reason );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DEC_Agent_PathfinderRule( const DEC_Agent_PathfinderRule& );            //!< Copy constructor
    DEC_Agent_PathfinderRule& operator=( const DEC_Agent_PathfinderRule& ); //!< Assignment operator
    //@}

    //! @name Initialization
    //@{
    void InitializeFuseauData( const MT_Vector2D& from, const MT_Vector2D& to );
    void InitializeAutomateFuseauData( const MT_Vector2D& from );
    //@}

    //! @name Helpers
    //@{
    double GetCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, std::ostream* reason );
    double GetTerrainCost( const TerrainData& data ) const;
    double GetDangerDirectionCost( const MT_Vector2D& to ) const;
    double GetAltitudeCost( double rAltitudeTo ) const;
    double GetFuseauxCost( const MT_Vector2D& from, const MT_Vector2D& to,
            std::ostream* reason ) const;
    double GetUrbanBlockCost( const MT_Vector2D& from, const MT_Vector2D& to ) const;
    double GetObjectsCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double& rSpeed ) const;
    double GetEnemiesCost( const MT_Vector2D& from, const MT_Vector2D& to ) const;
    double GetPopulationsCost( const MT_Vector2D& to ) const;
    //@}

private:
    const DEC_Agent_Path& path_;
    const TER_World& world_;
    const PHY_RawVisionData& altitudeData_;
    // Misc
    const float rMaxSpeed_;
    const bool bShort_;
    // Terrain
    const TerrainData& avoidedTerrain_;
    const double rAvoidedTerrainCost_;
    const TerrainData& preferedTerrain_;
    const double rPreferedTerrainCost_;
    // Altitude
    const double rMinAltitude_;
    const double rMaxAltitude_;
    const double rAltitudeCostPerMeter_;
    const double rMaxSlope_;
    const double rSlopeDeceleration_;
    // Fuseau
    const MIL_Fuseau* pFuseau_;
    double rMaximumFuseauDistance_;
    const double rComfortFuseauDistance_;
    const double rFuseauCostPerMeterOut_;
    const double rFuseauCostPerMeterIn_;
    // Fuseau automate
    const MIL_Fuseau*  pAutomateFuseau_;
    double rMaximumAutomataFuseauDistance_;
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

#endif // __DEC_Agent_PathfinderRule_h_
