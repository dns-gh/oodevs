// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Circle_h_
#define __Circle_h_

#include "Location_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  Circle
    @brief  Circle
*/
// Created: AGE 2006-08-09
// =============================================================================
class Circle : public Location_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Circle();
    virtual ~Circle();
    //@}

    //! @name Operations
    //@{
    virtual void PopPoint();
    virtual void AddPoint( const geometry::Point2f& point );
    virtual bool IsValid() const;
    virtual bool IsDone() const;
    virtual Location_ABC& Clone() const;
    //@}

    //! @name Operations
    //@{
    virtual void Accept( LocationVisitor_ABC& ) const;
    virtual void Draw( const GlTools_ABC& tools ) const;
    virtual QString GetName() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Circle& operator=( const Circle& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point2f center_;
    bool centerSet_;
    float radius_;
    //@}
};

}

#endif // __Circle_h_
