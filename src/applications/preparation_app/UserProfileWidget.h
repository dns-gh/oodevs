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
class UserProfileControls_ABC;
class ControlsChecker_ABC;
class Model;

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
                                ControlsChecker_ABC& checker, Model& model );
    virtual ~UserProfileWidget();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    void Display( UserProfile& profile );
    void SetEnabled( bool enabled );
    void Show();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnLoginChanged();
    void OnPasswordChanged( const QString& text );
    void OnUserRole( const QString& role );
    void OnHideFilterChanged( int state );
    void OnShowFilterChanged( int state );
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
    void Update();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::ExtensionTypes& extensions_;
    ControlsChecker_ABC& checker_;
    Model& model_;
    UserProfile* profile_;
    QLineEdit* login_;
    QLineEdit* password_;
    Q3GroupBox* userRoleGroup_;
    QLabel* userRoleLabel_;
    QComboBox* userRole_;
    kernel::DictionaryType* userRoleDico_;
    std::string dicoKind_;
    std::string dicoLanguage_;
    UserProfileControls_ABC* pPopulations_;
    UserProfileControls_ABC* pUnits_;
    QLabel* informationControls_;
    std::vector< std::string > supervisors_;
    QCheckBox* pHidefilter_;
    QCheckBox* pShowFilter_;
    //@}
};

#endif // __UserProfileWidget_h_
