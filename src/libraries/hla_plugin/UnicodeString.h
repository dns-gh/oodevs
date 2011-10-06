// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_UnicodeString_h
#define plugins_hla_UnicodeString_h

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  UnicodeString
    @brief  Unicode string
*/
// Created: SLI 2011-06-23
// =============================================================================
class UnicodeString
{
public:
    //! @name Constructors/Destructor
    //@{
             UnicodeString();
    explicit UnicodeString( const std::string& data );
    virtual ~UnicodeString();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << static_cast< uint32 >( data_.size() );
        for( std::wstring::size_type i = 0; i < data_.size(); ++i )
            archive << static_cast< uint16 >( data_[ i ] );
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        uint32 size;
        archive >> size;
        data_.resize( size );
        for( unsigned int i = 0; i < size; ++i )
        {
            uint16 character;
            archive >> character;
            data_[ i ] = character;
        }
    }
    std::string str() const;
    //@}

private:
    //! @name Member data
    //@{
    std::wstring data_;
    //@}
};

}
}

#endif // plugins_hla_UnicodeString_h
