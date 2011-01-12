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
#include <sstream>
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

namespace
{
    unsigned char ConvertAlignment( const sword::EnumDiplomacy& diplomacy )
    {
        if( diplomacy == sword::unknown_diplo )
            return sword::friend_diplo;
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
    , alignment_( unsigned char( message.type() ) )
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
// Name: Party::MakeIdentifier
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
std::wstring Party::MakeIdentifier() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Party::Create
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Party::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    try
    {
        handle_ = WISE_INVALID_HANDLE;
        std::map< std::wstring, WISE_HANDLE > attributes;
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, name_, L"Party", handle_, attributes ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Identifier" ], long( GetId() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Name" ], name_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Alignment" ], alignment_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->AddObjectToDatabase( database, handle_ ) );
        driver.NotifyInfoMessage( FormatMessage( L"Created." ) );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( FormatMessage( L"Creation failed." ), error );
    }
}

// -----------------------------------------------------------------------------
// Name: Party::Destroy
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Party::Destroy( CWISEDriver& driver, const WISE_HANDLE& database ) const
{
    for( T_Diplomacies::const_iterator it = diplomacies_.begin(); it != diplomacies_.end(); ++it )
        
    WiseEntity::Destroy( driver, database );
}

namespace
{
    std::wstring MakeIdentifier( const Model& model, const sword::ChangeDiplomacy& message )
    {
        if( const Party* party1 = model.ResolveParty( message.party1().id() ) )
            if( const Party* party2 = model.ResolveParty( message.party2().id() ) )
            {
                std::wstringstream ss;
                ss << L"PartyDiplomacy:" << party1->GetId() << L"->" << party2->GetId();
                return ss.str();
            }
        return L"Unknown party diplomacy";
    }
}

// -----------------------------------------------------------------------------
// Name: Party::Update
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
void Party::Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::ChangeDiplomacy& message )
{
    const std::wstring identifier = ::MakeIdentifier( model_, message );
    try
    {
        WiseReference*& diplomacy = diplomacies_[ message.party2().id() ];
        if( !diplomacy )
        {
            CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, identifier, L"PartyDiplomacy", diplomacy->handle_, diplomacy->attributes_ ) );
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, diplomacy->handle_, diplomacy->attributes_[ L"PartyFrom" ], long( message.party1().id() ), currentTime ) );
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, diplomacy->handle_, diplomacy->attributes_[ L"PartyTo" ], long( message.party2().id() ), currentTime ) );
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, diplomacy->handle_, diplomacy->attributes_[ L"Diplomacy" ], unsigned char( message.diplomacy() ), currentTime ) );
            CHECK_WISE_RESULT_EX( driver.GetSink()->AddObjectToDatabase( database, diplomacy->handle_ ) );
            driver.NotifyInfoMessage( FormatMessage( identifier + L"' created." ) );
        }
        else
        {
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, diplomacy->handle_, diplomacy->attributes_[ L"Diplomacy" ], unsigned char( message.diplomacy() ), currentTime ) );       
            driver.NotifyInfoMessage( FormatMessage( identifier + L"' updated." ) );
        }
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyWarningMessage( FormatMessage( identifier + L"' update failed." ), error );
    }
}
