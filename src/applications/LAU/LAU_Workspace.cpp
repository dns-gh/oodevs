//*****************************************************************************
//
// $Created : AML 03-03-24 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 38 $
// $Workfile: LAU_Workspace.cpp $
//
//*****************************************************************************

#include "LAU_pch.h"
#include "LAU_Workspace.h"


#ifndef USE_INLINE
#   include "LAU_Workspace.inl"
#endif

#include "LAU_App.h"

#include <process.h> // pour spawnl

//-----------------------------------------------------------------------------
// Name   : LAU_Workspace constructor

// Created: AML 03-03-27
//-----------------------------------------------------------------------------
LAU_Workspace::LAU_Workspace( LAU_App& app, MT_InputArchive_ABC& archive )
	: app_          ( app               )
    , bInitialized_ ( false             )
    , netMgr_       ( *this, archive    )   // warning C4355: 'this' : used in base member initializer list => ok mais c'est vrai que ca se fait pas du tout!
{
    std::string strDefaultScipioBaseExercises = "d:\\Scipio\\Scenarios\\";
    std::string strDefaultScipioBaseCheckpoints = "Checkpoints\\";    // nom du repertoire contenant les sauvegardes

#ifdef _DEBUG
    const int nDefaultValueBetweenTwoPings = 60;
    const int nDefaultMaxWaitPong = 40;
    const int nDefaultMaxWaitInit = 600;
#else
    const int nDefaultValueBetweenTwoPings = 180;
    const int nDefaultMaxWaitPong = 60;
    const int nDefaultMaxWaitInit = 300;
#endif // _DEBUG

    MT_LOG_INFO_MSG( "Initializing LAU_Workspace..." );

    int nTimeBetweenTwoPings    = nDefaultValueBetweenTwoPings;
    int nTimeMaxWaitPong        = nDefaultMaxWaitPong;
    int nTimeMaxWaitInit        = nDefaultMaxWaitInit;

    if( !archive.Section( "Path" ) )
        throw MT_ScipioException( "LAU_Workspace::Constructor", __FILE__, __LINE__, "Section 'Path' doesn't exist", archive.RetrieveLastError()->GetInfo() ); 
    if ( !archive.ReadField( "ScipioBaseExercises", strDefaultScipioBaseExercises ) )
        throw MT_ScipioException( "LAU_Workspace::Constructor", __FILE__, __LINE__, "Field 'ScipioBaseExercises' doesn't exist", archive.RetrieveLastError()->GetInfo() );
    if ( !archive.ReadField( "ScipioBaseCheckpoints", strDefaultScipioBaseCheckpoints ) )
        throw MT_ScipioException( "LAU_Workspace::Constructor", __FILE__, __LINE__, "Field 'ScipioBaseExercises' doesn't exist", archive.RetrieveLastError()->GetInfo() );

    archive.EndSection(); // Path

    if( !archive.Section( "Timing" ) )
        throw MT_ScipioException( "LAU_Workspace::Constructor", __FILE__, __LINE__, "Section 'Timing' doesn't exist", archive.RetrieveLastError()->GetInfo() ); 

    if ( !archive.ReadField( "TimeBetweenTwoPings", nTimeBetweenTwoPings ) )
        throw MT_ScipioException( "LAU_Workspace::Constructor", __FILE__, __LINE__, "Field 'TimeBetweenTwoPings' doesn't exist", archive.RetrieveLastError()->GetInfo() );
    if ( !archive.ReadField( "TimeMaxWaitPong", nTimeMaxWaitPong ) )
        throw MT_ScipioException( "LAU_Workspace::Constructor", __FILE__, __LINE__, "Field 'TimeMaxWaitPong' doesn't exist", archive.RetrieveLastError()->GetInfo() );        
    if ( !archive.ReadField( "TimeMaxWaitInit", nTimeMaxWaitInit ) )
        throw MT_ScipioException( "LAU_Workspace::Constructor", __FILE__, __LINE__, "Field 'TimeMaxWaitPong' doesn't exist", archive.RetrieveLastError()->GetInfo() );        

    archive.EndSection(); // Timing

    strScipioBaseExercises_     = strDefaultScipioBaseExercises;
    strScipioBaseCheckpoints_   = strDefaultScipioBaseCheckpoints;

    tTimeBetweenTwoPings_   .SetNbrSeconds( nTimeBetweenTwoPings );
    tTimeMaxWaitPong_       .SetNbrSeconds( nTimeMaxWaitPong );
    tTimeMaxWaitInit_       .SetNbrSeconds( nTimeMaxWaitInit );

    MT_LOG_INFO_MSG( MT_FormatString( "Settings: LAU_Workspace.BaseExercises: %s", strScipioBaseExercises_.c_str() ).c_str() );
    MT_LOG_INFO_MSG( MT_FormatString( "Settings: LAU_Workspace.BaseCheckpoints: %s", strScipioBaseCheckpoints_.c_str() ).c_str() );
    MT_LOG_INFO_MSG( MT_FormatString( "Settings: LAU_Workspace.TimeBetweenTwoPings: %d", nTimeBetweenTwoPings ).c_str() );
    MT_LOG_INFO_MSG( MT_FormatString( "Settings: LAU_Workspace.TimeMaxWaitPong: %d", nTimeMaxWaitPong ).c_str() );
    MT_LOG_INFO_MSG( MT_FormatString( "Settings: LAU_Workspace.TimeMaxWaitInit: %d", nTimeMaxWaitInit ).c_str() );
}


