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
#include <QtGui/QDockWidget.h>

namespace kernel
{
    class Controllers;
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
class MessagePanel : public QDockWidget
                   , public CommandHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MessagePanel( QMainWindow* mainWindow, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler );
    virtual ~MessagePanel();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const Command& command );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnButtonPressed( int id );
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
    //@}

private:
    //! @name Member data
    //@{
    QMainWindow* mainWindow_;
    CommandHandler& handler_;
    std::auto_ptr< CommandPublisher > publisher_;
    QLabel* text_;
    Q3ButtonGroup* buttons_;
    std::string activePrompt_;
    //@}
};

#endif // __MessagePanel_h_
