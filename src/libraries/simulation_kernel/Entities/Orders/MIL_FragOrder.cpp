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

#include "ActionManager.h"
#include "MIL_FragOrderType.h"
#include "MIL_MissionParameterFactory.h"
#include "MIL_EnumerationParameter.h"
#include "MIL_OrderContext.h"
#include "MIL_Report.h"
#include "Decision/DEC_Representations.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Brain.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Populations/MIL_Population.h"
#include "MIL_MissionParameter_ABC.h"
#include "MIL_Time_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "Tools/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_FragOrder::MIL_FragOrder( const MIL_FragOrderType& type, uint32_t id )
    : type_( type )
    , id_  ( id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_FragOrder::MIL_FragOrder( const MIL_FragOrderType& type,
                              const MIL_FragOrder& rhs,
                              uint32_t id )
    : type_      ( type )
    , id_        ( id )
    , parameters_( rhs.parameters_ )
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
// Name: MIL_FragOrder::SetParameters
// Created: BAX 2013-08-29
// -----------------------------------------------------------------------------
void MIL_FragOrder::SetParameters( const DEC_KnowledgeResolver_ABC& resolver,
                                   const sword::MissionParameters& parameters )
{
    if( static_cast< int >( type_.GetParameters().size() ) != parameters.elem_size() )
    {
        MT_LOG_ERROR_MSG( "Frag Order " << type_.GetName() << " invalid parameters" );
        throw MASA_EXCEPTION_ASN( sword::OrderAck::ErrorCode, sword::OrderAck::error_invalid_parameter );
    }
    MIL_MissionParameterFactory::Copy( type_, parameters, parameters_, resolver );
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
void MIL_FragOrder::Register( sword::Brain& brain )
{
    brain.RegisterFunction( "CreateFragOrder", &MIL_FragOrder::CreateFragOrder );
    brain.RegisterMethod( "GetType", &MIL_FragOrder::GetDIAType );
    brain.RegisterMethod( "GetambianceMission_", &MIL_FragOrder::GetAmbianceMission );
    brain.RegisterMethod( "Getmunition_", &MIL_FragOrder::GetMunition );
    brain.RegisterMethod( "Getmunitions_", &MIL_FragOrder::GetMunitions );
    brain.RegisterMethod( "GetnbIT_", &MIL_FragOrder::GetNbIT );
    brain.RegisterMethod( "GetnbrAmbulances_", &MIL_FragOrder::GetNbrAmbulances );
    brain.RegisterMethod( "GetnbrRemorqueurs_", &MIL_FragOrder::GetNbrRemorqueurs );
    brain.RegisterMethod( "GetorderConduiteAutomateActiverObstacle_", &MIL_FragOrder::GetOrderConduiteAutomateActiverObstacle );
    brain.RegisterMethod( "GetorderConduiteChangerAmbiance_", &MIL_FragOrder::GetOrderConduiteChangerAmbiance );
    brain.RegisterMethod( "SetorderConduiteChangerAmbiance_", &MIL_FragOrder::SetOrderConduiteChangerAmbiance );
    brain.RegisterMethod( "GetorderConduiteChangerPositionDebarquement_", &MIL_FragOrder::GetOrderConduiteChangerPositionDebarquement );
    brain.RegisterMethod( "GetorderConduiteChangerReglesEngagementPopulation_", &MIL_FragOrder::GetOrderConduiteChangerReglesEngagementPopulation );
    brain.RegisterMethod( "GetorderConduiteChangerReglesEngagement_", &MIL_FragOrder::GetOrderConduiteChangerReglesEngagement );
    brain.RegisterMethod( "GetorderConduiteModifierPrioritesBlesses_", &MIL_FragOrder::GetOrderConduiteModifierPrioritesBlesses );
    brain.RegisterMethod( "GetorderConduiteModifierPrioritesReparations_", &MIL_FragOrder::GetOrderConduiteModifierPrioritesReparations );
    brain.RegisterMethod( "GetorderConduiteModifierPrioritesTactiquesBlesses_", &MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesBlesses );
    brain.RegisterMethod( "GetorderConduiteModifierPrioritesTactiquesReparations_", &MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesReparations );
    brain.RegisterMethod( "GetEquipmentTypeListParameter", &MIL_FragOrder::GetEquipmentTypeListParameter );
    brain.RegisterMethod( "GetIntegerParameter", &MIL_FragOrder::GetIntegerParameter );
    brain.RegisterMethod( "GetAutomatListParameter", &MIL_FragOrder::GetAutomatListParameter );
    brain.RegisterMethod( "GetAgentParameter", &MIL_FragOrder::GetAgentParameter );
    brain.RegisterMethod( "GetLocationParameter", &MIL_FragOrder::GetLocationParameter );
    brain.RegisterMethod( "GetPointParameter", &MIL_FragOrder::GetPointParameter );
    brain.RegisterMethod( "GetMedicalPriorityParameter", &MIL_FragOrder::GetMedicalPriorityParameter );
    brain.RegisterMethod( "GetAgentKnowledgeParameter", &MIL_FragOrder::GetAgentKnowledgeParameter );
    brain.RegisterMethod( "GetResourceTypeParameter", &MIL_FragOrder::GetResourceTypeParameter );
    brain.RegisterMethod( "GetResourceTypeListParameter", &MIL_FragOrder::GetResourceTypeListParameter );
    brain.RegisterMethod( "GetObjectParameter", &MIL_FragOrder::GetObjectParameter );
    brain.RegisterMethod( "GetObjectListParameter", &MIL_FragOrder::GetObjectListParameter );
    brain.RegisterMethod( "GetorderConduiteModifierRegimeTravailMaintenance_", &MIL_FragOrder::GetOrderConduiteModifierRegimeTravailMaintenance );
    brain.RegisterMethod( "GetorderConduitePopulationChangerAttitude_", &MIL_FragOrder::GetOrderConduitePopulationChangerAttitude );
    brain.RegisterMethod( "GetpionARenforcer_", &MIL_FragOrder::GetPionARenforcer );
    brain.RegisterMethod( "GetpionRenforce_", &MIL_FragOrder::GetPionRenforce );
    brain.RegisterMethod( "GetpointCible_", &MIL_FragOrder::GetPointCible );
    brain.RegisterMethod( "GetzoneCible_", &MIL_FragOrder::GetZoneCible );
    brain.RegisterMethod( "GetporteeAction_", &MIL_FragOrder::GetPorteeAction );
    brain.RegisterMethod( "GetsiteFranchissementOriginal_", &MIL_FragOrder::GetSiteFranchissementOriginal );
    brain.RegisterMethod( "GetsiteFranchissementVariante_", &MIL_FragOrder::GetSiteFranchissementVariante );
    brain.RegisterMethod( "GetAgentKnowledge_", &MIL_FragOrder::GetAgentKnowledge );
    brain.RegisterMethod( "GetObjectKnowledge_", &MIL_FragOrder::GetObjectKnowledge );
    brain.RegisterMethod( "GetAgent_", &MIL_FragOrder::GetAgent );
    brain.RegisterMethod( "HasIntegerParameter", &MIL_FragOrder::HasIntegerParameter );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::CreateFragOrder
// Created: DDA 2011-01-17
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_FragOrder > MIL_FragOrder::CreateFragOrder( std::string typeText )
{
    const MIL_FragOrderType* type = MIL_FragOrderType::FindByDiaType( typeText );
    if( !type )
        return boost::shared_ptr< MIL_FragOrder >();
    // incomplete order
    return boost::make_shared< MIL_FragOrder >( *type, 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetAmbianceMission
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetAmbianceMission() const
{
    static const std::string parameterName( "ambianceMission_" );
    return GetIntegerParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetMunition
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
const PHY_DotationCategory* MIL_FragOrder::GetMunition() const
{
    static const std::string parameterName( "munitions_" );
    return GetResourceTypeParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetMunitions
// Created: MMC 2013-01-21
// -----------------------------------------------------------------------------
std::vector< const PHY_DotationCategory* > MIL_FragOrder::GetMunitions() const
{
    static const std::string parameterName( "munitions_" );
    return GetResourceTypeListParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetNbIT
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetNbIT() const
{
    static const std::string parameterName( "nbIT_" );
    return GetIntegerParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetNbrAmbulances
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetNbrAmbulances() const
{
    static const std::string parameterName( "nbrAmbulances_" );
    return GetIntegerParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetNbrRemorqueurs
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetNbrRemorqueurs() const
{
    static const std::string parameterName( "nbrRemorqueurs_" );
    return GetIntegerParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteAutomateActiverObstacle
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteAutomateActiverObstacle() const
{
    static const std::string parameterName( "orderConduiteAutomateActiverObstacle_" );
    return GetIntegerParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteChangerAmbiance
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteChangerAmbiance() const
{
    static const std::string parameterName( "orderConduiteChangerAmbiance_" );
    return GetIntegerParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteChangerAmbiance
// Created: LMT 2011-11-08
// -----------------------------------------------------------------------------
void MIL_FragOrder::SetOrderConduiteChangerAmbiance( int value )
{
    if( !parameters_.empty() )
        throw MASA_EXCEPTION( "Frag Order " + type_.GetName() + " : Can't set parameter already exists" );
    boost::shared_ptr< MIL_MissionParameter_ABC > parameter( new MIL_EnumerationParameter( value ) );
    parameters_.push_back( parameter );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteChangerPositionDebarquement
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > MIL_FragOrder::GetOrderConduiteChangerPositionDebarquement() const
{
    static const std::string parameterName( "orderConduiteChangerPositionDebarquement_" );
    return GetPointParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteChangerReglesEngagementPopulation
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteChangerReglesEngagementPopulation() const
{
    static const std::string parameterName( "orderConduiteChangerReglesEngagementPopulation_" );
    return GetIntegerParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteChangerReglesEngagement
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteChangerReglesEngagement() const
{
    static const std::string parameterName( "orderConduiteChangerReglesEngagement_" );
    return GetIntegerParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierPrioritesBlesses
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
MIL_FragOrder::T_MedicalPriorityVector MIL_FragOrder::GetOrderConduiteModifierPrioritesBlesses() const
{
    static const std::string parameterName( "orderConduiteModifierPrioritesBlesses_" );
    return GetMedicalPriorityParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierPrioritesReparations
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
MIL_FragOrder::T_MaintenancePriorityVector MIL_FragOrder::GetOrderConduiteModifierPrioritesReparations() const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == "orderConduiteModifierPrioritesReparations_" )
        {
            T_MaintenancePriorityVector result;
            if( parameters_[i]->ToMaintenancePriorities( result ) )
                return result;
            else
                return MIL_FragOrder::T_MaintenancePriorityVector();
        }
    }
    throw MASA_EXCEPTION( "Frag Order " + type_.GetName() + " : Unknown parameter orderConduiteModifierPrioritesReparations_" );

}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesBlesses
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesBlesses() const
{
    static const std::string parameterName( "orderConduiteModifierPrioritesTactiquesBlesses_" );
    return GetAutomatListParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetEquipmentTypeListParameter
// Created: NMI 2014-05-27
// -----------------------------------------------------------------------------
std::vector< const PHY_ComposanteTypePion* > MIL_FragOrder::GetEquipmentTypeListParameter( const std::string& name ) const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == name )
        {
            std::vector< const PHY_ComposanteTypePion* > result;
            if( parameters_[i]->ToEquipmentTypeList( result ) )
                return result;
            else
                return std::vector< const PHY_ComposanteTypePion* >();
        }
    }
    throw MASA_EXCEPTION( "Frag Order " + type_.GetName() + " : Unknown parameter: " + name );
}

bool MIL_FragOrder::HasIntegerParameter( const std::string& name ) const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == name )
        {
            int result;
            if( parameters_[i]->ToId( result ) )
                return true;
            float realResult;
            if( parameters_[i]->ToNumeric( realResult ) )
                return true;
            return false;
        }
    }
    return false;
}

int MIL_FragOrder::GetIntegerParameter( const std::string& name ) const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == name )
        {
            int result = 0;
            if( parameters_[i]->ToId( result ) )
                return result;
            float realResult;
            if( parameters_[i]->ToNumeric( realResult ) )
                return static_cast< int >( realResult );
        }
    }
    throw MASA_EXCEPTION( "Frag Order " + type_.GetName() + " : Unknown parameter: " + name );
}

std::vector< DEC_Decision_ABC* > MIL_FragOrder::GetAutomatListParameter( const std::string& name ) const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == name )
        {
            std::vector< DEC_Decision_ABC* > result;
            if( parameters_[i]->ToAutomatList( result ) )
                return result;
            else
                return std::vector< DEC_Decision_ABC* >();
        }
    }
    throw MASA_EXCEPTION( "Frag Order " + type_.GetName() + " : Unknown parameter: " + name );
}

const DEC_Decision_ABC* MIL_FragOrder::GetAgentParameter( const std::string& name ) const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == name )
        {
            const DEC_Decision_ABC* result = 0;
            if( parameters_[i]->ToAgent( result ) )
                return result;
            else
                return 0;
        }
    }
    throw MASA_EXCEPTION( "Frag Order " + type_.GetName() + " : Unknown parameter: " + name );
}

boost::shared_ptr< TER_Localisation > MIL_FragOrder::GetLocationParameter( const std::string& name ) const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == name )
        {
            boost::shared_ptr< TER_Localisation > result;
            if( parameters_[i]->ToPolygon( result ) )
                return result;
            else
                return boost::shared_ptr< TER_Localisation >();
        }
    }
    throw MASA_EXCEPTION( "Frag Order " + type_.GetName() + " : Unknown parameter: " + name );
}

