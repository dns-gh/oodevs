//*****************************************************************************
//
// $Created: JDY 03-06-23 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Project_Data.cpp $
// $Author: Ape $
// $Modtime: 22/04/05 16:47 $
// $Revision: 11 $
// $Workfile: ADN_Project_Data.cpp $
//
//*****************************************************************************
#include "ADN_Pch.h"
#include "ADN_Project_Data.h"

#include "res/resource.h"
#include "ADN_Tools.h"
#include "ADN_Workspace.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_ResourceXml.h"
#include "ADN_Xml_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_DataException.h"

#include <windows.h>

ADN_Project_Data::WorkDirInfos  ADN_Project_Data::workDir_;
std::string                     ADN_Project_Data::FileInfos::szUntitled_  ="Untitled";


//-----------------------------------------------------------------------------
// Name: SimInfos::SimInfos
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Project_Data::SimInfos::SimInfos()
: bAutoStart_( 0 )
, nTimeStep_( 0 )
, nTimeFactor_( 0 )
, nAutosaveTime_( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: SimInfos::ReadArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::SimInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section("SIM");

    int nTemp;
    input.ReadField( "AutoStart", nTemp );
    bAutoStart_ = (nTemp != 0);
    input.ReadField( "TimeStep", nTimeStep_ );
    input.ReadField( "TimeFactor", nTimeFactor_ );
    input.Section( "SauvegardesAutomatiques" );
    input.ReadField( "TempsEntreSauvegardes", nAutosaveTime_ );
    input.ReadField( "NombreMax", nNbrMax_ );
    input.EndSection(); // SauvegardesAutomatiques

    input.EndSection();   // SIM
}


// -----------------------------------------------------------------------------
// Name: SimInfos::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::SimInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section("SIM");

    output.WriteField( "AutoStart", bAutoStart_.GetData() ? 1 : 0 );
    output.WriteField( "TimeStep", nTimeStep_.GetData() );
    output.WriteField( "TimeFactor", nTimeFactor_.GetData() );
    output.Section( "SauvegardesAutomatiques" );
    output.WriteField( "TempsEntreSauvegardes", nAutosaveTime_.GetData() );
    output.WriteField( "NombreMax", nNbrMax_.GetData() );
    output.EndSection(); // SauvegardesAutomatiques

    output.EndSection();    // SIM
}


// -----------------------------------------------------------------------------
// Name: PathfinderInfo::PathfinderInfo
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_Project_Data::PathfinderInfo::PathfinderInfo()
: nPathfinderNbr_( 2 )
, nDistanceThreshold_( 15000 )
, szRulesFile_( "PathfindRules.xml" )
{
}


// -----------------------------------------------------------------------------
// Name: PathfinderInfo::ReadArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Project_Data::PathfinderInfo::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Pathfind" );
    input.ReadField( "PathfindNumber", nPathfinderNbr_ );
    input.ReadField( "DistanceThreshold", nDistanceThreshold_ );
    input.ReadField( "Rules", szRulesFile_ );
    input.EndSection(); // Pathfind
}


// -----------------------------------------------------------------------------
// Name: PathfinderInfo::WriteArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Project_Data::PathfinderInfo::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Pathfind" );
    output.WriteField( "PathfindNumber", nPathfinderNbr_.GetData() );
    output.WriteField( "DistanceThreshold", nDistanceThreshold_.GetData() );
    output.WriteField( "Rules", szRulesFile_.GetData() );
    output.EndSection(); // Pathfind
}


//-----------------------------------------------------------------------------
// Name: DataInfos::DataInfos
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Project_Data::DataInfos::DataInfos()
: szTerrain_()
, szDecisional_()
, szNetwork_()
, szSizes_()
, szArmors_()
, szObjects_()
, szEquipements_()
, szLaunchers_()
, szWeapons_()
, szSensors_()
, szComponents_()
, szUnits_()
, szAutomata_()
, szNBC_()
, szWeather_()
, szHealth_()
, szIDs_()
, szODB_()
{
}


