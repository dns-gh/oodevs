// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __OutsideUrbanBlockPosition_h_
#define __OutsideUrbanBlockPosition_h_

#include <geometry/Types.h>
#include "UrbanBlockPosition_ABC.h"

// =============================================================================
/** @class  OutsideUrbanBlockPosition
    @brief  OutsideUrbanBlockPosition
*/
// Created: SLG 2010-04-27
// =============================================================================
class OutsideUrbanBlockPosition : public UrbanBlockPosition_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             OutsideUrbanBlockPosition();
    virtual ~OutsideUrbanBlockPosition();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f   GetFirerPosition( MIL_Agent_ABC& target, urbanLocation::UrbanLocationComputer_ABC::Results& firerResult ) const;
    virtual geometry::Point2f   GetTargetPosition( MIL_Agent_ABC& firer, urbanLocation::UrbanLocationComputer_ABC::Results& targetResult ) const;
    virtual float               ComputeRatioPionInside( urbanLocation::UrbanLocationComputer_ABC::Results& result, const MT_Ellipse& attritionSurface ) const;
    virtual MT_Float            ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OutsideUrbanBlockPosition( const OutsideUrbanBlockPosition& );            //!< Copy constructor
    OutsideUrbanBlockPosition& operator=( const OutsideUrbanBlockPosition& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __OutsideUrbanBlockPosition_h_
