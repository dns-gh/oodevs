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
#include "Model.h"
#include "Personnel.h"
#include "Resource.h"
#include "protocol/Simulation.h"
#include <sstream>
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Agent::Agent( const Model& model, const MsgsSimToClient::MsgUnitCreation& message )
    : id_( message.unit().id() )
    , name_( message.nom().begin(), message.nom().end() )
    , type_( message.type().id() )
    , superior_( model.ResolveAutomat( message.automat().id() ) )
    , handle_( WISE_INVALID_HANDLE )
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
// Name: Agent::GetId
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
unsigned long Agent::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetHandle
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
WISE_HANDLE Agent::GetHandle() const
{
    return handle_;
}

// -----------------------------------------------------------------------------
// Name: Agent::MakeIdentifier
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
std::wstring Agent::MakeIdentifier() const
{
    std::wstringstream ss;
    ss << GetId() << "-" << name_;
    return ss.str();
}

// -----------------------------------------------------------------------------
// Name: Agent::Create
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Agent::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    const std::wstring identifier( MakeIdentifier() );
    try
    {
        handle_ = WISE_INVALID_HANDLE;
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, identifier, L"Agent", handle_, attributes_ ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Identifier" ], long( id_ ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Name" ], name_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Type" ], long( type_ ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Superior" ], superior_ ? superior_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->AddObjectToDatabase( database, handle_ ) );
        driver.NotifyInfoMessage( L"Agent '" + identifier + L"' created." );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"Failed to create agent '" + identifier + L"'.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::Destroy
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Agent::Destroy( CWISEDriver& driver, const WISE_HANDLE& database ) const
{
    const std::wstring identifier( MakeIdentifier() );
    try
    {
        if( handle_ != WISE_INVALID_HANDLE )
        {
            CHECK_WISE_RESULT_EX( driver.GetSink()->RemoveObjectFromDatabase( database, handle_ ) );
            handle_ = WISE_INVALID_HANDLE;
        }
        driver.NotifyInfoMessage( L"Agent '" + identifier + L"' destroyed." );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"Failed to destroy agent '" + identifier + L"'.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Agent::Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const MsgsSimToClient::MsgUnitAttributes& message )
{
    try
    {
        if( message.has_altitude() )
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Altitude" ], long( message.altitude() ), currentTime ) );
        if( message.has_direction() )
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Heading" ], long( message.direction().heading() ), currentTime ) );
        if( message.has_embarque() )
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Mounted" ], unsigned char( message.embarque() ), currentTime ) );
        if( message.has_etat_operationnel() )
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"OperationalState" ], unsigned char( message.etat_operationnel() ), currentTime ) );
        if( message.has_hauteur() || message.has_position() )
        {
            if( message.has_hauteur() )
                position_.v3 = message.hauteur();
            if( message.has_position() )
            {
                position_.v1 = message.position().latitude();
                position_.v2 = message.position().longitude();
            }
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Position" ], position_, currentTime ) );
        }
        if( message.has_vitesse() )
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Speed" ], long( message.vitesse() ), currentTime ) );
        if( message.has_dotation_eff_materiel() )
            UpdateComponents( driver, database, currentTime, message.dotation_eff_materiel(), equipments_, L"Equipments" );
        if( message.has_dotation_eff_personnel() )
            UpdateComponents( driver, database, currentTime, message.dotation_eff_personnel(), personnel_, L"Personnel" );
        if( message.has_dotation_eff_ressource() )
            UpdateComponents( driver, database, currentTime, message.dotation_eff_ressource(), resources_, L"Resources" );
        driver.NotifyDebugMessage( L"Agent '" + MakeIdentifier() + L"' updated.", MessageCategoryDebugLevel0 );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyWarningMessage( L"Failed to update agent '" + MakeIdentifier() + L"'.", error );
    }
}

namespace
{
    template< typename Message >
    unsigned long ExtractId( const Message& message )
    {
        return message.type().id();
    }
    unsigned long ExtractId( const MsgsSimToClient::HumanDotations::HumanDotation& message )
    {
        return message.rang();
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
    std::list< CWISEAttributeGroup > list;
    for( int i = 0; i < message.elem_size(); ++i )
    {
        C*& component = components[ ExtractId( message.elem( i ) ) ];
        if( !component )
            component = new C( message.elem( i ) );
        else
            component->Update( message.elem( i ) );
    }
    for( std::map< unsigned long, C* >::const_iterator it = components.begin(); it != components.end(); ++it )
        it->second->AddAttributeGroup( list, *cache );
    CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ fieldName ], list, currentTime ) );
}
