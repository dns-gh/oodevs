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
class DEC_Path;
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
             DEC_Agent_PathfinderRule( const PHY_Speeds& speeds, const MT_Vector2D& from, const MT_Vector2D& to, bool bShort );
    virtual ~DEC_Agent_PathfinderRule();
    //@}

    //! @name Operations
    //@{
    virtual float EvaluateCost( const geometry::Point2f& from, const geometry::Point2f& to );
    virtual float GetCost     ( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween );
    //@}

    //! @name Costs Configuration
    //@{
    void AddEnemyKnowledge ( const DEC_Path_KnowledgeAgent & enemy, MT_Float rCostOnTarget, MT_Float rCostAtSecurityRange );
    void AddObjectKnowledge( const DEC_Path_KnowledgeObject& object, MT_Float rCost );

    void SetMaximumEnemyCost  ( MT_Float rNewCost );
    void SetDangerDirection   ( const MT_Vector2D& dangerDirection, MT_Float rBaseCost, MT_Float rLinearCost );
    void SetPreferedTerrain   ( const TerrainData& data, MT_Float rCostOut );
    void SetAvoidedTerrain    ( const TerrainData& data, MT_Float rCostIn );
    void SetAltitudePreference( MT_Float rMaxSlope, MT_Float rCostPerMeter ); 
    void SetAutomataFuseau    ( const MIL_Fuseau& fuseau, MT_Float rMaxDistanceOut, MT_Float rCostPerMeterOut );
    void SetFuseau            ( const MIL_Fuseau& fuseau, MT_Float rMaxDistanceOut, MT_Float rCostPerMeterOut, MT_Float rComfortDistanceIn, MT_Float rCostPerMeterIn );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DEC_Agent_PathfinderRule( const DEC_Agent_PathfinderRule& );            //!< Copy constructor
    DEC_Agent_PathfinderRule& operator=( const DEC_Agent_PathfinderRule& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< DEC_Path_KnowledgeAgent >       T_PathKnowledgeAgentVector;
    typedef T_PathKnowledgeAgentVector::const_iterator CIT_PathKnowledgeAgentVector;

    typedef std::vector< DEC_Path_KnowledgeObject >       T_PathKnowledgeObjectVector;
    typedef T_PathKnowledgeObjectVector::const_iterator CIT_PathKnowledgeObjectVector;
    //@}

    //! @name Helpers
    //@{
    MT_Float GetCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType );
    MT_Float TerrainCost( const TerrainData& data ) const;
    //@}

private:
    //! @name Member data
    //@{
    const TER_World&         world_;
    const PHY_RawVisionData& altitudeData_;

    const PHY_Speeds& speeds_;
    const MIL_Fuseau* pFuseau_;
    const MIL_Fuseau* pAutomateFuseau_;
    MT_Vector2D dangerDirection_;
    MT_Float    rDangerDirectionBaseCost_;
    MT_Float    rDangerDirectionLinearCost_;
    MT_Vector2D dangerPoint_;
    float    rMaxSpeed_;
    bool     bShort_;
    MT_Float rMaximumFuseauDistance_;
    MT_Float rComfortFuseauDistance_;
    MT_Float rFuseauCostPerMeterOut_;
    MT_Float rFuseauCostPerMeterIn_;
    MT_Float rMaximumAutomataFuseauDistance_;
    MT_Float rAutomataFuseauCostPerMeterOut_;

    T_PathKnowledgeAgentVector  agents_;
    MT_Float rMaxEnemyCost_;
    T_PathKnowledgeObjectVector objects_;

    TerrainData avoid_;
    MT_Float    rAvoidCost_;
    TerrainData prefer_;
    MT_Float    rPreferCost_;

    MT_Float rMinAltitude_;
    MT_Float rMaxAltitude_;
    MT_Float rAltitudeCostPerMeter_;
    MT_Float rMaxSlope_;
    //@}
};

#endif // __DEC_Agent_PathfinderRule_h_
