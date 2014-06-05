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
#include "ADN_MainWindow.h"

class ADN_GeneralConfig;

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

private:
    //! @name Member data
    //@{
    std::unique_ptr< const ADN_GeneralConfig > config_;
    std::unique_ptr< ADN_MainWindow > mainWindow_;
    //@}
};

#endif // __ADN_App_h_
