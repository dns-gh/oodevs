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

#include "ContentPage.h"
#include "DirectoryExerciseLister.h"

namespace kernel
{
    class Controllers;
}

namespace tools
{
    class GeneralConfig;
}

class ProgressPage;
class ExerciseList;
class QSpinBox;

// =============================================================================
/** @class  BattleCenterLauncherPage
    @brief  BattleCenterLauncherPage
*/
// Created: SBO 2008-10-15
// =============================================================================
class BattleCenterLauncherPage : public ContentPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             BattleCenterLauncherPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const tools::GeneralConfig& config );
    virtual ~BattleCenterLauncherPage();
    //@}

private slots:
    //! @name Slots
    //@{
    void SelectExercise( const QString& name );
    void StartExercise();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BattleCenterLauncherPage( const BattleCenterLauncherPage& );            //!< Copy constructor
    BattleCenterLauncherPage& operator=( const BattleCenterLauncherPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update();
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    kernel::Controllers&        controllers_;
    ProgressPage*               progressPage_;
    QLineEdit*                  host_;
    QSpinBox*                   port_;
    ExerciseList*               exercises_;
    QString                     exercise_;
    DirectoryExerciseLister     lister_;
    //@}
};

#endif // __BattleCenterLauncherPage_h_
