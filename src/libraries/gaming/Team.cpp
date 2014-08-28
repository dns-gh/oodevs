// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Team.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

namespace
{
    kernel::Karma MakeKarma( const sword::EnumDiplomacy& diplomacy )
    {
        switch( diplomacy )
        {
            case sword::friendly : return kernel::Karma::friend_;
            case sword::enemy  : return kernel::Karma::enemy_;
            case sword::neutral: return kernel::Karma::neutral_;
            case sword::unknown:
            default: return kernel::Karma::unknown_;
        }
    }

    QString MakeName( const std::string& name )
    {
        return name.empty() ? QString( tools::translate( "gaming::Team", "Army %1" ) ).arg( name.c_str() ) : name.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
Team::Team( const sword::PartyCreation& message,
            kernel::Controllers& controllers,
            const T_CanBeRenamedFunctor& canBeRenamedFunctor )
    : gui::Team( controllers, message.party().id(), QString( message.name().c_str() ), canBeRenamedFunctor )
    , karma_( MakeKarma( message.type() ) )
{
    AddExtension( *this );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
Team::~Team()
{
    Destroy();
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Team::GetKarma
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
const kernel::Karma& Team::GetKarma() const
{
    return karma_;
}

// -----------------------------------------------------------------------------
// Name: Team::DoUpdate
// Created: ABR 2014-08-28
// -----------------------------------------------------------------------------
void Team::DoUpdate( const sword::PartyUpdate& message )
{
    if( message.has_name() )
        name_ = QString::fromStdString( message.name() );
    Touch();
}
