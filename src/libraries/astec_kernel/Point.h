// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Point_h_
#define __Point_h_

#include "Location_ABC.h"

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

    //! @name Construction
    //@{
    virtual void PopPoint();
    virtual void AddPoint( const geometry::Point2f& point );
    virtual bool IsValid() const;
    virtual bool IsDone() const;
    //@}

    //! @name Operations
    //@{
    virtual void Accept( LocationVisitor_ABC& ) const;
    virtual void Draw( const GlTools_ABC& tools ) const;
    virtual std::string GetName() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Point( const Point& );            //!< Copy constructor
    Point& operator=( const Point& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    bool pointSet_;
    geometry::Point2f point_;
    //@}
};

#endif // __Point_h_
