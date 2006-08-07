// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StatusBar_h_
#define __StatusBar_h_

#include "astec_gaming/ElementObserver_ABC.h"
#include "astec_gaming/Simulation.h"

class DetectionMap;
class CoordinateConverter_ABC;
class Controllers;

// =============================================================================
/** @class  StatusBar
    @brief  Status bar
*/
// Created: SBO 2006-04-14
// =============================================================================
class StatusBar : public QObject
                , public Observer_ABC
                , public ElementObserver_ABC< Simulation >
                , public ElementObserver_ABC< Simulation::sStartTick >
                , public ElementObserver_ABC< Simulation::sEndTick >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             StatusBar( QStatusBar* bar, const DetectionMap& detection, const CoordinateConverter_ABC& converter, Controllers& controllers );
    virtual ~StatusBar();
    //@}

public slots:
    //! @name Slots
    //@{
    void OnMouseMove( const geometry::Point2f& position );
    void OnMouseMove( const geometry::Point3f& position );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnLag();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    StatusBar( const StatusBar& );            //!< Copy constructor
    StatusBar& operator=( const StatusBar& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void NotifyUpdated( const Simulation::sStartTick& startTick );
    virtual void NotifyUpdated( const Simulation::sEndTick& endTick );
    //@}

private:
    //! @name Member data
    //@{
    const DetectionMap& detection_;
    const CoordinateConverter_ABC& converter_;
    bool lastSimulationStatus_;

    QLabel* pPositionXYZ_;
    QLabel* pPositionMgrs_;
    QLabel* pPositionLatLong_;
    QLabel* pTime_;
    QLabel* pTick_;
    QTimer* pLagTimer_;
    //@}
};

#endif // __StatusBar_h_
