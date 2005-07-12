//*****************************************************************************
//
// Name     : LAU_Workspace.h
//
// $Created : AML 03-03-24 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 24 $
// $Workfile: LAU_Workspace.h $
//
//*****************************************************************************

#ifndef __LAU_Workspace_h_
#define __LAU_Workspace_h_

#include "LAU.h"


#include "LAU_NetMgr.h"
#include "LAU_ASN_Messages.h"

#include "LAU_SIM.h"

#include "MT/MT_Time/MT_TimeSpan.h"
#include "ProcessManager.h"

class LAU_Client_ABC;
class LAU_App;


//*****************************************************************************
// Object : PUT THE COMMENTS ON THE CLASS HERE
// Created: AML 03-03-24
//*****************************************************************************
class LAU_Workspace
{
	MT_COPYNOTALLOWED( LAU_Workspace )


    typedef std::vector< ASN1T_IdAndLibelle >                    T_ASN1T_IdAndLibelleVector;
    typedef T_ASN1T_IdAndLibelleVector::iterator                 IT_ASN1T_IdAndLibelleVector;
    typedef T_ASN1T_IdAndLibelleVector::const_iterator           CIT_ASN1T_IdAndLibelleVector;

    typedef std::vector< ASN1T_CheckPointInfo >                    T_ASN1T_CheckPointInfoVector;
    typedef T_ASN1T_CheckPointInfoVector::iterator                 IT_ASN1T_CheckPointInfoVector;
    typedef T_ASN1T_CheckPointInfoVector::const_iterator           CIT_ASN1T_CheckPointInfoVector;


    typedef std::vector< ASN1T_IdExAndLaunchInformation >                    T_ASN1T_IdExAndLaunchInformationVector;
    typedef T_ASN1T_IdExAndLaunchInformationVector::iterator                 IT_ASN1T_IdExAndLaunchInformationVector;
    typedef T_ASN1T_IdExAndLaunchInformationVector::const_iterator           CIT_ASN1T_IdExAndLaunchInformationVector;


public:
    explicit        LAU_Workspace( LAU_App& app, MT_InputArchive_ABC& archive );
    virtual         ~LAU_Workspace();

    //-----------------------------------------------------------------------------
    /** @name Initialization */
    //-----------------------------------------------------------------------------
    //@{ 
    void            Initialize();
    void            Terminate ();
    //@}

    //-----------------------------------------------------------------------------
    /** @name Execution */
    //-----------------------------------------------------------------------------
    //@{ 
    void            Start();
    void            Stop();
	bool          Execute();

    void            PrintAllSimProcess();

    void            CheckExecutionOfAllSim();   // ping les applications de chaque sim, et/ou verifie les numeros de process
    //@}


    //-----------------------------------------------------------------------------
    /** @name Message reception */
    //-----------------------------------------------------------------------------
    //@{ 
    void            ReceiveMsg_MosLau_RequestListMission        ( LAU_Client_ABC& client );
    void            ReceiveMsg_MosLau_LaunchSim                 ( LAU_Client_ABC& client, const uint32 nExerciceID, const uint32 nScenarioID, const char* szSimCommandLine );
    void            ReceiveMsg_MosLau_RequestListSim            ( LAU_Client_ABC& client );
    void            ReceiveMsg_MosLau_RequestListCheckpoint     ( LAU_Client_ABC& client, const uint32 nExerciceID );
    void            ReceiveMsg_MosLau_LaunchSimFromCheckpoint   ( LAU_Client_ABC& client, const uint32 nExerciceID, const uint32 nCheckpointID, const char* szSimCommandLine );
    //@}
    
    
    //-----------------------------------------------------------------------------
    /** @name Message emission */
    //-----------------------------------------------------------------------------
    //@{ 
    void            SendMsg_LauMos_ListMission              ( LAU_Client_ABC& client );
    void            SendMsg_LauMos_SimLaunched              ( LAU_Client_ABC& client, const uint32 nExerciceID, const ASN1T_EnumCtrlErrorCodeLauncher nErrorCode, const uint16 nNumPort = 0, const uint32 nDinMagicNumber = 0 );
    void            SendMsg_LauMos_ListSim                  ( LAU_Client_ABC& client );
    void            SendMsg_LauMos_SimFailure               ( LAU_Client_ABC& client, const uint32 nExerciceID );
    void            SendMsg_LauMos_ListCheckpoint           ( LAU_Client_ABC& client, const uint32 nExerciceID );

    //@}

    //-----------------------------------------------------------------------------
    /** @name Accessors */
    //-----------------------------------------------------------------------------
    //@{
    const MT_TimeSpan& GetTimeBetweenTwoPings() const;
    const MT_TimeSpan& GetTimeMaxWaitPong() const;
    const MT_TimeSpan& GetTimeMaxWaitInit() const;
    
    //@}

    //-----------------------------------------------------------------------------
    /** @name Public Function */
    //-----------------------------------------------------------------------------
    //@{ 
    void            MosHasDisconnect    ( LAU_MOS* pThisMosIsNoMore );  // un MOS a terminé (failure ou pas)
    LAU_SIM*        FindSim             ( const uint32 nExerciceID );   // retourne la SIM avec cet ID ou NULL si aucune

    bool          KillSimRemote       ( const uint32 nExerciceID );   // detruit le processus lié a une sim (comme ca direct)
    //@}

    //-----------------------------------------------------------------------------
    /** @name Private Function */
    //-----------------------------------------------------------------------------
    //@{ 
private:
    void ComputeListMission  ();
    void ComputeListCheckpoint( const int nExerciceID = -1 );    // calcule la liste des checkpoints lié a un exercice, ou a tout les exercice si nExerciceID == -1
    bool LaunchSim           ( LAU_MOS* pMosClient, const uint32 nExerciceID, const std::string& strExecPath, const std::string& strCommandLine ); // lance la SIM !!!
    bool KillSim             ( const uint32 nProcessHandle );    // kill une sim, commence par la methode gentille, puis tend vers le dur

    bool ExtractOdbFieldUuid  ( const std::string& strSimDataDirectory, std::string& strUuidRet ); // retourne le champ uuid de l'odb d'un certain repertoire de data // ASSUME: le nom du dir finis par / ou 
    bool ExtractCheckPointName( const std::string& strFileName        , std::string& strCheckPointName ); // retourne le champ uuid de l'odb d'un certain repertoire de data // ASSUME: le nom du dir finis par / ou \
    //@}


    static void     DestroyAllPendingSystemWindow();    // detruit les fenetres "sim.exe - Erreur d'application" qui trainent

private:
    LAU_App&	app_;
	bool		bInitialized_;

    LAU_NetMgr  netMgr_;

    // les infos
    T_ASN1T_IdAndLibelleVector      listMission_;
    T_ASN1T_CheckPointInfoVector    listCheckpoint_;

    T_LAU_SIM_PtrVector  listSim_;   // (owned)

    ProcessManager processManager_;

    // les parametres
    std::string   strScipioBaseExercises_;
    std::string   strScipioBaseCheckpoints_;
    MT_TimeSpan   tTimeBetweenTwoPings_;          // interval entre 2 pingspong
    MT_TimeSpan   tTimeMaxWaitPong_;              // temps que la sim a pour repondre a un ping
    MT_TimeSpan   tTimeMaxWaitInit_;              // temps que la sim a pour faire son init et se connecter

};

#ifdef USE_INLINE
#   include "LAU_Workspace.inl"
#endif 

#endif // __LAU_Workspace_h_
