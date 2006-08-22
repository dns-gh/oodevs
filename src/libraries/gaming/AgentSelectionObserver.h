// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentSelectionObserver_h_
#define __AgentSelectionObserver_h_

#include "clients_kernel/SelectionObserver_ABC.h"

namespace kernel
{
    class Population_ABC;
    class Agent_ABC;
    class Entity_ABC;
}

// =============================================================================
/** @class  AgentSelectionObserver
    @brief  AgentSelectionObserver
*/
// Created: AGE 2006-03-09
// =============================================================================
class AgentSelectionObserver : public kernel::SelectionObserver_ABC
                             , public kernel::SelectionObserver_Base< kernel::Population_ABC >
                             , public kernel::SelectionObserver_Base< kernel::Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentSelectionObserver();
    virtual ~AgentSelectionObserver();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentSelectionObserver( const AgentSelectionObserver& );            //!< Copy constructor
    AgentSelectionObserver& operator=( const AgentSelectionObserver& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Population_ABC& element );
    virtual void Select( const kernel::Agent_ABC& element );
    virtual void NotifySelected( const kernel::Entity_ABC* ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC* selected_;
    //@}
};

#endif // __AgentSelectionObserver_h_
