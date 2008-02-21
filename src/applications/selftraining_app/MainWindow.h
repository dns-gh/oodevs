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

namespace tools
{
    class GeneralConfig;
}

// =============================================================================
/** @class  MainWindow
    @brief  MainWindow
*/
// Created: SBO 2008-02-21
// =============================================================================
class MainWindow : public QMainWindow
{

public:
    //! @name Constructors/Destructor
    //@{
             MainWindow();
    virtual ~MainWindow();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MainWindow( const MainWindow& );            //!< Copy constructor
    MainWindow& operator=( const MainWindow& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< tools::GeneralConfig > config_;
    QWidgetStack* pages_;
    //@}
};

#endif // __MainWindow_h_
