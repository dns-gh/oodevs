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
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class Automat_ABC;
    class Formation_ABC;
    class Entity_ABC;
}

namespace gui
{
    class AutomatsLayer;
    class FormationLayer;
}

class Simulation;
class ProfileFilter;

// =============================================================================
/** @class  OrbatToolbar
    @brief  OrbatToolbar
*/
// Created: SBO 2007-03-05
// =============================================================================
class OrbatToolbar : public Q3HBox
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                   , public tools::ElementObserver_ABC< kernel::Formation_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
                   , public tools::ElementObserver_ABC< Simulation >
                   , public tools::ElementObserver_ABC< ProfileFilter >
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
    void Aggregate();
    void DisaggregateAll();
    void Aggregate( int id );
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
    virtual void NotifyCreated( const kernel::Formation_ABC& );
    virtual void NotifyDeleted( const kernel::Formation_ABC& );
    virtual void NotifyUpdated( const Simulation& simu );
    virtual void NotifyUpdated( const ProfileFilter& filter );

    void Filter( const kernel::Entity_ABC& entity );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::Automat_ABC* > T_Automats;
    typedef T_Automats::iterator                     IT_Automats;
    typedef T_Automats::const_iterator              CIT_Automats;

    typedef std::vector< const kernel::Formation_ABC* > T_Formations;
    typedef T_Formations::iterator                     IT_Formations;
    typedef T_Formations::const_iterator              CIT_Formations;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ProfileFilter& filter_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    gui::AutomatsLayer& automatsLayer_;
    gui::FormationLayer& formationsLayer_;
    T_Automats automats_;
    T_Formations formations_;
    QToolButton* filterBtn_;
    Q3PopupMenu* menu_;
    //@}
};

#endif // __OrbatToolbar_h_
