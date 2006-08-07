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

#include "astec_gaming/ElementObserver_ABC.h"
#include "astec_gaming/Observer_ABC.h"

#include <list>

class Controllers;
class Report_ABC;
class Entity_ABC;

// =============================================================================
/** @class  EventToolbar
    @brief  Toolbar event
*/
// Created: SBO 2006-06-20
// =============================================================================
class EventToolbar : public QToolBar
                   , public Observer_ABC
                   , public ElementObserver_ABC< Report_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             EventToolbar( QMainWindow* pParent, Controllers& controllers );
    virtual ~EventToolbar();
    //@}

private slots:
    //! @name Slots
    //@{
    void GasClicked();
    void ConflictClicked();
    void MessageClicked();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EventToolbar( const EventToolbar& );
    EventToolbar& operator=( const EventToolbar& );
    //@}

    //! @name Types
    //@{
    typedef std::list< const Entity_ABC* > T_Agents;
    typedef T_Agents::const_iterator    CIT_Agents;
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Report_ABC& report );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    T_Agents messageAgents_;

    QToolButton* gasButton_;
    QToolButton* conflictButton_;
    QToolButton* messageButton_;
    //@}
};

#endif // __EventToolbar_h_
