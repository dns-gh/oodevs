// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __GhostSymbolDialog_h_
#define __GhostSymbolDialog_h_

#include <boost/noncopyable.hpp>
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class Ghost_ABC;
    class SymbolFactory;
}

namespace gui
{
    class ColorStrategy_ABC;
    class SymbolIcons;

    class NatureEditionWidget;
}

class GhostSymbolEditor;

// =============================================================================
/** @class  GhostSymbolDialog
    @brief  GhostSymbolDialog
*/
// Created: ABR 2011-10-28
// =============================================================================
class GhostSymbolDialog :  public QDialog
                        , private boost::noncopyable
                        , public tools::Observer_ABC
                        , public kernel::ContextMenuObserver_ABC< kernel::Ghost_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             GhostSymbolDialog( QWidget* parent, kernel::Controllers& controllers, kernel::SymbolFactory& symbolsFactory, gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy );
    virtual ~GhostSymbolDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Ghost_ABC& ghost, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Validate();
    void Reject();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                     controllers_;
    kernel::SafePointer< kernel::Ghost_ABC > selected_;
    GhostSymbolEditor*                       symbolEditor_;
    //@}
};

#endif // __GhostSymbolDialog_h_
