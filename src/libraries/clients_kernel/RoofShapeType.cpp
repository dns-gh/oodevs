// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "RoofShapeType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

unsigned int RoofShapeType::nNextID_ = 0;

// -----------------------------------------------------------------------------
// Name: RoofShapeType constructor
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
RoofShapeType::RoofShapeType( xml::xistream& xis )
    : id_  ( nNextID_++ )
    , name_( xis.attribute< std::string >( "name" ) )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: RoofShapeType destructor
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
RoofShapeType::~RoofShapeType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: RoofShapeType::GetName
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
const std::string& RoofShapeType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: RoofShapeType::GetId
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
unsigned int RoofShapeType::GetId() const
{
    return id_;
}
