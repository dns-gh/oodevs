// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationComparator_h_
#define __LocationComparator_h_

#include "clients_kernel/LocationVisitor_ABC.h"

namespace actions
{
    namespace gui
    {

class LocationComparator : public kernel::LocationVisitor_ABC
{
public:
    explicit LocationComparator( const T_PointVector& points );
    virtual ~LocationComparator();

    //! @name Operations
    //@{
    bool HasMatched() const;
    //@}

    //! @name LocationVisitor_ABC implementations
    //@{
    virtual void VisitLines( const T_PointVector& points );
    virtual void VisitPoint    ( const geometry::Point2f& ) {}
    virtual void VisitRectangle( const T_PointVector& ) {}
    virtual void VisitPolygon  ( const T_PointVector& ) {}
    virtual void VisitCircle   ( const geometry::Point2f&, float ) {}
    virtual void VisitCurve    ( const T_PointVector& ) {}
    virtual void VisitPath     ( const geometry::Point2f&, const T_PointVector& ) {}
    virtual void VisitText     ( const QString&, const QFont&, const geometry::Point2f& ) {}
    //@}

private:
    //! @name Member data
    //@{
    const T_PointVector& points_;
    bool hasMatched_;
    //@}
};

    } //! namespace gui
} //! namespace actions

#endif // __LocationComparator_h_
