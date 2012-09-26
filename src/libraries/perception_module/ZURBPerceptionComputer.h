// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_ZURB_PERCEPTION_COMPUTER_H
#define SWORD_PERCEPTION_ZURB_PERCEPTION_COMPUTER_H

namespace sword
{
namespace wrapper
{
    class View;
}
namespace perception
{
    class PerceptionLevel;
// =============================================================================
/** @class  ZURBComputer
    @brief  ZURB Computer
*/
// Created: SLG 2010-04-29
// =============================================================================
class ZURBPerceptionComputer
{
public:
    //! @name Constructors/Destructor
    //@{
             ZURBPerceptionComputer( double roll, unsigned int tick );
    virtual ~ZURBPerceptionComputer();
    //@}

    //! @name Operations
    //@{
    const PerceptionLevel& ComputePerception( const wrapper::View& model, const wrapper::View& perceiver, const wrapper::View& target ) const;
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
    bool ComputeParametersPerception( const wrapper::View& model, const wrapper::View& perceiver, const wrapper::View& target, BestSensorsParameters& parameters ) const;
    const PerceptionLevel& GetLevelWithDelay( unsigned int delay, const PerceptionLevel& level ) const;
    //@}
private:
    //! @name Member data
    //@{
    double roll_;
    unsigned int tick_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_ZURB_PERCEPTION_COMPUTER_H
