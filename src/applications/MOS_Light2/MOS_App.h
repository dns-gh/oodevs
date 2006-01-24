//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_App.h $
// $Author: Age $
// $Modtime: 13/05/05 17:15 $
// $Revision: 21 $
// $Workfile: MOS_App.h $
//
//*****************************************************************************

#ifndef __App_h_
#define __App_h_

#include "MOS_Types.h"

#include "MOS_RawVisionData.h"
#include "MOS_Resource.h"

#include <qapplication.h>

class MOS_MOSServer;
class MOS_AgentManager;
class MOS_LineManager;
class MOS_World;
class MOS_ObjectManager;
class MOS_Meteo_Manager;
class MOS_MainWindow;
class MOS_Agent;
class MOS_Agent_ABC;
class MOS_Population;
class MOS_Report_ABC;
class MOS_Gtia;
class MOS_Team;
class MOS_Population;
class MOS_PopulationConcentration;
class MOS_PopulationFlow;
class MOS_AgentKnowledge;
class MOS_ObjectKnowledge;
class MOS_PopulationKnowledge;
class MOS_PopulationConcentrationKnowledge;
class MOS_PopulationFlowKnowledge;
class MOS_Object_ABC;
class MOS_TacticalLine_ABC;
class QSplashScreen;
class MOS_TypePopulation;
class MOS_Resource;

typedef std::map< MIL_AgentID, std::string > T_MosId_String_Map;
typedef T_MosId_String_Map::iterator         IT_MosId_String_Map;
typedef T_MosId_String_Map::const_iterator   CIT_MosId_String_Map;

class MOS_SensorType;
class MOS_LogSupplyConsign;
class MOS_LogMedicalConsign;
class MOS_LogMaintenanceConsign;

//=============================================================================
// Created: NLD 2002-07-15
//=============================================================================
class MOS_App : public QApplication
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_App );

