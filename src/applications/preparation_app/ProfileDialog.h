// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfileDialog_h_
#define __ProfileDialog_h_

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
    class SymbolIcons;
}

class UserProfileList;
class UserProfileWidget;
class ProfilesModel;

// =============================================================================
/** @class  ProfileDialog
    @brief  ProfileDialog
*/
// Created: SBO 2007-01-16
// =============================================================================
class ProfileDialog : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ProfileDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::SymbolIcons& icons, ProfilesModel& model );
    virtual ~ProfileDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint () const;
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
    ProfileDialog( const ProfileDialog& );            //!< Copy constructor
    ProfileDialog& operator=( const ProfileDialog& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const ProfilesModel& model_;
    UserProfileList* list_;
    UserProfileWidget* pages_;
    //@}
};

#endif // __ProfileDialog_h_
