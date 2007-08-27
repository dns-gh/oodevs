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
#include "dispatcher/Model.h"
#include "dispatcher/Config.h"
#include "dispatcher/PluginConfig.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: Connector constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
Connector::Connector( const dispatcher::Config& config, const dispatcher::Model& model )
    : pScopeEditor_( 0 )
    , model_( model )
    , reportFactory_( new ReportFactory( config, model ) )
{
    ::CoInitialize( NULL ); // Initialize COM

// #if ! defined( _ARCGIS_VERSION_ )  // $$$$ JCR 2007-06-14: do not use license checking on arcgis version lower than 9.2
    CheckOutLicences( esriLicenseProductCodeArcInfo );
// #endif

    const dispatcher::PluginConfig& pluginConfig = config.GetPluginConfig( "crossbow" );
    ConnectToGeodatabase( config.BuildGameChildFile( pluginConfig.GetParameter( "geodatabase" ) ) );
}

// -----------------------------------------------------------------------------
// Name: Connector destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
Connector::~Connector()
{
    if( IsLocked() )
        Unlock();
    if( spLicenseHandler_ )
        spLicenseHandler_->Shutdown();
    //::CoUninitialize(); // Release COM // $$$$ SBO 2007-06-12:
}

// -----------------------------------------------------------------------------
// Name: Connector::CheckOutLicences
// Created: JCR 2007-06-14
// -----------------------------------------------------------------------------
void Connector::CheckOutLicences( esriLicenseProductCode eProcuct )
{
    esriLicenseStatus status;
    if( FAILED( spLicenseHandler_.CreateInstance( CLSID_AoInitialize ) ) )
        throw std::runtime_error( "Unable to initialize license manager" );

    if( FAILED( spLicenseHandler_->IsProductCodeAvailable( eProcuct, &status ) ) )
        throw std::runtime_error( "No product available" );

    if( FAILED( spLicenseHandler_->Initialize( esriLicenseProductCodeArcInfo, &status ) ) )
        throw std::runtime_error( "Unable to retrieve license information" );
}

// -----------------------------------------------------------------------------
// Name: Connector::ConnectToGeodatabase
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::ConnectToGeodatabase( const std::string& geodatabase )
{
    IWorkspaceFactoryPtr spWorkspaceFactory;
    if( FAILED( spWorkspaceFactory.CreateInstance( CLSID_AccessWorkspaceFactory ) ) )
        throw std::runtime_error( "Unable to create Database access from CLSID_AccessWorkspaceFactory id." );

    // Connect to Geodatabase
    if( FAILED( spWorkspaceFactory->OpenFromFile( CComBSTR( geodatabase.c_str() ), NULL, &spWorkspace_ ) ) )
        throw std::runtime_error( "Unable connect to geodatabase: " + geodatabase );

    // QI for the IFeatureWorkspace
    if( FAILED( spWorkspace_->QueryInterface( IID_IFeatureWorkspace, (LPVOID*)&spFeatureWorkspace_ ) ) )
        throw std::runtime_error( "Cannot retrieve IFeatureWorkspace interface." );

    LoadStatialReference( "vmap" );
}

