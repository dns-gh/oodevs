// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __AlgorithmsFactories_h_
#define __AlgorithmsFactories_h_

namespace firing
{
    class WeaponAvailabilityComputerFactory_ABC;
    class ComposantesAbleToBeFiredComputerFactory_ABC;
}
namespace location
{
    class LocationComputerFactory_ABC;
}
namespace posture
{
    class PostureComputerFactory_ABC;
}
namespace dotation
{
    class ConsumptionComputerFactory_ABC;
}
namespace moving
{
    class MoveComputerFactory_ABC;
}
namespace detection
{
    class DetectionComputerFactory_ABC;
}
namespace human
{
    class HealComputerFactory_ABC;
}
namespace transport
{
	class TransportCapacityComputerFactory_ABC;
}
// =============================================================================
/** @class  AlgorithmsFactories
    @brief  AlgorithmsFactories
*/
// Created: MGD 2009-08-13
// =============================================================================
class AlgorithmsFactories : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             AlgorithmsFactories();
    virtual ~AlgorithmsFactories();
    //@}

    //! @name Attribute
    //@{
    std::auto_ptr< firing::WeaponAvailabilityComputerFactory_ABC > weaponAvailabilityComputerFactory_;
    std::auto_ptr< firing::ComposantesAbleToBeFiredComputerFactory_ABC > composantesAbleToBeFiredComputerFactory_;
    std::auto_ptr< posture::PostureComputerFactory_ABC > postureComputerFactory_;
    std::auto_ptr< location::LocationComputerFactory_ABC > locationComputerFactory_;
    std::auto_ptr< dotation::ConsumptionComputerFactory_ABC > consumptionComputerFactory_;
    std::auto_ptr< moving::MoveComputerFactory_ABC > moveComputerFactory_;
    std::auto_ptr< detection::DetectionComputerFactory_ABC > detectionComputerFactory_;
    std::auto_ptr< transport::TransportCapacityComputerFactory_ABC > transportCapacityComputerFactory_;
    std::auto_ptr< human::HealComputerFactory_ABC > healComputerFactory_;
    //@}
};

#endif // __AlgorithmsFactories_h_