// -----------------------------------------------------------------------------
// Name: DataInfos::ReadArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::DataInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section("Donnees");
    input.ReadField( "Terrain", szTerrain_ );
    input.ReadField( "Decisionnel", szDecisional_ );
    input.ReadField( "Reseau", szNetwork_ );
    input.ReadField( "Volumes", szSizes_ );
    input.ReadField( "Protections", szArmors_ );
    input.ReadField( "Objets", szObjects_ );
    input.ReadField( "Dotations", szEquipements_ );
    input.ReadField( "Lanceurs", szLaunchers_ );
    input.ReadField( "Armements", szWeapons_ );
    input.ReadField( "Capteurs", szSensors_ );
    input.ReadField( "Composantes", szComponents_ );
    input.ReadField( "Pions", szUnits_ );
    input.ReadField( "Automates", szAutomata_ );
    input.ReadField( "NBC", szNBC_ );
    input.ReadField( "Meteo", szWeather_ );
    input.ReadField( "Sante", szHealth_ );
    input.ReadField( "ClasseIDs", szIDs_ );
    input.ReadField( "FacteursHumains", szHumanFactors_ );
    input.ReadField( "Pannes", szBreakdowns_ );
    input.ReadField( "GroupesConnaissance", szKnowledgeGroups_ );
    input.ReadField( "Logistique", szLog_ );
    input.ReadField( "Ravitaillement", szSupply_ );
    input.ReadField( "Communications", szCom_ );
    input.ReadField( "ODB", szODB_ );
    input.ReadField( "Pathfind", szPathfinder_ );
    input.ReadField( "Missions", szMissions_ );
    input.ReadField( "HLA", szHLA_ );
    input.EndSection();   // Donnees
}


// -----------------------------------------------------------------------------
// Name: DataInfos::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::DataInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section("Donnees");
    output.WriteField( "Terrain", szTerrain_.GetData() );
    output.WriteField( "Decisionnel", szDecisional_.GetData() );
    output.WriteField( "Reseau", szNetwork_.GetData() );
    output.WriteField( "Volumes", szSizes_.GetData() );
    output.WriteField( "Protections", szArmors_.GetData() );
    output.WriteField( "Objets", szObjects_.GetData() );
    output.WriteField( "Dotations", szEquipements_.GetData() );
    output.WriteField( "Lanceurs", szLaunchers_.GetData() );
    output.WriteField( "Armements", szWeapons_.GetData() );
    output.WriteField( "Capteurs", szSensors_.GetData() );
    output.WriteField( "Composantes", szComponents_.GetData() );
    output.WriteField( "Pions", szUnits_.GetData() );
    output.WriteField( "Automates", szAutomata_.GetData() );
    output.WriteField( "NBC", szNBC_.GetData() );
    output.WriteField( "Meteo", szWeather_.GetData() );
    output.WriteField( "Sante", szHealth_.GetData() );
    output.WriteField( "ClasseIDs", szIDs_.GetData() );
    output.WriteField( "FacteursHumains", szHumanFactors_.GetData() );
    output.WriteField( "Pannes", szBreakdowns_.GetData() );
    output.WriteField( "GroupesConnaissance", szKnowledgeGroups_.GetData() );
    output.WriteField( "Logistique", szLog_.GetData() );
    output.WriteField( "Ravitaillement", szSupply_.GetData() );
    output.WriteField( "Communications", szCom_.GetData() );
    output.WriteField( "ODB", szODB_.GetData() );
    output.WriteField( "Pathfind", szPathfinder_.GetData() );
    output.WriteField( "Missions", szMissions_.GetData() );
    output.WriteField( "HLA", szHLA_.GetData() );
    output.EndSection();   // Donnees
}


