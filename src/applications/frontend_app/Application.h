// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Application_h_
#define __Application_h_

#include <qapplication.h>

class QMainWindow;

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: SBO 2007-01-26
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
    //! @name Copy/Assignment
    //@{
    Application( const Application& );            //!< Copy constructor
    Application& operator=( const Application& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    QMainWindow* mainWindow_;
    //@}
};

#endif // __Application_h_
