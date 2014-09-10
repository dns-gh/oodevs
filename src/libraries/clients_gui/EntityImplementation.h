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
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

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
    //! @name Types
    //@{
    typedef std::function< void( const QString& ) > T_RenameObserver;
    typedef std::function< bool( const I& ) >       T_CanBeRenamedFunctor;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             EntityImplementation( kernel::Controller& controller,
                                   unsigned long id,
                                   const QString& name,
                                   const T_CanBeRenamedFunctor& canBeRenamedFunctor = T_CanBeRenamedFunctor() );
    virtual ~EntityImplementation();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual void Rename( const QString& name );
    virtual void SetName( const QString& name );
    virtual bool CanBeRenamed() const;
    void SetRenameObserver( const T_RenameObserver& renameObserver );

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
    const T_CanBeRenamedFunctor canBeRenamedFunctor_;
    T_RenameObserver renameObserver_;
    //@}

protected:
    //! @name Member data
    //@{
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
                                                 const T_CanBeRenamedFunctor& canBeRenamedFunctor /* = T_CanBeRenamedFunctor() */ )
    : controller_( controller )
    , id_( id )
    , displayId_( GetDisplayId() )
    , name_( name )
    , dictionary_( new PropertiesDictionary( controller ) )
    , canBeRenamedFunctor_( canBeRenamedFunctor )
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
    if( renameObserver_ )
        renameObserver_( name );
}

} //! namespace gui

#include "EntityImplementation.inl"

#endif // CLIENTS_GUI_ENTITY_IMPLEMENTATION_H__
