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
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/Authentication.h"
#include "Entities/MIL_EntityManager.h"

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
    , bBuildByGen_            ( false )
    , density_                ( 1.0f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( const PHY_DotationCategory& dotation, unsigned int nFullNbrDotation, float density /* = 1.0f */ )
    : nFullNbrDotation_       ( nFullNbrDotation )
    , nCurrentNbrDotation_    ( nFullNbrDotation )
    , constructionPercentage_ ( 1., 0.05, 0., 1.)
    , dotation_               ( &dotation )
    , bBuildByGen_            ( false )
    , density_                ( density )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( const PHY_DotationCategory& dotation, const sword::ObjectAttributes& asn )
    : nFullNbrDotation_       ( asn.construction().dotation() )
    , nCurrentNbrDotation_    ( asn.construction().dotation() )
    , constructionPercentage_ ( 1., 0.05, 0., 1.)
    , dotation_               ( &dotation )
    , bBuildByGen_            ( false )
    , density_                ( asn.construction().density() )
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
// Name: ConstructionAttribute::SetMaxDotations
// Created: ABR 2012-03-20
// -----------------------------------------------------------------------------
void ConstructionAttribute::SetMaxDotations( const PHY_DotationCategory& category, unsigned int nFullNbrDotation )
{
    nFullNbrDotation_    = nFullNbrDotation;
    nCurrentNbrDotation_ = static_cast< int >( constructionPercentage_.Get() * nFullNbrDotation );
    dotation_            = &category;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Load
// Created: JCR 2008-08-26
// -----------------------------------------------------------------------------
void ConstructionAttribute::Load( xml::xistream& xis )
{
    const double completion = std::max( 0.01, xis.attribute< double >( "completion", 1.f ) );
    constructionPercentage_.Set( completion );
    nCurrentNbrDotation_ = static_cast< unsigned int >( constructionPercentage_.Get() * nFullNbrDotation_ );
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
    density_ = rhs.density_;
    bBuildByGen_  = rhs.bBuildByGen_;
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
        NotifyAttributeUpdated( eOnUpdate );
        dotation_ = rhs.dotation_;
    }
    if( nFullNbrDotation_ != rhs.nFullNbrDotation_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        nFullNbrDotation_ = rhs.nFullNbrDotation_;
    }
    if( nCurrentNbrDotation_ != rhs.nCurrentNbrDotation_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        nCurrentNbrDotation_ = rhs.nCurrentNbrDotation_;
    }
    constructionPercentage_.Set( rhs.constructionPercentage_.Get() );
    if( constructionPercentage_.NeedToBeSent() )
        NotifyAttributeUpdated( eOnUpdate );

    if( density_ != rhs.density_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        density_ = rhs.density_;
    }

    if( bBuildByGen_ != rhs.bBuildByGen_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        bBuildByGen_ = rhs.bBuildByGen_;
    }

    return NeedUpdate( eOnUpdate );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::load
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void ConstructionAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> dotation_;
    ar >> nFullNbrDotation_;
    ar >> nCurrentNbrDotation_;
    ar >> density_;
    double tmp;
    ar >> tmp;
    constructionPercentage_.Set( tmp );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::save
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void ConstructionAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << dotation_;
    double constructionPercentage = constructionPercentage_.Get();
    ar << nFullNbrDotation_;
    ar << nCurrentNbrDotation_;
    ar << density_;
    ar << constructionPercentage;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::WriteODB
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void ConstructionAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "construction" )
            << xml::attribute( "completion", constructionPercentage_.Get() );
    if( dotation_ != 0 && nFullNbrDotation_ != 0 )
        xos << xml::start( "resources" )
                << xml::start( "dotation" )
                    << xml::attribute( "name", dotation_->GetName() )
                    << xml::attribute( "count", nFullNbrDotation_ )
                << xml::end
            << xml::end;
     xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::SendFullState
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void ConstructionAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    if( dotation_ )
    {
        asn.mutable_construction()->mutable_resource()->set_id( dotation_->GetMosID() );
        asn.mutable_construction()->set_dotation( nCurrentNbrDotation_ );
    }
    asn.mutable_construction()->set_percentage( unsigned int( constructionPercentage_.Send() * 100. ) );
    asn.mutable_construction()->set_density( density_ );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
bool ConstructionAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::OnMagicActionUpdate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::OnUpdate( const sword::MissionParameter_Value& attribute )
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
    unsigned int tmp = static_cast< unsigned int >( constructionPercentage_.Get() * nFullNbrDotation_ );
    if( nCurrentNbrDotation_ != tmp || constructionPercentage_.NeedToBeSent() )
        NotifyAttributeUpdated( eOnUpdate );
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

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::SetDensity
// Created: LGY 2012-08-27
// -----------------------------------------------------------------------------
void ConstructionAttribute::SetDensity( double density )
{
    density_ = static_cast< float >( density );
}
