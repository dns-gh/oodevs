// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "Automat.h"
#include "Formation.h"
#include "KnowledgeGroup.h"
#include "Model.h"
#include "Party.h"
#include "client_proxy/SwordMessagePublisher_ABC.h"
#include "protocol/SimulationSenders.h"
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

namespace
{
    template< typename T >
    const Entity_ABC* ResolveSuperior( const Model& model, const T& parent )
    {
        if( parent.has_formation() )
            return model.ResolveFormation( parent.formation().id() );
        if( parent.has_automat() )
            return model.ResolveAutomat( parent.automat().id() );
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Automat::Automat( const Model& model, const sword::AutomatCreation& message )
    : WiseEntity( message.automat().id(), L"automat" )
    , model_( model )
    , name_( message.name().begin(), message.name().end() )
    , type_( message.type().id() )
    , party_( model.ResolveParty( message.party().id() ) )
    , knowledgeGroup_( model.ResolveKnowledgeGroup( message.knowledge_group().id() ) )
    , superior_( ResolveSuperior( model, message.parent() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat::Create
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Automat::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    try
    {
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, GetIdentifier(), L"Orbat.Automat", handle_, attributes_ ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Identifier" ], long( GetId() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Name" ], name_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Type" ], long( type_ ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Superior" ], superior_ ? superior_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Party" ], party_ ? party_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"KnowledgeGroup" ], knowledgeGroup_ ? knowledgeGroup_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
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
// Name: Automat::Update
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
void Automat::Update( SwordMessagePublisher_ABC& publisher, const WISE_HANDLE& attribute, const CWISEValueUnion& value )
{
    if( attribute == attributes_[ L"Superior" ] )
    {
        const WiseEntity* superior = model_.ResolveEntity( value.GetHandleValue() );
        simulation::UnitMagicAction message;
        message().mutable_tasker()->mutable_automat()->set_id( GetId() );
        sword::MissionParameter& parameter = *message().mutable_parameters()->add_elem();
        parameter.set_null_value( false );
        if( superior && superior->IsA< Automat >() )
        {
            message().set_type( sword::UnitMagicAction::change_automat_superior );
            parameter.add_value()->mutable_automat()->set_id( superior->GetId() );
        }
        else if( superior && superior->IsA< Formation >() )
        {
            message().set_type( sword::UnitMagicAction::change_formation_superior );
            parameter.add_value()->mutable_formation()->set_id( superior->GetId() );
        }
        message.Send( publisher );
    }
    else if( attribute == attributes_[ L"KnowledgeGroup" ] )
    {
        const WiseEntity* superior = model_.ResolveEntity( value.GetHandleValue() );
        simulation::UnitMagicAction message;
        message().mutable_tasker()->mutable_automat()->set_id( GetId() );
        message().set_type( sword::UnitMagicAction::change_knowledge_group );
        sword::MissionParameter& parameter = *message().mutable_parameters()->add_elem();
        parameter.set_null_value( false );
        parameter.add_value()->mutable_knowledgegroup()->set_id( superior ? superior->GetId() : 0 );
        message.Send( publisher );
    }
    else if( attribute == attributes_[ L"AutomationEnabled" ] )
    {
        simulation::SetAutomatMode message;
        message().mutable_automate()->set_id( GetId() );
        message().set_mode( value.GetByteValue() ? sword::engaged : sword::disengaged );
        message.Send( publisher, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
void Automat::Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::AutomatAttributes& message )
{
    try
    {
        if( handle_ == WISE_INVALID_HANDLE )
        {
            driver.NotifyWarningMessage( FormatMessage( L"Not initialized." ), MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_W_NOT_INITIATED ) );
            return;
        }
        if( message.has_mode() )
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Automation" ], unsigned char( message.mode() ), currentTime ) );
        if( message.has_operational_state() )
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"OperationalState" ], unsigned char( message.operational_state() ), currentTime ) );
        driver.NotifyDebugMessage( FormatMessage( L"Updated." ), 2 );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyWarningMessage( FormatMessage( L"Update failed." ), error );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
void Automat::Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::AutomatChangeSuperior& message )
{
    try
    {
        if( handle_ == WISE_INVALID_HANDLE )
        {
            driver.NotifyWarningMessage( FormatMessage( L"Not initialized." ), MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_W_NOT_INITIATED ) );
            return;
        }
        if( message.has_superior() )
        {
            superior_ = ResolveSuperior( model_, message.superior() );
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Superior" ], superior_ ? superior_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        }
        driver.NotifyDebugMessage( FormatMessage( L"Superior updated." ), 0 );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyWarningMessage( FormatMessage( L"Superior update failed." ), error );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
void Automat::Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::AutomatChangeKnowledgeGroup& message )
{
    try
    {
        if( handle_ == WISE_INVALID_HANDLE )
        {
            driver.NotifyWarningMessage( FormatMessage( L"Not initialized." ), MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_W_NOT_INITIATED ) );
            return;
        }
        if( message.has_knowledge_group() )
        {
            knowledgeGroup_ = model_.ResolveKnowledgeGroup( message.knowledge_group().id() );
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"KnowledgeGroup" ], knowledgeGroup_ ? knowledgeGroup_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        }
        driver.NotifyDebugMessage( FormatMessage( L"Knowledge group updated." ), 0 );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyWarningMessage( FormatMessage( L"Knowledge group update failed." ), error );
    }
}
