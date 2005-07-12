//*****************************************************************************
// Created: MCO 2002-03-12
//*****************************************************************************
// $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_AdminService.cpp $
// $Author: Aml $
// $Modtime: 24/04/03 17:16 $
// $Revision: 2 $
// $Workfile: LAU_AdminService.cpp $
//*****************************************************************************

#ifdef __GNUG__
#	pragma implementation
#endif

#include "LAU_pch.h"
#include "LAU_App.h"

#include <windows.h>
#include "LAU_AdminService.h"
//#include "LAU_AdminServiceMsg.h" // Event message ids
//#include "LAU_AdminApp.h"
//#include "SYS/SYS_Exception_ABC.h"
#include <sstream>

////using namespace VTS;
////using namespace SYS;


LAU_AdminService::LAU_AdminService( const char* szServiceName )
    : LAU_AdminService_ABC( szServiceName )
{
    // EMPTY
    OutputLogDebug( "Constructor", __FILE__, __LINE__ );
}

LAU_AdminService::~LAU_AdminService()
{
    // EMPTY
    OutputLogDebug( "Destructor", __FILE__, __LINE__ );
}


//-----------------------------------------------------------------------------
// Name: LAU_AdminService::OnInit
// Created: MCO 2002-03-12
//-----------------------------------------------------------------------------
bool LAU_AdminService::OnInit( int nArgc, char** ppArgv )
{
    OutputLogDebug( "OnInit", __FILE__, __LINE__ );

    LAU_App* pApp = new LAU_App( nArgc, ppArgv );
    pApp->Initialize();

    return true;
}


//-----------------------------------------------------------------------------
// Name: LAU_AdminService::Run
// Created: MCO 2002-03-12
//-----------------------------------------------------------------------------
void LAU_AdminService::Run()
{
    OutputLogDebug( "Run", __FILE__, __LINE__ );

    try
    {
        bool32 bContinue = true;

        LAU_App* pApp = LAU_App::Instance();

        while( bContinue && IsRunning() )
        {
            bContinue = pApp->RunOneFrame();
        }
    }
    catch( MT_ScipioException& exception )
    {
        std::stringstream strMsg;
        strMsg << "Context : "     << exception.GetContext()     << std::endl
               << "File : "        << exception.GetFile()        << std::endl
               << "Line : "        << exception.GetLine()        << std::endl
               << "Message : "     << exception.GetMsg()         << std::endl
               << "Description : " << exception.GetDescription() << std::endl;
          
        MessageBox( 0, strMsg.str().c_str(), "Scipio - SimLauncher - Initialization error", MB_ICONERROR | MB_OK );
    }
    catch( MT_Exception& exception )
    {
        std::stringstream strMsg;
        strMsg << "Context : "     << exception.GetContext()     << std::endl
               << "Message : "     << exception.GetInfo()        << std::endl;
        
        MessageBox( 0, strMsg.str().c_str(), "Scipio - SimLauncher - MT_Exception", MB_ICONERROR | MB_OK );
    }
}

