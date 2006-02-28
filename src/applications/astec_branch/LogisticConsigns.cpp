// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LogisticConsigns.h"

// -----------------------------------------------------------------------------
// Name: LogisticConsigns constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogisticConsigns::LogisticConsigns()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogisticConsigns::~LogisticConsigns()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::AddConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::AddConsign( LogSupplyConsign& consign )
{
    requestedSupplies_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::RemoveConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::RemoveConsign( LogSupplyConsign& consign )
{
    requestedSupplies_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::AddConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::AddConsign( LogMaintenanceConsign& consign )
{
    requestedMaintenances_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::RemoveConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::RemoveConsign( LogMaintenanceConsign& consign )
{
    requestedMaintenances_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::AddConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::AddConsign( LogMedicalConsign& consign )
{
    requestedMedical_.insert( &consign );
}
    
// -----------------------------------------------------------------------------
// Name: LogisticConsigns::RemoveConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::RemoveConsign( LogMedicalConsign& consign )
{
    requestedMedical_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::HandleConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::HandleConsign( LogSupplyConsign& consign )
{
    handledSupplies_.insert( &consign );
}
    
// -----------------------------------------------------------------------------
// Name: LogisticConsigns::TerminateConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::TerminateConsign( LogSupplyConsign& consign )
{
    handledSupplies_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::HandleConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::HandleConsign( LogMaintenanceConsign& consign )
{
    handledMaintenances_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::TerminateConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::TerminateConsign( LogMaintenanceConsign& consign )
{
    handledMaintenances_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::HandleConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::HandleConsign( LogMedicalConsign& consign )
{
    handledMedical_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::TerminateConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::TerminateConsign( LogMedicalConsign& consign )
{
    handledMedical_.erase( &consign );
}