//-----------------------------------------------------------------------------
// Name   : LAU_Workspace::~LAU_Workspace
// Created: AML 03-03-27
//-----------------------------------------------------------------------------
LAU_Workspace::~LAU_Workspace()
{
    Terminate();
}

//-----------------------------------------------------------------------------
// Name   : LAU_Workspace::Initialize
// Created: AML 03-03-27
//-----------------------------------------------------------------------------
void LAU_Workspace::Initialize()
{
	if( bInitialized_ )
		return;

	bInitialized_ = true;
}


//-----------------------------------------------------------------------------
// Name   : LAU_Workspace::Terminate
// Created: AML 03-03-27
//-----------------------------------------------------------------------------
void LAU_Workspace::Terminate()
{
	if( !bInitialized_ )
		return;

    CIT_LAU_SIM_PtrVector itSim, itSimEnd;
    for( itSim = listSim_.begin(), itSimEnd = listSim_.end(); itSim != itSimEnd; ++itSim )
         delete (*itSim);
    listSim_.clear();
	bInitialized_ = false;
}


//-----------------------------------------------------------------------------
// Name   : LAU_Workspace::Start
// Created: AML 03-03-27
//-----------------------------------------------------------------------------
void LAU_Workspace::Start()
{

}



//-----------------------------------------------------------------------------
// Name   : LAU_Workspace::Stop
// Created: AML 03-03-27
//-----------------------------------------------------------------------------
void LAU_Workspace::Stop()
{

}



//-----------------------------------------------------------------------------
// Name   : LAU_Workspace::Execute
// Created: AML 03-03-27
//-----------------------------------------------------------------------------
bool LAU_Workspace::Execute()
{
    netMgr_.Update();
    CheckExecutionOfAllSim();
	return true;
}


//-----------------------------------------------------------------------------
// Name: LAU_Workspace::PrintAllSimProcess
// Created: AML 03-04-23
//-----------------------------------------------------------------------------
void LAU_Workspace::PrintAllSimProcess()
{
    //
    // Affiche toutes les SIM
    //
    std::string strInfo;
    std::string strAllInfo;
    strAllInfo.reserve( 1000 );

    uint nSizeMaxOfLine = 0;


    CIT_LAU_SIM_PtrVector itSim;
    for( itSim = listSim_.begin(); itSim != listSim_.end(); ++itSim )
    {
        (*itSim)->GetDescription( strInfo );
        if( nSizeMaxOfLine < strInfo.size() )
        {
            nSizeMaxOfLine = strInfo.size();
        }
        strAllInfo += strInfo + "\n";
    }

    // ajoute des lignes autour des SIM:
    strInfo = std::string( nSizeMaxOfLine, '-' );
    strInfo += "\n";

    strAllInfo.insert( 0, strInfo );
    strAllInfo += strInfo;

    strAllInfo.insert( 0, "\n" );
    MT_LOG_INFO_MSG( strAllInfo.c_str() );

    //
    // Affiche l'etat du Process Manager
    //

    if( processManager_.HasProcess() )
    {
        processManager_.UpdateState();
        MT_LOG_INFO_MSG( ( std::string( "\n" ) + processManager_.GetAllProcessDescription() ).c_str() );
    }

}


