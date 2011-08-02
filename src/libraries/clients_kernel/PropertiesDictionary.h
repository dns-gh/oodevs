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

#include "Extension_ABC.h"
#include "Property_ABC.h"
#include <QtCore/qstringlist.h>

namespace kernel
{
    class Displayer_ABC;
    class Controller;

// =============================================================================
/** @class  PropertiesDictionary
    @brief  PropertiesDictionary
*/
// Created: SBO 2006-10-17
// =============================================================================
class PropertiesDictionary : public Extension_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PropertiesDictionary( Controller& controller );
    virtual ~PropertiesDictionary();
    //@}

    //! @name Operations
    //@{
    bool HasKey( const QString& name ) const;

    template< typename T, typename Owner, typename Setter >
    void Register( const Owner& owner, const QString& name, T& value, const Setter& setter )
    {
        Property_ABC*& property = properties_[ name ];
        delete property;
        property = new Property< T, Owner, Setter >( controller_, owner, value, setter );
    }

    template< typename T > struct setter;
    template< typename T, typename Owner >
    void Register( const Owner& owner, const QString& name, T& value )
    {
        Register( owner, name, value, setter< T >() );
    }

    template< typename O, typename T > struct caller;
    template< typename T, typename Owner, typename ConcreteOwner  >
    void Register( const Owner& owner, const QString& name, T& value, ConcreteOwner& c, void (ConcreteOwner::*set)( const T& ) )
    {
        Register( owner, name, value, caller< ConcreteOwner, T >( c, set ) );
    }

    void Remove( const QString& name );

    void Display( Displayer_ABC& displayer );
    void Display( const QString& name, Displayer_ABC& displayer );
    void DisplaySubPath( const QString& path, Displayer_ABC& displayer );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PropertiesDictionary( const PropertiesDictionary& );            //!< Copy constructor
    PropertiesDictionary& operator=( const PropertiesDictionary& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    typedef std::map< QString, Property_ABC* >  T_Properties;
    typedef T_Properties::iterator             IT_Properties;
    typedef T_Properties::const_iterator      CIT_Properties;
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
    Controller& controller_;
    T_Properties properties_;
    //@}
};

}

#endif // __PropertiesDictionary_h_
