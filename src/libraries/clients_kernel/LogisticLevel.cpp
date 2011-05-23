// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "LogisticLevel.h"
#include "protocol/Protocol.h"

namespace kernel
{

LogisticLevel LogisticLevel::none_( (int)sword::none, "none");
LogisticLevel LogisticLevel::logistic_base_( (int)sword::logistic_base, "logistic-base");

// -----------------------------------------------------------------------------
// Name: LogisticLevel constructor
// Created: AHC 2010-10-07
// -----------------------------------------------------------------------------
LogisticLevel::LogisticLevel(int id, const std::string& name)
    : id_(id)
    , name_( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticLevel destructor
// Created: AHC 2010-10-07
// -----------------------------------------------------------------------------
LogisticLevel::~LogisticLevel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticLevel::GetName
// Created: AHC 2010-10-07
// -----------------------------------------------------------------------------
const std::string& LogisticLevel::GetName() const
{
    return name_ ;
}

// -----------------------------------------------------------------------------
// Name: LogisticLevel::Resolve
// Created: AHC 2010-10-07
// -----------------------------------------------------------------------------
const LogisticLevel& LogisticLevel::Resolve( const std::string& name )
{
    if( name == logistic_base_.GetName() )
        return logistic_base_;
    return none_;
}

// -----------------------------------------------------------------------------
// Name: LogisticLevel::Resolve
// Created: AHC 2010-10-07
// -----------------------------------------------------------------------------
const LogisticLevel& LogisticLevel::Resolve( int id )
{
    if( id == logistic_base_.GetId() )
        return logistic_base_;
    return none_;
}

// -----------------------------------------------------------------------------
// Name: LogisticLevel::operator==
// Created: AHC 2010-10-07
// -----------------------------------------------------------------------------
bool LogisticLevel::operator==( const LogisticLevel& rhs ) const
{
    return rhs.id_ == id_;
}

// -----------------------------------------------------------------------------
// Name: LogisticLevel::operator!=
// Created: AHC 2010-10-07
// -----------------------------------------------------------------------------
bool LogisticLevel::operator!=( const LogisticLevel& rhs ) const
{
    return !(*this == rhs);
}

// -----------------------------------------------------------------------------
// Name: LogisticLevel::GetId
// Created: AHC 2010-10-07
// -----------------------------------------------------------------------------
int LogisticLevel::GetId() const
{
    return id_;
}

}
