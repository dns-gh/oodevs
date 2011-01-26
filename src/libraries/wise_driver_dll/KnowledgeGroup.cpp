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
    , model_( model )
    , type_( message.type().begin(), message.type().end() )
    , scrambled_( message.jam() )
    , enabled_( true )
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
// Name: KnowledgeGroup::Create
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
void KnowledgeGroup::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    try
    {
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, GetIdentifier(), L"Orbat.KnowledgeGroup", handle_, attributes_ ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Identifier" ], long( GetId() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Party" ], party_ ? party_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Superior" ], superior_ ? superior_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Scrambled" ], unsigned char( scrambled_ ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Type" ], type_, currentTime ) );
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
// Name: KnowledgeGroup::Update
// Created: SBO 2011-01-05
// -----------------------------------------------------------------------------
void KnowledgeGroup::Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::KnowledgeGroupUpdate& message )
{
    try
    {
        if( message.has_party() )
        {
            party_ = model_.ResolveParty( message.party().id() );
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Party" ], party_ ? party_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        }
        if( message.has_parent() )
        {
            superior_ = model_.ResolveKnowledgeGroup( message.parent().id() );
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Superior" ], superior_ ? superior_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        }
        if( message.has_enabled() )
        {
            enabled_ = message.enabled();
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Enabled" ], unsigned char( enabled_ ), currentTime ) );
        }
        if( message.has_type() )
        {
            type_ = std::wstring( message.type().begin(), message.type().end() );
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Type" ], type_, currentTime ) );
        }
        driver.NotifyDebugMessage( FormatMessage( L"Updated." ), 2 );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyWarningMessage( FormatMessage( L"Update failed." ), error );
    }
}
