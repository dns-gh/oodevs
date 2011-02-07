// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Formation.h"
#include "SerializationTools.h"
#include "Dimension.h"
#include <hla/UpdateFunctor_ABC.h>
#include <hla/AttributeIdentifier.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
Formation::Formation()
    : onRoad_ ( false )
    , changed_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation::Serialize
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void Formation::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    if( updateAll || changed_ )
    {
        SerializeFormation( functor );
        SerializeDimension( functor );
        changed_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: Formation::SerializeFormation
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void Formation::SerializeFormation( ::hla::UpdateFunctor_ABC& functor ) const
{
    long value = onRoad_ ? 5 : 1; // column, assembly
    ::hla::Serializer serializer;
    serializer << value;
    functor.Visit( ::hla::AttributeIdentifier( "Formation" ), serializer );
}

// -----------------------------------------------------------------------------
// Name: Formation::SerializeDimension
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void Formation::SerializeDimension( ::hla::UpdateFunctor_ABC& functor ) const
{
    Dimension dim( ( onRoad_ ? 200.f : 100.f ),
                   ( onRoad_ ?  50.f : 100.f ),
                   3.f );
    ::hla::Serializer serializer;
    dim.Serialize( serializer );
    functor.Visit( ::hla::AttributeIdentifier( "Dimensions" ), serializer );
}

// -----------------------------------------------------------------------------
// Name: Formation::Update
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void Formation::Update( bool isOnRoad )
{
    changed_ = isOnRoad != onRoad_;
    onRoad_ = isOnRoad;
}
