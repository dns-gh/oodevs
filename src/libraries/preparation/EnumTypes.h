// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EnumTypes_h_
#define __EnumTypes_h_

#include "ENT/ENT_Tr.h"

template< typename T >
struct Enum_ABC
{
    Enum_ABC( const T& value ) : value_( value ) {}
    const T GetValue() const { return value_; }

    virtual QString     ToString() const = 0;
    virtual std::string ToXml() const = 0;

protected:
    T value_;
};

#define DECLARE_ENUMTYPE( name )                                                                        \
struct Enum_##name : public Enum_ABC< E_##name > {                                                      \
    Enum_##name( int value ) : Enum_ABC< E_##name >( (E_##name)value ) {}                               \
    Enum_##name( const E_##name& value ) : Enum_ABC< E_##name >( value ) {}                             \
    Enum_##name( const std::string& xml ) : Enum_ABC< E_##name >( ENT_Tr::ConvertTo##name( xml ) )      \
    { if( value_ == -1 ) throw std::runtime_error( "Invalid " #name " value '" + xml + "'" ); }         \
    virtual QString ToString() const { return ENT_Tr::ConvertFrom##name( value_, ENT_Tr::eToTr ).c_str(); }     \
    virtual std::string ToXml() const { return ENT_Tr::ConvertFrom##name( value_, ENT_Tr::eToSim ); }   \
    static int max() { return int( eNbr##name ); }                                                      \
};

DECLARE_ENUMTYPE( PopulationAttitude )
DECLARE_ENUMTYPE( DemolitionTargetType )
DECLARE_ENUMTYPE( NbcState )

#endif // __EnumTypes_h_
