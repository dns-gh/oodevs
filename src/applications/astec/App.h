//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/App.h $
// $Author: Age $
// $Modtime: 13/05/05 17:15 $
// $Revision: 21 $
// $Workfile: App.h $
//
//*****************************************************************************

#ifndef __App_h_
#define __App_h_

#include "Types.h"

#include "RawVisionData.h"
#include "Resource.h"

#include <qapplication.h>

class Network;
class AgentManager;
class LineManager;
class World;
class ObjectManager;
class Meteo_Manager;
class MainWindow;
class Agent;
class Agent_ABC;
class Population;
class Report_ABC;
class Gtia;
class Team;
class Population;
class PopulationConcentration;
class PopulationFlow;
class AgentKnowledge;
class ObjectKnowledge;
class PopulationKnowledge;
class PopulationConcentrationKnowledge;
class PopulationFlowKnowledge;
class Object_ABC;
class TacticalLine_ABC;
class QSplashScreen;
class TypePopulation;
class Resource;

typedef std::map< MIL_AgentID, std::string > T_MosId_String_Map;
typedef T_MosId_String_Map::iterator         IT_MosId_String_Map;
typedef T_MosId_String_Map::const_iterator   CIT_MosId_String_Map;

class SensorType;
class LogSupplyConsign;
class LogMedicalConsign;
class LogMaintenanceConsign;
class Options;

//=============================================================================
// Created: NLD 2002-07-15
//=============================================================================
class App : public QApplication
{
    Q_OBJECT
    MT_COPYNOTALLOWED( App );

public:
    //-------------------------------------------------------------------------
    /** @name Constructor, destructor and accessor */
    //-------------------------------------------------------------------------
    //@{
    App( int nArgc, char** ppArgv );
    ~App();

    static App& GetApp();
    //@}

    void InitializeData( const std::string& strFilename );

    bool Is3D() const;
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
    void NotifyConnexionLost();
    void NotifyPauseStatusChanged( bool bPaused );
    void NotifySpeedChanged( int nSpeed );
    void NotifyTimeChanged( uint nTimeInSeconds );
    void NotifyTickStartEnd( bool bTickStart );

    void NotifyAgentCreated( Agent& agent );
    void NotifyAgentUpdated( Agent& agent );
    void NotifyAgentReparented( Agent& agent );

    void NotifyObjectCreated( Object_ABC& object );
    void NotifyObjectUpdated( Object_ABC& object );
    void NotifyObjectDeleted( Object_ABC& object );

	void NotifyPopulationCreated( Population& population );
	void NotifyPopulationUpdated( Population& population );
    void NotifyPopulationDeleted( Population& population );
    void NotifyPopulationConcentrationCreated( const PopulationConcentration& concentration );
    void NotifyPopulationConcentrationDeleted( const PopulationConcentration& concentration );
    void NotifyPopulationConcentrationUpdated( const PopulationConcentration& concentration );
    void NotifyPopulationFlowCreated         ( const PopulationFlow&          flow          );
    void NotifyPopulationFlowDeleted         ( const PopulationFlow&          flow          );
    void NotifyPopulationFlowUpdated         ( const PopulationFlow&          flow          );

    void NotifyLogisticConsignUpdated( LogSupplyConsign& consign );
    void NotifyLogisticConsignUpdated( LogMedicalConsign& consign );
    void NotifyLogisticConsignUpdated( LogMaintenanceConsign& consign );

    void NotifyGtiaCreated( Gtia& gtia );
    void NotifyGtiaDeleted( Gtia& gtia );

    void NotifyTeamCreated( Team& team );
    void NotifyTeamDeleted( Team& team );

    void NotifyReportCreated( Agent_ABC& agent, Report_ABC& report );

    void NotifyAgentOutOfGas( Agent& agent );
    void NotifyAgentRefueled( Agent& agent );

    void NotifyConflictStarted( Agent_ABC&  origin );
    void NotifyConflictEnded  ( Agent_ABC&  origin );
    void NotifyObjectExplosion( Object_ABC& object );

    void NotifyAgentKnowledgeCreated( Gtia& gtia, AgentKnowledge& knowledge );
    void NotifyAgentKnowledgeUpdated( Gtia& gtia, AgentKnowledge& knowledge );
    void NotifyAgentKnowledgeDeleted( Gtia& gtia, AgentKnowledge& knowledge );

