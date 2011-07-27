// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Application_h_
#define __Application_h_

#include "clients_kernel/Application_ABC.h"

namespace kernel
{
    class Controllers;
}

class FileLoaderObserver;
class MainWindow;
class StaticModel;
class Model;
class Config;

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: SBO 2006-07-05
// =============================================================================
class Application : public Application_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Application( int argc, char** argv, const QString& license );
    virtual ~Application();
    //@}

    //! @name Operations
    //@{
    virtual bool notify( QObject* receiver, QEvent* e );
    bool Initialize();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Application( const Application& );            //!< Copy constructor
    Application& operator=( const Application& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    bool Initialize( int argc, char** argv );
    void DisplayError( const QString& text ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< FileLoaderObserver > observer_;
    std::auto_ptr< Config > config_;
    std::auto_ptr< kernel::Controllers > controllers_;
    std::auto_ptr< StaticModel > staticModel_;
    std::auto_ptr< Model > model_;
    MainWindow* mainWindow_;
    const QString license_;
    //@}
};

#endif // __Application_h_
