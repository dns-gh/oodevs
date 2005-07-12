//*****************************************************************************
//
// $Created: JDY 03-06-19 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_App.h $
// $Author: Ape $
// $Modtime: 14/04/05 15:56 $
// $Revision: 5 $
// $Workfile: ADN_App.h $
//
//*****************************************************************************

#ifndef __ADN_App_h_
#define __ADN_App_h_

#include <qapplication.h>

class ADN_MainWindow;


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

    void Initialize( uint nArgc, char** ppArgv );

public:
    ADN_MainWindow* GetMainWindow();

public:
    static ADN_App* pApplication_;

private:
    ADN_MainWindow* pMainWindow_; 
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