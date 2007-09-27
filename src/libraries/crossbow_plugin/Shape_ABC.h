// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Shape_ABC_h_
#define __Shape_ABC_h_

namespace crossbow
{
    class Row_ABC;
    class Line;
    class Point;

    class ShapeVisitor_ABC
    {
    public:
                 ShapeVisitor_ABC() {}
        virtual ~ShapeVisitor_ABC() {}

        virtual void Visit( const Line& ) {}
        virtual void Visit( const Point& ) {}

    };

// =============================================================================
/** @class  Shape_ABC
    @brief  Shape_ABC
*/
// Created: SBO 2007-08-30
// =============================================================================
class Shape_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Shape_ABC() {}
    virtual ~Shape_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Accept( ShapeVisitor_ABC& visitor ) const = 0;
    //@}
};

}

#endif // __Shape_ABC_h_
