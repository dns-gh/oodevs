// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Polygon_h_
#define __Polygon_h_

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
    Polygon( const Polygon& );            //!< Copy constructor
    Polygon& operator=( const Polygon& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    T_PointVector points_;
    //@}
};

}

#endif // __Polygon_h_
