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
#include "clients_kernel/IntelligenceHierarchies.h"
#include <xeumeuleu/xml.h>

using namespace xml;
using namespace actions;
using namespace parameters;

namespace
{
    const kernel::Karma& ConvertToKarma( const ASN1T_EnumDiplomacy& asn )
    {
        switch( asn )
        {
        case EnumDiplomacy::ami:
            return kernel::Karma::friend_;
        case EnumDiplomacy::ennemi:
            return kernel::Karma::enemy_;
        case EnumDiplomacy::neutre:
            return kernel::Karma::neutral_;
        }
        return kernel::Karma::unknown_;
    }

    ASN1T_EnumDiplomacy ConvertToDiplomacy( const kernel::Karma& karma )
    {
        if( karma == kernel::Karma::friend_ )
            return EnumDiplomacy::ami;
        if( karma == kernel::Karma::enemy_ )
            return EnumDiplomacy::ennemi;
        if( karma == kernel::Karma::neutral_ )
            return EnumDiplomacy::neutre;
        return EnumDiplomacy::inconnu;
    }

    // Reverts diplomacy effect applied on karma => "diplomacy" karma
    const kernel::Karma& ComputeKarma( const kernel::Karma& karma, const kernel::Entity_ABC& parent )
    {
        const kernel::Karma parentKarma( static_cast< const kernel::Team_ABC& >( parent.Get< kernel::IntelligenceHierarchies >().GetTop() ) );
        return parentKarma.RelativeTo( karma );
    }
}

// -----------------------------------------------------------------------------
// Name: Karma constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
Karma::Karma( const kernel::OrderParameter& parameter, const kernel::Karma& karma, const kernel::Entity_ABC& parent )
    : Parameter< QString >( parameter )
    , karma_( ComputeKarma( karma, parent ) )
{
    SetValue( karma_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: Karma constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
Karma::Karma( const kernel::OrderParameter& parameter, const ASN1T_EnumDiplomacy& asn )
    : Parameter< QString >( parameter )
    , karma_( ConvertToKarma( asn ) )
{
    SetValue( karma_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: Karma constructor
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
Karma::Karma( const kernel::OrderParameter& parameter, xml::xistream& xis )
    : Parameter< QString >( parameter )
    , karma_( kernel::Karma::ResolveId( attribute< std::string >( xis, "value" ) ) )
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
void Karma::CommitTo( ASN1T_EnumDiplomacy& asn ) const
{
    asn = ConvertToDiplomacy( karma_ );
}

// -----------------------------------------------------------------------------
// Name: Karma::Serialize
// Created: SBO 2007-10-31
// -----------------------------------------------------------------------------
void Karma::Serialize( xml::xostream& xos ) const
{
    Parameter< QString >::Serialize( xos );
    xos << attribute( "value", karma_.GetId() );
}
