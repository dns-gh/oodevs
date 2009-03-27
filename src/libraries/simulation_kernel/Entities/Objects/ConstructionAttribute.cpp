// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ConstructionAttribute.h"
#include "Entities\Agents\Units\Dotations\PHY_DotationType.h"
#include "Entities\Agents\Units\Dotations\PHY_DotationCategory.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeConstruction.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "HLA/HLA_UpdateFunctor.h"
#include <hla/AttributeIdentifier.h>

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( ConstructionAttribute, "ConstructionAttribute" )

using namespace hla;

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute()
    : nFullNbrDotation_ ( 0 )
    , nCurrentNbrDotation_ ( 0 )
    , rConstructionPercentage_ ( 1. )
    , dotation_  ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( const PHY_DotationCategory& dotation, uint nFullNbrDotation )
    : nFullNbrDotation_ ( nFullNbrDotation )
    , nCurrentNbrDotation_ ( 0 )
    , rConstructionPercentage_ ( 0. )
    , dotation_ ( &dotation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Load
// Created: JCR 2008-08-26
// -----------------------------------------------------------------------------
void ConstructionAttribute::Load( xml::xistream& xis )
{
    assert( dotation_ );
    MT_Float completion = 1.;

    xis >> xml::optional() >> xml::attribute( "completion", completion );
    if( completion > 0. && completion <= 1. )
        rConstructionPercentage_ = completion;
    nCurrentNbrDotation_ = uint( rConstructionPercentage_ * nFullNbrDotation_ );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( const PHY_DotationCategory& dotation, const ASN1T_ObjectAttributes& asn )
    : nFullNbrDotation_ ( asn.construction.dotation_nbr )
    , nCurrentNbrDotation_ ( asn.construction.dotation_nbr )
    , rConstructionPercentage_ ( 1. )
    , dotation_  ( &dotation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ConstructionAttribute::~ConstructionAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Instanciate
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void ConstructionAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach( *new DEC_Knowledge_ObjectAttributeConstruction( *this ) );
}


// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ConstructionAttribute& ConstructionAttribute::operator=( const ConstructionAttribute& rhs )
{
    nFullNbrDotation_ = rhs.nFullNbrDotation_;
    nCurrentNbrDotation_ = rhs.nCurrentNbrDotation_;
    rConstructionPercentage_ = rhs.rConstructionPercentage_;
    dotation_ = rhs.dotation_;    
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::load
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void ConstructionAttribute::load( MIL_CheckPointInArchive& ar, const uint )
{
    std::string dotation;
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this )
       >> dotation
       >> nCurrentNbrDotation_
       >> rConstructionPercentage_;
    dotation_ = PHY_DotationType::FindDotationCategory( dotation );
    if ( !dotation_ )
       throw std::runtime_error( "Unknown dotation category - " + dotation + " - " ); 
}
    
// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::save
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void ConstructionAttribute::save( MIL_CheckPointOutArchive& ar, const uint ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    if ( dotation_ )
       ar << dotation_->GetName();
    else
       ar << "";
    ar << nFullNbrDotation_
       << nCurrentNbrDotation_
       << rConstructionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::WriteODB
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void ConstructionAttribute::WriteODB( xml::xostream& xos ) const
{    
    xos << xml::start( "construction" ) 
            << xml::attribute( "completion", rConstructionPercentage_ )
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::SendFullState
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void ConstructionAttribute::SendFullState( ASN1T_ObjectAttributes& asn ) const
{
    if ( dotation_ )
    {
        asn.m.constructionPresent = 1;        
        
        asn.construction.m.dotation_typePresent = 1;
        asn.construction.dotation_type = dotation_->GetMosID();

        asn.construction.m.dotation_nbrPresent = 1;
        asn.construction.dotation_nbr = nCurrentNbrDotation_;
        
        asn.construction.m.percentagePresent = 1;
        asn.construction.percentage = rConstructionPercentage_ * 100.;
    }
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ConstructionAttribute::SendUpdate( ASN1T_ObjectAttributes& asn ) const
{
    if ( NeedUpdate() )
    {
        asn.m.constructionPresent = 1;

        asn.construction.m.dotation_nbrPresent = 1;
        asn.construction.dotation_nbr = nCurrentNbrDotation_;

        asn.construction.m.percentagePresent = 1;
        asn.construction.percentage = rConstructionPercentage_ * 100.;
        Reset( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::OnMagicActionUpdate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::OnUpdate( const ASN1T_ObjectAttributes& asn )
{
    if( asn.m.constructionPresent )
    {
        if ( asn.construction.m.percentagePresent )
            Set( asn.construction.percentage / 100. );
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Set
// Created: JCR 2008-08-12
// -----------------------------------------------------------------------------
void ConstructionAttribute::Set( MT_Float percentage )
{
    rConstructionPercentage_ = std::max( 0., std::min( 1., percentage ) );
    nCurrentNbrDotation_ = (uint)( rConstructionPercentage_ * nFullNbrDotation_ );
    NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::UpdateConstruction
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void ConstructionAttribute::Build( MT_Float rDeltaPercentage )
{
    Set( rConstructionPercentage_ + rDeltaPercentage );        
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::HasDotation
// Created: JCR 2008-08-11
// -----------------------------------------------------------------------------
bool ConstructionAttribute::HasDotation( const PHY_DotationCategory& dotation ) const
{
    return ( &dotation == dotation_ ) && GetState() > 0;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::UpdateConstruction
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
uint ConstructionAttribute::GetMaxDotation() const
{
    return nFullNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::GetDotationNeededForConstruction
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
uint ConstructionAttribute::GetDotationNeededForConstruction( MT_Float rDeltaPercentage ) const
{
    const MT_Float rNewPercentage = std::max( 0., std::min( 1., rConstructionPercentage_ + rDeltaPercentage ) );
    return (uint)( rNewPercentage * nFullNbrDotation_ ) - nCurrentNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::GetDotationRecoveredWhenDestroying
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
uint ConstructionAttribute::GetDotationRecoveredWhenDestroying( MT_Float rDeltaPercentage ) const
{
    const MT_Float rNewPercentage = std::max( 0., std::min( 1., rConstructionPercentage_ - rDeltaPercentage ) );
    return nCurrentNbrDotation_ - (uint)( rNewPercentage * nFullNbrDotation_ );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::GetPercentage
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
MT_Float ConstructionAttribute::GetState() const
{
    return rConstructionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Serialize
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void ConstructionAttribute::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "completion", NeedUpdate( eOnHLAUpdate ), rConstructionPercentage_ );
    Reset( eOnHLAUpdate );
}
    
// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Deserialize
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void ConstructionAttribute::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "completion" )
    {
        double rConstruction;
        deserializer >> rConstruction;
        if( rConstruction > 100 )
            rConstruction -= 100;
        Build( rConstruction );
    }
}

