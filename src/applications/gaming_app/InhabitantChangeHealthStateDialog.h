// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InhabitantChangeHealthStateDialog_h_
#define __InhabitantChangeHealthStateDialog_h_

#include "ChangeHealthStateDialog_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class Inhabitant_ABC;
}

// =============================================================================
/** @class  InhabitantChangeHealthStateDialog
    @brief  InhabitantChangeHealthStateDialog
*/
// Created: ABR 2011-01-25
// =============================================================================
class InhabitantChangeHealthStateDialog : public ChangeHealthStateDialog_ABC
                                        , public kernel::ContextMenuObserver_ABC< kernel::Inhabitant_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             InhabitantChangeHealthStateDialog( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~InhabitantChangeHealthStateDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void CreateMagicAction( actions::ActionsModel& actionsModel ) const;
    virtual unsigned int GetValue( unsigned int index ) const;
    //@}
};

#endif // __InhabitantChangeHealthStateDialog_h_
