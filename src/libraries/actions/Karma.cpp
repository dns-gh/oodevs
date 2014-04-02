// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Karma.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;
using namespace parameters;

namespace
{
    const kernel::Karma& ConvertToKarma( const sword::EnumDiplomacy& diplomacy )
    {
        switch( diplomacy )
        {
        case sword::friendly:
            return kernel::Karma::friend_;
        case sword::enemy:
            return kernel::Karma::enemy_;
        case sword::neutral:
            return kernel::Karma::neutral_;
        default:
            return kernel::Karma::unknown_;
        }
    }

    sword::EnumDiplomacy ConvertToDiplomacy( const kernel::Karma& karma )
    {

        if (karma == kernel::Karma::friend_)
            return sword::friendly;
        else if (karma == kernel::Karma::enemy_)
            return sword::enemy;
        else if (karma == kernel::Karma::neutral_)
            return sword::neutral;
        else
            return sword::unknown;
    }
}

// -----------------------------------------------------------------------------
// Name: Karma constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
Karma::Karma( const kernel::OrderParameter& parameter, const kernel::Karma& karma )
    : Parameter< QString >( parameter )
    , karma_( karma )
{
    SetValue( karma_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: Karma constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
Karma::Karma( const kernel::OrderParameter& parameter, const sword::EnumDiplomacy& message )
    : Parameter< QString >( parameter )
    , karma_( ConvertToKarma( message ) )
{
    SetValue( karma_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: Karma destructor
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
Karma::~Karma()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Karma::CommitTo
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
void Karma::CommitTo( T_Setter setter ) const
{
    setter( ConvertToDiplomacy( karma_ ) );
}

// -----------------------------------------------------------------------------
// Name: Karma::Serialize
// Created: SBO 2007-10-31
// -----------------------------------------------------------------------------
void Karma::Serialize( xml::xostream& xos ) const
{
    Parameter< QString >::Serialize( xos );
    xos << xml::attribute( "value", karma_.GetId() );
}

// -----------------------------------------------------------------------------
// Name: Karma::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Karma::SerializeType() const
{
    return "karma";
}
