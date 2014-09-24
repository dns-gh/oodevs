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

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/Filter_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Agent_ABC;
    class AgentKnowledge_ABC;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class Population_ABC;
    class Object_ABC;
    class ObjectKnowledge_ABC;
    class Team_ABC;
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
class OrbatToolbar : public QFrame
                   , public tools::Observer_ABC
                   , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::AgentKnowledge_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::ObjectKnowledge_ABC >
                   , public tools::ElementObserver_ABC< Simulation >
                   , public tools::ElementObserver_ABC< Profile >
                   , public tools::ElementObserver_ABC< kernel::Filter_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             OrbatToolbar( QWidget* parent,
                           kernel::Controllers& controllers,
                           ProfileFilter& filter,
                           const std::shared_ptr< gui::AutomatsLayer >& automats,
                           const std::shared_ptr< gui::FormationLayer >& formations );
    virtual ~OrbatToolbar();
    //@}

public:
    //! @name Accesors
    //@{
    const gui::AggregateToolbar* GetToolbar() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSetFilter();
    void OnClearFilter();
    //@}

private:
    //! @name Helpers
    //@{
    virtual QSize minimumSizeHint() const;
    virtual void UpdateContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::AgentKnowledge_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::ObjectKnowledge_ABC& entity, kernel::ContextMenu& menu );

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
