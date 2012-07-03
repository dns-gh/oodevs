// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_LEGACY_FLOOD_MODEL_FACTORY_H
#define SWORD_LEGACY_FLOOD_MODEL_FACTORY_H

#include "Adapters/FloodModelFactory_ABC.h"
#include "flood/ElevationGetter_ABC.h"
#include <boost/serialization/export.hpp>

namespace sword
{
namespace legacy
{
// =============================================================================
/** @class  FloodModelFactory
    @brief  Flood model factory
*/
// Created: LGY 2012-06-19
// =============================================================================
class FloodModelFactory : public sword::FloodModelFactory_ABC
                        , private flood::ElevationGetter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FloodModelFactory();
    virtual ~FloodModelFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< flood::FloodModel_ABC > CreateFloodModel() const;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive& archive, const unsigned int )
    {
        archive & boost::serialization::base_object< FloodModelFactory_ABC >( *this );
    }
    //@}

private:
    //! @name Helpers
    //@{
    virtual short GetElevationAt( const geometry::Point2f& point ) const;
    //@}
};

}
}

BOOST_CLASS_EXPORT_KEY( sword::legacy::FloodModelFactory )

#endif // SWORD_LEGACY_FLOOD_MODEL_FACTORY_H
