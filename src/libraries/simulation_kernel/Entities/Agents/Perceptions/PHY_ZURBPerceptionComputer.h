// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_ZURBPerceptionComputer_h_
#define __PHY_ZURBPerceptionComputer_h_

#include "PHY_PerceptionComputer_ABC.h"
#include <geometry/Types.h>

namespace urban
{
    class TerrainObject_ABC;
}

// =============================================================================
/** @class  PHY_ZURBComputer
    @brief  PHY_ZURBComputer
*/
// Created: SLG 2010-04-29
// =============================================================================
class PHY_ZURBPerceptionComputer : public PHY_PerceptionComputer_ABC
{
public:
    //! @name Types
    //@{
    struct Distances
    {
        Distances() : identificationDist_( 0 ), recognitionDist_( 0 ), detectionDist_( 0 ) {}
        double identificationDist_;
        double recognitionDist_;
        double detectionDist_;

    };
    struct Polygons
    {
        geometry::Polygon2f identificationPolygon_;
        geometry::Polygon2f recognitionPolygon_;
        geometry::Polygon2f detectionPolygon_;
    };
    struct BestSensorsParameters
    {
        BestSensorsParameters() : delay_( 0 ) {}
        Distances distances_;
        unsigned int delay_;
    };
//@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_ZURBPerceptionComputer( const MIL_Agent_ABC& perceiver, float roll, unsigned int tick );
    virtual ~PHY_ZURBPerceptionComputer();
    //@}

    //! @name Operations
    //@{
    virtual const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& target ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_ZURBPerceptionComputer( const PHY_ZURBPerceptionComputer& );            //!< Copy constructor
    PHY_ZURBPerceptionComputer& operator=( const PHY_ZURBPerceptionComputer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ComputePerceptionPolygon( const MIL_Agent_ABC& target, BestSensorsParameters& parameters, Polygons& polygons ) const;
    void ComputeParametersPerception( const MIL_Agent_ABC& target, BestSensorsParameters& parameters ) const;
    const PHY_PerceptionLevel& GetLevelWithDelay( unsigned int delay, const PHY_PerceptionLevel& level ) const;
    void MakePolygon( geometry::Polygon2f& polygon, const urban::TerrainObject_ABC& block, double distance ) const;
    void MakePolygon( geometry::Polygon2f& polygon, const geometry::Point2f& targetPosition, double distance ) const;
    //@}

private:
    //! @name Member data
    //@{
    float roll_;
    unsigned int tick_;
    //@}
};

#endif // __PHY_ZURBPerceptionComputer_h_
