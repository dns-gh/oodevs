// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_NBC_Datas.cpp $
// $Author: Ape $
// $Modtime: 22/04/05 15:49 $
// $Revision: 11 $
// $Workfile: ADN_NBC_Datas.cpp $
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
// Name: NbcAgentInfos::NbcAgentInfos
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_Datas::NbcAgentInfos::NbcAgentInfos()
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, strName_()
, nMosId_( ADN_Workspace::GetWorkspace().GetNbc().GetData().GetNextId() )
, rNbAlivedHumans_( 100.0 )
, rNbHurtedHumans1_( 0.0 )
, rNbHurtedHumans2_( 0.0 )
, rNbHurtedHumans3_( 0.0 )
, rNbHurtedHumansE_( 0.0 )
, rNbDeadHumans_( 0.0 )
, lifeTime_( "0s" )
, rSpreadAngle_( 0.0 )
{
    strName_.SetDataName( "le nom d'" );
    strName_.SetParentNode( *this );
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
    lifeTime_.SetDataName( "la durée de vie d'" );
    lifeTime_.SetParentNode( *this );
    rSpreadAngle_.SetDataName( "l'angle de propagation dû au vent d'" );
    rSpreadAngle_.SetParentNode( *this );
}


// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::GetNodeName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_NBC_Datas::NbcAgentInfos::GetNodeName()
{
    std::string strResult( "une menace NBC de type " );
    return strResult + strName_.GetData();
}


// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::GetItemName
// Created: AGN 2004-05-18
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

    pCopy->rNbAlivedHumans_ = rNbAlivedHumans_.GetData();
    pCopy->rNbHurtedHumans1_ = rNbHurtedHumans1_.GetData();
    pCopy->rNbHurtedHumans2_ = rNbHurtedHumans2_.GetData();
    pCopy->rNbHurtedHumans3_ = rNbHurtedHumans3_.GetData();
    pCopy->rNbHurtedHumansE_ = rNbHurtedHumansE_.GetData();
    pCopy->rNbDeadHumans_ = rNbDeadHumans_.GetData();

    pCopy->lifeTime_ = lifeTime_.GetData();
    pCopy->rSpreadAngle_ = rSpreadAngle_.GetData();

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

    // attritions
    input.Section( "Attritions" );

    double rTmp;
    // alive humans percentage
    input.ReadField( "Humains_NonBlesse", rTmp );
    if( rTmp < 0.0 || rTmp > 1.0 )
        throw ADN_DataException( "Donnée invalide",
        MT_FormatString( "Le pourcentage d'humains vivants lors d'une attrition par l'agent NBC %s est en dehors de l'intervalle [0;1] autorisé", strName.c_str() ).c_str(),
        MT_FormatString( "Veuillez modifier la valeur du champ NBC::AgentsNBC::AgentNBC::Humains_NonBlesse" ).c_str() );
    rNbAlivedHumans_ = rTmp * 100.0;

    // U1 hurted humans percentage
    input.ReadField( "Humains_U1", rTmp );
    if( rTmp < 0.0 || rTmp > 1.0 )
        throw ADN_DataException( "Donnée invalide",
        MT_FormatString( "Le pourcentage d'humains avec une blessure de niveau U1 lors d'une attrition par l'agent NBC %s est en dehors de l'intervalle [0;1] autorisé", strName.c_str() ).c_str(),
        MT_FormatString( "Veuillez modifier la valeur du champ NBC::AgentsNBC::AgentNBC::Humains_BlessesU1" ).c_str() );
    rNbHurtedHumans1_ = rTmp * 100.0;

    // U2 hurted humans percentage
    input.ReadField( "Humains_U2", rTmp );
    if( rTmp < 0.0 || rTmp > 1.0 )
        throw ADN_DataException( "Donnée invalide",
        MT_FormatString( "Le pourcentage d'humains avec une blessure de niveau U2 lors d'une attrition par l'agent NBC %s est en dehors de l'intervalle [0;1] autorisé", strName.c_str() ).c_str(),
        MT_FormatString( "Veuillez modifier la valeur du champ NBC::AgentsNBC::AgentNBC::Humains_BlessesU2" ).c_str() );
    rNbHurtedHumans2_ = rTmp * 100.0;

    // U3 hurted humans percentage
    input.ReadField( "Humains_U3", rTmp );
    if( rTmp < 0.0 || rTmp > 1.0 )
        throw ADN_DataException( "Donnée invalide",
        MT_FormatString( "Le pourcentage d'humains avec une blessure de niveau U3 lors d'une attrition par l'agent NBC %s est en dehors de l'intervalle [0;1] autorisé", strName.c_str() ).c_str(),
        MT_FormatString( "Veuillez modifier la valeur du champ NBC::AgentsNBC::AgentNBC::Humains_BlessesU3" ).c_str() );
    rNbHurtedHumans3_ = rTmp * 100.0;

    // U4 hurted humans percentage
    input.ReadField( "Humains_UE", rTmp );
    if( rTmp < 0.0 || rTmp > 1.0 )
        throw ADN_DataException( "Donnée invalide",
        MT_FormatString( "Le pourcentage d'humains avec une blessure de niveau UE lors d'une attrition par l'agent NBC %s est en dehors de l'intervalle [0;1] autorisé", strName.c_str() ).c_str(),
        MT_FormatString( "Veuillez modifier la valeur du champ NBC::AgentsNBC::AgentNBC::Humains_BlessesUE" ).c_str() );
    rNbHurtedHumansE_ = rTmp * 100.0;

    // dead humans percentage
    input.ReadField( "Humains_Mort", rTmp );
    if( rTmp < 0.0 || rTmp > 1.0 )
        throw ADN_DataException( "Donnée invalide",
        MT_FormatString( "Le pourcentage d'humains morts lors d'une attrition par l'agent NBC %s est en dehors de l'intervalle [0;1] autorisé", strName.c_str() ).c_str(),
        MT_FormatString( "Veuillez pour modifier la valeur du champ NBC::AgentsNBC::AgentNBC::Humains_Morts" ).c_str() );
    rNbDeadHumans_ = rTmp * 100.0;

    // check the data. The sum must be equal to 100
    if( rNbAlivedHumans_.GetData() + rNbHurtedHumans1_.GetData() + rNbHurtedHumans2_.GetData() + rNbHurtedHumans3_.GetData() + rNbHurtedHumansE_.GetData() + rNbDeadHumans_.GetData() != 100.0 )
        throw ADN_DataException( "Incohérence des données",
        MT_FormatString( "La répartition de l'attrition sur les humains de l'agent NBC %s ne couvre pas tous les cas possibles (soit 100%).", strName.c_str() ).c_str(),
        MT_FormatString( "Veuillez vérifier les valeurs des différents champs de l'agent NBC %s.", strName.c_str() ).c_str() );

    input.EndSection(); // Attrition

    input.ReadField( "DureeDeVie", lifeTime_ );

    /*
    if( rTmp < 0.0 )
        throw ADN_DataException( "Donnée invalide",
        MT_FormatString( "La durée de vie de l'agent NBC %s est inférieure à 0", strName.c_str() ).c_str(),
        MT_FormatString( "Veuillez modifier la valeur du champ NBC::AgentsNBC::AgentNBC::DureeDeVie" ).c_str() );
    rLifeTime_ = rTmp;*/

    input.Section( "AngleDePropagation" );
    std::string strUnit( "degre" );
    input.ReadAttribute( "unite", strUnit, ADN_XmlInput_Helper::eNothing );

    int nAngle = 0;
    input.Read( nAngle );

    if( strUnit == "degre" )
    {
        if( nAngle < 0 || nAngle > 360 )
            throw ADN_DataException( "Mauvaises données lors du chargement d'un agent NBC",
            MT_FormatString( "L'agent NBC %s a un angle de propagation hors de la plage de valeur autorisée.", strName_.GetData().c_str() ).c_str(),
            "Assurez vous que l'angle soit entre 0 et 360." );
        rSpreadAngle_ = nAngle;
    }
    else if( strUnit == "radian" )
    {
        if( nAngle < 0 || nAngle > 2 * MT_PI )
            throw ADN_DataException( "Mauvaises données lors du chargement d'un agent NBC",
            MT_FormatString( "L'agent NBC %s a un angle de propagation hors de la plage de valeur autorisée.", strName_.GetData().c_str() ).c_str(),
            "Assurez vous que l'angle soit entre 0 et 2 PI." );
        rSpreadAngle_ = (int)(((double)nAngle)*180/MT_PI);
    }
    else
        throw ADN_DataException( "Mauvaises données lors du chargement d'un agent NBC",
        MT_FormatString( "Unitée inconnue pour l'angle de propagation de l'agent NBC %s.", strName_.GetData().c_str() ).c_str(),
        "Utilisez soit \"degre\" soit \"radian\"." );

    input.EndSection(); // AngleDePropagation 

    input.EndSection(); // AgentNBC
}


// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcAgentInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "AgentNBC" );
    if( rNbAlivedHumans_.GetData() + rNbHurtedHumans1_.GetData() + rNbHurtedHumans2_.GetData() + rNbHurtedHumans3_.GetData() + rNbHurtedHumansE_.GetData() + rNbDeadHumans_.GetData() != 100.0 )
        throw ADN_DataException( "Incohérence des données",
        MT_FormatString( "La répartition de l'attrition sur les humains de l'agent NBC %s ne couvre pas tous les cas possibles (soit 100%).", strName_.GetData().c_str() ).c_str(),
        MT_FormatString( "Vérifier les valeurs des différents champs de l'agent NBC %s.", strName_.GetData().c_str() ).c_str() );

    output.WriteAttribute( "nom", strName_.GetData() );
    output.WriteField( "MosID", nMosId_ );

    output.Section( "Attritions" );
    output.WriteField( "Humains_NonBlesse", rNbAlivedHumans_.GetData() / 100.0 );
    output.WriteField( "Humains_U1", rNbHurtedHumans1_.GetData() / 100.0 );
    output.WriteField( "Humains_U2", rNbHurtedHumans2_.GetData() / 100.0 );
    output.WriteField( "Humains_U3", rNbHurtedHumans3_.GetData() / 100.0 );
    output.WriteField( "Humains_UE", rNbHurtedHumansE_.GetData() / 100.0 );
    output.WriteField( "Humains_Mort", rNbDeadHumans_.GetData() / 100.0 );
    output.EndSection(); // Attritions

    output.WriteField( "DureeDeVie", lifeTime_.GetData() );
    output.Section( "AngleDePropagation" );
    output.WriteAttribute( "unite", "degre" );
    output << rSpreadAngle_.GetData();
    output.EndSection();

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
