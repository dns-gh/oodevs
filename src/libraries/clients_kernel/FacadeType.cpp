// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "FacadeType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

unsigned int FacadeType::nNextID_ = 0;

// -----------------------------------------------------------------------------
// Name: FacadeType constructor
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
FacadeType::FacadeType( xml::xistream& xis )
    : id_( nNextID_++ )
    , name_( xis.attribute< std::string >( "name" ) )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: FacadeType destructor
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
FacadeType::~FacadeType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: FacadeType::GetName
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
const std::string& FacadeType::GetName() const 
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: FacadeType::GetId
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
unsigned int FacadeType::GetId() const
{
    return id_;
}
