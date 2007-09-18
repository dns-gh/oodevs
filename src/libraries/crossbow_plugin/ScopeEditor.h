// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_ScopeEditor_h_
#define __crossbow_ScopeEditor_h_

#include "ESRI.h"
#include "game_asn/Asn.h"

namespace dispatcher
{
    class Model;
}

namespace crossbow
{
    class ReportFactory;
    class FolkManager;

// =============================================================================
/** @class  ScopeEditor
    @brief  ScopeEditor
*/
// Created: JCR 2007-04-30
// =============================================================================
class ScopeEditor
{
public:
    //! @name Constructors/Destructor
    //@{
             ScopeEditor( const dispatcher::Model& model, const ReportFactory& reportFactory, const FolkManager& folk );
    virtual ~ScopeEditor();
    //@}

    //! @name Operators
    //@{
    void StartEditing( IWorkspacePtr spWorkspace, ISpatialReferencePtr spSpatialReference );
    void StartEdit();
    void StopEdit();
    void StopEditing();
    bool Clear( IFeatureClassPtr spFeatureClass );
    template< typename ASN1T_MsgCreation >
    bool Insert( IFeatureClassPtr spFeatureClass, const ASN1T_MsgCreation& asn );
    template< typename ASN1T_MsgCreation >
    bool Insert( ITablePtr spTable, const ASN1T_MsgCreation& msg );
    //@}

    //! @name Update operators
    //@{
    bool Delete( IFeatureClassPtr spFeatureClass, ASN1T_OID oid );
    template< typename Message >
    bool Update( IFeatureClassPtr spFeatureClass, const Message& message );
    bool Update( IFeatureClassPtr spFeatureClass, const ASN1T_MsgFolkGraphEdgeUpdate& msg );
    void Write( const ASN1T_MsgUnitAttributes& msg );
    void Write( const ASN1T_MsgUnitKnowledgeUpdate& msg );
    void Write( const ASN1T_MsgObjectUpdate& msg );    
    void Write( const ASN1T_MsgLimaUpdate& msg );
    void Write( const ASN1T_MsgFolkGraphEdgeUpdate& msg );
    void FlushPopulation( IFeatureClassPtr spFeatureClass );
	void Flush();
    //@}

    //! @name
    //@{
    template< typename IContainerTypePtr, typename VariantType >
    HRESULT Write( IContainerTypePtr spFeature, BSTR name, VariantType type );
    template< typename FeatureType >
    HRESULT WriteGeometry( FeatureType spFeature, IGeometryPtr geometry );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ScopeEditor( const ScopeEditor& );            //!< Copy constructor
    ScopeEditor& operator=( const ScopeEditor& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ThrowError();
    IFeatureCursorPtr UpdateCursor( IFeatureClassPtr spFeatureClass, std::string idField, ASN1T_OID oid );
    void Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgUnitCreation& msg );
    void Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgUnitKnowledgeCreation& msg );
    void Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgLimitCreation& msg );
    void Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgLimaCreation& msg );
    void Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgObjectCreation& msg );
    void Write( IRowBufferPtr spBuffer, const ASN1T_MsgReport& msg );
    void Write( IRowBufferPtr spBuffer, const ASN1T_MsgFormationCreation& asn );
    void Write( IRowBufferPtr spBuffer, const ASN1T_MsgAutomatCreation& asn );
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model&   model_;
    const ReportFactory&       reportFactory_;
    const FolkManager&         folk_;
    IFeaturePtr                spFeature_;
    IWorkspaceEditPtr          spWorkspaceEdit_;
    ISpatialReferencePtr       spSpatialReference_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
template< typename Message >
bool ScopeEditor::Update( IFeatureClassPtr spFeatureClass, const Message& message )
{
    IFeatureCursorPtr spCursor = UpdateCursor( spFeatureClass, "Public_OID", message.oid );
    if( spCursor )
        spCursor->NextFeature( &spFeature_ );
    return spFeature_ != NULL;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Insert
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
template< typename ASN1T_MsgCreation >
bool ScopeEditor::Insert( IFeatureClassPtr spFeatureClass, const ASN1T_MsgCreation& msg )
{
    CComVariant         varID;
    IFeatureCursorPtr   spCursor;
    IFeatureBufferPtr   spBuffer;

    spFeatureClass->Insert( VARIANT_TRUE, &spCursor );
    spFeatureClass->CreateFeatureBuffer( &spBuffer );
    Write( spBuffer, msg );
    if( !FAILED( spCursor->InsertFeature( spBuffer, &varID ) ) )
        spFeatureClass->GetFeature( varID.lVal , &spFeature_ );
    else
        ThrowError();
    return true;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Insert
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
template< typename ASN1T_MsgCreation >
bool ScopeEditor::Insert( ITablePtr spTable, const ASN1T_MsgCreation& msg )
{
    CComVariant     varID;    
    IRowPtr         spRow;
        
    spTable->CreateRow( &spRow );    
    Write( spRow, msg );    
    spRow->Store();
    return true;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-02
// -----------------------------------------------------------------------------
template< typename IContainerTypePtr, typename VariantType >
HRESULT ScopeEditor::Write( IContainerTypePtr spContainer, BSTR name, VariantType info )
{
    IFieldsPtr  spFlds;
    long        id;
    HRESULT     res;

    spContainer->get_Fields( &spFlds );
    if( ! FAILED( res = spFlds->FindField( name, &id ) ) )
        spContainer->put_Value( id, CComVariant( info ) );
    else
        ThrowError();
    return res;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::WritePosition
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
template< typename FeatureType >
HRESULT ScopeEditor::WriteGeometry( FeatureType spFeature, IGeometryPtr spGeometry )
{
    HRESULT     res;
    if( FAILED( res = spFeature->putref_Shape( spGeometry ) ) )
        ThrowError();
    return res;
}

}

#endif // __crossbow_ScopeEditor_h_
