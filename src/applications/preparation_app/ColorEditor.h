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
    class Controllers;
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
                  , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
                  , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ColorEditor( QWidget* parent, kernel::Controllers& controllers,
                          gui::ColorStrategy_ABC& colorStrategy, gui::ColorEditor_ABC& colorEditor );
    virtual ~ColorEditor();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
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
