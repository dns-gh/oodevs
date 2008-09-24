// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "Mission.h"
#include "MissionMapping.h"
#include "MissionParameterFactory.h"
#include "MissionParameterPhaseLine.h"
#include "MissionParameterPhaseLines.h"
#include "SerializationTools.h"
#include "dispatcher/Model.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Agent.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/OrderParameter.h"
#include <xeumeuleu/xml.h>
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Mission::Mission( xml::xistream& xis, const dispatcher::Model& model )
    : model_( model )
    , type_( ResolveMission( xis ) )
    , automatTaskee_( 0 )
    , agentTaskee_( 0 )
    , factory_( 0 )
{
    ReadTaskeeWho( xis );
    if( automatTaskee_ )
        factory_.reset( new MissionParameterFactory( *this, type_, *automatTaskee_ ) );
    else
        factory_.reset( new MissionParameterFactory( *this, type_, *agentTaskee_ ) );
    ReadParameters( xis );
}

// -----------------------------------------------------------------------------
// Name: Mission destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Mission::~Mission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission::ResolveMission
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
const kernel::MissionType& Mission::ResolveMission( xml::xistream& xis )
{
    std::string code;
    xis >> xml::start( NS( "What", "cbml" ) )
            >> xml::start( NS( "C_BML_WhatWhen", "cbml" ) )
                >> xml::start( NS( "WhatWhenInstance", "cbml" ) )
                    >> xml::start( NS( "ActionTask", "cbml" ) )
                        >> xml::content( NS( "ActivityCode", "jc3iedm" ), code )
                    >> xml::end()
                >> xml::end()
            >> xml::end()
        >> xml::end();
    return model_.GetMissionTypes().Get( GetMissionIdFromCode( model_.GetMissionTypes(), code ) );
}

namespace
{
    template< typename Entity >
    struct NameFinder
    {
        explicit NameFinder( const std::string& name ) : name_( name ), result_( 0 ) {}
        void operator()( const Entity& entity ) const
        {
            if( entity.name_ == name_ )
                result_ = &entity;
        }
        std::string name_;
        mutable const Entity* result_;
    };

    const dispatcher::Automat* FindAutomat( const dispatcher::Model& model, const std::string& name )
    {
        NameFinder< dispatcher::Automat > automatFinder( name );
        model.automats_.Apply( automatFinder );
        return automatFinder.result_;
    }

    const dispatcher::Agent* FindAgent( const dispatcher::Model& model, const std::string& name )
    {
        NameFinder< dispatcher::Agent > agentFinder( name );
        model.agents_.Apply( agentFinder );
        return agentFinder.result_;
    }
}

// -----------------------------------------------------------------------------
// Name: Mission::ReadTaskeeWho
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::ReadTaskeeWho( xml::xistream& xis )
{
    std::string name;
    xis >> xml::start( NS( "TaskeeWho", "cbml" ) )
            >> xml::start( NS( "C_BML_Who", "cbml" ) )
                >> xml::start( NS( "WhoRef", "cbml" ) )
                    >> xml::start( NS( "UnitRef", "cbml" ) )
                        >> xml::content( NS( "OID", "jc3iedm" ), name )
                    >> xml::end()
                >> xml::end()
            >> xml::end()
        >> xml::end();
    automatTaskee_ = FindAutomat( model_, name );
    if( automatTaskee_ )
        return;
    agentTaskee_ = FindAgent( model_, name );
    if( !agentTaskee_ )
        throw std::runtime_error( __FUNCTION__ ": Unable to resolve TaskeeWho" );
}