boost::shared_ptr< MT_Vector2D > MIL_FragOrder::GetPointParameter( const std::string& name ) const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == name )
        {
            boost::shared_ptr< MT_Vector2D > result;
            if( parameters_[i]->ToPoint( result ) )
                return result;
            else
                return boost::shared_ptr< MT_Vector2D >();
        }
    }
    throw MASA_EXCEPTION( "Frag Order " + type_.GetName() + " : Unknown parameter: " + name );
}

MIL_FragOrder::T_MedicalPriorityVector MIL_FragOrder::GetMedicalPriorityParameter( const std::string& name ) const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == name )
        {
            T_MedicalPriorityVector result;
            if( parameters_[i]->ToMedicalPriorities( result ) )
                return result;
            else
                return MIL_FragOrder::T_MedicalPriorityVector();
        }
    }
    throw MASA_EXCEPTION( "Frag Order " + type_.GetName() + " : Unknown parameter: " + name );
}

boost::shared_ptr< DEC_Knowledge_Agent > MIL_FragOrder::GetAgentKnowledgeParameter( const std::string& name ) const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == name )
        {
            boost::shared_ptr< DEC_Knowledge_Agent > result;
            if( parameters_[i]->ToAgentKnowledge( result ) )
                return result;
            else
                return boost::shared_ptr< DEC_Knowledge_Agent >();
        }
    }
    throw MASA_EXCEPTION( "Frag Order " + type_.GetName() + " : Unknown parameter: " + name );
}

