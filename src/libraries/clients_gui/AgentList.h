// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentList_h_
#define __AgentList_h_

#include "EntitySearchBox.h"
#include "clients_kernel/Agent_ABC.h"

namespace kernel
{
    class Controllers;
}

class Publisher_ABC;

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  AgentList
    @brief  Agent list
*/
// Created: AGE 2006-08-10
// =============================================================================
template< typename List >
class AgentList : public QVBox
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentList( kernel::Controllers& controllers, Publisher_ABC& publisher, ItemFactory_ABC& factory );
    virtual ~AgentList();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentList( const AgentList& );            //!< Copy constructor
    AgentList& operator=( const AgentList& ); //!< Assignement operator
    //@}
};

// -----------------------------------------------------------------------------
// Name: AgentList constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
template< typename List >
AgentList< List >::AgentList( kernel::Controllers& controllers, Publisher_ABC& publisher, ItemFactory_ABC& factory )
{
    new EntitySearchBox< kernel::Agent_ABC >( this, controllers );
    new List( this, controllers, publisher, factory );
}

// -----------------------------------------------------------------------------
// Name: AgentList destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
template< typename List >
AgentList< List >::~AgentList()
{
    // NOTHING
}

}

#endif // __AgentList_h_
