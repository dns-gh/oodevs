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
    //@}
};

#endif // __AlgorithmsFactories_h_