// -----------------------------------------------------------------------------
// Name: Mission::ReadParameters
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::ReadParameters( xml::xistream& xis )
{
    xis >> xml::start( NS( "TaskControlMeasures", "cbml" ) )
            >> xml::list( NS( "TaskControlMeasure", "cbml" ), *this, &Mission::ReadParameter )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Mission::ReadParameter
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::ReadParameter( xml::xistream& xis )
{
    std::auto_ptr< MissionParameter_ABC > parameter( factory_->CreateParameter( xis, model_ ) );
    AddParameter( *parameter );
    parameter.release();
}

// -----------------------------------------------------------------------------
// Name: Mission::AddParameter
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::AddParameter( MissionParameter_ABC& parameter )
{
    if( boost::algorithm::to_lower_copy( parameter.GetType().GetType() ) == "phaselinelist" )
    {
        MissionParameterPhaseLines* lines = 0;
        for( T_Parameters::iterator it = parameters_.begin(); it != parameters_.end(); ++it )
            if( &(*it)->GetType() == &parameter.GetType() )
            {
                lines = static_cast< MissionParameterPhaseLines* >( *it );
                break;
            }
        if( !lines )
        {
            lines = new MissionParameterPhaseLines( parameter.GetType() );
            parameters_.push_back( lines );
        }
        lines->AddLine( static_cast< MissionParameterPhaseLine& >( parameter ) );
    }
    else
        parameters_.push_back( &parameter );
}

// -----------------------------------------------------------------------------
// Name: Mission::Send
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::Send( dispatcher::SimulationPublisher_ABC& publisher ) const
{
    if( automatTaskee_ != 0 )
        SendAutomatMission( publisher );
    else
        SendAgentMission( publisher );
}

// -----------------------------------------------------------------------------
// Name: Mission::SendAutomatMission
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void Mission::SendAutomatMission( dispatcher::SimulationPublisher_ABC& publisher ) const
{
    simulation::AutomatOrder asn;
    asn().oid = automatTaskee_->GetId();
    asn().mission = type_.GetId();
    asn().formation = EnumAutomatOrderFormation::deux_echelons;
    asn().parametres.n = type_.Count();
    if( asn().parametres.n > 0 )
        asn().parametres.elem = new ASN1T_MissionParameter[ asn().parametres.n ];
    Serialize( asn().parametres );
    asn.Send( publisher );
    Clean( asn().parametres );
}

// -----------------------------------------------------------------------------
// Name: Mission::SendAgentMission
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void Mission::SendAgentMission( dispatcher::SimulationPublisher_ABC& publisher ) const
{
    simulation::UnitOrder asn;
    asn().oid = agentTaskee_->GetId();
    asn().mission = type_.GetId();
    asn().parametres.n = type_.Count();
    if( asn().parametres.n > 0 )
        asn().parametres.elem = new ASN1T_MissionParameter[ asn().parametres.n ];
    Serialize( asn().parametres );
    asn.Send( publisher );
    Clean( asn().parametres );
}

namespace
{
    int GetParameterIndex( const kernel::MissionType& mission, const kernel::OrderParameter& param )
    {
        kernel::Iterator< const kernel::OrderParameter& > it( mission.CreateIterator() );
        for( unsigned int i = 0; it.HasMoreElements(); ++i )
        {
            const kernel::OrderParameter& parameter = it.NextElement();
            if( &parameter == &param )
                return i;
        }
        return -1;
    }

    void MarkParameters( ASN1T_MissionParameters& asn )
    {
        for( unsigned i = 0; i < asn.n; ++i )
        {
            asn.elem[i].null_value = 1;
            asn.elem[i].value.t = 0;
            asn.elem[i].value.u.aBool = 0;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Mission::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::Serialize( ASN1T_MissionParameters& asn ) const
{
    MarkParameters( asn );

    SerializeDummyParameters( asn );
    for( T_Parameters::const_iterator it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        const unsigned int index = GetParameterIndex( type_, (*it)->GetType() );
        if( index >= 0 )
            (*it)->Serialize( asn.elem[index] );
    }

    FillEmptyParameters( asn );
}

// -----------------------------------------------------------------------------
// Name: Mission::FillEmptyParameters
// Created: AGE 2008-05-28
// -----------------------------------------------------------------------------
void Mission::FillEmptyParameters( ASN1T_MissionParameters& asn ) const
{
    kernel::Iterator< const kernel::OrderParameter& > it( type_.CreateIterator() );
    for( unsigned int i = 0; it.HasMoreElements(); ++i )
    {
        const kernel::OrderParameter& parameter = it.NextElement();
        ASN1T_MissionParameter& asnParam = asn.elem[i];
        if( asnParam.value.t == 0 )
        {
            const std::string type = boost::algorithm::to_lower_copy( parameter.GetType() );
            if( type == "phaselinelist" )
            {
                static ASN1T_LimasOrder limas;
                limas.elem = 0; limas.n = 0;

                asnParam.null_value = 0;
                asnParam.value.t = T_MissionParameter_value_limasOrder;
                asnParam.value.u.limasOrder = &limas;
            }
            else
                throw std::runtime_error( "Missing parameter: " + type + " for mission " + type_.GetName() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Mission::SerializeDummyParameters
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::SerializeDummyParameters( ASN1T_MissionParameters& asn ) const
{
    kernel::Iterator< const kernel::OrderParameter& > it( type_.CreateIterator() );
    for( unsigned int i = 0; it.HasMoreElements(); ++i )
    {
        const kernel::OrderParameter& parameter = it.NextElement();
        ASN1T_MissionParameter& asnParam = asn.elem[i];
        asnParam.null_value = parameter.IsOptional() ? 1 : 0;
        const std::string type = boost::algorithm::to_lower_copy( parameter.GetType() );
        if( type == "bool" )
        {
            asnParam.null_value = 0; // $$$$ SBO 2008-05-26: FIXME: simulation bug
            asnParam.value.t = T_MissionParameter_value_aBool;
            asnParam.value.u.aBool = false;
        }
        else if( type == "intelligencelist" )
        {
            asnParam.value.t = T_MissionParameter_value_intelligenceList;
            asnParam.value.u.intelligenceList = new ASN1T_IntelligenceList();
            asnParam.value.u.intelligenceList->n = 0;
            asnParam.value.u.intelligenceList->elem = 0;
        }
        else if( type == "objectivelist" )
        {
            asnParam.value.t = T_MissionParameter_value_missionObjectiveList;
            asnParam.value.u.missionObjectiveList = new ASN1T_MissionObjectiveList();
            asnParam.value.u.missionObjectiveList->n = 0;
            asnParam.value.u.missionObjectiveList->elem = 0;
        }
        else if( type == "genobjectlist" )
        {
            asnParam.value.t = T_MissionParameter_value_plannedWorkList;
            asnParam.value.u.plannedWorkList = new ASN1T_PlannedWorkList();
            asnParam.value.u.plannedWorkList->n = 0;
            asnParam.value.u.plannedWorkList->elem = 0;
        }
        else if( type == "direction" )
        {
            asnParam.value.t = T_MissionParameter_value_heading;
            asnParam.value.u.heading = 0;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Mission::Clean
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::Clean( ASN1T_MissionParameters& asn ) const
{
    for( T_Parameters::const_iterator it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        const unsigned int index = GetParameterIndex( type_, (*it)->GetType() );
        if( index >= 0 )
            (*it)->Clean( asn.elem[index] );
    }
    CleanDummyParameters( asn );
    delete[] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: Mission::CleanDummyParameters
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::CleanDummyParameters( ASN1T_MissionParameters& asn ) const
{
    for( unsigned int i = 0; i < asn.n; ++i )
        switch( asn.elem[i].value.t )
        {
        case T_MissionParameter_value_intelligenceList:
            delete asn.elem[i].value.u.intelligenceList; break;
        case T_MissionParameter_value_missionObjectiveList:
            delete asn.elem[i].value.u.missionObjectiveList; break;
        case T_MissionParameter_value_plannedWorkList:
            delete asn.elem[i].value.u.plannedWorkList; break;
        }
}

// -----------------------------------------------------------------------------
// Name: Mission::IsSet
// Created: SBO 2008-05-23
// -----------------------------------------------------------------------------
bool Mission::IsSet( const kernel::OrderParameter& parameter ) const
{
    for( T_Parameters::const_iterator it = parameters_.begin(); it != parameters_.end(); ++it )
        if( &(*it)->GetType() == &parameter )
            return boost::algorithm::to_lower_copy( parameter.GetType() ) != "phaselinelist";
    return false;
}
