// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __InsideUrbanBlockPosition_h_
#define __InsideUrbanBlockPosition_h_

#include "UrbanBlockPosition_ABC.h"

class UrbanObjectWrapper;

// =============================================================================
/** @class  InsideUrbanBlockPosition
    @brief  InsideUrbanBlockPosition
*/
// Created: SLG 2010-04-27
// =============================================================================
class InsideUrbanBlockPosition : public UrbanBlockPosition_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit InsideUrbanBlockPosition( const UrbanObjectWrapper& urbanObject );
    virtual ~InsideUrbanBlockPosition();
    //@}

    //! @name Operations
    //@{
    virtual MT_Vector2D GetFirerPosition( MIL_Agent_ABC& target, urbanLocation::UrbanLocationComputer_ABC::Results& firerResult ) const;
    virtual MT_Vector2D GetTargetPosition( MIL_Agent_ABC& firer, urbanLocation::UrbanLocationComputer_ABC::Results& targetResult ) const;
    virtual double ComputeRatioPionInside( urbanLocation::UrbanLocationComputer_ABC::Results& result, const MT_Ellipse& attritionSurface ) const;
    virtual double ComputeRatioPionInside( urbanLocation::UrbanLocationComputer_ABC::Results& result, const TER_Polygon& polygon, double modificator ) const;
    virtual double ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const;
    //@}

private:
    //! @name Member data
    //@{
    const UrbanObjectWrapper& urbanObject_;
    //@}
};

#endif // __InsideUrbanBlockPosition_h_
