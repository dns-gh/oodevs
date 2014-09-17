// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventTaskWidget_h_
#define __EventTaskWidget_h_

#include "EventWidget_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include <boost/shared_ptr.hpp>

namespace gui
{
    class EntitySymbols;
    class EventTaskPresenter;
    struct EventTaskViewState;
    class RichLineEdit;
    class RichTextEdit;
    class TaskerWidget;
}

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class EntityResolver_ABC;
    class Formation_ABC;
    class Inhabitant_ABC;
    class Population_ABC;
    class Profile_ABC;
    class Team_ABC;
    class Time_ABC;
}

class Model;

// =============================================================================
/** @class  EventTaskWidget
    @brief  EventTaskWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventTaskWidget : public EventTaskWidget_ABC
                      , public tools::Observer_ABC
                      , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Inhabitant_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                      , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventTaskWidget( gui::EventPresenter& presenter,
                              kernel::Controllers& controllers,
                              const gui::EntitySymbols& symbols,
                              const kernel::Profile_ABC& profile,
                              const kernel::EntityResolver_ABC& model,
                              const kernel::Time_ABC& simulation );
    virtual ~EventTaskWidget();
    //@}

private:
    //! @name gui::EventTaskWidget_ABC implementation
    //@{
    virtual void Purge();
    virtual void BlockSignals( bool blocked );
    virtual void Build( const gui::EventTaskViewState& state );
    //@}

    //! @name Observers implementation
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Inhabitant_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Team_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    //@}

    //! @name Helpers
    //@{
    void AddContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    void OnTargetChanged( const kernel::Entity_ABC* entity );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnNewTaskClicked();
    void OnReplaceTaskerClicked();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profile_;
    const kernel::EntityResolver_ABC& model_;
    const kernel::Time_ABC& simulation_;
    boost::shared_ptr< gui::EventTaskPresenter > taskPresenter_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;

    gui::TaskerWidget* taskerWidget_;
    gui::RichLineEdit* label_;
    gui::RichTextEdit* description_;
    gui::RichLineEdit* url_;
    QLabel* bytes_;
    QPushButton* showButton_;
    gui::RichTextEdit* payload_;
    QLabel* payloadLabel_;
    int cursorPos_;
    unsigned long lastTaskerId_;
    //@}
};

#endif // __EventTaskWidget_h_
