// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_EntityImplementation_h
#define __kernel_EntityImplementation_h

#include "Serializable_ABC.h"
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
    class PropertiesDictionary;

// =============================================================================
/** @class  EntityImplementation
    @brief  Entity implementation
*/
// Created: AGE 2006-10-12
// =============================================================================
template< typename I >
class EntityImplementation : public I
                           , public Serializable_ABC
                           , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityImplementation( Controller& controller, unsigned long id, const QString& name, bool readOnly = false );
    virtual ~EntityImplementation();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual void Rename( const QString& name );

    void Polish();
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
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    PropertiesDictionary* dictionary_;
    //@}

protected:
    //! @name Member data
    //@{
    const unsigned long id_;
    QString name_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: EntityImplementation constructor
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
EntityImplementation< I >::EntityImplementation( Controller& controller, unsigned long id, const QString& name, bool readOnly = false )
    : controller_( controller )
    , id_        ( id )
    , name_      ( name )
    , dictionary_( new PropertiesDictionary( controller ) )
{
    Attach( *dictionary_ );
    dictionary_->Register( *this, tools::translate( "EntityImplementation", "Info/Identifier" ), id_ );
    dictionary_->Register( *this, tools::translate( "EntityImplementation", "Info/Name" ), name_, *this, &EntityImplementation::Rename, readOnly );
}

}

#include "EntityImplementation.inl"

#endif // __kernel_EntityImplementation_h
