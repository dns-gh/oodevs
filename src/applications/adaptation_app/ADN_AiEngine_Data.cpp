// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
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

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_Data constructor
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
ADN_AiEngine_Data::ADN_AiEngine_Data()
: ADN_Data_ABC()
, rPertinenceMaxDecrease_( 0.0 )
, rOperationalStateMaxDecrease_( 0.0 )
, rNeutralizedStateMaxDecrease_( 0.0 )
, rMinorEquipmentWeight_( 0.0 )
, rMajorEquipmentWeight_( 0.0 )
, rHumanWeight_( 0.0 )
, rDefaultFeedbackTime_( "1s" ) 
{
    rMinorEquipmentWeight_.SetDataName( "Poids des composantes non majeures." );
    rMajorEquipmentWeight_.SetDataName( "Poids des composantes majeures." );
    rMajorEquipmentWeight_.SetUndoAvailable( false );
    rHumanWeight_         .SetDataName( "Poids du personnel." );

    rPertinenceMaxDecrease_.SetDataName( "la d�gradation de la dangerosit� par la pertinence" );
    rOperationalStateMaxDecrease_.SetDataName( "la d�gradation de la dangerosit� par l'�tat op�rationnel" );
    rNeutralizedStateMaxDecrease_.SetDataName( "la d�gradation de la dangerosit� par l'�tat de neutralisation" );
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
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szDecisional_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_Data::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_AiEngine_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "decisonal" )
            >> xml::start( "dangerosity-modifiers" )
                >> xml::attribute( "max-accuracy", rPertinenceMaxDecrease_ )
                >> xml::attribute( "max-operational-state", rOperationalStateMaxDecrease_ )
                >> xml::attribute( "max-neutralized-state", rNeutralizedStateMaxDecrease_ )
            >> xml::end();

    if( rPertinenceMaxDecrease_.GetData() < 0.0 || rPertinenceMaxDecrease_.GetData() > 100.0 )
        throw ADN_DataException( "Donn�e invalide",
        "La d�gradation maximale due � la pertinence d'une connaissance doit �tre comprise dans l'intervalle [0;100].",
        "Veuillez �diter le fichier de configuration du moteur d�cisionnel pour modifier le champ DegradationMaxParPertinence" );

    if( rOperationalStateMaxDecrease_.GetData() < 0.0 || rOperationalStateMaxDecrease_.GetData() > 100.0 )
        throw ADN_DataException( "Donn�e invalide",
        "La d�gradation maximale due � l'�tat op�rationnel d'une connaissance doit �tre comprise dans l'intervalle [0;100].",
        "Veuillez �diter le fichier de configuration du moteur d�cisionnel pour modifier le champ DegradationMaxParEtatOps" );

    if( rNeutralizedStateMaxDecrease_.GetData() < 0.0 || rNeutralizedStateMaxDecrease_.GetData() > 100.0 )
        throw ADN_DataException( "Donn�e invalide",
        "La d�gradation maximale due � l'�tat de neutralisation d'une connaissance doit �tre comprise dans l'intervalle [0;100].",
        "Veuillez �diter le fichier de configuration du moteur d�cisionnel pour modifier le champ DegradationMaxParEtatNeutralise" );

    input >> xml::start( "operational-state-weights" )
            >> xml::attribute( "component", rMinorEquipmentWeight_ )
            >> xml::attribute( "major-component", rMajorEquipmentWeight_ )
            >> xml::attribute( "crew", rHumanWeight_ )
          >> xml::end();

    if( rMinorEquipmentWeight_.GetData() < 0.0 || rMinorEquipmentWeight_.GetData() > 1.0 )
        throw ADN_DataException( "Donn�e invalide",
        "Le poids des composantes non majeures doit �tre compris entre 0 et 1.",
        "Veuillez �diter le fichier de configuration du moteur d�cisionnel pour modifier le champ PoidsComposantesNonMajeures." );
    
    if( rMajorEquipmentWeight_.GetData() < 0.0 || rMajorEquipmentWeight_.GetData() > 1.0 )
        throw ADN_DataException( "Donn�e invalide",
        "Le poids des composantes majeures doit �tre compris entre 0 et 1.",
        "Veuillez �diter le fichier de configuration du moteur d�cisionnel pour modifier le champ PoidsComposantesMajeures." );

    if( rMinorEquipmentWeight_.GetData() + rMajorEquipmentWeight_.GetData() != 1.0 )
        throw ADN_DataException( "Donn�e invalide",
        "La somme des poids des composantes doit �tre �gal � 1.",
        "Veuillez �diter le fichier de configuration du moteur d�cisionnel pour modifier les champs PoidsComposantesMajeures et PoidsComposantesNonMajeures." );

    if( rHumanWeight_.GetData() < 0.0 || rHumanWeight_.GetData() > 1.0 )
        throw ADN_DataException( "Donn�e invalide",
            "Le poids du personnel doit �tre compris entre 0 et 1.",
            "Veuillez �diter le fichier de configuration du moteur d�cisionnel pour modifier le champ PoidsPersonnel." );

    input >> xml::start( "force-ratio" )
            >> xml::attribute( "default-feedback-time", rDefaultFeedbackTime_ )
          >> xml::end();

    input >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_Data::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_AiEngine_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "decisonal" )
            << xml::start( "dangerosity-modifiers" )
                << xml::attribute( "max-accuracy", rPertinenceMaxDecrease_ )
                << xml::attribute( "max-operational-state", rOperationalStateMaxDecrease_ )
                << xml::attribute( "max-neutralized-state", rNeutralizedStateMaxDecrease_ )
            << xml::end()
            << xml::start( "operational-state-weights" )
                << xml::attribute( "component", rMinorEquipmentWeight_ )
                << xml::attribute( "major-component", rMajorEquipmentWeight_ )
                << xml::attribute( "crew", rHumanWeight_ )
            << xml::end()
            << xml::start( "force-ratio" )
                << xml::attribute( "default-feedback-time", rDefaultFeedbackTime_ )
            << xml::end()
           << xml::end();
}
