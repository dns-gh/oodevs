// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventMagicWidget_h_
#define __EventMagicWidget_h_

#include "EventWidget_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class EntitySymbols;
    class EventMagicPresenter;
}

class Model;
class TaskerWidget;

// =============================================================================
/** @class  EventMagicWidget
    @brief  EventMagicWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventMagicWidget : public EventMagicWidget_ABC
                       , public tools::Observer_ABC
                       , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
                       , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventMagicWidget( gui::EventPresenter& presenter,
                               kernel::Controllers& controllers,
                               Model& model,
                               const gui::EntitySymbols& symbols );
    virtual ~EventMagicWidget();
    //@}

private:
    //! @name EventMagicWidget_ABC implementation
    //@{
    virtual void Purge();
    virtual void BlockSignals( bool blocked );
    virtual void Build( const gui::EventMagicViewState& state );
    //@}

    //! @name Observers implementation
    //@{
    virtual void NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    //@}

    //! @name Helpers
    //@{
    void OnTargetChanged( const kernel::Entity_ABC* entity );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnReplaceTargetClicked();
    void OnClearTaskerClicked();
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    boost::shared_ptr< gui::EventMagicPresenter > magicPresenter_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    unsigned int lastTaskerId_;
    TaskerWidget* taskerWidget_;
    QLabel* name_;
    //@}
};

#endif // __EventMagicWidget_h_
