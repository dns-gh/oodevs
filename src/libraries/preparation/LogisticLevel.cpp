// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LogisticLevel.h"
#include "clients_kernel/LogisticLevel.h"

// -----------------------------------------------------------------------------
// Name: LogisticLevel constructor
// Created: HBD 2010-11-15
// -----------------------------------------------------------------------------
prepa::LogisticLevel::LogisticLevel()
{
    Register( "none" , kernel::LogisticLevel::none_ );
    Register( "logistic-base", kernel::LogisticLevel::logistic_base_ );
    Register( "tc2"  ,  kernel::LogisticLevel::tc2_ );}

// -----------------------------------------------------------------------------
// Name: LogisticLevel destructor
// Created: HBD 2010-11-15
// -----------------------------------------------------------------------------
prepa::LogisticLevel::~LogisticLevel()
{
    // NOTHING
}


