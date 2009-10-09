// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __XmliaWidget_h_
#define __XmliaWidget_h_

#include "tools/ElementObserver_ABC.h"
#include "gaming/CommandHandler_ABC.h"

namespace kernel
{
    class Controllers;
}

class UserProfile;
class Publisher_ABC;
class CommandPublisher;
class CommandHandler;

// =============================================================================
/** @class  XmliaWidget
    @brief  XmliaWidget
*/
// Created: SBO 2008-06-11
// =============================================================================
class XmliaWidget : public QListView
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< UserProfile >
                 , public CommandHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             XmliaWidget( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler );
    virtual ~XmliaWidget();
    //@}

signals:
    //! @name Signals
    //@{
    void Selected( const UserProfile& profile );
    //@}

private slots:
    //! @name Operations
    //@{
    void OnDoubleClicked( QListViewItem* item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    XmliaWidget( const XmliaWidget& );            //!< Copy constructor
    XmliaWidget& operator=( const XmliaWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Receive( const Command& command );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    CommandHandler& handler_;
    std::auto_ptr< CommandPublisher > publisher_;
    QLabel* text_;
    //@}
};

#endif // __XmliaWidget_h_
