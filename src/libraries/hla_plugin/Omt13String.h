// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Omt13String_h
#define plugins_hla_Omt13String_h

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  Omt13String
    @brief  Omt13String
*/
// Created: SLI 2011-09-22
// =============================================================================
class Omt13String
{
public:
    //! @name Constructors/Destructor
    //@{
             Omt13String();
    explicit Omt13String( const std::string& data );
    virtual ~Omt13String();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        for( std::string::size_type i = 0; i < data_.size(); ++i )
            archive << data_[ i ];
        archive << static_cast< char >( 0 );
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        int8 data = -1;
        while( data != 0 )
        {
            archive >> data;
            if( data != 0 )
                data_.push_back( data );
        }
    }
    std::string str() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string data_;
    //@}
};

}
}

#endif // plugins_hla_Omt13String_h
