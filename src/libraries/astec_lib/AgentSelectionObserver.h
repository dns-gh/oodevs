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

#include "SelectionObserver_ABC.h"

class Population;
class Agent;
class Entity_ABC;

// =============================================================================
/** @class  AgentSelectionObserver
    @brief  AgentSelectionObserver
*/
// Created: AGE 2006-03-09
// =============================================================================
class AgentSelectionObserver
    : public SelectionObserver_ABC
    , public SelectionObserver_Base< Population >
    , public SelectionObserver_Base< Agent >
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
    virtual void Select( const Population& element );
    virtual void Select( const Agent& element );
    virtual void NotifySelected( const Entity_ABC* ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    const Entity_ABC* selected_;
    //@}
};

#endif // __AgentSelectionObserver_h_
