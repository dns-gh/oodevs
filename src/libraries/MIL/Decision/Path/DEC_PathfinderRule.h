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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathfinderRule.h $
// $Author: Age $
// $Modtime: 13/05/05 18:29 $
// $Revision: 4 $
// $Workfile: DEC_PathfinderRule.h $
//
// *****************************************************************************

#ifndef __DEC_PathfinderRule_h_
#define __DEC_PathfinderRule_h_

#include "pathfind/TerrainRule_ABC.h"
#include "DEC_Path_KnowledgeAgent.h"
#include "DEC_Path_KnowledgeObject.h"

class MIL_Fuseau;
class PHY_Speeds;
class DEC_Path;
class PHY_RawVisionData;

// =============================================================================
/** @class  DEC_PathfinderRule
    @brief  DEC_PathfinderRule
*/
// Created: AGE 2005-03-08
// =============================================================================
class DEC_PathfinderRule : public TerrainRule_ABC
{
public:
    //! @name Types
    //@{
    enum E_AltitudePreference
    {
        eDontCare,
        ePreferHigh,
        ePreferLow
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_PathfinderRule( const DEC_Path& path, const TerrainData& avoid, const TerrainData& prefer, const MT_Vector2D& from, const MT_Vector2D& to, bool bShort, E_AltitudePreference alt, MT_Float rMaxFuseauDistance );
    virtual ~DEC_PathfinderRule();
    //@}

    //! @name Operations
    //@{
    void AddEnemyKnowledge ( const DEC_Path_KnowledgeAgent & enemy );
    void AddObjectKnowledge( const DEC_Path_KnowledgeObject& object, MT_Float rCostIn, MT_Float rCostOut );
    void ChangeDangerDirectionCost( MT_Float rNewCost );

    virtual float EvaluateCost( const geometry::Point2f& from, const geometry::Point2f& to );
    virtual float GetCost     ( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DEC_PathfinderRule( const DEC_PathfinderRule& );            //!< Copy constructor
    DEC_PathfinderRule& operator=( const DEC_PathfinderRule& ); //!< Assignement operator
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
    MT_Float    rDangerDirectionCost_;
    MT_Vector2D dangerPoint_;
    float    rMaxSpeed_;
    bool     bShort_;
    MT_Float rFuseauDistance_;

    T_PathKnowledgeAgentVector  agents_;
    T_PathKnowledgeObjectVector objects_;

    TerrainData avoid_;
    TerrainData prefer_;
    bool bAvoid_;
    bool bPrefer_;

    MT_Float rMinAltitude_;
    MT_Float rMaxAltitude_;
    E_AltitudePreference altitudePreference_;
    MT_Float rMaxSlope_;
    //@}
};

#endif // __DEC_PathfinderRule_h_