const PHY_DotationCategory* MIL_FragOrder::GetResourceTypeParameter( const std::string& name ) const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == name )
        {
            const PHY_DotationCategory* result;
            if( parameters_[i]->ToDotationType( result ) )
                return result;
            else
                return 0;
        }
    }
    throw MASA_EXCEPTION( "Frag Order " + type_.GetName() + " : Unknown parameter: " + name );
}

std::vector< const PHY_DotationCategory* > MIL_FragOrder::GetResourceTypeListParameter( const std::string& name ) const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == name )
        {
            std::vector< const PHY_DotationCategory* > result;
            if( parameters_[i]->ToDotationTypeList( result ) )
                return result;
            else
                return std::vector< const PHY_DotationCategory* >();
        }
    }
    throw MASA_EXCEPTION( "Frag Order " + type_.GetName() + " : Unknown parameter: " + name );
}

boost::shared_ptr< DEC_Knowledge_Object > MIL_FragOrder::GetObjectParameter( const std::string& name ) const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == name )
        {
            boost::shared_ptr< DEC_Knowledge_Object > result;
            if( parameters_[i]->ToObjectKnowledge( result ) )
                return result;
            else
                return boost::shared_ptr< DEC_Knowledge_Object >();
        }
    }
    throw MASA_EXCEPTION( "Frag Order " + type_.GetName() + " : Unknown parameter: " + name );
}

