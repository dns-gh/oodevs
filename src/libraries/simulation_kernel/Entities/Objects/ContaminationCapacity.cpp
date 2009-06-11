// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ContaminationCapacity.h"
#include "Object.h"
#include "MIL_NbcAgentType.h"
#include "NBCAttribute.h"
#include "ToxicAttribute_ABC.h"
#include "MIL_ToxicEffectManipulator.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include <boost/ptr_container/serialize_ptr_vector.hpp>
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( ContaminationCapacity, "ContaminationCapacity" )

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
ContaminationCapacity::ContaminationCapacity() 
    : maxToxic_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
ContaminationCapacity::ContaminationCapacity( xml::xistream& xis )
    : type_ ( xml::attribute< std::string >( xis, "type" ) )
    , maxToxic_ ( xml::attribute< int >( xis, "max-toxic" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity constructor
// Created: JCR 2008-06-13
// -----------------------------------------------------------------------------
ContaminationCapacity::ContaminationCapacity( const ContaminationCapacity& from )
    : type_ ( from.type_ )
    , maxToxic_ ( from.maxToxic_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity destructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
ContaminationCapacity::~ContaminationCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void ContaminationCapacity::serialize
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
template< typename Archive > 
void ContaminationCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this )
         & type_
         & maxToxic_
         & decontaminatedZones_;    
}
    
// -----------------------------------------------------------------------------
// Name: ContaminationCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void ContaminationCapacity::Register( Object& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ContaminationCapacity::Instanciate( Object& object ) const
{    
    ContaminationCapacity* capacity = new ContaminationCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}
	
// -----------------------------------------------------------------------------
// Name: ContaminationCapacity::ProcessAgentInside
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void ContaminationCapacity::ProcessAgentInside( Object& object, MIL_Agent_ABC& agent )
{
    const MT_Vector2D& position = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const NBCAttribute* pNBC = object.RetrieveAttribute< NBCAttribute >();
    
    if( !IsInsideDecontaminatedZone( position ) && pNBC && pNBC->IsContaminating() )
    {
        const ToxicAttribute_ABC* pAttribute = object.RetrieveAttribute< ToxicAttribute_ABC >();
        if( pAttribute )
        {
            MIL_ToxicEffectManipulator contamination( pAttribute->GetContaminationEffect( *pNBC, position ) );
            agent.GetRole< PHY_RoleInterface_NBC >().Contaminate( contamination );
        }
        else
        {
            MIL_ToxicEffectManipulator contamination( object.GetAttribute< NBCAttribute >().GetNBCAgents(), 1 );
            agent.GetRole< PHY_RoleInterface_NBC >().Contaminate( contamination );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity::IsAgentInsideDecontaminatedZone
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
bool ContaminationCapacity::IsInsideDecontaminatedZone( const MT_Vector2D& position ) const
{    
    for( CIT_LocalisationVector it = decontaminatedZones_.begin(); it != decontaminatedZones_.end(); ++it )
        if( it->IsInside( position ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ContaminationCapacity::Decontaminate
// Created: JCR 2008-08-19
// -----------------------------------------------------------------------------
void ContaminationCapacity::DecontaminateZone( const TER_Localisation& zone )
{
    decontaminatedZones_.push_back( new TER_Localisation( zone ) );
}
