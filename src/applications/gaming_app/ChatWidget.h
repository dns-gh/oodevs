// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ChatWidget_h_
#define __ChatWidget_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "gaming/CommandHandler_ABC.h"

namespace kernel
{
    class Controllers;
}

class UserProfile;
class Publisher_ABC;
class CommandPublisher;
class CommandHandler;
class ChatRoom;

// =============================================================================
/** @class  ChatWidget
    @brief  ChatWidget
*/
// Created: SBO 2008-06-11
// =============================================================================
class ChatWidget : public QHBox
                 , public kernel::Observer_ABC
                 , public kernel::ElementObserver_ABC< UserProfile >
                 , public CommandHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ChatWidget( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler );
    virtual ~ChatWidget();
    //@}

private slots:
    //! @name Operations
    //@{
    void OnProfileSelected( const UserProfile& profile );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ChatWidget( const ChatWidget& );            //!< Copy constructor
    ChatWidget& operator=( const ChatWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const UserProfile& profile );
    virtual void Receive( const Command& command );
    void Select( const QString& name );
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, ChatRoom* > T_Rooms;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    CommandHandler& handler_;
    std::auto_ptr< CommandPublisher > publisher_;
    QTabWidget* tabs_;
    QListBox* profiles_;
    T_Rooms rooms_;
    //@}
};

#endif // __ChatWidget_h_
