// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MainWindow_h_
#define __MainWindow_h_

#include <qmainwindow.h>

namespace kernel
{
    class Controllers;
}

class QWidgetStack;

class Config;
class ExerciseService;
class NetworkExerciseLister;

// =============================================================================
/** @class  MainWindow
    @brief  MainWindow
*/
// Created: SBO 2008-02-21
// =============================================================================
class MainWindow : public QMainWindow
{

    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit MainWindow( kernel::Controllers& controllers );
    virtual ~MainWindow();
    //@}

public slots:
    //! @name Slots
    //@{
    void Maximize();
    //@}

protected:
    //! @name From QWidget
    //@{
    virtual void resizeEvent( QResizeEvent * );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MainWindow( const MainWindow& );            //!< Copy constructor
    MainWindow& operator=( const MainWindow& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SetStyle();
    void CenterWindow();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Config >                   config_;
    QWidgetStack*                             pages_;
    std::auto_ptr< ExerciseService >          exercises_ ;
    std::auto_ptr< NetworkExerciseLister >    exerciseLister_;
    //@}
};

#endif // __MainWindow_h_
