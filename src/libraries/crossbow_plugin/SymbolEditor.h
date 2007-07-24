// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SymbolEditor_h_
#define __SymbolEditor_h_

#include "ESRI.h"
#include "game_asn/Asn.h"
#include "Resolver_ABC.h"

namespace kernel
{
    class AgentType;
    class AutomatType;
}

namespace dispatcher
{
    class Model;
}

namespace crossbow
{
    class ScopeEditor;

// =============================================================================
/** @class  SymbolEditor
    @brief  SymbolEditor
*/
// Created: JCR 2007-05-15
// =============================================================================
class SymbolEditor
{
    enum E_Field { affiliation = 1, function = 4, status = 3, echelon = 10 };
public:
    //! @name Constructors/Destructor
    //@{
             SymbolEditor( ScopeEditor& scope, const dispatcher::Model& model );
    virtual ~SymbolEditor();
    //@}

    //! @name
    //@{
    void Write( IFeaturePtr spFeature, const ASN1T_MsgLimitCreation& asn );
    void Write( IFeaturePtr spFeature, const ASN1T_MsgLimaCreation& asn );
    void Write( IFeaturePtr spFeature, const ASN1T_MsgObjectCreation& asn );
    void Write( IFeaturePtr spFeature, const ASN1T_MsgUnitCreation& asn, const kernel::Resolver_ABC< kernel::AgentType >& resolver );
    void Write( IFeaturePtr spFeature, const ASN1T_MsgUnitKnowledgeCreation& asn );
    void Write( IFeaturePtr spFeature, const ASN1T_MsgUnitKnowledgeUpdate& asn, const kernel::Resolver_ABC< kernel::AgentType >& resolver );
    //@}

    //! @name
    //@{
    void Write( IRowBufferPtr spRow, const ASN1T_MsgFormationCreation& asn );
    void Write( IRowBufferPtr spRow, const ASN1T_MsgAutomatCreation& asn, const kernel::Resolver_ABC< kernel::AutomatType >& resolver );
    //@}

    //! @name
    //@{
    static std::string GetLevel( ASN1T_EnumNatureLevel level );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SymbolEditor( const SymbolEditor& );            //!< Copy constructor
    SymbolEditor& operator=( const SymbolEditor& ); //!< Assignement operator
    //@}

    //! @name Knowledge functions
    //@{
    void UpdateOnIdentification( std::string& symbol, const ASN1T_MsgUnitKnowledgeUpdate& asn, const kernel::AgentType& );
    void UpdateOnRecognition( std::string& symbol, const ASN1T_MsgUnitKnowledgeUpdate& asn, const kernel::AgentType& );
    void UpdateOnDetection( std::string& symbol );
    const kernel::AgentType& RetrieveAgentType( const ASN1T_MsgUnitKnowledgeUpdate& asn, const kernel::Resolver_ABC< kernel::AgentType >& resolver );
    //@}

    //! @name
    //@{
    void UpdateAffiliation  ( std::string& symbol, ASN1T_EnumDiplomacy diplomatie ) const;
    void UpdateFunction     ( std::string& symbol, const std::string& type ) const;
    void UpdateEchelon      ( std::string& symbol, ASN1T_EnumNatureLevel level, bool pc = false ) const;
    std::string  ExtractCode( const std::string& symbol ) const;
    std::string  BuildSymbol( const std::string& symbol ) const;
    //@}

private:
    //! @name Member data
    //@{
    ScopeEditor& scope_;
    const dispatcher::Model& model_;
    //@}
};

}

#endif // __SymbolEditor_h_
