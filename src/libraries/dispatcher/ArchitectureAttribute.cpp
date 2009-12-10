// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "ArchitectureAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute constructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
ArchitectureAttribute::ArchitectureAttribute( const Model& model, const ASN1T_UrbanAttributes& asnMsg )
: UrbanObjectAttribute_ABC( model, asnMsg )
, height_ ( 0. )
, floorNumber_     ( 0 )
, basementLevelNumber_ ( 0 )
, roofShape_ ( "" )
, material_ ( "" )
, innerCluttering_ ( 0. )
, facadeOpacity_ ( 0. )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute destructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
ArchitectureAttribute::~ArchitectureAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute::Update
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void ArchitectureAttribute::Update( const ASN1T_UrbanAttributes& asnMsg )
{
    if( asnMsg.m.architecturePresent )
    {
        //if ( asnMsg.architecture.m.heightPresent )
            height_ = asnMsg.architecture.height;
        //if ( asnMsg.architecture.m.floorNumberPresent )
            floorNumber_ = asnMsg.architecture.floorNumber;
        //if ( asnMsg.architecture.m.basementLevelNumberPresent )
            basementLevelNumber_ = asnMsg.architecture.basementLevelNumber;  
        //if ( asnMsg.architecture.m.basementLevelNumberPresent )
            roofShape_ = asnMsg.architecture.roofShape; 
        //if ( asnMsg.architecture.m.basementLevelNumberPresent )
            material_ = asnMsg.architecture.material; 
        //if ( asnMsg.architecture.m.basementLevelNumberPresent )
            innerCluttering_ = asnMsg.architecture.innerCluttering; 
        //if ( asnMsg.architecture.m.basementLevelNumberPresent )
            facadeOpacity_ = asnMsg.architecture.facadeOpacity; 
    }
}

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute::Send
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void ArchitectureAttribute::Send( ASN1T_UrbanAttributes& asnMsg ) const
{
    asnMsg.m.architecturePresent = 1;

    //asnMsg.architecture.m.heightPresent = 1;
    asnMsg.architecture.height = height_;

    //asnMsg.architecture.m.floorNumberPresent = 1;
    asnMsg.architecture.floorNumber = floorNumber_;

    //asnMsg.architecture.m.basementLevelNumberPresent = 1;
    asnMsg.architecture.basementLevelNumber = basementLevelNumber_;

    //asnMsg.architecture.m.roofShapePresent = 1;
    asnMsg.architecture.roofShape = roofShape_.c_str();

    //asnMsg.architecture.m.materialPresent = 1;
    asnMsg.architecture.material = material_.c_str();

    //asnMsg.architecture.m.innerClutteringPresent = 1;
    asnMsg.architecture.innerCluttering = innerCluttering_;

    //asnMsg.architecture.m.facadeOpacityPresent = 1;
    asnMsg.architecture.facadeOpacity = facadeOpacity_;
}

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute::AsnDelete
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void ArchitectureAttribute::AsnDelete( ASN1T_UrbanAttributes& /*asnMsg*/ ) const
{
    //    delete asnMsg.u.mine_jam;
}
