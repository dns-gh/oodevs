// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Formatter_h_
#define __Formatter_h_

#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qlocale.h>
#pragma warning( pop )
#include "DictionaryExtensions.h"
#include "Displayer_ABC.h"
#include "Types.h"
#include "SubTypes.h"

namespace sword
{
    enum LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus;
}

namespace kernel
{
    class UrbanKnowledge_ABC;
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
struct NumberFormatter
{
    template< typename T >
    void operator()( const T& value, Displayer_ABC& displayer ) const
    {
        static QLocale locale;
        displayer.AddToDisplay( locale.toString( value ) );
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
struct Formatter< long > : public NumberFormatter {};
template< >
struct Formatter< unsigned long > : public NumberFormatter
{
    void operator()( const unsigned long& value, Displayer_ABC& displayer ) const
    {
        static QLocale locale;
        displayer.AddToDisplay( locale.toString( static_cast< const unsigned int >( value ) ) );
    }
};
template< >
struct Formatter< char >
{
    void operator()( const char& value, Displayer_ABC& displayer ) const;
};
template< >
struct Formatter< const char* >
{
    void operator()( const char* value, Displayer_ABC& displayer ) const;
};
template< unsigned int N >
struct Formatter< const char [N] > : public Formatter< const char* > { };

template< >
struct Formatter< std::string >
{
    void operator()( const std::string& value, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< QDateTime >
{
    void operator()( const QDateTime& value, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< QTime >
{
    void operator()( const QTime& value, Displayer_ABC& displayer ) const;
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
    void operator()( const ValueNotSet& , Displayer_ABC& displayer ) const;
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

template< typename T, typename U >
struct Formatter< StrongType< T, U > > : public Formatter< const T* >
{
    void operator()( const StrongType< T, U >& element, Displayer_ABC& displayer ) const
    {
        displayer.AddToDisplay( (const T&)( element ) );
    }
};

template< typename T >
struct Formatter< StrongType< T, LogisticBaseSuperior_ > > : public Formatter< const T* >
{
    void operator()( const StrongType< T, LogisticBaseSuperior_ >& element, Displayer_ABC& displayer ) const
    {
        if( element )
        {
            if( const DictionaryExtensions* dico = element->Retrieve< DictionaryExtensions >() )
            {
                const std::string& value = dico->GetValue( "NomLong" );
                if( !value.empty() )
                {
                    displayer.AddToDisplay( QString( value.c_str() ) );
                    return;
                }
            }
        }
        displayer.AddToDisplay( (const T&)( element ) );
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
        displayer.AddToDisplay( element.unit_ );
    }
};

class Unit;
template< >
struct Formatter< Unit >
{
    void operator()( const Unit& element, Displayer_ABC& displayer ) const;
};

// =============================================================================
/** @class  Formatter
    @brief  Urban
*/
// Created: MGD 2009-12-11
// =============================================================================
template< >
struct Formatter< kernel::UrbanKnowledge_ABC >
{
    void operator()( const kernel::UrbanKnowledge_ABC& value, Displayer_ABC& displayer ) const;
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
struct Formatter< E_OperationalStatus >
{
    void operator()( const E_OperationalStatus& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_ForceRatioStatus >
{
    void operator()( const E_ForceRatioStatus& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_Roe >
{
    void operator()( const E_Roe& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_PopulationRoe >
{
    void operator()( const E_PopulationRoe& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_MeetingEngagementStatus >
{
    void operator()( const E_MeetingEngagementStatus& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_FireAvailability >
{
    void operator()( const E_FireAvailability& e, Displayer_ABC& displayer ) const;
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
struct Formatter< E_UnitExperience >
{
    void operator()( const E_UnitExperience& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_UnitStress >
{
    void operator()( const E_UnitStress& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_UnitMorale >
{
    void operator()( const E_UnitMorale& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_UnitTiredness >
{
    void operator()( const E_UnitTiredness& e, Displayer_ABC& displayer ) const;
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
struct Formatter< E_HumanRank >
{
    void operator()( const E_HumanRank& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_LogSupplyHandlingStatus >
{
    void operator()( const E_LogSupplyHandlingStatus& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_LogMedicalHandlingStatus >
{
    void operator()( const E_LogMedicalHandlingStatus& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus >
{
    void operator()( const sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< E_LogFuneralHandlingStatus >
{
    void operator()( const E_LogFuneralHandlingStatus& e, Displayer_ABC& displayer ) const;
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
    void operator()( const Separator& , Displayer_ABC& displayer ) const;
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

}

#endif // __Formatter_h_
