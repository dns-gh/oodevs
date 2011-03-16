// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AgentChangeAffinitiesDialog_h_
#define __AgentChangeAffinitiesDialog_h_

#include "ChangeAffinitiesDialog.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class Agent_ABC;
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
/** @class  AgentChangeAffinitiesDialog
    @brief  Agent change affinities dialog
*/
// Created: LGY 2011-03-16
// =============================================================================
class AgentChangeAffinitiesDialog : public ChangeAffinitiesDialog
                                  , public tools::Observer_ABC
                                  , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentChangeAffinitiesDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel,
                                          actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~AgentChangeAffinitiesDialog();
    //@}

    //! @name Operations
    //@{
    virtual void DoValidate();
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentChangeAffinitiesDialog( const AgentChangeAffinitiesDialog& );            //!< Copy constructor
    AgentChangeAffinitiesDialog& operator=( const AgentChangeAffinitiesDialog& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    actions::ActionsModel& actionsModel_;
    //@}
};

#endif // __AgentChangeAffinitiesDialog_h_
