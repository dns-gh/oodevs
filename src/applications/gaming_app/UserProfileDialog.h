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

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class EntitySymbols;
}

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
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileDialog( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons, UserProfileFactory_ABC& factory );
    virtual ~UserProfileDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint () const;
    bool CanBeShown( const Profile& profile ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnReject();
    //@}

private:
    //! @name Member data
    //@{
    UserProfileList* list_;
    UserProfileWidget* pages_;
    //@}
};

#endif // __ProfileDialog_h_
