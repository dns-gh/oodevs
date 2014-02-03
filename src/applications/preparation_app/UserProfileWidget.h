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

#include <boost/noncopyable.hpp>
#include "clients_gui/RichWidget.h"

namespace kernel
{
    class Controllers;
    class DictionaryType;
    class Entity_ABC;
    class ExtensionTypes;
}

namespace gui
{
    class EntitySymbols;
}

class UserProfile;
class UserProfileRights_ABC;
class Model;
class ProfilesChecker_ABC;

// =============================================================================
/** @class  UserProfileWidget
    @brief  UserProfileWidget
*/
// Created: SBO 2007-01-16
// =============================================================================
class UserProfileWidget : public gui::RichWidget< QTabWidget >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileWidget( const QString& objectName, QWidget* parent, kernel::Controllers& controllers, const gui::EntitySymbols& icons,
                                const kernel::ExtensionTypes& extensions, ProfilesChecker_ABC& checker, Model& model );
    virtual ~UserProfileWidget();
    //@}

    //! @name Operations
    //@{
    void Display( UserProfile& profile );
    void SetEnabled( bool enabled );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnLoginChanged();
    void OnPasswordChanged( const QString& text );
    void OnSupervisorChanged( bool supervisor );
    void OnTimeControlChanged( bool timeControl );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::ExtensionTypes& extensions_;
    ProfilesChecker_ABC& checker_;
    Model& model_;
    UserProfile* profile_;
    QLineEdit* login_;
    QLineEdit* password_;
    QLineEdit* automats_;
    QLineEdit* knowledgeGroups_;
    QCheckBox* supervisor_;
    QCheckBox* timeControl_;
    UserProfileRights_ABC* unitRights_;
    UserProfileRights_ABC* populationRights_;
    //@}
};

#endif // __UserProfileWidget_h_
