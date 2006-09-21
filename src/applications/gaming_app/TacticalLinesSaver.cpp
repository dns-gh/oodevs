// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TacticalLinesSaver.h"
#include "gaming/LimitsModel.h"

// -----------------------------------------------------------------------------
// Name: TacticalLinesSaver constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TacticalLinesSaver::TacticalLinesSaver( QObject* parent, LimitsModel& model )
    : QObject( parent )
    , model_( model )
{
    LoadLimits();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesSaver destructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TacticalLinesSaver::~TacticalLinesSaver()
{
    bool saveFile = QMessageBox::question( 0, tr( "TacticalLines.xml" ), 
                                               tr( "Les limites/limas ont été modifiées:\n voulez-vous sauvegarder TacticalLines.xml ?" ),
                                               QMessageBox::Yes,
                                               QMessageBox::No ) == QMessageBox::Yes;
    bool retry = true;
    while( saveFile && retry )
        retry = SaveLimits();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesSaver::SaveLimits
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
bool TacticalLinesSaver::SaveLimits()
{
    try
    {
        model_.Save( "TacticalLines.xml" );
    }
    catch( std::exception& e )
    {
        int retry = QMessageBox::warning( 0, 
                    tr("Erreur d'écriture fichier"),
                    tr("Le fichier de description des limites et limas TacticalLines.xml n'a pas pu etre sauvegardé.\n"
                    "Vérifiez qu'il ne soit pas portegé en écriture.\nMessage : ") + e.what(),
                    QMessageBox::Retry,
                    QMessageBox::Abort );
        return retry == QMessageBox::Retry;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesSaver::LoadLimits
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void TacticalLinesSaver::LoadLimits()
{
    try
    {
        model_.Load( "TacticalLines.xml" );
    }
    catch( ... )
    {
        // NOTHING
    }
}
