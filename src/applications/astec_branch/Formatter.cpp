// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Formatter.h"
#include "Tools.h"

// -----------------------------------------------------------------------------
// Name: FormatterNotImplemented::Error
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void FormatterNotImplemented::Error( Displayer_ABC& displayer, const type_info& type ) const
{
    throw std::runtime_error( std::string( typeid( displayer ).name() ) + " can't display " + type.name() );
}

void Formatter< E_UnitPosture >::operator()( const E_UnitPosture& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromUnitPosture( e ) );
}

void Formatter< E_EtatOperationnel >::operator()( const E_EtatOperationnel& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromEtatOperationnel( e ) );
}

void Formatter< E_EtatRapFor >::operator()( const E_EtatRapFor& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromEtatRapFor( e ) );
}

void Formatter< E_Roe >::operator()( const E_Roe& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromRoe( e ) );
}

void Formatter< E_RoePopulation >::operator()( const E_RoePopulation& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromRoePopulation( e ) );
}

void Formatter< E_EtatCombatRencontre >::operator()( const E_EtatCombatRencontre& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromEtatCombatRencontre( e ) );
}

void Formatter< E_DisponibiliteAuTir >::operator()( const E_DisponibiliteAuTir& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromDisponibiliteAuTir( e ) );
}

void Formatter< E_PerceptionResult >::operator()( const E_PerceptionResult& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( Tools::ToString( e ) );
}

void Formatter< E_NatureLevel >::operator()( const E_NatureLevel& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromNatureLevel( e ) );
}

void Formatter< E_UnitNatureWeapon >::operator()( const E_UnitNatureWeapon& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromUnitNatureWeapon( e ) );
}

void Formatter< E_UnitNatureSpecialization >::operator()( const E_UnitNatureSpecialization& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromUnitNatureSpecialization( e ) );
}

void Formatter< E_UnitNatureQualifier >::operator()( const E_UnitNatureQualifier& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromUnitNatureQualifier( e ) );
}

void Formatter< E_UnitNatureCategory >::operator()( const E_UnitNatureCategory& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromUnitNatureCategory( e ) );
}

void Formatter< E_UnitNatureMobility >::operator()( const E_UnitNatureMobility& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromUnitNatureMobility( e ) );
}

void Formatter< E_UnitCapaciteMission >::operator()( const E_UnitCapaciteMission& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromUnitCapaciteMission( e ) );
}

void Formatter< E_TroopHealthState >::operator()( const E_TroopHealthState& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( Tools::ToString( e ) );
}

void  Formatter< E_HumanWound >::operator()( const E_HumanWound& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromHumanWound( e ) );
}

void  Formatter< E_LogRavitaillementTraitementEtat >::operator()( const E_LogRavitaillementTraitementEtat& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromLogRavitaillementTraitementEtat( e ) );
}

void  Formatter< E_LogSanteTraitementEtat >::operator()( const E_LogSanteTraitementEtat& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromLogSanteTraitementEtat( e ) );
}

void  Formatter< E_LogMaintenanceTraitementEtat >::operator()( const E_LogMaintenanceTraitementEtat& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromLogMaintenanceTraitementEtat( e ) );
}



