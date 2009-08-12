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
#include "MIL_MissionParameter_ABC.h"
#include "MIL_MissionParameterFactory.h"
#include "MIL_ParameterType_MedicalPriorities.h"
#include "MIL_ParameterType_MaintenancePriorities.h"
#include "Decision/DEC_Representations.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include <directia/Brain.h>

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_FragOrder::MIL_FragOrder( const MIL_FragOrderType& type, DEC_Representations& engine, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const ASN1T_MsgFragOrder& asn )
    : type_           ( type )
    , representations_( engine )
{
    MIL_MissionParameterFactory::Copy( asn.parametres, parameters_, knowledgeResolver );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_FragOrder::MIL_FragOrder( const MIL_FragOrderType& type, DEC_Representations& engine, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_FragOrder& rhs )
    : type_           ( type )
    , representations_( engine )
    , parameters_     ( rhs.parameters_ )
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

namespace
{
    int GetIntegerParameter( const std::string& name, const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters, const MIL_FragOrderType& type )
    {
        unsigned int parametersNumber = parameters.size();
        for (unsigned int i = 0; i < parametersNumber; ++i )
        {
            if( type.GetParameterName( i ) == name )
            {
                int result = 0;         
                if( parameters[i]->ToId( result ) )
                    return result;
            }
        }
        throw std::runtime_error( std::string( "Unknown parameter: " ) + name );
    }

    std::vector< DEC_Decision_ABC* > GetAutomatListParameter( const std::string& name, const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters, const MIL_FragOrderType& type )
    {
        unsigned int parametersNumber = parameters.size();
        for (unsigned int i = 0; i < parametersNumber; ++i )
        {
            if( type.GetParameterName( i ) == name )
            {
                std::vector< DEC_Decision_ABC* > result;         
                if( parameters[i]->ToAutomatList( result ) )
                    return result;
            }
        }
        throw std::runtime_error( std::string( "Unknown parameter: " ) + name );
    }

    DEC_Decision_ABC* GetAgentParameter( const std::string& name, const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters, const MIL_FragOrderType& type )
    {
        unsigned int parametersNumber = parameters.size();
        for (unsigned int i = 0; i < parametersNumber; ++i )
        {
            if( type.GetParameterName( i ) == name )
            {
                DEC_Decision_ABC* result = 0;
                if( parameters[i]->ToAgent( result ) )
                    return result;
            }
        }
        throw std::runtime_error( std::string( "Unknown parameter: " ) + name );
    }

    boost::shared_ptr< MT_Vector2D > GetPointParameter( const std::string& name, const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters, const MIL_FragOrderType& type )
    {
        unsigned int parametersNumber = parameters.size();
        for (unsigned int i = 0; i < parametersNumber; ++i )
        {
            if( type.GetParameterName( i ) == name )
            {
                boost::shared_ptr< MT_Vector2D > result;
                if( parameters[i]->ToPoint( result ) )
                    return result;
            }
        }
        throw std::runtime_error( std::string( "Unknown parameter: " ) + name );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetAmbianceMission
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetAmbianceMission() const
{
    static const std::string parameterName( "ambianceMission_" );
    return GetIntegerParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetMunitions
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
const PHY_DotationCategory* MIL_FragOrder::GetMunitions() const
{
    unsigned int parametersNumber = parameters_.size();
    for (unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == "munitions_" )
        {
            const PHY_DotationCategory* result;
            if( parameters_[i]->ToDotationType( result ) )
                return result;
        }
    }
    throw std::runtime_error( "Unknown parameter munitions_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetNbIT
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetNbIT() const
{
    static const std::string parameterName( "nbIT_" );
    return GetIntegerParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetNbrAmbulances
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetNbrAmbulances() const
{
    static const std::string parameterName( "nbrAmbulances_" );
    return GetIntegerParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetNbrRemorqueurs
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetNbrRemorqueurs() const
{
    static const std::string parameterName( "nbrRemorqueurs_" );
    return GetIntegerParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteAutomateActiverObstacle
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteAutomateActiverObstacle() const
{
    static const std::string parameterName( "orderConduiteAutomateActiverObstacle_" );
    return GetIntegerParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteChangerAmbiance
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteChangerAmbiance() const
{
    static const std::string parameterName( "orderConduiteChangerAmbiance_" );
    return GetIntegerParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteChangerPositionDebarquement
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > MIL_FragOrder::GetOrderConduiteChangerPositionDebarquement() const
{
    static const std::string parameterName( "orderConduiteChangerPositionDebarquement_" );
    return GetPointParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteChangerReglesEngagementPopulation
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteChangerReglesEngagementPopulation() const
{
    static const std::string parameterName( "orderConduiteChangerReglesEngagementPopulation_" );
    return GetIntegerParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteChangerReglesEngagement
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteChangerReglesEngagement() const
{
    static const std::string parameterName( "orderConduiteChangerReglesEngagement_" );
    return GetIntegerParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierPrioritesBlesses
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
T_MedicalPriorityVector MIL_FragOrder::GetOrderConduiteModifierPrioritesBlesses() const
{
    unsigned int parametersNumber = parameters_.size();
    for (unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == "orderConduiteModifierPrioritesBlesses_" )
        {
            T_MedicalPriorityVector result;
            if( parameters_[i]->ToMedicalPriorities( result ) )
                return result;
        }
    }
    throw std::runtime_error( "Unknown parameter orderConduiteModifierPrioritesBlesses_" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierPrioritesReparations
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
T_MaintenancePriorityVector MIL_FragOrder::GetOrderConduiteModifierPrioritesReparations() const
{
    unsigned int parametersNumber = parameters_.size();
    for (unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == "orderConduiteModifierPrioritesReparations_" )
        {
            T_MaintenancePriorityVector result;
            if( parameters_[i]->ToMaintenancePriorities( result ) )
                return result;
        }
    }
    throw std::runtime_error( "Unknown parameter orderConduiteModifierPrioritesReparations_" );

}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesBlesses
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesBlesses() const
{
    static const std::string parameterName( "orderConduiteModifierPrioritesTactiquesBlesses_" );
    return GetAutomatListParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesReparations
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesReparations() const
{
    static const std::string parameterName( "orderConduiteModifierPrioritesTactiquesReparations_" );
    return GetAutomatListParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierRegimeTravailMaintenance
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteModifierRegimeTravailMaintenance() const
{
    static const std::string parameterName( "orderConduiteModifierRegimeTravailMaintenance_" );
    return GetIntegerParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduitePopulationChangerAttitude
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduitePopulationChangerAttitude() const
{
    static const std::string parameterName( "orderConduitePopulationChangerAttitude_" );
    return GetIntegerParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetPionARenforcer
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
DEC_Decision_ABC* MIL_FragOrder::GetPionARenforcer() const
{
    static const std::string parameterName( "pionARenforcer_" );
    return GetAgentParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetPionRenforce
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
DEC_Decision_ABC* MIL_FragOrder::GetPionRenforce() const
{
    static const std::string parameterName( "pionRenforce_" );
    return GetAgentParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetPointCible
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > MIL_FragOrder::GetPointCible() const
{
    static const std::string parameterName( "pointCible_" );
    return GetPointParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetPorteeAction
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetPorteeAction() const
{
    static const std::string parameterName( "porteeAction_" );
    return GetIntegerParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetSiteFranchissementOriginal
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetSiteFranchissementOriginal() const
{
    static const std::string parameterName( "siteFranchissementOriginal_" );
    return GetIntegerParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetSiteFranchissementVariante
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetSiteFranchissementVariante() const
{
    static const std::string parameterName( "siteFranchissementVariante_" );
    return GetIntegerParameter( parameterName, parameters_, type_ );
}

