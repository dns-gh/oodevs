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

#include "game_asn/Asn.h"

namespace dispatcher
{
    class Model;
}

namespace crossbow
{
    class Row_ABC;

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
    explicit SymbolEditor( const dispatcher::Model& model );
    virtual ~SymbolEditor();
    //@}

    //! @name Operations
    //@{
    void Update( Row_ABC& row, const ASN1T_MsgLimitCreation& asn );
    void Update( Row_ABC& row, const ASN1T_MsgLimaCreation& asn );
    void Update( Row_ABC& row, const ASN1T_MsgObjectCreation& asn );
    void Update( Row_ABC& row, const ASN1T_MsgUnitCreation& asn );
    void Update( Row_ABC& row, const ASN1T_MsgUnitKnowledgeCreation& asn );
    void Update( Row_ABC& row, const ASN1T_MsgUnitKnowledgeUpdate& asn );
    void Update( Row_ABC& row, const ASN1T_MsgFormationCreation& asn );
    void Update( Row_ABC& row, const ASN1T_MsgAutomatCreation& asn );
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
    //@}
};

}

#endif // __SymbolEditor_h_
