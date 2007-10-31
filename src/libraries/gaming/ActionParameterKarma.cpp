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
#include "clients_kernel/Karma.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

namespace
{
    const Karma& ResolveKarma( const ASN1T_EnumDiplomacy& asn )
    {
        // $$$$ SBO 2007-10-29: should use formation karma
        switch( asn )
        {
        case EnumDiplomacy::ami:
            return Karma::friend_;
        case EnumDiplomacy::ennemi:
            return Karma::enemy_;
        }
        return Karma::neutral_;
    }

    ASN1T_EnumDiplomacy ResolveDiplomacy( const Karma& karma )
    {
        // $$$$ SBO 2007-10-29: should use formation karma
        if( karma == Karma::friend_ )
            return EnumDiplomacy::ami;
        if( karma == Karma::enemy_ )
            return EnumDiplomacy::ennemi;
        return EnumDiplomacy::neutre;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterKarma constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
ActionParameterKarma::ActionParameterKarma( const kernel::OrderParameter& parameter, const kernel::Karma& karma )
    : ActionParameter< QString >( parameter, karma.GetName() )
    , karma_( karma )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterKarma constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
ActionParameterKarma::ActionParameterKarma( const kernel::OrderParameter& parameter, const ASN1T_EnumDiplomacy& asn )
    : ActionParameter< QString >( parameter )
    , karma_( ResolveKarma( asn ) )
{
    SetValue( karma_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterKarma constructor
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
ActionParameterKarma::ActionParameterKarma( const kernel::OrderParameter& parameter, xml::xistream& xis )
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
    asn = ResolveDiplomacy( karma_ );
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
