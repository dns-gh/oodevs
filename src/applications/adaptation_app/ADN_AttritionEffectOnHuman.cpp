#include "adaptation_app_pch.h"
#include "ADN_AttritionEffectOnHuman.h"
#include "ADN_tr.h"
#include "ADN_DataException.h"

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
// Name: AttritionEffectOnHuman::ReadArchive
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
void AttritionEffectOnHuman::ReadArchive( xml::xistream& input )
{
    std::string equipment;
    input >> xml::attribute( "equipment-state", equipment )
        >> xml::attribute( "injured-percentage", nInjuredPercentage_ )
        >> xml::attribute( "dead-percentage", nDeadPercentage_ );
    nEquipmentState_ = ADN_Tr::ConvertToEquipmentState( equipment );
    if( nEquipmentState_ == E_EquipmentState_ADN( -1 ) )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Categories - Invalid equipment state '%1'" ).arg( equipment.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: AttritionEffectOnHuman::WriteArchive
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
void AttritionEffectOnHuman::WriteArchive( xml::xostream& output )
{
    output << xml::start( "attrition-effect" )
        << xml::attribute( "equipment-state", ADN_Tr::ConvertFromEquipmentState( nEquipmentState_.GetData() ) )
        << xml::attribute( "injured-percentage", nInjuredPercentage_ )
        << xml::attribute( "dead-percentage", nDeadPercentage_ )
        << xml::end();
}

