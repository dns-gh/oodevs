// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Polygon_h_
#define __Polygon_h_

#include "Zone_ABC.h"
#include <geometry/Types.h>

// =============================================================================
/** @class  Polygon
    @brief  Polygon
*/
// Created: AGE 2007-10-09
// =============================================================================
class Polygon : public Zone_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Polygon( const std::vector< Position >& positions );
    virtual ~Polygon();
    //@}

    //! @name Operations
    //@{
    virtual bool Contains( const Position& position ) const;
    //@}

private:
    //! @name Member data
    //@{
    geometry::Polygon2f polygon_;
    //@}
};

#endif // __Polygon_h_
