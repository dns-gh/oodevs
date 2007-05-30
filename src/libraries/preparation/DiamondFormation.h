// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DiamondFormation_h_
#define __DiamondFormation_h_

// =============================================================================
/** @class  DiamondFormation
    @brief  DiamondFormation
*/
// Created: AGE 2007-05-29
// =============================================================================
class DiamondFormation
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DiamondFormation( const geometry::Point2f& center, float spacing = 500.f );
    virtual ~DiamondFormation();
    //@}

    //! @name Operations
    //@{
    geometry::Point2f NextPosition();
    //@}

private:
    //! @name Member data
    //@{
    geometry::Vector2f center_;
    float spacing_;
    float angle_, radius_;
    unsigned int onLevel_, totalOnLevel_;
    //@}
};

#endif // __DiamondFormation_h_
