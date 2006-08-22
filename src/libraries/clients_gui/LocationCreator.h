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
                      , public kernel::Observer_ABC
                      , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LocationCreator( QWidget* parent, const std::string& menu, ParametersLayer& layer, ShapeHandler_ABC& handler );
             LocationCreator( QWidget* parent, ParametersLayer& layer, ShapeHandler_ABC& handler );
    virtual ~LocationCreator();
    //@}

    //! @name Operations
    //@{
    void Allow( bool point, bool line, bool polygon, bool circle ); // $$$$ AGE 2006-08-09: pas terrible
    //@}

public slots:
    //! @name Slots
    //@{
    void StartPoint();
    void StartLine();
    void StartPolygon();
    void StartCircle();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const geometry::Point2f&, kernel::ContextMenu& );
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
    
    std::string menu_;
    QPopupMenu* pPopupMenu_;
    geometry::Point2f popupPoint_;
    //@}
};

}

#endif // __LocationCreator_h_
