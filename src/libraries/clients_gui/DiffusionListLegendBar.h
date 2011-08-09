// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef gui_DiffusionListLegendBar_h_
#define gui_DiffusionListLegendBar_h_

#include <boost/noncopyable.hpp>

#pragma warning( push, 0 )
#include <QtGui/qcolor.h>
#pragma warning( pop )

class QWidget;
class QPaintEvent;

namespace gui
{

// =============================================================================
/** @class  DiffusionListLegendBar
    @brief  DiffusionListLegendBar
*/
// Created: FPT 2011-06-21
// =============================================================================

class DiffusionListLegendBar : public QWidget
                             , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
            DiffusionListLegendBar( QWidget* parent, const QColor& color, int size );
    virtual ~DiffusionListLegendBar();
    //@}
private:
    //! @name Operations
    //@{
    virtual void paintEvent( QPaintEvent* );
    //@}

private:
    //! @name Member data
    //@{
    int            size_;
    QColor         color_;
    //@}
};

}

#endif // gui_DiffusionListLegendBar_
