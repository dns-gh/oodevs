// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef gui_AggregateToolbar_h
#define gui_AggregateToolbar_h

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <tools/Resolver.h>
#include <vector>

namespace kernel
{
    class Automat_ABC;
    class Formation_ABC;
    class Entity_ABC;
    class Controllers;
    class ContextMenu;
    class Profile_ABC;
}

namespace gui
{
    class GLView_ABC;

// =============================================================================
/** @class  AggregateToolbar
    @brief  Aggregate toolbar
*/
// Created: LGY 2011-10-14
// =============================================================================
class AggregateToolbar : public QHBoxLayout
                       , public tools::Observer_ABC
                       , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             AggregateToolbar( kernel::Controllers& controllers,
                               GLView_ABC& view,
                               const kernel::Profile_ABC& profile,
                               const tools::Resolver< kernel::Formation_ABC >& formations,
                               const tools::Resolver< kernel::Automat_ABC >& automats,
                               bool showDisplayModes );
    virtual ~AggregateToolbar();
    //@}

signals:
    void LockDragAndDrop( bool lock );
    void ChangeDisplay( int mode );

private slots:
    //! @name Slots
    //@{
    void Aggregate();
    void Disaggregate();
    void AggregateAllAutomat();
    void DisaggregateAll();
    void ToggleAggregationLevel( QAction* action );
    void OnChangeDisplay( int id );
    void UpdateLevelMenu();
    //@}

private:
    virtual void NotifyContextMenu( const kernel::Automat_ABC&, kernel::ContextMenu& );
    virtual void NotifyContextMenu( const kernel::Formation_ABC&, kernel::ContextMenu& );
    void OnContextMenu( const kernel::Entity_ABC&, kernel::ContextMenu& );
    void AggregateLevel( const std::string& level );
    void DisaggregateLevel( const std::string& level );

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    GLView_ABC& view_;
    const kernel::Profile_ABC& profile_;
    const tools::Resolver< kernel::Formation_ABC >& formations_;
    const tools::Resolver< kernel::Automat_ABC >& automats_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    std::string currentLevel_;
    QMenu* levelMenu_;
    QMenu* displayMenu_;
    bool modelLoaded_;
    //@}
};

} //! namespace gui

#endif // gui_AggregateToolbar_h
