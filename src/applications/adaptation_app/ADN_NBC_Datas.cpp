// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_NBC_Datas.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_DataException.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_Tools.h"

#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::NbcIntoxInfos
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
ADN_NBC_Datas::NbcIntoxInfos::NbcIntoxInfos( const std::string& nodeName )
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , nodeName_( nodeName )
    , bIntoxPresent_( false )
    , rNbAlivedHumans_( 100.0 )
    , rNbHurtedHumans1_( 0.0 )
    , rNbHurtedHumans2_( 0.0 )
    , rNbHurtedHumans3_( 0.0 )
    , rNbHurtedHumansE_( 0.0 )
    , rNbDeadHumans_( 0.0 )
    , bContaminationPresent_( false )
{
    rNbAlivedHumans_.SetDataName( "le pourcentage d'humains survivants dans" );
    rNbAlivedHumans_.SetParentNode( *this );
    rNbHurtedHumans1_.SetDataName( "le pourcentage d'humains blessés de niveau U1 dans" );
    rNbHurtedHumans1_.SetParentNode( *this );
    rNbHurtedHumans2_.SetDataName( "le pourcentage d'humains blessés de niveau U2 dans" );
    rNbHurtedHumans2_.SetParentNode( *this );
    rNbHurtedHumans3_.SetDataName( "le pourcentage d'humains blessés de niveau U3 dans" );
    rNbHurtedHumans3_.SetParentNode( *this );
    rNbHurtedHumansE_.SetDataName( "le pourcentage d'humains blessés de niveau UE dans" );
    rNbHurtedHumansE_.SetParentNode( *this );
    rNbDeadHumans_.SetDataName( "le pourcentage d'humains décédés dans" );
    rNbDeadHumans_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::GetNodeName
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
std::string ADN_NBC_Datas::NbcIntoxInfos::GetNodeName()
{
    return nodeName_;
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::GetItemName
// Created: SBO 2006-10-31
// -----------------------------------------------------------------------------
std::string ADN_NBC_Datas::NbcIntoxInfos::GetItemName()
{
    return nodeName_;
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::CopyFrom
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcIntoxInfos::CopyFrom( NbcIntoxInfos& infos )
{
    bIntoxPresent_ = infos.bIntoxPresent_.GetData();
    rNbAlivedHumans_ = infos.rNbAlivedHumans_.GetData();
    rNbHurtedHumans1_ = infos.rNbHurtedHumans1_.GetData();
    rNbHurtedHumans2_ = infos.rNbHurtedHumans2_.GetData();
    rNbHurtedHumans3_ = infos.rNbHurtedHumans3_.GetData();
    rNbHurtedHumansE_ = infos.rNbHurtedHumansE_.GetData();
    rNbDeadHumans_ = infos.rNbDeadHumans_.GetData();
    bContaminationPresent_ = infos.bContaminationPresent_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::ReadArchive
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcIntoxInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    bIntoxPresent_ = input.Section( "Intoxication", ADN_XmlInput_Helper::eNothing );
    if( bIntoxPresent_.GetData() )
    {
        double rTmp;
        // alive humans percentage
        input.ReadField( "Humains_NonBlesse", rTmp );
        if( rTmp < 0.0 || rTmp > 1.0 )
            throw ADN_DataException( "Donnée invalide",
            MT_FormatString( "Le pourcentage d'humains vivants lors d'une intoxication '%s' par l'agent NBC par '%s' est en dehors de l'intervalle [0;1] autorisé", GetNodeName().c_str(), GetParentNode()->GetNodeName().c_str() ).c_str(),
            MT_FormatString( "Veuillez modifier la valeur du champ NBC::AgentsNBC::AgentNBC::%s::Intoxication::Humains_NonBlesse", GetNodeName().c_str() ).c_str() );
        rNbAlivedHumans_ = rTmp * 100.0;

        // U1 hurted humans percentage
        input.ReadField( "Humains_U1", rTmp );
        if( rTmp < 0.0 || rTmp > 1.0 )
            throw ADN_DataException( "Donnée invalide",
            MT_FormatString( "Le pourcentage d'humains avec une blessure de niveau U1 lors d'une intoxication par '%s' par l'agent NBC '%s' est en dehors de l'intervalle [0;1] autorisé", GetNodeName().c_str(), GetParentNode()->GetNodeName().c_str() ).c_str(),
            MT_FormatString( "Veuillez modifier la valeur du champ NBC::AgentsNBC::AgentNBC::%s::Intoxication::Humains_BlessesU1", GetNodeName().c_str() ).c_str() );
        rNbHurtedHumans1_ = rTmp * 100.0;

        // U2 hurted humans percentage
        input.ReadField( "Humains_U2", rTmp );
        if( rTmp < 0.0 || rTmp > 1.0 )
            throw ADN_DataException( "Donnée invalide",
            MT_FormatString( "Le pourcentage d'humains avec une blessure de niveau U2 lors d'une intoxication par '%s' par l'agent NBC %s est en dehors de l'intervalle [0;1] autorisé", GetNodeName().c_str(), GetParentNode()->GetNodeName().c_str() ).c_str(),
            MT_FormatString( "Veuillez modifier la valeur du champ NBC::AgentsNBC::AgentNBC::%s::Intoxication::Humains_BlessesU2", GetNodeName().c_str() ).c_str() );
        rNbHurtedHumans2_ = rTmp * 100.0;

        // U3 hurted humans percentage
        input.ReadField( "Humains_U3", rTmp );
        if( rTmp < 0.0 || rTmp > 1.0 )
            throw ADN_DataException( "Donnée invalide",
            MT_FormatString( "Le pourcentage d'humains avec une blessure de niveau U3 lors d'une intoxication par '%s' par l'agent NBC '%s' est en dehors de l'intervalle [0;1] autorisé", GetNodeName().c_str(), GetParentNode()->GetNodeName().c_str() ).c_str(),
            MT_FormatString( "Veuillez modifier la valeur du champ NBC::AgentsNBC::AgentNBC::%s::Intoxication::Humains_BlessesU3", GetNodeName().c_str() ).c_str() );
        rNbHurtedHumans3_ = rTmp * 100.0;

        // U4 hurted humans percentage
        input.ReadField( "Humains_UE", rTmp );
        if( rTmp < 0.0 || rTmp > 1.0 )
            throw ADN_DataException( "Donnée invalide",
            MT_FormatString( "Le pourcentage d'humains avec une blessure de niveau UE lors d'une intoxication par '%s' par l'agent NBC '%s' est en dehors de l'intervalle [0;1] autorisé", GetNodeName().c_str(), GetParentNode()->GetNodeName().c_str() ).c_str(),
            MT_FormatString( "Veuillez modifier la valeur du champ NBC::AgentsNBC::AgentNBC::%s::Intoxication::Humains_BlessesUE", GetNodeName().c_str() ).c_str() );
        rNbHurtedHumansE_ = rTmp * 100.0;

        // dead humans percentage
        input.ReadField( "Humains_Mort", rTmp );
        if( rTmp < 0.0 || rTmp > 1.0 )
            throw ADN_DataException( "Donnée invalide",
            MT_FormatString( "Le pourcentage d'humains morts lors d'une intoxication par '%s' par l'agent NBC '%s' est en dehors de l'intervalle [0;1] autorisé", GetNodeName().c_str(), GetParentNode()->GetNodeName().c_str() ).c_str(),
            MT_FormatString( "Veuillez modifier la valeur du champ NBC::AgentsNBC::AgentNBC::%s::Intoxication::Humains_Morts", GetNodeName().c_str() ).c_str() );
        rNbDeadHumans_ = rTmp * 100.0;

        // check the data. The sum must be equal to 100
        if( rNbAlivedHumans_.GetData() + rNbHurtedHumans1_.GetData() + rNbHurtedHumans2_.GetData() + rNbHurtedHumans3_.GetData() + rNbHurtedHumansE_.GetData() + rNbDeadHumans_.GetData() != 100.0 )
            throw ADN_DataException( "Incohérence des données",
            MT_FormatString( "La répartition de l'intoxication '%s' sur les humains de l'agent NBC '%s' ne couvre pas tous les cas possibles (soit 100%).", GetNodeName().c_str(), GetParentNode()->GetNodeName().c_str() ).c_str(),
            MT_FormatString( "Veuillez vérifier les valeurs des différents champs d'intoxication '%s' de l'agent NBC '%s'.", GetNodeName().c_str(), GetParentNode()->GetNodeName().c_str() ).c_str() );
        input.EndSection();
    }

    bContaminationPresent_ = input.Section( "Contamination", ADN_XmlInput_Helper::eNothing );
    if( bContaminationPresent_.GetData() )
        input.EndSection();
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::WriteArchive
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcIntoxInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    if( bIntoxPresent_.GetData() )
    {
        if( rNbAlivedHumans_.GetData() + rNbHurtedHumans1_.GetData() + rNbHurtedHumans2_.GetData() + rNbHurtedHumans3_.GetData() + rNbHurtedHumansE_.GetData() + rNbDeadHumans_.GetData() != 100.0 )
            throw ADN_DataException( "Incohérence des données",
            MT_FormatString( "La répartition de l'intoxication '%s' sur les humains de l'agent NBC %s ne couvre pas tous les cas possibles (soit 100%).", GetNodeName().c_str(), GetParentNode()->GetNodeName().c_str() ).c_str(),
            MT_FormatString( "Vérifier les valeurs des différents champs d'intoxication '%s' de l'agent NBC %s.", GetNodeName().c_str(), GetParentNode()->GetNodeName().c_str() ).c_str() );

        output.Section( "Intoxication" );
        output.WriteField( "Humains_NonBlesse", rNbAlivedHumans_.GetData() / 100.0 );
        output.WriteField( "Humains_U1", rNbHurtedHumans1_.GetData() / 100.0 );
        output.WriteField( "Humains_U2", rNbHurtedHumans2_.GetData() / 100.0 );
        output.WriteField( "Humains_U3", rNbHurtedHumans3_.GetData() / 100.0 );
        output.WriteField( "Humains_UE", rNbHurtedHumansE_.GetData() / 100.0 );
        output.WriteField( "Humains_Mort", rNbDeadHumans_.GetData() / 100.0 );
        output.EndSection();
    }
    if( bContaminationPresent_.GetData() )
    {
        output.Section( "Contamination" );
        output.EndSection();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::NbcGazInfos
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
ADN_NBC_Datas::NbcGazInfos::NbcGazInfos()
    : intoxInfos_( "gaz" )
    , lifeTime_( "0s" )
    , rSpreadAngle_( 0.0 )
{
    lifeTime_.SetDataName( "la durée de vie d'" );
    lifeTime_.SetParentNode( *this );
    rSpreadAngle_.SetDataName( "l'angle de propagation dû au vent d'" );
    rSpreadAngle_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::GetNodeName
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
std::string ADN_NBC_Datas::NbcGazInfos::GetNodeName()
{
    return GetParentNode()->GetNodeName();
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::GetItemName
// Created: SBO 2006-10-31
// -----------------------------------------------------------------------------
std::string ADN_NBC_Datas::NbcGazInfos::GetItemName()
{
    return "";
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::CopyFrom
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcGazInfos::CopyFrom( NbcGazInfos& infos )
{
    intoxInfos_.CopyFrom( infos.intoxInfos_ );
    lifeTime_ = infos.lifeTime_.GetData();
    rSpreadAngle_ = infos.rSpreadAngle_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::ReadArchive
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcGazInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    intoxInfos_.ReadArchive( input );

    input.ReadField( "DureeDeVie", lifeTime_ );
    
//    if( rTmp < 0.0 )
//        throw ADN_DataException( "Donnée invalide",
//        MT_FormatString( "La durée de vie de l'agent NBC %s est inférieure à 0", strName.c_str() ).c_str(),
//        MT_FormatString( "Veuillez modifier la valeur du champ NBC::AgentsNBC::AgentNBC::DureeDeVie" ).c_str() );
//    rLifeTime_ = rTmp;

    input.Section( "AngleDePropagation" );
    std::string strUnit( "degre" );
    input.ReadAttribute( "unite", strUnit, ADN_XmlInput_Helper::eNothing );

    int nAngle = 0;
    input.Read( nAngle );

    if( strUnit == "degre" )
    {
        if( nAngle < 0 || nAngle > 360 )
            throw ADN_DataException( "Mauvaises données lors du chargement d'un agent NBC",
            MT_FormatString( "L'agent NBC %s a un angle de propagation hors de la plage de valeur autorisée.", GetParentNode()->GetNodeName().c_str() ).c_str(),
            "Assurez vous que l'angle soit entre 0 et 360." );
        rSpreadAngle_ = nAngle;
    }
    else if( strUnit == "radian" )
    {
        if( nAngle < 0 || nAngle > 2 * MT_PI )
            throw ADN_DataException( "Mauvaises données lors du chargement d'un agent NBC",
            MT_FormatString( "L'agent NBC %s a un angle de propagation hors de la plage de valeur autorisée.", GetParentNode()->GetNodeName().c_str() ).c_str(),
            "Assurez vous que l'angle soit entre 0 et 2 PI." );
        rSpreadAngle_ = (int)(((double)nAngle)*180/MT_PI);
    }
    else
        throw ADN_DataException( "Mauvaises données lors du chargement d'un agent NBC",
        MT_FormatString( "Unitée inconnue pour l'angle de propagation de l'agent NBC %s.", GetParentNode()->GetNodeName().c_str() ).c_str(),
        "Utilisez soit \"degre\" soit \"radian\"." );

    input.EndSection(); // AngleDePropagation
}
    
// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::WriteArchive
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcGazInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    intoxInfos_.WriteArchive( output );
    output.WriteField( "DureeDeVie", lifeTime_.GetData() );
    output.Section( "AngleDePropagation" );
    output.WriteAttribute( "unite", "degre" );
    output << rSpreadAngle_.GetData();
    output.EndSection();
}

// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::NbcAgentInfos
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_Datas::NbcAgentInfos::NbcAgentInfos()
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , strName_()
    , nMosId_( ADN_Workspace::GetWorkspace().GetNbc().GetData().GetNextId() )
    , liquidInfos_( "liquide" )
    , bGazPresent_( false )
    , gazInfos_()
{
    strName_.SetDataName( "le nom d'" );
    strName_.SetParentNode( *this );
    liquidInfos_.SetParentNode( *this );
    bGazPresent_.SetParentNode( *this );
    gazInfos_.SetParentNode( *this );
}


// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::GetNodeName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_NBC_Datas::NbcAgentInfos::GetNodeName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::GetItemName
// Created: SBO 2006-10-31
// -----------------------------------------------------------------------------
std::string ADN_NBC_Datas::NbcAgentInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_NBC_Datas::NbcAgentInfos* ADN_NBC_Datas::NbcAgentInfos::CreateCopy()
{
    NbcAgentInfos* pCopy = new NbcAgentInfos();
    pCopy->liquidInfos_.CopyFrom( liquidInfos_ );
    pCopy->gazInfos_.CopyFrom( gazInfos_ );
    pCopy->bGazPresent_ = bGazPresent_.GetData();
    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::ReadArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcAgentInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "AgentNBC" );

    std::string strName;
    input.ReadAttribute( "nom", strName );
    strName_ = strName;

    input.Section( "Liquide" );
    liquidInfos_.ReadArchive( input );    
    input.EndSection();

    bGazPresent_ = input.Section( "Gaz", ADN_XmlInput_Helper::eNothing );
    if( bGazPresent_.GetData() )
    {
        gazInfos_.ReadArchive( input );
        input.EndSection();
    }

    input.EndSection();
}


// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcAgentInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "AgentNBC" );

    output.WriteAttribute( "nom", strName_.GetData() );
    output.WriteField( "MosID", nMosId_ );

    output.Section( "Liquide" );
    liquidInfos_.WriteArchive( output );
    output.EndSection();

    if( bGazPresent_.GetData() )
    {
        output.Section( "Gaz" );
        gazInfos_.WriteArchive( output );
        output.EndSection();
    }
    output.EndSection(); // AgentNBC
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas constructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_Datas::ADN_NBC_Datas()
: ADN_Data_ABC()
, nNextId_    ( 1 )
{
    vNbcAgent_.SetItemTypeName( "un agent NBC" );
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas destructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_Datas::~ADN_NBC_Datas()
{
    Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::FilesNeeded
// Created: APE 2004-12-06
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szNBC_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::Reset
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::Reset()
{
    nNextId_ = 1;
    vNbcAgent_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::ReadArchive
// Created: AGN 2004-07-02
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "NBC" );

    input.Section( "ImpactTenueNBC" );
    input.ReadField( "CoefModificationVitesseMax", rNbcSuitMaxSpeedMultiplier_ );
    input.ReadField( "CoefModificationTempsRechargement", rNbcSuitReloadSpeedMultiplier_ );
    input.EndSection(); // ImpactTenueNBC

    input.ReadField( "VitesseVentLimiteAvantPropagationDirectionnelle", rWindSpeedLimitForSpreading_ );

    input.Section( "AgentsNBC" );
    while( input.NextListElement() )
    {
        std::auto_ptr<NbcAgentInfos> spNew( new NbcAgentInfos() );
        spNew->ReadArchive( input );
        vNbcAgent_.AddItem( spNew.release() );
    }
    input.EndSection(); // AgentsNBC
    input.EndSection(); // NBC
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "NBC" );

    output.Section( "ImpactTenueNBC" );
    output.WriteField( "CoefModificationVitesseMax", rNbcSuitMaxSpeedMultiplier_.GetData() );
    output.WriteField( "CoefModificationTempsRechargement", rNbcSuitReloadSpeedMultiplier_.GetData() );
    output.EndSection(); // ImpactTenueNBC

    output.WriteField( "VitesseVentLimiteAvantPropagationDirectionnelle", rWindSpeedLimitForSpreading_.GetData() );

    output.Section( "AgentsNBC" );
    for( T_NbcAgentInfos_Vector::iterator itAgent = vNbcAgent_.begin(); itAgent != vNbcAgent_.end(); ++itAgent )
    {
        (*itAgent)->WriteArchive( output );
    }
    output.EndSection(); // AgentsNBC

    output.EndSection(); // NBC
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::GetNextId
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
int ADN_NBC_Datas::GetNextId()
{
    return nNextId_++;
}
