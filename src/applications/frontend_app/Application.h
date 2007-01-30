// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Application_h_
#define __Application_h_

#include <qapplication.h>

namespace kernel
{
    class Controllers;
}

namespace frontend
{
    class Model;
    class Networker;
    class Profile;
}

class QMainWindow;

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: SBO 2007-01-26
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

private slots:
    //! @name Operations
    //@{
    void UpdateData();
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
    std::auto_ptr< kernel::Controllers > controllers_;
    std::auto_ptr< frontend::Model >     model_;
    std::auto_ptr< frontend::Profile >   profile_;
    std::auto_ptr< frontend::Networker > networker_;
    QTimer*                              networkTimer_;
    QMainWindow*                         mainWindow_;
    //@}
};

#endif // __Application_h_
