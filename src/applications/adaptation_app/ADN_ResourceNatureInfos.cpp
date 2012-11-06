#include "adaptation_app_pch.h"
#include "ADN_ResourceNatureInfos.h"
#include "ADN_Categories_Data.h"
#include "ADN_tr.h"

using namespace helpers;

// -----------------------------------------------------------------------------
// Name: ResourceNatureInfos::ResourceNatureInfos
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
ResourceNatureInfos::ResourceNatureInfos()
    : nId_( ADN_Categories_Data::GetNewIdentifier() )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNatureInfos::ResourceNatureInfos
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
ResourceNatureInfos::ResourceNatureInfos( const std::string& name, int id )
    : ADN_RefWithName( name )
    , nId_( id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNatureInfos::CreateCopy
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
ResourceNatureInfos* ResourceNatureInfos::CreateCopy()
{
    ResourceNatureInfos* result = new ResourceNatureInfos();
    return result;
}
