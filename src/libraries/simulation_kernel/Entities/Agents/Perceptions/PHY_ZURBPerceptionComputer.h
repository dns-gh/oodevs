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
#include "geometry/Types.h"

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

    struct Distances
    {
        Distances():identificationDist( 0 ), recognitionDist( 0 ), detectionDist( 0 ){}
        double identificationDist;
        double recognitionDist;
        double detectionDist;
        
    };

    struct Polygons
    {
        geometry::Polygon2f identificationPolygon;
        geometry::Polygon2f recognitionPolygon;
        geometry::Polygon2f detectionPolygon;
    };
    struct BestSensorsParameters
    {
        Distances distances;
        unsigned int delay;
    };


public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_ZURBPerceptionComputer( const MIL_Agent_ABC& perceiver, float roll, int tick );
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
    const Polygons              ComputePerceptionPolygon( const MIL_Agent_ABC& target, BestSensorsParameters& parameters ) const;
    const BestSensorsParameters ComputeParametersPerception( const MIL_Agent_ABC& target ) const;
    const PHY_PerceptionLevel&  GetLevelWithDelay( unsigned int delay, const PHY_PerceptionLevel& level ) const;
    geometry::Polygon2f         MakePolygon( const urban::TerrainObject_ABC& block, double distance ) const;
    geometry::Polygon2f         MakePolygon( const geometry::Point2f targetPosition, double distance ) const;
    //@}

private:
    //! @name Member data
    //@{
    float roll_;
    unsigned int tick_;
    //@}

private:
    static MT_Random randomGenerator_;
};

#endif // __PHY_ZURBPerceptionComputer_h_
