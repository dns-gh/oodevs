// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "LogMedicalWaitingAttention.h"

using namespace extractors;

// -----------------------------------------------------------------------------
// Name: LogMedicalWaitingAttention constructor
// Created: FPO 2011-05-09
// -----------------------------------------------------------------------------
LogMedicalWaitingAttention::LogMedicalWaitingAttention()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogMedicalWaitingAttention constructor
// Created: FPO 2011-07-08
// -----------------------------------------------------------------------------
LogMedicalWaitingAttention::LogMedicalWaitingAttention( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogMedicalWaitingAttention destructor
// Created: FPO 2011-05-09
// -----------------------------------------------------------------------------
LogMedicalWaitingAttention::~LogMedicalWaitingAttention()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogMedicalWaitingAttention::Extract
// Created: FPO 2011-05-09
// -----------------------------------------------------------------------------
int LogMedicalWaitingAttention::Extract( const sword::SimToClient& wrapper )
{
    const sword::LogMedicalHandlingUpdate& logMedicalUpdate = wrapper.message().log_medical_handling_update();
    return logMedicalUpdate.state() == sword::LogMedicalHandlingUpdate::waiting_for_medical_attention ? 1 : 0;
}
