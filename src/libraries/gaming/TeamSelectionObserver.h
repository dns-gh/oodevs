// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamSelectionObserver_h_
#define __TeamSelectionObserver_h_

#include "clients_kernel/SelectionObserver_ABC.h"

class Agent_ABC;
class KnowledgeGroup_ABC;
class Team_ABC;
class Object_ABC;
class Population_ABC;

// =============================================================================
/** @class  TeamSelectionObserver
    @brief  TeamSelectionObserver
*/
// Created: AGE 2006-02-24
// =============================================================================
class TeamSelectionObserver
    : public SelectionObserver_ABC
    , public SelectionObserver_Base< Team_ABC >
    , public SelectionObserver_Base< KnowledgeGroup_ABC >
    , public SelectionObserver_Base< Agent_ABC >
    , public SelectionObserver_Base< Object_ABC >
    , public SelectionObserver_Base< Population_ABC>
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamSelectionObserver();
    virtual ~TeamSelectionObserver();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TeamSelectionObserver( const TeamSelectionObserver& );            //!< Copy constructor
    TeamSelectionObserver& operator=( const TeamSelectionObserver& ); //!< Assignement operator
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const Team_ABC& element );
    virtual void Select( const KnowledgeGroup_ABC& element );
    virtual void Select( const Agent_ABC& element );
    virtual void Select( const Object_ABC& element );
    virtual void Select( const Population_ABC& element );
    virtual void Select( const Team_ABC* ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    const Team_ABC* selected_;
    //@}
};

#endif // __TeamSelectionObserver_h_
