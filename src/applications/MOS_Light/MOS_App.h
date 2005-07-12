//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_App.h $
// $Author: Nld $
// $Modtime: 1/04/05 18:52 $
// $Revision: 25 $
// $Workfile: MOS_App.h $
//
//*****************************************************************************

#ifndef __App_h_
#define __App_h_

#include "MOS_Types.h"

#include "MOS_RawVisionData.h"

#include <qapplication.h>

class MOS_MainWindow;
class MOS_MOSServer;
class MOS_AgentManager;
class MOS_LineManager;
class MOS_World;
class MOS_DynaObjectManager;
class MOS_TestManager;
class MOS_Meteo_Manager;
class MOS_SensorType;


#include "MT/MT_Archive/MT_FlatBinaryOutputArchive.h"
#include "MT/MT_Archive/MT_FlatBinaryInputArchive.h"

//=============================================================================
// Created: NLD 2002-07-15
//=============================================================================
class MOS_App : public QApplication
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_App );

public:
    //! @name 
    //@{
    typedef std::map< MIL_AgentID, std::string > T_MosId_String_Map;
    typedef T_MosId_String_Map::iterator         IT_MosId_String_Map;
    typedef T_MosId_String_Map::const_iterator   CIT_MosId_String_Map;
    //@}

public:
    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    static void     CreateApp ( int nArgc, char** ppArgv );
    static MOS_App& GetApp    ();
    static void     DestroyApp();
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Run       ();
    void Terminate ();
    //@}
    
    //-------------------------------------------------------------------------
    /** @name Actions */
    //-------------------------------------------------------------------------
    //@{
    void NotifyTimeHasChanged( uint nTimeInSeconds );
    void HideEverything();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_MOSServer&         GetMOSServer        () const;
    MOS_MainWindow&        GetMainWindow       () const;
    MOS_AgentManager&      GetAgentManager     () const;
    MOS_TestManager&       GetTestManager      () const;
    MOS_LineManager&       GetLineManager      () const;
    MOS_World&             GetWorld            () const;
    MOS_DynaObjectManager& GetDynaObjectManager() const;
    MOS_RawVisionData&     GetRawVisionData    ();
    int                    GetTime             () const;
    MOS_Meteo_Manager&     GetWeatherManager   () const;
    const T_MosId_String_Map&    GetEquipements      () const;
    const T_MosId_String_Map&    GetRessources() const;
    //@}

    void SavePoint( const MT_Vector2D& point );

    std::string GetRessourceName        ( MIL_AgentID ) const;
    uint        GetRessourceID          ( const std::string& strName ) const;
    std::string GetEquipementName       ( MIL_AgentID ) const;
    uint        GetEquipementID         ( const std::string& strName ) const;
//    std::string GetTransportDotationName( MIL_AgentID ) const;
    std::string GetBreakDownName        ( uint )        const;
    std::string GetAgentNBCName         ( uint )        const;

    uint GetRandomAgentNBC() const;

    const MOS_SensorType* FindSensorType( const std::string& strName ) const;

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MOS_SensorType* > T_SensorTypeMap;
    typedef T_SensorTypeMap::const_iterator                CIT_SensorTypeMap;
    //@}

private slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void OnTimeOut();
    //@}

private:
     MOS_App( int nArgc, char** ppArgv );
    ~MOS_App();

    void InitializeRawVisionData( MT_InputArchive_ABC& scipioArchive );
    void InitializeTerrainData  ( MT_InputArchive_ABC& scipioArchive );
    void InitializeRessourceIDs ( MT_InputArchive_ABC& scipioArchive );
    void InitializeEquipementIDs( MT_InputArchive_ABC& scipioArchive );
    void InitializeAgentNBCIDs  ( MT_InputArchive_ABC& scipioArchive );
    void InitializeMeteo        ( MT_InputArchive_ABC& scipioArchive );
    void InitializeSensors      ( MT_InputArchive_ABC& scipioArchive );
    void InitializeHumanFactors ( MT_InputArchive_ABC& scipioArchive );
    void InitializeBreakdownIDs ( MT_InputArchive_ABC& scipioArchive );

    void RegisterSensorType( const MOS_SensorType& sensor );
    
private:
    MOS_MOSServer*         pMOSServer_;
    MOS_World*             pWorld_;
    MOS_AgentManager*      pAgentManager_;
    MOS_LineManager*       pLineManager_;
    MOS_MainWindow*        pMainWindow_;
    MOS_DynaObjectManager* pDynaObjectManager_;
    MOS_Meteo_Manager*     pWeatherManager_;
    MOS_RawVisionData      rawData_;
    MOS_TestManager*       pTestManager_;
    
    int                    nTimeSeconds_;
    bool                   bRunning_;

    typedef std::vector< std::string > T_StringVector;
    typedef T_StringVector::iterator   IT_StringVector;
    T_StringVector          pointsToSave_;

    T_MosId_String_Map equipementNameMap_;
    T_MosId_String_Map ressourcesNameMap_;
    T_MosId_String_Map breakDownsNameMap_;
    T_MosId_String_Map nbcAgentsMap_;

    T_SensorTypeMap sensorTypes_;
    
private:
    static MOS_App*                     pTheApp_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_App.inl"
#endif

#endif // __App_h_