    void NotifyObjectKnowledgeCreated( Team& team, ObjectKnowledge& knowledge );
    void NotifyObjectKnowledgeUpdated( Team& team, ObjectKnowledge& knowledge );
    void NotifyObjectKnowledgeDeleted( Team& team, ObjectKnowledge& knowledge );

    void NotifyPopulationKnowledgeCreated( Gtia& gtia, PopulationKnowledge& knowledge );
    void NotifyPopulationKnowledgeUpdated( Gtia& gtia, PopulationKnowledge& knowledge );
    void NotifyPopulationKnowledgeDeleted( Gtia& gtia, PopulationKnowledge& knowledge );

    void NotifyPopulationConcentrationKnowledgeCreated( Gtia& gtia, PopulationConcentrationKnowledge& knowledge );
    void NotifyPopulationConcentrationKnowledgeUpdated( Gtia& gtia, PopulationConcentrationKnowledge& knowledge );
    void NotifyPopulationConcentrationKnowledgeDeleted( Gtia& gtia, PopulationConcentrationKnowledge& knowledge );

    void NotifyPopulationFlowKnowledgeCreated( Gtia& gtia, PopulationFlowKnowledge& knowledge );
    void NotifyPopulationFlowKnowledgeUpdated( Gtia& gtia, PopulationFlowKnowledge& knowledge );
    void NotifyPopulationFlowKnowledgeDeleted( Gtia& gtia, PopulationFlowKnowledge& knowledge );

    void NotifyTacticalLineDeleted( TacticalLine_ABC& line );

    void NotifyTypePopulationCreated( TypePopulation& type );
    //@}

    //! @name Accessors
    //@{
    Network&       GetNetwork       () const;
    AgentManager&  GetAgentManager  () const;
    LineManager&   GetLineManager   () const;
    World&         GetWorld         () const;
    ObjectManager& GetObjectManager () const;
    RawVisionData& GetRawVisionData ();
    Meteo_Manager& GetWeatherManager() const;
    MainWindow&    GetMainWindow    () const;
    uint           GetTime          () const;
    uint           GetTickDuration  () const;
    //@}

    void SetSplashText( const QString& strText );

    //! @name Helpers
    //@{
    std::string                 GetResourceName         ( MIL_AgentID )                     const;
    const Resource&             GetResource             ( MIL_AgentID )                     const;
    std::string                 GetEquipmentName        ( MIL_AgentID )                     const;
    uint                        GetEquipementID         ( const std::string& strName )      const;
    std::string                 GetTransportDotationName( MIL_AgentID )                     const;
    std::string                 GetBreakDownName        ( uint )                            const;
    std::string                 GetNBCName              ( uint )                            const;
    uint                        GetNBCID                ( const std::string& strName )      const;
    const T_MosId_String_Map&   GetNBCNames             ()                                  const;
    const T_MosId_String_Map&   GetResourceNames        ()                                  const;
    const T_MosId_String_Map&   GetEquipmentNames       ()                                  const;
    const SensorType*           FindSensorType          ( const std::string& strName )      const;
    unsigned int                GetRessourceID          ( const std::string& strRessource ) const;
    //@}

    //! @name Options
    //@{
    Options& GetOptions() const;
    //@}

signals:
    void ConnexionStatusChanged( bool bConnected );
    void PauseStatusChanged( bool bPaused );
    void SpeedChanged( int nSpeed );
    void TimeChanged( uint nTimeInSeconds );
    void TickStartEnd( bool bTickStart );

    void AgentCreated( Agent& agent );
    void AgentUpdated( Agent& agent );
    void AgentReparented( Agent& agent );
	
	void PopulationCreated( Population& );
	void PopulationUpdated( Population& );
    void PopulationDeleted( Population& );

    void PopulationConcentrationCreated( const PopulationConcentration& concentration );
    void PopulationConcentrationDeleted( const PopulationConcentration& concentration );
    void PopulationConcentrationUpdated( const PopulationConcentration& concentration );
    void PopulationFlowCreated         ( const PopulationFlow& flow );
    void PopulationFlowDeleted         ( const PopulationFlow& flow );
    void PopulationFlowUpdated         ( const PopulationFlow& flow );