//-----------------------------------------------------------------------------
// Name: NetInfos::NetInfos
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Project_Data::NetInfos::NetInfos()
: nServerPort_()
, nServerMagic_()
, nLauncherPort_()
, nLauncherMagic_()
, bNetworkThreadActive_()
{
}


// -----------------------------------------------------------------------------
// Name: NetInfos::ReadArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::NetInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Reseau" );

    input.Section( "AgentServer-MOSServer" );
    input.ReadField( "BasePort", nServerPort_ );
    input.ReadField( "Magic", nServerMagic_ );
    input.EndSection();   // AgentServer-MOSServer

    input.Section( "AgentServer-Launcher" );
    input.ReadField( "Port", nLauncherPort_ );
    input.ReadField( "Magic", nLauncherMagic_ );
    input.EndSection();   // AgentServer-Launcher

    input.ReadField( "ThreadReseauActif", bNetworkThreadActive_ );
    input.EndSection();   // Reseau
}


// -----------------------------------------------------------------------------
// Name: NetInfos::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::NetInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Reseau" );

    output.Section( "AgentServer-MOSServer" );
    output.WriteField( "BasePort", nServerPort_.GetData() );
    output.WriteField( "Magic", nServerMagic_.GetData() );
    output.EndSection();   // AgentServer-MOSServer

    output.Section( "AgentServer-Launcher" );
    output.WriteField( "Port", nLauncherPort_.GetData() );
    output.WriteField( "Magic", nLauncherMagic_.GetData() );
    output.EndSection();   // AgentServer-Launcher

    output.WriteField( "ThreadReseauActif", bNetworkThreadActive_.GetData() );

    output.EndSection();   // Reseau
}


//-----------------------------------------------------------------------------
// Name: FileInfos::FileInfos
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
ADN_Project_Data::FileInfos::FileInfos()
: szFileName_()
{
}


//-----------------------------------------------------------------------------
// Name: FileInfos::FileInfos
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
ADN_Project_Data::FileInfos::FileInfos(const std::string& filename)
: szFileName_(ADN_Project_Data::GetWorkDirInfos().GetPartPath(filename))
{
}


//-----------------------------------------------------------------------------
// Name: FileInfos::operator =
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
ADN_Project_Data::FileInfos& ADN_Project_Data::FileInfos::operator =(const std::string&filename)
{
    szFileName_=ADN_Project_Data::GetWorkDirInfos().GetPartPath(filename);
    return *this;
}


//-----------------------------------------------------------------------------
// Name: WorkDirInfos::WorkDirInfos
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
ADN_Project_Data::WorkDirInfos::WorkDirInfos()
: szWorkingDir_()
, szTempDir_()
, bTmpActivated_(false)
{
}


//-----------------------------------------------------------------------------
// Name: WorkDirInfos::GetFullPath
// Created: JDY 03-06-24
//-----------------------------------------------------------------------------
std::string ADN_Project_Data::WorkDirInfos::GetFullPath( const std::string& part, E_WorkDir e )
{
    return ( ( e == eWorking ) ? szWorkingDir_.GetData() : szTempDir_.GetData() ) + part;
}


//-----------------------------------------------------------------------------
// Name: WorkDirInfos::GetPartPath
// Created: JDY 03-06-24
//-----------------------------------------------------------------------------
std::string ADN_Project_Data::WorkDirInfos::GetPartPath( const std::string& full, E_WorkDir e )
{
    std::string dir = ( e == eWorking ) ? szWorkingDir_.GetData() : szTempDir_.GetData();
    if( strcmpi( dir.c_str(), full.substr( 0, dir.size() ).c_str() ) )
        return std::string();
    else
        return full.substr( dir.size(), full.size() - dir.size() );
}


