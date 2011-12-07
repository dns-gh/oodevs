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
    class ExtensionTypes;
}

namespace gui
{
    class ItemFactory_ABC;
    class EntitySymbols;
}

class UserProfileList;
class UserProfileWidget;
class Model;
class ProfilesChecker_ABC;

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
             ProfileDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory,
                            gui::EntitySymbols& icons, Model& model, const kernel::ExtensionTypes& extensions );
    virtual ~ProfileDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnReject();
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
    std::auto_ptr< ProfilesChecker_ABC > pChecher_;
    UserProfileList* list_;
    UserProfileWidget* pages_;
    //@}
};

#endif // __ProfileDialog_h_
