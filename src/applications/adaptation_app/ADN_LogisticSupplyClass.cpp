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
: id_ ( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyClass::LogisticSupplyClass
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
LogisticSupplyClass::LogisticSupplyClass( const std::string name, int id )
: strName_( name )
, id_ ( id )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyClass::SetId
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
void LogisticSupplyClass::SetId( int id )
{
    id_ = id;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyClass::GetId
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
int LogisticSupplyClass::GetId()
{
    return id_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClass::CreateCopy
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
LogisticSupplyClass* LogisticSupplyClass::CreateCopy()
{
    LogisticSupplyClass* result = new LogisticSupplyClass();
    result->SetId( ADN_Categories_Data::GetNewIdentifier() );
    return result;
}
