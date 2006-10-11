// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "SupplyStates.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DataDictionary.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SupplyStates constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
SupplyStates::SupplyStates( Controller& controller, const Resolver_ABC< EquipmentType >& resolver, const Resolver_ABC< DotationType >& dotationResolver, DataDictionary& dico )
    : controller_( controller )
    , resolver_( resolver )
    , dotationResolver_( dotationResolver )
{
    dico.Register( tools::translate( "Logistique", "Chaine ravitaillement/Chaine activ�e" ), bChainEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyStates destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
SupplyStates::~SupplyStates()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void SupplyStates::DoUpdate( const ASN1T_MsgLogRavitaillementEtat& message )
{
    if( message.m.chaine_activeePresent )
        bChainEnabled_ = message.chaine_activee;

    if( message.m.disponibilites_transporteurs_convoisPresent )
    {
        dispoTransporters_.resize( message.disponibilites_transporteurs_convois.n );
        for( uint i = 0; i < message.disponibilites_transporteurs_convois.n; ++i )
            dispoTransporters_[i] = Availability( resolver_, message.disponibilites_transporteurs_convois.elem[i] );
    }

    if( message.m.stocksPresent )
    {
        uint nSize = message.stocks.n;
        while( nSize > 0 )
        {
            ASN1T_DotationStock& value = message.stocks.elem[ --nSize ];

            DotationType& type = dotationResolver_.Get( value.ressource_id );
            Dotation* dotation = Find( value.ressource_id );
            if( dotation )
                dotation->quantity_ = value.quantite_disponible;
            else
                Register( value.ressource_id, *new Dotation( type, value.quantite_disponible ) );
        }
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void SupplyStates::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Logistique", "Etat chaine rav." ) )
                .Display( tools::translate( "Logistique", "Etat chaine" ), 
                    bChainEnabled_ ? tools::translate( "Logistique", "Activ�e" )
                                   : tools::translate( "Logistique", "D�sactiv�e" ) );
}