//-----------------------------------------------------------------------------
// Name: LAU_Workspace::CheckExecutionOfAllSim
// Object : ping les applications de chaque sim, et/ou verifie les numeros de process
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
void LAU_Workspace::CheckExecutionOfAllSim()
{
    IT_LAU_SIM_PtrVector itSim;
    for( itSim = listSim_.begin(); itSim != listSim_.end(); )
    {
        if( !(*itSim)->CheckExecution( netMgr_.GetMsgMgr() ) )
        {
            // l'execution a failée, on en avertit MOS, et on detruit notre sim

            PrintAllSimProcess();

            bool bIsMosAlive = (*itSim)->GetMosClient() != 0;
            bool bFailure = 0 != processManager_.GetExitCode( (*itSim)->GetProcessID() );

            if( bIsMosAlive )
            {
                // on avertit MOS

                if( !(*itSim)->HasBeenInitialised() )
                {
                    if( !(*itSim)->IsAskToStop() )
                    {
                        // elle a failée d'elle meme avant meme de commencer, c'est une erreur d'init
                        SendMsg_LauMos_SimLaunched( *(LAU_Client_ABC*)( (*itSim)->GetMosClient() ), (*itSim)->GetExerciceID(), EnumCtrlErrorCodeLauncher::error_error_scenario_init );
                    }
                    else
                    {
                        // on a failée, car on l'a killé, on ne dit rien
                    }
                }
                else
                {
                    if( bFailure )
                    {
                        // elle a failée apres avoir commencer, c'est une erreur d'exec
                        SendMsg_LauMos_SimFailure( *(LAU_Client_ABC*)( (*itSim)->GetMosClient() ), (*itSim)->GetExerciceID() );
                    }
                    else
                    {
                        // tout c'est bien passé
                        netMgr_.GetMsgMgr().SendMsg_LauMos_ExecSuccess( *( (*itSim)->GetMosClient() ), (*itSim)->GetExerciceID(), true );
                    }
                }
            }

            // on detruit notre sim
            MT_LOG_INFO_MSG( MT_FormatString( "Launcher: Workspace: Closing SIM ID: %d process: %d", (*itSim)->GetExerciceID(), (*itSim)->GetProcessHandle() ).c_str() );

            // KillSim( (*itSim)->GetProcessHandle() );
            bool bRet = processManager_.FinishProcess( (*itSim)->GetProcessID() );
            if( !bRet )
            {
                MT_LOG_INFO_MSG( MT_FormatString( "Launcher: Workspace: Killing SIM ID: %d process: %d", (*itSim)->GetExerciceID(), (*itSim)->GetProcessHandle() ).c_str() );
                processManager_.FinishProcess( (*itSim)->GetProcessID(), true );
            }

            DestroyAllPendingSystemWindow();

            delete (*itSim);
            itSim = listSim_.erase( itSim );
        }
        else
           ++itSim;
    }
}


//-----------------------------------------------------------------------------
// Message reception
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name   : LAU_Workspace::ReceiveMsg_MosLau_RequestListMission
// Created: AML 03-04-03
//-----------------------------------------------------------------------------
void LAU_Workspace::ReceiveMsg_MosLau_RequestListMission( LAU_Client_ABC& client )
{
    SendMsg_LauMos_ListMission( client );
}


//-----------------------------------------------------------------------------
// Name: LAU_Workspace::ReceiveMsg_MosLau_LaunchSim
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
void LAU_Workspace::ReceiveMsg_MosLau_LaunchSim( LAU_Client_ABC& client, const uint32 nExerciceID, const uint32 nScenarioID, const char* szSimCommandLine )
{
    if( FindSim( nExerciceID ) != NULL )
    {
        // un exercice de meme ID a deja etait lancé...
        SendMsg_LauMos_SimLaunched( client, nExerciceID, EnumCtrlErrorCodeLauncher::error_already_started );
        return;
    }

    CIT_ASN1T_IdAndLibelleVector itMission, itMissionEnd;
    for( itMission = listMission_.begin(), itMissionEnd = listMission_.end(); itMission != itMissionEnd; ++itMission )
    {
         if( (*itMission).nID == nScenarioID )
            break;
    }

    if( itMission == itMissionEnd )
    {
        // id mission not found
        SendMsg_LauMos_SimLaunched( client, nExerciceID, EnumCtrlErrorCodeLauncher::error_invalid_scenario_identifier );
    }
    else
    {
        // ok
        std::string strExecPath( strScipioBaseExercises_ + (*itMission).libelle );
        std::string strSimCommandLine = szSimCommandLine;
        bool bRet = LaunchSim( (LAU_MOS*)&client, nExerciceID, strExecPath, strSimCommandLine );
        if( !bRet )
        {
            SendMsg_LauMos_SimLaunched( client, nExerciceID, EnumCtrlErrorCodeLauncher::error_error_scenario_init );
        }
    }
    
}


//-----------------------------------------------------------------------------
// Name: LAU_Workspace::ReceiveMsg_MosLau_RequestListSim
// Created: AML 01-06-12
//-----------------------------------------------------------------------------
void LAU_Workspace::ReceiveMsg_MosLau_RequestListSim( LAU_Client_ABC& client )
{
    SendMsg_LauMos_ListSim( client );
}


//-----------------------------------------------------------------------------
// Name: LAU_Workspace::ReceiveMsg_MosLau_RequestListCheckpoint
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
void LAU_Workspace::ReceiveMsg_MosLau_RequestListCheckpoint( LAU_Client_ABC& client, const uint32 nExerciceID )
{
    SendMsg_LauMos_ListCheckpoint( client, nExerciceID );
}


