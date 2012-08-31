// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef kernel_EntityType_h
#define kernel_EntityType_h

#include "Extension_ABC.h"
#include "Serializable_ABC.h"
#include <boost/noncopyable.hpp>
#include "PropertiesDictionary.h"
#include "PopulationType.h"
#include "InhabitantType.h"
#include "AutomatType.h"

namespace kernel
{
    class PropertiesDictionary;
    class Entity_ABC;
// =============================================================================
/** @class  EntityType
    @brief  Entity Type
*/
// Created: LGY 2012-08-29
// =============================================================================
template< typename T >
class EntityType : public Extension_ABC
                 , public Serializable_ABC
                 , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    EntityType( const Entity_ABC& entity, const T& type, PropertiesDictionary& dictionary )
        : type_( type )
    {
        dictionary.Register( entity, tools::translate( "EntityType", "Info/Type" ), type_, true );
    }
    virtual ~EntityType()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const
    {
        xos << xml::attribute( "type", type_.GetName() );
    }
    //@}

    //! @name Accessors
    //@{
    const T& GetType() const
    {
        return type_;
    }
    //@}

private:
    //! @name Member data
    //@{
    const T& type_;
    //@}
};

}

#endif // kernel_EntityType_h
