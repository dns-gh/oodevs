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

#include "clients_gui/ModalDialog.h"
#include "tools/ElementObserver_ABC.h"

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

class UserProfile;
class UserProfileList;
class UserProfileWidget;
class Model;
class ControlsChecker_ABC;

// =============================================================================
/** @class  ProfileDialog
    @brief  ProfileDialog
*/
// Created: SBO 2007-01-16
// =============================================================================
class ProfileDialog : public ModalDialog
                    , public tools::Observer_ABC
                    , public tools::ElementObserver_ABC< UserProfile >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ProfileDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory,
                            const gui::EntitySymbols& icons, Model& model, const kernel::ExtensionTypes& extensions );
    virtual ~ProfileDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint() const;
    virtual void showEvent( QShowEvent* pEvent );
    virtual void hideEvent( QHideEvent* pEvent );
    virtual void NotifyCreated( const UserProfile& profile );
    virtual void NotifyDeleted( const UserProfile& profile );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnReject();
    void OnProfileChanged( const UserProfile* profile, const UserProfile* editor );
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
    kernel::Controllers& controllers_;
    std::auto_ptr< ControlsChecker_ABC > pChecher_;
    UserProfileList* list_;
    UserProfileWidget* pages_;
    QComboBox* timeControlCombo_;
    //@}
};

#endif // __ProfileDialog_h_
