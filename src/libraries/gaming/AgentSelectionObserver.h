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

#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Population_ABC;
    class Agent_ABC;
    class Entity_ABC;
    class Automat_ABC;
}

// =============================================================================
/** @class  AgentSelectionObserver
    @brief  AgentSelectionObserver
*/
// Created: AGE 2006-03-09
// =============================================================================
class AgentSelectionObserver : public tools::SelectionObserver_ABC
                             , public tools::SelectionObserver_Base< kernel::Population_ABC >
                             , public tools::SelectionObserver_Base< kernel::Agent_ABC >
                             , public tools::SelectionObserver_Base< kernel::Automat_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentSelectionObserver();
    virtual ~AgentSelectionObserver();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentSelectionObserver( const AgentSelectionObserver& );            //!< Copy constructor
    AgentSelectionObserver& operator=( const AgentSelectionObserver& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Population_ABC& element );
    virtual void Select( const kernel::Agent_ABC& element );
    virtual void Select( const kernel::Automat_ABC& element );
    virtual void NotifySelected( const kernel::Entity_ABC* ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC* selected_;
    //@}
};

#endif // __AgentSelectionObserver_h_
