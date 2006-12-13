// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_AiEngine_Data.h"

#include "ADN_DataException.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_Tools.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"


// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_Data constructor
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
ADN_AiEngine_Data::ADN_AiEngine_Data()
: ADN_Data_ABC()
, nDebuggerPort_( 0 )
, rPertinenceMaxDecrease_( 0.0 )
, rOperationalStateMaxDecrease_( 0.0 )
, rNeutralizedStateMaxDecrease_( 0.0 )
, rMinorEquipmentWeight_( 0.0 )
, rMajorEquipmentWeight_( 0.0 )
, rHumanWeight_( 0.0 )
, rDefaultFeedbackTime_( 1 ) 
{
    rMinorEquipmentWeight_.SetDataName( "Poids des composantes non majeures." );
    rMajorEquipmentWeight_.SetDataName( "Poids des composantes majeures." );
    rMajorEquipmentWeight_.SetUndoAvailable( false );
    rHumanWeight_         .SetDataName( "Poids du personnel." );

    rPertinenceMaxDecrease_.SetDataName( "la dégradation de la dangerosité par la pertinence" );
    rOperationalStateMaxDecrease_.SetDataName( "la dégradation de la dangerosité par l'état opérationnel" );
    rNeutralizedStateMaxDecrease_.SetDataName( "la dégradation de la dangerosité par l'état de neutralisation" );
}

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_Data destructor
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
ADN_AiEngine_Data::~ADN_AiEngine_Data()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_Data::Reset
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
void ADN_AiEngine_Data::Reset()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_Data::FilesNeeded
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
void ADN_AiEngine_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back(ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szDecisional_.GetData());
}


// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_Data::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_AiEngine_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Decisionnel" );

    input.Section( "DirectIA" );
    input.ReadField( "RepertoireBinaires", strBinariesFolder_ );
    input.ReadField( "RepertoireSources", strSourcesFolder_ );
    input.ReadField( "DIATypes", strDIATypesFile_ );
    input.ReadField( "DIAWorkspace", strDIAWorkspaceFile_ );
    input.ReadField( "Modeles", strModelsFile_ );
    input.ReadField( "Missions", strMissionsFile_ );
    input.ReadField( "PortDebogueur", nDebuggerPort_ );
    input.EndSection(); // DirectIA

    input.Section( "Dangerosite" );

    double rTmp = 0.0;

    input.ReadField( "DegradationMaxParPertinence", rTmp );
    if( rTmp < 0.0 || rTmp > 100.0 )
        throw ADN_DataException( "Donnée invalide",
        "La dégradation maximale due à la pertinence d'une connaissance doit être comprise dans l'intervalle [0;100].",
        "Veuillez éditer le fichier de configuration du moteur décisionnel pour modifier le champ DegradationMaxParPertinence" );
    rPertinenceMaxDecrease_ = rTmp;

    input.ReadField( "DegradationMaxParEtatOps", rTmp );
    if( rTmp < 0.0 || rTmp > 100.0 )
        throw ADN_DataException( "Donnée invalide",
        "La dégradation maximale due à l'état opérationnel d'une connaissance doit être comprise dans l'intervalle [0;100].",
        "Veuillez éditer le fichier de configuration du moteur décisionnel pour modifier le champ DegradationMaxParEtatOps" );
    rOperationalStateMaxDecrease_ = rTmp;

    input.ReadField( "DegradationMaxParEtatNeutralise", rTmp );
    if( rTmp < 0.0 || rTmp > 100.0 )
        throw ADN_DataException( "Donnée invalide",
        "La dégradation maximale due à l'état de neutralisation d'une connaissance doit être comprise dans l'intervalle [0;100].",
        "Veuillez éditer le fichier de configuration du moteur décisionnel pour modifier le champ DegradationMaxParEtatNeutralise" );
    rNeutralizedStateMaxDecrease_ = rTmp;

    input.EndSection(); // Dangerosite

    input.Section( "EtatOps" );

    input.ReadField( "PoidsComposantesNonMajeures", rTmp );
    if( rTmp < 0.0 || rTmp > 1.0 )
        throw ADN_DataException( "Donnée invalide",
        "Le poids des composantes non majeures doit être compris entre 0 et 1.",
        "Veuillez éditer le fichier de configuration du moteur décisionnel pour modifier le champ PoidsComposantesNonMajeures." );
    rMinorEquipmentWeight_ = rTmp;

    input.ReadField( "PoidsComposantesMajeures", rTmp );
    if( rTmp < 0.0 || rTmp > 1.0 )
        throw ADN_DataException( "Donnée invalide",
        "Le poids des composantes majeures doit être compris entre 0 et 1.",
        "Veuillez éditer le fichier de configuration du moteur décisionnel pour modifier le champ PoidsComposantesMajeures." );
    rMajorEquipmentWeight_ = rTmp;

    if( rMinorEquipmentWeight_.GetData() + rMajorEquipmentWeight_.GetData() != 1.0 )
        throw ADN_DataException( "Donnée invalide",
        "La somme des poids des composantes doit être égal à 1.",
        "Veuillez éditer le fichier de configuration du moteur décisionnel pour modifier les champs PoidsComposantesMajeures et PoidsComposantesNonMajeures." );

    input.ReadField( "PoidsPersonnel", rTmp );
    if( rTmp < 0.0 || rTmp > 1.0 )
        throw ADN_DataException( "Donnée invalide",
            "Le poids du personnel doit être compris entre 0 et 1.",
            "Veuillez éditer le fichier de configuration du moteur décisionnel pour modifier le champ PoidsPersonnel." );
    rHumanWeight_ = rTmp;
  
    input.EndSection(); // EtatOps

    input.Section( "RapportDeForce" );
    input.ReadTimeField( "TempsDeRemonteeParDefaut", rDefaultFeedbackTime_ );
    input.EndSection(); // RapportDeForce

    input.EndSection(); // Decisionnel
}


// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_Data::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_AiEngine_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Decisionnel" );

    output.Section( "DirectIA" );
    output.WriteField( "RepertoireBinaires", strBinariesFolder_.GetData() );
    output.WriteField( "RepertoireSources", strSourcesFolder_.GetData() );
    output.WriteField( "DIATypes", strDIATypesFile_.GetData() );
    output.WriteField( "DIAWorkspace", strDIAWorkspaceFile_.GetData() );
    output.WriteField( "Modeles", strModelsFile_.GetData() );
    output.WriteField( "Missions", strMissionsFile_.GetData() );
    output.WriteField( "PortDebogueur", nDebuggerPort_.GetData() );
    output.EndSection(); // DirectIA

    output.Section( "Dangerosite" );
    output.WriteField( "DegradationMaxParPertinence", rPertinenceMaxDecrease_.GetData() );
    output.WriteField( "DegradationMaxParEtatOps", rOperationalStateMaxDecrease_.GetData() );
    output.WriteField( "DegradationMaxParEtatNeutralise", rNeutralizedStateMaxDecrease_.GetData() );
    output.EndSection(); // Dangerosite

    output.Section( "EtatOps" );
    output.WriteField( "PoidsComposantesNonMajeures", rMinorEquipmentWeight_.GetData() );
    output.WriteField( "PoidsComposantesMajeures"   , rMajorEquipmentWeight_.GetData() );
    output.WriteField( "PoidsPersonnel"             , rHumanWeight_         .GetData() );
    output.EndSection(); // EtatOps

    output.Section( "RapportDeForce" );
    output.WriteField( "TempsDeRemonteeParDefaut", ADN_Tools::SecondToString( rDefaultFeedbackTime_.GetData() ) );
    output.EndSection(); // RapportDeForce

    output.EndSection(); // Decisionnel
}
