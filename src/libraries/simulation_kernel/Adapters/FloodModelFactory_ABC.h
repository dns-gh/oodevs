// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_FLOOD_MODEL_FACTORY_ABC_H
#define SWORD_FLOOD_MODEL_FACTORY_ABC_H

#include <memory>

namespace propagation
{
    class FloodModel_ABC;
}

namespace sword
{
// =============================================================================
/** @class  FloodModelFactory_ABC
    @brief  Flood model factory definition
*/
// Created: LGY 2012-06-13
// =============================================================================
class FloodModelFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FloodModelFactory_ABC() {}
    virtual ~FloodModelFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::unique_ptr< propagation::FloodModel_ABC > CreateFloodModel() const = 0;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive&, const unsigned int )
    {
        // NOTHING
    }
    //@}
};

}

#endif // SWORD_FLOOD_MODEL_FACTORY_ABC_H
