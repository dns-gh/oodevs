// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Federate_ABC_h
#define plugins_hla_Federate_ABC_h

#include <hla/AttributeIdentifier.h>
#include <boost/noncopyable.hpp>
#include <string>
#include <vector>

namespace hla
{
    class ClassIdentifier;
    class Class_ABC;
    class ObjectIdentifier;
    class InteractionIdentifier;
    class Interaction_ABC;
    class FederateAmbassador_ABC;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  Federate_ABC
    @brief  Federate definition
*/
// Created: SLI 2011-06-10
// =============================================================================
class Federate_ABC : private boost::noncopyable
{
public:
    typedef std::vector< ::hla::AttributeIdentifier > T_AttributeIdentifiers;
    typedef std::vector< std::string >                T_FomFiles;
    //! @name Constructors/Destructor
    //@{
             Federate_ABC() {}
    virtual ~Federate_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Connect() = 0;
    virtual void Disconnect() = 0;

    virtual bool Create( const std::string& federation, const std::string& fomFile ) = 0;
    virtual bool Create( const std::string& federation, const T_FomFiles& fomFiles ) = 0;
    virtual bool Destroy( const std::string& federation ) = 0;

    virtual bool Join( const std::string& federation, bool timeConstrained, bool timeRegulating ) = 0;
    virtual void Resign() = 0;

    virtual void Step() = 0;
    virtual void Tick() = 0;

    virtual void Register( const ::hla::ClassIdentifier& classID, ::hla::Class_ABC& objectClass, bool publish, bool subscribe ) = 0;
    virtual void Register( const ::hla::InteractionIdentifier& interactionID, ::hla::Interaction_ABC& interactionClass, bool publish, bool subscribe ) = 0;

    virtual void Register( ::hla::FederateAmbassador_ABC& listener ) = 0;

    // Ownership control
    virtual void DivestRequest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes ) = 0;
    virtual void UnconditionalDivest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes ) = 0;
    virtual void AcquisitionRequest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes ) = 0;
    virtual void UnconditionalAcquisition( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes ) = 0;
    //@}
};

}
}

#endif // plugins_hla_Federate_ABC_h
