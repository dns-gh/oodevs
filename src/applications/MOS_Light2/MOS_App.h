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

#include <qapplication.h>

class MOS_MOSServer;
class MOS_AgentManager;
class MOS_LineManager;
class MOS_World;
class MOS_DynaObjectManager;
class MOS_Meteo_Manager;
class MOS_MainWindow;
class MOS_Agent;
class MOS_Report_ABC;
class MOS_Gtia;
class MOS_Team;
class MOS_AgentKnowledge;
class MOS_DynaObjectKnowledge;
class MOS_DynaObject;
class MOS_TacticalLine_ABC;
class QSplashScreen;

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

    void NotifyDynaObjectCreated( MOS_DynaObject& object );
    void NotifyDynaObjectUpdated( MOS_DynaObject& object );
    void NotifyDynaObjectDeleted( MOS_DynaObject& object );

    void NotifyLogisticConsignUpdated( MOS_LogSupplyConsign& consign );
    void NotifyLogisticConsignUpdated( MOS_LogMedicalConsign& consign );
    void NotifyLogisticConsignUpdated( MOS_LogMaintenanceConsign& consign );
    
    void NotifyGtiaCreated( MOS_Gtia& gtia );
    void NotifyGtiaDeleted( MOS_Gtia& gtia );

    void NotifyTeamCreated( MOS_Team& team );

    void NotifyReportCreated( MOS_Agent& agent, MOS_Report_ABC& report );

    void NotifyAgentOutOfGas( MOS_Agent& agent );
    void NotifyAgentRefueled( MOS_Agent& agent );

    void NotifyAgentConflictStarted( MOS_Agent& agent );
    void NotifyAgentConflictEnded  ( MOS_Agent& agent );

    void NotifyAgentKnowledgeCreated( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );
    void NotifyAgentKnowledgeUpdated( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );
    void NotifyAgentKnowledgeDeleted( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );

    void NotifyObjectKnowledgeCreated( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge );
    void NotifyObjectKnowledgeUpdated( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge );
    void NotifyObjectKnowledgeDeleted( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge );

    void NotifyTacticalLineDeleted( MOS_TacticalLine_ABC& line );
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_MOSServer&         GetMOSServer        () const;
    MOS_AgentManager&      GetAgentManager     () const;
    MOS_LineManager&       GetLineManager      () const;
    MOS_World&             GetWorld            () const;
    MOS_DynaObjectManager& GetDynaObjectManager() const;
    MOS_RawVisionData&     GetRawVisionData    ();
    MOS_Meteo_Manager&     GetWeatherManager   () const;
    MOS_MainWindow&        GetMainWindow       () const;
    uint                   GetTime             () const;
    //@}

    void SetSplashText( const QString& strText );

    //-------------------------------------------------------------------------
    /** @name Helpers */
    //-------------------------------------------------------------------------
    //@{
    std::string GetResourceName         ( MIL_AgentID ) const;
    std::string GetEquipmentName        ( MIL_AgentID ) const;
    uint        GetEquipementID         ( const std::string& strName ) const;
    std::string GetTransportDotationName( MIL_AgentID ) const;
    std::string GetBreakDownName        ( uint )        const;
    std::string GetNBCName              ( uint )        const;
    const T_MosId_String_Map& GetNBCNames() const;
    const MOS_SensorType* FindSensorType( const std::string& strName ) const;
    unsigned int GetRessourceID( const std::string& strRessource ) const;
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

    void DynaObjectCreated( MOS_DynaObject& object );
    void DynaObjectUpdated( MOS_DynaObject& object );
    void DynaObjectDeleted( MOS_DynaObject& object );
    
    void GtiaCreated( MOS_Gtia& gtia );
    void GtiaDeleted( MOS_Gtia& gtia );

    void TeamCreated( MOS_Team& team );

    void ReportCreated( MOS_Agent& agent, MOS_Report_ABC& report );

    void AgentOutOfGas( MOS_Agent& agent );
    void AgentRefueled( MOS_Agent& agent );

    void AgentConflictStarted( MOS_Agent& agent );
    void AgentConflictEnded( MOS_Agent& agent );

    void AgentKnowledgeCreated( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );
    void AgentKnowledgeUpdated( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );
    void AgentKnowledgeDeleted( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );

    void ObjectKnowledgeCreated( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge );
    void ObjectKnowledgeUpdated( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge );
    void ObjectKnowledgeDeleted( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge );

    void TacticalLineDeleted( MOS_TacticalLine_ABC& line );

private slots:
    void UpdateData();
    void UpdateDisplay();

private:
    void InitializeRawVisionData( MT_InputArchive_ABC& scipioArchive );
    void InitializeTerrainData  ( MT_InputArchive_ABC& scipioArchive );
    void InitializeRessourceIDs ( MT_InputArchive_ABC& scipioArchive );
    void InitializeEquipementIDs( MT_InputArchive_ABC& scipioArchive );
    void InitializeAgentNBCIDs  ( MT_InputArchive_ABC& scipioArchive );
    void InitializeMeteo        ( MT_InputArchive_ABC& scipioArchive );
    void InitializeSensors      ( MT_InputArchive_ABC& scipioArchive );
    void InitializeHumanFactors ( MT_InputArchive_ABC& scipioArchive );
    void InitializeObjectIds    ( MT_InputArchive_ABC& scipioArchive );

    friend class MOS_GLTool;
private:
    typedef std::map< std::string, const MOS_SensorType* > T_SensorTypeMap;
    typedef T_SensorTypeMap::const_iterator                CIT_SensorTypeMap;

private:
    //! @name Member data
    //@{
    MOS_MOSServer*         pMOSServer_;
    MOS_World*             pWorld_;
    MOS_AgentManager*      pAgentManager_;
    MOS_LineManager*       pLineManager_;
    MOS_DynaObjectManager* pDynaObjectManager_;
    MOS_Meteo_Manager*     pWeatherManager_;
    MOS_RawVisionData      rawData_;

    MOS_MainWindow*        pMainWindow_;
    QSplashScreen*         pSplashScreen_;

    uint                   nTimeSeconds_;

    T_MosId_String_Map     equipementNameMap_;
    T_MosId_String_Map     resourcesNameMap_;
    T_MosId_String_Map     breakDownsNameMap_;
    T_MosId_String_Map     nbcNamesMap_;

    T_SensorTypeMap        sensorTypes_;

    bool                   bODBEditor_;
    std::string            strODBFilename_;

    // $$$$ SBO 2005-07-26: to manage -conffile cmd line option
    std::string            strRootDirectory_;
    std::string            strRootConfigFile_;

    QTimer*                pNetworkTimer_;
    QTimer*                pDisplayTimer_;
    //@}

private:
    static MOS_App*        pInstance_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_App.inl"
#endif

#endif // __App_h_
