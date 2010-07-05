// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_Point_h_
#define __kernel_Point_h_

#include "Location_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  Point
    @brief  Point
*/
// Created: AGE 2006-08-09
// =============================================================================
class Point : public Location_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Point();
    virtual ~Point();
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
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Point& operator=( const Point& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    bool pointSet_;
    geometry::Point2f point_;
    //@}
};

}

#endif // __kernel_Point_h_
