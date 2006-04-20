// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntitySearchBox_h_
#define __EntitySearchBox_h_

#include "EntitySearchBox_ABC.h"
#include "ElementObserver_ABC.h"
#include "Controllers.h"
#include "ActionController.h"

// =============================================================================
/** @class  EntitySearchBox
    @brief  EntitySearchBox
*/
// Created: SBO 2006-04-20
// =============================================================================
template< typename Entity >
class EntitySearchBox : public EntitySearchBox_ABC
                      , private Observer_ABC
                      , public ElementObserver_ABC< Entity >
{
public:
    //! @name Constructors/Destructor
    //@{
             EntitySearchBox( QWidget* pParent, Controllers& controllers );
    virtual ~EntitySearchBox();
    //@}

     //! @name Types
    //@{
    class SearchableItem_ABC
    {
    public:
        virtual ~SearchableItem_ABC() {};
        virtual bool Matches( const QString& input ) const = 0;
        virtual void Activate() = 0;
    };
    //@}

private:
    //! @name Slots
    //@{
    virtual void Search( const QString& input );
    virtual void FindNext();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EntitySearchBox( const EntitySearchBox& );            //!< Copy constructor
    EntitySearchBox& operator=( const EntitySearchBox& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< SearchableItem_ABC* > T_Items;
    typedef typename T_Items::iterator                 IT_Items;
    typedef typename T_Items::const_iterator          CIT_Items;
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Entity& );
    virtual void NotifyDeleted( const Entity& );

    void AddItem( SearchableItem_ABC& item );
    void RemoveItem( unsigned long id );

    void Find();
    bool Find( CIT_Items begin, CIT_Items end );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    T_Items items_;
    QString currentSearch_;
    CIT_Items lastItem_;
    //@}
};

namespace
{
    template< typename Entity >
    class Searchable : public EntitySearchBox< Entity >::SearchableItem_ABC
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
// Name: EntitySearchBox constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
EntitySearchBox< Entity >::EntitySearchBox( QWidget* pParent, Controllers& controllers )
    : EntitySearchBox_ABC( pParent )
    , controllers_( controllers )
   
{
    setMargin( 2 );
    setSpacing( 10 );

    QLineEdit* edit = new QLineEdit( this );
    QPushButton* next = new QPushButton( QIconSet( MAKE_ICON( search ) ), "", this ); 
    next->setAccel( Qt::Key_F3 );
    next->setMaximumWidth( 30 );

    connect( edit, SIGNAL( textChanged( const QString& ) ), this, SLOT( Search( const QString& ) ) );
    connect( next, SIGNAL( pressed() ), this, SLOT( FindNext() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
EntitySearchBox< Entity >::~EntitySearchBox()
{
    controllers_.Remove( *this );
    for( CIT_Items it = items_.begin(); it != items_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox::NotifyCreated
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
void EntitySearchBox< Entity >::NotifyCreated( const Entity& entity )
{
    AddItem( *new Searchable< Entity >( controllers_, entity ) );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox::NotifyDeleted
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
void EntitySearchBox< Entity >::NotifyDeleted( const Entity& entity )
{
    RemoveItem( entity.GetId() );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox::AddItem
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
void EntitySearchBox< Entity >::AddItem( SearchableItem_ABC& item )
{
    items_.push_back( &item );
    lastItem_ = items_.begin();
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox::RemoveItem
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
void EntitySearchBox< Entity >::RemoveItem( unsigned long id )
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
// Name: EntitySearchBox::Search
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
void EntitySearchBox< Entity >::Search( const QString& input )
{
    currentSearch_ = input.lower();
    Find();
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox::Find
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
bool EntitySearchBox< Entity >::Find( CIT_Items begin, CIT_Items end )
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
// Name: EntitySearchBox::Find
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
void EntitySearchBox< Entity >::Find()
{
    if( ! Find( lastItem_, items_.end() ) )
        Find( items_.begin(), lastItem_ );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox::FindNext
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename Entity >
void EntitySearchBox< Entity >::FindNext()
{
    ++lastItem_;
    if( lastItem_ >= items_.end() )
        lastItem_ = items_.begin();
    Find();
}

#endif // __EntitySearchBox_h_