// -----------------------------------------------------------------------------
// Name: Connector::LoadStatialReference
// Created: JCR 2007-05-11
// -----------------------------------------------------------------------------
void Connector::LoadStatialReference( const std::string& feature )
{
    IFeatureDatasetPtr  spFeatureDataset;
    IGeoDatasetPtr      spGeodataset;

    spFeatureWorkspace_->OpenFeatureDataset( CComBSTR( feature.c_str() ), &spFeatureDataset );
    spFeatureDataset->QueryInterface( IID_IGeoDataset, (LPVOID*)&spGeodataset );
    spGeodataset->get_SpatialReference( &spSpatialReference_ );
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
IFeatureClassPtr Connector::LoadFeatureClass( const std::string& feature, bool clear )
{
    IFeatureClassPtr spFeatureClass;

    if( FAILED( spFeatureWorkspace_->OpenFeatureClass( CComBSTR( feature.c_str() ), &spFeatureClass ) ) )
        check_error();
    // $$$$ SBO 2007-05-30: throw if feature not found
    if( spFeatureClass != 0 && clear )
        ClearFeatureClass( spFeatureClass );
    return spFeatureClass;
}

// -----------------------------------------------------------------------------
// Name: Connector::ClearFeatureClass
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void Connector::ClearFeatureClass( IFeatureClassPtr spFeatureClass )
{
    ScopeEditor( model_, *reportFactory_ ).Clear( spFeatureClass );
}

// -----------------------------------------------------------------------------
// Name: Connector::GetFeatureClass
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
IFeatureClassPtr Connector::GetFeatureClass( const std::string& feature )
{
    IFeatureClassPtr& spFeatureClass = features_[ feature ];
    if( spFeatureClass == NULL )
        features_[ feature ] = LoadFeatureClass( feature, true );
    return spFeatureClass;
}

// -----------------------------------------------------------------------------
// Name: Connector::GetTable
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
ITablePtr Connector::GetTable( const std::string& name )
{
    ITablePtr spTable = tables_[ name ];
    if( spTable == NULL )
    {
        if( FAILED( spFeatureWorkspace_->OpenTable( CComBSTR( name.c_str() ), &spTable ) ) )
        {
            check_error();
            throw std::runtime_error( "cannot open" + name );
        }
        IQueryFilterPtr spFilter;
        spFilter.CreateInstance( CLSID_QueryFilter );
        spFilter->put_WhereClause( CComBSTR( "1" ) );
        spTable->DeleteSearchedRows( spFilter );
        tables_[ name ] = spTable;
    }
    return spTable;
}

// -----------------------------------------------------------------------------
// Name: Connector::Lock
// Created: JCR 2007-05-11
// -----------------------------------------------------------------------------
void Connector::Lock()
{
    pScopeEditor_ = new ScopeEditor( model_, *reportFactory_ );
    pScopeEditor_->StartEdit( spWorkspace_, spSpatialReference_ );
}

// -----------------------------------------------------------------------------
// Name: Connector::Unlock
// Created: JCR 2007-05-11
// -----------------------------------------------------------------------------
void Connector::Unlock()
{
    assert( pScopeEditor_ );
    pScopeEditor_->StopEdit();
    delete pScopeEditor_;
    pScopeEditor_ = 0;
}

// -----------------------------------------------------------------------------
// Name: Connector::Islock
// Created: JCR 2007-05-14
// -----------------------------------------------------------------------------
bool Connector::IsLocked() const
{
    return pScopeEditor_ != 0;
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
// Name: Connector::Insert
// Created: JCR 2007-05-24
// -----------------------------------------------------------------------------
template< typename ASN_Type >
void Connector::Insert( IFeatureClassPtr spFeature, const ASN_Type& asn )
{
    if( pScopeEditor_ )
        pScopeEditor_->Insert( spFeature, asn );
}

// -----------------------------------------------------------------------------
// Name: Connector::Insert
// Created: JCR 2007-06-11
// -----------------------------------------------------------------------------
template< typename ASN_Type >
void Connector::Insert( ITablePtr spTable, const ASN_Type& asn )
{
    if( pScopeEditor_ )
        pScopeEditor_->Insert( spTable, asn );
}

// -----------------------------------------------------------------------------
// Name: Connector::Delete
// Created: JCR 2007-05-24
// -----------------------------------------------------------------------------
template< typename ASN_Type >
void Connector::Delete( IFeatureClassPtr spFeature, const ASN_Type& asn_oid )
{
    if( pScopeEditor_ )
        pScopeEditor_->Delete( spFeature, asn_oid );
}

// -----------------------------------------------------------------------------
// Name: Connector::Update
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::Send( const ASN1T_MsgsSimToClient& asn )
{
    bool bForceLock = !IsLocked();

    if( bForceLock )
        Lock();
    switch ( asn.msg.t )
    {
    case T_MsgsSimToClient_msg_msg_formation_creation:  Insert( GetTable( "Formations" ), *asn.msg.u.msg_formation_creation ); break;
    case T_MsgsSimToClient_msg_msg_automat_creation:    Insert( GetTable( "Formations" ), *asn.msg.u.msg_automat_creation ); break;

    case T_MsgsSimToClient_msg_msg_lima_creation:       Insert( GetFeatureClass( "TacticalLines" ), *asn.msg.u.msg_lima_creation ); break;
    case T_MsgsSimToClient_msg_msg_lima_update:         Send( GetFeatureClass( "TacticalLines" ), *asn.msg.u.msg_lima_update ); break;
    case T_MsgsSimToClient_msg_msg_lima_destruction:    Delete( GetFeatureClass( "TacticalLines" ), asn.msg.u.msg_lima_destruction ); break;

    case T_MsgsSimToClient_msg_msg_limit_creation:      Insert( GetFeatureClass( "BoundaryLimits" ), *asn.msg.u.msg_limit_creation ); break;
    case T_MsgsSimToClient_msg_msg_limit_destruction:   Delete( GetFeatureClass( "BoundaryLimits" ), asn.msg.u.msg_lima_destruction ); break;

    case T_MsgsSimToClient_msg_msg_unit_creation:       Insert( GetFeatureClass( "UnitForces" ), *asn.msg.u.msg_unit_creation ); break;
    case T_MsgsSimToClient_msg_msg_unit_attributes:     Send( GetFeatureClass( "UnitForces" ), *asn.msg.u.msg_unit_attributes ); break;
    case T_MsgsSimToClient_msg_msg_unit_destruction:    Delete( GetFeatureClass( "UnitForces" ), asn.msg.u.msg_unit_destruction ); break;

    case T_MsgsSimToClient_msg_msg_unit_knowledge_creation:     Insert( GetFeatureClass( "KnowledgeUnits" ), *asn.msg.u.msg_unit_knowledge_creation ); break;
    case T_MsgsSimToClient_msg_msg_unit_knowledge_update:       Send( GetFeatureClass( "KnowledgeUnits" ), *asn.msg.u.msg_unit_knowledge_update ); break;
    case T_MsgsSimToClient_msg_msg_unit_knowledge_destruction:  Delete( GetFeatureClass( "KnowledgeUnits" ), asn.msg.u.msg_unit_knowledge_destruction->oid );  break;

    case T_MsgsSimToClient_msg_msg_object_creation:     Send( *asn.msg.u.msg_object_creation ); break;
    case T_MsgsSimToClient_msg_msg_object_update:       Send( *asn.msg.u.msg_object_update ); break;
    case T_MsgsSimToClient_msg_msg_object_destruction:  Send( asn.msg.u.msg_object_destruction ); break;
    case T_MsgsSimToClient_msg_msg_report:              Send( *asn.msg.u.msg_report ); break;
    }
    if( bForceLock )
        Unlock();
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
// Name: Connector::Send
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void Connector::Send( IFeatureClassPtr /*pFeatureClass*/, const ASN1T_MsgLimaUpdate& /*msg*/ )
{
    // $$$$ AGE 2007-08-27: WTF
}

// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::Send( IFeatureClassPtr pFeatureClass, const ASN1T_MsgUnitAttributes& msg )
{
    if( pScopeEditor_ && pScopeEditor_->Update( pFeatureClass, msg.oid ) )
    {
        pScopeEditor_->Write( msg );
        pScopeEditor_->Flush();
    }
}

// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::Send( IFeatureClassPtr pFeatureClass, const ASN1T_MsgUnitKnowledgeUpdate& msg )
{
    if( pScopeEditor_ && pScopeEditor_->Update( GetFeatureClass( "KnowledgeUnits" ), msg.oid ) )
    {
        pScopeEditor_->Write( msg );
        pScopeEditor_->Flush();
    }
}

// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: JCR 2007-05-24
// -----------------------------------------------------------------------------
void Connector::Send( const ASN1T_MsgObjectDestruction& asn )
{
    if( pScopeEditor_ )
    {
        pScopeEditor_->Delete( GetFeatureClass( "TacticalObjectPoint" ), asn );
        pScopeEditor_->Delete( GetFeatureClass( "TacticalObjectLine" ), asn );
        pScopeEditor_->Delete( GetFeatureClass( "TacticalObjectArea" ), asn );
    }
}

// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::Send( const ASN1T_MsgObjectCreation& msg )
{
    switch ( msg.location.type )
    {
    case EnumLocationType::point:
        pScopeEditor_->Insert( GetFeatureClass( "TacticalObjectPoint" ), msg );
        break;
    case EnumLocationType::line:
        pScopeEditor_->Insert( GetFeatureClass( "TacticalObjectLine" ), msg );
        break;
    default:
        pScopeEditor_->Insert( GetFeatureClass( "TacticalObjectArea" ), msg );
    }
}

// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Connector::Send( const ASN1T_MsgObjectUpdate& msg )
{
    ScopeEditor& editor = *pScopeEditor_;
    bool         bResult = false;

    switch ( msg.location.type )
    {
    case EnumLocationType::point:
        bResult = editor.Update( GetFeatureClass( "TacticalObjectPoint" ), msg.oid );
        break;
    case EnumLocationType::line:
        bResult = editor.Update( GetFeatureClass( "TacticalObjectLine" ), msg.oid );
        break;
    default:
        bResult = editor.Update( GetFeatureClass( "TacticalObjectArea" ), msg.oid );
    }

    if( bResult )
    {
        editor.Write( msg );
        editor.Flush();
    }
}

// -----------------------------------------------------------------------------
// Name: Connector::Send
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void Connector::Send( const ASN1T_MsgReport& msg )
{
    if( pScopeEditor_ )
        pScopeEditor_->Insert( GetTable( "Reports" ), msg );
}
