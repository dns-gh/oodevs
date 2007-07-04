// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelineRuler_h_
#define __TimelineRuler_h_

#include <qcanvas.h>

// =============================================================================
/** @class  TimelineRuler
    @brief  TimelineRuler
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelineRuler : public QCanvasRectangle
{

public:
    //! @name Constructors/Destructor
    //@{
             TimelineRuler( QCanvas* canvas, QCanvasView* view );
    virtual ~TimelineRuler();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineRuler( const TimelineRuler& );            //!< Copy constructor
    TimelineRuler& operator=( const TimelineRuler& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void draw( QPainter& painter );
    void DrawTimeline( QPainter& painter );
    void DrawGrid( QPainter& painter );
    //@}

private:
    //! @name Member data
    //@{
    QCanvasView& view_;
    unsigned int tickStep_;
    unsigned int pageStep_;
    //@}
};

#endif // __TimelineRuler_h_
