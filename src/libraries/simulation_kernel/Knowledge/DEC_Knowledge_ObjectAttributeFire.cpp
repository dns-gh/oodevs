// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectAttributeFire.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectAttributeFire.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeFire.h"
#include "Entities/Objects/MIL_FireClass.h"
#include "Entities/Objects/FireAttribute.h"
#include "DEC_Knowledge_Object.h"

#include "MIL.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectAttributeFire, "DEC_Knowledge_ObjectAttributeFire" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeFire constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeFire::DEC_Knowledge_ObjectAttributeFire()
    : attr_       ( 0 )
    , pFireClass_ ( 0 )
    , heat_       ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeFire constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeFire::DEC_Knowledge_ObjectAttributeFire( const FireAttribute& attr )
    : attr_       ( &attr )
    , pFireClass_ ( 0 )
    , heat_       ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeFire destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeFire::~DEC_Knowledge_ObjectAttributeFire()
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectFire::load
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeFire::load( MIL_CheckPointInArchive& file, const uint )
{
    unsigned int nID;
    
    file >> *const_cast< FireAttribute* >( attr_ )
         >> nID
         >> heat_;
    pFireClass_ = MIL_FireClass::Find( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectFire::save
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeFire::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    unsigned int nID = ( pFireClass_ ? pFireClass_->GetID() : (uint)-1 );
    file << boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
    file << attr_
         << nID
         << heat_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeFire::Register
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeFire::Register( DEC_Knowledge_Object& knObject )
{
    knObject.AttachExtension( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeFire::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeFire::UpdateAttributes()
{
    if ( attr_ )
    {
        if ( !pFireClass_ )
            pFireClass_ = & attr_->GetClass();
        heat_ = attr_->GetHeat();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeFire::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeFire::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeFire::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeFire::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeFire::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeFire::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeFire::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeFire::Send( ASN1T_ObjectAttributes& asn ) const
{
    if ( attr_ )
    {
        asn.m.firePresent = 1;
        asn.fire.class_id = pFireClass_->GetID();
        asn.fire.heat     = heat_;    
    }
}
