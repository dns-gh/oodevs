// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ESRI_ScopeEditor_h_
#define __ESRI_ScopeEditor_h_

#include "ESRI.h"
#include "game_asn/Asn.h"
#include "Resolver_ABC.h"

class MT_Vector2D;

namespace kernel
{
    class AgentType;
}

namespace dispatcher
{
namespace esri
{
    class World;

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
    explicit ScopeEditor( IWorkspacePtr spWorkspace/*, IdBuffer& buffer_*/, const kernel::Resolver_ABC< kernel::AgentType >& resolver, const World& world );
    virtual ~ScopeEditor();
    //@}

    //! @name Operators
    //@{
    bool Clear( IFeatureClassPtr spFeatureClass );
    bool Insert( IFeatureClassPtr spFeatureClass, ASN1T_OID oid );
    bool Update( IFeatureClassPtr spFeatureClass, ASN1T_OID oid );
    void Write( const ASN1T_MsgLimaCreation& msg );
    void Write( const ASN1T_MsgLimitCreation& msg );
    void Write( const ASN1T_MsgPionCreation& msg );
    void Write( const ASN1T_MsgUnitAttributes& msg );
    void Write( const ASN1T_MsgObjectCreation& msg );
    void Write( const ASN1T_MsgObjectUpdate& msg );
    void Flush();
    //@}                                   

private:
    //! @name Copy/Assignement
    //@{
    ScopeEditor( const ScopeEditor& );            //!< Copy constructor
    ScopeEditor& operator=( const ScopeEditor& ); //!< Assignement operator
    //@}

    //! @name 
    //@{
    template< typename VariantType >
    void Write( IFeaturePtr spFeature, BSTR name, VariantType type );
    void WritePosition( IFeaturePtr spFeature, const ASN1T_CoordUTM& coord );
    void ValidateGeometry( IFeaturePtr spFeature, IPointPtr& spPoint );
    //@}

    //! @name 
    //@{
    void ConvertCoordMosToSim( const ASN1T_CoordUTM& asnCoordUTM, MT_Vector2D& vSimPos ) const;
    std::string BuildSymbol( const std::string& symbol ) const;
    //@}

private:
    //! @name Member data
    //@{    
    IWorkspaceEditPtr   spWorkspaceEdit_;
    // IFeatureBufferPtr   spFeatureBuffer_;
    IFeaturePtr         spFeature_;    
    //@}

    //! @name 
    //@{
    const kernel::Resolver_ABC< kernel::AgentType >& resolver_;
    const World& world_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Write
// Created: JCR 2007-05-02
// -----------------------------------------------------------------------------
template< typename VariantType >
void ScopeEditor::Write( IFeaturePtr spFeature, BSTR name, VariantType info )
{
    IFieldsPtr  spFlds;
    long        id;

    spFeature->get_Fields( &spFlds );
    if ( ! FAILED( spFlds->FindField( name, &id ) ) )    
        spFeature->put_Value( id, CComVariant( info ) );    
}

}
}

#endif // __ScopeEditor_h_
