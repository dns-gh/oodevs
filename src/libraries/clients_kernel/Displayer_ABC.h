// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Displayer_ABC_h_
#define __Displayer_ABC_h_

#include "DictionaryExtensions.h"
#include "Types.h"
#include "SubTypes.h"
#include "tools/VirtualTemplate.h"

#include <boost/noncopyable.hpp>

#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qlocale.h>
#pragma warning( pop )

class QString;

namespace sword
{
    enum LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus;
    enum LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus;
    enum LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus;
    enum LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus;
}

namespace kernel
{

// =============================================================================
/** @class  Displayer_ABC
    @brief  Displayer_ABC
*/
// Created: AGE 2006-02-21
// =============================================================================
class Displayer_ABC : public tools::VirtualTemplate< Displayer_ABC >
                    , private boost::noncopyable
{
    friend tools::VirtualTemplate< Displayer_ABC >;

public:
    //! @name Constructors/Destructor
    //@{
             Displayer_ABC();
    virtual ~Displayer_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Clear() = 0;
    virtual void Hide() = 0;
    virtual bool IsValid() const;
    template< typename T >
    Displayer_ABC& Display( const T& element );

    template< typename T >
    Displayer_ABC& Display( const QString& name, const T& element );

    template< typename T >
    Displayer_ABC& Start( const T& element );
    template< typename T >
    Displayer_ABC& Add( const T& element );
    Displayer_ABC& End();

    template< typename T >
    void AddToDisplay( const T& element );
    void AddToDisplay( const QString& element );

    virtual Displayer_ABC& Group( const QString& name ) { return SubItem( name ); };
    virtual Displayer_ABC& Item( const QString& name ) { return SubItem( name ); };
    //@}

protected:
    //! @name Helpers
    //@{
    template< typename T >
    void DisplayNotImplemented()
    {
        FormatterNotImplemented().Error( *this, typeid( T ) );
    }
    Displayer_ABC& NotToBeCalled( const char* ) const;
    //@}

private:
    //! @name Helpers
    //@{
    template< typename T >
    void Call( const T& element );
    //@}

    //! @name Operations
    //@{
    virtual Displayer_ABC& SubItem( const QString& name ) = 0;
    virtual void StartDisplay() = 0;
    virtual void DisplayFormatted( const QString& formatted ) = 0;
    virtual void EndDisplay() = 0;
    //@}
};

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
template< typename T >
inline
Displayer_ABC& Displayer_ABC::Display( const T& element )
{
    StartDisplay();
    AddToDisplay( element );
    EndDisplay();
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
template< typename T >
inline
Displayer_ABC& Displayer_ABC::Display( const QString& name, const T& element )
{
    try
    {
        SubItem( name ).Display( element );
    }
    catch( const std::exception& )
    {
    }
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::Start
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
template< typename T >
Displayer_ABC& Displayer_ABC::Start( const T& element )
{
    StartDisplay();
    AddToDisplay( element );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::Add
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
template< typename T >
Displayer_ABC& Displayer_ABC::Add( const T& element )
{
    AddToDisplay( element );
    return *this;
};

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::Call
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
template< typename T >
void Displayer_ABC::Call( const T& element )
{
    Formatter< T >()( element, *this );
}

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::AddToDisplay
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
template< typename T >
void Displayer_ABC::AddToDisplay( const T& element )
{
    tools::VirtualTemplate< Displayer_ABC >::Call( element );
}

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
struct Formatter< kernel::Duration >
{
    void operator()( const kernel::Duration& value, Displayer_ABC& displayer ) const;
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
struct Formatter< sword::LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus >
{
    void operator()( const sword::LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< sword::LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus >
{
    void operator()( const sword::LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus >
{
    void operator()( const sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus& e, Displayer_ABC& displayer ) const;
};

template< >
struct Formatter< sword::LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus >
{
    void operator()( const sword::LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus& e, Displayer_ABC& displayer ) const;
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

#endif // __Displayer_ABC_h_
