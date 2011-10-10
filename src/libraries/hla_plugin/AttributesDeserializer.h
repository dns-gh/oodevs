// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AttributesDeserializer_h
#define plugins_hla_AttributesDeserializer_h

#include <boost/noncopyable.hpp>
#include <map>
#include <boost/function.hpp>

namespace hla
{
    class Deserializer;
}

namespace plugins
{
namespace hla
{
    class RemoteAgentListener_ABC;

// =============================================================================
/** @class  AttributesDeserializer
    @brief  Attributes deserializer
*/
// Created: SLI 2011-10-10
// =============================================================================
class AttributesDeserializer : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef boost::function< void( ::hla::Deserializer&, const std::string&, RemoteAgentListener_ABC& ) > T_Notification;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             AttributesDeserializer( const std::string& identifier, RemoteAgentListener_ABC& listener );
    virtual ~AttributesDeserializer();
    //@}

    //! @name Operations
    //@{
    void Register( const std::string& attribute, T_Notification notification );
    void Deserialize( const std::string& identifier, ::hla::Deserializer& deserializer );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, T_Notification > T_Notifications;
    //@}

private:
    //! @name Member data
    //@{
    const std::string identifier_;
    RemoteAgentListener_ABC& listener_;
    T_Notifications notifications_;
    //@}
};

}
}

#endif // plugins_hla_AttributesDeserializer_h
