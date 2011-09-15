// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_UnitTypeResolver_ABC_h
#define plugins_hla_UnitTypeResolver_ABC_h

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
/** @class  UnitTypeResolver_ABC
    @brief  UnitTypeResolver definition
*/
// Created: SLI 2011-09-15
// =============================================================================
class UnitTypeResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitTypeResolver_ABC() {}
    virtual ~UnitTypeResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual unsigned long Resolve( const rpr::EntityType& type ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_UnitTypeResolver_ABC_h
