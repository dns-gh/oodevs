// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_DictionaryExtensions_h_
#define __MIL_DictionaryExtensions_h_

#include "MIL.h"

namespace sword
{
    class UnitMagicAction;
}

namespace xml
{
    class xistream;
    class xostream;
}

// =============================================================================
/** @class  MIL_DictionaryExtensions
    @brief  MIL_DictionaryExtensions
*/
// Created: ABR 2011-05-11
// =============================================================================
class MIL_DictionaryExtensions : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_DictionaryExtensions();
    explicit MIL_DictionaryExtensions( xml::xistream& xis );
    virtual ~MIL_DictionaryExtensions();
    //@}

    //! @name Operations
    //@{
    void WriteODB( xml::xostream& xos ) const;
    template< typename T >
    void SendFullState( T& msg ) const;
    template< typename T >
    void UpdateNetwork( T& msg );
    void OnReceiveMsgChangeExtensions( const sword::UnitMagicAction& msg );
    bool HasChanged() const;
    bool IsEmpty() const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadExtension( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_Extensions;
    typedef T_Extensions::const_iterator       CIT_Extensions;
    //@}

private:
    //! @name Member data
    //@{
    T_Extensions extensions_;
    bool hasChanged_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_DictionaryExtensions )

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions::SendFullState
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
template< typename T >
void MIL_DictionaryExtensions::SendFullState( T& msg ) const
{
    for( CIT_Extensions it = extensions_.begin(); it != extensions_.end(); ++it )
    {
        sword::Extension_Entry& entry = *msg().mutable_extension()->add_entries();
        entry.set_name( it->first );
        entry.set_value( it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_DictionaryExtensions::UpdateNetwork
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
template< typename T >
void MIL_DictionaryExtensions::UpdateNetwork( T& msg )
{
    if( hasChanged_ )
    {
        msg().mutable_extension();
        SendFullState( msg );
        hasChanged_ = false;
    }
}

#endif // __MIL_DictionaryExtensions_h_
