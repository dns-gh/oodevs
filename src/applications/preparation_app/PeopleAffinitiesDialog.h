// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _PeopleAffinitiesDialog_h
#define _PeopleAffinitiesDialog_h

#include "AffinitiesDialog.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

// =============================================================================
/** @class  PeopleAffinitiesDialog
    @brief  People affinities dialog
*/
// Created: LGY 2011-03-16
// =============================================================================
class PeopleAffinitiesDialog : public AffinitiesDialog
                             , public kernel::ContextMenuObserver_ABC< kernel::Inhabitant_ABC >
                             , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             PeopleAffinitiesDialog( QWidget* parent, kernel::Controllers& controllers );
    virtual ~PeopleAffinitiesDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // _PeopleAffinitiesDialog_h
