// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "NBCAgent.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: NBCAgent constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
NBCAgent::NBCAgent( xml::xistream& input )
{
    input >> xml::attribute( "name", name_ )
          >> xml::attribute( "id", id_ );
}

// -----------------------------------------------------------------------------
// Name: NBCAgent destructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
NBCAgent::~NBCAgent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCAgent::GetId
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
unsigned long NBCAgent::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: NBCAgent::GetName
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
std::string NBCAgent::GetName() const
{
    return name_;
}
//
//// -----------------------------------------------------------------------------
//// Name: NBCAgent::Update
//// Created: ABR 2011-11-29
//// -----------------------------------------------------------------------------
//void NBCAgent::Update()
//{
//    controller_.Update( kernel::DictionaryUpdated( owner_, propertyName_ ) );
//}
