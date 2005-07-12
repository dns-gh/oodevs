// ntservice.h
//
// Definitions for LAU_AdminService_ABC
//

#ifndef _LAU_ADMINSERVICE_ABC_H_
#define _LAU_ADMINSERVICE_ABC_H_

#include "LAU.h"
#include <winsvc.h>


//=============================================================================
/**
*   @class  LAU_AdminService_ABC
*   @brief  Implements basics for a Windows (NT/2000) Service
*   @par    Using example
*   @code
*     Design a class which inherits from LAU_AdminService_ABC :
*
*        class LAU_AdminService : public LAU_AdminService_ABC
*        {
*        public:
*            LAU_AdminService( const char* szServiceName );
*            virtual ~LAU_AdminService();
*
*        protected:
*	         virtual bool OnInit( int nArgc, char** ppArgv );
*            virtual void Run();
*            virtual void OnStop();
*        };
*
*     Implement needed callbacks :
*      
*        bool LAU_AdminService::OnInit( int nArgc, char** ppArgv )
*        {
*            // do whatever your application needs to initialize
*
*            return bInit;
*        }
*
*        void LAU_AdminService::Run()
*        {
*            while( IsRunning() )
*            {
*                // this is application main loop
*                // do whatever your application does
*            }
*        }
*
*        void LAU_AdminService::OnStop()
*        {
*            // do your application cleanups
*            // WARNING it should NOT take more than 30 sec to close
*            //  or Windows Services Manager will think the service
*            //  is no more responding.
*            // If you need more than 30 sec, spawn a thread to do
*            //  the cleanup and return immediatly from OnStop().
*        }
*
*     The 'main' should look like this :
*
*        int main( int nArgc, char* ppArgv[] )
*        {
*            LAU_AdminService service( "MyService" );
*
*            service.StartService();
*
*            // When we get here, the service has been stopped
*            return service.GetExitCode();
*        }
*
*     This code assumes that the service MyService has already been
*      installed, if it has not the StartService() will fail and a
*      corresponding error will be added in Windows events log.
*     The application is not supposed to be run manually but only
*      throught Windows Services Manager console, therefore we can
*      suppose the service has indeed been installed.
*
*     WARNING: because of Windows way of managing services (and system
*              callbacks) there need to be some static variables and methods
*              in the class implementation without any proper means to
*              ensure a singleton design. YOU SHOULD NOT use several services
*              in a given application.
*   @endcode
*/
// Created: MCO 2002-03-13
//=============================================================================
class LAU_AdminService_ABC
{
public:
    //-------------------------------------------------------------------------
    /** @name Constructor/Destructor */
    //-------------------------------------------------------------------------
    //@{
    LAU_AdminService_ABC( const char* szServiceName ); //!< WARNING : Only ONE service per application can be used
    virtual ~LAU_AdminService_ABC();

    static LAU_AdminService_ABC* Instance();
    //@}

    //-------------------------------------------------------------------------
    /** @name Service name accessor */
    //-------------------------------------------------------------------------
    //@{
    const char* GetName() const;
    //@}


    //-------------------------------------------------------------------------
    /** @name Options (to put before starting service?) */
    //-------------------------------------------------------------------------
    //@{
    void SetInteractWithDesktop( const bool32 bNewVal = true );
    //@}

    
    //-------------------------------------------------------------------------
    /** @name Service installation/uninstallation */
    //-------------------------------------------------------------------------
    //@{
    bool Install();
    bool Uninstall();
    bool IsInstalled() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Service running */
    //-------------------------------------------------------------------------
    //@{
    bool         StartService();     //!< Should be called at end of application initialization
    bool         IsRunning() const;  //!< Should be used in Run() to test if a stop message has shown up
    unsigned int GetExitCode()const; //!< Application should return this to system at exit
    //@}

    //-------------------------------------------------------------------------
    /** @name Windows Event Log */
    //-------------------------------------------------------------------------
    //@{
    void Log( WORD wType, const char* pszS1 );
    //@}

protected:
    //-------------------------------------------------------------------------
    /** @name Service initialisation */
    //-------------------------------------------------------------------------
    //@{
	virtual bool OnInit( int nArgc, char** ppArgv );
    //@}
    //-------------------------------------------------------------------------
    /** @name Main thread to handle service job */
    //-------------------------------------------------------------------------
    //@{
    virtual void Run() = 0;
    //@}
    //-------------------------------------------------------------------------
    /** @name Alternate thread to handle user operations */
    //-------------------------------------------------------------------------
    //@{
    virtual void OnStop();
    virtual void OnInterrogate();
    virtual void OnPause();
    virtual void OnContinue();
    virtual void OnShutdown();
    virtual bool OnUserControl( DWORD dwOpcode );
    //@}

private:
    void LogEvent  ( WORD wType, DWORD dwID, const char* pszS1 = NULL, const char* pszS2 = NULL, const char* pszS3 = NULL ); //$$ FIXME, quite weird syntax
    bool Initialize( DWORD dwArgc, LPTSTR* lpszArgv );
    void SetStatus ( DWORD dwState );

    static void CALLBACK ServiceMain( DWORD dwArgc, LPTSTR* lpszArgv );
    static void CALLBACK Handler    ( DWORD dwOpcode );

    // data members
    char                    szServiceName_[64];

    SERVICE_STATUS_HANDLE   hServiceStatus_;
    SERVICE_STATUS          status_;    // changé le dwWaitHint si le demmarrage du service prend trop de temps (pour ne pas avoir d'erreur)
    
    bool                    bIsRunning_;

    HANDLE                  hEventSource_;

    // static data
    static LAU_AdminService_ABC* pInstance_;
};

#include "LAU_AdminService_ABC.inl"

#endif // _LAU_ADMINSERVICE_ABC_H_
