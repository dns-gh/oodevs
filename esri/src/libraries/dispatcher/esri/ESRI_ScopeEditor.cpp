// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ESRI_ScopeEditor.h"
#include "AgentType.h"
#include "ESRI_World.h"

using namespace dispatcher;
using namespace esri;

// -----------------------------------------------------------------------------
// Name: ScopeEditor constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ScopeEditor::ScopeEditor( IWorkspacePtr spWorkspace, const kernel::Resolver_ABC< kernel::AgentType >& resolver, const World& world )
    : spWorkspaceEdit_  ( spWorkspace )
    , resolver_         ( resolver )
    , world_            ( world )
{     
    spWorkspaceEdit_->StartEditing( VARIANT_TRUE );
    spWorkspaceEdit_->StartEditOperation(); 
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ScopeEditor::~ScopeEditor()
{
    // Flush();
    spWorkspaceEdit_->StopEditOperation();
    spWorkspaceEdit_->StopEditing( VARIANT_TRUE );
}

// -----------------------------------------------------------------------------
// Name: ESRI_ScopeEditor::Clear
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
bool ScopeEditor::Clear( IFeatureClassPtr spFeatureClass )
{
    IFeatureCursorPtr   spCursor;

    spFeatureClass->Insert( true, &spCursor );
    spFeatureClass->Search( 0, VARIANT_TRUE, &spCursor );    
    spCursor->NextFeature( &spFeature_ );
    while ( spFeature_ )
    {
        spFeature_->Delete();
        spCursor->DeleteFeature();
        spCursor->Flush();
        spCursor->NextFeature( &spFeature_ );        
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Insert
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
bool ScopeEditor::Insert( IFeatureClassPtr spFeatureClass, ASN1T_OID oid )
{
    BSTR        oidName;          
    long        oidField;
    
    if ( spFeatureClass )
    {
        spFeatureClass->CreateFeature( &spFeature_ );
        spFeatureClass->FindField( CComBSTR( L"Public_OID" ), &oidField );
        spFeature_->put_Value( oidField, CComVariant( oid ) );        
    }
    return spFeature_ != NULL;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
bool ScopeEditor::Update( IFeatureClassPtr spFeatureClass, ASN1T_OID oid )
{           
    IFeatureCursorPtr   spCursor;
    IQueryFilterPtr spQueryFilter;
    
    spQueryFilter.CreateInstance( CLSID_QueryFilter );
    spFeatureClass->Insert( true, &spCursor );

    std::stringstream ss;
    ss << "Public_OID = " << oid;
    spQueryFilter->put_WhereClause( CComBSTR( ss.str().c_str() ) );
    
    spFeatureClass->Search( spQueryFilter, VARIANT_TRUE, &spCursor );
    spCursor->NextFeature( &spFeature_ ); 
    return spFeature_ != NULL;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Flush
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Flush()
{
    if ( spFeature_ )
        spFeature_->Store();    
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgLimaCreation& /*msg*/ )
{
     // $$$$ JCR 2007-05-03: TODO
}   
    
// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgLimitCreation& /*msg*/ )
{
    // $$$$ JCR 2007-05-03: TODO
}
    
// -----------------------------------------------------------------------------
// Name: ESRI_ScopeEditor::BuildSymbol
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
std::string ScopeEditor::BuildSymbol( const std::string& symbol ) const
{
    // 15
    const std::string::size_type npos = symbol.find_first_of( '/' );
    std::string symbolName( ::upper( symbol ), npos + 1, symbol.size() - npos );
        
    std::replace( symbolName.begin(), symbolName.end(), '*', 'H' );    
    symbolName.append( 15 - symbolName.size(), '-' );    
    return symbolName;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
/** @param  msg 
*/
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgPionCreation& msg )
{
    const kernel::AgentType& aType( resolver_.Get( msg.type_pion ) );    
        
    Write( spFeature_, CComBSTR( L"Name" ), msg.nom );    
    Write( spFeature_, CComBSTR( L"Symbol_ID" ), BuildSymbol( aType.GetSymbol() ).c_str() );
}
    
// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgUnitAttributes& msg )
{
    WritePosition( spFeature_, msg.position );
}
    
// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgObjectCreation& /*msg*/ )
{
    // $$$$ JCR 2007-05-03: TODO
}
    
// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::Write( const ASN1T_MsgObjectUpdate& /*msg*/ )
{
    // $$$$ JCR 2007-05-03: TODO
}

// -----------------------------------------------------------------------------
// Name: ESRI_ScopeEditor::ConvertCoordMosToSim
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void ScopeEditor::ConvertCoordMosToSim( const ASN1T_CoordUTM& asnCoordUTM, MT_Vector2D& vSimPos ) const
{
    std::string strMgrs( (const char*)asnCoordUTM.data, 15 );
    
    world_.MosToSimMgrsCoord( strMgrs, vSimPos );  
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::WritePosition
// Created: JCR 2007-05-02
// -----------------------------------------------------------------------------
void ScopeEditor::WritePosition( IFeaturePtr spFeature, const ASN1T_CoordUTM& asnCoordUTM )
{
    MT_Vector2D     vPosition;
    IPointPtr       spPoint;

    ConvertCoordMosToSim( asnCoordUTM, vPosition );    
    ValidateGeometry( spFeature, spPoint );    
    spPoint->put_X( vPosition.rX_ );
    spPoint->put_X( vPosition.rY_ );
    spPoint->put_Z( 0. );
    spFeature->putref_Shape( spPoint );    
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::ValidateGeometry
// Created: JCR 2007-05-02
// -----------------------------------------------------------------------------
void ScopeEditor::ValidateGeometry( IFeaturePtr spFeature, IPointPtr& spPoint )
{
    IGeometryPtr    spGeometry;
    
    if ( FAILED( spFeature->get_Shape( &spGeometry ) ) )            
        spPoint.CreateInstance( CLSID_Point );        
    else
        spGeometry->QueryInterface( IID_IPoint, (LPVOID*)&spPoint );
    // throw ?
}
