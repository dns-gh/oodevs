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
    void AddSubMenu( QPopupMenu* parent, const QString& label, const QIconSet& iconSet, Options& options, const std::string& option )
    {
        OptionMenu< TristateOption >* optionMenu = new OptionMenu< TristateOption >( parent, options, option );
        TristateOption::Populate( *optionMenu );
        parent->insertItem( iconSet, label, optionMenu );
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
    menu->insertItem( tr( "Fermer" ) );
    menu->insertSeparator();
    menu->insertItem( tr( "Recharger" ) );
    menu->insertSeparator();
    menu->insertItem( tr( "Ouvrir fichier d'ordres..." ) );
    menu->insertItem( tr( "Enregistrer fichier d'ordre..." ) );
    menu->insertSeparator();
    menu->insertItem( tr( "Quitter" ) );
    insertItem( tr( "Fichier" ), menu );

    menu = new QPopupMenu( this );
    QPopupMenu* subMenu = new QPopupMenu( menu );

    AddSubMenu( subMenu, tr( "Lignes de vision" )  , MAKE_ICON( visionlines )   , controllers.options_, "VisionLines" );
    AddSubMenu( subMenu, tr( "Cones de vision" )   , MAKE_ICON( visioncones )   , controllers.options_, "VisionCones" );
    AddSubMenu( subMenu, tr( "Surfaces de vision" ), MAKE_ICON( visionsurfaces ), controllers.options_, "VisionSurfaces" );
    subMenu->insertSeparator();
    AddSubMenu( subMenu, tr( "Itinéraires" )       , MAKE_ICON( path )          , controllers.options_, "Paths" );
    AddSubMenu( subMenu, tr( "Chemin parcouru" )   , MAKE_ICON( oldpath )       , controllers.options_, "OldPaths" );
    menu->insertItem( tr( "Unités..." ), subMenu );

    subMenu = new QPopupMenu( menu );
    AddSubMenu( subMenu, tr( "Liens logistiques" )              , MAKE_ICON( loglink )    , controllers.options_, "LogisticLinks" );
    AddSubMenu( subMenu, tr( "Liens logistiques manquant" )     , MAKE_ICON( missinglog ) , controllers.options_, "MissingLogisticLinks" );
    AddSubMenu( subMenu, tr( "Actions logistiques" ), MAKE_ICON( realtimelog ), controllers.options_, "RealTimeLogistic" );
    menu->insertItem( tr( "Logistique..." ), subMenu );

    subMenu = new QPopupMenu( menu );
    AddSubMenu( subMenu, tr( "Petits textes" )   , MAKE_ICON( textsmall )    , controllers.options_, "SmallText" );
    AddSubMenu( subMenu, tr( "Grands textes" )   , MAKE_ICON( textbig )      , controllers.options_, "BigText" );
    AddSubMenu( subMenu, tr( "Lignes tactiques" ), MAKE_ICON( tacticallines ), controllers.options_, "TacticalLines" );

    OptionMenu< bool >* boolMenu = new OptionMenu< bool >( subMenu, controllers.options_, "Weather" );
    boolMenu->AddItem( tr( "On" ), true );
    boolMenu->AddItem( tr( "Off" ), false );
    subMenu->insertItem( MAKE_ICON( weather ), tr( "Météo" ), boolMenu );

    subMenu->insertSeparator();

    OptionMenu< float >* gridMenu = new OptionMenu< float >( subMenu, controllers.options_, "GridSize" );
    gridMenu->AddItem( tr( "Off"    ),  -1 );
    gridMenu->AddItem( tr( "100m"  ),  0.1f );
    gridMenu->AddItem( tr( "250m" ),  0.25f );
    gridMenu->AddItem( tr( "500m"  ),  0.5f );
    gridMenu->AddItem( tr( "1km"  ),  1.0f );
    gridMenu->AddItem( tr( "2.5km"  ),  2.5f );
    gridMenu->AddItem( tr( "5km"  ),  5.0f );
    gridMenu->AddItem( tr( "10km" ), 10.0f );
    subMenu->insertItem( tr( "Grille" ), gridMenu );

    menu->insertItem( tr( "Terrain..." ), subMenu );
    menu->insertSeparator();

    boolMenu = new OptionMenu< bool >( menu, controllers.options_, "3D" );
    boolMenu->AddItem( tr( "2D" ), false );
    boolMenu->AddItem( tr( "3D" ), true );
    menu->insertItem( MAKE_ICON( threed ), tr( "Mode d'affichage" ), boolMenu );

    menu->insertSeparator();
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
