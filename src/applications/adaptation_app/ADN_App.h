// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_App_h_
#define __ADN_App_h_

#include "clients_gui/Application_ABC.h"

class ADN_MainWindow;
class ADN_Config;

namespace gui
{
    class ApplicationMonitor;
}

// =============================================================================
/** @class  ADN_App
*/
// Created: APE 2004-12-02
// =============================================================================
class ADN_App : public gui::Application_ABC
{
public:
             ADN_App( gui::ApplicationMonitor& monitor, int argc, char** argv );
    virtual ~ADN_App();

    //! @name gui::Application_ABC operations
    //@{
    virtual int Run();
    virtual void CreateTranslators();
    //@}

    //! @name Accessors
    //@{
    static QMainWindow* GetMainWindow();
    //@}

private:
    //! @name Member data
    //@{
    ADN_MainWindow*             mainWindow_;
    tools::Path                 inputFile_;
    tools::Path                 outputFile_;
    tools::Path                 newFile_;
    //@}
};

#endif // __ADN_App_h_
