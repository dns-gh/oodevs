// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ChatWidget_h_
#define __ChatWidget_h_

#include "gaming/CommandHandler_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class UserProfile_ABC;
}

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
class ChatWidget : public Q3HBox
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< kernel::UserProfile_ABC >
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
    void OnProfileSelected( const kernel::UserProfile_ABC& profile );
    void OnTabClosed( int index );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::UserProfile_ABC& profile );
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
    std::unique_ptr< CommandPublisher > publisher_;
    QTabWidget* tabs_;
    QListWidget* profiles_;
    T_Rooms rooms_;
    //@}
};

#endif // __ChatWidget_h_
