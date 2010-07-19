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

#include "ContentPage.h"
#include "Profile.h"

namespace kernel
{
    class Controllers;
}

class Config;
class ExerciseList;
class NetworkExerciseLister;
class ProgressPage;
class QSpinBox;

// =============================================================================
/** @class  BattleCenterJoinPage
    @brief  BattleCenterJoinPage
*/
// Created: SBO 2008-10-14
// =============================================================================
class BattleCenterJoinPage : public ContentPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             BattleCenterJoinPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const Config& config, NetworkExerciseLister& lister );
    virtual ~BattleCenterJoinPage();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnJoin();
    void SelectExercise( const QString& exercise, const Profile& profile );
    void ReloadExerciseList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BattleCenterJoinPage( const BattleCenterJoinPage& );            //!< Copy constructor
    BattleCenterJoinPage& operator=( const BattleCenterJoinPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Config& config_;
    NetworkExerciseLister& exerciseLister_;
    ProgressPage* progressPage_;
    QLineEdit* host_;
    QSpinBox* port_;
    ExerciseList* exercises_;
    QString exercise_;
    Profile profile_;
    //@}
};

#endif // __BattleCenterJoinPage_h_
