// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UrbanDestructionCapacity.h"
#include "MIL_Object_ABC.h"
#include "PHY_ObjectExplosionFireResult.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Weapons/PHY_UrbanAttritionData.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/MIL_Army.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_ObjectManager.h"

BOOST_CLASS_EXPORT_IMPLEMENT( UrbanDestructionCapacity )

// -----------------------------------------------------------------------------
// Name: UrbanDestructionCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
UrbanDestructionCapacity::UrbanDestructionCapacity( xml::xistream& xis )
    : data_ ( new PHY_UrbanAttritionData( xis ) )
    , appliedOnce_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanDestructionCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
UrbanDestructionCapacity::UrbanDestructionCapacity()
    : appliedOnce_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanDestructionCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
UrbanDestructionCapacity::UrbanDestructionCapacity( const UrbanDestructionCapacity& from )
    : data_ ( new PHY_UrbanAttritionData( *from.data_ ) )
    , appliedOnce_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanDestructionCapacity constructor
// Created: JCR 2011-08-12
// -----------------------------------------------------------------------------
UrbanDestructionCapacity::UrbanDestructionCapacity( const PHY_UrbanAttritionData& data )
    : data_ ( new PHY_UrbanAttritionData( data ) )
    , appliedOnce_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanDestructionCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
UrbanDestructionCapacity::~UrbanDestructionCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanDestructionCapacity::operator=
// Created: JCR 2011-08-12
// -----------------------------------------------------------------------------
UrbanDestructionCapacity& UrbanDestructionCapacity::operator=( const UrbanDestructionCapacity& from )
{
    data_.reset( new PHY_UrbanAttritionData( *from.data_ ) );
    appliedOnce_ = from.appliedOnce_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: UrbanDestructionCapacity::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void UrbanDestructionCapacity::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >> boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    // TODO ar >> category_;

}

// -----------------------------------------------------------------------------
// Name: UrbanDestructionCapacity::save
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void UrbanDestructionCapacity::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    // TODO ar << category_;
}

// -----------------------------------------------------------------------------
// Name: UrbanDestructionCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void UrbanDestructionCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: UrbanDestructionCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void UrbanDestructionCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    UrbanDestructionCapacity* capacity = new UrbanDestructionCapacity( *this );
    object.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: UrbanDestructionCapacity::Update
// Created: JCR 2011-08-12
// -----------------------------------------------------------------------------
void UrbanDestructionCapacity::Update( MIL_Object_ABC& object, unsigned int /*time*/ )
{
    typedef std::vector< TER_Object_ABC* >::iterator iterator;

    if( !appliedOnce_ && data_.get() )
    {
        std::vector< TER_Object_ABC* > objects;
        TER_World::GetWorld().GetObjectManager().GetListWithinLocalisation( object.GetLocalisation(), objects );

        for( iterator it = objects.begin(); it != objects.end(); ++it )
        {
            MIL_Object_ABC* pObject = static_cast< MIL_Object_ABC* >( *it );
            const TER_Localisation& location = object.GetLocalisation();
            if( location.GetType() == TER_Localisation::ePoint )
            {
                if( pObject->GetLocalisation().IsInside( location.GetPoints().front() ) )
                    pObject->ApplyDestruction( location, *data_ );
            }
            else
                pObject->ApplyDestruction( location, *data_ );
        }
        appliedOnce_ = true;
    }
}
