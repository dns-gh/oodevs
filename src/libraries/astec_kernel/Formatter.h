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
#include "Types.h"
#include "ENT/ENT_Tr.h"

// =============================================================================
/** @class  Formatter
    @brief  Formatter
*/
// Created: AGE 2006-02-21
// =============================================================================
class FormatterNotImplemented
{
public:
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
struct Formatter< const char [N] > : public Formatter< const char* > { };


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
        displayer.AddToDisplay( QString( " - " ) );
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

template< typename T >
struct Formatter< T* > : public Formatter< const T* > {};

template< typename T > class SafePointer;
template< typename T >
struct Formatter< SafePointer< T > > : public Formatter< const T* > {};

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
        displayer.AddToDisplay( element.unit_ );
    }
};

class Unit;
template< >
struct Formatter< Unit >
{
    void operator()( const Unit& element, Displayer_ABC& displayer ) const
    {
        displayer.AddToDisplay( (const QString&)element );
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
    void operator()( const E_UnitPosture& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_EtatOperationnel >
{
    void operator()( const E_EtatOperationnel& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_EtatRapFor >
{
    void operator()( const E_EtatRapFor& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_Roe >
{
    void operator()( const E_Roe& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_RoePopulation >
{
    void operator()( const E_RoePopulation& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_EtatCombatRencontre >
{
    void operator()( const E_EtatCombatRencontre& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_DisponibiliteAuTir >
{
    void operator()( const E_DisponibiliteAuTir& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_PerceptionResult >
{
    void operator()( const E_PerceptionResult& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_NatureLevel >
{
    void operator()( const E_NatureLevel& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_UnitNatureWeapon >
{
    void operator()( const E_UnitNatureWeapon& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_UnitNatureSpecialization >
{
    void operator()( const E_UnitNatureSpecialization& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_UnitNatureQualifier >
{
    void operator()( const E_UnitNatureQualifier& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_UnitNatureCategory >
{
    void operator()( const E_UnitNatureCategory& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_UnitNatureMobility >
{
    void operator()( const E_UnitNatureMobility& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_UnitCapaciteMission >
{
    void operator()( const E_UnitCapaciteMission& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_TroopHealthState >
{
    void operator()( const E_TroopHealthState& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_HumanWound >
{
    void operator()( const E_HumanWound& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_LogRavitaillementTraitementEtat >
{
    void operator()( const E_LogRavitaillementTraitementEtat& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_LogSanteTraitementEtat >
{
    void operator()( const E_LogSanteTraitementEtat& e, Displayer_ABC& displayer ) const;
};


template< >
struct Formatter< E_LogMaintenanceTraitementEtat >
{
    void operator()( const E_LogMaintenanceTraitementEtat& e, Displayer_ABC& displayer ) const;
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
