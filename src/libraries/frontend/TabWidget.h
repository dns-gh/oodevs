// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __frontend_TabWidget_h_
#define __frontend_TabWidget_h_

#include <boost/noncopyable.hpp>
#include <QtGui/qtabwidget.h>
#include <QtGui/qtabbar.h>

namespace frontend
{

// =============================================================================
/** @class  TabWidget
    @brief  TabWidget
*/
// Created: ABR 2011-11-03
// =============================================================================
class TabWidget : public QTabWidget
{
public:
    //! @name Constructors/Destructor
    //@{
    TabWidget( QWidget* parent )
        : QTabWidget( parent )
    {
        setBackgroundOrigin( QWidget::WindowOrigin );
        tabBar()->setBackgroundOrigin( QWidget::WindowOrigin );
        setMargin( 0 );
    }
    virtual ~TabWidget() {}
    //@}
};

}

#endif // __frontend_TabWidget_h_
