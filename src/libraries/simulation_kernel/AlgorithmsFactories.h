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

#include <boost/serialization/export.hpp>

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
namespace human
{
    class HealComputerFactory_ABC;
}

class OnComponentFunctorComputerFactory_ABC;

// =============================================================================
/** @class  AlgorithmsFactories
    @brief  Algorithms factories
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

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name CheckPoints
    //@{
    template< typename Archive > friend void save_construct_data( Archive& archive, const AlgorithmsFactories* factories, const unsigned int /*version*/ );
    template< typename Archive > friend void load_construct_data( Archive& archive, AlgorithmsFactories* factories, const unsigned int /*version*/ );
    //@}

public:
    //! @name Attribute
    //@{
    std::auto_ptr< posture::PostureComputerFactory_ABC > postureComputerFactory_;
    std::auto_ptr< location::LocationComputerFactory_ABC > locationComputerFactory_;
    std::auto_ptr< dotation::ConsumptionComputerFactory_ABC > consumptionComputerFactory_;
    std::auto_ptr< moving::MoveComputerFactory_ABC > moveComputerFactory_;
    std::auto_ptr< human::HealComputerFactory_ABC > healComputerFactory_;
    std::auto_ptr< OnComponentFunctorComputerFactory_ABC > onComponentFunctorComputerFactory_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( AlgorithmsFactories )

// -----------------------------------------------------------------------------
// Name: AlgorithmsFactories::save_construct_data
// Created: MGD 2009-09-30
// -----------------------------------------------------------------------------
template< typename Archive >
void save_construct_data( Archive& /*archive*/, const AlgorithmsFactories* /*role*/, const unsigned int /*version*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AlgorithmsFactories::load_construct_data
// Created: MGD 2009-09-30
// -----------------------------------------------------------------------------
template< typename Archive >
void load_construct_data( Archive& /*archive*/, AlgorithmsFactories* factories, const unsigned int /*version*/ )
{
    ::new( factories )AlgorithmsFactories();
}

// -----------------------------------------------------------------------------
// Name: AlgorithmsFactories::serialize
// Created: MGD 2009-09-30
// -----------------------------------------------------------------------------
template< typename Archive > void AlgorithmsFactories::serialize( Archive&, const unsigned int )
{
    // NOTHING
}

#endif // __AlgorithmsFactories_h_
