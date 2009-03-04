// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrbatToolbar_h_
#define __OrbatToolbar_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class Automat_ABC;
    class Entity_ABC;
}

namespace gui
{
    class AutomatsLayer;
}

class Simulation;
class ProfileFilter;

// =============================================================================
/** @class  OrbatToolbar
    @brief  OrbatToolbar
*/
// Created: SBO 2007-03-05
// =============================================================================
class OrbatToolbar : public QHBox
                   , public kernel::Observer_ABC
                   , public kernel::ElementObserver_ABC< kernel::Automat_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
                   , public kernel::ElementObserver_ABC< Simulation >
                   , public kernel::ElementObserver_ABC< ProfileFilter >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             OrbatToolbar( QWidget* parent, kernel::Controllers& controllers, ProfileFilter& filter, gui::AutomatsLayer& automats );
    virtual ~OrbatToolbar();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSetFilter();
    void OnClearFilter();

    void AggregateAll();
    void DisaggregateAll();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrbatToolbar( const OrbatToolbar& );            //!< Copy constructor
    OrbatToolbar& operator=( const OrbatToolbar& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual QSize minimumSizeHint() const;
    virtual void NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyCreated( const kernel::Automat_ABC& );
    virtual void NotifyDeleted( const kernel::Automat_ABC& );
    virtual void NotifyUpdated( const Simulation& simu );
    virtual void NotifyUpdated( const ProfileFilter& filter );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::Automat_ABC* > T_Agents;
    typedef T_Agents::iterator                       IT_Agents;
    typedef T_Agents::const_iterator                CIT_Agents;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ProfileFilter& filter_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    gui::AutomatsLayer& automatsLayer_;
    T_Agents automats_;
    QToolButton* filterBtn_;
    //@}
};

#endif // __OrbatToolbar_h_
