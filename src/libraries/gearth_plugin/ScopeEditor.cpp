// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gearth_plugin_pch.h"
#include "ScopeEditor.h"
#include "PositionEditor.h"
#include "LineEditor.h"
#include "LocationEditor.h"
#include "SymbolEditor.h"
#include "StyleEditor.h"
#include "ReportFactory.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Model.h"
#include "dispatcher/Config.h"
#include "dispatcher/PluginConfig.h"

#include "xeumeuleu/xml.h"

using namespace gearth;

// -----------------------------------------------------------------------------
// Name: ScopeEditor constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ScopeEditor::ScopeEditor( const dispatcher::Config& config, const dispatcher::Model& model, const ReportFactory& reportFactory )
    : model_            ( model )
    , reportFactory_    ( reportFactory )    
    , styles_           ( new StyleEditor( config ) )
{
    const dispatcher::PluginConfig& plugin = config.GetPluginConfig( "gearth" );
    xos_.reset( new xml::xofstream( config.BuildGameChildFile( plugin.GetParameter( "output" ) ) ) );
    *xos_ << xml::start( "kml" )
          << xml::attribute( "xmlns", "http://earth.google.com/kml/2.1" )
            << xml::start( "Document" )
                << xml::content( "name", plugin.GetParameter( "name" ) )
                << xml::content( "description", plugin.GetParameter( "description" ) );    
    styles_->Load( *xos_ );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ScopeEditor::~ScopeEditor()
{
    Flush();
    *xos_ << xml::end(); // Document
    *xos_ << xml::end(); // 
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::VisitModel
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ScopeEditor::VisitModel( dispatcher::ModelVisitor_ABC& visitor )
{
    model_.Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Flush
// Created: JCR 2007-09-03
// -----------------------------------------------------------------------------
void ScopeEditor::Flush()
{
    for ( IT_StubStreams it = stubs_.begin(); it != stubs_.end(); ++it )
        (*xos_) << *(it->second);
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void ScopeEditor::Send( const ASN1T_MsgsSimToClient& asn )
{
    switch ( asn.msg.t )
    {
    case T_MsgsSimToClient_msg_msg_formation_creation:      Write( *asn.msg.u.msg_formation_creation ); break;
    case T_MsgsSimToClient_msg_msg_automat_creation:        Write( *asn.msg.u.msg_automat_creation ); break;

    case T_MsgsSimToClient_msg_msg_lima_creation:           Write( *asn.msg.u.msg_lima_creation ); break;
    case T_MsgsSimToClient_msg_msg_limit_creation:          Write( *asn.msg.u.msg_limit_creation ); break;    
    case T_MsgsSimToClient_msg_msg_unit_creation:           Write( *asn.msg.u.msg_unit_creation ); break;
    case T_MsgsSimToClient_msg_msg_unit_attributes:         Write( *asn.msg.u.msg_unit_attributes ); break;
    
    case T_MsgsSimToClient_msg_msg_unit_knowledge_creation: Write( *asn.msg.u.msg_unit_knowledge_creation ); break;
    case T_MsgsSimToClient_msg_msg_unit_knowledge_update:   Write( *asn.msg.u.msg_unit_knowledge_update ); break;

    case T_MsgsSimToClient_msg_msg_object_creation:         Write( *asn.msg.u.msg_object_creation ); break;    
    case T_MsgsSimToClient_msg_msg_report:                  Write( *asn.msg.u.msg_report ); break;
    }
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Send
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
void ScopeEditor::Send( const ASN1T_MsgsAuthenticationToClient& /*asn*/ )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ScopeEditor::Send
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
void ScopeEditor::Send( const ASN1T_MsgsReplayToClient& /*asn*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgFormationCreation& /*asn*/ )
{
    // TODO 
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgAutomatCreation& /*asn*/ )
{
    // TODO 
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgLimaCreation& asn )
{
    stubs_[ asn.oid ].reset( new xml::xobufferstream() );
    xml::xobufferstream& xobs = *stubs_[ asn.oid ];
    
    xobs << xml::start( "Placemark" )
            << xml::content( "Name", asn.tactical_line.name )
            << xml::content( "styleUrl", SymbolEditor( *styles_, model_ ).GetSymbol( asn ) );
    LineEditor( xobs ).Write( asn.tactical_line.geometry );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgLimitCreation& asn )
{
    stubs_[ asn.oid ].reset( new xml::xobufferstream() );
    xml::xobufferstream& xobs = *stubs_[ asn.oid ];
    
    xobs << xml::start( "Placemark" )
            << xml::content( "Name", asn.tactical_line.name )
            << xml::content( "styleUrl", SymbolEditor( *styles_, model_ ).GetSymbol( asn ) );
    LineEditor( xobs ).Write( asn.tactical_line.geometry );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgUnitCreation& asn )
{
    stubs_[ asn.oid ].reset( new xml::xobufferstream() );
    xml::xobufferstream& xobs = *stubs_[ asn.oid ];
    
    xobs << xml::start( "Placemark" )
          << xml::content( "Name", asn.nom )
          << xml::content( "styleUrl", SymbolEditor( *styles_, model_ ).GetSymbol( asn ) );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgUnitAttributes& asn )
{
    // model_.GetAgents().Find( asn.oid );    
    T_StubStreams::iterator it = stubs_.find( asn.oid );
    if ( it != stubs_.end() )
    {
//        if( asn.m.vitessePresent ) ;
//            Write( spFeature_, CComBSTR( L"Speed" ), asn.vitesse );
//        if( asn.m.etat_operationnelPresent )
//            Write( spFeature_, CComBSTR( L"OpsState" ), asn.etat_operationnel_brut );
        if( asn.m.positionPresent )
            PositionEditor( *(it->second) ).Write( asn.position );
    }      
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgUnitKnowledgeCreation& asn )
{    
    stubs_[ asn.oid ].reset( new xml::xobufferstream() );
    xml::xobufferstream& xobs = *stubs_[ asn.oid ];

    xobs << xml::start( "Placemark" )
          << xml::content( "Name", asn.oid )
          << xml::content( "styleUrl", SymbolEditor( *styles_, model_ ).GetSymbol( asn ) );

//    if( const dispatcher::Agent* realAgent = model_.GetAgents().Find( asn.oid_unite_reelle ) ) // $$$$ SBO 2007-08-24: snif
//        Write( spBuffer, CComBSTR( L"ObserverAffiliation" ), tools::app6::GetAffiliation( realAgent->BuildSymbol() ).c_str() );

    // PositionEditor( *(it->second) ).Write( asn.position );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgUnitKnowledgeUpdate& asn )
{
    T_StubStreams::iterator it = stubs_.find( asn.oid );
    if ( it != stubs_.end() )
    {
//        if( asn.m.speedPresent )
//            Write( spFeature_, CComBSTR( L"Speed" ), asn.speed );
//        if( asn.m.identification_levelPresent )
//            Write( spFeature_, CComBSTR( L"IdentificationLevel" ), asn.identification_level );
//        if( asn.m.max_identification_levelPresent )
//            SymbolEditor( *this, model_ ).Write( spFeature_, asn );
        if( asn.m.positionPresent )
            PositionEditor( *(it->second) ).Write( asn.position );
    }
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgObjectCreation& asn )
{
    stubs_[ asn.oid ].reset( new xml::xobufferstream() );
    xml::xobufferstream& xobs = *stubs_[ asn.oid ];
    xobs << xml::start( "Placemark" )
          << xml::content( "Name", asn.name )
          << xml::content( "styleUrl", SymbolEditor( *styles_, model_ ).GetSymbol( asn ) );

    switch ( asn.location.type )
    {
    case EnumLocationType::circle:
    case EnumLocationType::point:
        PositionEditor( xobs ).Write( asn.location.coordinates.elem[ 0 ] );
        break;
    case EnumLocationType::line:
        LineEditor( xobs ).Write( asn.location.coordinates );
        break;
    default:
        LocationEditor( xobs ).Write( asn.location );
    }
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgReport& asn )
{
    T_StubStreams::iterator it = stubs_.find( asn.oid );
    if ( it != stubs_.end() )
    {
        *(it->second) << reportFactory_.CreateMessage( asn ).c_str();
    }    
}