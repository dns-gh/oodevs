// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MainWindow_h_
#define __MainWindow_h_

#pragma warning(push, 0)
#include <QtGui/qmainwindow.h>
#pragma warning(pop)
#include <string>

class SVGLWidget;

// =============================================================================
/** @class  MainWindow
    @brief  Main window
*/
// Created: AGE 2004-12-23
// =============================================================================
class MainWindow : public QMainWindow
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MainWindow( const std::string& filename );
    virtual ~MainWindow();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MainWindow( const MainWindow& );            //!< Copy constructor
    MainWindow& operator=( const MainWindow& ); //!< Assignement operator
    //@}
};

#endif // __MainWindow_h_
