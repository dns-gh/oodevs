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
#include "Object.h"
#include "ConstructionAttribute.h"

#include "MIL_AgentServer.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"

#include "Entities\Agents\Units\Dotations\PHY_DotationType.h"
#include "Entities\Agents\Units\Dotations\PHY_DotationCategory.h"
#include "Entities\Agents\Units\Dotations\PHY_ConsumptionType.h"

#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/DEC_Knowledge_Object.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( BuildableCapacity, "BuildableCapacity" )

// -----------------------------------------------------------------------------
// Name: BuildableCapacity constructor
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
BuildableCapacity::BuildableCapacity( const PHY_ConsumptionType& consumption, ConstructionCapacity::E_UnitType type, xml::xistream& xis )
    : default_ ( &consumption )
    , unitType_ ( type )
    , dotation_ ( 0 )
    , nFullNbrDotation_ ( 0 )
{
    xis >> xml::optional()
        >> xml::start( "resources" )
            >> list( "dotation", *this, &BuildableCapacity::ReadDotation )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::ReadDotation
// Created: JCR 2008-08-26
// -----------------------------------------------------------------------------
void BuildableCapacity::ReadDotation( xml::xistream& xis )
{
    if( dotation_ == 0 )
    {
        std::string dotation( xml::attribute< std::string >( xis, "name" ) );
        dotation_ = PHY_DotationType::FindDotationCategory( dotation );
        if ( !dotation_ )
            throw std::runtime_error( "Unknown dotation category - " + dotation + " - " ); 
        nFullNbrDotation_ = xml::attribute< int >( xis, "count" );
    }
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::BuildableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BuildableCapacity::BuildableCapacity()
    : default_ ( 0 )    
    , unitType_ ( ConstructionCapacity::eRaw )
    , dotation_ ( 0 )
    , nFullNbrDotation_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BuildableCapacity::BuildableCapacity( const BuildableCapacity& from )
    : default_ ( from.default_ )
    , unitType_ ( from.unitType_ )
    , dotation_ ( from.dotation_ )
    , nFullNbrDotation_ ( from.nFullNbrDotation_ )
{
    // NOTHING
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
// Name: BuildableCapacity::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BuildableCapacity::load( MIL_CheckPointInArchive& ar, const uint )
{
    std::string consumption, dotation;
    ar >> boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar >> consumption
       >> unitType_    
       >> dotation
       >> nFullNbrDotation_;
    default_ = PHY_ConsumptionType::FindConsumptionType( consumption );
    if ( !default_ )
        throw std::runtime_error( "Unknown consumption category - " + consumption + " - " ); 
    if ( dotation != "" )
    {
        dotation_ = PHY_DotationType::FindDotationCategory( dotation );
        if ( !dotation_ )
            throw std::runtime_error( "Unknown dotation category - " + dotation + " - " ); 
    }
}
    
// -----------------------------------------------------------------------------
// Name: BuildableCapacity::save
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BuildableCapacity::save( MIL_CheckPointOutArchive& ar, const uint ) const
{    
    ar << boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    ar << default_->GetName()
       << unitType_;
    if ( dotation_ )
       ar << dotation_->GetName();
    else
       ar << "";
    ar << nFullNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BuildableCapacity::Register( Object& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void BuildableCapacity::Instanciate( Object& object ) const
{
    object.AddCapacity( new BuildableCapacity( *this ) );
    if ( unitType_ == ConstructionCapacity::eRaw && dotation_ )
        object.GetAttribute< ConstructionAttribute >() = ConstructionAttribute( *dotation_, nFullNbrDotation_ );
    else if ( unitType_ == ConstructionCapacity::eDensity && dotation_ )
    {
        const TER_Localisation& location = object.GetLocalisation();
        object.GetAttribute< ConstructionAttribute >() = ConstructionAttribute( *dotation_, nFullNbrDotation_ * location.GetArea() );
    }
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
uint BuildableCapacity::GetMaxDotation() const
{
    return nFullNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Construct
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void BuildableCapacity::Construct( Object& object )
{
    ChangeConstructionPercentage( object, 1. );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Construct
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void BuildableCapacity::Construct( Object& object, float rDeltaPercentage )
{    
    object.GetAttribute< ConstructionAttribute >().Build( rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Destroy
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void BuildableCapacity::Destroy( Object& object, float rDeltaPercentage )
{    
    object.GetAttribute< ConstructionAttribute >().Build( -rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Destroy
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void BuildableCapacity::Destroy( Object& object )
{
    ChangeConstructionPercentage( object, 0. );

    object.MarkForDestruction();

    // All the knowledges associated to this object MUST be destroyed (for all the teams ..)
    const MIL_EntityManager::T_ArmyMap& armies = MIL_AgentServer::GetWorkspace().GetEntityManager().GetArmies();
    for( MIL_EntityManager::CIT_ArmyMap itArmy = armies.begin(); itArmy != armies.end(); ++itArmy )
        itArmy->second->GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( object );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::ChangeConstructionPercentage
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void BuildableCapacity::ChangeConstructionPercentage( Object& object, float rNewConstructionPercentage )
{
    if( object.IsMarkedForDestruction() )
        return;

    const MT_Float rDeltaPercentage = rNewConstructionPercentage - object.GetAttribute< ConstructionAttribute >().GetState();
    if( rDeltaPercentage == 0. )
        return;
    if( rDeltaPercentage > 0. )
        Construct( object, rDeltaPercentage );
    else    
        Destroy( object, -rDeltaPercentage );    
}
