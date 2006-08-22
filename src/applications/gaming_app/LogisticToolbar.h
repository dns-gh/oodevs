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

#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Agent_ABC;
}

namespace gui
{
    class AgentsLayer;
}

// =============================================================================
/** @class  LogisticToolbar
    @brief  LogisticToolbar
*/
// Created: AGE 2006-03-30
// =============================================================================
class LogisticToolbar : public QToolBar
                      , public kernel::Observer_ABC
                      , public kernel::ElementObserver_ABC< kernel::Agent_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticToolbar( QMainWindow* pParent, kernel::Controllers& controllers, gui::AgentsLayer& agents ); // $$$$ AGE 2006-05-02: for aggregation...
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
    virtual void NotifyCreated( const kernel::Agent_ABC& );
    virtual void NotifyDeleted( const kernel::Agent_ABC& );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::Agent_ABC* > T_Agents;
    typedef T_Agents::iterator         IT_Agents;
    typedef T_Agents::const_iterator  CIT_Agents;
    //@}

    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::AgentsLayer& agents_;
    T_Agents automats_;
    //@}
};

#endif // __LogisticToolbar_h_
