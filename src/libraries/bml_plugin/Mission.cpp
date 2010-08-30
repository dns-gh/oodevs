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
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Agent.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/StaticModel.h"
#include "protocol/simulationsenders.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Mission::Mission( xml::xistream& xis, const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel )
    : model_( model )
    , staticModel_( staticModel )
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
                    >> xml::end
                >> xml::end
            >> xml::end
        >> xml::end;
    const tools::Resolver< kernel::MissionType >& missions = staticModel_.types_;
    return missions.Get( GetMissionIdFromCode( missions, code ) );
}

namespace
{
    const dispatcher::Automat_ABC* FindAutomat( const dispatcher::Model_ABC& model, const std::string& name )
    {
        tools::Iterator< const dispatcher::Automat_ABC& > it( model.Automats().CreateIterator() );
        while( it.HasMoreElements() )
        {
            const dispatcher::Automat_ABC& element = it.NextElement();
            if( element.GetName().ascii() == name )
                return &element;
        }
        return 0;
    }

    const dispatcher::Agent_ABC* FindAgent( const dispatcher::Model_ABC& model, const std::string& name )
    {
        tools::Iterator< const dispatcher::Agent_ABC& > it( model.Agents().CreateIterator() );
        while( it.HasMoreElements() )
        {
            const dispatcher::Agent_ABC& element = it.NextElement();
            if( element.GetName().ascii() == name )
                return &element;
        }
        return 0;
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
                    >> xml::end
                >> xml::end
            >> xml::end
        >> xml::end;
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
        >> xml::end;
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
    asn().mutable_tasker()->set_id( automatTaskee_->GetId());
    asn().mutable_type()->set_id( type_.GetId());
    if( asn().parameters().elem_size() > 0 )
        for( int i = 0; i < asn().parameters().elem_size(); ++i )
            asn().mutable_parameters()->add_elem();
    Serialize( *asn().mutable_parameters() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Mission::SendAgentMission
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void Mission::SendAgentMission( dispatcher::SimulationPublisher_ABC& publisher ) const
{
    simulation::UnitOrder asn;
    asn().mutable_tasker()->set_id( agentTaskee_->GetId() );
    asn().mutable_type()->set_id( type_.GetId() );
    if( type_.Count() > 0 )
        for (unsigned int i = 0; i < type_.Count(); ++i)
            asn().mutable_parameters()->add_elem();
    Serialize( *asn().mutable_parameters() );
    asn.Send( publisher );
}

namespace
{
    int GetParameterIndex( const kernel::MissionType& mission, const kernel::OrderParameter& param )
    {
        tools::Iterator< const kernel::OrderParameter& > it( mission.CreateIterator() );
        for( unsigned int i = 0; it.HasMoreElements(); ++i )
        {
            const kernel::OrderParameter& parameter = it.NextElement();
            if( &parameter == &param )
                return i;
        }
        return -1;
    }

    void MarkParameters( Common::MsgMissionParameters& asn )
    {
        for( int i = 0; i < asn.elem_size(); ++i )
        {
            asn.mutable_elem(i)->set_null_value ( 1 );
            asn.mutable_elem(i)->mutable_value()->set_abool( 0 );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Mission::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Mission::Serialize( Common::MsgMissionParameters& asn ) const
{
    MarkParameters( asn );
    SerializeDummyParameters( asn );
    for( T_Parameters::const_iterator it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        const unsigned int index = GetParameterIndex( type_, (*it)->GetType() );
        if( index >= 0 )
            (*it)->Serialize( *asn.mutable_elem( index ) );
    }
    FillEmptyParameters( asn );
}

// -----------------------------------------------------------------------------
// Name: Mission::FillEmptyParameters
// Created: AGE 2008-05-28
// -----------------------------------------------------------------------------
void Mission::FillEmptyParameters( Common::MsgMissionParameters& asn ) const
{
    tools::Iterator< const kernel::OrderParameter& > it( type_.CreateIterator() );
    for( unsigned int i = 0; it.HasMoreElements(); ++i )
    {
        const kernel::OrderParameter& parameter = it.NextElement();
        Common::MsgMissionParameter& asnParam = *asn.mutable_elem(i);
        if( !asnParam.has_value() )
        {
            const std::string type = boost::algorithm::to_lower_copy( parameter.GetType() );
            if( type == "phaselinelist" )
            {
                static Common::MsgLimasOrder limas;
                asnParam.set_null_value( 0 );
                *asnParam.mutable_value()->mutable_limasorder() = limas ;
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
void Mission::SerializeDummyParameters( Common::MsgMissionParameters& asn ) const
{
    tools::Iterator< const kernel::OrderParameter& > it( type_.CreateIterator() );
    for( unsigned int i = 0; it.HasMoreElements(); ++i )
    {
        const kernel::OrderParameter& parameter = it.NextElement();
        Common::MsgMissionParameter& asnParam = *asn.mutable_elem(i);
        asnParam.set_null_value( parameter.IsOptional() ? 1 : 0 );
        const std::string type = boost::algorithm::to_lower_copy( parameter.GetType() );
        if( type == "bool" )
        {
            asnParam.set_null_value( 0 ); // $$$$ SBO 2008-05-26: FIXME: simulation bug
            asnParam.mutable_value()->set_abool( false );
        }
        else if( type == "intelligencelist" )
        {
            asnParam.mutable_value()->mutable_intelligencelist()->Clear();
        }
        else if( type == "objectivelist" )
        {
            asnParam.mutable_value()->mutable_missionobjectivelist()->Clear();
        }
        else if( type == "genobjectlist" )
        {
            asnParam.mutable_value()->mutable_plannedworklist()->Clear();
        }
        else if( type == "direction" )
        {
            asnParam.mutable_value()->mutable_heading()->set_heading ( 0 );
        }
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
