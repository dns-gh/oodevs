// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "KnowledgeGroup.h"
#include "Model.h"
#include "Party.h"
#include "protocol/Simulation.h"
#include <sstream>
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( const Model& model, const sword::KnowledgeGroupCreation& message )
    : WiseEntity( message.knowledge_group().id(), L"knowledge-group" )
    , type_( message.type().begin(), message.type().end() )
    , scrambled_( message.jam() )
    , party_( model.ResolveParty( message.party().id() ) )
    , superior_( model.ResolveKnowledgeGroup( message.parent().id() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::MakeIdentifier
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
std::wstring KnowledgeGroup::MakeIdentifier() const
{
    std::wstringstream ss;
    ss << GetId();
    return ss.str();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Create
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
void KnowledgeGroup::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    try
    {
        const std::wstring identifier( MakeIdentifier() );
        handle_ = WISE_INVALID_HANDLE;
        std::map< std::wstring, WISE_HANDLE > attributes;
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, identifier, L"KnowledgeGroup", handle_, attributes ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Identifier" ], long( GetId() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Party" ], party_ ? party_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Superior" ], superior_ ? superior_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Scrambled" ], unsigned char( scrambled_ ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->AddObjectToDatabase( database, handle_ ) );
        driver.NotifyInfoMessage( FormatMessage( L"Created." ) );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( FormatMessage( L"Creation failed." ), error );
    }
}
