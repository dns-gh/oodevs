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
#include "clients_gui/LanguageChangeObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class LinkInterpreter_ABC;
}

namespace frontend
{
    class LauncherClient;
}

namespace tools
{
    class Loader_ABC;
}

class Config;
class Q3WidgetStack;
class SessionTray;

// =============================================================================
/** @class  MainWindow
    @brief  MainWindow
*/
// Created: SBO 2008-02-21
// =============================================================================
class MainWindow : public gui::LanguageChangeObserver_ABC< Q3MainWindow >
                 , private boost::noncopyable
{

    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MainWindow( Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, frontend::LauncherClient& launcherClient );
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
    std::auto_ptr< gui::LinkInterpreter_ABC > interpreter_;
    Q3WidgetStack* pages_;
    //@}
};

#endif // __MainWindow_h_
