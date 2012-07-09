// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __CriticalIntelligenceDialog_h_
#define __CriticalIntelligenceDialog_h_

#include "clients_gui/CriticalIntelligenceDialog.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
    class Population_ABC;
    class Controllers;
    class Profile_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

class StaticModel;

// =============================================================================
/** @class  CriticalIntelligenceDialog
    @brief  Critical intelligence dialog
*/
// Created: LGY 2011-05-27
// =============================================================================
class CriticalIntelligenceDialog : public gui::CriticalIntelligenceDialog
                                 , public tools::Observer_ABC
                                 , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                                 , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             CriticalIntelligenceDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel,
                                         actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~CriticalIntelligenceDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Slots
    //@{
    virtual void OnAccept();
    virtual void OnReject();
    virtual void OnShow();
    //@}

private:
    //! @name Member data
    //@{
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    actions::ActionsModel& actionsModel_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

#endif // __CriticalIntelligenceDialog_h_
