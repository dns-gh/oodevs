// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UnitStateDialog_h_
#define __UnitStateDialog_h_

#include "clients_gui/UnitStateDialog.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Formation_ABC;
    class Team_ABC;
}

class StaticModel;

// =============================================================================
/** @class  UnitStateDialog
    @brief  UnitStateDialog
*/
// Created: ABR 2011-07-05
// =============================================================================
class UnitStateDialog : public gui::UnitStateDialog
                      , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UnitStateDialog( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel );
    virtual ~UnitStateDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void Validate() const;
    virtual void Reset();
    //@}
};

#endif // __UnitStateDialog_h_