public:
    //-------------------------------------------------------------------------
    /** @name Constructor, destructor and accessor */
    //-------------------------------------------------------------------------
    //@{
    MOS_App( int nArgc, char** ppArgv );
    ~MOS_App();

    static MOS_App& GetApp();
    //@}

    void InitializeData( const std::string& strFilename );

    bool Is3D();
    bool Is3DEnabled() const;
    bool IsODBEdition();
    void ReadODB( std::string strFilename );
    void WriteODB( std::string strFilename );
    void WriteODB();
    const std::string& GetRootConfigFile() const;

    //-------------------------------------------------------------------------
    /** @name Notifications */
    //-------------------------------------------------------------------------
    //@{
    void NotifyConnexionStatusChanged( bool bConnected );
    void NotifyPauseStatusChanged( bool bPaused );
    void NotifySpeedChanged( int nSpeed );
    void NotifyTimeChanged( uint nTimeInSeconds );
    void NotifyTickStartEnd( bool bTickStart );

    void NotifyAgentCreated( MOS_Agent& agent );
    void NotifyAgentUpdated( MOS_Agent& agent );
    void NotifyAgentReparented( MOS_Agent& agent );

    void NotifyObjectCreated( MOS_Object_ABC& object );
    void NotifyObjectUpdated( MOS_Object_ABC& object );
    void NotifyObjectDeleted( MOS_Object_ABC& object );

	void NotifyPopulationCreated( MOS_Population& population );
	void NotifyPopulationUpdated( MOS_Population& population );
    void NotifyPopulationDeleted( MOS_Population& population );
    void NotifyPopulationConcentrationCreated( const MOS_PopulationConcentration& concentration );
    void NotifyPopulationConcentrationDeleted( const MOS_PopulationConcentration& concentration );
    void NotifyPopulationConcentrationUpdated( const MOS_PopulationConcentration& concentration );
    void NotifyPopulationFlowCreated         ( const MOS_PopulationFlow&          flow          );
    void NotifyPopulationFlowDeleted         ( const MOS_PopulationFlow&          flow          );
    void NotifyPopulationFlowUpdated         ( const MOS_PopulationFlow&          flow          );

    void NotifyLogisticConsignUpdated( MOS_LogSupplyConsign& consign );
    void NotifyLogisticConsignUpdated( MOS_LogMedicalConsign& consign );
    void NotifyLogisticConsignUpdated( MOS_LogMaintenanceConsign& consign );

    void NotifyGtiaCreated( MOS_Gtia& gtia );
    void NotifyGtiaDeleted( MOS_Gtia& gtia );

    void NotifyTeamCreated( MOS_Team& team );
    void NotifyTeamDeleted( MOS_Team& team );

    void NotifyReportCreated( MOS_Agent_ABC& agent, MOS_Report_ABC& report );

    void NotifyAgentOutOfGas( MOS_Agent& agent );
    void NotifyAgentRefueled( MOS_Agent& agent );

    void NotifyConflictStarted( MOS_Agent_ABC&  origin );
    void NotifyConflictEnded  ( MOS_Agent_ABC&  origin );
    void NotifyObjectExplosion( MOS_Object_ABC& object );

    void NotifyAgentKnowledgeCreated( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );
    void NotifyAgentKnowledgeUpdated( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );
    void NotifyAgentKnowledgeDeleted( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );

    void NotifyObjectKnowledgeCreated( MOS_Team& team, MOS_ObjectKnowledge& knowledge );
    void NotifyObjectKnowledgeUpdated( MOS_Team& team, MOS_ObjectKnowledge& knowledge );
    void NotifyObjectKnowledgeDeleted( MOS_Team& team, MOS_ObjectKnowledge& knowledge );

    void NotifyPopulationKnowledgeCreated( MOS_Gtia& gtia, MOS_PopulationKnowledge& knowledge );
    void NotifyPopulationKnowledgeUpdated( MOS_Gtia& gtia, MOS_PopulationKnowledge& knowledge );
    void NotifyPopulationKnowledgeDeleted( MOS_Gtia& gtia, MOS_PopulationKnowledge& knowledge );

    void NotifyPopulationConcentrationKnowledgeCreated( MOS_Gtia& gtia, MOS_PopulationConcentrationKnowledge& knowledge );
    void NotifyPopulationConcentrationKnowledgeUpdated( MOS_Gtia& gtia, MOS_PopulationConcentrationKnowledge& knowledge );
    void NotifyPopulationConcentrationKnowledgeDeleted( MOS_Gtia& gtia, MOS_PopulationConcentrationKnowledge& knowledge );

    void NotifyPopulationFlowKnowledgeCreated( MOS_Gtia& gtia, MOS_PopulationFlowKnowledge& knowledge );
    void NotifyPopulationFlowKnowledgeUpdated( MOS_Gtia& gtia, MOS_PopulationFlowKnowledge& knowledge );
    void NotifyPopulationFlowKnowledgeDeleted( MOS_Gtia& gtia, MOS_PopulationFlowKnowledge& knowledge );

    void NotifyTacticalLineDeleted( MOS_TacticalLine_ABC& line );

    void NotifyTypePopulationCreated( MOS_TypePopulation& type );
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_MOSServer&     GetMOSServer     () const;
    MOS_AgentManager&  GetAgentManager  () const;
    MOS_LineManager&   GetLineManager   () const;
    MOS_World&         GetWorld         () const;
    MOS_ObjectManager& GetObjectManager () const;
    MOS_RawVisionData& GetRawVisionData ();
    MOS_Meteo_Manager& GetWeatherManager() const;
    MOS_MainWindow&    GetMainWindow    () const;
    uint               GetTime          () const;
    uint               GetTickDuration  () const;
    //@}

    void SetSplashText( const QString& strText );

    //-------------------------------------------------------------------------
    /** @name Helpers */
    //-------------------------------------------------------------------------
    //@{
    std::string                 GetResourceName         ( MIL_AgentID )                     const;
    const MOS_Resource&         GetResource             ( MIL_AgentID )                     const;
    std::string                 GetEquipmentName        ( MIL_AgentID )                     const;
    uint                        GetEquipementID         ( const std::string& strName )      const;
    std::string                 GetTransportDotationName( MIL_AgentID )                     const;
    std::string                 GetBreakDownName        ( uint )                            const;
    std::string                 GetNBCName              ( uint )                            const;
    uint                        GetNBCID                ( const std::string& strName )      const;
    const T_MosId_String_Map&   GetNBCNames             ()                                  const;
    const T_MosId_String_Map&   GetResourceNames        ()                                  const;
    const T_MosId_String_Map&   GetEquipmentNames       ()                                  const;
    const MOS_SensorType*       FindSensorType          ( const std::string& strName )      const;
    unsigned int                GetRessourceID          ( const std::string& strRessource ) const;
    //@}

