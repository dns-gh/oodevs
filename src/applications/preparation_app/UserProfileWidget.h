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

#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class DictionaryType;
    class Entity_ABC;
    class ExtensionTypes;
    class ModelLoaded;
    class ModelUnLoaded;
}

namespace gui
{
    class ItemFactory_ABC;
    class EntitySymbols;
}

class UserProfile;
class UserProfileRights_ABC;
class ControlsChecker_ABC;

// =============================================================================
/** @class  UserProfileWidget
    @brief  UserProfileWidget
*/
// Created: SBO 2007-01-16
// =============================================================================
class UserProfileWidget : public QTabWidget
                        , public tools::Observer_ABC
                        , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                        , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory,
                                gui::EntitySymbols& icons, const kernel::ExtensionTypes& extensions,
                                ControlsChecker_ABC& checker );
    virtual ~UserProfileWidget();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    void Display( UserProfile& profile );
    void SetEnabled( bool enabled );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnLoginChanged( const QString& text );
    void OnPasswordChanged( const QString& text );
    void OnSupervisorChanged( bool supervisor );
    void OnUserRoleActivation( bool enable );
    void OnUserRole( const QString& role );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UserProfileWidget( const UserProfileWidget& );            //!< Copy constructor
    UserProfileWidget& operator=( const UserProfileWidget& ); //!< Assignment operator
    //@}

private:
    //! @name Helpers
    //@{
    void ActivateControls();
    void DeactivateControls();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::ExtensionTypes& extensions_;
    ControlsChecker_ABC& checker_;
    UserProfile* profile_;
    QLineEdit* login_;
    QLineEdit* password_;
    QLabel* supervisorLabel_;
    QCheckBox* supervisor_;
    Q3GroupBox* userRoleGroup_;
    QLabel* userRoleLabel_;
    QComboBox* userRole_;
    kernel::DictionaryType* userRoleDico_;
    std::string dicoKind_;
    std::string dicoLanguage_;
    QStackedWidget* pPopulations_;
    QStackedWidget* pUnits_;
    QStackedWidget* pInformations_;
    std::vector< std::string > supervisors_;
    //@}
};

#endif // __UserProfileWidget_h_
