// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Itinerary.h"
#include "protocol/Simulation.h"

DEC_Itinerary::DEC_Itinerary( const sword::Pathfind& path )
    : id_( path.id() )
{

}

DEC_Itinerary::~DEC_Itinerary()
{

}

uint32_t DEC_Itinerary::GetId() const
{
    return id_;
}
