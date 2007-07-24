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
#include "Connector.h"
#include "PositionEditor.h"
#include "LineEditor.h"
#include "LocationEditor.h"
#include "SymbolEditor.h"
#include "AgentType.h"
#include "AutomatType.h"
#include "dispatcher/ModelWalker.h"
#include "dispatcher/Model.h"
#include "dispatcher/ModelVisitors.h"

using namespace dispatcher;
using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: ScopeEditor constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ScopeEditor::ScopeEditor( Connector& connector, const kernel::Resolver_ABC< kernel::AgentType >& agentResolver, const kernel::Resolver_ABC< kernel::AutomatType >& automatResolver )
    : connector_        ( connector )
    , agentResolver_    ( agentResolver )
    , automatResolver_  ( automatResolver )
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

namespace
{
    std::string GetString( BSTR bstrIN )
    {
        TCHAR       szFinal[ 255 ];

        // direct conversion from BSTR to LPCTSTR only works in Unicode
        // you have to go through _bstr_t to have it work in ANSI and Unicode
        _stprintf( szFinal, _T( "%s" ), ( LPCTSTR )_bstr_t( bstrIN ) );
        return std::string( szFinal );
    }

    void check_error()
    {
        IErrorInfoPtr ipError;
        BSTR          strError;

        ::GetErrorInfo( 0, &ipError );
        ipError->GetDescription( &strError );
        std::cerr << GetString( strError ) << std::endl;
    }
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::CheckError
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ScopeEditor::CheckError()
{
    check_error();
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::UpdateCursor
// Created: JCR 2007-05-24
// -----------------------------------------------------------------------------
IFeatureCursorPtr ScopeEditor::UpdateCursor( IFeatureClassPtr spFeatureClass, ASN1T_OID oid )
{
    IFeatureCursorPtr   spCursor;
    IQueryFilterPtr     spQueryFilter;

    std::stringstream ss;
    ss << "Public_OID = " << oid;

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
    IFeatureCursorPtr   spCursor = UpdateCursor( spFeatureClass, oid );

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
    IFeatureCursorPtr   spCursor = UpdateCursor( spFeatureClass, oid );

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
    {
        SymbolEditor    editor( *this, connector_.GetModel() );
        editor.Write( spRow, asn );
    }
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
    {
        SymbolEditor    editor( *this, connector_.GetModel() );
        editor.Write( spRow, asn, automatResolver_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ScopeEditor::Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgLimaCreation& asn )
{
    Write( spBuffer, CComBSTR( L"Public_OID" ), asn.oid );
    Write( spBuffer, CComBSTR( L"Name" ), asn.tactical_line.name );
    {
        SymbolEditor    editor( *this, connector_.GetModel() );
        editor.Write( spBuffer, asn );
    }
    {
        LineEditor  editor( *this, spSpatialReference_ );
        editor.CreateGeometry( spBuffer, asn.tactical_line.geometry );
    }
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ScopeEditor::Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgLimitCreation& asn )
{
    Write( spBuffer, CComBSTR( L"Public_OID" ), asn.oid );
    Write( spBuffer, CComBSTR( L"Name" ), asn.tactical_line.name );
    {
        SymbolEditor    editor( *this, connector_.GetModel() );
        editor.Write( spBuffer, asn );
    }
    {
        LineEditor  editor( *this, spSpatialReference_ );
        editor.CreateGeometry( spBuffer, asn.tactical_line.geometry );
    }
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
    {
        SymbolEditor    editor( *this, connector_.GetModel() );
        editor.Write( spBuffer, asn, agentResolver_ );
    }
    {
        PositionEditor  editor( *this, spSpatialReference_ );
        editor.CreateGeometry( spBuffer );
    }
    {
        ModelWalker walker( connector_.GetModel() );
        FormationModelVisitor   visitor( walker );
        walker.WalkTo( visitor, asn );
        Write( spBuffer, CComBSTR( L"Formation_OID" ), visitor.GetOID() );
    }
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
    {
        PositionEditor editor( *this, spSpatialReference_ );
        editor.Write( spFeature_, asn.position );
    }
}

namespace
{
    std::string GetDiplomatie( ASN1T_EnumDiplomacy diplomatie )
    {
        const char cDiplomatie[ 4 ] = { 'U' /*Unkown*/, 'F' /*Friend*/, 'H' /*Hostile*/, 'N' /*Neutral*/};
        return std::string( 1, cDiplomatie[ ( unsigned )diplomatie ] );
    }
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
    {
        ModelWalker         walker( connector_.GetModel() );
        SideModelVisitor    visitor( walker );
        walker.WalkTo( visitor, asn );
        Write( spBuffer, CComBSTR( L"ObserverAffiliation" ), GetDiplomatie( visitor.GetSide() ).c_str() );
    }
    {
        SymbolEditor    editor( *this, connector_.GetModel() );
        editor.Write( spBuffer, asn );
    }
    {
        PositionEditor  editor( *this, spSpatialReference_ );
        editor.CreateGeometry( spBuffer );
    }
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
    {
        SymbolEditor    editor( *this, connector_.GetModel() );
        editor.Write( spFeature_, asn, agentResolver_ );
    }
    if( asn.m.positionPresent )
    {
        PositionEditor editor( *this, spSpatialReference_ );
        editor.Write( spFeature_, asn.position );
    }
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void ScopeEditor::Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgObjectCreation& asn )
{
    Write( spBuffer, CComBSTR( L"Public_OID" ), asn.oid );
    Write( spBuffer, CComBSTR( L"Name" ), asn.name );
    {
        SymbolEditor    editor( *this, connector_.GetModel() );
        editor.Write( spBuffer, asn );
    }
    switch ( asn.location.type )
    {
    case EnumLocationType::point:
        {
            PositionEditor  editor( *this, spSpatialReference_ );
            editor.CreateGeometry( spBuffer, asn.location.coordinates.elem[ 0 ] );
            break;
        }
    case EnumLocationType::line:
        {
            LineEditor  editor( *this, spSpatialReference_ );
            editor.CreateGeometry( spBuffer, asn.location.coordinates );
            break;
        }
    default:
        {
            LocationEditor  editor( *this, spSpatialReference_ );
            editor.CreateGeometry( spBuffer, asn.location );
        }
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
void ScopeEditor::Write( IFeatureBufferPtr /*spBuffer*/, const ASN1T_MsgReport& /*asn*/ )
{
    // $$$$ JCR 2007-05-03: TODO
}
