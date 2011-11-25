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

#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/Filter_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class AutomatsLayer;
    class FormationLayer;
    class AggregateToolbar;
}

class Simulation;
class ProfileFilter;
class Filter_ABC;
class Profile;

// =============================================================================
/** @class  OrbatToolbar
    @brief  OrbatToolbar
*/
// Created: SBO 2007-03-05
// =============================================================================
class OrbatToolbar : public Q3HBox
                   , public tools::Observer_ABC
                   , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
                   , public tools::ElementObserver_ABC< Simulation >
                   , public tools::ElementObserver_ABC< Profile >
                   , public tools::ElementObserver_ABC< kernel::Filter_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             OrbatToolbar( QWidget* parent, kernel::Controllers& controllers, ProfileFilter& filter,
                           gui::AutomatsLayer& automats, gui::FormationLayer& formations );
    virtual ~OrbatToolbar();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSetFilter();
    void OnClearFilter();
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
    virtual void NotifyUpdated( const Simulation& simu );
    virtual void NotifyUpdated( const kernel::Filter_ABC& filter );
    virtual void NotifyUpdated( const Profile& profile );

    void Filter( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ProfileFilter& filter_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    gui::AggregateToolbar* pAggregateToolbar_;
    QToolButton* filterBtn_;
    QString current_;
    //@}
};

#endif // __OrbatToolbar_h_
