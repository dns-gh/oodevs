// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DEC_EquipmentListContext_h_
#define __DEC_EquipmentListContext_h_

#include "DEC_AgentContext_ABC.h"
#include "Entities/Agents/Units/PHY_Speeds.h"

class PHY_ComposanteTypePion;

// =============================================================================
/** @class  DEC_EquipmentListContext
    @brief  Provides path related context for the pathfinder
*/
// Created: MCO 2014-05-19
// =============================================================================
class DEC_EquipmentListContext : public DEC_AgentContext_ABC
{
public:
    explicit DEC_EquipmentListContext( const std::vector< const PHY_ComposanteTypePion* >& equipments );
    virtual ~DEC_EquipmentListContext();

    virtual const MIL_Fuseau& GetFuseau() const;
    virtual const MIL_Fuseau& GetAutomataFuseau() const;
    virtual const MT_Vector2D& GetDirDanger() const;
    virtual const PHY_Speeds& GetUnitSpeeds() const;
    virtual const DEC_Agent_PathClass& GetPathClass() const;

    virtual double GetUnitMaxSlope() const;
    virtual double GetUnitSlopeDeceleration() const;
    virtual double GetUnitMajorWeight() const;
    virtual double GetCostOutsideOfAllObjects() const;

    virtual const T_PathKnowledgeAgentVector& GetPathKnowledgeAgents() const;
    virtual const T_PathKnowledgeObjectByTypesVector& GetPathKnowledgeObjects() const;
    virtual const T_PathKnowledgePopulationVector& GetPathKnowledgePopulations() const;

private:
    const PHY_Speeds speeds_;
    double maxSlope_;
    double slopeDeceleration_;
    double maxWeight_;
};

#endif // __DEC_EquipmentListContext_h_
