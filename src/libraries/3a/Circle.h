// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Circle_h_
#define __Circle_h_

#include "Zone_ABC.h"
#include "Position.h"

// =============================================================================
/** @class  Circle
    @brief  Circle
*/
// Created: AGE 2007-10-09
// =============================================================================
class Circle : public Zone_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Circle( const std::vector< Position >& positions );
    virtual ~Circle();
    //@}

    //! @name Operations
    //@{
    virtual bool Contains( const Position& position ) const;
    //@}

private:
    //! @name Member data
    //@{
    const Position center_;
    float radius_;
    //@}
};

#endif // __Circle_h_
