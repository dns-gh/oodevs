// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "VisionMeteoModel.h"
#include "MeteoModel.h"
#include "meteo/Meteo.h"

// -----------------------------------------------------------------------------
// Name: VisionMeteoModel constructor
// Created: LGY 2014-04-09
// -----------------------------------------------------------------------------
VisionMeteoModel::VisionMeteoModel( const MeteoModel& model )
    : global_( model.GetGlobalMeteo() )
{
    const auto& locals = model.GetLocalMeteos();
    for( auto it = locals.begin(); it != locals.end(); ++it )
        locals_.push_back( **it );
}

// -----------------------------------------------------------------------------
// Name: VisionMeteoModel destructor
// Created: LGY 2014-04-09
// -----------------------------------------------------------------------------
VisionMeteoModel::~VisionMeteoModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VisionMeteoModel::GetMeteo
// Created: LGY 2014-04-09
// -----------------------------------------------------------------------------
const weather::Meteo& VisionMeteoModel::GetMeteo( const geometry::Point2f& point ) const
{
    for( auto rit = locals_.rbegin(); rit != locals_.rend(); ++rit )
        if( rit->IsInside( point ) )
            return *rit;
    return global_;
}
