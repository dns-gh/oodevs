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

#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

class Simulation;

// =============================================================================
/** @class  TimelineRuler
    @brief  TimelineRuler
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelineRuler : public QWidget
                    , public kernel::Observer_ABC
                    , public kernel::ElementObserver_ABC< Simulation >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TimelineRuler( QWidget* parent, kernel::Controllers& controllers, const unsigned int height );
    virtual ~TimelineRuler();
    //@}

    //! @name Operations
    //@{
    unsigned long ConvertToPosition( const QDateTime& datetime ) const;
    long ConvertToPixels( long secs ) const;
    long ConvertToSeconds( long pixels ) const;
    void ZoomIn();
    void ZoomOut();
    //@}

private slots:
    //! @name Slots
    //@{
    void SetContentsPos( int x, int y );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineRuler( const TimelineRuler& );            //!< Copy constructor
    TimelineRuler& operator=( const TimelineRuler& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void paintEvent( QPaintEvent* event );
    void DrawTimeline( QPainter& painter ) const;
    void DrawDates( QPainter& painter ) const;
    void DrawTimes( QPainter& painter ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QDateTime            initialDateTime_;
    const unsigned short tickHeight_;
    unsigned int         tickStep_;
    unsigned int         startX_;
    //@}
};

#endif // __TimelineRuler_h_
