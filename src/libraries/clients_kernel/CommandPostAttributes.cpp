// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "CommandPostAttributes.h"
#include "CommunicationHierarchies.h"
#include "Karma.h"
#include "GlTools_ABC.h"
#include "Diplomacies_ABC.h"
#include "Viewport_ABC.h"
#include "AgentType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes constructor
// Created: LGY 2012-05-04
// -----------------------------------------------------------------------------
CommandPostAttributes::CommandPostAttributes( const Entity_ABC& entity, const AgentType& type, bool commandPost )
    : entity_     ( entity )
    , type_       ( type )
    , commandPost_( commandPost )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes destructor
// Created: LGY 2012-05-04
// -----------------------------------------------------------------------------
CommandPostAttributes::~CommandPostAttributes()
{
    // NOTHING
}

namespace
{
    std::string GetExtension( const kernel::Karma& karma )
    {
        if( karma == kernel::Karma::friend_ )
            return "F";
        else if( karma == kernel::Karma::enemy_ )
            return "H";
        else if( karma == kernel::Karma::neutral_ )
            return "N";
        return "F";
    }
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes::Draw
// Created: LGY 2012-05-04
// -----------------------------------------------------------------------------
void CommandPostAttributes::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( commandPost_ && viewport.IsHotpointVisible() )
        if( const CommunicationHierarchies* pHierarchy = entity_.Retrieve< CommunicationHierarchies >() )
            if( const Diplomacies_ABC* pDiplomacy = pHierarchy->GetTop().Retrieve< Diplomacies_ABC >() )
                tools.DrawApp6Symbol( type_.GetHQSymbol() + GetExtension( pDiplomacy->GetKarma() ), where, -1.f );
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes::IsCommandPost
// Created: LGY 2012-05-04
// -----------------------------------------------------------------------------
bool CommandPostAttributes::IsCommandPost() const
{
    return commandPost_;
}
