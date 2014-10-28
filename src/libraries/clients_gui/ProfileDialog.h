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

#include "ModalDialog.h"

namespace kernel
{
    class Controllers;
    class EntityResolver_ABC;
    class Profile_ABC;
    class ProfilesModel_ABC;
}

namespace gui
{
class EntitySymbols;
class UserProfileList;
class UserProfileWidget;

// =============================================================================
/** @class  ProfileDialog
    @brief  ProfileDialog
*/
// Created: SBO 2007-01-16
// =============================================================================
class ProfileDialog : public ModalDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ProfileDialog( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, 
                            const EntitySymbols& icons, const kernel::EntityResolver_ABC& resolver, kernel::ProfilesModel_ABC& profiles );
    virtual ~ProfileDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint() const;
    bool CanBeShown( const kernel::Profile_ABC& profile ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    //@}

private:
    //! @name Member data
    //@{
    UserProfileList* list_;
    //@}
};

}

#endif // __ProfileDialog_h_
