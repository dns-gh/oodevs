// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "Menu.h"
//#include "AboutDialog.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TristateOption.h"
#include "clients_gui/OptionMenu.h"
#include "clients_gui/resources.h"

using namespace kernel;
using namespace gui;

namespace
{
    void Populate( OptionMenu< TristateOption >& menu )
    {
        menu.AddItem( TristateOption::AutoName(), TristateOption::Auto() );
        menu.AddItem( TristateOption::OnName(), TristateOption::On() );
        menu.AddItem( TristateOption::OffName(), TristateOption::Off() );
    }

    void AddSubMenu( QPopupMenu* parent, const QString& label, const QIconSet& iconSet, Options& options, const std::string& option )
    {
        OptionMenu< TristateOption >* optionMenu = new OptionMenu< TristateOption >( parent, options, option );
        Populate( *optionMenu );
        parent->insertItem( iconSet, label, optionMenu );
    }
}

// -----------------------------------------------------------------------------
// Name: Menu constructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::Menu( QMainWindow* pParent, Controllers& controllers, QDialog& prefDialog )
    : QMenuBar( pParent )
{
    QPopupMenu* menu = new QPopupMenu( this );
    int itemId = menu->insertItem( MAKE_ICON( open ), tr( "&Ouvrir scipio.xml..." ), parent(), SLOT( Open() ) );
    menu->setAccel( CTRL + Key_O, itemId );
    menu->insertItem( tr( "Fermer" ), parent(), SLOT( Close() ) );
    menu->insertSeparator();
    itemId = menu->insertItem( MAKE_ICON( save ), tr( "&Sauvegarder" ), parent(), SLOT( Save() ) );
    menu->setAccel( CTRL + Key_S, itemId );
    menu->insertSeparator();
    menu->insertItem( tr( "&Quitter" ), qApp, SLOT( quit() ), CTRL + Key_Q );
    itemId = insertItem( tr( "&Fichier" ), menu );
    setAccel( ALT + Key_F, itemId );

    menu = new QPopupMenu( this );
    QPopupMenu* subMenu = new QPopupMenu( menu );

    AddSubMenu( subMenu, tr( "Liens logistiques" )              , MAKE_ICON( loglink )    , controllers.options_, "LogisticLinks" );
    AddSubMenu( subMenu, tr( "Liens logistiques manquant" )     , MAKE_ICON( missinglog ) , controllers.options_, "MissingLogisticLinks" );
    menu->insertItem( tr( "Logistique..." ), subMenu );

    subMenu = new QPopupMenu( menu );
    AddSubMenu( subMenu, tr( "Petits textes" )   , MAKE_ICON( textsmall )    , controllers.options_, "SmallText" );
    AddSubMenu( subMenu, tr( "Grands textes" )   , MAKE_ICON( textbig )      , controllers.options_, "BigText" );
    AddSubMenu( subMenu, tr( "Lignes tactiques" ), MAKE_ICON( tacticallines ), controllers.options_, "TacticalLines" );

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

    menu->insertItem( tr( "&Préférences..." ), &prefDialog, SLOT( exec() ), CTRL + Key_P );
    itemId = insertItem( tr( "Affi&chage" ), menu );
    setAccel( ALT + Key_C, itemId );

//    menu = new QPopupMenu( this );
//    menu->insertItem( tr( "A propos" ), new AboutDialog( this, factory ), SLOT( exec() ) );
//    itemId = insertItem( tr( "Ai&de" ), menu );
//    setAccel( ALT + Key_D, itemId );
}
    
// -----------------------------------------------------------------------------
// Name: Menu destructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::~Menu()
{
    // NOTHING
}
