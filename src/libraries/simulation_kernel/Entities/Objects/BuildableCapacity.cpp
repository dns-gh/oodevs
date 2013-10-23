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
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Tools/MIL_Tools.h"

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
    , finalised_( false )
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
    , finalised_( false )
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
    , finalised_( false )
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
            throw MASA_EXCEPTION( "Unknown dotation category - " + dotation + " - " );
        nFullNbrDotation_ = xis.attribute< int >( "count" );
    }
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BuildableCapacity::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    unsigned int dotationId;
    ar >> boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar >> default_
       >> dotationId
       >> nFullNbrDotation_
       >> unitType_
       >> finalised_;
    dotation_ = PHY_DotationType::FindDotationCategory( dotationId );
    if( !dotation_ && dotationId )
        throw MASA_EXCEPTION( "Unknown dotation category." );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::save
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BuildableCapacity::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar << default_
       << (const unsigned int&)( dotation_ ? dotation_->GetMosID() : 0 )
       << nFullNbrDotation_
       << unitType_
       << finalised_;
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
            object.GetAttribute< ConstructionAttribute >().SetMaxDotations( *dotation_, nFullNbrDotation_ );
        else if( unitType_ == ConstructionCapacity::eDensity )
            object.GetAttribute< ConstructionAttribute >().SetMaxDotations( *dotation_, nFullNbrDotation_ );
    }
    finalised_ = true; // $$$$ LDC FIXME the nFullNbrDotation_ should be in the object or attribute not the capacity
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::GetDotationNumber
// Created: LMT 2012-01-25
// -----------------------------------------------------------------------------
unsigned int BuildableCapacity::GetDotationNumber( const TER_Localisation& location ) const
{
    if( finalised_ )
        return nFullNbrDotation_;
    if( dotation_ )
    {
        if( location.GetType() == TER_Localisation::ePoint )
            return std::max( 1u, nFullNbrDotation_ );
        else if( unitType_ == ConstructionCapacity::eRaw )
            return std::max( 1u, static_cast< unsigned int >( nFullNbrDotation_ ) );
        else if( unitType_ == ConstructionCapacity::eDensity )
        {
            if( location.GetType() == TER_Localisation::eLine )
                return std::max( 1u, static_cast< unsigned int >( nFullNbrDotation_ * MIL_Tools::ConvertSimToMeter( location.GetLength() ) / 1000.f ) );
            else
                return std::max( 1u, static_cast< unsigned int >( nFullNbrDotation_ * MIL_Tools::ConvertSimToMeter( location.GetArea() ) / 1000000.f ) );
        }
    }
    return nFullNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::SetDensity
// Created: LDC 2012-06-25
// -----------------------------------------------------------------------------
void BuildableCapacity::SetDensity( double density )
{
    nFullNbrDotation_ = static_cast< unsigned int >( density * 10000 ); // $$$$ LDC Density of 0.1 = 1 mine per 100 square meter (from TTA 702 \\install\Masa\simulation\projects\scipio\doc\V1.STAB\DOC MODELISATION\DOCTRINE et memento\GEN\Doctrine page 131)
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
    const float state = static_cast< float >( object.GetAttribute< ConstructionAttribute >().GetState() );
    if( state == rNewConstructionPercentage )
        return;
    if( rNewConstructionPercentage == 0 )
        Construct( object, -1 );
    else if( rNewConstructionPercentage == 1 )
        Construct( object, 1 );
    else
        Construct( object, rNewConstructionPercentage - state );
}
