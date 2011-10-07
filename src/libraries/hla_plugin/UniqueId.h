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
class UniqueId
{
public:
    //! @name Constructors/Destructor
    //@{
             UniqueId();
    explicit UniqueId( const std::string& identifier );
    virtual ~UniqueId();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        for( unsigned i = 0; i < 11; ++i )
            archive << identifier_[ i ];
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        for( unsigned i = 0; i < 11; ++i )
            archive >> identifier_[ i ];
    }
    std::string str() const;
    //@}

private:
    //! @name Member data
    //@{
    char identifier_[ 11 ];
    //@}
};

}
}

#endif // plugins_hla_UniqueId_h
