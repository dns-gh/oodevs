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
#include "gaming/AgentSelectionObserver.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

class AgentsModel;
class Publisher_ABC;
class ReportsModel;

// =============================================================================
/** @class  EventToolbar
    @brief  Toolbar event
*/
// Created: SBO 2006-06-20
// =============================================================================
class EventToolbar : public gui::RichToolBar
                   , public tools::ElementObserver_ABC< kernel::Profile_ABC >
                   , public AgentSelectionObserver
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
              EventToolbar( QMainWindow* pParent, kernel::Controllers& controllers,
                            ReportsModel& model, const AgentsModel& agents, Publisher_ABC& publisher );
    virtual ~EventToolbar();
    //@}

private slots:
    //! @name Slots
    //@{
    void MessageClicked();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    void UpdateMessageButton();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ReportsModel& model_;
    const AgentsModel& agents_;
    QToolButton* messageButton_;
    //@}
};

#endif // __EventToolbar_h_
