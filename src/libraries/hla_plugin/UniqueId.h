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

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

namespace plugins
{
namespace hla
{

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
    explicit UniqueIdBase( const std::string& identifier )
    {
        ::memset( identifier_, 0, sizeof( identifier_ ) );
        const std::size_t length = std::min( sizeof( identifier_ ), identifier.length() );
        ::memcpy( identifier_, identifier.c_str(), length );
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
    UniqueIdSerializer( int netnVersion );
    ~UniqueIdSerializer();

    template< typename Archive >
    void operator()( boost::shared_ptr< std::string > id, Archive& serializer ) const
    {
        Serialize( *id, serializer );
    }
    template< typename Archive >
    void operator()( const boost::shared_ptr< std::vector< std::string > >& id, Archive& serializer ) const
    {
        BOOST_FOREACH( std::string v, *id )
            Serialize( v, serializer );
    }
private:
    template< typename Archive>
    void Serialize( const std::string& v, Archive& serializer ) const
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
            throw std::runtime_error( "NETN version not recognized" );
        }
    }

private:
    int netnVersion_;
};

}
}

#endif // plugins_hla_UniqueId_h
