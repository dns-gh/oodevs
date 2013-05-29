// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileDialog_h_
#define __UserProfileDialog_h_

#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class EntitySymbols;
}

class UserProfile;
class UserProfileList;
class UserProfileWidget;
class UserProfileFactory_ABC;
class Profile;

// =============================================================================
/** @class  ProfileDialog
    @brief  ProfileDialog
*/
// Created: SBO 2007-01-16
// =============================================================================
class UserProfileDialog : public QDialog
                        , public tools::Observer_ABC
                        , public tools::ElementObserver_ABC< UserProfile >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& itemFactory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons, UserProfileFactory_ABC& factory );
    virtual ~UserProfileDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint () const;
    bool CanBeShown( const Profile& profile ) const;
    virtual void NotifyCreated( const UserProfile& profile );
    virtual void NotifyUpdated( const UserProfile& profile );
    virtual void NotifyDeleted( const UserProfile& profile );

    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnReject();
    void OnProfileChanged( const UserProfile* profile, const UserProfile* editor );
    void OnTimeControlChanged( const QString& text );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UserProfileDialog( const UserProfileDialog& );            //!< Copy constructor
    UserProfileDialog& operator=( const UserProfileDialog& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    UserProfileList* list_;
    UserProfileWidget* pages_;
    QComboBox* timeControlCombo_;
    const UserProfile* controlTimeProfile_;
    //@}
};

#endif // __ProfileDialog_h_
