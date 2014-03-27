// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileWidget_h_
#define __UserProfileWidget_h_

#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class EntitySymbols;
}

class UserProfile;
class UserProfileRights_ABC;

// =============================================================================
/** @class  UserProfileWidget
    @brief  UserProfileWidget
*/
// Created: SBO 2007-01-16
// =============================================================================
class UserProfileWidget : public QTabWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileWidget( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons );
    virtual ~UserProfileWidget();
    //@}

    //! @name Operations
    //@{
    void Display( const UserProfile& profile );
    void Commit();
    void Clean();
    void Reset();
    //@}

private:
    //! @name Helpers
    //@{
    bool NeedsSaving() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::SafePointer< UserProfile > selectedProfile_;
    const kernel::Profile_ABC& profile_;
    std::auto_ptr< UserProfile > editedProfile_;
    UserProfileRights_ABC* unitRights_;
    UserProfileRights_ABC* populationRights_;
    QLineEdit* login_;
    QLineEdit* password_;
    QCheckBox* supervisor_;
    QCheckBox* timeControl_;
    //@}
};

#endif // __UserProfileWidget_h_
