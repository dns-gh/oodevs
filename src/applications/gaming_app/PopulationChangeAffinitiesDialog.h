// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PopulationChangeAffinitiesDialog_h_
#define __PopulationChangeAffinitiesDialog_h_

#include "ChangeAffinitiesDialog.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Population_ABC;
    class Profile_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

class StaticModel;

// =============================================================================
/** @class  PopulationChangeAffinitiesDialog
    @brief  Population change affinities dialog
*/
// Created: LGY 2011-03-16
// =============================================================================
class PopulationChangeAffinitiesDialog : public ChangeAffinitiesDialog
                                       , public tools::Observer_ABC
                                       , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationChangeAffinitiesDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel,
                                               actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~PopulationChangeAffinitiesDialog();
    //@}

    //! @name Operations
    //@{
    virtual void DoValidate();
    virtual void NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu );
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

#endif // __PopulationChangeAffinitiesDialog_h_
