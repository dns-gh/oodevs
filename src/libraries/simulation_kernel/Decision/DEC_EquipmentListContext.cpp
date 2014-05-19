// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_EquipmentListContext.h"
#include "DEC_Agent_PathClass.h"
#include "DEC_PathType.h"
#include "DEC_Path_KnowledgeAgent.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include <boost/range/numeric.hpp>

DEC_EquipmentListContext::DEC_EquipmentListContext( const std::vector< const PHY_ComposanteTypePion* >& equipments )
    : speeds_( equipments )
    , maxSlope_( std::numeric_limits< double >::max() )
    , slopeDeceleration_( 0 )
    , maxWeight_( 0 )
{
    for( auto it = equipments.begin(); it != equipments.end(); ++it )
    {
        maxSlope_ = std::min( maxSlope_, (*it)->GetMaxSlope() );
        slopeDeceleration_ = std::max( slopeDeceleration_, (*it)->GetSlopeDeceleration() );
        maxWeight_ = std::max( maxWeight_, (*it)->GetWeight() );
    }
}

DEC_EquipmentListContext::~DEC_EquipmentListContext()
{
    // NOTHING
}

namespace
{
    const MIL_Fuseau fuseau;
}

const MIL_Fuseau& DEC_EquipmentListContext::GetFuseau() const
{
    return fuseau;
}

const MIL_Fuseau& DEC_EquipmentListContext::GetAutomataFuseau() const
{
    return fuseau;
}

const MT_Vector2D& DEC_EquipmentListContext::GetDirDanger() const
{
    static const MT_Vector2D v;
    return v;
}

const PHY_Speeds& DEC_EquipmentListContext::GetUnitSpeeds() const
{
    return speeds_;
}

const DEC_Agent_PathClass& DEC_EquipmentListContext::GetPathClass() const
{
    static const DEC_Agent_PathClass& class_ =
        DEC_Agent_PathClass::GetPathClass( DEC_PathType::movement_, false, false );
    return class_;
}

double DEC_EquipmentListContext::GetUnitMaxSlope() const
{
    return maxSlope_;
}

double DEC_EquipmentListContext::GetUnitSlopeDeceleration() const
{
    return slopeDeceleration_;
}

double DEC_EquipmentListContext::GetUnitMajorWeight() const
{
    return maxWeight_;
}

double DEC_EquipmentListContext::GetCostOutsideOfAllObjects() const
{
    return 0;
}

const DEC_EquipmentListContext::T_PathKnowledgeAgentVector& DEC_EquipmentListContext::GetPathKnowledgeAgents() const
{
    static const T_PathKnowledgeAgentVector k;
    return k;
}

const DEC_EquipmentListContext::T_PathKnowledgeObjectByTypesVector& DEC_EquipmentListContext::GetPathKnowledgeObjects() const
{
    static const T_PathKnowledgeObjectByTypesVector k;
    return k;
}

const DEC_EquipmentListContext::T_PathKnowledgePopulationVector& DEC_EquipmentListContext::GetPathKnowledgePopulations() const
{
    static const T_PathKnowledgePopulationVector k;
    return k;
}
