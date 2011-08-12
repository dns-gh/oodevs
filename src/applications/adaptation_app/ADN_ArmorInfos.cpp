#include "adaptation_app_pch.h"
#include "ADN_ArmorInfos.h"
#include "ADN_tr.h"
#include "ADN_DataException.h"

using namespace helpers;

// -----------------------------------------------------------------------------
// Name: ArmorInfos::ArmorInfos
// Created: APE 2004-11-09
// -----------------------------------------------------------------------------
ArmorInfos::ArmorInfos()
: strName_      ()
, neutralizationAverageTime_ ( "0s" )
, neutralizationVariance_    ( "0s" )
, rBreakdownEVA_  ( 0 )
, rBreakdownNEVA_ ( 0 )
{
    strName_.SetDataName( "le nom de la catégorie de blindage" );
    strName_.SetParentNode( *this );

}

// -----------------------------------------------------------------------------
// Name: CreateDefaultAttrition
// Created: HBD 2010-05-06
// -----------------------------------------------------------------------------
void ArmorInfos::CreateDefaultAttrition()
{
    AttritionEffectOnHuman* pNewEffect = new AttritionEffectOnHuman();
    pNewEffect->nEquipmentState_ = eEquipmentState_ADN_FixableWithEvac ;
    pNewEffect->strName_ = ADN_Tr::ConvertFromEquipmentState(eEquipmentState_ADN_FixableWithEvac );
    vAttritionEffects_.AddItem( pNewEffect );
    pNewEffect = new AttritionEffectOnHuman();
    pNewEffect->nEquipmentState_ = eEquipmentState_ADN_FixableInPlace ;
    pNewEffect->strName_ = ADN_Tr::ConvertFromEquipmentState(eEquipmentState_ADN_FixableInPlace );
    vAttritionEffects_.AddItem( pNewEffect );
    pNewEffect = new AttritionEffectOnHuman();
    pNewEffect->nEquipmentState_ =  eEquipmentState_ADN_Destroyed ;
    pNewEffect->strName_ = ADN_Tr::ConvertFromEquipmentState(eEquipmentState_ADN_Destroyed );
    vAttritionEffects_.AddItem( pNewEffect );
}

// -----------------------------------------------------------------------------
// Name: ArmorInfos::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ArmorInfos::ReadArchive( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "name", strName_ )
        >> xml::attribute( "type", type );
    nType_ = ADN_Tr::ConvertToProtectionType( type );
    if( nType_ == E_ProtectionType( -1 ) )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Categories - Invalid armor type '%1'" ).arg( type.c_str() ).ascii() );

    input >> xml::start( "neutralization" )
        >> xml::attribute( "average-time", neutralizationAverageTime_ )
        >> xml::attribute( "variance", neutralizationVariance_ )
        >> xml::end();

    if( nType_ != eProtectionType_Human )
    {
        input >> xml::start( "random-breakdown-probability" )
            >> xml::attribute( "eva", rBreakdownEVA_ )
            >> xml::attribute( "neva", rBreakdownNEVA_ )
            >> xml::end()
            >> xml::start( "attrition-effects" )
            >> xml::list( "attrition-effect", *this, &ArmorInfos::ReadAttrition )
            >> xml::end();
    }
}

// -----------------------------------------------------------------------------
// Name: ArmorInfos::ReadAttrition
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ArmorInfos::ReadAttrition( xml::xistream& input )
{
    AttritionEffectOnHuman* pNewEffect = new AttritionEffectOnHuman();
    pNewEffect->ReadArchive( input );
    vAttritionEffects_.AddItem( pNewEffect );
}

// -----------------------------------------------------------------------------
// Name: ArmorInfos::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ArmorInfos::WriteArchive( xml::xostream& output )
{
    if( strName_.GetData().empty() )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Categories - Duplicated armor type name" ).ascii() );

    if( nType_ == eProtectionType_Human )
    {
        rBreakdownEVA_  = 0.;
        rBreakdownNEVA_ = 0.;
    }

    std::string strData( strName_.GetData() );
    output << xml::start( "protection" )
        << xml::attribute( "name", trim( strData ) )
        << xml::attribute( "type", ADN_Tr::ConvertFromProtectionType( nType_.GetData() ) );

    output << xml::start( "neutralization" )
        << xml::attribute( "average-time", neutralizationAverageTime_ )
        << xml::attribute( "variance", neutralizationVariance_ )
        << xml::end();

    if( nType_ != eProtectionType_Human )
    {
        output << xml::start( "random-breakdown-probability" )
            << xml::attribute( "eva", rBreakdownEVA_ )
            << xml::attribute( "neva", rBreakdownNEVA_ )
            << xml::end()
            << xml::start( "attrition-effects" );
        for( IT_AttritionEffectOnHuman_Vector it = vAttritionEffects_.begin(); it != vAttritionEffects_.end(); ++it )
            (*it)->WriteArchive( output );
        output << xml::end();
    }
    output << xml::end();
}
