// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ESRI_Connector.h"
#include "ESRI_ScopeEditor.h"

using namespace dispatcher;
using namespace esri;

// -----------------------------------------------------------------------------
// Name: Connector constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
Connector::Connector( const std::string& geodatabase, const kernel::Resolver_ABC< kernel::AgentType >& resolver, const World& world )
    : world_ ( world )
    , resolver_ ( resolver )    
{
    ::CoInitialize( NULL ); // Initialize COM
    ConnectToGeodatabase( geodatabase );
}

// -----------------------------------------------------------------------------
// Name: Connector destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
Connector::~Connector()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Connector::ConnectToGeodatabase
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::ConnectToGeodatabase( const std::string& geodatabase )
{    
    IWorkspaceFactoryPtr    spWorkspaceFactory;
    
    spWorkspaceFactory.CreateInstance( CLSID_AccessWorkspaceFactory );
    // Connect to Geodatabase
    if ( FAILED( spWorkspaceFactory->OpenFromFile( CComBSTR( geodatabase.c_str() ), NULL, &spWorkspace_ ) ) )
        throw std::runtime_error( "Unable connect to geodatabase: " + geodatabase );
                                
	// QI for the IFeatureWorkspace
    if ( FAILED( spWorkspace_->QueryInterface( IID_IFeatureWorkspace, (LPVOID*)&spFeatureWorkspace_ ) ) )
        throw std::runtime_error( "Unable cannot to retrieve IFeatureWorkspace interface." );
    
    // $$$$ JCR 2007-04-30: 
    spFeatureWorkspace_->OpenFeatureClass( CComBSTR( L"FriendlyForces" ), &spUnitFeatureClass_ );
    ClearFeatureClass( spUnitFeatureClass_ );
    // spFeatureWorkspace_->OpenFeatureClass( CComBSTR( L"TacticalLines" ), &spUnitFeatureClass_ );
    // spFeatureWorkspace_->OpenFeatureClass( CComBSTR( L"BoundaryLimits" ), &spUnitFeatureClass_ );
}

// -----------------------------------------------------------------------------
// Name: ESRI_Connector::ClearFeatureClass
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void Connector::ClearFeatureClass( IFeatureClassPtr spFeatureClass )
{
    ScopeEditor  editor( spWorkspace_, resolver_, world_ );

    editor.Clear( spFeatureClass );        
}

// -----------------------------------------------------------------------------
// Name: Connector::Update
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::Send( const ASN1T_MsgsSimToClient& asn )
{    
    ScopeEditor  editor( spWorkspace_, resolver_, world_ );
    
    switch ( asn.msg.t )
    {
    case T_MsgsSimToClient_msg_msg_lima_creation:   Send( editor, *asn.msg.u.msg_lima_creation ); break;
    case T_MsgsSimToClient_msg_msg_limit_creation:  Send( editor, *asn.msg.u.msg_limit_creation ); break;  
    case T_MsgsSimToClient_msg_msg_pion_creation:   Send( editor, *asn.msg.u.msg_pion_creation ); break;
    case T_MsgsSimToClient_msg_msg_unit_attributes: Send( editor, *asn.msg.u.msg_unit_attributes ); break;
//    case T_MsgsSimToClient_msg_msg_object_creation: Send( editor, *asn.msg.u.msg_object_creation ); break;
//    case T_MsgsSimToClient_msg_msg_object_update:   Send( editor, *asn.msg.u.msg_object_update ); break;
    }
}

// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::Send( ScopeEditor& editor, const ASN1T_MsgLimaCreation& msg )
{    
//    editor.Insert( spLimaFeatureClass_, msg.oid );
//    editor.Write( msg );
//    editor.Flush();
}

// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::Send( ScopeEditor& editor, const ASN1T_MsgLimitCreation& msg )
{
//    editor.Insert( spLimitFeatureClass_, msg.oid );
//    editor.Write( msg );
//    editor.Flush();
}
    
// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::Send( ScopeEditor& editor, const ASN1T_MsgPionCreation& msg )
{
    if ( editor.Insert( spUnitFeatureClass_, msg.oid_pion ) )
    {
        editor.Write( msg );
        editor.Flush();
    }
}
    
// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::Send( ScopeEditor& editor, const ASN1T_MsgUnitAttributes& msg )
{
    if( editor.Update( spUnitFeatureClass_, msg.oid_pion ) )
    {
        editor.Write( msg );
        editor.Flush();
    }
}
    
// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::Send( ScopeEditor& editor, const ASN1T_MsgObjectCreation& msg )
{
    if ( editor.Insert( spObjectFeatureClass_, msg.oid ) )
    {
        editor.Write( msg );
        editor.Flush();
    }
}
    
// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::Send( ScopeEditor& editor, const ASN1T_MsgObjectUpdate& msg )
{
    if ( editor.Update( spObjectFeatureClass_, msg.oid ) )
    {
        editor.Write( msg );
        editor.Flush();
    }
}
