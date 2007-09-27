// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_ScopeEditor_h_
#define __crossbow_ScopeEditor_h_

#include "game_asn/Asn.h"

namespace dispatcher
{
    class Model;
}

namespace crossbow
{
    class Table_ABC;
    class ReportFactory;
    class ShapeFactory;

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
             ScopeEditor( const dispatcher::Model& model, const ReportFactory& reportFactory );
    virtual ~ScopeEditor();
    //@}

    //! @name Operators
    //@{
    void Update( Table_ABC& table, const ASN1T_MsgUnitCreation& msg );
    void Update( Table_ABC& table, const ASN1T_MsgUnitKnowledgeCreation& msg );
    void Update( Table_ABC& table, const ASN1T_MsgLimitCreation& msg );
    void Update( Table_ABC& table, const ASN1T_MsgLimaCreation& msg );
    void Update( Table_ABC& table, const ASN1T_MsgObjectCreation& msg );
    void Update( Table_ABC& table, const ASN1T_MsgReport& msg );
    void Update( Table_ABC& table, const ASN1T_MsgFormationCreation& asn );
    void Update( Table_ABC& table, const ASN1T_MsgAutomatCreation& asn );

    void Update( Table_ABC& table, const ASN1T_MsgUnitAttributes& msg );
    void Update( Table_ABC& table, const ASN1T_MsgUnitKnowledgeUpdate& msg );

    void Update( Table_ABC& table, const ASN1T_MsgUnitDestruction& msg );
    void Update( Table_ABC& table, const ASN1T_MsgUnitKnowledgeDestruction& msg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ScopeEditor( const ScopeEditor& );            //!< Copy constructor
    ScopeEditor& operator=( const ScopeEditor& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model& model_;
    const ReportFactory&     reportFactory_;
    //@}
};

}

#endif // __crossbow_ScopeEditor_h_
