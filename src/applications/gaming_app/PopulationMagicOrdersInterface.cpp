// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PopulationMagicOrdersInterface.h"
#include "moc_PopulationMagicOrdersInterface.cpp"

#include "clients_gui/LocationCreator.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "gaming/StaticModel.h"
#include "gaming/tools.h"
#include "protocol/simulationsenders.h"


using namespace Common;
using namespace kernel;
using namespace gui;

namespace
{
    class InitializedLineEdit : public QLineEdit
    {
    public:
        InitializedLineEdit( QWidget* parent, const QString& initialValue )
            : QLineEdit( initialValue, parent ), initialValue_( initialValue ) {}

        virtual void keyPressEvent( QKeyEvent* e )
        {
            if( text() == initialValue_ )
            {
                clear();
                setAlignment( Qt::AlignRight );
            }
            QLineEdit::keyPressEvent( e );
        }

        virtual void showEvent( QShowEvent* e )
        {
            if( text() == initialValue_ )
            {
                selectAll();
                setFocus();
            }
            QLineEdit::showEvent( e );
        }

    private:
        QString initialValue_;
    };
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
PopulationMagicOrdersInterface::PopulationMagicOrdersInterface( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const StaticModel& staticModel, ParametersLayer& layer, const Profile_ABC& profile )
    : QObject( parent )
    , controllers_( controllers )
    , publisher_( publisher )
    , static_( staticModel )
    , profile_( profile )
    , selectedEntity_( controllers )
    , magicMove_( false )
{
    magicMoveLocation_ = new LocationCreator( 0, layer, *this );
    magicMoveLocation_->Allow( false, false, false, false );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface destructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
PopulationMagicOrdersInterface::~PopulationMagicOrdersInterface()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::NotifyContextMenu
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::NotifyContextMenu( const Population_ABC& entity, ContextMenu& menu )
{
    if( !profile_.CanDoMagic( entity ) )
        return;

    selectedEntity_ = &entity;
    QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
    AddMagic( tr( "Teleport" ), SLOT( Move() ), magicMenu );
    AddMagic( tr( "Kill all" ), SLOT( KillAllPopulation() ), magicMenu );
    AddValuedMagic( magicMenu, menu, tr( "Kill people:" ), SLOT( KillSomePopulation() ) );
    AddValuedMagic( magicMenu, menu, tr( "Resurect people:" ), SLOT( ResurectSomePopulation() ) );

    QPopupMenu* choiceMenu = new QPopupMenu( magicMenu );
    for( unsigned int i = 0; i < unsigned int( eNbrPopulationAttitude ); ++i )
        choiceMenu->insertItem( tools::ToString( (E_PopulationAttitude)i ), 0, i );
    magicMenu->insertItem( tr( "Change population attitude" ), choiceMenu );
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::Handle
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::Handle( Location_ABC& location )
{
    if( magicMove_ && location.IsValid() )
        location.Accept( *this );
    controllers_.Unregister( *magicMoveLocation_ );
    magicMove_ = false;
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::Move
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::Move()
{
    if( magicMove_ )
        return;
    controllers_.Register( *magicMoveLocation_ );
    magicMove_ = true;
    magicMoveLocation_->StartPoint();
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::KillAllPopulation
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::KillAllPopulation()
{
    if( selectedEntity_ )
    {
        simulation::PopulationMagicAction magicAction;
        magicAction().set_oid( selectedEntity_->GetId() );
        //magicAction.action().t ( T_MsgPopulationMagicAction_action_destruction_totale;
        magicAction.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::KillSomePopulation
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::KillSomePopulation()
{
    if( selectedEntity_ )
        if( const QLineEdit* editor = dynamic_cast< const QLineEdit* >( sender() ) )
        {
            simulation::PopulationMagicAction magicAction;
            magicAction().set_oid( selectedEntity_->GetId() );
            magicAction().mutable_action()->mutable_tuer()->set_kill( editor->text().toUInt() );
            magicAction.Send( publisher_ );
        }
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::ResurectSomePopulation
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::ResurectSomePopulation()
{
    if( selectedEntity_ )
        if( const QLineEdit* editor = dynamic_cast< const QLineEdit* >( sender() ) )
        {
            simulation::PopulationMagicAction magicAction;
            magicAction().set_oid( selectedEntity_->GetId() );
            magicAction().mutable_action()->mutable_ressusciter()->set_resurrect( editor->text().toUInt() );
            magicAction.Send( publisher_ );
        }
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::ChangePopulationAttitude
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::ChangePopulationAttitude( int index )
{
    if( selectedEntity_ )
    {
        simulation::PopulationMagicAction magicAction;
        magicAction().set_oid( selectedEntity_->GetId() );
        MsgMagicActionPopulationChangeAttitude params;
        params.set_attitude( ( EnumPopulationAttitude )index );


        *magicAction().mutable_action()->mutable_change_attitude() = params;
        magicAction.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::AddMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
int PopulationMagicOrdersInterface::AddMagic( const QString& label, const char* slot, QPopupMenu* menu )
{
    return menu->insertItem( label, this, slot );
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::AddValuedMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::AddValuedMagic( QPopupMenu* parent, ContextMenu& menu, const QString& label, const char* slot )
{
    QPopupMenu* valueMenu = new QPopupMenu( parent );
    QLineEdit* valueEditor = new InitializedLineEdit( valueMenu, tr( "Enter value" ) );
    valueMenu->insertItem( valueEditor );
    parent->insertItem( label, valueMenu );
    QToolTip::add( valueEditor, tr( "Type-in value then press 'Enter'" ) );
    connect( valueEditor, SIGNAL( returnPressed() ), this, slot );
    connect( valueEditor, SIGNAL( returnPressed() ), menu, SLOT( hide() ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationMagicOrdersInterface::VisitPoint
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void PopulationMagicOrdersInterface::VisitPoint( const geometry::Point2f& point )
{
    if( selectedEntity_ )
    {
        MsgCoordLatLong utm;
        static_.coordinateConverter_.ConvertToGeo( point, utm );
        simulation::PopulationMagicAction message;
        message().set_oid( selectedEntity_->GetId() );

        *message().mutable_action()->mutable_move_to()->mutable_coord() = utm;
        message.Send( publisher_, 56 );
        const_cast< Entity_ABC& >( *selectedEntity_ ).Update( message() );
    }
}
