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

#include "clients_gui/RichToolBar.h"
#include <tools/ElementObserver_ABC.h>

#include <deque>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Profile_ABC;
}

class Report;
class Simulation;

// =============================================================================
/** @class  EventToolbar
    @brief  Toolbar event
*/
// Created: SBO 2006-06-20
// =============================================================================
class EventToolbar : public gui::RichToolBar
                   , public tools::ElementObserver_ABC< Report >
                   , public tools::ElementObserver_ABC< kernel::Profile_ABC >
                   , public tools::ElementObserver_ABC< Simulation >
                   , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             EventToolbar( QMainWindow* pParent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile );
    virtual ~EventToolbar();
    //@}

private slots:
    //! @name Slots
    //@{
    void MessageClicked();
    //@}

private:
    //! @name Types
    //@{
    typedef std::deque< const kernel::Entity_ABC* > T_Agents;
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Report& report );
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    void UpdateMessageButton();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_Agents messageAgents_;
    const kernel::Profile_ABC& profile_;
    QToolButton* messageButton_;
    //@}
};

#endif // __EventToolbar_h_
