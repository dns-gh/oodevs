// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "KnowledgeGroupTypes.h"
#include "KnowledgeGroupType.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupTypes constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
KnowledgeGroupTypes::KnowledgeGroupTypes()
    : default_()
{
    default_ = new KnowledgeGroupType( tools::translate( "KnowledgeGroupTypes", "Standard" ), "Standard" );
    Register( "Standard", *default_ );
    Register( "Alat", *new KnowledgeGroupType( tools::translate( "KnowledgeGroupTypes", "Alat" ), "Alat" ) );
}
    
// -----------------------------------------------------------------------------
// Name: KnowledgeGroupTypes destructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
KnowledgeGroupTypes::~KnowledgeGroupTypes()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupTypes::GetDefault
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
KnowledgeGroupType& KnowledgeGroupTypes::GetDefault() const
{
    return *default_;
}
