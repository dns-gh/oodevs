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

#include "clients_gui/RichWidget.h"

namespace kernel
{
    class Controllers;
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
             UserProfileWidget( const QString& objectName, QWidget* parent, kernel::Controllers& controllers,
                                const gui::EntitySymbols& icons, Model& model );
    virtual ~UserProfileWidget();
    //@}

    //! @name Operations
    //@{
    void Display( UserProfile& profile );
    void SetChecker( const ProfilesChecker_ABC* pChecker );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnLoginChanged();
    void OnPasswordChanged( const QString& text );
    void OnSupervisorChanged( bool supervisor );
    void OnTimeControlChanged( bool timeControl );
    void UpdateAutomatsAndKnowledgeGroups();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const ProfilesChecker_ABC* pChecker_;
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
