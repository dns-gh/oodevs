// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gearth_SymbolEditor_h_
#define __gearth_SymbolEditor_h_

#include "game_asn/Asn.h"

namespace dispatcher
{
    class Model;
}

namespace gearth
{
    class StyleEditor;

// =============================================================================
/** @class  SymbolEditor
    @brief  SymbolEditor
*/
// Created: JCR 2007-05-15
// =============================================================================
class SymbolEditor
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SymbolEditor( const StyleEditor& styleEditor, const dispatcher::Model& model );
    virtual ~SymbolEditor();
    //@}

    //! @name Operations
    //@{
    std::string GetSymbol( const ASN1T_MsgUnitCreation& asn ) const;
    std::string GetSymbol( const ASN1T_MsgUnitKnowledgeCreation& asn ) const;
    std::string GetSymbol( const ASN1T_MsgObjectCreation& asn ) const;
    std::string GetSymbol( const ASN1T_MsgFormationCreation& asn ) const;
    std::string GetSymbol( const ASN1T_MsgAutomatCreation& asn ) const;
    std::string GetSymbol( const ASN1T_MsgLimaCreation& asn ) const;
    std::string GetSymbol( const ASN1T_MsgLimitCreation& asn ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SymbolEditor( const SymbolEditor& );            //!< Copy constructor
    SymbolEditor& operator=( const SymbolEditor& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{    
    const dispatcher::Model& model_;
    const StyleEditor& styles_;
    //@}
};

}

#endif // __gearth_SymbolEditor_h_
