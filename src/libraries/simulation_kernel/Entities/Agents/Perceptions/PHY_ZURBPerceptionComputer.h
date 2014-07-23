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

class MT_Vector2D;
class TER_Polygon;
class MIL_Agent_ABC;
class PHY_PerceptionLevel;

// =============================================================================
/** @class  PHY_ZURBComputer
    @brief  PHY_ZURBComputer
*/
// Created: SLG 2010-04-29
// =============================================================================
class PHY_ZURBPerceptionComputer : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
     PHY_ZURBPerceptionComputer( const MIL_Agent_ABC& perceiver, double roll, unsigned int tick );
    ~PHY_ZURBPerceptionComputer();
    //@}

    //! @name Operations
    //@{
    const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& target ) const;
    //@}

private:
    //! @name Types
    //@{
    struct BestSensorsParameters
    {
        BestSensorsParameters()
            : identificationDist_( 0 )
            , recognitionDist_( 0 )
            , detectionDist_( 0 )
            , delay_( 0 )
        {
            // NOTHING
        }
        double identificationDist_;
        double recognitionDist_;
        double detectionDist_;
        unsigned int delay_;
    };
    //@}

private:
    //! @name Helpers
    //@{
    void ComputePerceptionPolygon( double distance, TER_Polygon& polygon ) const;
    bool ComputeParametersPerception( const MIL_Agent_ABC& target, BestSensorsParameters& parameters ) const;
    const PHY_PerceptionLevel& GetLevelWithDelay( unsigned int delay, const PHY_PerceptionLevel& level ) const;
    //@}

private:
    //! @name Member data
    //@{
    const MIL_Agent_ABC& perceiver_;
    double roll_;
    unsigned int tick_;
    //@}
};

#endif // __PHY_ZURBPerceptionComputer_h_
