// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "Agent.h"
#include "Automat.h"
#include "Equipment.h"
#include "KnowledgeGroup.h"
#include "Model.h"
#include "Personnel.h"
#include "Resource.h"
#include "client_proxy/SwordMessagePublisher_ABC.h"
#include "protocol/Simulation.h"
#include "protocol/SimulationSenders.h"
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Agent::Agent( const Model& model, const sword::UnitCreation& message )
    : WiseEntity( message.unit().id(), L"agent" )
    , model_( model )
    , name_( message.name().begin(), message.name().end() )
    , type_( message.type().id() )
    , superior_( model.ResolveAutomat( message.automat().id() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent::Create
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Agent::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    try
    {
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, GetIdentifier(), L"Orbat.Agent", handle_, attributes_ ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Identifier" ], long( GetId() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Name" ], name_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Type" ], long( type_ ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Superior" ], superior_ ? superior_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->AddObjectToDatabase( database, handle_ ) );
        driver.NotifyDebugMessage( FormatMessage( L"Created." ), 0 );
    }
    catch( WISE_RESULT& error )
    {
        handle_ = WISE_INVALID_HANDLE;
        driver.NotifyErrorMessage( FormatMessage( L"Creation failed." ), error );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Agent::Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::UnitAttributes& message )
{
    try
    {
        if( message.has_altitude() )
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Altitude" ], long( message.altitude() ), currentTime ) );
        if( message.has_direction() )
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Heading" ], long( message.direction().heading() ), currentTime ) );
        if( message.has_embarked() )
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Mounted" ], unsigned char( message.embarked() ), currentTime ) );
        if( message.has_operational_state() )
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"OperationalState" ], unsigned char( message.operational_state() ), currentTime ) );
        if( message.has_height() || message.has_position() )
        {
            if( message.has_height() )
                position_.v3 = message.height();
            if( message.has_position() )
            {
                position_.v1 = message.position().latitude();
                position_.v2 = message.position().longitude();
            }
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Position" ], position_, currentTime ) );
        }
        if( message.has_speed() )
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Speed" ], long( message.speed() ), currentTime ) );
        if( message.has_roe() )
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"RuleOfEngagement" ], unsigned char( message.roe() ), currentTime ) );
        if( message.has_communications() )
        {
            if( message.communications().has_knowledge_group() )
            {
                const WiseEntity* entity = model_.ResolveKnowledgeGroup( message.communications().knowledge_group().id() );
                CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Communication.KnowledgeGroup" ], entity ? entity->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
            }
            if( message.communications().has_jammed() )
                CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Communication.Jammed" ], unsigned char( message.communications().jammed() ), currentTime ) );
        }
        if( message.has_equipment_dotations() )
            UpdateComponents( driver, database, currentTime, message.equipment_dotations(), equipments_, L"Equipments" );
        if( message.has_human_dotations() )
            UpdateComponents( driver, database, currentTime, message.human_dotations(), personnel_, L"Personnel" );
        if( message.has_resource_dotations() )
            UpdateComponents( driver, database, currentTime, message.resource_dotations(), resources_, L"Resources" );
        driver.NotifyDebugMessage( FormatMessage( L"Updated." ), 2 );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyWarningMessage( FormatMessage( L"Update failed." ), error );
    }
}

namespace
{
    template< typename Message >
    unsigned long ExtractId( const Message& message )
    {
        return message.type().id();
    }
    unsigned long ExtractId( const sword::HumanDotations::HumanDotation& message )
    {
        return message.rank();
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::UpdateComponents
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
template< class M, class C >
void Agent::UpdateComponents( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const M& message, std::map< unsigned long, C* >& components, const std::wstring& fieldName )
{
    IWISEStringCache* cache = dynamic_cast< IWISEStringCache* >( driver.GetSink() );
    CHECK_VALID_POINTER_EX( cache, MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_E_NOT_INITIATED ) );
    for( int i = 0; i < message.elem_size(); ++i )
    {
        C*& component = components[ ExtractId( message.elem( i ) ) ];
        if( !component )
            component = new C( message.elem( i ) );
        else
            component->Update( message.elem( i ) );
    }
    std::list< CWISEAttributeGroup > list;
    for( std::map< unsigned long, C* >::const_iterator it = components.begin(); it != components.end(); ++it )
        it->second->AddAttributeGroup( list, *cache );
    CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ fieldName ], list, currentTime ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: SBO 2011-01-05
// -----------------------------------------------------------------------------
void Agent::Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::UnitChangeSuperior& message )
{
    try
    {
        if( message.has_parent() )
        {
            superior_ = model_.ResolveAutomat( message.parent().id() );
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Superior" ], superior_ ? superior_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        }
        driver.NotifyDebugMessage( FormatMessage( L"Updated." ), 2 );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyWarningMessage( FormatMessage( L"Update failed." ), error );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: SBO 2011-01-05
// -----------------------------------------------------------------------------
void Agent::Update( SwordMessagePublisher_ABC& publisher, const WISE_HANDLE& attribute, const CWISEValueUnion& value )
{
    // Position change triggers a Teleport magic action
    if( attribute == attributes_[ L"Position" ] )
    {
        const CWISEVec3 position = value.GetVec3Value();
        simulation::UnitMagicAction message;
        message().mutable_tasker()->mutable_unit()->set_id( GetId() );
        message().set_type( sword::UnitMagicAction::move_to );
        sword::MissionParameter& parameter = *message().mutable_parameters()->add_elem();
        parameter.set_null_value( false );
        sword::Location& location = *parameter.add_value()->mutable_point()->mutable_location();
        location.set_type( sword::Location::point );
        sword::CoordLatLong& coordinates = *location.mutable_coordinates()->add_elem();
        coordinates.set_latitude( position.v1 );
        coordinates.set_longitude( position.v2 );
        message.Send( publisher );
    }
    // Superior change triggers a ChangeSuperior magic action
    else if( attribute == attributes_[ L"Superior" ] )
    {
        const WiseEntity* superior = model_.ResolveEntity( value.GetHandleValue() );
        simulation::UnitMagicAction message;
        message().mutable_tasker()->mutable_unit()->set_id( GetId() );
        message().set_type( sword::UnitMagicAction::unit_change_superior );
        sword::MissionParameter& parameter = *message().mutable_parameters()->add_elem();
        parameter.set_null_value( false );
        parameter.add_value()->mutable_automat()->set_id( superior ? superior->GetId() : 0 );
        message.Send( publisher );
    }
}
