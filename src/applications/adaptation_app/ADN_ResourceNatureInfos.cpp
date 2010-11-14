#include "adaptation_app_pch.h"
#include "ADN_ResourceNatureInfos.h"
#include "ADN_tr.h"

using namespace helpers;

// -----------------------------------------------------------------------------
// Name: ResourceNatureInfos::ResourceNatureInfos
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
ResourceNatureInfos::ResourceNatureInfos()
: id_ ( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNatureInfos::ResourceNatureInfos
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
ResourceNatureInfos::ResourceNatureInfos( const std::string name, int id )
: ADN_Type_String( name )
, id_ ( id )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNatureInfos::SetId
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
void ResourceNatureInfos::SetId( int id )
{
    id_ = id;
}

// -----------------------------------------------------------------------------
// Name: ResourceNatureInfos::GetId
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
int ResourceNatureInfos::GetId()
{
    return id_.GetData();
}
