// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MainWindow_h_
#define __MainWindow_h_

#include <qmainwindow.h>

class QWidgetStack;

namespace gui 
{
    class LinkInterpreter_ABC ; 
}

namespace tools
{
    class GeneralConfig;
}

namespace kernel
{
    class Controllers ; 
}


class Session ; 

// =============================================================================
/** @class  MainWindow
    @brief  MainWindow
*/
// Created: SBO 2008-02-21
// =============================================================================
class MainWindow : public QMainWindow
{
   
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    MainWindow( kernel::Controllers& controllers, boost::shared_ptr< Session > sessionStatus );
    virtual ~MainWindow();
    //@}

public slots:

    void Maximize(); 

private:
    //! @name Copy/Assignment
    //@{
    MainWindow( const MainWindow& );            //!< Copy constructor
    MainWindow& operator=( const MainWindow& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SetStyle();
    void CenterWindow();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< tools::GeneralConfig >       config_;
    std::auto_ptr< gui::LinkInterpreter_ABC >   interpreter_; 
    QWidgetStack* pages_;
    //@}
};

#endif // __MainWindow_h_
