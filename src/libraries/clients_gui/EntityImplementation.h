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
}

namespace gui
{
    class PropertiesDictionary;
}

namespace gui
{
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
             EntityImplementation( kernel::Controller& controller, unsigned long id, const QString& name, bool readOnly = false );
    virtual ~EntityImplementation();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual void Rename( const QString& name );

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
EntityImplementation< I >::EntityImplementation( kernel::Controller& controller, unsigned long id, const QString& name, bool readOnly = false )
    : controller_( controller )
    , id_        ( id )
    , displayId_ ( GetDisplayId() )
    , name_      ( name )
    , dictionary_( new PropertiesDictionary( controller ) )
{
    Attach( *dictionary_ );
    dictionary_->Register( *this, tools::translate( "EntityImplementation", "Info/Identifier" ), displayId_ );
    dictionary_->Register( *this, tools::translate( "EntityImplementation", "Info/Name" ), name_, *this, &EntityImplementation::Rename, readOnly );
}

}

#include "EntityImplementation.inl"

#endif // CLIENTS_GUI_ENTITY_IMPLEMENTATION_H__
