// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Armors_Data.h"
#include "ADN_Enums.h"
#include "ADN_Project_Data.h"
#include "ADN_tr.h"

using namespace helpers;

// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data::ArmorInfos constructor
// Created: APE 2004-11-09
// -----------------------------------------------------------------------------
ADN_Armors_Data::ArmorInfos::ArmorInfos()
    : neutralizationAverageTime_ ( "0s" )
    , neutralizationVariance_    ( "0s" )
    , rBreakdownEVA_  ( 0 )
    , rBreakdownNEVA_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data::ArmorInfos::CreateDefaultAttrition
// Created: HBD 2010-05-06
// -----------------------------------------------------------------------------
void ADN_Armors_Data::ArmorInfos::CreateDefaultAttrition()
{
    AttritionEffectOnHuman* pNewEffect = new AttritionEffectOnHuman();
    pNewEffect->nEquipmentState_ = eEquipmentState_ADN_FixableWithEvac ;
    pNewEffect->strName_ = ADN_Tr::ConvertFromEquipmentState_ADN(eEquipmentState_ADN_FixableWithEvac );
    vAttritionEffects_.AddItem( pNewEffect );
    pNewEffect = new AttritionEffectOnHuman();
    pNewEffect->nEquipmentState_ = eEquipmentState_ADN_FixableInPlace ;
    pNewEffect->strName_ = ADN_Tr::ConvertFromEquipmentState_ADN(eEquipmentState_ADN_FixableInPlace );
    vAttritionEffects_.AddItem( pNewEffect );
    pNewEffect = new AttritionEffectOnHuman();
    pNewEffect->nEquipmentState_ =  eEquipmentState_ADN_Destroyed ;
    pNewEffect->strName_ = ADN_Tr::ConvertFromEquipmentState_ADN(eEquipmentState_ADN_Destroyed );
    vAttritionEffects_.AddItem( pNewEffect );
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data::ArmorInfos::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Armors_Data::ArmorInfos::ReadArchive( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "name", strName_ )
        >> xml::attribute( "type", type );
    nType_ = ADN_Tr::ConvertToProtectionType( type );
    if( nType_ == E_ProtectionType( -1 ) )
        throw MASA_EXCEPTION( tr( "Categories - Invalid armor type '%1'" ).arg( type.c_str() ).toStdString() );

    input >> xml::start( "neutralization" )
        >> xml::attribute( "average-time", neutralizationAverageTime_ )
        >> xml::attribute( "variance", neutralizationVariance_ )
        >> xml::end;

    if( nType_ != eProtectionType_Human )
    {
        input >> xml::start( "random-breakdown-probability" )
            >> xml::attribute( "eva", rBreakdownEVA_ )
            >> xml::attribute( "neva", rBreakdownNEVA_ )
            >> xml::end
            >> xml::start( "attrition-effects" )
            >> xml::list( "attrition-effect", *this, &ArmorInfos::ReadAttrition )
            >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data::ArmorInfos::ReadAttrition
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Armors_Data::ArmorInfos::ReadAttrition( xml::xistream& input )
{
    AttritionEffectOnHuman* pNewEffect = new AttritionEffectOnHuman();
    pNewEffect->ReadArchive( input );
    vAttritionEffects_.AddItem( pNewEffect );
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data::ArmorInfos::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Armors_Data::ArmorInfos::WriteArchive( xml::xostream& output )
{
    if( strName_.GetData().empty() )
        throw MASA_EXCEPTION( tr( "Categories - Duplicated armor type name" ).toStdString() );

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
        << xml::end;

    if( nType_ != eProtectionType_Human )
    {
        output << xml::start( "random-breakdown-probability" )
            << xml::attribute( "eva", rBreakdownEVA_ )
            << xml::attribute( "neva", rBreakdownNEVA_ )
            << xml::end
            << xml::start( "attrition-effects" );
        for( auto it = vAttritionEffects_.begin(); it != vAttritionEffects_.end(); ++it )
            (*it)->WriteArchive( output );
        output << xml::end;
    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ArmorInfos::CreateCopy
// Created: ABR 2012-04-24
// -----------------------------------------------------------------------------
ADN_Armors_Data::ArmorInfos* ADN_Armors_Data::ArmorInfos::CreateCopy()
{
    ArmorInfos* pCopy = new ArmorInfos();
    pCopy->nType_ = nType_.GetData();
    pCopy->neutralizationAverageTime_ = neutralizationAverageTime_.GetData();
    pCopy->neutralizationVariance_ = neutralizationVariance_.GetData();
    pCopy->rBreakdownEVA_ = rBreakdownEVA_.GetData();
    pCopy->rBreakdownNEVA_ = rBreakdownNEVA_.GetData();
    for( auto it = vAttritionEffects_.begin(); it != vAttritionEffects_.end(); ++it )
        pCopy->vAttritionEffects_.AddItem( *it );
    return pCopy;
}



// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data constructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_Armors_Data::ADN_Armors_Data()
    : ADN_Data_ABC( eCategories, eArmors )
{
    vArmors_.AddUniquenessChecker( eError, duplicateName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data destructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_Armors_Data::~ADN_Armors_Data()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data::FilesNeeded
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Armors_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szArmors_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data::Reset
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Armors_Data::Reset()
{
    vArmors_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data::ReadArchive
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Armors_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "protections" )
              >> xml::list( "protection", *this, &ADN_Armors_Data::ReadArmor )
          >> xml::end;
    vArmors_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data::ReadProtection
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Armors_Data::ReadArmor( xml::xistream& input )
{
    std::auto_ptr< ArmorInfos > spNewArmor( new ArmorInfos() );
    spNewArmor->ReadArchive( input );
    vArmors_.AddItem( spNewArmor.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data::WriteArchive
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Armors_Data::WriteArchive( xml::xostream& output )
{
    if( vArmors_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "protections" );
    ADN_Tools::AddSchema( output, "Armors" );
    for( T_ArmorInfos_Vector::const_iterator itArmor = vArmors_.begin(); itArmor != vArmors_.end(); ++itArmor )
        ( *itArmor )->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data::CheckDatabaseValidity
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Armors_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    if( vArmors_.size() == 0 )
        checker.AddError( eMissingArmor, "", eCategories );
}
