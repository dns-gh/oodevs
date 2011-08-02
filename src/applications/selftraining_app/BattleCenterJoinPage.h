// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __BattleCenterJoinPage_h_
#define __BattleCenterJoinPage_h_

#include "LauncherClientPage.h"
#include "frontend/Profile.h"

namespace frontend
{
    class Exercise_ABC;
    class ExerciseFilter_ABC;
}

namespace kernel
{
    class Controllers;
}

namespace tools
{
    class Loader_ABC;
}

class Config;
class ExerciseList;
class ProgressPage;
class QSpinBox;

// =============================================================================
/** @class  BattleCenterJoinPage
    @brief  BattleCenterJoinPage
*/
// Created: SBO 2008-10-14
// =============================================================================
class BattleCenterJoinPage : public LauncherClientPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             BattleCenterJoinPage( Q3WidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const Config& config, const tools::Loader_ABC& fileLoader, frontend::LauncherClient& launcher );
    virtual ~BattleCenterJoinPage();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnJoin();
    void SelectExercise( const frontend::Exercise_ABC& exercise, const frontend::Profile& profile );
    void ClearSelection();
    void UpdateExerciseList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BattleCenterJoinPage( const BattleCenterJoinPage& );            //!< Copy constructor
    BattleCenterJoinPage& operator=( const BattleCenterJoinPage& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Config& config_;
    const tools::Loader_ABC& fileLoader_;
    ProgressPage* progressPage_;
    QLineEdit* host_;
    QSpinBox* port_;
    ExerciseList* exercises_;
    const frontend::Exercise_ABC* exercise_;
    frontend::Profile profile_;
    std::auto_ptr< frontend::ExerciseFilter_ABC > filter_;
    //@}
};

#endif // __BattleCenterJoinPage_h_
