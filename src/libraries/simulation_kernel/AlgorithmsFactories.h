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
