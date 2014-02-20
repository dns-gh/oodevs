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

#include <boost/noncopyable.hpp>
#include "clients_gui/WidgetLanguageObserver_ABC.h"
#include <QtGui/qmainwindow.h>
#include <QtGui/qsystemtrayicon.h>

namespace kernel
{
    class Controllers;
}

namespace tools
{
    class Loader_ABC;
}

class Application;
class ExerciseContainer;
class Config;
class QStackedWidget;
class SessionTray;

// =============================================================================
/** @class  MainWindow
    @brief  MainWindow
*/
// Created: SBO 2008-02-21
// =============================================================================
class MainWindow : public gui::WidgetLanguageObserver_ABC< QMainWindow >
                 , private boost::noncopyable
{

    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MainWindow( Application& app, Config& config,
                         const tools::Loader_ABC& fileLoader,
                         kernel::Controllers& controllers,
                         ExerciseContainer& exercises );
    virtual ~MainWindow();
    //@}

public slots:
    //! @name Slots
    //@{
    void Maximize( QSystemTrayIcon::ActivationReason reason );
    //@}

protected:
    //! @name From QWidget
    //@{
    virtual void resizeEvent( QResizeEvent * );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    void SetStyle();
    void CenterWindow();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< SessionTray > sessionTray_;
    QStackedWidget* pages_;
    //@}
};

#endif // __MainWindow_h_
