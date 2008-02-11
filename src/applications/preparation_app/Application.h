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

#include <qapplication.h>

namespace kernel
{
    class Controllers;
    class Workers;
}

class MainWindow;
class StaticModel;
class Model;
class Config;
class FlexLmLicense;

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: SBO 2006-07-05
// =============================================================================
class Application : public QApplication
{

public:
    //! @name Constructors/Destructor
    //@{
             Application( int argc, char** argv, const QString& locale, const FlexLmLicense& license );
    virtual ~Application();
    //@}

    //! @name Operations
    //@{
    void Initialize();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Application( const Application& );            //!< Copy constructor
    Application& operator=( const Application& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Initialize( int argc, char** argv );
    void AddTranslator( const QString& locale, const char* t );
    //@}

private:
    //! @name Member data
    //@{
    Config* config_;
    kernel::Controllers* controllers_;
    StaticModel* staticModel_;
    Model* model_;
    kernel::Workers* workers_;
    MainWindow* mainWindow_;
    const FlexLmLicense& license_;
    //@}
};


#endif // __Application_h_
