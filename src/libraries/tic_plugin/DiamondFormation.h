// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DiamondFormation_h_
#define __DiamondFormation_h_

#include "Formation_ABC.h"
#include <vector>

namespace plugins
{
namespace tic
{

// =============================================================================
/** @class  DiamondFormation
    @brief  Diamond formation
*/
// Created: AGE 2008-04-01
// =============================================================================
class DiamondFormation : public Formation_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DiamondFormation();
    virtual ~DiamondFormation();
    //@}

    //! @name Operations
    //@{
    virtual void Start( const geometry::Point2f& at, const geometry::Vector2f& towards, unsigned count );
    virtual void Apply( Movable_ABC* movable );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DiamondFormation( const DiamondFormation& );            //!< Copy constructor
    DiamondFormation& operator=( const DiamondFormation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddPlatform( const geometry::Point2f& center, float baseAngle, unsigned& level, unsigned& platformCount );
    //@}

private:
    //! @name Member data
    //@{
    std::vector< geometry::Point2f > points_;
    //@}
};

}
}

#endif // __DiamondFormation_h_
