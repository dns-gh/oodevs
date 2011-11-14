// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Application_h_
#define __Application_h_

namespace frontend
{
    class LauncherClient;
}

namespace kernel
{
    class Controllers;
}

namespace tools
{
    class Loader_ABC;
    class RealFileLoaderObserver_ABC;
}

class Config;
class Launcher;

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: SBO 2008-02-21
// =============================================================================
class Application : public QApplication
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             Application( int argc, char** argv );
    virtual ~Application();
    //@}

    //! @name Operations
    //@{
    void Initialize();
    void CreateTranslators();
    void DeleteTranslators();
    void SetLauncherRootDir( const std::string& directory );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTimer();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Application( const Application& );            //!< Copy constructor
    Application& operator=( const Application& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddTranslator( const std::string file, const QString& locale );
    virtual bool notify( QObject* emitter, QEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Config > config_;
    std::auto_ptr< tools::RealFileLoaderObserver_ABC > fileLoaderObserver_;
    std::auto_ptr< tools::Loader_ABC > fileLoader_;
    std::auto_ptr< kernel::Controllers > controllers_;
    std::auto_ptr< Launcher > launcher_;
    std::auto_ptr< frontend::LauncherClient > launcherClient_;
    std::vector< QTranslator* > translators_;
    Q3MainWindow* mainWindow_;
    QTimer* timer_;
    //@}

};

#endif // __Application_h_
