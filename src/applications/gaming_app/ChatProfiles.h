// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ChatProfiles_h_
#define __ChatProfiles_h_

#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

class UserProfile;

// =============================================================================
/** @class  ChatProfiles
    @brief  ChatProfiles
*/
// Created: SBO 2008-06-11
// =============================================================================
class ChatProfiles : public Q3ListBox
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< UserProfile >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ChatProfiles( QWidget* parent, kernel::Controllers& controllers );
    virtual ~ChatProfiles();
    //@}

signals:
    //! @name Signals
    //@{
    void Selected( const UserProfile& profile );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelected( Q3ListBoxItem* item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ChatProfiles( const ChatProfiles& );            //!< Copy constructor
    ChatProfiles& operator=( const ChatProfiles& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const UserProfile& profile );
    virtual void NotifyDeleted( const UserProfile& profile );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __ChatProfiles_h_
