// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_FragOrder.h"
#include "MIL_FragOrderType.h"
#include "MIL_OrderContext.h"
#include "MIL_MissionParameterFactory.h"
#include "MIL_ParameterType_MedicalPriorities.h"
#include "MIL_ParameterType_MaintenancePriorities.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Representations.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include <directia/Brain.h>

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_FragOrder::MIL_FragOrder( const MIL_FragOrderType& type, DEC_Decision_ABC& decision, DEC_Representations& engine, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const ASN1T_MsgFragOrder& asn )
    : type_           ( type )
    , representations_( engine )
    , decision_       ( decision )
{
    MIL_MissionParameterFactory::Copy( asn.parametres, parameters_, knowledgeResolver );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_FragOrder::MIL_FragOrder( const MIL_FragOrderType& type, DEC_Decision_ABC& decision, DEC_Representations& engine, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_FragOrder& rhs )
    : type_           ( type )
    , representations_( engine )
    , parameters_     ( rhs.parameters_ )
    , decision_       ( decision )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder destructor
// Created: NLD 2006-11-25
// -----------------------------------------------------------------------------
MIL_FragOrder::~MIL_FragOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::Launch
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
void MIL_FragOrder::Launch()
{
    representations_.AddToOrdersCategory( this );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetDIAType
// Created: LDC 2009-07-09
// -----------------------------------------------------------------------------
std::string MIL_FragOrder::GetDIAType() const
{
    return type_.GetDIAType();
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::Register
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
void MIL_FragOrder::Register( directia::Brain& brain )
{
    brain.RegisterFunction( "GetType", &MIL_FragOrder::GetDIAType );
    brain.RegisterFunction( "GetambianceMission_", &MIL_FragOrder::GetAmbianceMission );
    brain.RegisterFunction( "Getmunitions_", &MIL_FragOrder::GetMunitions );
    brain.RegisterFunction( "GetnbIT_", &MIL_FragOrder::GetNbIT );
    brain.RegisterFunction( "GetnbrAmbulances_", &MIL_FragOrder::GetNbrAmbulances );
    brain.RegisterFunction( "GetnbrRemorqueurs_", &MIL_FragOrder::GetNbrRemorqueurs );
    brain.RegisterFunction( "GetorderConduiteAutomateActiverObstacle_", &MIL_FragOrder::GetOrderConduiteAutomateActiverObstacle );
    brain.RegisterFunction( "GetorderConduiteChangerAmbiance_", &MIL_FragOrder::GetOrderConduiteChangerAmbiance );
    brain.RegisterFunction( "GetorderConduiteChangerPositionDebarquement_", &MIL_FragOrder::GetOrderConduiteChangerPositionDebarquement );
    brain.RegisterFunction( "GetorderConduiteChangerReglesEngagementPopulation_", &MIL_FragOrder::GetOrderConduiteChangerReglesEngagementPopulation );
    brain.RegisterFunction( "GetorderConduiteChangerReglesEngagement_", &MIL_FragOrder::GetOrderConduiteChangerReglesEngagement );
    brain.RegisterFunction( "GetorderConduiteModifierPrioritesBlesses_", &MIL_FragOrder::GetOrderConduiteModifierPrioritesBlesses );
    brain.RegisterFunction( "GetorderConduiteModifierPrioritesReparations_", &MIL_FragOrder::GetOrderConduiteModifierPrioritesReparations );
    brain.RegisterFunction( "GetorderConduiteModifierPrioritesTactiquesBlesses_", &MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesBlesses );
    brain.RegisterFunction( "GetorderConduiteModifierPrioritesTactiquesReparations_", &MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesReparations );
    brain.RegisterFunction( "GetorderConduiteModifierRegimeTravailMaintenance_", &MIL_FragOrder::GetOrderConduiteModifierRegimeTravailMaintenance );
    brain.RegisterFunction( "GetorderConduitePopulationChangerAttitude_", &MIL_FragOrder::GetOrderConduitePopulationChangerAttitude );
    brain.RegisterFunction( "GetpionARenforcer_", &MIL_FragOrder::GetPionARenforcer );
    brain.RegisterFunction( "GetpionRenforce_", &MIL_FragOrder::GetPionRenforce );
    brain.RegisterFunction( "GetpointCible_", &MIL_FragOrder::GetPointCible );
    brain.RegisterFunction( "GetporteeAction_", &MIL_FragOrder::GetPorteeAction );
    brain.RegisterFunction( "GetsiteFranchissementOriginal_", &MIL_FragOrder::GetSiteFranchissementOriginal );
    brain.RegisterFunction( "GetsiteFranchissementVariante_", &MIL_FragOrder::GetSiteFranchissementVariante );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetAmbianceMission
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetAmbianceMission() const
{
    return decision_.GetVariable<int>( "ambianceMission_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetMunitions
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
boost::shared_ptr< PHY_DotationCategory > MIL_FragOrder::GetMunitions() const
{
    return decision_.GetVariable< boost::shared_ptr< PHY_DotationCategory > >( "munitions_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetNbIT
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetNbIT() const
{
    return decision_.GetVariable< int >( "nbIT_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetNbrAmbulances
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetNbrAmbulances() const
{
    return decision_.GetVariable< int >( "nbrAmbulances_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetNbrRemorqueurs
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetNbrRemorqueurs() const
{
    return decision_.GetVariable< int >( "nbrRemorqueurs_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteAutomateActiverObstacle
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteAutomateActiverObstacle() const
{
    return decision_.GetVariable< int >( "orderConduiteAutomateActiverObstacle_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteChangerAmbiance
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteChangerAmbiance() const
{
    return decision_.GetVariable< int >( "orderConduiteChangerAmbiance_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteChangerPositionDebarquement
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > MIL_FragOrder::GetOrderConduiteChangerPositionDebarquement() const
{
    return decision_.GetVariable< boost::shared_ptr< MT_Vector2D > >( "orderConduiteChangerPositionDebarquement_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteChangerReglesEngagementPopulation
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteChangerReglesEngagementPopulation() const
{
    return decision_.GetVariable< int >( "orderConduiteChangerReglesEngagementPopulation_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteChangerReglesEngagement
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteChangerReglesEngagement() const
{
    return decision_.GetVariable< int >( "orderConduiteChangerReglesEngagement_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierPrioritesBlesses
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
MIL_ParameterType_MedicalPriorities* MIL_FragOrder::GetOrderConduiteModifierPrioritesBlesses() const
{
    return decision_.GetVariable< MIL_ParameterType_MedicalPriorities* >( "orderConduiteModifierPrioritesBlesses_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierPrioritesReparations
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
MIL_ParameterType_MaintenancePriorities* MIL_FragOrder::GetOrderConduiteModifierPrioritesReparations() const
{
    return decision_.GetVariable< MIL_ParameterType_MaintenancePriorities* >( "orderConduiteModifierPrioritesReparations_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesBlesses
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesBlesses() const
{
    return decision_.GetVariable< std::vector< DEC_Decision_ABC* > >( "orderConduiteModifierPrioritesTactiquesBlesses_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesReparations
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesReparations() const
{
    return decision_.GetVariable< std::vector< DEC_Decision_ABC* > >( "orderConduiteModifierPrioritesTactiquesReparations_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierRegimeTravailMaintenance
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteModifierRegimeTravailMaintenance() const
{
    return decision_.GetVariable< int >( "orderConduiteModifierRegimeTravailMaintenance_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduitePopulationChangerAttitude
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduitePopulationChangerAttitude() const
{
    return decision_.GetVariable< int >( "orderConduitePopulationChangerAttitude_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetPionARenforcer
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
DEC_Decision_ABC* MIL_FragOrder::GetPionARenforcer() const
{
    return decision_.GetVariable< DEC_Decision_ABC* >( "pionARenforcer_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetPionRenforce
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
DEC_Decision_ABC* MIL_FragOrder::GetPionRenforce() const
{
    return decision_.GetVariable< DEC_Decision_ABC* >( "pionRenforce_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetPointCible
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > MIL_FragOrder::GetPointCible() const
{
    return decision_.GetVariable< boost::shared_ptr< MT_Vector2D > >( "pointCible_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetPorteeAction
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetPorteeAction() const
{
    return decision_.GetVariable< int >( "porteeAction_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetSiteFranchissementOriginal
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetSiteFranchissementOriginal() const
{
    return decision_.GetVariable< int >( "siteFranchissementOriginal_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetSiteFranchissementVariante
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetSiteFranchissementVariante() const
{
    return decision_.GetVariable< int >( "siteFranchissementVariante_" );
}

