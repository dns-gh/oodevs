// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef CLIENTS_GUI_ENTITY_IMPLEMENTATION_H__
#define CLIENTS_GUI_ENTITY_IMPLEMENTATION_H__

#include "clients_kernel/Serializable_ABC.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/InstanciationComplete.h"
#include "clients_kernel/Tools.h"
#include "PropertiesDictionary.h"
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Controller;
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;

// =============================================================================
/** @class  EntityImplementation
    @brief  Entity implementation
*/
// Created: AGE 2006-10-12
// =============================================================================
template< typename I >
class EntityImplementation : public I
                           , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityImplementation( kernel::Controller& controller,
                                   unsigned long id,
                                   const QString& name,
                                   actions::ActionsModel* actionsModel = 0 );
    virtual ~EntityImplementation();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual void Rename( const QString& name );
    virtual void SetName( const QString& name );
    virtual bool CanBeRenamed() const;

    void Polish();
    void ForceNewId( unsigned long id );
    //@}

protected:
    //! @name Modifiers
    //@{
    void Destroy();
    void Touch();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void PublishRename();
    //@}

private:
    //! @name Helpers
    //@{
    I& This();
    std::string GetDisplayId()
    {
        std::stringstream m;
        m << id_;
        return m.str();
    }
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    PropertiesDictionary* dictionary_;
    //@}

protected:
    //! @name Member data
    //@{
    actions::ActionsModel* actionsModel_;
    unsigned long id_;
    QString name_;
    std::string displayId_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: EntityImplementation constructor
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
EntityImplementation< I >::EntityImplementation( kernel::Controller& controller,
                                                 unsigned long id,
                                                 const QString& name,
                                                 actions::ActionsModel* actionsModel )
    : controller_( controller )
    , dictionary_( new PropertiesDictionary( controller ) )
    , actionsModel_( actionsModel )
    , id_( id )
    , name_( name )
    , displayId_( GetDisplayId() )
{
    Attach( *dictionary_ );
    dictionary_->Register( *this, tools::translate( "EntityImplementation", "Info/Identifier" ), displayId_ );
    dictionary_->Register( *this, tools::translate( "EntityImplementation", "Info/Name" ), name_, *this, &EntityImplementation::Rename, [&](){ return !CanBeRenamed(); } );
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::SetName
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
template< typename I >
void EntityImplementation< I >::SetName( const QString& name )
{
    if( name == name_ )
        return;
    const QString property = tools::translate( "EntityImplementation", "Info/Name" );
    name_ = name;
    controller_.Update( gui::DictionaryUpdated( *this, property ) );
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::Rename
// Created: LGY 2012-08-24
// -----------------------------------------------------------------------------
template< typename I >
void EntityImplementation< I >::Rename( const QString& name )
{
    if( name == name_ )
        return;
    SetName( name );
    Touch();
    PublishRename();
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::PublishRename
// Created: LDC 2014-11-14
// -----------------------------------------------------------------------------
template< typename I >
void EntityImplementation< I >::PublishRename()
{
    if( actionsModel_ )
        actionsModel_->PublishRename( *this, name_ );
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation destructor
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
EntityImplementation< I >::~EntityImplementation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::GetName
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
QString EntityImplementation< I >::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::GetId
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
unsigned long EntityImplementation< I >::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::Polish
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
void EntityImplementation< I >::Polish()
{
    Update( kernel::InstanciationComplete() );
    controller_.Create( This() );
    controller_.Create( *(Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::Touch
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
void EntityImplementation< I >::Touch()
{
    controller_.Update( This() );
    controller_.Update( *(Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::Destroy
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
void EntityImplementation< I >::Destroy()
{
    controller_.Delete( This() );
    controller_.Delete( *(Entity_ABC*)this );
    DestroyExtensions();
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::This
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
I& EntityImplementation< I >::This()
{
    return *this;
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::SerializeAttributes
// Created: LGY 2012-08-24
// -----------------------------------------------------------------------------
template< typename I >
void EntityImplementation< I >::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", static_cast< long >( id_ ) )
        << xml::attribute( "name", name_.toStdString() );
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation ForceNewId
// Created: MMC 2013-05-29
// -----------------------------------------------------------------------------
template< typename I >
void EntityImplementation< I >::ForceNewId( unsigned long id )
{
    id_ = id;
    displayId_ = GetDisplayId();
    Touch();
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::CanBeRenamed
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
template< typename I >
bool EntityImplementation< I >::CanBeRenamed() const
{
    return true;
}

}

#endif // CLIENTS_GUI_ENTITY_IMPLEMENTATION_H__
