// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASAGroup
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AttributesUpdater.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AttributesUpdater constructor
// Created: AHC 2012-03-07
// -----------------------------------------------------------------------------
AttributesUpdater::AttributesUpdater( const std::string& identifier, ObjectListener_ABC& listener )
    : serializer_( )
    , deserializer_( identifier, listener )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttributesUpdater constructor
// Created: AHC 2012-03-07
// -----------------------------------------------------------------------------
AttributesUpdater::~AttributesUpdater()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttributesUpdater constructor
// Created: AHC 2012-03-07
// -----------------------------------------------------------------------------
void AttributesUpdater::Deserialize( const std::string& identifier, ::hla::Deserializer_ABC& deserializer )
{
    deserializer_.Deserialize( identifier, deserializer );
}

// -----------------------------------------------------------------------------
// Name: AttributesUpdater Serialize
// Created: AHC 2012-03-07
// -----------------------------------------------------------------------------
void AttributesUpdater::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    serializer_.Serialize( functor, updateAll );
}
