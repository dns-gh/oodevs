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
#include <boost/noncopyable.hpp>

class MIL_Agent_ABC;
class PHY_DotationCategory;
class MT_Ellipse;
class TER_Polygon;

// =============================================================================
/** @class  UrbanBlockPosition_ABC
    @brief  UrbanBlockPosition_ABC
*/
// Created: SLG 2010-04-27
// =============================================================================
class UrbanBlockPosition_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanBlockPosition_ABC(){}
    virtual ~UrbanBlockPosition_ABC(){}
    //@}

    //! @name Operations
    //@{
    virtual MT_Vector2D GetFirerPosition( MIL_Agent_ABC& target, urbanLocation::UrbanLocationComputer_ABC::Results& firerResult ) const = 0;
    virtual MT_Vector2D GetTargetPosition( MIL_Agent_ABC& firer, urbanLocation::UrbanLocationComputer_ABC::Results& targetResult ) const = 0;
    virtual double ComputeRatioPionInside( urbanLocation::UrbanLocationComputer_ABC::Results& result, const MT_Ellipse& attritionSurface ) const = 0;
    virtual double ComputeRatioPionInside( urbanLocation::UrbanLocationComputer_ABC::Results& result, const TER_Polygon& polygon, double modificator ) const = 0;
    virtual double ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const = 0;
    //@}
};

#endif // __UrbanBlockPosition_ABC_h_
