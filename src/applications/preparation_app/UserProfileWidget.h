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

namespace kernel
{
    class Controllers;
    class DictionaryType;
    class Entity_ABC;
    class ExtensionTypes;
}

namespace gui
{
    class ItemFactory_ABC;
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
class UserProfileWidget : public QTabWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory,
                                gui::EntitySymbols& icons, const kernel::ExtensionTypes& extensions,
                                ProfilesChecker_ABC& checker, Model& model );
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
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UserProfileWidget( const UserProfileWidget& );            //!< Copy constructor
    UserProfileWidget& operator=( const UserProfileWidget& ); //!< Assignment operator
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
    QCheckBox* supervisor_;
    UserProfileRights_ABC* unitRights_;
    UserProfileRights_ABC* populationRights_;
    //@}
};

#endif // __UserProfileWidget_h_