std::vector< boost::shared_ptr< DEC_Knowledge_Object > > MIL_FragOrder::GetObjectListParameter( const std::string& name ) const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        if( type_.GetParameterName( i ) == name )
        {
            std::vector< boost::shared_ptr< DEC_Knowledge_Object > > result;
            if( parameters_[i]->ToObjectKnowledgeList( result ) )
                return result;
            else
                return std::vector< boost::shared_ptr< DEC_Knowledge_Object > >();
        }
    }
    throw MASA_EXCEPTION( "Frag Order " + type_.GetName() + " : Unknown parameter: " + name );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesReparations
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > MIL_FragOrder::GetOrderConduiteModifierPrioritesTactiquesReparations() const
{
    static const std::string parameterName( "orderConduiteModifierPrioritesTactiquesReparations_" );
    return GetAutomatListParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduiteModifierRegimeTravailMaintenance
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduiteModifierRegimeTravailMaintenance() const
{
    static const std::string parameterName( "orderConduiteModifierRegimeTravailMaintenance_" );
    return GetIntegerParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetOrderConduitePopulationChangerAttitude
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetOrderConduitePopulationChangerAttitude() const
{
    static const std::string parameterName( "orderConduitePopulationChangerAttitude_" );
    return GetIntegerParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetPionARenforcer
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
const DEC_Decision_ABC* MIL_FragOrder::GetPionARenforcer() const
{
    static const std::string parameterName( "pionARenforcer_" );
    return GetAgentParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetPionRenforce
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
const DEC_Decision_ABC* MIL_FragOrder::GetPionRenforce() const
{
    static const std::string parameterName( "pionRenforce_" );
    return GetAgentParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetPointCible
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > MIL_FragOrder::GetPointCible() const
{
    static const std::string parameterName( "pointCible_" );
    return GetPointParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > MIL_FragOrder::GetZoneCible
// Created: JCR 2010-11-04
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > MIL_FragOrder::GetZoneCible() const
{
    static const std::string parameterName( "zoneCible_" );
    return GetLocationParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetPorteeAction
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetPorteeAction() const
{
    static const std::string parameterName( "porteeAction_" );
    return GetIntegerParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetSiteFranchissementOriginal
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetSiteFranchissementOriginal() const
{
    static const std::string parameterName( "siteFranchissementOriginal_" );
    return GetIntegerParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetSiteFranchissementVariante
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
int MIL_FragOrder::GetSiteFranchissementVariante() const
{
    static const std::string parameterName( "siteFranchissementVariante_" );
    return GetIntegerParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetAgentKnowledge
// Created: MGD 2010-02-16
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > MIL_FragOrder::GetAgentKnowledge() const
{
    static const std::string parameterName( "agentKnowledge_" );
    return GetAgentKnowledgeParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetObjectKnowledge
// Created: GGE 2011-11-17
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< DEC_Knowledge_Object > > MIL_FragOrder::GetObjectKnowledge() const
{
    static const std::string parameterName( "objectKnowledge_" );
    return GetObjectListParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::GetAgent
// Created: MGD 2010-02-27
// -----------------------------------------------------------------------------
const DEC_Decision_ABC* MIL_FragOrder::GetAgent() const
{
    static const std::string parameterName( "agent_" );
    return GetAgentParameter( parameterName );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::Send
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void MIL_FragOrder::Send( ActionManager& actions, MIL_AgentPion& pion ) const
{
    MIL_Report::PostEvent( pion, report::eRC_FragOrderReceived, type_.GetName() );
    client::FragOrder message;
    message().mutable_tasker()->mutable_unit()->set_id( pion.GetID() );
    Send( actions, message );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::Send
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void MIL_FragOrder::Send( ActionManager& actions, MIL_Automate& automat ) const
{
    MIL_Report::PostEvent( automat, report::eRC_FragOrderReceived, type_.GetName() );
    client::FragOrder message;
    message().mutable_tasker()->mutable_automat()->set_id( automat.GetID() );
    Send( actions, message );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::Send
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void MIL_FragOrder::Send( ActionManager& actions, MIL_Population& population ) const
{
    MIL_Report::PostEvent( population, report::eRC_FragOrderReceived, type_.GetName() );
    client::FragOrder message;
    message().mutable_tasker()->mutable_crowd()->set_id( population.GetID() );
    Send( actions, message );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::Send
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void MIL_FragOrder::Send( ActionManager& actions, client::FragOrder& message ) const
{
    message().mutable_type()->set_id( type_.GetID() );
    Serialize( *message().mutable_parameters() );
    NET_ASN_Tools::WriteGDH( MIL_Time_ABC::GetTime().GetRealTime(), *message().mutable_start_time() );
    message().set_name( type_.GetName() );
    message().set_id( id_ );
    message.Send( NET_Publisher_ABC::Publisher() );
    actions.Send( actions.Register( message() ), 0, "" );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::Serialize
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void MIL_FragOrder::Serialize( sword::MissionParameters& message ) const
{
    // $$$$ _RC_ PHC 2011-01-13: try to merge FragOrder with Mission_ABC
    if( !type_.Copy( parameters_, message, MIL_OrderContext() ) )
    {
        MT_LOG_ERROR_MSG( "Frag Order " + type_.GetName() + " impossible to serialize parameters" );
        throw MASA_EXCEPTION_ASN( sword::OrderAck::ErrorCode, sword::OrderAck::error_invalid_parameter );
    }
}

