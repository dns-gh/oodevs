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
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: NBCAgent constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
NBCAgent::NBCAgent( xistream& input )
{
    int id;
    std::string name;
    input >> attribute( "nom", name )
          >> content( "MosID", id );
    id_ = id;
    name_ = name.c_str();
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
QString NBCAgent::GetName() const
{
    return name_;
}