//-----------------------------------------------------------------------------
// Name: LAU_Workspace::ReceiveMsg_MosLau_LaunchSimFromCheckpoint
// Created: AML 03-07-25
//-----------------------------------------------------------------------------
void LAU_Workspace::ReceiveMsg_MosLau_LaunchSimFromCheckpoint( LAU_Client_ABC& client, const uint32 nExerciceID, const uint32 nCheckpointID, const char* szSimCommandLine )
{
    if( FindSim( nExerciceID ) != NULL )
    {
        // un exercice de meme ID a deja etait lancé...
        SendMsg_LauMos_SimLaunched( client, nExerciceID, EnumCtrlErrorCodeLauncher::error_already_started );
        return;
    }

    CIT_ASN1T_CheckPointInfoVector itCheckpoint, itCheckpointEnd;
    for( itCheckpoint = listCheckpoint_.begin(), itCheckpointEnd = listCheckpoint_.end(); itCheckpoint != itCheckpointEnd; ++itCheckpoint )
    {
         if( (*itCheckpoint).nID == nCheckpointID )
            break;
    }

    if( itCheckpoint == itCheckpointEnd )
    {
        // id mission not found
        SendMsg_LauMos_SimLaunched( client, nExerciceID, EnumCtrlErrorCodeLauncher::error_invalid_checkpoint_identifier );
    }
    else
    {
        // ok
        std::string strExecPath( strScipioBaseExercises_ + (*itCheckpoint).libelle_scenario );
        std::string strSimCommandLine = szSimCommandLine;

        // ajout du parametre lié au checkpoint
        strSimCommandLine += " -checkpoint ";
        strSimCommandLine += (*itCheckpoint).libelle_checkpoint;


        bool bRet = LaunchSim( (LAU_MOS*)&client, nExerciceID, strExecPath, strSimCommandLine );
        if( !bRet )
        {
            SendMsg_LauMos_SimLaunched( client, nExerciceID, EnumCtrlErrorCodeLauncher::error_error_checkpoint_init );
        }
    }
}


//-----------------------------------------------------------------------------
// Message emission
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name: LAU_Workspace::SendMsg_LauMos_ListMission
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
void LAU_Workspace::SendMsg_LauMos_ListMission( LAU_Client_ABC& client )
{
    ComputeListMission();

    NET_ASN_Msg_LauMos_ListMission asnMsg;

    asnMsg.GetAsnMsg().n      = listMission_.size();
    asnMsg.GetAsnMsg().elem   = &( *( listMission_.begin() ) );

    asnMsg.Send( netMgr_.GetMsgMgr(), client );
}


//-----------------------------------------------------------------------------
// Name: LAU_Workspace::SendMsg_LauMos_SimLaunched
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
void LAU_Workspace::SendMsg_LauMos_SimLaunched( LAU_Client_ABC& client, const uint32 nExerciceID, const ASN1T_EnumCtrlErrorCodeLauncher nErrorCode, const uint16 nNumPort, const uint32 nDinMagicNumber )
{
    NET_ASN_Msg_LauMos_SimLaunched asnMsg;

    asnMsg.GetAsnMsg().id_exercice      = nExerciceID;

    asnMsg.GetAsnMsg().launch_information.error_code       = nErrorCode;
    asnMsg.GetAsnMsg().launch_information.num_port         = nNumPort;
    asnMsg.GetAsnMsg().launch_information.din_magic_number = nDinMagicNumber;
    

    asnMsg.Send( netMgr_.GetMsgMgr(), client );
}


//-----------------------------------------------------------------------------
// Name: LAU_Workspace::SendMsg_LauMos_ListSim
// Created: AML 01-06-12
//-----------------------------------------------------------------------------
void LAU_Workspace::SendMsg_LauMos_ListSim( LAU_Client_ABC& client )
{
    T_ASN1T_IdExAndLaunchInformationVector listSimInfo;

    listSimInfo.clear();

    ASN1T_IdExAndLaunchInformation idExAndInfo;

    CIT_LAU_SIM_PtrVector itSim, itSimEnd;
    for( itSim = listSim_.begin(), itSimEnd = listSim_.end(); itSim != itSimEnd; ++itSim )
    {

        idExAndInfo.id_exercice = (*itSim)->GetExerciceID();
        
        if( (*itSim)->HasBeenInitialised() )
        {
            // on envoie les infos seulement sur les sims finis d'etre initialisé: 
            idExAndInfo.m.launch_informationPresent = true;
            idExAndInfo.launch_information.num_port = (*itSim)->GetPort();
            idExAndInfo.launch_information.din_magic_number = (*itSim)->GetMagic();
            idExAndInfo.launch_information.error_code = (EnumCtrlErrorCodeLauncher::Root)((*itSim)->GetLaunchErrorCode());
        }
        else
        {
            idExAndInfo.m.launch_informationPresent = false;
        }

        listSimInfo.push_back( idExAndInfo );
    }


    NET_ASN_Msg_LauMos_ListSim asnMsg;

    asnMsg.GetAsnMsg().n      = listSimInfo.size();
    asnMsg.GetAsnMsg().elem   = &( *( listSimInfo.begin() ) );

    asnMsg.Send( netMgr_.GetMsgMgr(), client );
}



