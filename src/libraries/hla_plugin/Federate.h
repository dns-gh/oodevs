// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Federate_h
#define plugins_hla_Federate_h

#include "Federate_ABC.h"
#include <memory>

namespace hla
{
    class Federate;
    class RtiAmbassador_ABC;
    class Time_ABC;
    class TimeInterval_ABC;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  Federate
    @brief  Federate
*/
// Created: SLI 2011-06-10
// =============================================================================
class Federate : public Federate_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Federate( ::hla::RtiAmbassador_ABC& ambassador, const std::string& name, const ::hla::Time_ABC& time, const ::hla::TimeInterval_ABC& lookAhead );
    virtual ~Federate();
    //@}

    //! @name Operations
    //@{
    virtual bool Connect();
    virtual void Disconnect();

    virtual bool Create( const std::string& federation, const std::string& fomFile );
    virtual bool Create( const std::string& federation, const T_FomFiles& fomFiles );
    virtual bool Destroy( const std::string& federation );

    virtual bool Join( const std::string& federation, bool timeConstrained, bool timeRegulating );
    virtual void Resign();

    virtual void Step();
    virtual void Tick();

    virtual void Register( const ::hla::ClassIdentifier& classID, ::hla::Class_ABC& objectClass, bool publish, bool subscribe );
    virtual void Register( const ::hla::InteractionIdentifier& interactionID, ::hla::Interaction_ABC& interactionClass, bool publish, bool subscribe );

    virtual void Register( ::hla::FederateAmbassador_ABC& listener );

    const ::hla::FederateIdentifier& GetFederateHandle() const;

    // Ownership control
    virtual void DivestRequest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag );
    virtual void UnconditionalDivest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes );
    virtual void AcquisitionRequest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes );
    virtual void UnconditionalAcquisition( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< ::hla::Federate > federate_;
    //@}
};

}
}

#endif // plugins_hla_Federate_h
