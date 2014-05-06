// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SuppliableCapacity.h"
#include "MIL_Object_ABC.h"
#include "MineAttribute.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Tools/MIL_Tools.h"
#include "simulation_terrain/TER_Localisation.h"

BOOST_CLASS_EXPORT_IMPLEMENT( SuppliableCapacity )

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity constructor
// Created: LDC 2014-05-06
// -----------------------------------------------------------------------------
SuppliableCapacity::SuppliableCapacity()
    : default_( 0 )
    , unitType_( ConstructionCapacity::eRaw )
    , dotation_( 0 )
    , nFullNbrDotation_( 0 )
    , finalised_( false )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity constructor
// Created: LDC 2014-05-06
// -----------------------------------------------------------------------------
SuppliableCapacity::SuppliableCapacity( const SuppliableCapacity& from )
    : default_( from.default_ )
    , unitType_( from.unitType_ )
    , dotation_( from.dotation_ )
    , nFullNbrDotation_( from.nFullNbrDotation_ )
    , finalised_( false )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity constructor
// Created: LDC 2014-05-06
// -----------------------------------------------------------------------------
SuppliableCapacity::SuppliableCapacity( const PHY_ConsumptionType& consumption, ConstructionCapacity::E_UnitType type, xml::xistream& xis )
    : default_( &consumption )
    , dotation_( 0 )
    , nFullNbrDotation_( 0 )
    , unitType_( type )
    , finalised_( false )
{
    xis >> xml::optional
        >> xml::start( "resources" )
            // $$$$ _RC_ SBO 2009-06-11: Not a real list, only one resource
            >> xml::list( "resource", *this, &SuppliableCapacity::ReadDotation )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity destructor
// Created: LDC 2014-05-06
// -----------------------------------------------------------------------------
SuppliableCapacity::~SuppliableCapacity()
{
        // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: SuppliableCapacity::load
// Created: LDC 2014-05-06
// -----------------------------------------------------------------------------
void SuppliableCapacity::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    unsigned int consumptionId;
    ar >> boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar >> consumptionId
       >> dotation_
       >> nFullNbrDotation_
       >> unitType_
       >> finalised_;
    default_  = PHY_ConsumptionType::FindConsumptionType( consumptionId );
    if( !default_ )
        throw MASA_EXCEPTION( "Unknown consumption category." );
}

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity::save
// Created: LDC 2014-05-06
// -----------------------------------------------------------------------------
void SuppliableCapacity::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar << (const unsigned int&)( default_ ? default_->GetID() : 0 )
       << dotation_
       << nFullNbrDotation_
       << unitType_
       << finalised_;
}

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity::GetDotationNumber
// Created: LMT 2012-01-25
// -----------------------------------------------------------------------------
unsigned int SuppliableCapacity::GetDotationNumber( const TER_Localisation& location ) const
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
// Name: SuppliableCapacity::Finalize
// Created: LGY 2012-01-25
// -----------------------------------------------------------------------------
void SuppliableCapacity::Finalize( MIL_Object_ABC& object )
{
    if( dotation_ )
    {
        TER_Localisation localisation = object.GetLocalisation();
        nFullNbrDotation_ = GetDotationNumber( localisation );
        object.GetAttribute< MineAttribute >().SetMaxDotations( *dotation_, nFullNbrDotation_ );
    }
    finalised_ = true; // $$$$ LDC FIXME the nFullNbrDotation_ should be in the object or attribute not the capacity
}

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity::SetDensity
// Created: LDC 2012-06-25
// -----------------------------------------------------------------------------
void SuppliableCapacity::SetDensity( double density )
{
    nFullNbrDotation_ = static_cast< unsigned int >( density * 10000 ); // $$$$ LDC Density of 0.1 = 1 mine per 100 square meter (from TTA 702 \\install\Masa\simulation\projects\scipio\doc\V1.STAB\DOC MODELISATION\DOCTRINE et memento\GEN\Doctrine page 131)
}

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity::ReadDotation
// Created: JCR 2008-08-26
// -----------------------------------------------------------------------------
void SuppliableCapacity::ReadDotation( xml::xistream& xis )
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
// Name: SuppliableCapacity::GetDotationCategory
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
const PHY_DotationCategory* SuppliableCapacity::GetDotationCategory() const
{
    return dotation_;
}

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity::GetDefaultConsumptionMode
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
const PHY_ConsumptionType& SuppliableCapacity::GetDefaultConsumptionMode() const
{
    assert( default_ );
    return *default_;
}

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity::GetUnit
// Created: JCR 2008-08-26
// -----------------------------------------------------------------------------
ConstructionCapacity::E_UnitType SuppliableCapacity::GetUnit() const
{
    return unitType_;
}

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity::GetMaxDotation
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
unsigned int SuppliableCapacity::GetMaxDotation() const
{
    return nFullNbrDotation_;
}
