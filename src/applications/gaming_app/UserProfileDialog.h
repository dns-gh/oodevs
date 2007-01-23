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
    class ItemFactory_ABC;
    class SymbolIcons;
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
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& itemFactory, const kernel::Profile_ABC& profile, gui::SymbolIcons& icons, UserProfileFactory_ABC& factory );
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
    void OnOk();
    void OnApply();
    void OnCancel();
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
    UserProfileList* list_;
    UserProfileWidget* pages_;
    //@}
};

#endif // __ProfileDialog_h_
