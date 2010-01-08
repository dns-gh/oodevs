// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "BlockPhTargetModifier.h"
#include "tools/Iterator.h"
#include <urban/BlockModel.h>
#include <urban/Block.h>
#include <urban/Architecture.h>
#include <urban/Soil.h>


// -----------------------------------------------------------------------------
// Name: BlockPhTargetModifier constructor
// Created: SLG 2010-01-07
// -----------------------------------------------------------------------------
BlockPhTargetModifier::BlockPhTargetModifier()
{
    //TODO Lire les facteurs provenant de ADN
}

// -----------------------------------------------------------------------------
// Name: BlockPhFirerModifier destructor
// Created: SLG 2010-01-07
// -----------------------------------------------------------------------------
BlockPhTargetModifier::~BlockPhTargetModifier()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: BlockPhTargetModifier Execute
// Created: SLG 2010-01-07
// -----------------------------------------------------------------------------
float BlockPhTargetModifier::Execute( urban::BlockModel& blockModel, const geometry::Point2f& targetPosition )
{
    float modifier = 1;
    bool isInside = false;
    tools::Iterator< const urban::Block& > it = blockModel.CreateIterator();
    while( it.HasMoreElements() )
    {
        const urban::TerrainObject_ABC& block = it.NextElement();
        isInside = block.GetFootprint()->IsInside( targetPosition );
        if( isInside )
        {
            const urban::Architecture* architecture = block.RetrievePhysicalFeature< urban::Architecture >();
            if ( architecture )
                modifier -= ArchitectureModifier( *architecture ); 
            const urban::Soil* soil = block.RetrievePhysicalFeature< urban::Soil >();
            if ( soil )
                modifier -= SoilModifier( *soil ); 
        }
    }
    return modifier;
}

// -----------------------------------------------------------------------------
// Name: BlockPhTargetModifier::ArchitectureModifier
// Created: SLG 2010-01-07
// -----------------------------------------------------------------------------
float BlockPhTargetModifier::ArchitectureModifier( const urban::Architecture& architecture ) const 
{
    float modifier = architecture.GetHeight()*0.005 + architecture.GetFacadeOpacity()*0.2;  // $$$$ _RC_ SLG 2010-01-08:  les facteurs doivent etre paramétrable dans le nouvel ADN
    return modifier > 0.25 ? 0.25 : modifier;

}

// -----------------------------------------------------------------------------
// Name: BlockPhTargetModifier::SoilModifier
// Created: SLG 2010-01-07
// -----------------------------------------------------------------------------
float BlockPhTargetModifier::SoilModifier( const urban::Soil& soil) const 
{
    float modifier = soil.GetOccupation() * 0.1; // $$$$ _RC_ SLG 2010-01-08:  les facteurs doivent etre paramétrable dans le nouvel ADN
    return modifier > 0.25 ? 0.25 : modifier;
}