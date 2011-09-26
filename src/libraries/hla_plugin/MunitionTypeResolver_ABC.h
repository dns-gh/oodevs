// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MunitionTypeResolver_ABC_h
#define MunitionTypeResolver_ABC_h

#include <boost/noncopyable.hpp>

namespace rpr
{
    class EntityType;
}

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  MunitionTypeResolver_ABC
    @brief  Munition type resolver definition
*/
// Created: VPR 2011-09-26
// =============================================================================
class MunitionTypeResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MunitionTypeResolver_ABC() {}
    virtual ~MunitionTypeResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual rpr::EntityType Resolve( unsigned int munitionIdentifier ) const = 0;
    //@}
};

}
}

#endif // MunitionTypeResolver_ABC_h
