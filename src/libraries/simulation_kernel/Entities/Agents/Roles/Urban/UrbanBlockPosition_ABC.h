// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __UrbanBlockPosition_ABC_h_
#define __UrbanBlockPosition_ABC_h_

#include "UrbanLocationComputer_ABC.h"

class MIL_Agent_ABC;
class PHY_DotationCategory;

// =============================================================================
/** @class  UrbanBlockPosition_ABC
    @brief  UrbanBlockPosition_ABC
*/
// Created: SLG 2010-04-27
// =============================================================================
class UrbanBlockPosition_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanBlockPosition_ABC(){}
    virtual ~UrbanBlockPosition_ABC(){}
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f   GetFirerPosition( MIL_Agent_ABC& target, urbanLocation::UrbanLocationComputer_ABC::Results& firerResult ) const = 0;
    virtual geometry::Point2f   GetTargetPosition( MIL_Agent_ABC& firer, urbanLocation::UrbanLocationComputer_ABC::Results& targetResult ) const = 0;
    virtual float               ComputeRatioPionInside( urbanLocation::UrbanLocationComputer_ABC::Results& result, const MT_Ellipse& attritionSurface ) const = 0;
    virtual float               ComputeRatioPionInside( urbanLocation::UrbanLocationComputer_ABC::Results& result, const geometry::Polygon2f& polygon, float /*modificator*/ ) const = 0;
    virtual MT_Float            ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanBlockPosition_ABC( const UrbanBlockPosition_ABC& );            //!< Copy constructor
    UrbanBlockPosition_ABC& operator=( const UrbanBlockPosition_ABC& ); //!< Assignment operator
    //@}
};

#endif // __UrbanBlockPosition_ABC_h_
