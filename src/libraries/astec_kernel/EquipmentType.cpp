// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_kernel_pch.h"
#include "EquipmentType.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: EquipmentType constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
EquipmentType::EquipmentType( xml::xistream& xis )
{
    int id;
    xis >> attribute( "nom", name_ )
        >> content( "MosID", id );
    id_ = id;
}

// -----------------------------------------------------------------------------
// Name: EquipmentType destructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
EquipmentType::~EquipmentType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentType::GetName
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
std::string EquipmentType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: EquipmentType::GetId
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
unsigned long EquipmentType::GetId() const
{
    return id_;
}
