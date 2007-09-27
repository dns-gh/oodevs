// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "Connector.h"
#include "Database.h"
#include "ScopeEditor.h"
#include "FolkEditor.h"
#include "ReportFactory.h"
#include "Table_ABC.h"
#include "dispatcher/Model.h"
#include "dispatcher/Config.h"
#include "dispatcher/PluginConfig.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: Connector constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
Connector::Connector( const dispatcher::Config& config, const dispatcher::Model& model )
    : database_     ( 0 )
    , model_        ( model )
    , reportFactory_( new ReportFactory( config, model_ ) )
    , folkEditor_   ( new FolkEditor() )
    , scopeEditor_  ( new ScopeEditor( model_, *reportFactory_ ) )
{
    ::CoInitialize( NULL );
//    #if ! defined( _ARCGIS_VERSION_ )  // $$$$ JCR 2007-06-14: do not use license checking on arcgis version lower than 9.2
    CheckOutLicences( esriLicenseProductCodeArcInfo );
//    #endif

    database_.reset( new Database( config ) ); // $$$$ SBO 2007-08-30: do license check somewhere else to put in init list
}

// -----------------------------------------------------------------------------
// Name: Connector destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
Connector::~Connector()
{
    database_.release();
    scopeEditor_.release();
    folkEditor_.release();
    if( spLicenseHandler_ )
        spLicenseHandler_->Shutdown();
    ::CoUninitialize();
}

// -----------------------------------------------------------------------------
// Name: Connector::CheckOutLicences
// Created: JCR 2007-06-14
// -----------------------------------------------------------------------------
void Connector::CheckOutLicences( esriLicenseProductCode product )
{
    if( FAILED( spLicenseHandler_.CreateInstance( CLSID_AoInitialize ) ) )
        throw std::runtime_error( "Unable to initialize license manager" );
    esriLicenseStatus status;
    if( FAILED( spLicenseHandler_->IsProductCodeAvailable( product, &status ) ) )
        throw std::runtime_error( "No product available" );
    if( FAILED( spLicenseHandler_->Initialize( product, &status ) ) )
        throw std::runtime_error( "Unable to retrieve license information" );
}

// -----------------------------------------------------------------------------
// Name: Connector::GetTable
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
Table_ABC& Connector::GetTable( const std::string& name, bool clear /*= true*/ )
{
    Table_ABC*& table = tables_[name];
    if( !table )
    {
        table = database_->OpenTable( name );
        if( table && clear )
            table->Clear();
    }
    if( !table )
        throw std::runtime_error( "Unable to open table : " + name );
    return *table;
}

// -----------------------------------------------------------------------------
// Name: Connector::Lock
// Created: JCR 2007-05-11
// -----------------------------------------------------------------------------
void Connector::Lock()
{
    database_->Lock();
}

// -----------------------------------------------------------------------------
// Name: Connector::Unlock
// Created: JCR 2007-05-11
// -----------------------------------------------------------------------------
void Connector::Unlock()
{
    database_->UnLock();
}

// -----------------------------------------------------------------------------
// Name: Connector::GetTable
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
Table_ABC& Connector::GetTable( const ASN1T_Location& location )
{
    std::string className = "TacticalObjectArea";
    switch ( location.type )
    {
    case EnumLocationType::point: className = "TacticalObjectPoint"; break;
    case EnumLocationType::line:  className = "TacticalObjectLine"; break;
    }
    return GetTable( className );
}
    
