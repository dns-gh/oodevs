// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "EntitySearchToolbar.h"
#include "moc_EntitySearchToolbar.cpp"
#include "Controllers.h"
#include "Agent.h"
#include "Population.h"
#include "Object.h"
#include "ActionController.h"

namespace
{
    template< typename Entity >
    class Searchable : public EntitySearchToolbar::SearchableItem_ABC
    {
    public:
        Searchable( Controllers& controllers, const Entity& entity )
            : controllers_( &controllers )
            , entity_( &entity )
            , id_( QString::number( entity.GetId() ) )
            , name_( entity.GetName().c_str() )
        {
            name_ = name_.lower();
        };
        virtual ~Searchable()
        {};
        virtual bool Matches( const QString& input ) const {
            return id_.find( input ) != -1 || name_.find( input ) != -1;
        };
        virtual void Activate() 
        {
            controllers_->actions_.Select( *entity_ );
            controllers_->actions_.Activate( *entity_ );
        };
    private:
        Controllers* controllers_;
        const Entity* entity_;
        QString id_;
        QString name_;
    };
}

// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
EntitySearchToolbar::EntitySearchToolbar( QMainWindow* pParent, Controllers& controllers )
    : QToolBar( pParent )
    , controllers_( controllers )
    
{
    searchDialog_ = CreateSearchDialog( pParent );
    searchDialog_->hide();

    // $$$$ AGE 2006-04-20: icon
    QToolButton* button = new QToolButton( MAKE_ICON( weather ), "", "", 0, "", this, "" );
    button->setToggleButton( true );
    connect( button, SIGNAL( toggled( bool ) ), searchDialog_, SLOT( setShown( bool ) ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
EntitySearchToolbar::~EntitySearchToolbar()
{
    controllers_.Remove( *this );
    for( CIT_Items it = items_.begin(); it != items_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar::NotifyCreated
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void EntitySearchToolbar::NotifyCreated( const Agent& agent )
{
    AddItem( *new Searchable< Agent >( controllers_, agent ) );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar::NotifyDeleted
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void EntitySearchToolbar::NotifyDeleted( const Agent& agent )
{
    RemoveItem( agent.GetId() );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar::NotifyCreated
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void EntitySearchToolbar::NotifyCreated( const Population& popu )
{
    AddItem( *new Searchable< Population >( controllers_, popu ) );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar::NotifyDeleted
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void EntitySearchToolbar::NotifyDeleted( const Population& popu )
{
    RemoveItem( popu.GetId() );
}
    
// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar::NotifyCreated
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void EntitySearchToolbar::NotifyCreated( const Object& object )
{
    AddItem( *new Searchable< Object >( controllers_, object ) );
}
    
// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar::NotifyDeleted
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void EntitySearchToolbar::NotifyDeleted( const Object& object )
{
    RemoveItem( object.GetId() );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar::AddItem
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void EntitySearchToolbar::AddItem( SearchableItem_ABC& item )
{
    items_.push_back( &item );
    lastItem_ = items_.begin();
}

// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar::RemoveItem
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void EntitySearchToolbar::RemoveItem( unsigned long id )
{
    // $$$$ AGE 2006-04-20: may be dangerous
    const QString input = QString::number( id );
    for( IT_Items it = items_.begin(); it != items_.end(); ++it )
    {
        if( (*it)->Matches( input ) )
        {
            std::swap( *it, items_.back() );
            delete items_.back();
            items_.pop_back();
            lastItem_ = items_.begin();
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar::CreateSearchDialog
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
QDialog* EntitySearchToolbar::CreateSearchDialog( QWidget* pParent )
{
    QDialog* dialog = new QDialog( pParent, "", 0, WStyle_Customize | WStyle_NormalBorder | WStyle_Title );
    dialog->setCaption( tr( "Recherche" ) );
    QHBoxLayout* layout = new QHBoxLayout( dialog );
    layout->setMargin( 2 );
    layout->setSpacing( 10 );

    QLineEdit* edit = new QLineEdit( dialog );
    layout->addWidget( edit );
    QPushButton* next = new QPushButton( tr( "Next" ), dialog ); 
    next->setAccel( Qt::Key_F3 );
    layout->addWidget( next );

    connect( edit, SIGNAL( textChanged( const QString& ) ), this, SLOT( Search( const QString& ) ) );
    connect( next, SIGNAL( pressed() ), this, SLOT( FindNext() ) );
    return dialog;
}

// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar::Search
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void EntitySearchToolbar::Search( const QString& input )
{
    currentSearch_ = input.lower();
    Find();
}

// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar::Find
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
bool EntitySearchToolbar::Find( CIT_Items begin, CIT_Items end )
{
    for( CIT_Items it = begin; it < end; ++it )
    {
        if( (*it)->Matches( currentSearch_ ) )
        {
            (*it)->Activate();
            lastItem_ = it;
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar::Find
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void EntitySearchToolbar::Find()
{
    if( ! Find( lastItem_, items_.end() ) )
        Find( items_.begin(), lastItem_ );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchToolbar::FindNext
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void EntitySearchToolbar::FindNext()
{
    ++lastItem_;
    if( lastItem_ >= items_.end() )
        lastItem_ = items_.begin();
    Find();
}