signals:
    void ConnexionStatusChanged( bool bConnected );
    void PauseStatusChanged( bool bPaused );
    void SpeedChanged( int nSpeed );
    void TimeChanged( uint nTimeInSeconds );
    void TickStartEnd( bool bTickStart );

    void AgentCreated( MOS_Agent& agent );
    void AgentUpdated( MOS_Agent& agent );
    void AgentReparented( MOS_Agent& agent );
	
	void PopulationCreated( MOS_Population& );
	void PopulationUpdated( MOS_Population& );
    void PopulationDeleted( MOS_Population& );

    void PopulationConcentrationCreated( const MOS_PopulationConcentration& concentration );
    void PopulationConcentrationDeleted( const MOS_PopulationConcentration& concentration );
    void PopulationConcentrationUpdated( const MOS_PopulationConcentration& concentration );
    void PopulationFlowCreated         ( const MOS_PopulationFlow& flow );
    void PopulationFlowDeleted         ( const MOS_PopulationFlow& flow );
    void PopulationFlowUpdated         ( const MOS_PopulationFlow& flow );

    void ObjectCreated( MOS_Object_ABC& object );
    void ObjectUpdated( MOS_Object_ABC& object );
    void ObjectDeleted( MOS_Object_ABC& object );
    
    void GtiaCreated( MOS_Gtia& gtia );
    void GtiaDeleted( MOS_Gtia& gtia );

    void TeamCreated( MOS_Team& team );
    void TeamDeleted( MOS_Team& team );

    void ReportCreated( MOS_Agent_ABC& agent, MOS_Report_ABC& report );

    void AgentOutOfGas( MOS_Agent& agent );
    void AgentRefueled( MOS_Agent& agent );

    void ConflictStarted( MOS_Agent_ABC&  origin );
    void ConflictEnded  ( MOS_Agent_ABC&  origin );
    void ObjectExplosion( MOS_Object_ABC& object );

    void AgentKnowledgeCreated( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );
    void AgentKnowledgeUpdated( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );
    void AgentKnowledgeDeleted( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );

    void ObjectKnowledgeCreated( MOS_Team& team, MOS_ObjectKnowledge& knowledge );
    void ObjectKnowledgeUpdated( MOS_Team& team, MOS_ObjectKnowledge& knowledge );
    void ObjectKnowledgeDeleted( MOS_Team& team, MOS_ObjectKnowledge& knowledge );

    void PopulationKnowledgeCreated( MOS_Gtia& gtia, MOS_PopulationKnowledge& knowledge );
    void PopulationKnowledgeUpdated( MOS_Gtia& gtia, MOS_PopulationKnowledge& knowledge );
    void PopulationKnowledgeDeleted( MOS_Gtia& gtia, MOS_PopulationKnowledge& knowledge );

    void PopulationConcentrationKnowledgeCreated( MOS_Gtia& gtia, MOS_PopulationConcentrationKnowledge& knowledge );
    void PopulationConcentrationKnowledgeUpdated( MOS_Gtia& gtia, MOS_PopulationConcentrationKnowledge& knowledge );
    void PopulationConcentrationKnowledgeDeleted( MOS_Gtia& gtia, MOS_PopulationConcentrationKnowledge& knowledge );

    void PopulationFlowKnowledgeCreated( MOS_Gtia& gtia, MOS_PopulationFlowKnowledge& knowledge );
    void PopulationFlowKnowledgeUpdated( MOS_Gtia& gtia, MOS_PopulationFlowKnowledge& knowledge );
    void PopulationFlowKnowledgeDeleted( MOS_Gtia& gtia, MOS_PopulationFlowKnowledge& knowledge );

    void TacticalLineDeleted( MOS_TacticalLine_ABC& line );

    void TypePopulationCreated( MOS_TypePopulation& type );

private slots:
    void UpdateData();
    void UpdateDisplay();
    void OnSpeedChanged( int nTickDuration );

private:
    void InitializeRawVisionData( MOS_InputArchive& scipioArchive );
    void InitializeTerrainData  ( MOS_InputArchive& scipioArchive );
    void InitializeRessourceIDs ( MOS_InputArchive& scipioArchive );
    void InitializeEquipementIDs( MOS_InputArchive& scipioArchive );
    void InitializeAgentNBCIDs  ( MOS_InputArchive& scipioArchive );
    void InitializeMeteo        ( MOS_InputArchive& scipioArchive );
    void InitializeSensors      ( MOS_InputArchive& scipioArchive );
    void InitializeHumanFactors ( MOS_InputArchive& scipioArchive );
    void InitializeObjectIds    ( MOS_InputArchive& scipioArchive );

    friend class MOS_GLTool;
private:
    typedef std::map< std::string, const MOS_SensorType* > T_SensorTypeMap;
    typedef T_SensorTypeMap::const_iterator                CIT_SensorTypeMap;

public:
    typedef std::map< uint, MOS_Resource >                T_ResourceMap;
    typedef T_ResourceMap::iterator                       IT_ResourceMap;
    typedef T_ResourceMap::const_iterator                 CIT_ResourceMap;

private:
    //! @name Member data
    //@{
    MOS_MOSServer*     pMOSServer_;
    MOS_World*         pWorld_;
    MOS_AgentManager*  pAgentManager_;
    MOS_LineManager*   pLineManager_;
    MOS_ObjectManager* pObjectManager_;
    MOS_Meteo_Manager* pWeatherManager_;
    MOS_RawVisionData  rawData_;

    MOS_MainWindow*    pMainWindow_;
    QSplashScreen*     pSplashScreen_;
    bool               b3dEnabled_;

    uint               nTimeSeconds_;
    uint               nTickDuration_;

    T_MosId_String_Map equipementNameMap_;
    T_MosId_String_Map resourcesNameMap_;
    T_MosId_String_Map breakDownsNameMap_;
    T_MosId_String_Map nbcNamesMap_;

    T_ResourceMap      resourcesMap_;

    T_SensorTypeMap    sensorTypes_;

    bool               bODBEditor_;
    std::string        strODBFilename_;

    std::string        strRootDirectory_;
    std::string        strRootConfigFile_;

    QTimer*            pNetworkTimer_;
    QTimer*            pDisplayTimer_;
    //@}

private:
    static MOS_App*    pInstance_;
};


#   include "MOS_App.inl"

#endif // __App_h_
