// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "BuildableCapacity.h"
#include "MIL_Object_ABC.h"
#include "ConstructionAttribute.h"
#include "MIL_AgentServer.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Tools/MIL_Tools.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( BuildableCapacity )

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::BuildableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BuildableCapacity::BuildableCapacity()
    : default_( 0 )
    , dotation_( 0 )
    , nFullNbrDotation_( 0 )
    , unitType_( ConstructionCapacity::eRaw )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BuildableCapacity::BuildableCapacity( const BuildableCapacity& from )
    : default_( from.default_ )
    , dotation_( from.dotation_ )
    , nFullNbrDotation_( from.nFullNbrDotation_ )
    , unitType_( from.unitType_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity constructor
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
BuildableCapacity::BuildableCapacity( const PHY_ConsumptionType& consumption, ConstructionCapacity::E_UnitType type, xml::xistream& xis )
    : default_( &consumption )
    , dotation_( 0 )
    , nFullNbrDotation_( 0 )
    , unitType_( type )
{
    xis >> xml::optional
        >> xml::start( "resources" )
            // $$$$ _RC_ SBO 2009-06-11: Not a real list, only one resource
            >> xml::list( "resource", *this, &BuildableCapacity::ReadDotation )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BuildableCapacity::~BuildableCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::ReadDotation
// Created: JCR 2008-08-26
// -----------------------------------------------------------------------------
void BuildableCapacity::ReadDotation( xml::xistream& xis )
{
    if( dotation_ == 0 )
    {
        std::string dotation( xis.attribute< std::string >( "name" ) );
        dotation_ = PHY_DotationType::FindDotationCategory( dotation );
        if( !dotation_ )
            throw std::runtime_error( "Unknown dotation category - " + dotation + " - " );
        nFullNbrDotation_ = xis.attribute< int >( "count" );
    }
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BuildableCapacity::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    unsigned int consumptionId, dotationId;
    ar >> boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar >> consumptionId
       >> dotationId
       >> nFullNbrDotation_
       >> unitType_;
    default_  = PHY_ConsumptionType::FindConsumptionType( consumptionId );
    if( !default_ )
        throw std::runtime_error( __FUNCTION__ " Unknown consumption category" );
    dotation_ = PHY_DotationType::FindDotationCategory( dotationId );
    if( !dotation_ && dotationId )
        throw std::runtime_error( __FUNCTION__ " Unknown dotation category" );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::save
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BuildableCapacity::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar << (const unsigned int&)default_->GetID()
       << (const unsigned int&)( dotation_ ? dotation_->GetMosID() : 0 )
       << nFullNbrDotation_
       << unitType_;
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BuildableCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Finalize
// Created: LGY 2012-01-25
// -----------------------------------------------------------------------------
void BuildableCapacity::Finalize( MIL_Object_ABC& object )
{
    TER_Localisation location = object.GetLocalisation();
    if( dotation_ )
    {
        nFullNbrDotation_ = GetDotationNumber( location );
        if( unitType_ == ConstructionCapacity::eRaw )
            object.GetAttribute< ConstructionAttribute >() = ConstructionAttribute( *dotation_, nFullNbrDotation_, nFullNbrDotation_ / 1000.f );
        else if( unitType_ == ConstructionCapacity::eDensity )
            object.GetAttribute< ConstructionAttribute >() = ConstructionAttribute( *dotation_, nFullNbrDotation_, nFullNbrDotation_ / 1000000.f );
    }
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::GetDotationNumber
// Created: LMT 2012-01-25
// -----------------------------------------------------------------------------
unsigned int BuildableCapacity::GetDotationNumber( const TER_Localisation& location ) const
{
    if( dotation_ )
    {
        if( location.GetType() == TER_Localisation::ePoint )
            return std::max( 1u, nFullNbrDotation_ );
        else if( unitType_ == ConstructionCapacity::eRaw )
            return std::max( 1u, nFullNbrDotation_ * static_cast< unsigned int >( MIL_Tools::ConvertSimToMeter( location.GetLength() ) / 1000.f ) );
        else if( unitType_ == ConstructionCapacity::eDensity )
            return std::max( 1u, nFullNbrDotation_ * static_cast< unsigned int >( MIL_Tools::ConvertSimToMeter( location.GetArea() ) * nFullNbrDotation_ / 1000000.f ) );
    }
    return nFullNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void BuildableCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new BuildableCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::GetDotationCategory
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
const PHY_DotationCategory* BuildableCapacity::GetDotationCategory() const
{
    return dotation_;
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::GetDefaultConsumptionMode
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
const PHY_ConsumptionType& BuildableCapacity::GetDefaultConsumptionMode() const
{
    assert( default_ );
    return *default_;
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::GetUnit
// Created: JCR 2008-08-26
// -----------------------------------------------------------------------------
ConstructionCapacity::E_UnitType BuildableCapacity::GetUnit() const
{
    return unitType_;
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::GetMaxDotation
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
unsigned int BuildableCapacity::GetMaxDotation() const
{
    return nFullNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Construct
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void BuildableCapacity::Construct( MIL_Object_ABC& object )
{
    ChangeConstructionPercentage( object, 1. );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Construct
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void BuildableCapacity::Construct( MIL_Object_ABC& object, float rDeltaPercentage )
{
    object.GetAttribute< ConstructionAttribute >().Build( rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Destroy
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void BuildableCapacity::Destroy( MIL_Object_ABC& object, float rDeltaPercentage )
{
    object.GetAttribute< ConstructionAttribute >().Build( -rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Destroy
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void BuildableCapacity::Destroy( MIL_Object_ABC& object )
{
    ChangeConstructionPercentage( object, 0. );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::ChangeConstructionPercentage
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void BuildableCapacity::ChangeConstructionPercentage( MIL_Object_ABC& object, float rNewConstructionPercentage )
{
    if( object.IsMarkedForDestruction() )
        return;
    const float rDeltaPercentage = rNewConstructionPercentage - static_cast< float >( object.GetAttribute< ConstructionAttribute >().GetState() );
    if( rDeltaPercentage == 0 )
        return;
    if( rDeltaPercentage > 0 )
        Construct( object, rDeltaPercentage );
    else
        Destroy( object, -rDeltaPercentage );
}
