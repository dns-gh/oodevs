#include "adaptation_app_pch.h"
#include "ADN_LogisticSupplyClass.h"
#include "ADN_Categories_Data.h"
#include "ADN_tr.h"

using namespace helpers;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyClass::LogisticSupplyClass
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
LogisticSupplyClass::LogisticSupplyClass()
    : nId_( ADN_Categories_Data::GetNewIdentifier() )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyClass::LogisticSupplyClass
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
LogisticSupplyClass::LogisticSupplyClass( const std::string& name, int id )
    : ADN_RefWithName( name )
    , nId_( id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClass::CreateCopy
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
LogisticSupplyClass* LogisticSupplyClass::CreateCopy()
{
    LogisticSupplyClass* result = new LogisticSupplyClass();
    return result;
}
