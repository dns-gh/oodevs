// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef rpr_EntityTypeResolver_ABC_h
#define rpr_EntityTypeResolver_ABC_h

#include <boost/noncopyable.hpp>
#include "EntityType.h"

namespace rpr
{
// =============================================================================
/** @class  EntityTypeResolver_ABC
    @brief  Entity type resolver definition
*/
// Created: SLI 2011-05-02
// =============================================================================
class EntityTypeResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityTypeResolver_ABC() {}
    virtual ~EntityTypeResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual rpr::EntityType Find( const std::string& name ) const = 0;
    //@}
};

}

#endif // rpr_EntityTypeResolver_ABC_h
