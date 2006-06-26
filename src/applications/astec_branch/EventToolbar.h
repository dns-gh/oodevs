// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EventToolbar_h_
#define __EventToolbar_h_

#include "ContextMenuObserver_ABC.h"
#include "Observer_ABC.h"

class Controllers;
class Agent;

// =============================================================================
/** @class  ControllerToolbar
    @brief  Toolbar event subscribing/unsubscribing
*/
// Created: SBO 2006-06-20
// =============================================================================
class EventToolbar : public QToolBar
                   , public Observer_ABC
                   , public ContextMenuObserver_ABC< Agent >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             EventToolbar( QMainWindow* pParent, Controllers& controllers );
    virtual ~EventToolbar();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const Agent&, QPopupMenu& );
    //@}

private slots:
    //! @name Slots
    //@{
    void Subscribe();
    void UnSubscribe();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EventToolbar( const EventToolbar& );
    EventToolbar& operator=( const EventToolbar& );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const Agent* >   T_Agents;
    typedef T_Agents::const_iterator    CIT_Agents;
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    T_Agents subscribed_;
    const Agent* selected_;
    //@}
};

#endif // __EventToolbar_h_
