// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MineAttribute.h"
#include "Object.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MineAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyPassThrough< MineAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyPassThrough< MineAttribute > )

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute()
    : dotation_           ( 0 )
    , nFullNbrDotation_   ( 0 )
    , nCurrentNbrDotation_( 0 )
    , miningPercentage_   ( 1., 0.05, 0., 1.)
    , bMustBeMined_       ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute( const PHY_DotationCategory& dotation, unsigned int nDefaultMaxNbrDotation )
    : dotation_           ( &dotation )
    , nFullNbrDotation_   ( nDefaultMaxNbrDotation )
    , nCurrentNbrDotation_( nDefaultMaxNbrDotation )
    , miningPercentage_   ( 1., 0.05, 0., 1.)
    , bMustBeMined_       ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: RPD 2009-10-19
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute( const sword::MissionParameter_Value& attributes  )
    : dotation_           ( 0 )
    , nFullNbrDotation_   ( 0 )
    , nCurrentNbrDotation_( 0 )
    , miningPercentage_   ( 1., 0.05, 0., 1.)
    , bMustBeMined_       ( false )
{
    if( attributes.list_size() == 2 )
        miningPercentage_.Set( static_cast< double >( attributes.list( 1 ).quantity() )/100. );
    else if( attributes.list_size() == 5 )
    {
        int dotationId = attributes.list( 1 ).identifier();
        dotation_ = PHY_DotationType::FindDotationCategory( dotationId );
        if( !dotation_ && 0 != dotationId )
            throw MASA_EXCEPTION( "Unknown 'Dotation Type' for mine attribute" );
        nCurrentNbrDotation_ = attributes.list( 2 ).quantity();
        nFullNbrDotation_ = static_cast< unsigned int >( attributes.list( 3 ).areal() );
        miningPercentage_.Set( attributes.list( 4 ).quantity() * 0.01 );
    }
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Load
// Created: JCR 2008-08-26
// -----------------------------------------------------------------------------
void MineAttribute::Load( xml::xistream& xis )
{
    const double completion = xis.attribute< double >( "density", 1.f );
    if( completion >= 0. && completion <= 1. )
        miningPercentage_.Set( completion );
    nCurrentNbrDotation_ = static_cast< unsigned int >( miningPercentage_.Get() * nFullNbrDotation_ );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
MineAttribute::~MineAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::SetMaxDotations
// Created: ABR 2012-03-20
// -----------------------------------------------------------------------------
void MineAttribute::SetMaxDotations( const PHY_DotationCategory& category, unsigned int nFullNbrDotation )
{
    dotation_            = &category;
    nFullNbrDotation_    = nFullNbrDotation;
    nCurrentNbrDotation_ = static_cast< int >( miningPercentage_.Get() * nFullNbrDotation );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::MustBeMined
// Created: MMC 2013-06-14
// -----------------------------------------------------------------------------
bool MineAttribute::MustBeMined() const
{
    return bMustBeMined_;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::SetMustBeMined
// Created: MMC 2013-06-14
// -----------------------------------------------------------------------------
void MineAttribute::SetMustBeMined( bool val )
{
    bMustBeMined_ = val;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
MineAttribute& MineAttribute::operator=( const MineAttribute& rhs )
{
    nCurrentNbrDotation_ = rhs.nCurrentNbrDotation_;
    miningPercentage_ = rhs.miningPercentage_;
    nFullNbrDotation_ = rhs.nFullNbrDotation_;
    dotation_ = rhs.dotation_;
    bMustBeMined_ = rhs.bMustBeMined_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::load
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void MineAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    double percentage = 0.;
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> dotation_;
    ar >> nFullNbrDotation_;
    ar >> nCurrentNbrDotation_;
    ar >> percentage;
    ar >> bMustBeMined_;
    miningPercentage_.Set( percentage );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::save
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void MineAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << dotation_;
    ar << nFullNbrDotation_;
    ar << nCurrentNbrDotation_;
    ar << (double)miningPercentage_.Get();
    ar << bMustBeMined_;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::WriteODB
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void MineAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "mine" ) 
            << xml::attribute( "density", miningPercentage_.Get() ) 
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Instanciate
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void MineAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< MineAttribute > >( *new T_KnowledgeProxyType() );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void MineAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< MineAttribute, MineAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void MineAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    if( dotation_ )
    {
        asn.mutable_mine()->mutable_resource()->set_id( dotation_->GetMosID() );
        asn.mutable_mine()->set_dotation( nCurrentNbrDotation_ );
    }
    asn.mutable_mine()->set_percentage( unsigned int( miningPercentage_.Send() * 100. ) );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
bool MineAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
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
// Name: MineAttribute::OnUpdate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void MineAttribute::OnUpdate( const sword::MissionParameter_Value& attribute )
{
    if( attribute.list_size() > 4 )
    {
        Set( attribute.list( 4 ).quantity() / 100. ); // four first parameters not used
        NotifyAttributeUpdated( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::UpdateConstruction
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MineAttribute::Set( double percentage )
{
    miningPercentage_.Set( percentage );
    unsigned int tmp = (unsigned int)( miningPercentage_.Get() * nFullNbrDotation_ );
    if( nCurrentNbrDotation_ != tmp || miningPercentage_.NeedToBeSent() )
        NotifyAttributeUpdated( eOnUpdate );
    nCurrentNbrDotation_ = tmp;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::UpdateConstruction
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MineAttribute::Update( double rDeltaPercentage )
{
    Set( miningPercentage_.Get() + rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::GetDotationNeededForConstruction
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
unsigned int MineAttribute::GetDotationNeededForConstruction( double rDeltaPercentage ) const
{
    const double rNewPercentage = std::max( 0., std::min( 1., miningPercentage_.Get() + rDeltaPercentage ) );
    return (unsigned int)( rNewPercentage * nFullNbrDotation_ ) - nCurrentNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::GetDotationRecoveredWhenDestroying
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
unsigned int MineAttribute::GetDotationRecoveredWhenDestroying( double rDeltaPercentage ) const
{
    const double rNewPercentage = std::max( 0., std::min( 1., miningPercentage_.Get() - rDeltaPercentage ) );
    return nCurrentNbrDotation_ - (unsigned int)( rNewPercentage * nFullNbrDotation_ );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::GetPercentage
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
double MineAttribute::GetState() const
{
    return miningPercentage_.Get();
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::GetCurrentDotations
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
unsigned int MineAttribute::GetCurrentDotations() const
{
    return nCurrentNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::GetDotationType
// Created: JSR 2010-05-27
// -----------------------------------------------------------------------------
unsigned int MineAttribute::GetDotationType() const
{
    return dotation_ ? dotation_->GetMosID() : 0;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Update
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool MineAttribute::Update( const MineAttribute& rhs )
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
    miningPercentage_.Set( rhs.miningPercentage_.Get() );
    if( miningPercentage_.NeedToBeSent() )
        NotifyAttributeUpdated( eOnUpdate );

    return NeedUpdate( eOnUpdate );
}