    void ObjectCreated( Object_ABC& object );
    void ObjectUpdated( Object_ABC& object );
    void ObjectDeleted( Object_ABC& object );
    
    void GtiaCreated( Gtia& gtia );
    void GtiaDeleted( Gtia& gtia );

    void TeamCreated( Team& team );
    void TeamDeleted( Team& team );

    void ReportCreated( Agent_ABC& agent, Report_ABC& report );

    void AgentOutOfGas( Agent& agent );
    void AgentRefueled( Agent& agent );

    void ConflictStarted( Agent_ABC&  origin );
    void ConflictEnded  ( Agent_ABC&  origin );
    void ObjectExplosion( Object_ABC& object );

    void AgentKnowledgeCreated( Gtia& gtia, AgentKnowledge& knowledge );
    void AgentKnowledgeUpdated( Gtia& gtia, AgentKnowledge& knowledge );
    void AgentKnowledgeDeleted( Gtia& gtia, AgentKnowledge& knowledge );

    void ObjectKnowledgeCreated( Team& team, ObjectKnowledge& knowledge );
    void ObjectKnowledgeUpdated( Team& team, ObjectKnowledge& knowledge );
    void ObjectKnowledgeDeleted( Team& team, ObjectKnowledge& knowledge );

    void PopulationKnowledgeCreated( Gtia& gtia, PopulationKnowledge& knowledge );
    void PopulationKnowledgeUpdated( Gtia& gtia, PopulationKnowledge& knowledge );
    void PopulationKnowledgeDeleted( Gtia& gtia, PopulationKnowledge& knowledge );

    void PopulationConcentrationKnowledgeCreated( Gtia& gtia, PopulationConcentrationKnowledge& knowledge );
    void PopulationConcentrationKnowledgeUpdated( Gtia& gtia, PopulationConcentrationKnowledge& knowledge );
    void PopulationConcentrationKnowledgeDeleted( Gtia& gtia, PopulationConcentrationKnowledge& knowledge );

    void PopulationFlowKnowledgeCreated( Gtia& gtia, PopulationFlowKnowledge& knowledge );
    void PopulationFlowKnowledgeUpdated( Gtia& gtia, PopulationFlowKnowledge& knowledge );
    void PopulationFlowKnowledgeDeleted( Gtia& gtia, PopulationFlowKnowledge& knowledge );

    void TacticalLineDeleted( TacticalLine_ABC& line );

    void TypePopulationCreated( TypePopulation& type );

private slots:
    void UpdateData();
    void UpdateDisplay();
    void OnSpeedChanged( int nTickDuration );

private:
    void InitializeRawVisionData( InputArchive& scipioArchive );
    void InitializeTerrainData  ( InputArchive& scipioArchive );
    void InitializeRessourceIDs ( InputArchive& scipioArchive );
    void InitializeEquipementIDs( InputArchive& scipioArchive );
    void InitializeAgentNBCIDs  ( InputArchive& scipioArchive );
    void InitializeMeteo        ( InputArchive& scipioArchive );
    void InitializeSensors      ( InputArchive& scipioArchive );
    void InitializeHumanFactors ( InputArchive& scipioArchive );
    void InitializeObjectIds    ( InputArchive& scipioArchive );

    friend class GLTool;
private:
    typedef std::map< std::string, const SensorType* > T_SensorTypeMap;
    typedef T_SensorTypeMap::const_iterator                CIT_SensorTypeMap;

public:
    typedef std::map< uint, Resource >                T_ResourceMap;
    typedef T_ResourceMap::iterator                       IT_ResourceMap;
    typedef T_ResourceMap::const_iterator                 CIT_ResourceMap;

private:
    //! @name Member data
    //@{
    Network*       pMOSServer_;
    World*         pWorld_;
    AgentManager*  pAgentManager_;
    LineManager*   pLineManager_;
    ObjectManager* pObjectManager_;
    Meteo_Manager* pWeatherManager_;
    RawVisionData  rawData_;

    MainWindow*    pMainWindow_;
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
    Options*           pOptions_;
    //@}

private:
    static App*    pInstance_;
};


#   include "App.inl"

#endif // __App_h_
