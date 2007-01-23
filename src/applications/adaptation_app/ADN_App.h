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

#include <qapplication.h>

class ADN_MainWindow;
class ADN_Config;

// =============================================================================
/** @class  ADN_App
*/
// Created: APE 2004-12-02
// =============================================================================
class ADN_App 
: public QApplication
{
    MT_COPYNOTALLOWED( ADN_App );

public:
     ADN_App( int argc, char** argv );
    ~ADN_App();

    bool Initialize( const std::string& inputFile, const std::string& outputFile );

public:
    ADN_MainWindow* GetMainWindow();

public:
    static ADN_App* pApplication_;

private:
    ADN_MainWindow* pMainWindow_; 
    ADN_Config*     config_;
};


//-----------------------------------------------------------------------------
// Name: ADN_App::GetMainWindow
// Created: AGN 03-08-05
//-----------------------------------------------------------------------------
inline
ADN_MainWindow* ADN_App::GetMainWindow()
{
    return pMainWindow_;
}


#endif // __ADN_App_h_