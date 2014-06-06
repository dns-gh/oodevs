// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MessagePanel_h_
#define __MessagePanel_h_

#include "gaming/CommandHandler_ABC.h"
#include "clients_gui/RichToolBar.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class RichLabel;
    class RichToolBar;
    class ItemFactory_ABC;
}

class CommandHandler;
class CommandPublisher;
class Publisher_ABC;

// =============================================================================
/** @class  MessagePanel
    @brief  MessagePanel
*/
// Created: SBO 2009-03-04
// =============================================================================
class MessagePanel : public gui::RichToolBar
                   , public CommandHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MessagePanel( QMainWindow* mainWindow, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler, gui::ItemFactory_ABC& factory );
    virtual ~MessagePanel();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const Command& command );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnButtonPressed();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessagePanel( const MessagePanel& );            //!< Copy constructor
    MessagePanel& operator=( const MessagePanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Display( const QString& message );
    void AddButton( const QString& title );
    void Clear();
    void ClearButtons();
    virtual QSize sizeHint() const;
    virtual void NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode );
    //@}

private:
    //! @name Member data
    //@{
    QMainWindow* mainWindow_;
    CommandHandler& handler_;
    gui::ItemFactory_ABC& factory_;
    std::unique_ptr< CommandPublisher > publisher_;
    gui::RichLabel* text_;
    QWidget* buttons_;
    std::string activePrompt_;
    bool isVisible_;
    //@}
};

#endif // __MessagePanel_h_
