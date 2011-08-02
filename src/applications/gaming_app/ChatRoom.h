// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ChatRoom_h_
#define __ChatRoom_h_

#include "gaming/CommandHandler_ABC.h"

class CommandPublisher;
class CommandHandler;

// =============================================================================
/** @class  ChatRoom
    @brief  ChatRoom
*/
// Created: SBO 2008-06-11
// =============================================================================
class ChatRoom : public Q3VBox
               , public CommandHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ChatRoom( QWidget* parent, CommandPublisher& publisher, CommandHandler& handler, QString filter = "" );
    virtual ~ChatRoom();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const Command& command );
    //@}

protected slots:
    //! @name Slots
    //@{
    void OnTextChanged( const QString& text );
    virtual void OnSend();
    //@}

private:
    //! @name Helpers
    //@{
    void Send( const std::string& target );
    bool Matches( const QString& source, const QString& target ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ChatRoom( const ChatRoom& );            //!< Copy constructor
    ChatRoom& operator=( const ChatRoom& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    CommandPublisher& publisher_;
    CommandHandler& handler_;
    QString filter_;
    Q3TextEdit* history_;
    QLineEdit* text_;
    QPushButton* sendBtn_;
    //@}
};

#endif // __ChatRoom_h_
