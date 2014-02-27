// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Displayer_ABC.h"
#include "Tools.h"
#include "Units.h"
#include "UrbanKnowledge_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormatterNotImplemented::Error
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void FormatterNotImplemented::Error( Displayer_ABC& displayer, const type_info& type ) const
{
    throw MASA_EXCEPTION( std::string( typeid( displayer ).name() ) + " can't display " + type.name() );
}

// -----------------------------------------------------------------------------
// Name: Formatter base types
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void Formatter< char >::operator()( const char& value, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( QString( QChar( value ) ) );
}

void Formatter< const char* >::operator()( const char* value, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( QString( value ) );
}

void Formatter< std::string >::operator()( const std::string& value, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( QString( value.c_str() ) );
}

void Formatter< QDateTime >::operator()( const QDateTime& value, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( value.toString( "dd/MM/yy HH:mm" ) );
}

void Formatter< Duration >::operator()( const Duration& value, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::DurationFromSeconds( value ) );
}

void Formatter< ValueNotSet >::operator()( const ValueNotSet& , Displayer_ABC& displayer ) const
{
    static const QString notSet = tools::translate( "Formatter", " - " );
    displayer.AddToDisplay( notSet );
}

void Formatter< Unit >::operator()( const Unit& value, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( value.AsString() );
}

void Formatter< kernel::UrbanKnowledge_ABC >::operator()( const kernel::UrbanKnowledge_ABC& value, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( QString( value.GetName() ) );
}

void Formatter< Separator >::operator()( const Separator& , Displayer_ABC& displayer ) const
{
    static const QString separator = tools::translate( "Formatter", ", " );
    displayer.AddToDisplay( separator );
}

// -----------------------------------------------------------------------------
// Name: Formatter enums
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void Formatter< E_UnitPosture >::operator()( const E_UnitPosture& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void Formatter< E_OperationalStatus >::operator()( const E_OperationalStatus& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void Formatter< E_ForceRatioStatus >::operator()( const E_ForceRatioStatus& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void Formatter< E_Roe >::operator()( const E_Roe& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void Formatter< E_PopulationRoe >::operator()( const E_PopulationRoe& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void Formatter< E_MeetingEngagementStatus >::operator()( const E_MeetingEngagementStatus& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void Formatter< E_FireAvailability >::operator()( const E_FireAvailability& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void Formatter< E_PerceptionResult >::operator()( const E_PerceptionResult& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void Formatter< E_NatureLevel >::operator()( const E_NatureLevel& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void Formatter< E_UnitExperience >::operator()( const E_UnitExperience& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void Formatter< E_UnitStress >::operator()( const E_UnitStress& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void Formatter< E_UnitMorale >::operator()( const E_UnitMorale& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void Formatter< E_UnitTiredness >::operator()( const E_UnitTiredness& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void Formatter< E_TroopHealthState >::operator()( const E_TroopHealthState& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void  Formatter< E_HumanWound >::operator()( const E_HumanWound& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void  Formatter< E_HumanRank >::operator()( const E_HumanRank& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( tools::ToString( e ) );
}

void  Formatter< sword::LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus >::operator()( const sword::LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( QString::fromStdString( ENT_Tr::ConvertFromLogSupplyHandlingStatus( e ) ) );
}

void  Formatter< sword::LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus >::operator()( const sword::LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( QString::fromStdString( ENT_Tr::ConvertFromLogMedicalHandlingStatus( e ) ) );
}

void  Formatter< sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus >::operator()( const sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( QString::fromStdString( ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( e ) ) );
}

void  Formatter< sword::LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus >::operator()( const sword::LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus& e, Displayer_ABC& displayer ) const
{
    displayer.AddToDisplay( QString::fromStdString( ENT_Tr::ConvertFromLogFuneralHandlingStatus( e ) ) );
}
