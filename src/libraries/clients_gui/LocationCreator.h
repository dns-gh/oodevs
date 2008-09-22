// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationCreator_h_
#define __LocationCreator_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    struct Nothing;
    class Location_ABC;
}

namespace gui
{
    class ParametersLayer;
    class ShapeHandler_ABC;
    class Drawing_ABC;

// =============================================================================
/** @class  LocationCreator
    @brief  Location creator
*/
// Created: APE 2004-05-06
// =============================================================================
class LocationCreator : public QObject
                      , public kernel::Observer_ABC
                      , public kernel::ContextMenuObserver_ABC< kernel::Nothing >
                      , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
                      , public kernel::ContextMenuObserver_ABC< Drawing_ABC >
                      , public kernel::ElementObserver_ABC    < Drawing_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LocationCreator( QObject* parent, const QString& menu, ParametersLayer& layer, ShapeHandler_ABC& handler );
             LocationCreator( QObject* parent, ParametersLayer& layer, ShapeHandler_ABC& handler );
    virtual ~LocationCreator();
    //@}

    //! @name Operations
    //@{
    bool Allows( const kernel::Location_ABC& location ) const;
    void Allow( bool point, bool line, bool polygon, bool circle );
    //@}

public slots:
    //! @name Slots
    //@{
    void StartPoint();
    void StartLine();
    void StartPolygon();
    void StartCircle();
    void AddDrawing();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Nothing&, kernel::ContextMenu& );
    virtual void NotifyContextMenu( const geometry::Point2f&, kernel::ContextMenu& );
    virtual void NotifyContextMenu( const Drawing_ABC&, kernel::ContextMenu& );
    virtual void NotifyDeleted( const Drawing_ABC& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LocationCreator( const LocationCreator& );
    LocationCreator& operator=( const LocationCreator& );
    //@}

private:
    //! @name Member data
    //@{
    ParametersLayer& layer_;
    ShapeHandler_ABC& handler_;
    
    QString menu_;
    geometry::Point2f popupPoint_;

    const Drawing_ABC* drawing_;

    bool pointAllowed_;
    bool lineAllowed_;
    bool polygonAllowed_;
    bool circleAllowed_;
    //@}
};

}

#endif // __LocationCreator_h_
