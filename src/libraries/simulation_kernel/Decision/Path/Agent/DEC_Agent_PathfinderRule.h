// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-03-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Agent_PathfinderRule.h $
// $Author: Age $
// $Modtime: 24/06/05 11:42 $
// $Revision: 5 $
// $Workfile: DEC_Agent_PathfinderRule.h $
//
// *****************************************************************************

#ifndef __DEC_Agent_PathfinderRule_h_
#define __DEC_Agent_PathfinderRule_h_

#include "pathfind/TerrainRule_ABC.h"
#include "DEC_Path_KnowledgeAgent.h"
#include "DEC_Path_KnowledgeObject.h"

class MIL_Fuseau;
class PHY_Speeds;
class DEC_Agent_Path;
class PHY_RawVisionData;

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
    virtual float GetCost     ( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DEC_Agent_PathfinderRule( const DEC_Agent_PathfinderRule& );            //!< Copy constructor
    DEC_Agent_PathfinderRule& operator=( const DEC_Agent_PathfinderRule& ); //!< Assignement operator
    //@}

    //! @name Initialization
    //@{
    void InitializeFuseauData        ( const MT_Vector2D& from, const MT_Vector2D& to );
    void InitializeAutomateFuseauData( const MT_Vector2D& from );    
    //@}

    //! @name Helpers
    //@{
    MT_Float GetCost               ( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType );
    MT_Float GetTerrainCost        ( const TerrainData& data ) const;
    MT_Float GetDangerDirectionCost( const MT_Vector2D& to ) const;
    MT_Float GetAltitudeCost       ( MT_Float rAltitudeTo ) const;
    MT_Float GetFuseauxCost        ( const MT_Vector2D& from, const MT_Vector2D& to ) const;
    MT_Float GetUrbanBlockCost     ( const MT_Vector2D& from, const MT_Vector2D& to ) const;
    MT_Float GetObjectsCost        ( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    MT_Float GetEnemiesCost        ( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    MT_Float GetPopulationsCost    ( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    //@}

private:
    const DEC_Agent_Path&    path_;
    const TER_World&         world_;
    const PHY_RawVisionData& altitudeData_;

    // Misc
    const float rMaxSpeed_;
    const bool  bShort_;

    // Terrain
    const TerrainData& avoidedTerrain_;
    const MT_Float     rAvoidedTerrainCost_;
    const TerrainData& preferedTerrain_;
    const MT_Float     rPreferedTerrainCost_;

    // Altitude
    const MT_Float     rMinAltitude_;
    const MT_Float     rMaxAltitude_;
    const MT_Float     rAltitudeCostPerMeter_;
    const MT_Float     rMaxSlope_;

    // Fuseau
    const MIL_Fuseau*  pFuseau_;
          MT_Float     rMaximumFuseauDistance_;
    const MT_Float     rComfortFuseauDistance_;
    const MT_Float     rFuseauCostPerMeterOut_;
    const MT_Float     rFuseauCostPerMeterIn_;

    // Fuseau automate
    const MIL_Fuseau*  pAutomateFuseau_;
          MT_Float     rMaximumAutomataFuseauDistance_;
          MT_Float     rAutomataFuseauCostPerMeterOut_;
        
    // Danger direction
    const MT_Vector2D  dangerDirection_;
    const MT_Float     rDangerDirectionBaseCost_;
    const MT_Float     rDangerDirectionLinearCost_;
    const MT_Vector2D  dangerPoint_;

    // Enemies
    const MT_Float     rEnemyMaximumCost_;

    // Populations
    const MT_Float     rPopulationMaximumCost_;
};

#endif // __DEC_Agent_PathfinderRule_h_
