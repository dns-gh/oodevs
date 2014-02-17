#include "adaptation_app_pch.h"
#include "ADN_AttritionEffectOnHuman.h"
#include "ADN_tr.h"

using namespace helpers;

// -----------------------------------------------------------------------------
// Name: AttritionEffectOnHuman::AttritionEffectOnHuman
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
AttritionEffectOnHuman::AttritionEffectOnHuman()
:  nInjuredPercentage_( 0 )
,  nDeadPercentage_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttritionEffectOnHuman::GetItemName
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
std::string AttritionEffectOnHuman::GetItemName()
{
    return "";
}

// -----------------------------------------------------------------------------
// Name: AttritionEffectOnHuman::CreateCopy
// Created: ABR 2014-02-17
// -----------------------------------------------------------------------------
AttritionEffectOnHuman* AttritionEffectOnHuman::CreateCopy()
{
    AttritionEffectOnHuman* pCopy = new AttritionEffectOnHuman();
    pCopy->strName_ = strName_.GetData();
    pCopy->nEquipmentState_ = nEquipmentState_.GetData();
    pCopy->nInjuredPercentage_ = nInjuredPercentage_.GetData();
    pCopy->nDeadPercentage_ = nDeadPercentage_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: AttritionEffectOnHuman::ReadArchive
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
void AttritionEffectOnHuman::ReadArchive( xml::xistream& input )
{
    std::string equipment;
    input >> xml::attribute( "equipment-state", equipment )
        >> xml::attribute( "injured-percentage", nInjuredPercentage_ )
        >> xml::attribute( "dead-percentage", nDeadPercentage_ );
    nEquipmentState_ = ADN_Tr::ConvertToEquipmentState_ADN( equipment );
    if( nEquipmentState_ == E_EquipmentState_ADN( -1 ) )
        throw MASA_EXCEPTION( tools::translate( "AttritionEffectOnHuman", "Categories - Invalid equipment state '%1'" ).arg( equipment.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: AttritionEffectOnHuman::WriteArchive
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
void AttritionEffectOnHuman::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "attrition-effect" )
        << xml::attribute( "equipment-state", ADN_Tr::ConvertFromEquipmentState_ADN( nEquipmentState_.GetData() ) )
        << xml::attribute( "injured-percentage", nInjuredPercentage_ )
        << xml::attribute( "dead-percentage", nDeadPercentage_ )
        << xml::end;
}

