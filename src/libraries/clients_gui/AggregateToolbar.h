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

#include <tools/Resolver.h>
#include <vector>


namespace kernel
{
    class Automat_ABC;
    class Formation_ABC;
    class Controller;
    class ContextMenu;
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
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             AggregateToolbar( GLView_ABC& view,
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
    void DisaggregateAll();
    void Aggregate( int id );
    void OnLockDragAndDropToggled( bool toggled );
    void OnChangeDisplay( int id );
    //@}

private:
    //! @name Member data
    //@{
    GLView_ABC& view_;
    const tools::Resolver< kernel::Formation_ABC >& formations_;
    const tools::Resolver< kernel::Automat_ABC >& automats_;

    kernel::ContextMenu* levelMenu_;
    kernel::ContextMenu* displayMenu_;
    //@}
};

} //! namespace gui

#endif // gui_AggregateToolbar_h
