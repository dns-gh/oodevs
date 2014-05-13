// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef LOGISTIC_CONSIGN_HELPER_H
#define LOGISTIC_CONSIGN_HELPER_H

namespace logistic
{

unsigned int NewConsignId();
unsigned int RegisterConsignId( unsigned int id );

enum MaintenanceConsignError
{
    already_resolved = 0,
    diagnosis_team_unavailable,
    repair_team_unavailable,
    transporter_unavailable
};

} // namespace logistic

#endif // LOGISTIC_CONSIGN_HELPER_H
