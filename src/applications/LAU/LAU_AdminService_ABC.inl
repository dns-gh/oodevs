//*****************************************************************************
// $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_AdminService_ABC.inl $
// $Author: Aml $
// $Modtime: 13/06/03 11:26 $
// $Revision: 3 $
// $Workfile: LAU_AdminService_ABC.inl $
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::IsRunning
/**
*   @return Whether service is running or not
*
* Should be used in service Run() thread in a while loop
*/
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
inline
bool LAU_AdminService_ABC::IsRunning() const
{
    return bIsRunning_;
}

//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::GetName
/**
*   @return Service name
*/
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
inline
const char* LAU_AdminService_ABC::GetName() const
{
    return szServiceName_;
}


//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::SetInteractWithDesktop
// Created: AML 03-06-13
//-----------------------------------------------------------------------------
inline
void LAU_AdminService_ABC::SetInteractWithDesktop( const bool32 bNewVal )
{
    if( bNewVal )
    {
        status_.dwServiceType |= SERVICE_INTERACTIVE_PROCESS;
    }
    else
    {
        status_.dwServiceType &= ~SERVICE_INTERACTIVE_PROCESS;
    }
}


//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::GetExitCode
/**
*   @return Service exit code
*
* The application should return this value to the system (from main() most likely)
*/
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
inline
unsigned int LAU_AdminService_ABC::GetExitCode() const
{
    return status_.dwWin32ExitCode;
}
