// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ScopeEditor_h_
#define __ScopeEditor_h_

#include "ESRI.h"
#include "game_asn/Asn.h"
#include "Resolver_ABC.h"

namespace kernel
{
    class AgentType;
    class AutomatType;
}

namespace crossbow
{
    class Connector;

// =============================================================================
/** @class  ScopeEditor
    @brief  ScopeEditor
*/
// Created: JCR 2007-04-30
// =============================================================================
class ScopeEditor
{
    friend class Connector;

public:
    //! @name Constructors/Destructor
    //@{
             ScopeEditor( Connector& connector/*, IdBuffer& buffer_*/, const kernel::Resolver_ABC< kernel::AgentType >& agentResolver, const kernel::Resolver_ABC< kernel::AutomatType >& automatResolver );
    virtual ~ScopeEditor();
    //@}

    //! @name Operators
    //@{
    bool Clear( IFeatureClassPtr spFeatureClass );
    template< typename ASN_Type >
    bool Insert( IFeatureClassPtr spFeatureClass, const ASN_Type& asn );
    template< typename ASN_Type >
    bool Insert( ITablePtr spTable, const ASN_Type& msg );
    //@}

    //! @name Update operators
    //@{
    bool Delete( IFeatureClassPtr spFeatureClass, ASN1T_OID oid );
    bool Update( IFeatureClassPtr spFeatureClass, ASN1T_OID oid );
    void Write( const ASN1T_MsgUnitAttributes& msg );
    void Write( const ASN1T_MsgUnitKnowledgeUpdate& msg );
    void Write( const ASN1T_MsgObjectUpdate& msg );
    void Flush();
    //@}

    //! @name
    //@{
//    template< typename VariantType >
//    HRESULT Write( IFeaturePtr spFeature, BSTR name, VariantType type );
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

    //! @name
    //@{
    void StartEdit( IWorkspacePtr spWorkspace, ISpatialReferencePtr spSpatialReference );
    void StopEdit();
    void CheckError();
    //@}

    //! @name
    //@{
    IFeatureCursorPtr UpdateCursor( IFeatureClassPtr spFeatureClass, ASN1T_OID oid );
    //@}

    //! @name Write features
    //@{
    void Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgUnitCreation& msg );
    void Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgUnitKnowledgeCreation& msg );
    void Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgLimitCreation& msg );
    void Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgLimaCreation& msg );
    void Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgObjectCreation& msg );
    void Write( IFeatureBufferPtr spBuffer, const ASN1T_MsgReport& msg );
    //@}

    //! @name Write rows
    //@{
    void Write( IRowBufferPtr spBuffer, const ASN1T_MsgFormationCreation& asn );
    void Write( IRowBufferPtr spBuffer, const ASN1T_MsgAutomatCreation& asn );
    //@}

private:
    //! @name Member data
    //@{
    Connector&          connector_;
    IFeaturePtr         spFeature_;
    //@}

    //! @name
    //@{
    IWorkspaceEditPtr       spWorkspaceEdit_;
    ISpatialReferencePtr    spSpatialReference_;
    //@}

    //! @name
    //@{
    const kernel::Resolver_ABC< kernel::AgentType >& agentResolver_;
    const kernel::Resolver_ABC< kernel::AutomatType >& automatResolver_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Insert
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
template< typename ASN_Type >
bool ScopeEditor::Insert( IFeatureClassPtr spFeatureClass, const ASN_Type& msg )
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
        CheckError();
    return true;
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Insert
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
template< typename ASN_Type >
bool ScopeEditor::Insert( ITablePtr spTable, const ASN_Type& msg )
{
    CComVariant     varID;
    ICursorPtr      spCursor;
    IRowPtr         spRow;
    IRowBufferPtr   spBuffer;

    // spTable->Insert( VARIANT_TRUE, &spCursor );
    spTable->CreateRow( &spRow );
    // spTable->CreateRowBuffer( &spBuffer );
    Write( spRow, msg );
    // if( FAILED( spCursor->InsertRow( spBuffer, &varID ) ) )
    //     CheckError();
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
        CheckError();
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
        CheckError();
    return res;
}

}

#endif // __ScopeEditor_h_
