// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathematiques Appliquees SA (MASA)
//
// *****************************************************************************

#ifndef PROTOCOL_MESSAGE_PARAMETERS_H
#define PROTOCOL_MESSAGE_PARAMETERS_H

#include <tools/Exception.h>
#include <stdint.h>
#include <vector>

#define STR( WHAT ) static_cast< std::stringstream& >( std::stringstream() << WHAT ).str()

namespace boost
{
    template< typename T > class optional;
}

namespace sword
{
    class CoordLatLong;
    class MissionParameters;
    class ParentEntity;
    class Point;
    class Tasker;
    enum Location_Geometry;
}

namespace google
{
namespace protobuf
{
    class EnumDescriptor;
}
}

namespace protocol
{
    struct Exception : public tools::Exception
    {
        Exception( const std::string& file, const std::string& function, int line, const std::string& what );
        Exception( const Exception& other );
        virtual ~Exception() {}
    };

    typedef std::pair< std::string, std::string > Extension;

    uint32_t     GetParentEntityId( const sword::ParentEntity& parent );

    void         Check( bool valid, const std::string& msg, int i = -1, int j = -1, int k = -1 );
    void         Check( const void* pointer, const std::string& msg, int i = -1, int j = -1, int k = -1 );
    int          CheckCount( const sword::MissionParameters& params, int min, int max = 0 );
    int          CheckCount( int i, const sword::MissionParameters& params, int min, int max = 0 );
    int          CheckCount( int i, int j, const sword::MissionParameters& params, int min, int max = 0 );
    int          GetCount( const sword::MissionParameters& params, int i = -1, int j = -1 );

    bool                               IsNull( const sword::MissionParameters& params, int i );
    const std::string&                 GetString( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    bool                               GetBool( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    float                              GetReal( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    const std::string&                 GetDateTimeStr( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    int                                GetHeading( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    int                                GetQuantity( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    boost::optional< int >             TryGetQuantity( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    std::vector< sword::CoordLatLong > GetLocation( const sword::MissionParameters& params, int i );
    sword::Location_Geometry           GetLocationType( const sword::MissionParameters& params, int i );
    int                                GetUnsafeEnumeration( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    int                                GetEnumeration( const google::protobuf::EnumDescriptor* descriptor, const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    const sword::CoordLatLong&         GetPoint( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    uint32_t                           GetIdentifier( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    uint32_t                           GetKnowledgeGroup( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    boost::optional< uint32_t >        TryGetKnowledgeGroup( const sword::MissionParameters& params, int i = -1, int j = -1, int k = -1 );
    uint32_t                           GetAgentId( const sword::MissionParameters& params, int i, int  j = -1, int k = -1 );
    uint32_t                           GetAutomatId( const sword::MissionParameters& params, int i, int  j = -1, int k = -1 );
    uint32_t                           GetFormationId( const sword::MissionParameters& params, int i, int  j = -1, int k = -1 );
    boost::optional< uint32_t >        TryGetFormationId( const sword::MissionParameters& params, int i, int  j = -1, int k = -1 );
    boost::optional< uint32_t >        TryGetPathfindId( const sword::MissionParameters& params, int i, int  j = -1, int k = -1 );
    uint32_t                           GetPartyId( const sword::MissionParameters& params, int i, int  j = -1, int k = -1 );
    boost::optional< uint32_t >        TryGetPartyId( const sword::MissionParameters& params, int i, int  j = -1, int k = -1 );
    uint32_t                           GetResourceType( const sword::MissionParameters& params, int i, int j = -1, int k = -1 );
    std::vector< Extension >           GetExtensionList( const sword::MissionParameters& params, int i );

    boost::optional< uint32_t >        TryGetTasker( const sword::Tasker& tasker );
}

#define GET_ENUMERATION( ENUM, I, ... ) static_cast< ENUM >( protocol::GetEnumeration( ENUM ## _descriptor(), I, __VA_ARGS__ ) )

#endif // PROTOCOL_MESSAGE_PARAMETERS_H
