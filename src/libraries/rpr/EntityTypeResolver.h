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

#include "EntityTypeResolver_ABC.h"
#include "clients_kernel/ApproximativeMap.h"
#include <boost/noncopyable.hpp>
#include <map>
#include <vector>

namespace xml
{
    class xistream;
    class xisubstream;
}

namespace rpr
{
// =============================================================================
/** @class  EntityTypeResolver
    @brief  Entity type resolver
*/
// Created: AGE 2008-04-04
// =============================================================================
class EntityTypeResolver : public EntityTypeResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit EntityTypeResolver( xml::xisubstream xis );
    virtual ~EntityTypeResolver();
    //@}

    //! @name Operations
    //@{
    virtual rpr::EntityType Find( const std::string& name ) const;
    virtual std::string Resolve( const rpr::EntityType& type ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadEntry( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    struct T_Name
    {
        T_Name( const rpr::EntityType& type, const std::string& name )
            : type_( type )
            , name_( name )
        {}
        rpr::EntityType type_;
        std::string name_;
    };
    typedef std::vector< T_Name > T_Names;
    //@}

private:
    //! @name Member data
    //@{
    kernel::ApproximativeMap< rpr::EntityType > types_;
    T_Names names_;
    mutable std::map< std::string, rpr::EntityType > resolvedTypes_;
    mutable std::map< rpr::EntityType, std::string > resolvedNames_;
    rpr::EntityType defaultType_;
    std::string defaultName_;
    rpr::EntityType threshold_;
    //@}
};

}

#endif // __EntityTypeResolver_h_
