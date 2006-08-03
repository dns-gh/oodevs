// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticToolbar_h_
#define __LogisticToolbar_h_

#include "ElementObserver_ABC.h"

class Controllers;
class Agent_ABC;
class AgentsLayer;

// =============================================================================
/** @class  LogisticToolbar
    @brief  LogisticToolbar
*/
// Created: AGE 2006-03-30
// =============================================================================
class LogisticToolbar : public QToolBar
                      , public Observer_ABC
                      , public ElementObserver_ABC< Agent_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticToolbar( QMainWindow* pParent, Controllers& controllers, AgentsLayer& agents ); // $$$$ AGE 2006-05-02: for aggregation...
    virtual ~LogisticToolbar();
    //@}

private slots:
    //! @name Slots
    //@{
    void AggregateAll();
    void DisaggregateAll();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticToolbar( const LogisticToolbar& );            //!< Copy constructor
    LogisticToolbar& operator=( const LogisticToolbar& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Agent_ABC& );
    virtual void NotifyDeleted( const Agent_ABC& );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const Agent_ABC* > T_Agents;
    typedef T_Agents::iterator         IT_Agents;
    typedef T_Agents::const_iterator  CIT_Agents;
    //@}

    //! @name Member data
    //@{
    Controllers& controllers_;
    AgentsLayer& agents_;
    T_Agents automats_;
    //@}
};

#endif // __LogisticToolbar_h_
