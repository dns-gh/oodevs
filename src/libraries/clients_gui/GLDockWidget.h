// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef GLDockWidget_h
#define GLDockWidget_h

#include "RichDockWidget.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class GLStackedWidget;

// =============================================================================
/** @class  GLDockWidget
    @brief  GLDockWidget
*/
// Created: ABR 2014-06-12
// =============================================================================
class GLDockWidget : public RichDockWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             GLDockWidget( kernel::Controllers& controllers,
                           QMainWindow& parent,
                           const std::shared_ptr< GLStackedWidget >& view );
    virtual ~GLDockWidget();
    //@}

    //! @name Operations
    //@{
    std::shared_ptr< GLStackedWidget > GetStackedWidget() const;
    QAction* ToggleViewAction() const;
    //@}

signals:
    //! @name Signals
    //@{
    void OnClose( const QWidget& );
    //@} 

private:
    //! @name Helpers
    //@{
    virtual void closeEvent( QCloseEvent *event );
    //@}

private:
    //! @name Member data
    //@{
    QMainWindow& parent_;
    std::shared_ptr< GLStackedWidget > stackedWidget_;
    std::unique_ptr< QAction > toggleViewAction_;
    //@}
};

}

#endif // GLDockWidget_h
