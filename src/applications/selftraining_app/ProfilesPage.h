// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ProfilesPage_h_
#define __ProfilesPage_h_

#include "MenuPage.h"

namespace kernel
{
    class Controllers;
}

class Config;
class HomePage;
class NetworkExerciseLister;
class OptionsPage;

// =============================================================================
/** @class  ProfilesPage
    @brief  ProfilesPage
*/
// Created: JSR 2010-07-12
// =============================================================================
class ProfilesPage : public MenuPage
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ProfilesPage( QWidgetStack* pages, Config& config, kernel::Controllers& controllers, NetworkExerciseLister& lister );
    virtual ~ProfilesPage();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProfilesPage( const ProfilesPage& );            //!< Copy constructor
    ProfilesPage& operator=( const ProfilesPage& ); //!< Assignment operator
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTerrain();
    void OnUser();
    void OnAdvancedUser();
    void OnAdministrator();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnOptions();
    //@}

private:
    //! @name Member data
    //@{
    Config& config_;
    HomePage* homePage_;
    OptionsPage* optionsPage_;
    //@}
};

#endif // __ProfilesPage_h_
