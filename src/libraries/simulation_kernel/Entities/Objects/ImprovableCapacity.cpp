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
#include "Entities\Agents\Units\Dotations\PHY_DotationType.h"
#include "Entities\Agents\Units\Dotations\PHY_DotationCategory.h"
#include "Entities\Agents\Units\Dotations\PHY_ConsumptionType.h"
#include "Tools/MIL_Tools.h"
#include <xeumeuleu/xml.hpp>

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
            throw std::runtime_error( "Unknown dotation category - " + dotation + " - " );
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
    unsigned int consumptionId, dotationId;
    ar >> boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar >> consumptionId
       >> dotationId
       >> nFullNbrDotation_;
    default_  = PHY_ConsumptionType::FindConsumptionType( consumptionId );
    if( !default_ )
        throw std::runtime_error( __FUNCTION__ " Unknown consumption category" );
    dotation_ = PHY_DotationType::FindDotationCategory( dotationId );
    if( !dotation_ && dotationId )
        throw std::runtime_error( __FUNCTION__ " Unknown dotation category" );
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::save
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void ImprovableCapacity::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar << (const unsigned int&)( default_ ? default_->GetID() : 0 )
       << (const unsigned int&)( dotation_ ? dotation_->GetMosID() : 0 )
       << nFullNbrDotation_;
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
    TER_Localisation localisation = object.GetLocalisation();
    if( unitType_ == ConstructionCapacity::eRaw && dotation_ )
    {
        nFullNbrDotation_ *= static_cast< unsigned int >( MIL_Tools::ConvertSimToMeter( localisation.GetLength() ) / 1000.f );
        object.GetAttribute< MineAttribute >() = MineAttribute( *dotation_, nFullNbrDotation_ );
    }
    else if( unitType_ == ConstructionCapacity::eDensity && dotation_ )
    {
        nFullNbrDotation_ = static_cast< unsigned int >( MIL_Tools::ConvertSimToMeter( localisation.GetArea() ) * nFullNbrDotation_ / 1000000.f );
        object.GetAttribute< MineAttribute >() = MineAttribute( *dotation_, nFullNbrDotation_ );
    }
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
