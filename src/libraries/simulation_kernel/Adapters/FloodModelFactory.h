// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_FLOOD_MODEL_FACTORY_H
#define SWORD_FLOOD_MODEL_FACTORY_H

#include "FloodModelFactory_ABC.h"
#include <boost/serialization/export.hpp>

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

namespace sword
{
// =============================================================================
/** @class  FloodModelFactory
    @brief  Flood model factory
*/
// Created: LGY 2012-06-19
// =============================================================================
class FloodModelFactory : public FloodModelFactory_ABC
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
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}
};

}

BOOST_CLASS_EXPORT_KEY( sword::FloodModelFactory )

#endif // SWORD_FLOOD_MODEL_FACTORY_H
