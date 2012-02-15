#include "adaptation_app_pch.h"
#include "ADN_AttritionInfos.h"
#include "ADN_Categories_Data.h"
#include "ADN_WorkspaceElement.h"

using namespace helpers;

// -----------------------------------------------------------------------------
// Name: AttritionInfos::AttritionInfos
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
AttritionInfos::AttritionInfos( ADN_Armors_Data::ArmorInfos* ptr )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Armors_Data >( eArmors ).GetArmorsInfos(), ptr, true )
    , rDestroy_( 0 )
    , rRepairWithEvac_( 0 )
    , rRepairNoEvac_( 0 )
{
    // NOTHING
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
    rDestroy_ = rDestroy_.GetData() * 100;
    rRepairWithEvac_ = rRepairWithEvac_.GetData() * 100;
    rRepairNoEvac_ = rRepairNoEvac_.GetData() * 100;
}

// -----------------------------------------------------------------------------
// Name: AttritionInfos::WriteArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void AttritionInfos::WriteArchive( xml::xostream& output, const std::string& tag /* = "attrition"*/ ) const
{
    if( ptrArmor_.GetData()->GetType() != eProtectionType_Crowd )
    {
    output << xml::start( tag )
             << xml::attribute( "protection", *this )
             << xml::attribute( "destruction", rDestroy_.GetData() / 100.0 )
             << xml::attribute( "repairable-with-evacuation", rRepairWithEvac_.GetData() / 100.0 )
             << xml::attribute( "repairable-without-evacuation", rRepairNoEvac_.GetData() / 100.0 )
           << xml::end;
}
}

// -----------------------------------------------------------------------------
// Name: ADN_AttritionInfos::CreateCopy
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
AttritionInfos* AttritionInfos::CreateCopy()
{
    AttritionInfos* result = new AttritionInfos( GetCrossedElement() );
    result->CopyFrom( *this );
    return result;
}
