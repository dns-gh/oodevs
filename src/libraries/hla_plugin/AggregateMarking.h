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
        // NOTHING
    }

    explicit RprMarking( const std::string& name )
        : markingType_( 1 ) // ASCII
    {
        ::memset( markingData_, 0, sizeof( markingData_ ) );
        const std::size_t length = std::min( sizeof( markingData_ ), name.length() );
        ::memcpy( markingData_, name.c_str(), length );
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
        return std::string( reinterpret_cast< const char* >( markingData_ ) );
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
