// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef gui_EntityType_h
#define gui_EntityType_h

#include "PropertiesDictionary.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/PopulationType.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/AutomatType.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
// =============================================================================
/** @class  EntityType
    @brief  Entity Type
*/
// Created: LGY 2012-08-29
// =============================================================================
template< typename T >
class EntityType : public kernel::Extension_ABC
                 , public kernel::Serializable_ABC
                 , public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    EntityType( const kernel::Entity_ABC& entity, const T& type, PropertiesDictionary& dictionary )
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

#endif // gui_EntityType_h
