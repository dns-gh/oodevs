// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SimpleLocationDrawer_h_
#define __SimpleLocationDrawer_h_

#include "LocationVisitor_ABC.h"

namespace kernel
{
    class GlTools_ABC;

// =============================================================================
/** @class  SimpleLocationDrawer
    @brief  SimpleLocationDrawer
*/
// Created: SBO 2008-05-30
// =============================================================================
class SimpleLocationDrawer : public LocationVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SimpleLocationDrawer( const GlTools_ABC& tools );
    virtual ~SimpleLocationDrawer();
    //@}

    //! @name Operations
    //@{
    virtual void VisitLines  ( const T_PointVector& points );
    virtual void VisitPolygon( const T_PointVector& points );
    virtual void VisitCircle ( const geometry::Point2f& center, float radius );
    virtual void VisitPoint  ( const geometry::Point2f& point );
    virtual void VisitPath   ( const geometry::Point2f& first, const T_PointVector& points );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimpleLocationDrawer( const SimpleLocationDrawer& );            //!< Copy constructor
    SimpleLocationDrawer& operator=( const SimpleLocationDrawer& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const GlTools_ABC& tools_;
    //@}
};

}

#endif // __SimpleLocationDrawer_h_
