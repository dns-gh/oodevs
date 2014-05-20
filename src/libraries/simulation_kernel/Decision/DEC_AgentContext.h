// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DEC_AgentContext_h_
#define __DEC_AgentContext_h_

#include "DEC_AgentContext_ABC.h"
#include "Entities/Agents/Units/PHY_Speeds.h"
#include "Entities/Orders/MIL_Fuseau.h"

class MIL_Agent_ABC;

// =============================================================================
/** @class  DEC_AgentContext
    @brief  Provides path related context for the pathfinder
*/
// Created: MCO 2014-05-14
// =============================================================================
class DEC_AgentContext : public DEC_AgentContext_ABC
{
public:
    DEC_AgentContext( const MIL_Agent_ABC& agent, const DEC_Agent_PathClass& pathClass, const T_PointVector& points );

    virtual const MIL_Fuseau& GetFuseau() const;
    virtual const MIL_Fuseau& GetAutomataFuseau() const;
    virtual const PHY_Speeds& GetUnitSpeeds() const;
    virtual const MT_Vector2D& GetDirDanger() const;

    virtual const DEC_Agent_PathClass& GetPathClass() const;

    virtual double GetUnitMaxSlope() const;
    virtual double GetUnitSlopeDeceleration() const;
    virtual double GetUnitMajorWeight() const;
    virtual double GetCostOutsideOfAllObjects() const;

    virtual const T_PathKnowledgeAgentVector& GetPathKnowledgeAgents() const;
    virtual const T_PathKnowledgeObjectByTypesVector& GetPathKnowledgeObjects() const;
    virtual const T_PathKnowledgePopulationVector& GetPathKnowledgePopulations() const;

private:
    void Initialize( const MIL_Agent_ABC& agent, const T_PointVector& points );

private:
    const MIL_Fuseau fuseau_;
    const MIL_Fuseau automataFuseau_;
    const MT_Vector2D dangerDirection_;
    const double maxSlope_;
    const double slopeDeceleration_;
    const double majorWeight_;
    double costOutsideOfAllObjects_;
    const DEC_Agent_PathClass& class_;
    const PHY_Speeds speeds_;
    T_PathKnowledgeAgentVector pathKnowledgeAgents_;
    T_PathKnowledgeObjectByTypesVector pathKnowledgeObjects_;
    T_PathKnowledgePopulationVector pathKnowledgePopulations_;
};

#endif // __DEC_AgentContext_h_
