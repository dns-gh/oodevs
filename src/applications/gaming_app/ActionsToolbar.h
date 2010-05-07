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
    class ActionsFilter_ABC;
    class ActionsModel;
}

// =============================================================================
/** @class  ActionsToolbar
    @brief  ActionsToolbar
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionsToolbar : public QHBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ActionsToolbar( QWidget* parent, actions::ActionsModel& actions, const tools::ExerciseConfig& config, kernel::Controllers& controllers );
    virtual ~ActionsToolbar();
    //@}

    //! @name Operations
    //@{
    void SetFilter( const actions::ActionsFilter_ABC& filter );
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
    //@}

private slots:
    //! @name Helpers
    //@{
    void Load();
    void Save();
    void Purge();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actions_;
    const tools::ExerciseConfig& config_;
    const actions::ActionsFilter_ABC* filter_;
    QToolButton*  loadBtn_;
    QToolButton*  saveBtn_;
    QToolButton*  purgeBtn_;
    QMessageBox*  confirmation_;
    //@}
};

#endif // __ActionsToolbar_h_
