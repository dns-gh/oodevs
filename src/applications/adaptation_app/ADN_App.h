// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_App_h_
#define __ADN_App_h_

#include "clients_gui/Application_ABC.h"

class ADN_MainWindow;
class ADN_Config;

// =============================================================================
/** @class  ADN_App
*/
// Created: APE 2004-12-02
// =============================================================================
class ADN_App : public gui::Application_ABC
{

public:
             ADN_App( int argc, char** argv );
    virtual ~ADN_App();

    //! @name gui::Application_ABC operations
    //@{
    virtual int Run();
    virtual void CreateTranslators();
    //@}

    //! @name Accessors
    //@{
    ADN_MainWindow* GetMainWindow();
    const std::string& GetOutputFile() const;
    //@}

private:
    //! @name Member data
    //@{
    ADN_MainWindow*             mainWindow_;
    std::auto_ptr< ADN_Config > config_;
    std::string                 inputFile_;
    std::string                 outputFile_;
    //@}
};

//-----------------------------------------------------------------------------
// Name: ADN_App::GetMainWindow
// Created: AGN 03-08-05
//-----------------------------------------------------------------------------
inline
ADN_MainWindow* ADN_App::GetMainWindow()
{
    return mainWindow_;
}

// -----------------------------------------------------------------------------
// Name: ADN_App::GetOutputFile
// Created: ABR 2012-07-12
// -----------------------------------------------------------------------------
inline
const std::string& ADN_App::GetOutputFile() const
{
    return outputFile_;
}

#endif // __ADN_App_h_
