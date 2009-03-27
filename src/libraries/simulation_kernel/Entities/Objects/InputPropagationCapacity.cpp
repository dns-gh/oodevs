// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "InputPropagationCapacity.h"
#include "Object.h"
#include "ToxicAttribute_ABC.h"
#include "InputToxicCloudAttribute.h"
#include "simulation_terrain/TER_Localisation.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( InputPropagationCapacity, "InputPropagationCapacity" )

// -----------------------------------------------------------------------------
// Name: InputPropagationCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
InputPropagationCapacity::InputPropagationCapacity( xml::xistream& xis )
{
    xis >> xml::attribute( "model", model_ );
}

// -----------------------------------------------------------------------------
// Name: InputPropagationCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
InputPropagationCapacity::InputPropagationCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InputPropagationCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
InputPropagationCapacity::InputPropagationCapacity( const InputPropagationCapacity& from )
    : model_ ( from.model_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InputPropagationCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
InputPropagationCapacity::~InputPropagationCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InputPropagationCapacity::serialize
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void InputPropagationCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );        
    file & model_;
}

// -----------------------------------------------------------------------------
// Name: InputPropagationCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void InputPropagationCapacity::Register( Object& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: InputPropagationCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void InputPropagationCapacity::Instanciate( Object& object ) const
{    
    InputPropagationCapacity* capacity = new InputPropagationCapacity( *this );
    object.AddCapacity< PropagationCapacity_ABC >( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

namespace 
{
    void ConvertSimCoord( const TER_Localisation& location, T_PointVector& output )
    {        
        const T_PointVector& points = location.GetPoints();
        output.resize( points.size() );
        for ( int i = 0; i < points.size(); ++i )
            TER_World::GetWorld().MosToSimMgrsCoord( points[i].rX_, points[i].rY_, output[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: InputPropagationCapacity::Update
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
void InputPropagationCapacity::Update( Object& object, uint time )
{    
    ToxicAttribute_ABC* pAttr = object.RetrieveAttribute< ToxicAttribute_ABC >();
    if ( pAttr )
    {
        TER_Polygon   polygon;
        InputToxicCloudAttribute* pInput = static_cast< InputToxicCloudAttribute* >( pAttr );
        if ( pInput && pInput->Update( time, polygon ) )
            object.UpdateLocalisation( TER_Localisation( polygon ) );
    }
}