// -----------------------------------------------------------------------------
// Name: Connector::Update
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::Send( const ASN1T_MsgsSimToClient& asn )
{
    switch ( asn.msg.t )
    {
    case T_MsgsSimToClient_msg_msg_formation_creation:          scopeEditor_->Update( GetTable( "Formations" ), *asn.msg.u.msg_formation_creation ); break;
    case T_MsgsSimToClient_msg_msg_automat_creation:            scopeEditor_->Update( GetTable( "Formations" ), *asn.msg.u.msg_automat_creation ); break;

    case T_MsgsSimToClient_msg_msg_lima_creation:               scopeEditor_->Update( GetTable( "TacticalLines" ), *asn.msg.u.msg_lima_creation ); break;
//    case T_MsgsSimToClient_msg_msg_lima_update:                 scopeEditor_->Update( GetTable( "TacticalLines" ), *asn.msg.u.msg_lima_update ); break;
//    case T_MsgsSimToClient_msg_msg_lima_destruction:            scopeEditor_->Update( GetTable( "TacticalLines" ), asn.msg.u.msg_lima_destruction ); break;

    case T_MsgsSimToClient_msg_msg_limit_creation:              scopeEditor_->Update( GetTable( "BoundaryLimits" ), *asn.msg.u.msg_limit_creation ); break;
//    case T_MsgsSimToClient_msg_msg_limit_destruction:           scopeEditor_->Update( GetTable( "BoundaryLimits" ), asn.msg.u.msg_lima_destruction ); break;

    case T_MsgsSimToClient_msg_msg_unit_creation:               scopeEditor_->Update( GetTable( "UnitForces" ), *asn.msg.u.msg_unit_creation ); break;
    case T_MsgsSimToClient_msg_msg_unit_attributes:             scopeEditor_->Update( GetTable( "UnitForces" ), *asn.msg.u.msg_unit_attributes ); break;
    case T_MsgsSimToClient_msg_msg_unit_destruction:            scopeEditor_->Update( GetTable( "UnitForces" ), asn.msg.u.msg_unit_destruction ); break;

    case T_MsgsSimToClient_msg_msg_unit_knowledge_creation:     scopeEditor_->Update( GetTable( "KnowledgeUnits" ), *asn.msg.u.msg_unit_knowledge_creation ); break;
    case T_MsgsSimToClient_msg_msg_unit_knowledge_update:       scopeEditor_->Update( GetTable( "KnowledgeUnits" ), *asn.msg.u.msg_unit_knowledge_update ); break;
    case T_MsgsSimToClient_msg_msg_unit_knowledge_destruction:  scopeEditor_->Update( GetTable( "KnowledgeUnits" ), asn.msg.u.msg_unit_knowledge_destruction->oid );  break;

    case T_MsgsSimToClient_msg_msg_object_creation:             scopeEditor_->Update( GetTable( asn.msg.u.msg_object_creation->location ), *asn.msg.u.msg_object_creation ); break;
//    case T_MsgsSimToClient_msg_msg_object_update:               scopeEditor_->Update( GetTable( asn.msg.u.msg_object_update->location ), *asn.msg.u.msg_object_update ); break;
    case T_MsgsSimToClient_msg_msg_object_destruction:          Delete( asn.msg.u.msg_object_destruction ); break;
    case T_MsgsSimToClient_msg_msg_report:                      scopeEditor_->Update( GetTable( "Reports" ), *asn.msg.u.msg_report ); break;

    case T_MsgsSimToClient_msg_msg_folk_creation:               folkEditor_->Update( *asn.msg.u.msg_folk_creation ); break;
    case T_MsgsSimToClient_msg_msg_folk_graph_update:           folkEditor_->Update( GetTable( "Population", false ), *asn.msg.u.msg_folk_graph_update ); break;
    }
}

// -----------------------------------------------------------------------------
// Name: Connector::Delete
// Created: JCR 2007-05-24
// -----------------------------------------------------------------------------
void Connector::Delete( const ASN1T_MsgObjectDestruction& asn )
{
    std::stringstream ss;
    ss << "Public_OID=" << asn;
    GetTable( "TacticalObjectPoint" ).DeleteRows( ss.str() );
    GetTable( "TacticalObjectLine" ) .DeleteRows( ss.str() );
    GetTable( "TacticalObjectArea" ) .DeleteRows( ss.str() );
}
