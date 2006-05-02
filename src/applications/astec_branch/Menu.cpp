// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Menu.h"
#include "OptionMenu.h"
#include "Controllers.h"
#include "TristateOption.h"

namespace
{
    void AddSubMenu( QPopupMenu* parent, const std::string& label, Controllers& controllers, const std::string& option )
    {
        OptionMenu< TristateOption >* optionMenu = new OptionMenu< TristateOption >( parent, controllers.options_, option );
        TristateOption::Populate( *optionMenu );
        parent->insertItem( qApp->tr( label.c_str() ), optionMenu );
    }
}

// -----------------------------------------------------------------------------
// Name: Menu constructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::Menu( QMainWindow* pParent, Controllers& controllers )
    : QMenuBar( pParent )
{
    QPopupMenu* menu = new QPopupMenu( this );
    menu->insertItem( tr( "Ouvrir..." ) );
    menu->insertSeparator();
    menu->insertItem( tr( "Ouvrir fichier d'ordres..." ) );
    menu->insertItem( tr( "Enregistrer fichier d'ordre..." ) );
    menu->insertSeparator();
    menu->insertItem( tr( "Quitter" ) );
    insertItem( tr( "Fichier" ), menu );

    menu = new QPopupMenu( this );
    QPopupMenu* subMenu = new QPopupMenu( menu );

    AddSubMenu( subMenu, "Afficher lignes de vision"  , controllers, "VisionLines" );
    AddSubMenu( subMenu, "Afficher cones de vision"   , controllers, "VisionCones" );
    AddSubMenu( subMenu, "Afficher surfaces de vision", controllers, "VisionSurfaces" );
    AddSubMenu( subMenu, "Afficher les itinéraires"   , controllers, "Paths" );
    AddSubMenu( subMenu, "Afficher chemin parcouru"   , controllers, "OldPaths" );
    menu->insertItem( tr( "Unités..." ), subMenu );

    menu->insertItem( tr( "Logistique..." ) );
    menu->insertItem( tr( "Terrain..." ) );
    menu->insertSeparator();
    menu->insertItem( tr( "Barres d'outils" ) );
    menu->insertItem( tr( "Préférences..." ) );
    insertItem( tr( "Affichage" ), menu );

    menu = new QPopupMenu( this );
    menu->insertItem( tr( "A propos" ) );
    insertItem( tr( "Aide" ), menu );
}
    
// -----------------------------------------------------------------------------
// Name: Menu destructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::~Menu()
{
    // NOTHING
}
