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

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )
namespace bfs = boost::filesystem;

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
        const std::string filename( "TacticalLines.xml" );
        if( bfs::exists( bfs::path( filename, bfs::native ) ) )
            model_.Load( filename );
    }
    catch( ... )
    {
        // NOTHING
    }
}
