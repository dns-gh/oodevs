// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _ColorEditor_h
#define _ColorEditor_h

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Entity_ABC;
    class Team_ABC;
    class Formation_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Ghost_ABC;
    class Object_ABC;
    class Population_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;
    class ColorEditor_ABC;
}

// =============================================================================
/** @class  ColorEditor
    @brief  Color editor
*/
// Created: LGY 2011-06-23
// =============================================================================
class ColorEditor : public QObject
                  , public tools::Observer_ABC
                  , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                  , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                  , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                  , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                  , public kernel::ContextMenuObserver_ABC< kernel::Ghost_ABC >
                  , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
                  , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
                  , public tools::ElementObserver_ABC< kernel::Team_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ColorEditor( QWidget* parent, kernel::Controllers& controllers,
                          gui::ColorStrategy_ABC& colorStrategy, gui::ColorEditor_ABC& colorEditor );
    virtual ~ColorEditor();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Ghost_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyUpdated( const kernel::Team_ABC& team );
    //@}

private:
    //! @name Helpers
    //@{
    void Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Reset();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ColorStrategy_ABC& colorStrategy_;
    gui::ColorEditor_ABC& colorEditor_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

#endif // _ColorEditor_h
