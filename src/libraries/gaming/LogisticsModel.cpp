// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "LogisticsModel.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"
#include "LogSupplyConsign.h"
#include "LogisticConsignFactory_ABC.h"

// -----------------------------------------------------------------------------
// Name: LogisticsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogisticsModel::LogisticsModel( LogisticConsignFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogisticsModel::~LogisticsModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void LogisticsModel::Purge()
{
    Resolver< LogMaintenanceConsign >::DeleteAll();
    Resolver< LogMedicalConsign >::DeleteAll();
    Resolver< LogSupplyConsign >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::Delete
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
template< typename Type >
void LogisticsModel::Delete( unsigned long id )
{
    delete Resolver< Type >::Find( id );
    Resolver< Type >::Remove( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::CreateMaintenanceConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::CreateMaintenanceConsign( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asnMsg )
{
    Resolver< LogMaintenanceConsign >::Register( asnMsg.oid_consigne, *factory_.CreateMaintenanceConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::GetMaintenanceConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogMaintenanceConsign& LogisticsModel::GetMaintenanceConsign( unsigned long id )
{
    return Resolver< LogMaintenanceConsign >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DeleteMaintenanceConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::DeleteMaintenanceConsign( unsigned long id )
{
    Delete< LogMaintenanceConsign >( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::CreateMedicalConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::CreateMedicalConsign( const ASN1T_MsgLogSanteTraitementHumainCreation& asnMsg )
{
    Resolver< LogMedicalConsign >::Register( asnMsg.oid_consigne, *factory_.CreateMedicalConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::GetMedicalConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogMedicalConsign& LogisticsModel::GetMedicalConsign( unsigned long id )
{
    return Resolver< LogMedicalConsign >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DeleteMedicalConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::DeleteMedicalConsign( unsigned long id )
{
    Delete< LogMedicalConsign >( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::CreateSupplyConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::CreateSupplyConsign( const ASN1T_MsgLogRavitaillementTraitementCreation& asnMsg )
{
    Resolver< LogSupplyConsign >::Register( asnMsg.oid_consigne, *factory_.CreateSupplyConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::GetSupplyConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogSupplyConsign& LogisticsModel::GetSupplyConsign( unsigned long id )
{
    return Resolver< LogSupplyConsign >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DeleteSupplyConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::DeleteSupplyConsign( unsigned long id )
{
    Delete< LogSupplyConsign >( id );
}
