//*****************************************************************************
// $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_AdminService.h $
// $Author: Aml $
// $Modtime: 25/04/03 11:28 $
// $Revision: 2 $
// $Workfile: LAU_AdminService.h $
//*****************************************************************************

#ifndef __LAU_AdminService_h_
#define __LAU_AdminService_h_

#ifdef __GNUG__
#	pragma interface
#endif

#include "LAU.h"
#include "LAU_AdminService_ABC.h"

//=============================================================================
/**
*   @class  LAU_AdminService
*   @brief  Specialisation of LAU_AdminService_ABC
*/
// Created: MCO 2002-03-12
//=============================================================================
class LAU_AdminService : public LAU_AdminService_ABC
{
public:
    LAU_AdminService( const char* szServiceName );
    virtual ~LAU_AdminService();

protected:
	virtual bool OnInit( int nArgc, char** ppArgv );
    virtual void Run();
};

#endif // __LAU_AdminService_h_