//-----------------------------------------------------------------------------
// Name: WorkDirInfos::SetWorkingDirectory
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
void ADN_Project_Data::WorkDirInfos::SetWorkingDirectory(const std::string& filename)
{
    char szDrive[_MAX_PATH];
    char szDir[_MAX_PATH];
    char szFile[_MAX_PATH];
    char szExt[_MAX_PATH];
    char szPath[_MAX_PATH];
    _splitpath( filename.c_str(), szDrive, szDir, szFile, szExt );
    _makepath( szPath, szDrive, szDir, 0, 0 );
    szWorkingDir_ = std::string( szPath );
}


//-----------------------------------------------------------------------------
// Name: WorkDirInfos::UseTempDirectory
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
void ADN_Project_Data::WorkDirInfos::UseTempDirectory(bool bActivateTemp)
{
    bTmpActivated_=bActivateTemp;

    if( bActivateTemp )
    {
        char *pTempDir=(char*)malloc(sizeof(char)*_MAX_PATH);
        int   len;
        if( ( len=GetTempPath(_MAX_PATH,pTempDir) ) > _MAX_PATH )
        {
            pTempDir=(char*)realloc(pTempDir,sizeof(char)*len+1);
            assert( (int)GetTempPath(len+1 ,pTempDir) <= len+1 );
        }

        szTempDir_=ADN_Tools::Replace(pTempDir,'\\','/')+"scipio data.tmp/";

        free(pTempDir);
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Project_Data constructor
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
ADN_Project_Data::ADN_Project_Data()
: ADN_Data_ABC   ()
, simInfos_      ()
, dataInfos_     ()
, netInfos_      ()
, pathfinderInfo_()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Project_Data destructor
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
ADN_Project_Data::~ADN_Project_Data()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::SetFile
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
void ADN_Project_Data::SetFile( const std::string& strFile )
{
    // Set working directory
    workDir_.SetWorkingDirectory( strFile );

    // Set file
    szFile_ = strFile;
}


// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::FilesNeeded
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
void ADN_Project_Data::FilesNeeded( T_StringList& vFiles ) const
{
    ADN_Project_Data* that = const_cast<ADN_Project_Data*>(this);
    assert( ! that->szFile_.GetFileName().GetData().empty() );
//    vFiles.push_back( that->szFile_.GetFileName().GetData() );
    vFiles.push_back( that->dataInfos_.szNetwork_.GetData() );
    vFiles.push_back( that->dataInfos_.szPathfinder_.GetData() );
    vFiles.push_back( that->dataInfos_.szIDs_.GetData() );
}


//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::Reset
// Created: JDY 03-06-23
//-----------------------------------------------------------------------------
void ADN_Project_Data::Reset()
{
    assert( ! szFile_.GetFileName().GetData().empty() );

    // load default parameters
    ADN_XmlInput_Helper defaultScipioFile;
    defaultScipioFile.SetData( (char*)LockResource( LoadResource(GetModuleHandle(NULL),
                         FindResource(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_XML_SCIPIO),"xml"))));  
    this->ReadArchive( defaultScipioFile );

    ADN_XmlInput_Helper defaultNetworkFile;
    defaultNetworkFile.SetData( (char*)LockResource( LoadResource(GetModuleHandle(NULL),
        FindResource(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_XML_RESEAU),"xml"))));  
    netInfos_.ReadArchive( defaultNetworkFile );
}


