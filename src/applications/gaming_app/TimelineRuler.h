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
             TimelineRuler( QWidget* parent, kernel::Controllers& controllers, const Simulation& simulation, const unsigned int height );
    virtual ~TimelineRuler();
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
    QString Day( unsigned int dayOffset ) const;
    QString Time( unsigned int hourOffset ) const;
    //@}

private:
    //! @name Statics
    //@{
    static const unsigned int tickStep_;
    //@}

    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Simulation&    simulation_;
    unsigned int         startX_;
    const unsigned short tickHeight_;
    //@}
};

#endif // __TimelineRuler_h_
