// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __BattleCenterLauncherPage_h_
#define __BattleCenterLauncherPage_h_

#include "ScenarioLauncherPage.h"

// =============================================================================
/** @class  BattleCenterLauncherPage
    @brief  BattleCenterLauncherPage
*/
// Created: SBO 2008-10-15
// =============================================================================
class BattleCenterLauncherPage : public ScenarioLauncherPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             BattleCenterLauncherPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const frontend::Config& config, frontend::LauncherClient& launcher, gui::LinkInterpreter_ABC& interpreter );
    virtual ~BattleCenterLauncherPage();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BattleCenterLauncherPage( const BattleCenterLauncherPage& );            //!< Copy constructor
    BattleCenterLauncherPage& operator=( const BattleCenterLauncherPage& ); //!< Assignment operator
    //@}


 private slots:
    //! @name Slots
    //@{
    virtual void OnStart();
    //@}
    
    //! @name Helpers
    //@{
    virtual std::string BuildSessionName() const;
    //@}
};

#endif // __BattleCenterLauncherPage_h_
