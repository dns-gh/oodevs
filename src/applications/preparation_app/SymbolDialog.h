// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __SymbolDialog_h_
#define __SymbolDialog_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Agent_ABC;
    class Controllers;
    class Entity_ABC;
    class Ghost_ABC;
    class SymbolFactory;
}

namespace gui
{
    class ColorStrategy_ABC;
    class SymbolIcons;
    class NatureEditionWidget;
}

class EntitySymbolEditor;

// =============================================================================
/** @class  SymbolDialog
    @brief  SymbolDialog
*/
// Created: ABR 2012-08-08
// =============================================================================
class SymbolDialog : public QDialog
                   , public tools::Observer_ABC
                   , public kernel::ContextMenuObserver_ABC< kernel::Ghost_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             SymbolDialog( QWidget* parent, kernel::Controllers& controllers, kernel::SymbolFactory& symbolsFactory,
                           gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy );
    virtual ~SymbolDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Ghost_ABC& ghost, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
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
    kernel::Controllers&                      controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    EntitySymbolEditor*                       symbolEditor_;
    kernel::SymbolFactory&                    symbolsFactory_;
    //@}
};

#endif // __SymbolDialog_h_
