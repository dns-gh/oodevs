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
class ObjectManager;
class Meteo_Manager;
class MainWindow;
class Agent;
class Agent_ABC;
class Population;
class Report_ABC;
class KnowledgeGroup;
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
class Model;
class Simulation;
class Controller;

typedef std::map< MIL_AgentID, std::string > T_MosId_String_Map;
typedef T_MosId_String_Map::iterator         IT_MosId_String_Map;
typedef T_MosId_String_Map::const_iterator   CIT_MosId_String_Map;

class SensorType;
class LogSupplyConsign;
class LogMedicalConsign;
class LogMaintenanceConsign;

namespace xml { class xistream; }

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

    //@}

    //! @name Accessors
    //@{
    Network&       GetNetwork       () const;
    Model&         GetModel  () const { return *model_; };
    MainWindow&    GetMainWindow() const;
    //@}

    void SetSplashText( const QString& strText );

private slots:
    //! @name Slots
    //@{
    void UpdateData();
    void UpdateDisplay();
    //@}

private:
    void Initialize( const std::string& scipioXml );
    void InitializeTerrainData  ( xml::xistream& xis );
    void InitializeHumanFactors ( xml::xistream& xis );

    friend class GLTool;

private:
    //! @name Member data
    //@{
    Controller*     controller_;
    Model*          model_;
    Simulation*     simulation_;

    Network*        pMOSServer_;
    
    MainWindow*    pMainWindow_;
    QSplashScreen*     pSplashScreen_;

    QTimer*            pNetworkTimer_;
    QTimer*            pDisplayTimer_;
    //@}

private:
    static App*    pInstance_;
};


#   include "App.inl"

#endif // __App_h_