//-----------------------------------------------------------------------------
// Name: LAU_Workspace::SendMsg_LauMos_SimFailure
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
void LAU_Workspace::SendMsg_LauMos_SimFailure( LAU_Client_ABC& client, const uint32 nExerciceID )
{
    NET_ASN_Msg_LauMos_SimFailure asnMsg;

    asnMsg.GetAsnMsg()      = nExerciceID;    

    asnMsg.Send( netMgr_.GetMsgMgr(), client );
}


//-----------------------------------------------------------------------------
// Name: LAU_Workspace::SendMsg_LauMos_ListCheckpoint
// Created: AML 03-07-25
//-----------------------------------------------------------------------------
void LAU_Workspace::SendMsg_LauMos_ListCheckpoint( LAU_Client_ABC& client, const uint32 nExerciceID )
{
    ComputeListCheckpoint( nExerciceID );

    NET_ASN_Msg_LauMos_ListCheckpoint asnMsg;

    asnMsg.GetAsnMsg().id_exercice              = nExerciceID;
    asnMsg.GetAsnMsg().list_checkpoint.n        = listCheckpoint_.size();
    asnMsg.GetAsnMsg().list_checkpoint.elem     = &( *( listCheckpoint_.begin() ) );

    asnMsg.Send( netMgr_.GetMsgMgr(), client );
}


//-----------------------------------------------------------------------------
// Function
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name   : LAU_Workspace::ComputeListMission
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
void LAU_Workspace::ComputeListMission()
{
    // Nettoyage
    for( IT_ASN1T_IdAndLibelleVector itMission = listMission_.begin(); itMission != listMission_.end(); ++itMission )
    {
        ASN1T_IdAndLibelle& asnMission = *itMission;           
        free( (char*)asnMission.libelle );
        free( (char*)asnMission.uuid_odb );
    }
    listMission_.clear();   

    int nNumID = 1;

    // Search all sub directories in the Exercises Directory (not recursively)
    MT_DirectoryBrowser browserDir;
    browserDir.FindFiles( strScipioBaseExercises_, "*", MT_DirectoryBrowser::eChildDirs, false );

    const MT_DirEntryList& entryList = browserDir.GetEntryList();    

    for( MT_DirEntryList::const_iterator entryIt = entryList.begin(); entryIt != entryList.end(); ++entryIt )
    {
        const MT_DirEntry& entry = *entryIt;
        
        ASN1T_IdAndLibelle idAndLib;
        idAndLib.nID     = nNumID;
        idAndLib.libelle = strdup( entry.fileName_.c_str() );

        // lire dans chaque rep le champ <ODB><FILE> qui contient un path + nom de fichier (eg: "ODB/ODB_JVT.xml")
        // dans lequel il y a un champ d'id: <Identification>29D4F224-6A94-431e-BBF8-159E22BAB500
        // a renvoyer dans le champ "uuid-odb"
        std::string strUuid;
        ExtractOdbFieldUuid( entry.fullName_ + "\\", strUuid );
        idAndLib.uuid_odb = strdup( strUuid.c_str() );
        listMission_.push_back( idAndLib );
        ++nNumID;
    }
}

