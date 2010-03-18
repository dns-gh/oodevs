// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsToolbar_h_
#define __ActionsToolbar_h_

#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace tools
{
    class ExerciseConfig;
}

namespace actions
{
    class ActionsModel;
}
class ActionsScheduler;
class Publisher_ABC;
class Simulation;

// =============================================================================
/** @class  ActionsToolbar
    @brief  ActionsToolbar
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionsToolbar : public QHBox
                     , public tools::Observer_ABC
                     , public kernel::OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ActionsToolbar( QWidget* parent, actions::ActionsModel& actions, ActionsScheduler& scheduler, const tools::ExerciseConfig& config, kernel::Controllers& controllers );
    virtual ~ActionsToolbar();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionsToolbar( const ActionsToolbar& );            //!< Copy constructor
    ActionsToolbar& operator=( const ActionsToolbar& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void PurgeConfirmed( int result );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private slots:
    //! @name Helpers
    //@{
    void Load();
    void Save();
    void ToggleRecording( bool toggled );
    void Purge();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actions_;
    ActionsScheduler& scheduler_;
    const tools::ExerciseConfig& config_;
    QToolButton*  loadBtn_;
    QToolButton*  recordBtn_;
    QToolButton*  saveBtn_;
    QToolButton*  planningBtn_;
    QPixmap       pixRecord_, pixStop_;
    QMessageBox*  confirmation_;
    //@}
};

#endif // __ActionsToolbar_h_
