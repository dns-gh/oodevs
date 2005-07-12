//*****************************************************************************
//
// $Created : AML 03-04-01 $
//  $Archive: /MVW_v10/Build/SDK/LAU_MOS_Part/src/LMP_App.h $
//   $Author: Aml $
//  $Modtime: 17/04/03 15:19 $
// $Revision: 5 $
// $Workfile: LMP_App.h $
//
//*****************************************************************************

#ifndef __LMP_App_h_
#define __LMP_App_h_

#include "LMP.h"

#include <qapplication.h>

#include "LMP_pch.h"	// car appeler depuis les moc

class LMP_Workspace;
class LMP_MainWindow;

//*****************************************************************************
// Object : PUT THE COMMENTS ON THE CLASS HERE
// Created: AML 03-04-01
//*****************************************************************************
class LMP_App : public QApplication
{
    Q_OBJECT;

    MT_COPYNOTALLOWED( LMP_App );

public:
     LMP_App( int argc, char** argv );
    ~LMP_App();

    bool32 Run       (); //!< return true if the application must be restarted

    const std::string& GetLauncherHostname() const;


private slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotQuit();
    //@}
    
private:
    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    bool ParseCmdArgs( int nArgc, char** ppArgv );
    //@}
    
private:

    bool32              bRunning_;
    bool32              bDisplay_;

    LMP_Workspace*      pWorkSpace_;    // (owned)
    LMP_MainWindow*     pMainWindow_;   // (owned)

    std::string         strLauncherHostname_;

};


#ifdef USE_INLINE
#   include "LMP_App.inl"
#endif

#endif // __LMP_App_h_
