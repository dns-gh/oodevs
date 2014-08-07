// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASAGroup
//
// *****************************************************************************

#ifndef plugins_hla_AttributesUpdater_h
#define plugins_hla_AttributesUpdater_h

#include "AttributesSerializer.h"
#include "AttributesDeserializer.h"

#include <boost/noncopyable.hpp>

namespace hla
{
    class Deserializer;
}

namespace plugins
{
namespace hla
{

class ObjectListener_ABC;

// =============================================================================
/** @class  AttributesUpdater
    @brief  AttributesUpdater
*/
// Created: AHC 2012-03-06
// =============================================================================
class AttributesUpdater : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::function< void( ::hla::Deserializer_ABC&, const std::string&, ObjectListener_ABC& ) > T_Notification;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    AttributesUpdater( const std::string& identifier, ObjectListener_ABC& listener );
    virtual ~AttributesUpdater();
    //@}

    //! @name Operations
    //@{
    void Deserialize( const std::string& identifier, ::hla::Deserializer_ABC& deserializer );
    void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    template< typename T >
    void Register( const std::string& name, T_Notification notification, const T& value )
    {
        serializer_.Register( name, value );
        deserializer_.Register( name, notification );
    }
    template< typename T, typename F >
    void Register( const std::string& name, T_Notification notification, const T& value, const F& szr )
    {
        serializer_.Register( name, value, szr );
        deserializer_.Register( name, notification );
    }
    template< typename T >
    void Update( const std::string& name, const T& value )
    {
        serializer_.Update(name, value);
    }
    //@}

private:
    AttributesSerializer serializer_;
    AttributesDeserializer deserializer_;
};
}
}

#endif // plugins_hla_AttributesUpdater_h