//-----------------------------------------------------------------------------
// Name: LAU_Workspace::ComputeListCheckpoint
// Description calcule la liste des checkpoints lié a un exercice, ou a tout les exercice si nExerciceID == -1
// Created: AML 03-07-25
//-----------------------------------------------------------------------------
void LAU_Workspace::ComputeListCheckpoint( const int nExerciceID_ToList )
{

    //
    // ATTENTION:   les OID ne changent pas entre deux ComputeListCheckpoint meme si de nouveaux checkpoint existe,
    //              (et meme si nExerciceID_ToList est different bien sur)
    //
    // EXPLICATION: on a plusieurs MOS qui peuvent se connecter, en simultané, 
    // or les checkpoints peuvent etre ajouté souvent... 
    // donc on pourrais avoir une sequence: 
    //      - un mos demande la liste
    //      - une sim d'exo quelquonque fait un checkpoint
    //      - un autre mos demande la liste (la liste est alors un peu differente)
    //      - le premier mos demande a lancer une sim d'apres le checkpoint d'un certain OID, 
    //        cet OID doit correspondre au checkpoint de la premiere liste 

    //
    // Pour cela, on assume que tout nouveau checkpoint a par essence une date plus recente que les anciens
    // et on les trie par date (pas de bidouille d'horloge sur le serveur)

    // Nettoyage
    for( IT_ASN1T_CheckPointInfoVector itCheckPoint = listCheckpoint_.begin(); itCheckPoint != listCheckpoint_.end(); ++itCheckPoint )
    {
        ASN1T_CheckPointInfo& asnCheckPoint = *itCheckPoint;           
        free( (char*)asnCheckPoint.libelle_scenario );
        free( (char*)asnCheckPoint.libelle_checkpoint );
        if( asnCheckPoint.m.nom_checkpointPresent )
            free( (char*)asnCheckPoint.nom_checkpoint );
    }
    listCheckpoint_.clear();


    // Search all sub directories in the Exercises Directory (not recursively)
    // le browserDir ne doit pas etre effacé avant l'envoi du message!
    MT_DirectoryBrowser browserDirExecercices;
    browserDirExecercices.FindFiles( strScipioBaseExercises_, "*", MT_DirectoryBrowser::eChildDirs, false );

    int nNumScenario = 1;
    const MT_DirEntryList& entryList = browserDirExecercices.GetEntryList();

    for( MT_DirEntryList::const_iterator entryIt = entryList.begin(); entryIt != entryList.end(); ++entryIt )
    {
        // pour chaque scenario, on cherche les checkpoints

        const MT_DirEntry& entry = *entryIt;

        MT_DirectoryBrowser browserDirCheckpoint;
        browserDirCheckpoint.FindFiles( entry.fullName_ + "\\" + strScipioBaseCheckpoints_, "*", MT_DirectoryBrowser::eChildDirs, false );

        // trie par nom de fichier, ainsi tout les checkpoint d'un meme exercice sont a la suite
        browserDirCheckpoint.SetSortType( MT_DirectoryBrowser::eSortByName );
        const MT_DirEntryList& entryCheckpointList = browserDirCheckpoint.GetEntryList();

        int nNumExercicePred = -1000; // un numero impossible, (pour initialisé les var dans la boucle)
        int nNumCheckpoint   = -1000; // valeur impossible, juste pour que le compilo ne rale pas
        for( MT_DirEntryList::const_iterator entryItCheckpoint = entryCheckpointList.begin(); entryItCheckpoint != entryCheckpointList.end(); ++entryItCheckpoint )
        {
            const MT_DirEntry& entryCheckpoint = *entryItCheckpoint;

            // format des noms de repertoires: 05__2003_15_02__15h02m50 (IDEX__YYYY_MM_DD__HHhMMmSEC)
            int nNumExercice = atoi( entryCheckpoint.fileName_.c_str() );   // va faire un atoi sur le premier nombre trouvé

            if( nNumExercice != nNumExercicePred )
            {
                nNumExercicePred = nNumExercice;
                nNumCheckpoint = 1;
            }

            if( nExerciceID_ToList != -1 && nExerciceID_ToList != nNumExercice )
            {
                // on ne sort pas cette exercice
                continue;
            }


            //$$$ CRADO (comme le reste d'ailleurs ...)
            ASN1T_CheckPointInfo checkpointInfo;

            // les OID vont etre du type 10000001,10000002,20100001,20100002,20200001...
            static const int nMultiplicatorBaseByNumExercice = 100000;      // limite a 99999 save by ex
            static const int nMultiplicatorBaseByScenario = 10000000;       // limite a 99 ex

            checkpointInfo.nID                = nNumScenario * nMultiplicatorBaseByScenario + nNumExercice * nMultiplicatorBaseByNumExercice + nNumCheckpoint;
            checkpointInfo.libelle_scenario   = strdup( entry.fileName_.c_str() );
            checkpointInfo.libelle_checkpoint = strdup( entryCheckpoint.fileName_.c_str() );

            std::string strCheckPointName;
            ExtractCheckPointName( browserDirCheckpoint.GetRootDir() + "/" + entryCheckpoint.fileName_ + "/MetaData.xml", strCheckPointName );
            if( !strCheckPointName.empty() )
            {
                checkpointInfo.m.nom_checkpointPresent = 1;
                checkpointInfo.nom_checkpoint = strdup( strCheckPointName.c_str() );
            }
            

            listCheckpoint_.push_back( checkpointInfo );
            ++nNumCheckpoint;
        }
        ++nNumScenario;
    }
}

