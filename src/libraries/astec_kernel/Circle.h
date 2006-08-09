// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Circle_h_
#define __Circle_h_

#include "Location_ABC.h"

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
    Circle( const Circle& );            //!< Copy constructor
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

#endif // __Circle_h_
