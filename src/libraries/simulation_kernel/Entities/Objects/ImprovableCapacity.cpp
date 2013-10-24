// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ImprovableCapacity.h"
#include "MIL_Object_ABC.h"
#include "MineAttribute.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Tools/MIL_Tools.h"

BOOST_CLASS_EXPORT_IMPLEMENT( ImprovableCapacity )

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::ImprovableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ImprovableCapacity::ImprovableCapacity( const PHY_ConsumptionType& consumption, ConstructionCapacity::E_UnitType type, xml::xistream& xis )
    : default_( &consumption )
    , unitType_( type )
    , dotation_( 0 )
    , nFullNbrDotation_( 0 )
    , finalised_( false )
{
    xis >> xml::optional
        >> xml::start( "resources" )
            // $$$$ _RC_ SBO 2009-06-10: Not a real list, only allows one dotation
            >> xml::list( "resource", *this, &ImprovableCapacity::ReadDotation )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::ReadDotation
// Created: JCR 2008-08-26
// -----------------------------------------------------------------------------
void ImprovableCapacity::ReadDotation( xml::xistream& xis )
{
    if( dotation_ == 0 )
    {
        std::string dotation( xis.attribute< std::string >( "name" ) );
        dotation_ = PHY_DotationType::FindDotationCategory( dotation );
        if( !dotation_ )
            throw MASA_EXCEPTION( "Unknown dotation category - " + dotation + " - " );
        xis >> xml::attribute( "count", nFullNbrDotation_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::ImprovableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ImprovableCapacity::ImprovableCapacity()
    : default_( 0 )
    , unitType_( ConstructionCapacity::eRaw )
    , dotation_( 0 )
    , nFullNbrDotation_( 0 )
    , finalised_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ImprovableCapacity::ImprovableCapacity( const ImprovableCapacity& from )
    : default_( from.default_ )
    , unitType_( from.unitType_ )
    , dotation_( from.dotation_ )
    , nFullNbrDotation_( from.nFullNbrDotation_ )
    , finalised_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ImprovableCapacity::~ImprovableCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void ImprovableCapacity::load( MIL_CheckPointInArchive& ar, const unsigned int )
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
// Name: ImprovableCapacity::save
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void ImprovableCapacity::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar << (const unsigned int&)( default_ ? default_->GetID() : 0 )
       << dotation_
       << nFullNbrDotation_
       << unitType_
       << finalised_;
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void ImprovableCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::Finalize
// Created: LGY 2012-01-25
// -----------------------------------------------------------------------------
void ImprovableCapacity::Finalize( MIL_Object_ABC& object )
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
// Name: ImprovableCapacity::GetDotationNumber
// Created: LMT 2012-01-25
// -----------------------------------------------------------------------------
unsigned int ImprovableCapacity::GetDotationNumber( const TER_Localisation& location ) const
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
                return std::max( 1u, static_cast< unsigned int >( nFullNbrDotation_ * MIL_Tools::ConvertSimToMeter( location.GetArea() ) * nFullNbrDotation_ / 1000000.f ) );
        }
    }
    return nFullNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ImprovableCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new ImprovableCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::GetDotationCategory
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
const PHY_DotationCategory* ImprovableCapacity::GetDotationCategory() const
{
    return dotation_;
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::GetDefaultConsumptionMode
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
const PHY_ConsumptionType& ImprovableCapacity::GetDefaultConsumptionMode() const
{
    assert( default_ );
    return *default_;
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::GetUnit
// Created: JCR 2008-08-26
// -----------------------------------------------------------------------------
ConstructionCapacity::E_UnitType ImprovableCapacity::GetUnit() const
{
    return unitType_;
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::GetMaxDotation
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
unsigned int ImprovableCapacity::GetMaxDotation() const
{
    return nFullNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::Mine
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void ImprovableCapacity::Mine( MIL_Object_ABC& object )
{
    if( object.IsMarkedForDestruction() )
        return;
    const double rNewMiningPercentage = 1.;
    const double rDeltaPercentage = rNewMiningPercentage - object.GetAttribute< MineAttribute >().GetState();
    if( rDeltaPercentage == 0 )
        return;
    object.GetAttribute< MineAttribute >().Update( rDeltaPercentage );
}
