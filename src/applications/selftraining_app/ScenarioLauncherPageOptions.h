// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ScenarioLauncherPageOptions_h_
#define __ScenarioLauncherPageOptions_h_

#include "ContentPage.h"

namespace tools
{
    class GeneralConfig;
}

namespace kernel
{
    class Controllers;
}

class ProfileList;
class ProgressPage;

// =============================================================================
/** @class  ScenarioLauncherPageOptions
    @brief  ScenarioLauncherPageOptions
*/
// Created: RDS 2008-09-08
// =============================================================================
class ScenarioLauncherPageOptions : public ContentPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScenarioLauncherPageOptions( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const tools::GeneralConfig& config );
    virtual ~ScenarioLauncherPageOptions();
    //@}

public slots:
    //! @name Slots
    //@{
    void Update( const QString& exercise );
    void Start();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScenarioLauncherPageOptions( const ScenarioLauncherPageOptions& );            //!< Copy constructor
    ScenarioLauncherPageOptions& operator=( const ScenarioLauncherPageOptions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateSession( const QString& exercise, const QString& session );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    kernel::Controllers& controllers_;
    ProgressPage* progressPage_;
    ProfileList* profiles_;
    QString exercise_;
    //@}
};

#endif // __ScenarioLauncherPageOptions_h_
