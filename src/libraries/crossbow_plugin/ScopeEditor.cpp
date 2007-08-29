// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ScopeEditor.h"
#include "PositionEditor.h"
#include "LineEditor.h"
#include "LocationEditor.h"
#include "SymbolEditor.h"
#include "ReportFactory.h"
#include "FolkEditor.h"
#include "FolkManager.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Model.h"
#include "tools/App6Symbol.h"

using namespace dispatcher;
using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: ScopeEditor constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ScopeEditor::ScopeEditor( const dispatcher::Model& model, const ReportFactory& reportFactory, const FolkManager& folk )
    : model_        ( model )
    , reportFactory_( reportFactory )
    , folk_         ( folk )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ScopeEditor::~ScopeEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::StartEdit
// Created: JCR 2007-05-11
// -----------------------------------------------------------------------------
void ScopeEditor::StartEdit( IWorkspacePtr spWorkspace, ISpatialReferencePtr spSpatialReference )
{
    spWorkspaceEdit_ = spWorkspace;
    spSpatialReference_ = spSpatialReference;
    spWorkspaceEdit_->StartEditing( VARIANT_FALSE );
    spWorkspaceEdit_->StartEditOperation();
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::StopEdit
// Created: JCR 2007-05-11
// -----------------------------------------------------------------------------
void ScopeEditor::StopEdit()
{
    spWorkspaceEdit_->StopEditOperation();
    spWorkspaceEdit_->StopEditing( VARIANT_TRUE );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Clear
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
bool ScopeEditor::Clear( IFeatureClassPtr spFeatureClass )
{
    IFeatureCursorPtr   spCursor;

    if( ! FAILED( spFeatureClass->Update( 0, VARIANT_TRUE, &spCursor ) ) )
    {
        spCursor->NextFeature( &spFeature_ );
        while ( spFeature_ )
        {
            spCursor->DeleteFeature();
            spCursor->NextFeature( &spFeature_ );
        }
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::ThrowError
// Created: SBO 2007-08-24
// -----------------------------------------------------------------------------
void ScopeEditor::ThrowError()
{
    IErrorInfoPtr ipError;
    BSTR strError;
    ::GetErrorInfo( 0, &ipError );
    ipError->GetDescription( &strError );

    TCHAR szFinal[ 255 ];
    _stprintf( szFinal, _T( "%s" ), ( LPCTSTR )_bstr_t( strError ) );
    MT_LOG_ERROR_MSG( szFinal );
    //throw std::exception( szFinal ); // $$$$ SBO 2007-08-24: should throw
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::UpdateCursor
// Created: JCR 2007-05-24
// -----------------------------------------------------------------------------
IFeatureCursorPtr ScopeEditor::UpdateCursor( IFeatureClassPtr spFeatureClass, std::string idField, ASN1T_OID oid )
{
    IFeatureCursorPtr   spCursor;
    IQueryFilterPtr     spQueryFilter;

    std::stringstream ss;
    ss << idField << "=" << oid;

    spQueryFilter.CreateInstance( CLSID_QueryFilter );
    spQueryFilter->put_WhereClause( CComBSTR( ss.str().c_str() ) );
    spFeatureClass->Update( spQueryFilter, VARIANT_TRUE, &spCursor );
    return spCursor;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
bool ScopeEditor::Update( IFeatureClassPtr spFeatureClass, ASN1T_OID oid )
{
    IFeatureCursorPtr   spCursor = UpdateCursor( spFeatureClass, "Public_OID", oid );

    if( spCursor )
        spCursor->NextFeature( &spFeature_ );
    return spFeature_ != NULL;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void ScopeEditor::Update( IFeatureClassPtr spFeatureClass, const ASN1T_MsgFolkGraphEdgeUpdate& asn )
{
    IFeatureCursorPtr   spCursor = UpdateCursor( spFeatureClass, "LINK_ID", asn.oid );
    if( spCursor )
        spCursor->NextFeature( &spFeature_ );
    if ( spFeature_ != NULL )
        FolkEditor( *this, folk_ ).Write( spFeature_, asn );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Clear
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
bool ScopeEditor::Delete( IFeatureClassPtr spFeatureClass, ASN1T_OID oid )
{
    IFeatureCursorPtr   spCursor = UpdateCursor( spFeatureClass, "Public_OID", oid );

    if( spCursor )
    {
        spCursor->NextFeature( &spFeature_ );
        if( spFeature_ )
            spCursor->DeleteFeature();
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Flush
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Flush()
{
    if( spFeature_ )
        spFeature_->Store();
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ScopeEditor::Write( IRowBufferPtr spRow, const ASN1T_MsgFormationCreation& asn )
{
    Write( spRow, CComBSTR( L"Public_OID" ), asn.oid );
    Write( spRow, CComBSTR( L"Parent_OID" ), asn.oid_formation_parente );
    Write( spRow, CComBSTR( L"Name" ), asn.nom  );
    SymbolEditor( *this, model_ ).Write( spRow, asn );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ScopeEditor::Write( IRowBufferPtr spRow, const ASN1T_MsgAutomatCreation& asn )
{
    Write( spRow, CComBSTR( L"Public_OID" ), asn.oid );
    Write( spRow, CComBSTR( L"Parent_OID" ), asn.oid_formation );
    Write( spRow, CComBSTR( L"Name" ), asn.nom );
    SymbolEditor( *this, model_ ).Write( spRow, asn );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ScopeEditor::Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgLimaCreation& asn )
{
    Write( spBuffer, CComBSTR( L"Public_OID" ), asn.oid );
    Write( spBuffer, CComBSTR( L"Name" ), asn.tactical_line.name );
    SymbolEditor( *this, model_ ).Write( spBuffer, asn );
    LineEditor( *this, spSpatialReference_ ).CreateGeometry( spBuffer, asn.tactical_line.geometry );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ScopeEditor::Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgLimitCreation& asn )
{
    Write( spBuffer, CComBSTR( L"Public_OID" ), asn.oid );
    Write( spBuffer, CComBSTR( L"Name" ), asn.tactical_line.name );
    SymbolEditor( *this, model_ ).Write( spBuffer, asn );
    LineEditor( *this, spSpatialReference_ ).CreateGeometry( spBuffer, asn.tactical_line.geometry );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgUnitCreation& asn )
{
    Write( spBuffer, CComBSTR( L"Public_OID" ), asn.oid );
    Write( spBuffer, CComBSTR( L"Parent_OID" ), asn.oid_automate );
    Write( spBuffer, CComBSTR( L"Name" ), asn.nom );
    SymbolEditor( *this, model_ ).Write( spBuffer, asn );
    PositionEditor( *this, spSpatialReference_ ).CreateGeometry( spBuffer );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgUnitAttributes& asn )
{
    if( asn.m.vitessePresent )
        Write( spFeature_, CComBSTR( L"Speed" ), asn.vitesse );
    if( asn.m.etat_operationnelPresent )
        Write( spFeature_, CComBSTR( L"OpsState" ), asn.etat_operationnel_brut );
    if( asn.m.positionPresent )
        PositionEditor( *this, spSpatialReference_ ).Write( spFeature_, asn.position );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void ScopeEditor::Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgUnitKnowledgeCreation& asn )
{
    Write( spBuffer, CComBSTR( L"Public_OID" ), asn.oid );
    Write( spBuffer, CComBSTR( L"Group_OID" ), asn.oid_groupe_possesseur );
    Write( spBuffer, CComBSTR( L"RealUnit_OID" ), asn.oid_unite_reelle );

    if( const dispatcher::Agent* realAgent = model_.GetAgents().Find( asn.oid_unite_reelle ) ) // $$$$ SBO 2007-08-24: snif
        Write( spBuffer, CComBSTR( L"ObserverAffiliation" ), tools::app6::GetAffiliation( realAgent->BuildSymbol() ).c_str() );
    SymbolEditor( *this, model_ ).Write( spBuffer, asn );
    PositionEditor( *this, spSpatialReference_ ).CreateGeometry( spBuffer );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgUnitKnowledgeUpdate& asn )
{
    if( asn.m.speedPresent )
        Write( spFeature_, CComBSTR( L"Speed" ), asn.speed );
    if( asn.m.identification_levelPresent )
        Write( spFeature_, CComBSTR( L"IdentificationLevel" ), asn.identification_level );
    if( asn.m.max_identification_levelPresent )
        SymbolEditor( *this, model_ ).Write( spFeature_, asn );
    if( asn.m.positionPresent )
        PositionEditor( *this, spSpatialReference_ ).Write( spFeature_, asn.position );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void ScopeEditor::Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgObjectCreation& asn )
{
    Write( spBuffer, CComBSTR( L"Public_OID" ), asn.oid );
    Write( spBuffer, CComBSTR( L"Name" ), asn.name );
    SymbolEditor( *this, model_ ).Write( spBuffer, asn );
    switch ( asn.location.type )
    {
    case EnumLocationType::point:
        PositionEditor( *this, spSpatialReference_ ).CreateGeometry( spBuffer, asn.location.coordinates.elem[ 0 ] );
        break;
    case EnumLocationType::line:
        LineEditor( *this, spSpatialReference_ ).CreateGeometry( spBuffer, asn.location.coordinates );
        break;
    default:
        LocationEditor( *this, spSpatialReference_ ).CreateGeometry( spBuffer, asn.location );
    }
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgObjectUpdate& /*asn*/ )
{
    // $$$$ JCR 2007-05-03: TODO
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void ScopeEditor::Write( IRowBufferPtr spBuffer, const ASN1T_MsgReport& asn )
{
    Write( spBuffer, CComBSTR( L"unit_id" ), asn.oid );
    Write( spBuffer, CComBSTR( L"message" ), reportFactory_.CreateMessage( asn ).c_str() );
}
