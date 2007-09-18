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
    VARIANT_BOOL editing;
    if( !FAILED( spWorkspaceEdit_->IsBeingEdited( &editing ) ) && editing == VARIANT_TRUE )
    {
        StopEdit();
        StopEditing();
    }
    spWorkspaceEdit_ = 0;
    spSpatialReference_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::StartEdit
// Created: JCR 2007-05-11
// -----------------------------------------------------------------------------
void ScopeEditor::StartEditing( IWorkspacePtr spWorkspace, ISpatialReferencePtr spSpatialReference )
{
    if( FAILED( spWorkspace->QueryInterface( IID_IWorkspaceEdit, (LPVOID*)&spWorkspaceEdit_ ) ) || spWorkspaceEdit_ == 0 )
        return;
    spSpatialReference_ = spSpatialReference;
    VARIANT_BOOL editing;
    if( !FAILED( spWorkspaceEdit_->IsBeingEdited( &editing ) ) && editing == VARIANT_FALSE )
        spWorkspaceEdit_->StartEditing( VARIANT_FALSE );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::StartEdit
// Created: JCR 2007-09-11
// -----------------------------------------------------------------------------
void ScopeEditor::StartEdit()
{
    spWorkspaceEdit_->StartEditOperation();
}
    
// -----------------------------------------------------------------------------
// Name: ScopeEditor::StopEdit
// Created: JCR 2007-09-11
// -----------------------------------------------------------------------------
void ScopeEditor::StopEdit()
{
    spWorkspaceEdit_->StopEditOperation();
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::StopEdit
// Created: JCR 2007-05-11
// -----------------------------------------------------------------------------
void ScopeEditor::StopEditing()
{
    if ( spWorkspaceEdit_ )
        spWorkspaceEdit_->StopEditing( VARIANT_TRUE );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Clear
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
bool ScopeEditor::Clear( IFeatureClassPtr spFeatureClass )
{
    spWorkspaceEdit_->StartEditOperation();
    ITablePtr table;
    if( !FAILED( spFeatureClass.QueryInterface( IID_ITable, &table ) ) )
    {
        IQueryFilterPtr filter( CLSID_QueryFilter );
        filter->put_WhereClause( CComBSTR( "1" ) );
        table->DeleteSearchedRows( filter );
    }
    spWorkspaceEdit_->StopEditOperation();
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
    MT_LOG_ERROR_MSG( szFinal ); // $$$$ SBO 2007-08-24: should throw
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::UpdateCursor
// Created: JCR 2007-05-24
// -----------------------------------------------------------------------------
IFeatureCursorPtr ScopeEditor::UpdateCursor( IFeatureClassPtr spFeatureClass, std::string idField, ASN1T_OID oid )
{
    std::stringstream ss;
    ss << idField << "=" << oid;

    IFeatureCursorPtr spCursor;
    IQueryFilterPtr spQueryFilter( CLSID_QueryFilter );

    spQueryFilter->put_WhereClause( CComBSTR( ss.str().c_str() ) );
    spFeatureClass->Update( spQueryFilter, VARIANT_TRUE, &spCursor );
    return spCursor;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
bool ScopeEditor::Update( IFeatureClassPtr spFeatureClass, const ASN1T_MsgFolkGraphEdgeUpdate& asn )
{    
    IFeatureCursorPtr spCursor = UpdateCursor( spFeatureClass, "LINK_ID", asn.oid );
    if( spCursor )
        spCursor->NextFeature( &spFeature_ );    
    return spFeature_ != NULL;
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
// Name: ScopeEditor::FlushPopulation
// Created: JCR 2007-08-31
// -----------------------------------------------------------------------------
void ScopeEditor::FlushPopulation( IFeatureClassPtr spFeatureClass )
{
	IFeatureCursorPtr   spCursor;
	IQueryFilterPtr		spNullQuery;
	FolkEditor			folk( *this, folk_ );

    spWorkspaceEdit_->StartEditOperation();
	spFeatureClass->Update( spNullQuery, VARIANT_TRUE, &spCursor );
	if( spCursor )
		folk.Write( spCursor );	
    spWorkspaceEdit_->StopEditOperation();
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Flush
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Flush()
{    
    if( spFeature_ )
    {
        spWorkspaceEdit_->StartEditOperation();
        spFeature_->Store();
        spWorkspaceEdit_->StopEditOperation();
    }
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
    Write( spRow, CComBSTR( L"Parent_OID" ), asn.oid_parent.t == T_MsgAutomatCreation_oid_parent_formation ? asn.oid_parent.u.formation : asn.oid_parent.u.automate );
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
void ScopeEditor::Write( const ASN1T_MsgObjectUpdate& asn )
{
    // $$$$ JCR 2007-05-03: TODO
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgLimaUpdate& msg )
{
    // $$$$ SBO 2007-08-30: TODO
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgFolkGraphEdgeUpdate& asn )
{
    FolkEditor( *this, folk_ ).Write( spFeature_, asn );
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
