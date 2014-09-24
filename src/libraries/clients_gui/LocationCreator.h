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
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    struct Nothing;
    class Location_ABC;
    class Drawing_ABC;
}

namespace gui
{
    class ParametersLayer;
    class ShapeHandler_ABC;

// =============================================================================
/** @class  LocationCreator
    @brief  Location creator
*/
// Created: APE 2004-05-06
// =============================================================================
class LocationCreator : public QObject
                      , public tools::Observer_ABC
                      , public kernel::ContextMenuObserver_ABC< kernel::Nothing >
                      , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
                      , public kernel::ContextMenuObserver_ABC< kernel::Drawing_ABC >
                      , public tools::ElementObserver_ABC     < kernel::Drawing_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LocationCreator( QObject* parent,
                              const QString& menu,
                              const std::shared_ptr< ParametersLayer >& layer,
                              ShapeHandler_ABC& handler );
             LocationCreator( QObject* parent,
                              const std::shared_ptr< ParametersLayer >& layer,
                              ShapeHandler_ABC& handler );
    virtual ~LocationCreator();
    //@}

    //! @name Operations
    //@{
    bool Allows( const kernel::Location_ABC& location ) const;
    void Allow( bool point, bool line, bool polygon, bool circle, bool rectangle );
    //@}

public slots:
    //! @name Slots
    //@{
    void StartPoint();
    void StartLine();
    void StartRectangle();
    void StartPolygon();
    void StartCircle();
    void AddDrawing();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Nothing&, kernel::ContextMenu& );
    virtual void NotifyContextMenu( const geometry::Point2f&, kernel::ContextMenu& );
    virtual void NotifyContextMenu( const kernel::Drawing_ABC&, kernel::ContextMenu& );
    virtual void NotifyDeleted( const kernel::Drawing_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    std::shared_ptr< ParametersLayer > layer_;
    ShapeHandler_ABC& handler_;

    QString menu_;
    geometry::Point2f popupPoint_;

    const kernel::Drawing_ABC* drawing_;

    bool pointAllowed_;
    bool lineAllowed_;
    bool polygonAllowed_;
    bool circleAllowed_;
    bool rectangleAllowed_;
    //@}
};

}

#endif // __LocationCreator_h_
