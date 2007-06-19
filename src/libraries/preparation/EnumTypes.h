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

#include "tools.h"
// $$$$ SBO 2007-05-15: piece of shit

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

#define DECLARE_ENUMTYPE( name ) \
struct Enum_##name : public Enum_ABC< E_##name > { \
    Enum_##name( int value ) : Enum_ABC< E_##name >( (E_##name)value ) {}\
    Enum_##name( const E_##name& value ) : Enum_ABC< E_##name >( value ) {}\
    Enum_##name( const QString& name ) : Enum_ABC< E_##name >( tools::name##FromString( name ) ) {}\
    virtual QString ToString() const { return tools::ToString( value_ ); } \
    virtual std::string ToXml() const { return tools::ToXml( value_ ); } \
    static int max() { return int( eNbr##name ); } \
};

DECLARE_ENUMTYPE( PopulationAttitude )
DECLARE_ENUMTYPE( TypeObstacle )

#endif // __EnumTypes_h_
