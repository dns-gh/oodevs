// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MIL_MessageParameters_h_
#define MIL_MessageParameters_h_

// BAX migrate everybody to parameters::Check
// and remove this include and the following macros
#include "tools/NET_AsnException.h"
#define STR( WHAT ) static_cast< std::stringstream& >( std::stringstream() << WHAT ).str()

#define MASA_BADPARAM_MAGICACTION( reason )\
    MASA_BADPARAM_ASN( sword::MagicActionAck::ErrorCode,\
        sword::MagicActionAck::error_invalid_parameter,\
        STR( reason ) )

#define MASA_BADPARAM_UNIT( reason )\
    MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode,\
        sword::UnitActionAck::error_invalid_parameter,\
        STR( reason ) )

namespace sword
{
    class MissionParameters;
    class Point;
}

namespace boost
{
    template< typename T > class function;
}

namespace google
{
namespace protobuf
{
    class EnumDescriptor;
}
}

class DEC_Model_ABC;

namespace parameters
{
    void         Check( bool valid, const std::string& msg, int i = -1, int j = -1, int k = -1 );
    void         CheckCount( const sword::MissionParameters& params, int min, int max = 0 );
    void         CheckCount( int i, const sword::MissionParameters& params, int min, int max = 0 );
    void         CheckCount( int i, int j, const sword::MissionParameters& params, int min, int max = 0 );
    int          GetCount( const sword::MissionParameters& params, int i = -1, int j = -1 );

    std::string  GetString( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    bool         GetBool( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    int          GetQuantity( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    int          GetEnumeration( const google::protobuf::EnumDescriptor* descriptor, const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    sword::Point GetPoint( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    int          GetIdentifier( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );

    typedef boost::function< const DEC_Model_ABC*( const std::string& model ) > ModelFinder;
    const DEC_Model_ABC* GetModel( const sword::MissionParameters& params, const ModelFinder& finder );
}

#define GET_ENUMERATION( ENUM, I, ... ) static_cast< ENUM >( parameters::GetEnumeration( ENUM ## _descriptor(), I, __VA_ARGS__ ) )

#endif // MIL_MessageParameters_h_
