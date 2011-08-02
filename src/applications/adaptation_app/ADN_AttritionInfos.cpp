#include "adaptation_app_pch.h"
#include "ADN_AttritionInfos.h"
#include "ADN_Categories_Data.h"

using namespace helpers;

// -----------------------------------------------------------------------------
// Name: AttritionInfos::AttritionInfos
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
AttritionInfos::AttritionInfos( ArmorInfos* ptr )
    : ptrArmor_        ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos(), ptr )
    , rDestroy_        ( 0.0 )
    , rRepairWithEvac_ ( 0.0 )
    , rRepairNoEvac_   ( 0.0 )
{
    this->BindExistenceTo( &ptrArmor_ );
}

// -----------------------------------------------------------------------------
// Name: AttritionInfos::GetNodeName
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
std::string AttritionInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: AttritionInfos::GetItemName
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
std::string AttritionInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: AttritionInfos::CopyFrom
// Created: APE 2005-01-19
// -----------------------------------------------------------------------------
void AttritionInfos::CopyFrom( AttritionInfos& attritions )
{
    rDestroy_ = attritions.rDestroy_.GetData();
    rRepairNoEvac_ = attritions.rRepairNoEvac_.GetData();
    rRepairWithEvac_ = attritions.rRepairWithEvac_.GetData();
}

// -----------------------------------------------------------------------------
// Name: AttritionInfos::ReadArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void AttritionInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "destruction", rDestroy_ )
        >> xml::attribute( "repairable-with-evacuation", rRepairWithEvac_ )
        >> xml::attribute( "repairable-without-evacuation", rRepairNoEvac_ );
    rDestroy_        = rDestroy_.GetData() * 100.0;
    rRepairWithEvac_ = rRepairWithEvac_.GetData() * 100.0;
    rRepairNoEvac_   = rRepairNoEvac_.GetData() * 100.0;
}

// -----------------------------------------------------------------------------
// Name: AttritionInfos::WriteArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void AttritionInfos::WriteArchive( xml::xostream& output, const std::string& tag /* = "attrition"*/ )
{
    output << xml::start( tag )
        << xml::attribute( "protection", ptrArmor_.GetData()->strName_ )
        << xml::attribute( "destruction", rDestroy_.GetData() / 100.0 )
        << xml::attribute( "repairable-with-evacuation", rRepairWithEvac_.GetData() / 100.0 )
        << xml::attribute( "repairable-without-evacuation", rRepairNoEvac_.GetData() / 100.0 )
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: AttritionInfos::operator<<
// Created: SLG 2010-07-01
// -----------------------------------------------------------------------------
xml::xostream& AttritionInfos::operator<<( xml::xostream& xos )
{
    WriteArchive( xos );
    return xos;
}
