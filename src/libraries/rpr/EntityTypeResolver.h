// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityTypeResolver_h_
#define __EntityTypeResolver_h_

#include "clients_kernel/ApproximativeMap.h"
#include "rpr/EntityType.h"
#include <boost/noncopyable.hpp>
#include <map>

namespace xml
{
    class xistream;
    class xisubstream;
}

namespace rpr
{

// =============================================================================
/** @class  EntityTypeResolver
    @brief  EntityTypeResolver
*/
// Created: AGE 2008-04-04
// =============================================================================
class EntityTypeResolver : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit EntityTypeResolver( xml::xisubstream xis );
    virtual ~EntityTypeResolver();
    //@}

    //! @name Operations
    //@{
    rpr::EntityType Find( const std::string& name ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadEntry( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    kernel::ApproximativeMap< rpr::EntityType > types_;
    mutable std::map< std::string, rpr::EntityType > resolved_;
    rpr::EntityType default_;
    //@}
};

}

#endif // __EntityTypeResolver_h_
