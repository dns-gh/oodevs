// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileList_h_
#define __UserProfileList_h_

#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

class UserProfile;
class UserProfileWidget;
class UserProfileFactory_ABC;

// =============================================================================
/** @class  UserProfileList
    @brief  UserProfileList
*/
// Created: SBO 2007-01-16
// =============================================================================
class UserProfileList : public QVBox
                      , public kernel::Observer_ABC
                      , public kernel::ElementObserver_ABC< UserProfile >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileList( QWidget* parent, UserProfileWidget& pages, kernel::Controllers& controllers, UserProfileFactory_ABC& factory );
    virtual ~UserProfileList();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged();
    void OnCreate();
    void OnDelete();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UserProfileList( const UserProfileList& );            //!< Copy constructor
    UserProfileList& operator=( const UserProfileList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const UserProfile& profile );
    virtual void NotifyUpdated( const UserProfile& profile );
    virtual void NotifyDeleted( const UserProfile& profile );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const UserProfile* > T_UserProfiles;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&    controllers_;
    UserProfileWidget&      pages_;
    UserProfileFactory_ABC& factory_;
    T_UserProfiles          userProfiles_;
    QListBox*               list_;
    //@}
};

#endif // __UserProfileList_h_
