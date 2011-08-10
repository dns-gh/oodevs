// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TabWidget_h_
#define __TabWidget_h_

#include "TimelinePanel.h"

#pragma warning( push, 0 )
#include <QtGui/qtabwidget.h>
#pragma warning( pop )

class Q3VBox;

// =============================================================================
/** @class  TabWidget
    @brief  TabWidget
*/
// Created: FPT 2011-08-08
// =============================================================================
class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
        explicit TabWidget( Q3VBox* box );
    //@}

public slots:
     //! @name Signal
    //@{
        void OnViewChanged();
    //@}

public:
    //! @name Operations
    //@{
        void setConnect( TimelineWidget* timeline, TimelinePanel::T_Filters& filters, ActionsToolbar* toolbar);
    //@}

private:
    //! @name Member data
    //@{
        TimelinePanel::T_Filters* filters_;
        ActionsToolbar* toolbar_;
        TimelineWidget* timeline_;
    //@}
};

#endif // __TabWidget_h_