//-----------------------------------------------------------------------------
// Name   : LAU_Workspace::LaunchSim
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
bool LAU_Workspace::LaunchSim( LAU_MOS* pMosClient, const uint32 nExerciceID, const std::string& strExecPath, const std::string& strCommandLine )
{
    PrintAllSimProcess();

    // lance la sim, sans attendre la fin de son execution

    // prepare les parametres pour la sim

    char szAppName[] = "sim";
    char szParamsSeparatedBySpaces[256];    // les parametres sans le noms de l'exe
    sprintf( szParamsSeparatedBySpaces, "-exerciceID %d %s", nExerciceID, strCommandLine.c_str() );

    MT_LOG_INFO_MSG( MT_FormatString( "Launcher: Workspace: Launching Sim with optionnal param: '%s'", strCommandLine.c_str() ).c_str() );

	char bufOldDir[512];
	if( GetCurrentDirectory( 512, bufOldDir ) == 0 )
	{
        MT_LOG_ERROR_MSG( MT_FormatString( "Launcher: Workspace: GetCurrentDirectory error (error: %s (%d)", SystemGetErrorMessage( GetLastError() ), GetLastError() ).c_str() );

		return false;
	}

#ifdef _DEBUG
    MT_LOG_INFO_MSG( MT_FormatString( "Launcher: Current Directory: '%s'", bufOldDir ).c_str() );
#endif // _DEBUG

	if( SetCurrentDirectory( strExecPath.c_str() ) == 0 )
	{
        MT_LOG_ERROR_MSG( MT_FormatString( "Launcher: Workspace: SetCurrentDirectory error '%s' (error: %s (%d)", strExecPath.c_str(), SystemGetErrorMessage( GetLastError() ), GetLastError() ).c_str() );

		SetCurrentDirectory( bufOldDir );
		return false;
	}

#ifdef _DEBUG
    MT_LOG_INFO_MSG( MT_FormatString( "Launcher: New Directory: '%s'", strExecPath.c_str() ).c_str() );
#endif // _DEBUG

    // _P_NOWAIT: on voit les sortie consoles dans celle du Launcher
    // _P_DETACH: on ne voit aucune sortie console
//    int32 nProcessHandle = _spawnv( _P_NOWAIT, apArguments[0], apArguments );

    // voir avec AllocConsole... ou avec CreateProcess
    // pb reglé: ajoue d'un Free/Alloc au démarrage de la SIM

    int nID = processManager_.LaunchProcess( szAppName, szParamsSeparatedBySpaces );
    int32 nProcessHandle = (int32)processManager_.GetHandleProcess( nID );
    

    Beep( 5000, 100 );

	SetCurrentDirectory( bufOldDir );

    if( nProcessHandle == -1 )
    {
        MT_LOG_ERROR_MSG( MT_FormatString( "Launcher: Workspace: Spawning process error (returning 0x%x, LastError: %d)", nProcessHandle, GetLastError() ).c_str() );
        return false;
    }

    MT_LOG_INFO_MSG( MT_FormatString( "Launcher: Workspace: Spawning process handle: 0x%x", nProcessHandle ).c_str() );


    LAU_SIM* pSim = new LAU_SIM();

    listSim_.push_back( pSim );

    pSim->SetProcessHandle( nProcessHandle );
    pSim->SetProcessID( nID );
    pSim->SetMosClient( pMosClient );
    pSim->SetExerciceID( nExerciceID );

    PrintAllSimProcess();

    return true;
}


//-----------------------------------------------------------------------------
// Name: LAU_Workspace::KillSim
// Object: kill une sim, commence par la methode gentille, puis tend vers le dur
// Created: AML 03-04-18
//-----------------------------------------------------------------------------
bool LAU_Workspace::KillSim( const uint32 nProcessHandle )
{
    Beep( 3000, 200 );

    PrintAllSimProcess();

    uint32 nExitStatus;
    GetExitCodeProcess( (HANDLE)nProcessHandle, &nExitStatus );
    if( nExitStatus == 0 || nExitStatus != STILL_ACTIVE )
    {
        return true; // le process n'existait deja plus
    }

    TerminateProcess( (HANDLE)nProcessHandle, 0xFFFFFFFF );

    DestroyAllPendingSystemWindow();

    PrintAllSimProcess();

    return true;
}

//-----------------------------------------------------------------------------
// Name: LAU_Workspace::MosHasDisconnect
// Object : un MOS a terminé (failure ou pas)
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
void LAU_Workspace::MosHasDisconnect( LAU_MOS* pThisMosIsNoMore )
{
    IT_LAU_SIM_PtrVector itSim, itSimEnd;
    for( itSim = listSim_.begin(), itSimEnd = listSim_.end(); itSim != itSimEnd; ++itSim )
    {
         if( (*itSim)->GetMosClient() == pThisMosIsNoMore )
             (*itSim)->SetMosClient( 0 );
    }
}



