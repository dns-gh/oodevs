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
#include "Object.h"
#include "Entities\Agents\Units\Dotations\PHY_DotationType.h"
#include "Entities\Agents\Units\Dotations\PHY_DotationCategory.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeConstruction.h"
#include "hla/HLA_UpdateFunctor.h"
#include "protocol/protocol.h"
#include <hla/AttributeIdentifier.h>
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( ConstructionAttribute )

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute()
    : nFullNbrDotation_       ( 0 )
    , nCurrentNbrDotation_    ( 0 )
    , rConstructionPercentage_( 1. )
    , dotation_               ( 0 )
    , bBuildByGen_           ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( const PHY_DotationCategory& dotation, unsigned int nFullNbrDotation )
    : nFullNbrDotation_       ( nFullNbrDotation )
    , nCurrentNbrDotation_    ( nFullNbrDotation )
    , rConstructionPercentage_( 1.0 )
    , dotation_               ( &dotation )
    , bBuildByGen_            ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( const PHY_DotationCategory& dotation, const Common::MsgObjectAttributes& asn )
    : nFullNbrDotation_       ( asn.construction().dotation_nbr() )
    , nCurrentNbrDotation_    ( asn.construction().dotation_nbr() )
    , rConstructionPercentage_( 1. )
    , dotation_               ( &dotation )
    , bBuildByGen_            ( false )
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
// Name: ConstructionAttribute::Load
// Created: JCR 2008-08-26
// -----------------------------------------------------------------------------
void ConstructionAttribute::Load( xml::xistream& xis )
{
    assert( dotation_ );
    const MT_Float completion = xis.attribute< MT_Float >( "completion", 1.f );
    if( completion > 0. && completion <= 1. )
        rConstructionPercentage_ = completion;
    nCurrentNbrDotation_ = unsigned int( rConstructionPercentage_ * nFullNbrDotation_ );
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
// Name: ConstructionAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void ConstructionAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< ConstructionAttribute, ConstructionAttribute >( *this );
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
void ConstructionAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    std::string dotation;
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this )
       >> dotation
       >> nFullNbrDotation_
       >> nCurrentNbrDotation_
       >> rConstructionPercentage_;
    dotation_ = PHY_DotationType::FindDotationCategory( dotation );
    if( !dotation_ )
       throw std::runtime_error( "Unknown dotation category - " + dotation + " - " );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::save
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void ConstructionAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    if( dotation_ )
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
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::SendFullState
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void ConstructionAttribute::SendFullState( Common::MsgObjectAttributes& asn ) const
{
    if( dotation_ )
    {
        asn.mutable_construction()->mutable_resource()->set_id( dotation_->GetMosID() );
        asn.mutable_construction()->set_dotation_nbr( nCurrentNbrDotation_ );
        asn.mutable_construction()->set_percentage( unsigned int( rConstructionPercentage_ * 100. ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ConstructionAttribute::SendUpdate( Common::MsgObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnCreation ) | NeedUpdate( eOnUpdate ) )
    {
        asn.mutable_construction()->set_dotation_nbr( nCurrentNbrDotation_ );
        asn.mutable_construction()->set_percentage( unsigned int( rConstructionPercentage_ * 100. ) );
        Reset( eOnUpdate | eOnHLAUpdate);
    }
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::OnMagicActionUpdate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::OnUpdate( const Common::MsgMissionParameter_Value& attribute )
{
    if( attribute.list_size() > 4 )
    {
        Set( attribute.list( 4 ).quantity() / 100. ); // four first parameters not used
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
    nCurrentNbrDotation_ = (unsigned int)( rConstructionPercentage_ * nFullNbrDotation_ );
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
unsigned int ConstructionAttribute::GetMaxDotation() const
{
    return nFullNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::GetCurrentDotation
// Created: JSR 2010-05-27
// -----------------------------------------------------------------------------
unsigned int ConstructionAttribute::GetCurrentDotation() const
{
    return nCurrentNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::GetDotationType
// Created: JSR 2010-05-25
// -----------------------------------------------------------------------------
unsigned int ConstructionAttribute::GetDotationType() const
{
    return dotation_ ? dotation_->GetMosID() : 0;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::GetDotationNeededForConstruction
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
unsigned int ConstructionAttribute::GetDotationNeededForConstruction( MT_Float rDeltaPercentage ) const
{
    const MT_Float rNewPercentage = std::max( 0., std::min( 1., rConstructionPercentage_ + rDeltaPercentage ) );
    return (unsigned int)( rNewPercentage * nFullNbrDotation_ ) - nCurrentNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::GetDotationRecoveredWhenDestroying
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
unsigned int ConstructionAttribute::GetDotationRecoveredWhenDestroying( MT_Float rDeltaPercentage ) const
{
    const MT_Float rNewPercentage = std::max( 0., std::min( 1., rConstructionPercentage_ - rDeltaPercentage ) );
    return nCurrentNbrDotation_ - (unsigned int)( rNewPercentage * nFullNbrDotation_ );
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
void ConstructionAttribute::Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer )
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

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::GetPercentage
// Created: MGD 2010-03-26
// -----------------------------------------------------------------------------
bool ConstructionAttribute::NeedDestruction() const
{
    return !bBuildByGen_ && rConstructionPercentage_ == 0.f;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::NotifyBuildByGen
// Created: MGD 2010-03-26
// -----------------------------------------------------------------------------
void ConstructionAttribute::NotifyBuildByGen()
{
    bBuildByGen_ = true;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::NotifyStopBuildByGen
// Created: MGD 2010-03-26
// -----------------------------------------------------------------------------
void ConstructionAttribute::NotifyStopBuildByGen()
{
    bBuildByGen_ = false;
}
