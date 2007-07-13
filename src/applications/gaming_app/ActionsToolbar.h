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

class ActionsModel;
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
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ActionsToolbar( QWidget* parent, ActionsModel& actions, kernel::Controllers& controllers, Publisher_ABC& publisher, const Simulation& simulation );
    virtual ~ActionsToolbar();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionsToolbar( const ActionsToolbar& );            //!< Copy constructor
    ActionsToolbar& operator=( const ActionsToolbar& ); //!< Assignment operator
    //@}

private slots:
    //! @name Helpers
    //@{
    void Load();
    void Save();
    void Play();
    void Record();
    //@}

private:
    //! @name Member data
    //@{
    ActionsModel& actions_;
    std::auto_ptr< ActionsScheduler > scheduler_;
    QToolButton*  loadBtn_;
    QToolButton*  recordBtn_;
    QToolButton*  playBtn_;
    QToolButton*  saveBtn_;
    QPixmap       pixRecord_, pixStop_;
    //@}
};

#endif // __ActionsToolbar_h_
