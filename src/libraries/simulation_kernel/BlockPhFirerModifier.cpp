// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "BlockPhFirerModifier.h"
#include "tools/Iterator.h"
#include <urban/BlockModel.h>
#include <urban/Block.h>
#include <urban/Architecture.h>


// -----------------------------------------------------------------------------
// Name: BlockPhFirerModifier constructor
// Created: SLG 2010-01-07
// -----------------------------------------------------------------------------
BlockPhFirerModifier::BlockPhFirerModifier()
{
    //TODO Lire les facteurs provenant de ADN
}

// -----------------------------------------------------------------------------
// Name: BlockPhFirerModifier destructor
// Created: SLG 2010-01-07
// -----------------------------------------------------------------------------
BlockPhFirerModifier::~BlockPhFirerModifier()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: BlockPhFirerModifier Execute
// Created: SLG 2010-01-07
// -----------------------------------------------------------------------------
float BlockPhFirerModifier::Execute( urban::BlockModel& blockModel, const geometry::Point2f& firerPosition )
{
    float modifier = 1;
    bool isInside = false;
    tools::Iterator< const urban::Block& > it = blockModel.CreateIterator();
    while( it.HasMoreElements() )
    {
        const urban::TerrainObject_ABC& block = it.NextElement();
        isInside = block.GetFootprint()->IsInside( firerPosition );
        if( isInside )
        {
            const urban::Architecture* architecture = block.RetrievePhysicalFeature< urban::Architecture >();
            if ( architecture )
                modifier += HeightModifier( *architecture ); 
        }
    }
    return modifier;
}

// -----------------------------------------------------------------------------
// Name: BlockPhFirerModifier::HeightModifier
// Created: SLG 2010-01-07
// -----------------------------------------------------------------------------
float BlockPhFirerModifier::HeightModifier( const urban::Architecture& architecture ) const 
{
    float modifier = architecture.GetHeight()*0.007; // $$$$ _RC_ SLG 2010-01-08:  les facteurs doivent etre paramétrable dans le nouvel ADN
    return modifier > 1.5 ? 1.5 : modifier;
}