// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NullTerminatedAsciiString_h
#define plugins_hla_NullTerminatedAsciiString_h

#include <string>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  NullTerminatedAsciiString
    @brief  NullTerminatedAsciiString
*/
// Created: AHC 2012-10-05
// =============================================================================
class NullTerminatedAsciiString
{
public:
    //! @name Constructors/Destructor
    //@{
             NullTerminatedAsciiString();
    explicit NullTerminatedAsciiString( const std::string& data );
    virtual ~NullTerminatedAsciiString();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        for( std::string::size_type i = 0; i < data_.length(); ++i )
            archive << data_[i];
        archive << (char)0;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        char c;
        archive >> c;
        for( ; c != 0; archive >> c )
            data_.push_back( c );
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

#endif // plugins_hla_NullTerminatedAsciiString_h