//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::Load
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
void ADN_Project_Data::Load()
{
    assert( ! szFile_.GetFileName().GetData().empty() );

    // Read the Scipio file
    ADN_XmlInput_Helper input;

    if( !input.Open(szFile_.GetFileNameFull()) )
        throw ADN_OpenFile_Exception( szFile_.GetFileNameFull() );

    try
    {
        this->ReadArchive( input );
    }
    catch( ADN_Xml_Exception& xmlException )
    {
        throw ADN_Xml_Exception( szFile_.GetFileNameFull(), xmlException.GetContext(), xmlException.GetErrorMessage() );
    }
    catch( MT_ArchiveLogger_Exception& xmlException )
    {
        throw ADN_DataException( "", xmlException.what() );
    }

    // Read the network file.
    ADN_XmlInput_Helper netInput;
    std::string strNetFile = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData()
        + dataInfos_.szNetwork_.GetData();

    if( !netInput.Open( strNetFile ) )
        throw ADN_OpenFile_Exception( strNetFile );

    try
    {
        netInfos_.ReadArchive( netInput );
    }
    catch( ADN_Xml_Exception& xmlException )
    {
        throw ADN_Xml_Exception( strNetFile, xmlException.GetContext(), xmlException.GetErrorMessage() );
    }
    catch( MT_ArchiveLogger_Exception& xmlException )
    {
        throw ADN_DataException( "", xmlException.what() );
    }

    // Read the pathfinder file.
    ADN_XmlInput_Helper pathfinderInput;
    std::string strPathfinderFile = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData()
        + dataInfos_.szPathfinder_.GetData();

    if( !pathfinderInput.Open( strPathfinderFile ) )
        throw ADN_OpenFile_Exception( strPathfinderFile );

    try
    {
        pathfinderInfo_.ReadArchive( pathfinderInput );
    }
    catch( ADN_Xml_Exception& xmlException )
    {
        throw ADN_Xml_Exception( strPathfinderFile, xmlException.GetContext(), xmlException.GetErrorMessage() );
    }
    catch( MT_ArchiveLogger_Exception& xmlException )
    {
        throw ADN_DataException( "", xmlException.what() );
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::Save
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
void ADN_Project_Data::Save()
{
    assert( ! szFile_.GetFileName().GetData().empty() );

    // Save the scipio file.
    MT_XXmlOutputArchive output;
    this->WriteArchive( output );

    std::string szFile = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory()  + szFile_.GetFileName().GetData();
    ADN_Tools::CreatePathToFile( szFile );
    if( ! output.WriteToFile( szFile ) )
        throw ADN_SaveFile_Exception( szFile );

    // Save the network file.
    MT_XXmlOutputArchive netOutput;
    netInfos_.WriteArchive( netOutput );

    std::string szNetFile = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() + dataInfos_.szNetwork_.GetData();
    ADN_Tools::CreatePathToFile( szNetFile );
    if( ! netOutput.WriteToFile( szNetFile ) )
        throw ADN_SaveFile_Exception( szNetFile );

    // Save the pathfinder file.
    MT_XXmlOutputArchive pathfinderOutput;
    pathfinderInfo_.WriteArchive( pathfinderOutput );

    std::string szPathfinderFile = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() + dataInfos_.szPathfinder_.GetData();
    ADN_Tools::CreatePathToFile( szPathfinderFile );
    if( ! pathfinderOutput.WriteToFile( szPathfinderFile ) )
        throw ADN_SaveFile_Exception( szPathfinderFile );

    // Save the Id file (from an embedded resource)
    MT_TextOutputArchive idsOutput;
    const char* szIds = (char*)LockResource( 
            LoadResource( GetModuleHandle(NULL), FindResource( GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_XML_IDS_CLASSES), "xml" )));
    idsOutput.GetOutputStream() << szIds;

    std::string szIdsFile = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() + dataInfos_.szIDs_.GetData();
    ADN_Tools::CreatePathToFile( szIdsFile );
    if( ! idsOutput.WriteToFile( szIdsFile ) )
        throw ADN_SaveFile_Exception( szIdsFile );
}


// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::ReadArchive
// Created: AGN 2004-07-02
// -----------------------------------------------------------------------------
void ADN_Project_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section("Scipio");
    simInfos_.ReadArchive( input );
    dataInfos_.ReadArchive( input );
    input.EndSection();   // Scipio
}


// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section("Scipio");
    simInfos_.WriteArchive( output );
    dataInfos_.WriteArchive( output );
    output.EndSection();   // Scipio
}
