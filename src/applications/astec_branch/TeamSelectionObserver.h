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

#include "SelectionObserver_ABC.h"
class Agent;
class KnowledgeGroup;
class Team;

// =============================================================================
/** @class  TeamSelectionObserver
    @brief  TeamSelectionObserver
*/
// Created: AGE 2006-02-24
// =============================================================================
class TeamSelectionObserver
    : public SelectionObserver_ABC
    , public SelectionObserver_Base< Team >
    , public SelectionObserver_Base< KnowledgeGroup >
    , public SelectionObserver_Base< Agent >
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

    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const Team& element );
    virtual void Select( const KnowledgeGroup& element );
    virtual void Select( const Agent& element );
    virtual void Select( const Team* ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    const Team* selected_;
    //@}
};

#endif // __TeamSelectionObserver_h_
