// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_AggregateMarking_h
#define plugins_hla_AggregateMarking_h

#include <boost/lexical_cast.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  RprMarking
    @brief  rpr marking
*/
// Created: AGE 2008-02-21
// =============================================================================
template< int Number >
class RprMarking
{
public:
    //! @name Constructors/Destructor
    //@{
    RprMarking()
        : markingType_( 1 ) // ASCII
    {
        ::memset( markingData_, 0, sizeof( markingData_ ) );
    }

    explicit RprMarking( const std::string& name )
        : markingType_( 1 ) // ASCII
    {
        ::memset( markingData_, 0, sizeof( markingData_ ) );
        const std::size_t length = std::min( sizeof( markingData_ ), name.length() );
        ::memcpy( markingData_, name.c_str(), length );
    }

    RprMarking( const std::string& name, unsigned int identifier )
        : markingType_( 1 ) // ASCII
    {
        std::string data = name;
        std::string id = boost::lexical_cast< std::string >( identifier );
        if( name.size() + id.size() > Number )
            data.resize( Number - id.size() );
        data += id;
        ::memset( markingData_, 0, sizeof( markingData_ ) );
        const std::size_t length = std::min( sizeof( markingData_ ), data.length() );
        ::memcpy( markingData_, data.c_str(), length );
    }

    virtual ~RprMarking()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << markingType_
                << markingData_;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> markingType_
                >> markingData_;
    }
    std::string str() const
    {
        std::string result;
        for( unsigned int i = 0; i < Number; ++i )
        {
            if( !markingData_[ i ] )
                return result;
            result.push_back( markingData_[ i ] );
        }
        return result;
    }
    //@}

private:
    //! @name Member data
    //@{
    unsigned char markingType_;
    unsigned char markingData_[Number];
    //@}
};

typedef RprMarking< 31 > AggregateMarking;
typedef RprMarking< 11 > Marking;

}
}

#endif // plugins_hla_AggregateMarking_h