//-----------------------------------------------------------------------------
// Name: LAU_Workspace::KillSimRemote
// Object: detruit le processus lié a une sim (comme ca direct)
// Created: AML 03-06-16
//-----------------------------------------------------------------------------
bool LAU_Workspace::KillSimRemote( const uint32 nExerciceID )
{
    LAU_SIM* pSim = FindSim( nExerciceID );

    if( pSim == NULL )
    {
        MT_LOG_ERROR_MSG( MT_FormatString ( "Launcher: Workspace: KillSimRemote: Sim ID %d doesn't exists", nExerciceID ).c_str() );
        return NULL;
    }

    MT_LOG_INFO_MSG( MT_FormatString ( "Launcher: Workspace: KillSimRemote: Killing Sim ID %d", nExerciceID ).c_str() );

    uint32 nProcessHandle = pSim->GetProcessHandle();

    bool bRet = KillSim( nProcessHandle );
    return bRet;
}

// -----------------------------------------------------------------------------
// Name: LAU_Workspace::ExtractCheckPointName
// Created: NLD 2004-05-05
// -----------------------------------------------------------------------------
bool LAU_Workspace::ExtractCheckPointName( const std::string& strFileName, std::string& strCheckPointName )
{
    MT_LOG_INFO_MSG( MT_FormatString( "\tExtracting checkpoint %s", strFileName.c_str() ) );

    strCheckPointName = "";

    MT_XXmlInputArchive checkpointArchive;
    if( !checkpointArchive.Open( strFileName ) )
        return false;

    if( !checkpointArchive.Section( "CheckPoint" ) )
        return false;

    if( !checkpointArchive.ReadField( "Nom", strCheckPointName ) )
        return false;

    if( !checkpointArchive.EndSection() )
        return false;
    return true;
}


//-----------------------------------------------------------------------------
// Name: LAU_Workspace::ExtractOdbFieldUuid
// Object: retourne le champ uuid de l'odb d'un certain repertoire de data
// ASSUME: le nom du dir finis par / ou \
// Created: AML 03-07-28
//-----------------------------------------------------------------------------
bool LAU_Workspace::ExtractOdbFieldUuid( const std::string& strSimDataDirectory, std::string& strUuidRet )
{
    // lire dans chaque rep le champ <ODB><FILE> qui contient un path + nom de fichier (eg: "ODB/ODB_JVT.xml")
    // dans lequel il y a un champ d'id: <Identification>29D4F224-6A94-431e-BBF8-159E22BAB500
    // a renvoyer dans le champ "uuid-odb"
    
    const std::string strArchiveName = strSimDataDirectory + "Scipio.xml";

    strUuidRet = "no uuid - 00000000000000000000000000";

    MT_XXmlInputArchive archive;
    if( !archive.Open( strArchiveName ) )
        return false;

    if( !archive.Section( "Scipio" ) )
        return false;
    if( !archive.Section( "Donnees" ) )
        return false;

    std::string strOdbFileName;
    if ( !archive.ReadField( "ODB", strOdbFileName ) )
        return false;

    // pour faire plus propre on sort des sections

    archive.EndSection(); // Donnees
    archive.EndSection(); // Scipio

    const std::string strOdbPathFileName = strSimDataDirectory + strOdbFileName;

    if( !archive.Open( strOdbPathFileName ) )
        return false;

    if( !archive.Section( "ODB" ) )
        return false;

    if ( !archive.ReadAttribute( "id", strUuidRet ) )
        return false;
    return true;
}




//-----------------------------------------------------------------------------
// Name: LAU_Workspace::DestroyAllPendingSystemWindow
// Object: detruit les fenetres "sim.exe - Erreur d'application" qui trainent
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
void LAU_Workspace::DestroyAllPendingSystemWindow()
{
    // mettre le ShowUI a 0, le DoReport a 0, ainsi la fenetre est toute petite avec juste
    // sim.exe - Erreur d'application
    // rechercher les fenetres: sim.exe - Erreur d'application et les killer

    for( int i = 0; i < 8; ++i )
    {
        HWND hWnd = FindWindow( NULL, "sim.exe - Erreur d'application" );
        if( hWnd != 0 )
        {
////            CloseWindow( hWnd ); // ne fait rien, (reduit la fenetre juste...)
////            SetWindowText( hWnd, "!!!!! TO DESTROY !!!!!" ); // juste pour verifier que la window a bien été trouvée
////
////            DestroyWindow( hWnd ); // marche pas c'est pas une fenetre...
////            EndDialog( hWnd, IDCANCEL ); // marche pas tu n'a le droit que a l'interieur du dialogproc

            MT_LOG_INFO_MSG( "Launcher: Workspace: Killing an 'erreur d'application' window" );


            SendMessage( hWnd, WM_COMMAND, IDOK, 0 );   // ok => ne pas debugger!
        }
    }
}
