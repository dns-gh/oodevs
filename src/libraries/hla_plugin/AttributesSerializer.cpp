// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AttributesSerializer.h"
#include <hla/Serializer.h>
#include <hla/AttributeIdentifier.h>
#include <hla/UpdateFunctor_ABC.h>
#include <boost/foreach.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AttributesSerializer constructor
// Created: SLI 2011-06-20
// -----------------------------------------------------------------------------
AttributesSerializer::AttributesSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttributesSerializer destructor
// Created: SLI 2011-06-20
// -----------------------------------------------------------------------------
AttributesSerializer::~AttributesSerializer()
{
    // NOTHING
}

namespace
{
    template< typename T >
    void Serialize( ::hla::UpdateFunctor_ABC& functor, const T& attribute )
    {
        ::hla::Serializer serializer;
        attribute.second( serializer );
        functor.Visit( ::hla::AttributeIdentifier( attribute.first ), serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: AttributesSerializer::Serialize
// Created: SLI 2011-06-20
// -----------------------------------------------------------------------------
void AttributesSerializer::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    if( updateAll )
        BOOST_FOREACH( const T_Attribute& attribute, attributes_ )
            ::Serialize( functor, attribute );
    else
        BOOST_FOREACH( const T_Attribute& attribute, attributes_ )
            if( std::find( updates_.begin(), updates_.end(), attribute.first ) != updates_.end() )
                ::Serialize( functor, attribute );
    updates_.clear();
}
