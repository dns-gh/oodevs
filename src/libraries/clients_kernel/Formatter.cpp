// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Formatter.h"
#include "Tools.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormatterNotImplemented::Error
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void FormatterNotImplemented::Error( Displayer_ABC& displayer, const type_info& type ) const
{
    throw std::runtime_error( std::string( typeid( displayer ).name() ) + " can't display " + type.name() );
}

// -----------------------------------------------------------------------------
// Name: Formatter base types
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void Formatter< char >::operator()( const char& value, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( QString( QChar( value ) ) );
}

void Formatter< const char* >::operator()( const char* value, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( QString( value ) );
}

void Formatter< std::string >::operator()( const std::string& value, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( QString( value.c_str() ) );
}

void Formatter< ValueNotSet >::operator()( const ValueNotSet& , Displayer_ABC& displayer ) const {
    static const QString notSet = tools::translate( "Formatter", " - " );
    displayer.AddToDisplay( notSet );
}

void Formatter< Unit >::operator()( const Unit& value, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( (const QString&)value );
}

void Formatter< Separator >::operator()( const Separator& , Displayer_ABC& displayer ) const {
    static const QString separator = tools::translate( "Formatter", ", " );
    displayer.AddToDisplay( separator );
}

// -----------------------------------------------------------------------------
// Name: Formatter enums
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
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
    displayer.AddToDisplay( tools::ToString( e ) );
}

void Formatter< E_NatureLevel >::operator()( const E_NatureLevel& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromNatureLevel( e ) );
}

void Formatter< E_UnitExperience >::operator()( const E_UnitExperience& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromUnitExperience( e ) );
}

void Formatter< E_UnitMoral >::operator()( const E_UnitMoral& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromUnitMoral( e ) );
}

void Formatter< E_UnitFatigue >::operator()( const E_UnitFatigue& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( ENT_Tr::ConvertFromUnitFatigue( e ) );
}

void Formatter< E_TroopHealthState >::operator()( const E_TroopHealthState& e, Displayer_ABC& displayer ) const {
    displayer.AddToDisplay( tools::ToString( e ) );
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



