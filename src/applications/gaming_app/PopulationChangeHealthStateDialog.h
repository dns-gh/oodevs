// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PopulationChangeHealthStateDialog_h_
#define __PopulationChangeHealthStateDialog_h_

#include "ChangeHealthStateDialog_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class Population_ABC;
}

// =============================================================================
/** @class  PopulationChangeHealthStateDialog
    @brief  PopulationChangeHealthStateDialog
*/
// Created: JSR 2011-03-15
// =============================================================================
class PopulationChangeHealthStateDialog : public ChangeHealthStateDialog_ABC
                                        , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationChangeHealthStateDialog( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~PopulationChangeHealthStateDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void CreateMagicAction( actions::ActionsModel& actionsModel ) const;
    virtual unsigned int GetValue( unsigned int index ) const;
    //@}
};

#endif // __PopulationChangeHealthStateDialog_h_
