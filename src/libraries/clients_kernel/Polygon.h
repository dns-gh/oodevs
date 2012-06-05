// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_Polygon_h_
#define __kernel_Polygon_h_

#include "Location_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  Polygon
    @brief  Polygon
*/
// Created: AGE 2006-08-09
// =============================================================================
class Polygon : public Location_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Polygon();
    virtual ~Polygon();
    //@}

    //! @name Operations
    //@{
    virtual void PopPoint();
    virtual void AddPoint( const geometry::Point2f& point );
    virtual void Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision );
    virtual Location_ABC& Clone() const;
    //@}

    //! @name Accessors
    //@{
    virtual void Accept( LocationVisitor_ABC& ) const;
    virtual QString GetName() const;
    virtual bool IsValid() const;
    virtual bool IsDone() const;
    const T_PointVector& GetPoints() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Polygon& operator=( const Polygon& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    bool IsSegment() const;
    //@}

private:
    //! @name Member data
    //@{
    T_PointVector points_;
    //@}
};

}

#endif // __kernel_Polygon_h_
