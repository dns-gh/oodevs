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
#include "MIL_MissionParameterFactory.h"
#include "MIL_OrderContext.h"
#include "Decision/DEC_Representations.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Populations/MIL_Population.h"
#include "MIL_MissionParameter_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <directia/brain/Brain.h>

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_FragOrder::MIL_FragOrder( const MIL_FragOrderType& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const sword::FragOrder& asn )
    : type_( type )
{
    MIL_MissionParameterFactory::Copy( type, asn.parameters(), parameters_, knowledgeResolver );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_FragOrder::MIL_FragOrder( const MIL_FragOrderType& type, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, const MIL_FragOrder& rhs )
    : type_      ( type )
    , parameters_( rhs.parameters_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder constructor
// Created: DDA 2011-01-17
// -----------------------------------------------------------------------------
MIL_FragOrder::MIL_FragOrder( const MIL_FragOrderType& type )
    : type_      ( type )
    , parameters_()
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
void MIL_FragOrder::Register( directia::brain::Brain& brain )
{
    brain[ "CreateFragOrder" ] = &MIL_FragOrder::CreateFragOrder;
    brain.Register( "GetType", &MIL_FragOrder::GetDIAType );
    brain.Register( "GetambianceMission_", &MIL_FragOrder::GetAmbianceMission );
    brain.Register( "Getmunitions_", &MIL_FragOrder::GetMunitions );
    brain.Register( "GetnbIT_", &MIL_FragOrder::GetNbIT );
    brain.Register( "GetnbrAmbulances_", &MIL_FragOrder::GetNbrAmbulances );
    brain.Register( "GetnbrRemorqueurs_", &MIL_FragOrder::GetNbrRemorqueurs );
    brain.Register( "GetorderConduiteAutomateActiverObstacle_", &MIL_FragOrder::GetOrderConduiteAutomateActiverObstacle );
    brain.Register( "GetorderConduiteChangerAmbiance_", &MIL_FragOrder::GetOrderConduiteChangerAmbiance );
    brain.Register( "GetorderConduiteChangerPositionDebarquement_", &MIL_FragOrder::GetOrderConduiteChangerPositionDebarquement );
    brain.Register( "GetorderConduiteChangerReglesEngagementPopulation_", &MIL_FragOrder::GetOrderConduiteChangerReglesEngagementPopulation );
    brain.Register( "GetorderConduiteChangerReglesEngagement_", &MIL_FragOrder::GetOrderConduiteChangerReglesEngagement );
    brain.Register( "GetorderConduiteModifierPrioritesBlesses_", &MIL_FragOrder::GetOrderConduiteModifierPrioritesBlesses );
    brain.Register( "GetorderConduiteModifierPrioritesReparations_", &MIL_FragOrder::GetOrderConduiteModifierPrioritesReparations );
    brain.Register( "GetorderConduiteModifierPrioritesTactiquesBlesses_", &MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesBlesses );
    brain.Register( "GetorderConduiteModifierPrioritesTactiquesReparations_", &MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesReparations );
    brain.Register( "GetorderConduiteModifierRegimeTravailMaintenance_", &MIL_FragOrder::GetOrderConduiteModifierRegimeTravailMaintenance );
    brain.Register( "GetorderConduitePopulationChangerAttitude_", &MIL_FragOrder::GetOrderConduitePopulationChangerAttitude );
    brain.Register( "GetpionARenforcer_", &MIL_FragOrder::GetPionARenforcer );
    brain.Register( "GetpionRenforce_", &MIL_FragOrder::GetPionRenforce );
    brain.Register( "GetpointCible_", &MIL_FragOrder::GetPointCible );
    brain.Register( "GetzoneCible_", &MIL_FragOrder::GetZoneCible );
    brain.Register( "GetporteeAction_", &MIL_FragOrder::GetPorteeAction );
    brain.Register( "GetsiteFranchissementOriginal_", &MIL_FragOrder::GetSiteFranchissementOriginal );
    brain.Register( "GetsiteFranchissementVariante_", &MIL_FragOrder::GetSiteFranchissementVariante );
    brain.Register( "GetAgentKnowledge_", &MIL_FragOrder::GetAgentKnowledge );
    brain.Register( "GetAgent_", &MIL_FragOrder::GetAgent );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::CreateFragOrder
// Created: DDA 2011-01-17
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_FragOrder > MIL_FragOrder::CreateFragOrder( std::string type )
{
    const MIL_FragOrderType* fragOrderType = MIL_FragOrderType::FindByDiaType( type );
    if( fragOrderType )
        return boost::shared_ptr< MIL_FragOrder >( new MIL_FragOrder( *fragOrderType ) );
    else
        return boost::shared_ptr< MIL_FragOrder >();
}

namespace
{
    int GetIntegerParameter( const std::string& name, const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters, const MIL_FragOrderType& type )
    {
        unsigned int parametersNumber = static_cast< unsigned >( parameters.size() );
        for (unsigned int i = 0; i < parametersNumber; ++i )
        {
            if( type.GetParameterName( i ) == name )
            {
                int result = 0;
                if( parameters[i]->ToId( result ) )
                    return result;
                float realResult;
                if( parameters[i]->ToNumeric( realResult ) )
                    return static_cast< int >( realResult );
            }
        }
        throw std::runtime_error( std::string( "Unknown parameter: " ) + name );
    }

    std::vector< DEC_Decision_ABC* > GetAutomatListParameter( const std::string& name, const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters, const MIL_FragOrderType& type )
    {
        unsigned int parametersNumber = static_cast< unsigned >( parameters.size() );
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
        unsigned int parametersNumber = static_cast< unsigned >( parameters.size() );
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

    boost::shared_ptr< TER_Localisation > GetLocationParameter( const std::string& name, const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters, const MIL_FragOrderType& type )
    {
        unsigned int parametersNumber = static_cast< unsigned >( parameters.size() );
        for (unsigned int i = 0; i < parametersNumber; ++i )
        {
            if( type.GetParameterName( i ) == name )
            {
                boost::shared_ptr< TER_Localisation > result;
                if( parameters[i]->ToPolygon( result ) )
                    return result;
            }
        }
        throw std::runtime_error( std::string( "Unknown parameter: " ) + name );
    }

    boost::shared_ptr< MT_Vector2D > GetPointParameter( const std::string& name, const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters, const MIL_FragOrderType& type )
    {
        unsigned int parametersNumber = static_cast< unsigned >( parameters.size() );
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
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
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
MIL_FragOrder::T_MedicalPriorityVector MIL_FragOrder::GetOrderConduiteModifierPrioritesBlesses() const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
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
MIL_FragOrder::T_MaintenancePriorityVector MIL_FragOrder::GetOrderConduiteModifierPrioritesReparations() const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
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
// Name: boost::shared_ptr< MT_Vector2D > MIL_FragOrder::GetZoneCible
// Created: JCR 2010-11-04
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > MIL_FragOrder::GetZoneCible() const
{
    static const std::string parameterName( "zoneCible_" );
    return GetLocationParameter( parameterName, parameters_, type_ );
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

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetAgentKnowledge
// Created: MGD 2010-02-16
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > MIL_FragOrder::GetAgentKnowledge() const
{
    static const std::string parameterName( "pointCible_" );

    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for (unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == parameterName )
        {
            boost::shared_ptr< DEC_Knowledge_Agent > result;
            if( parameters_[i]->ToAgentKnowledge( result ) )
                return result;
        }
    }
    throw std::runtime_error( std::string( "Unknown parameter: " ) + parameterName );
}
// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetAgent
// Created: MGD 2010-02-27
// -----------------------------------------------------------------------------
DEC_Decision_ABC* MIL_FragOrder::GetAgent() const
{
  static const std::string parameterName( "agent_" );
  return GetAgentParameter( parameterName, parameters_, type_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::Send
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void MIL_FragOrder::Send( MIL_AgentPion& pion ) const
{
    client::FragOrder message;
    message().mutable_tasker()->mutable_unit()->set_id( pion.GetID() );
    Send( message );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::Send
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void MIL_FragOrder::Send( MIL_Automate& automat ) const
{
    client::FragOrder message;
    message().mutable_tasker()->mutable_automat()->set_id( automat.GetID() );
    Send( message );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::Send
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void MIL_FragOrder::Send( MIL_Population& population ) const
{
    client::FragOrder message;
    message().mutable_tasker()->mutable_crowd()->set_id( population.GetID() );
    Send( message );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::Send
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void MIL_FragOrder::Send( client::FragOrder& message ) const
{
    message().mutable_type()->set_id( type_.GetID() );
    Serialize( *message().mutable_parameters() );
    NET_ASN_Tools::WriteGDH( MIL_AgentServer::GetWorkspace().GetRealTime(), *message().mutable_start_time() );
    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::Serialize
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void MIL_FragOrder::Serialize( sword::MissionParameters& message ) const
{
    // $$$$ _RC_ PHC 2011-01-13: try to merge FragOrder with Mission_ABC
    if( !type_.Copy( parameters_, message, MIL_OrderContext() ) )
        throw std::runtime_error( std::string( "Frag Order " ) + type_.GetName() + " impossible to serialize parameters" );
}


