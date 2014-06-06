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

namespace kernel
{
    class Controllers;
    class ExtensionTypes;
}

namespace gui
{
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
class ProfileDialog : public ModalDialog
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ProfileDialog( QWidget* parent, kernel::Controllers& controllers, const gui::EntitySymbols& icons,
                            Model& model, const kernel::ExtensionTypes& extensions );
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
    std::unique_ptr< ProfilesChecker_ABC > pChecher_;
    UserProfileList* list_;
    UserProfileWidget* pages_;
    //@}
};

#endif // __ProfileDialog_h_
