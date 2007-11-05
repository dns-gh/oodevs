// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterKarma.h"
#include "Diplomacies.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/IntelligenceHierarchies.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

namespace
{
    const Karma& ConvertToKarma( const ASN1T_EnumDiplomacy& asn )
    {
        switch( asn )
        {
        case EnumDiplomacy::ami:
            return Karma::friend_;
        case EnumDiplomacy::ennemi:
            return Karma::enemy_;
        case EnumDiplomacy::neutre:
            return Karma::neutral_;
        }
        return Karma::unknown_;
    }

    ASN1T_EnumDiplomacy ConvertToDiplomacy( const Karma& karma )
    {
        if( karma == Karma::friend_ )
            return EnumDiplomacy::ami;
        if( karma == Karma::enemy_ )
            return EnumDiplomacy::ennemi;
        if( karma == Karma::neutral_ )
            return EnumDiplomacy::neutre;
        return EnumDiplomacy::inconnu;
    }

    // Reverts diplomacy effect applied on karma => "diplomacy" karma
    const Karma& ComputeKarma( const Karma& karma, const Entity_ABC& parent )
    {
        const Karma& parentKarma = parent.Get< IntelligenceHierarchies >().GetTop().Get< Diplomacies >().GetKarma();
        if( parentKarma == karma )
            return Karma::friend_;
        if( parentKarma == !karma )
            return Karma::enemy_;
        return karma;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterKarma constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
ActionParameterKarma::ActionParameterKarma( const OrderParameter& parameter, const Karma& karma, const Entity_ABC& parent )
    : ActionParameter< QString >( parameter )
    , karma_( ComputeKarma( karma, parent ) )
{
    SetValue( karma_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterKarma constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
ActionParameterKarma::ActionParameterKarma( const OrderParameter& parameter, const ASN1T_EnumDiplomacy& asn )
    : ActionParameter< QString >( parameter )
    , karma_( ConvertToKarma( asn ) )
{
    SetValue( karma_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterKarma constructor
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
ActionParameterKarma::ActionParameterKarma( const OrderParameter& parameter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
    , karma_( Karma::ResolveId( attribute< std::string >( xis, "value" ) ) )
{
    SetValue( karma_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterKarma destructor
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
ActionParameterKarma::~ActionParameterKarma()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterKarma::CommitTo
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
void ActionParameterKarma::CommitTo( ASN1T_EnumDiplomacy& asn ) const
{
    asn = ConvertToDiplomacy( karma_ );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterKarma::Serialize
// Created: SBO 2007-10-31
// -----------------------------------------------------------------------------
void ActionParameterKarma::Serialize( xml::xostream& xos ) const
{
    ActionParameter< QString >::Serialize( xos );
    xos << attribute( "value", karma_.GetId() );
}
