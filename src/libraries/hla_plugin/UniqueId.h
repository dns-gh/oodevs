// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_UniqueId_h
#define plugins_hla_UniqueId_h

#include "SerializationTools.h"
#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>

namespace plugins
{
namespace hla
{

class LocalAgentResolver_ABC;
class CallsignResolver_ABC;

// =============================================================================
/** @class  UniqueId
    @brief  Unique id
*/
// Created: SLI 2011-06-22
// =============================================================================
template< int Number >
class UniqueIdBase
{
public:
    //! @name Constructors/Destructor
    //@{
             UniqueIdBase() { ::memset( identifier_, 0, sizeof( identifier_ ) ); }
    explicit UniqueIdBase( const std::vector< char >& identifier )
    {
        ::memset( identifier_, 0, sizeof( identifier_ ) );
        const std::size_t length = std::min( sizeof( identifier_ ), identifier.size() );
        std::copy( identifier.begin(), identifier.begin() + length, identifier_ );
    }
    virtual ~UniqueIdBase() {}
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        for( unsigned i = 0; i < Number; ++i )
            archive << identifier_[ i ];
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        for( unsigned i = 0; i < Number; ++i )
            archive >> identifier_[ i ];
    }
    std::string str() const
    {
        std::string result;
        for( unsigned int i = 0; i < Number; ++i )
        {
            if( !identifier_[ i ] )
                return result;
            result.push_back( identifier_[ i ] );
        }
        return result;
    }
    void Read( std::vector< char >& identifier ) const
    {
        identifier.resize( Number, 0 );
        std::copy( identifier_, identifier_+Number, identifier.begin() );
    }
    std::vector< char > data() const
    {
        return std::vector< char >( &identifier_[0], &identifier_[0] + Number );
    }

    //@}

private:
    //! @name Member data
    //@{
    char identifier_[ Number ];
    //@}
};

typedef UniqueIdBase< 11 > UniqueId;
typedef UniqueIdBase< 16 > NETN_UUID;

// =============================================================================
/** @class  UniqueIdSerializer
    @brief  UniqueIdSerializer
*/
// Created: AHC 2012-09-03
// =============================================================================
struct UniqueIdSerializer
{
public:
    static std::string GetAgentId( const NETN_UUID& uniqueID, const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver );

public:
    UniqueIdSerializer( int netnVersion );
    ~UniqueIdSerializer();

    template< typename Archive >
    void operator()( boost::shared_ptr< std::vector< char > > id, Archive& serializer ) const
    {
        Serialize( *id, serializer );
    }
    template< typename Archive >
    void operator()( const boost::shared_ptr< std::vector< std::vector< char > > >& id, Archive& serializer ) const
    {
        std::for_each( id->begin(), id->end(), [&](const std::vector< char >& v)
            {
                Serialize( v, serializer );
            });
    }
    template< typename Archive >
    void operator()( const boost::shared_ptr< VariableArray< std::vector< char > > >& id, Archive& serializer ) const
    {
        serializer << static_cast< uint32_t >( id->list.size() );
        std::for_each( id->list.begin(), id->list.end(), [&](const std::vector< char >& v)
            {
                Serialize( v, serializer );
            });
    }
private:
    template< typename Archive>
    void Serialize( const std::vector< char >& v, Archive& serializer ) const
    {
        switch( netnVersion_ )
        {
        case 1:
            {
                UniqueId id( v );
                id.Serialize( serializer );
            }
            break;
        case 2:
            {
                NETN_UUID id( v );
                id.Serialize( serializer );
            }
            break;
        default:
            throw MASA_EXCEPTION( "NETN version not recognized" );
        }
    }

private:
    int netnVersion_;
};

}
}

#endif // plugins_hla_UniqueId_h
