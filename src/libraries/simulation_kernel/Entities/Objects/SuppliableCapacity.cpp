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
    ar >> boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar >> default_;
    ar >> dotation_;
    ar >> nFullNbrDotation_;
    ar >> unitType_;
    ar >> finalised_;
}

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity::save
// Created: LDC 2014-05-06
// -----------------------------------------------------------------------------
void SuppliableCapacity::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar << default_;
    ar << dotation_;
    ar << nFullNbrDotation_;
    ar << unitType_;
    ar << finalised_;
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

namespace
{
    double GetDensityFactor( const TER_Localisation& location )
    {
        // $$$$ JSR nFullNbrDotation_ is in number per 1km (linear) or 1km² (surfacic)
        // density is in number per 100m (linear) or 100m² (surfacic)
        // We multiply density by 10 if linear (100m->1km), or by 10000 if surfacic(100m²->1km²).
        return location.GetType() == TER_Localisation::eLine ? 10. : 10000.;
        // $$$$ LDC Density of 0.1 = 1 mine per 100 square meter (from TTA 702 \\file1\Masa\simulation\service\projets\scipio\doc\V1.STAB\DOC MODELISATION\DOCTRINE et memento\GEN\Doctrine page 131)
    }
}

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity::SetDensity
// Created: LDC 2012-06-25
// -----------------------------------------------------------------------------
void SuppliableCapacity::SetDensity( double density, const TER_Localisation& location )
{
    const double factor = GetDensityFactor( location );
    nFullNbrDotation_ = static_cast< unsigned int >( density * factor ); 
}

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity::GetDensity
// Created: LDC 2014-09-10
// -----------------------------------------------------------------------------
double SuppliableCapacity::GetDensity( const TER_Localisation& location ) const
{
    return nFullNbrDotation_ / GetDensityFactor( location );
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
