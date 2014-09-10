// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropertiesDictionary_h_
#define __PropertiesDictionary_h_

#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Extension_ABC.h"

#include "Property_ABC.h"
#include "DictionaryUpdated.h"

#pragma warning( push, 0 )
#include <QtCore/qstringlist.h>
#pragma warning( pop )
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Displayer_ABC;
    class Controller;
    class Extension_ABC;
}

namespace gui
{
// =============================================================================
/** @class  PropertiesDictionary
    @brief  Properties dictionary
*/
// Created: SBO 2006-10-17
// =============================================================================
class PropertiesDictionary : public kernel::Extension_ABC
                           , public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PropertiesDictionary( kernel::Controller& controller );
    virtual ~PropertiesDictionary();
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, Property_ABC* > T_Properties;
    //@}

    //! @name Operations
    //@{
    bool HasKey( const QString& name ) const;
    void Remove( const QString& name );
    void Display( kernel::Displayer_ABC& displayer );
    void Display( const QString& name, kernel::Displayer_ABC& displayer );
    const T_Properties& GetProperties() const;

    template< typename T, typename Setter, typename OwnerExtension >
    void RegisterExtension( const kernel::Entity_ABC& owner,
                            const OwnerExtension* ownerExtension,
                            const QString& name,
                            T& value,
                            const Setter& setter,
                            bool readOnly = false,
                            E_Category category = eNothing )
    {
        if( HasKey( name ) )
            controller_.Delete( DictionaryUpdated( owner, name ) );
        Property_ABC*& property = properties_[ name ];
        delete property;
        property = new Property< T, Setter, OwnerExtension >( controller_, owner, value, setter, name, readOnly, category, ownerExtension );
        controller_.Create( DictionaryUpdated( owner, name ) );
    }

    template< typename T, typename Setter, typename OwnerExtension >
    void RegisterExtension( const kernel::Entity_ABC& owner,
                            const OwnerExtension* ownerExtension,
                            const QString& name,
                            T& value,
                            const Setter& setter,
                            const Property_ABC::T_ReadOnlyFunctor& readOnlyFunctor = Property_ABC::T_ReadOnlyFunctor(),
                            E_Category category = eNothing )
    {
        if( HasKey( name ) )
            controller_.Delete( DictionaryUpdated( owner, name ) );
        Property_ABC*& property = properties_[ name ];
        delete property;
        property = new Property< T, Setter, OwnerExtension >( controller_, owner, value, setter, name, readOnlyFunctor, category, ownerExtension );
        controller_.Create( DictionaryUpdated( owner, name ) );
    }

    template< typename T > struct setter;
    template< typename T, typename OwnerExtension >
    void RegisterExtension( const kernel::Entity_ABC& owner,
                            const OwnerExtension* ownerExtension,
                            const QString& name,
                            T& value,
                            bool readOnly = false,
                            E_Category category = eNothing )
    {
        RegisterExtension( owner, ownerExtension, name, value, setter< T >(), readOnly, category );
    }

    template< typename O, typename T > struct caller;
    template< typename T, typename ConcreteOwner, typename OwnerExtension >
    void RegisterExtension( const kernel::Entity_ABC& owner,
                            const OwnerExtension* ownerExtension,
                            const QString& name,
                            T& value,
                            ConcreteOwner& c,
                            void (ConcreteOwner::*set)( const T& ),
                            bool readOnly = false,
                            E_Category category = eNothing )
    {
        RegisterExtension( owner, ownerExtension, name, value, caller< ConcreteOwner, T >( c, set ), readOnly, category );
    }

    template< typename T, typename Setter >
    void Register( const kernel::Entity_ABC& owner,
                   const QString& name,
                   T& value,
                   const Setter& setter,
                   bool readOnly = false,
                   E_Category category = eNothing )
    {
        RegisterExtension( owner, static_cast< const Extension_ABC* >( nullptr ), name, value, setter, readOnly, category );
    }

    template< typename T, typename Setter >
    void Register( const kernel::Entity_ABC& owner,
                   const QString& name, T& value,
                   const Setter& setter,
                   const Property_ABC::T_ReadOnlyFunctor& readOnlyFunctor = Property_ABC::T_ReadOnlyFunctor(),
                   E_Category category = eNothing )
    {
        RegisterExtension( owner, static_cast< const Extension_ABC* >( nullptr ), name, value, setter, readOnlyFunctor, category );
    }

    template< typename T > struct setter;
    template< typename T >
    void Register( const kernel::Entity_ABC& owner,
                   const QString& name,
                   T& value,
                   bool readOnly = false,
                   E_Category category = eNothing )
    {
        Register( owner, name, value, setter< T >(), readOnly, category );
    }

    template< typename O, typename T > struct caller;
    template< typename T, typename ConcreteOwner >
    void Register( const kernel::Entity_ABC& owner,
                   const QString& name,
                   T& value,
                   ConcreteOwner& c,
                   void (ConcreteOwner::*set)( const T& ),
                   bool readOnly = false,
                   E_Category category = eNothing )
    {
        Register( owner, name, value, caller< ConcreteOwner, T >( c, set ), readOnly, category );
    }

    template< typename O, typename T > struct caller;
    template< typename T, typename ConcreteOwner >
    void Register( const kernel::Entity_ABC& owner,
                   const QString& name,
                   T& value,
                   ConcreteOwner& c,
                   void (ConcreteOwner::*set)( const T& ),
                   const Property_ABC::T_ReadOnlyFunctor& readOnlyFunctor = Property_ABC::T_ReadOnlyFunctor(),
                   E_Category category = eNothing )
    {
        Register( owner, name, value, caller< ConcreteOwner, T >( c, set ), readOnlyFunctor, category );
    }
    //@}

    //! @name Types
    //@{
    template< typename T >
    struct setter {
        void operator()( T* target, const T& value ) { *target = value; }
    };

    template< typename T >
    struct setter< const T > {
        void operator()( const T* , const T& ) {}
    };

    template< typename O, typename T >
    struct caller
    {
        typedef void (O::*func)( const T& );
        caller( O& o, func s ) : o_( &o ), s_( s ) {};
        void operator()( T*, const T& value )
        {
            (o_->*s_)( value );
        }
    private:
        O* o_;
        func s_;
    };
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}

protected:
    //! @name Member data
    //@{
    T_Properties properties_;
    //@}
};

}

#endif // __PropertiesDictionary_h_
