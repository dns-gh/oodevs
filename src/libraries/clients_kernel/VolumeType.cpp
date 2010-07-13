// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "VolumeType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: VolumeType constructor
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
VolumeType::VolumeType( xml::xistream& input, unsigned int id )
    : id_( id )
{
    input >> xml::attribute( "name", name_ );
}

// -----------------------------------------------------------------------------
// Name: VolumeType destructor
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
VolumeType::~VolumeType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VolumeType::GetName
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
std::string VolumeType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: VolumeType::GetId
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
unsigned int VolumeType::GetId() const
{
    return id_;
}
