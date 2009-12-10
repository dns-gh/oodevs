// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "VegetationAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: VegetationAttribute constructor
// Created: SLG 2009-12-04 
// -----------------------------------------------------------------------------
VegetationAttribute::VegetationAttribute( const Model& model, const ASN1T_UrbanAttributes& asnMsg )
    : UrbanObjectAttribute_ABC( model, asnMsg )
    , type_     ( "" )
    , height_   ( 0 )
    , density_  ( 0. )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: VegetationAttribute destructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
VegetationAttribute::~VegetationAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: VegetationAttribute::Update
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void VegetationAttribute::Update( const ASN1T_UrbanAttributes& asnMsg )
{
    if( asnMsg.m.vegetationPresent )
    {
        //if ( asnMsg.vegetation.m.typePresent )
            type_ = asnMsg.vegetation.type;
        //if ( asnMsg.vegetation.m.heightPresent )
            height_ = asnMsg.vegetation.height;
        //if ( asnMsg.vegetation.m.densityPresent )
            density_ = asnMsg.vegetation.density;        
    }
}

// -----------------------------------------------------------------------------
// Name: VegetationAttribute::Send
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void VegetationAttribute::Send( ASN1T_UrbanAttributes& asnMsg ) const
{
    asnMsg.m.vegetationPresent = 1;

    //asnMsg.vegetation.m.typePresent = 1;
    asnMsg.vegetation.type = type_.c_str();

    //asnMsg.vegetation.m.heightPresent = 1;
    asnMsg.vegetation.height = height_;

    //asnMsg.vegetation.m.densityPresent = 1;
    asnMsg.vegetation.density = density_;
}

// -----------------------------------------------------------------------------
// Name: VegetationAttribute::AsnDelete
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void VegetationAttribute::AsnDelete( ASN1T_UrbanAttributes& /*asnMsg*/ ) const
{
    //    delete asnMsg.u.mine_jam;
}
