// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formatter_h_
#define __Formatter_h_

#include <qstring.h>
#include "Displayer_ABC.h"

// =============================================================================
/** @class  Formatter
    @brief  Formatter
*/
// Created: AGE 2006-02-21
// =============================================================================
class FormatterNotImplemented
{
protected:
    void Error( Displayer_ABC& displayer, const type_info& type ) const;
};

template< typename T >
struct Formatter : public FormatterNotImplemented {
    void operator()( const T&, Displayer_ABC& displayer ) const {
        Error( displayer, typeid( T ) );
    }
};

// =============================================================================
/** @class  Formatter
    @brief  base types
*/
// Created: AGE 2006-02-21
// =============================================================================
struct NumberFormatter {
    template< typename T >
    void operator()( const T& value, Displayer_ABC& displayer ) const {
        displayer.AddToDisplay( QString::number(value ) );
    }
};
template< >
struct Formatter< float > : public NumberFormatter {};
template< >
struct Formatter< double > : public NumberFormatter {};
template< >
struct Formatter< unsigned int > : public NumberFormatter {};
template< >
struct Formatter< int > : public NumberFormatter {};
template< >
struct Formatter< unsigned long > : public NumberFormatter {};
template< >
struct Formatter< long > : public NumberFormatter {};
template< >
struct Formatter< char > {
    void operator()( const char& value, Displayer_ABC& displayer ) const {
        displayer.AddToDisplay( QString( QChar( value ) ) );
    }
};
template< >
struct Formatter< const char* > {
    void operator()( const char* value, Displayer_ABC& displayer ) const {
        displayer.AddToDisplay( QString( value ) );
    }
};
template< unsigned int N >
struct Formatter< const char [N] > {
    void operator()( const char* value, Displayer_ABC& displayer ) const {
        displayer.AddToDisplay( QString( value ) );
    }
};


template< >
struct Formatter< std::string > {
    void operator()( const std::string& value, Displayer_ABC& displayer ) const {
        displayer.AddToDisplay( QString( value.c_str() ) );
    }
};

// =============================================================================
/** @class  Formatter
    @brief  Pointers
*/
// Created: AGE 2006-02-21
// =============================================================================
class ValueNotSet {};
template< >
struct Formatter< ValueNotSet >
{
    void operator()( const ValueNotSet& , Displayer_ABC& displayer ) const
    {
        displayer.AddToDisplay( QString( "-" ) );
    }
};

template< typename T >
struct Formatter< const T* >
{
    void operator()( const T* element, Displayer_ABC& displayer ) const
    {
        if( element )
            displayer.AddToDisplay( *element );
        else
            displayer.AddToDisplay( ValueNotSet() );
    }
};

// =============================================================================
/** @class  Formatter
    @brief  OptionalValue
*/
// Created: AGE 2006-02-21
// =============================================================================
template< typename T > class OptionalValue;
template< typename T >
struct Formatter< OptionalValue< T > >
{
    void operator()( const OptionalValue< T >& element, Displayer_ABC& displayer ) const
    {
        if( element.IsSet() )
            displayer.AddToDisplay( (const T&)( element ) );
        else 
            displayer.AddToDisplay( ValueNotSet() );
    }
};

// =============================================================================
/** @class  Formatter
    @brief  Units
*/
// Created: AGE 2006-02-21
// =============================================================================
template< typename T > class UnitedValue;
template< typename T >
struct Formatter< UnitedValue< T > >
{
    void operator()( const UnitedValue< T >& element, Displayer_ABC& displayer ) const
    {
        displayer.AddToDisplay( element.value_ );
        displayer.AddToDisplay( (const QString&)element.unit_ );
    }
};

// =============================================================================
/** @class  Formatter
    @brief  Enums
*/
// Created: AGE 2006-02-21
// =============================================================================
template< >
struct Formatter< E_UnitPosture >
{
    void operator()( const E_UnitPosture& e, Displayer_ABC& displayer ) const {
        displayer.AddToDisplay( ENT_Tr::ConvertFromUnitPosture( e ) );
    }
};

template< >
struct Formatter< E_EtatOperationnel >
{
    void operator()( const E_EtatOperationnel& e, Displayer_ABC& displayer ) const {
        displayer.AddToDisplay( ENT_Tr::ConvertFromEtatOperationnel( e ) );
    }
};

template< >
struct Formatter< E_EtatRapFor >
{
    void operator()( const E_EtatRapFor& e, Displayer_ABC& displayer ) const {
        displayer.AddToDisplay( ENT_Tr::ConvertFromEtatRapFor( e ) );
    }
};

template< >
struct Formatter< E_Roe >
{
    void operator()( const E_Roe& e, Displayer_ABC& displayer ) const {
        displayer.AddToDisplay( ENT_Tr::ConvertFromRoe( e ) );
    }
};

template< >
struct Formatter< E_RoePopulation >
{
    void operator()( const E_RoePopulation& e, Displayer_ABC& displayer ) const {
        displayer.AddToDisplay( ENT_Tr::ConvertFromRoePopulation( e ) );
    }
};

template< >
struct Formatter< E_EtatCombatRencontre >
{
    void operator()( const E_EtatCombatRencontre& e, Displayer_ABC& displayer ) const {
        displayer.AddToDisplay( ENT_Tr::ConvertFromEtatCombatRencontre( e ) );
    }
};

template< >
struct Formatter< E_DisponibiliteAuTir >
{
    void operator()( const E_DisponibiliteAuTir& e, Displayer_ABC& displayer ) const {
        displayer.AddToDisplay( ENT_Tr::ConvertFromDisponibiliteAuTir( e ) );
    }
};

// =============================================================================
/** @class  Formatter
    @brief  Containers
*/
// Created: AGE 2006-02-21
// =============================================================================
class Separator {};
template< >
struct Formatter< Separator >
{
    void operator()( const Separator& , Displayer_ABC& displayer ) const {
        displayer.AddToDisplay( ", " );
    }
};

template< typename T >
struct Formatter< std::vector< T > >
{
    void operator()( const std::vector< T >& v, Displayer_ABC& displayer ) const
    {
        if( v.empty() )
            displayer.AddToDisplay( ValueNotSet() );
        else
            for( std::vector< T >::const_iterator it = v.begin(); it != v.end(); ++it )
            {
                if( it != v.begin() )
                    displayer.AddToDisplay( Separator() );
                displayer.AddToDisplay( *it );
            }
    }
};


#endif // __Formatter_h_
