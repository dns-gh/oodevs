// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "Party.h"
#include "Model.h"
#include "protocol/Simulation.h"
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/WISEAttributeGroupConverter.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

namespace
{
    unsigned char ConvertAlignment( const sword::EnumDiplomacy& diplomacy )
    {
        if( diplomacy == sword::unknown )
            return sword::friendly;
        return char( diplomacy ) - 1;
    }
}

// -----------------------------------------------------------------------------
// Name: Party constructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Party::Party( const Model& model, const sword::PartyCreation& message )
    : WiseEntity( message.party().id(), L"party" )
    , model_( model )
    , name_( message.name().begin(), message.name().end() )
    , alignment_( ConvertAlignment( message.type() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Party destructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Party::~Party()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Party::Create
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Party::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    try
    {
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, GetIdentifier(), L"Orbat.Party", handle_, attributes_ ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Identifier" ], long( GetId() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Name" ], name_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Alignment" ], alignment_, currentTime ) );
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
// Name: Party::DoUpdate
// Created: SEB 2011-01-07
// -----------------------------------------------------------------------------
template< typename M >
void Party::DoUpdate( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const M& message )
{
    try
    {
        IWISEStringCache* cache = dynamic_cast< IWISEStringCache* >( driver.GetSink() );
        CHECK_VALID_POINTER_EX( cache, MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_E_NOT_INITIATED ) );
        const Party* party = model_.ResolveParty( message.party2().id() );
        if( !party )
            return;
        alliances_[ party->GetHandle() ] = ConvertAlignment( message.diplomacy() );
        std::list< CWISEAttributeGroup > list;
        for( T_Alliances::const_iterator it = alliances_.begin(); it != alliances_.end(); ++it )
        {
            CWISEAttributeGroupTemplate groupTemplate;
            groupTemplate.Add( L"Party", it->first );
            groupTemplate.Add( L"Type", it->second );
            CWISEAttributeGroup group;
            CHECK_WISE_RESULT_EX( CWISEAttributeGroupConverter::TemplateToValueGroup( groupTemplate, cache, L"Alliance", group ) );
            list.push_back( group );
        }
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Alliances" ], list, currentTime ) );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyWarningMessage( FormatMessage( L"Alliance update failed." ), error );
    }
}

// -----------------------------------------------------------------------------
// Name: Party::Update
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
void Party::Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::ChangeDiplomacy& message )
{
    DoUpdate( driver, database, currentTime, message );
}

// -----------------------------------------------------------------------------
// Name: Party::Update
// Created: SEB 2011-01-07
// -----------------------------------------------------------------------------
void Party::Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::ChangeDiplomacyAck& message )
{
    // $$$$ SEB 2011-01-07: Should not be needed as MsgChangeDiplomacy should be sent
    DoUpdate( driver, database, currentTime, message );
}
