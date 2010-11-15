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
#include "hla/HLA_UpdateFunctor.h"
#include "protocol/protocol.h"
#include <hla/AttributeIdentifier.h>
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( ConstructionAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyPassThrough< ConstructionAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyPassThrough< ConstructionAttribute > )

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute()
    : nFullNbrDotation_       ( 0 )
    , nCurrentNbrDotation_    ( 0 )
    , constructionPercentage_ ( 1., 0.05, 0., 1.)
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
    , constructionPercentage_ ( 1., 0.05, 0., 1.)
    , dotation_               ( &dotation )
    , bBuildByGen_            ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( const PHY_DotationCategory& dotation, const Common::ObjectAttributes& asn )
    : nFullNbrDotation_       ( asn.construction().dotation_nbr() )
    , nCurrentNbrDotation_    ( asn.construction().dotation_nbr() )
    , constructionPercentage_ ( 1., 0.05, 0., 1.)
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
    const double completion = xis.attribute< double >( "completion", 1.f );
    if( completion > 0. && completion <= 1. )
        constructionPercentage_.Set( completion );
    nCurrentNbrDotation_ = unsigned int( constructionPercentage_.Get() * nFullNbrDotation_ );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Instanciate
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void ConstructionAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< ConstructionAttribute > >( *new T_KnowledgeProxyType() );
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
    constructionPercentage_ = rhs.constructionPercentage_;
    dotation_ = rhs.dotation_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Update
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool ConstructionAttribute::Update( const ConstructionAttribute& rhs )
{
    if( dotation_ != rhs.dotation_ )
    {
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
        dotation_ = rhs.dotation_;
    }
    if( nFullNbrDotation_ != rhs.nFullNbrDotation_ )
    {
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
        nFullNbrDotation_ = rhs.nFullNbrDotation_;
    }
    if( nCurrentNbrDotation_ != rhs.nCurrentNbrDotation_ )
    {
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
        nCurrentNbrDotation_ = rhs.nCurrentNbrDotation_;
    }
    constructionPercentage_.Set( rhs.constructionPercentage_.Get() );
    if( constructionPercentage_.NeedToBeSent() )
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );

    return NeedUpdate( eOnUpdate );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::load
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void ConstructionAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    dotation_ = 0;
    std::string dotation;
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this )
       >> dotation
       >> nFullNbrDotation_
       >> nCurrentNbrDotation_;

    double tmp;
    ar >> tmp;
    constructionPercentage_.Set( tmp );
    if( !dotation.empty() )
    {
        dotation_ = PHY_DotationType::FindDotationCategory( dotation );
        if( !dotation_ )
            throw std::runtime_error( "Unknown dotation category - " + dotation + " - " );
    }
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::save
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void ConstructionAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    std::string emptyString;
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    if( dotation_ )
       ar << dotation_->GetName();
    else
       ar << emptyString;
    ar << nFullNbrDotation_
       << nCurrentNbrDotation_
       << (double)constructionPercentage_.Get();
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::WriteODB
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void ConstructionAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "construction" )
            << xml::attribute( "completion", constructionPercentage_.Get() )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::SendFullState
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void ConstructionAttribute::SendFullState( Common::ObjectAttributes& asn ) const
{
    if( dotation_ )
    {
        asn.mutable_construction()->mutable_resource()->set_id( dotation_->GetMosID() );
        asn.mutable_construction()->set_dotation_nbr( nCurrentNbrDotation_ );        
    }
    asn.mutable_construction()->set_percentage( unsigned int( constructionPercentage_.Send() * 100. ) );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ConstructionAttribute::SendUpdate( Common::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::OnMagicActionUpdate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::OnUpdate( const Common::MsgMissionParameter_Value& attribute )
{
    if( attribute.list_size() > 4 )
        Set( attribute.list( 4 ).quantity() / 100. ); // four first parameters not used
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Set
// Created: JCR 2008-08-12
// -----------------------------------------------------------------------------
void ConstructionAttribute::Set( double percentage )
{
    constructionPercentage_.Set( percentage );
    unsigned int tmp = (unsigned int)( constructionPercentage_.Get() * nFullNbrDotation_ );
    if( nCurrentNbrDotation_ != tmp || constructionPercentage_.NeedToBeSent() )
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
    nCurrentNbrDotation_ = tmp;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::UpdateConstruction
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void ConstructionAttribute::Build( double rDeltaPercentage )
{
    Set( constructionPercentage_.Get() + rDeltaPercentage );
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
// Name: ConstructionAttribute::GetDotationType
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool ConstructionAttribute::IsConstructed() const
{
    return constructionPercentage_.Get() >= 1.;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::GetDotationNeededForConstruction
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
unsigned int ConstructionAttribute::GetDotationNeededForConstruction( double rDeltaPercentage ) const
{
    const double rNewPercentage = std::max( 0., std::min( 1., constructionPercentage_.Get() + rDeltaPercentage ) );
    return (unsigned int)( rNewPercentage * nFullNbrDotation_ ) - nCurrentNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::GetDotationRecoveredWhenDestroying
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
unsigned int ConstructionAttribute::GetDotationRecoveredWhenDestroying( double rDeltaPercentage ) const
{
    const double rNewPercentage = std::max( 0., std::min( 1., constructionPercentage_.Get() - rDeltaPercentage ) );
    return nCurrentNbrDotation_ - (unsigned int)( rNewPercentage * nFullNbrDotation_ );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::GetPercentage
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
double ConstructionAttribute::GetState() const
{
    return constructionPercentage_.Get();
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Serialize
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void ConstructionAttribute::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "completion", NeedUpdate( eOnHLAUpdate ), constructionPercentage_.Get() );
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
    return !bBuildByGen_ && constructionPercentage_.Get() == 0.;
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
