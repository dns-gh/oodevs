// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_ABC_h_
#define __Formation_ABC_h_

#include <geometry/Types.h>

namespace tic
{
    class Movable_ABC;

// =============================================================================
/** @class  Formation_ABC
    @brief  Formation definition
*/
// Created: AGE 2008-04-01
// =============================================================================
class Formation_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation_ABC() {};
    virtual ~Formation_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Start( const geometry::Point2f& at, const geometry::Vector2f& towards, std::size_t count ) = 0;
    virtual void Apply( Movable_ABC* movable ) = 0;
    //@}

public:
    //! @name Helpers
    //@{
    static bool Compare( const geometry::Point2f& lhs, const geometry::Point2f& rhs, const geometry::Point2f& center, const geometry::Vector2f& towards );
    static void Sort( std::vector< geometry::Point2f >& points, const geometry::Point2f& center, const geometry::Vector2f& towards );
    //@}
};
}

#endif // __Formation_ABC_h_
