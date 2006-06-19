// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionLogger_h_
#define __ActionLogger_h_

#include "SelectionObserver_ABC.h"
class Controllers;
class Agent;
class Object;
class Team;
class Population;
class KnowledgeGroup;

// =============================================================================
/** @class  ActionLogger
    @brief  ActionLogger
*/
// Created: AGE 2006-06-19
// =============================================================================
class ActionLogger : public Observer_ABC, public SelectionObserver_ABC
                   , public SelectionObserver_Base< Agent >
                   , public SelectionObserver_Base< Object >
                   , public SelectionObserver_Base< Team >
                   , public SelectionObserver_Base< Population >
                   , public SelectionObserver_Base< KnowledgeGroup >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionLogger( Controllers& controllers );
    virtual ~ActionLogger();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ActionLogger( const ActionLogger& );            //!< Copy constructor
    ActionLogger& operator=( const ActionLogger& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const Agent& element );
    virtual void Select( const Object& element );
    virtual void Select( const Team& element );
    virtual void Select( const Population& element );
    virtual void Select( const KnowledgeGroup& element );

    template< typename T >
    void LogSelection( const T& element );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    bool selecting_;
    //@}
};

#endif // __ActionLogger_h_
