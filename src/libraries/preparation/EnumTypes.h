// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __EnumTypes_h_
#define __EnumTypes_h_

#include "ENT/ENT_Tr.h"

// $$$$ SBO 2007-05-15: piece of shit

template< typename T >
struct Enum_ABC
{
    Enum_ABC( const T& value ) : value_( value ) {}
    const T GetValue() const { return value_; }

    virtual QString ToString( ENT_Tr::E_Conversion conversion = ENT_Tr::eToTr ) const = 0;

protected:
    T value_;
};

#define DECLARE_ENUMTYPE( name ) \
struct Enum_##name : public Enum_ABC< E_##name > { \
    Enum_##name( int value ) : Enum_ABC< E_##name >( (E_##name)value ) {}\
    Enum_##name( const E_##name& value ) : Enum_ABC< E_##name >( value ) {}\
    Enum_##name( const QString& name ) : Enum_ABC< E_##name >( ENT_Tr::ConvertTo##name( name.ascii() ) ) {}\
    virtual QString ToString( ENT_Tr::E_Conversion conversion = ENT_Tr::eToTr ) const { return ENT_Tr::ConvertFrom##name( value_, conversion ).c_str(); } \
    static int max() { return int( eNbr##name ); } \
};

DECLARE_ENUMTYPE( PopulationAttitude )
DECLARE_ENUMTYPE( TypeObstacle )

#endif // __EnumTypes_h_
