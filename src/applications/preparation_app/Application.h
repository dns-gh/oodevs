// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
             Application( int argc, char** argv );
    virtual ~Application();
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
    //@}
};


#endif // __Application_h_
