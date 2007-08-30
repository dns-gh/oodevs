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
#include "ScopeEditor.h"
#include "ReportFactory.h"
#include "FolkManager.h"
#include "dispatcher/Model.h"
#include "dispatcher/Config.h"
#include "dispatcher/PluginConfig.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: Connector constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
Connector::Connector( const dispatcher::Config& config, const dispatcher::Model& model )
    : model_        ( model )
    , reportFactory_( new ReportFactory( config, model_ ) )
    , folk_         ( new FolkManager( config ) )
    , scopeEditor_  ( new ScopeEditor( model_, *reportFactory_, *folk_ ) )
{
    ::CoInitialize( NULL );
//    #if ! defined( _ARCGIS_VERSION_ )  // $$$$ JCR 2007-06-14: do not use license checking on arcgis version lower than 9.2
    CheckOutLicences( esriLicenseProductCodeArcInfo );
//    #endif

    const dispatcher::PluginConfig& pluginConfig = config.GetPluginConfig( "crossbow" );
    ConnectToGeodatabase( config.BuildGameChildFile( pluginConfig.GetParameter( "geodatabase" ) ) );
    LoadSpatialReference();
}

// -----------------------------------------------------------------------------
// Name: Connector destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
Connector::~Connector()
{
    scopeEditor_.release();
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
// Name: Connector::ConnectToGeodatabase
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::ConnectToGeodatabase( const std::string& geodatabase )
{
    IWorkspaceFactoryPtr spWorkspaceFactory( CLSID_AccessWorkspaceFactory );
    IWorkspacePtr workspace;
    if( spWorkspaceFactory == NULL )
        throw std::runtime_error( "Unable to create Access workspace factory." );
    if( FAILED( spWorkspaceFactory->OpenFromFile( CComBSTR( geodatabase.c_str() ), NULL, &workspace ) ) )
        throw std::runtime_error( "Unable to connect to geodatabase: " + geodatabase );
    if( FAILED( workspace->QueryInterface( IID_IFeatureWorkspace, (LPVOID*)&spWorkspace_ ) ) )
        throw std::runtime_error( "Cannot retrieve IFeatureWorkspace interface." );
}

// -----------------------------------------------------------------------------
// Name: Connector::LoadStatialReference
// Created: JCR 2007-05-11
// -----------------------------------------------------------------------------
void Connector::LoadSpatialReference()
{
    ISpatialReferenceFactoryPtr factory( CLSID_SpatialReferenceEnvironment );
    IGeographicCoordinateSystemPtr geoCoordSystem;
    if( factory == NULL || FAILED( factory->CreateGeographicCoordinateSystem( (int)esriSRGeoCS_WGS1984, &geoCoordSystem ) ) ) // _NTF / _WGS1984
        throw std::runtime_error( "Unable to initialize coordinate system" );
    spSpatialReference_ = geoCoordSystem;
}

namespace
{
    std::string GetString( BSTR bstrIN )
    {
        TCHAR       szFinal[ 255 ];

        // direct conversion from BSTR to LPCTSTR only works in Unicode
        _bstr_t bstrIntermediate( bstrIN ); // convert to _bstr_t

        // you have to go through _bstr_t to have it work in ANSI and Unicode
        _stprintf( szFinal, _T( "%s" ), ( LPCTSTR )bstrIntermediate );
        return std::string( szFinal );
    }

    void check_error()
    {
        IErrorInfoPtr ipError;
        BSTR          strError;

        ::GetErrorInfo( 0, &ipError );
        ipError->GetDescription( &strError );
        MT_LOG_ERROR_MSG( GetString( strError ) );
        // throw std::runtime_error( "Connector::Error: " + GetString( strError ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Connector::LoadFeatureClass
// Created: JCR 2007-05-11
// -----------------------------------------------------------------------------
IFeatureClassPtr Connector::LoadFeatureClass( const std::string& feature )
{
    IFeatureClassPtr spFeatureClass;
    if( FAILED( spWorkspace_->OpenFeatureClass( CComBSTR( feature.c_str() ), &spFeatureClass ) ) || spFeatureClass == NULL )
        check_error();
    else
    {
        ClearFeatureClass( spFeatureClass );
        SetSpatialReference( spFeatureClass );
    }
    return spFeatureClass;
}

// -----------------------------------------------------------------------------
// Name: Connector::ClearFeatureClass
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void Connector::ClearFeatureClass( IFeatureClassPtr spFeatureClass )
{
    scopeEditor_->Clear( spFeatureClass );
}

// -----------------------------------------------------------------------------
// Name: Connector::SetSpatialReference
// Created: SBO 2007-08-29
// -----------------------------------------------------------------------------
void Connector::SetSpatialReference( IFeatureClassPtr spFeatureClass )
{
    IGeoDatasetSchemaEditPtr geoDatasetSchemaEdit;
    if( !FAILED( spFeatureClass->QueryInterface( IID_IGeoDatasetSchemaEdit, (LPVOID*)&geoDatasetSchemaEdit ) ) )
    {
        VARIANT_BOOL canAlter;
        if( !FAILED( geoDatasetSchemaEdit->get_CanAlterSpatialReference( &canAlter ) ) && canAlter == VARIANT_TRUE )
        {
            geoDatasetSchemaEdit->AlterSpatialReference( spSpatialReference_ );
            return;
        }
    }
    check_error();
}

// -----------------------------------------------------------------------------
// Name: Connector::GetFeatureClass
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
IFeatureClassPtr Connector::GetFeatureClass( const std::string& feature )
{
    IFeatureClassPtr& spFeatureClass = features_[ feature ];
    if( spFeatureClass == NULL )
        spFeatureClass = LoadFeatureClass( feature );
    return spFeatureClass;
}

// -----------------------------------------------------------------------------
// Name: Connector::GetTable
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
ITablePtr Connector::GetTable( const std::string& name )
{
    ITablePtr& spTable = tables_[ name ];
    if( spTable == NULL )
    {
        if( FAILED( spWorkspace_->OpenTable( CComBSTR( name.c_str() ), &spTable ) ) )
        {
            check_error();
            throw std::runtime_error( "cannot open" + name );
        }
        IQueryFilterPtr spFilter( CLSID_QueryFilter );
        spFilter->put_WhereClause( CComBSTR( "1" ) );
        spTable->DeleteSearchedRows( spFilter );
    }
    return spTable;
}

// -----------------------------------------------------------------------------
// Name: Connector::Lock
// Created: JCR 2007-05-11
// -----------------------------------------------------------------------------
void Connector::Lock()
{
    scopeEditor_->StartEdit( spWorkspace_, spSpatialReference_ );
}

// -----------------------------------------------------------------------------
// Name: Connector::Unlock
// Created: JCR 2007-05-11
// -----------------------------------------------------------------------------
void Connector::Unlock()
{
    scopeEditor_->StopEdit();
}

// -----------------------------------------------------------------------------
// Name: Connector::VisitModel
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void Connector::VisitModel( dispatcher::ModelVisitor_ABC& visitor )
{
    model_.Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: Connector::Create
// Created: JCR 2007-05-24
// -----------------------------------------------------------------------------
template< typename Message >
void Connector::Create( IFeatureClassPtr spFeature, const Message& asn )
{
    scopeEditor_->Insert( spFeature, asn );
}

// -----------------------------------------------------------------------------
// Name: Connector::Create
// Created: JCR 2007-06-11
// -----------------------------------------------------------------------------
template< typename Message >
void Connector::Create( ITablePtr spTable, const Message& asn )
{
    scopeEditor_->Insert( spTable, asn );
}

// -----------------------------------------------------------------------------
// Name: Connector::Delete
// Created: JCR 2007-05-24
// -----------------------------------------------------------------------------
template< typename Message >
void Connector::Delete( IFeatureClassPtr spFeature, const Message& asn_oid )
{
    scopeEditor_->Delete( spFeature, asn_oid );
}

// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
template<typename Message >
void Connector::Update( IFeatureClassPtr pFeatureClass, const Message& asn )
{
    if( scopeEditor_->Update( pFeatureClass, asn ) )
    {
        scopeEditor_->Write( asn );
        scopeEditor_->Flush();
    }
}

// -----------------------------------------------------------------------------
// Name: Connector::GetObjectFeatureClass
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
IFeatureClassPtr Connector::GetObjectFeatureClass( const ASN1T_Location& location )
{
    std::string className = "TacticalObjectArea";
    switch ( location.type )
    {
    case EnumLocationType::point: className = "TacticalObjectPoint"; break;
    case EnumLocationType::line:  className = "TacticalObjectLine"; break;
    }
    return GetFeatureClass( className );
}
    
// -----------------------------------------------------------------------------
// Name: Connector::Update
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::Send( const ASN1T_MsgsSimToClient& asn )
{
    switch ( asn.msg.t )
    {
    case T_MsgsSimToClient_msg_msg_formation_creation:  Create( GetTable( "Formations" ), *asn.msg.u.msg_formation_creation ); break;
    case T_MsgsSimToClient_msg_msg_automat_creation:    Create( GetTable( "Formations" ), *asn.msg.u.msg_automat_creation ); break;

    case T_MsgsSimToClient_msg_msg_lima_creation:       Create( GetFeatureClass( "TacticalLines" ), *asn.msg.u.msg_lima_creation ); break;
    case T_MsgsSimToClient_msg_msg_lima_update:         Update( GetFeatureClass( "TacticalLines" ), *asn.msg.u.msg_lima_update ); break;
    case T_MsgsSimToClient_msg_msg_lima_destruction:    Delete( GetFeatureClass( "TacticalLines" ), asn.msg.u.msg_lima_destruction ); break;

    case T_MsgsSimToClient_msg_msg_limit_creation:      Create( GetFeatureClass( "BoundaryLimits" ), *asn.msg.u.msg_limit_creation ); break;
    case T_MsgsSimToClient_msg_msg_limit_destruction:   Delete( GetFeatureClass( "BoundaryLimits" ), asn.msg.u.msg_lima_destruction ); break;

    case T_MsgsSimToClient_msg_msg_unit_creation:       Create( GetFeatureClass( "UnitForces" ), *asn.msg.u.msg_unit_creation ); break;
    case T_MsgsSimToClient_msg_msg_unit_attributes:     Update( GetFeatureClass( "UnitForces" ), *asn.msg.u.msg_unit_attributes ); break;
    case T_MsgsSimToClient_msg_msg_unit_destruction:    Delete( GetFeatureClass( "UnitForces" ), asn.msg.u.msg_unit_destruction ); break;

    case T_MsgsSimToClient_msg_msg_unit_knowledge_creation:     Create( GetFeatureClass( "KnowledgeUnits" ), *asn.msg.u.msg_unit_knowledge_creation ); break;
    case T_MsgsSimToClient_msg_msg_unit_knowledge_update:       Update( GetFeatureClass( "KnowledgeUnits" ), *asn.msg.u.msg_unit_knowledge_update ); break;
    case T_MsgsSimToClient_msg_msg_unit_knowledge_destruction:  Delete( GetFeatureClass( "KnowledgeUnits" ), asn.msg.u.msg_unit_knowledge_destruction->oid );  break;

    case T_MsgsSimToClient_msg_msg_object_creation:     Create( GetObjectFeatureClass( asn.msg.u.msg_object_creation->location ), *asn.msg.u.msg_object_creation ); break;
    case T_MsgsSimToClient_msg_msg_object_update:       Update( GetObjectFeatureClass( asn.msg.u.msg_object_update->location ), *asn.msg.u.msg_object_update ); break;
    case T_MsgsSimToClient_msg_msg_object_destruction:  Delete( asn.msg.u.msg_object_destruction ); break;
    case T_MsgsSimToClient_msg_msg_report:              Create( GetTable( "Reports" ), *asn.msg.u.msg_report ); break;
    
    case T_MsgsSimToClient_msg_msg_folk_creation:           folk_->Send( *asn.msg.u.msg_folk_creation ); break;
    case T_MsgsSimToClient_msg_msg_folk_graph_edge_update:  Update( GetFeatureClass( "Population" ), *asn.msg.u.msg_folk_graph_edge_update ); break;
    }
}

// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void Connector::Send( const ASN1T_MsgsAuthenticationToClient& )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void Connector::Send( const ASN1T_MsgsReplayToClient& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Connector::Delete
// Created: JCR 2007-05-24
// -----------------------------------------------------------------------------
void Connector::Delete( const ASN1T_MsgObjectDestruction& asn )
{
    scopeEditor_->Delete( GetFeatureClass( "TacticalObjectPoint" ), asn );
    scopeEditor_->Delete( GetFeatureClass( "TacticalObjectLine" ), asn );
    scopeEditor_->Delete( GetFeatureClass( "TacticalObjectArea" ), asn );
}
