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
#include <geometry/Types.h>

namespace urban
{
    class TerrainObject_ABC;
}

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
             InsideUrbanBlockPosition( const urban::TerrainObject_ABC* urbanObject );
    virtual ~InsideUrbanBlockPosition();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f   GetFirerPosition( MIL_Agent_ABC& target, urbanLocation::UrbanLocationComputer_ABC::Results& firerResult ) const;
    virtual geometry::Point2f   GetTargetPosition( MIL_Agent_ABC& firer, urbanLocation::UrbanLocationComputer_ABC::Results& targetResult ) const;
    virtual float               ComputeRatioPionInside( urbanLocation::UrbanLocationComputer_ABC::Results& result, const MT_Ellipse& attritionSurface ) const;
    virtual float               ComputeRatioPionInside( urbanLocation::UrbanLocationComputer_ABC::Results& result, const geometry::Polygon2f& polygon, float modificator ) const;
    virtual double            ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InsideUrbanBlockPosition( const InsideUrbanBlockPosition& );            //!< Copy constructor
    InsideUrbanBlockPosition& operator=( const InsideUrbanBlockPosition& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    geometry::Point2f GetNearestUrbanBlockPoint( const geometry::Point2f pionPosition, const std::vector< geometry::Point2f > points ) const;
    geometry::Point2f GetFurthestUrbanBlockPoint( const geometry::Point2f pionPosition, const std::vector< geometry::Point2f > points ) const;
    //@}

private:
    //! @name Member data
    //@{
    const urban::TerrainObject_ABC* urbanObject_;
    //@}
};

#endif // __InsideUrbanBlockPosition_h_
