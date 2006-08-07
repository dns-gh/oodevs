// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "NBCAgent.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: NBCAgent constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
NBCAgent::NBCAgent( xistream& input )
{
    int id;
    input >> attribute( "nom", name_ )
          >> content( "MosID", id );
    id_ = id;
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
