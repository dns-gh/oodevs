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
#include "Resource.h"
#include "ElementObserver_ABC.h"

#include <qapplication.h>

class Network;
class MainWindow;
class QSplashScreen;
class Model;
class Simulation;
class Controllers;
class Workers;
class InitializationMessage;

namespace xml { class xistream; }

// =============================================================================
/** @class  App
    @brief  Application
*/
// Created: AGE 2006-03-15
// =============================================================================
class App : public QApplication
          , public Observer_ABC
          , public ElementObserver_ABC< InitializationMessage >
{
    Q_OBJECT

public:
    static App& GetApp(); // $$$$ SBO 2006-03-16: 
    static std::string BuildChildPath( const std::string& parent, const std::string& child );

    //! @name Constructor/destructor
    //@{
             App( int nArgc, char** ppArgv );
    virtual ~App();
    //@}

    //! @name Accessors
    //@{
    Network&    GetNetwork   () const { return *network_; };
    MainWindow& GetMainWindow() const { return *mainWindow_; };
    Model&      GetModel     () const { return *model_; };
    //@}

private slots:
    //! @name Slots
    //@{
    void UpdateData();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    App( const App& );
    App& operator=( const App& );
    //@}

    //! @name Helpers
    //@{
    std::string RetrieveValidConfigFile( const std::string& conffile );
    void Initialize( int nArgc, char** ppArgv );
    void Initialize( const std::string& scipioXml );
    void InitializeHumanFactors ( xml::xistream& xis, const std::string& conffile );
    void SetSplashText( const QString& strText );
    virtual void NotifyUpdated( const InitializationMessage& message );
    //@}

private:
    //! @name Member data
    //@{
    Controllers*    controllers_;
    Model*          model_;
    Simulation*     simulation_;
    Workers*        workers_;

    Network*        network_;
    
    MainWindow*     mainWindow_;
    QSplashScreen*  splashScreen_;

    QTimer*         networkTimer_;
    //@}

private:
    static App*    pInstance_;
};

#endif // __App_h_
