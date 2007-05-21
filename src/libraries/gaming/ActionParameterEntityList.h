// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterEntityList_h_
#define __ActionParameterEntityList_h_

#include "game_asn/Asn.h"
#include "ActionParameter.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
}

// =============================================================================
/** @class  ActionParameterEntityList
    @brief  ActionParameterEntityList
*/
// Created: SBO 2007-05-03
// =============================================================================
class ActionParameterEntityList : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterEntityList( const kernel::OrderParameter& parameter );
             ActionParameterEntityList( const kernel::OrderParameter& parameter, const ASN1T_ListAgent& asn, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver );
             ActionParameterEntityList( const kernel::OrderParameter& parameter, const ASN1T_ListAutomate& asn, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver );
             ActionParameterEntityList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver );
             ActionParameterEntityList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver );
    virtual ~ActionParameterEntityList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterEntityList( const ActionParameterEntityList& );            //!< Copy constructor
    ActionParameterEntityList& operator=( const ActionParameterEntityList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAgent( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver );
    void ReadAutomat( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver );
    template< typename ConcreteEntity >
    void ReadEntity( xml::xistream& xis, const kernel::Resolver_ABC< ConcreteEntity >& resolver );
    //@}
};

#endif // __ActionParameterEntityList_h_
