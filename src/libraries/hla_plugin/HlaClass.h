// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_HlaClass_h
#define plugins_hla_HlaClass_h

#include "RemoteAgentSubject_ABC.h"
#include "HlaClass_ABC.h"
#include "rpr/ForceIdentifier.h"
#include <hla/ObjectRegistration_ABC.h>
#include <hla/AttributeIdentifier.h>
#include <boost/shared_ptr.hpp>
#include <map>
#include <set>

namespace hla
{
    template< typename T > class Class;
}

namespace rpr
{
    class EntityType;
}

namespace plugins
{
namespace hla
{
    class Agent_ABC;
    class HlaObject_ABC;
    class HlaObjectFactory_ABC;
    class Federate_ABC;
    class ClassBuilder_ABC;
    class LocalAgentResolver_ABC;
    class RemoteHlaObjectFactory_ABC;
    class ClassListener_ABC;
    class ClassListenerComposite;
    class HlaObjectNameFactory_ABC;
    class OwnershipStrategy_ABC;

// =============================================================================
/** @class  HlaClass
    @brief  Aggregate entity class
*/
// Created: AGE 2008-02-22
// =============================================================================
class HlaClass : public RemoteAgentSubject_ABC
                , private HlaClass_ABC
                , private ::hla::ObjectRegistration_ABC< HlaObject_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             HlaClass( Federate_ABC& federate, LocalAgentResolver_ABC& resolver, const HlaObjectNameFactory_ABC& nameFactory,
                       std::unique_ptr< HlaObjectFactory_ABC > factory, std::unique_ptr< RemoteHlaObjectFactory_ABC > remoteFactory,
                       std::unique_ptr< ClassBuilder_ABC > builder, OwnershipStrategy_ABC& ownershipStrategy );
    virtual ~HlaClass();
    //@}

    //! @name Operations
    //@{
    virtual void Register( ClassListener_ABC& listener );
    virtual void Unregister( ClassListener_ABC& listener );
    void Created( Agent_ABC& agent, unsigned long identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol, const std::vector< char >& uniqueId );
    //@}

private:
    //! @name Operations
    //@{
    virtual HlaObject_ABC& Create( const ::hla::ObjectIdentifier& objectID, const std::string& objectName );
    virtual void Destroy( HlaObject_ABC& object );
    virtual void Reflected( HlaObject_ABC& object );
    virtual bool RequestConfirmDivestiture( const ::hla::ObjectIdentifier& objectID, const HlaObject_ABC& object, const T_AttributeIdentifiers& attributes );
    virtual void OwnershipAcquisitionNotification( const ::hla::ObjectIdentifier& objectID, const HlaObject_ABC& object, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag );
    virtual bool RequestOwnershipAssumption( const ::hla::ObjectIdentifier& objectID, const HlaObject_ABC& object, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag );
    virtual void RequestOwnershipRelease( const ::hla::ObjectIdentifier& objectID, const HlaObject_ABC& object, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag );
    virtual void Divest(const std::string& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag );
    virtual void Acquire(const std::string& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag );
    virtual const T_AttributeIdentifiers& GetAttributes() const;
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< HlaObject_ABC > T_Entity;
    typedef std::set< boost::shared_ptr< HlaObject_ABC  > > T_EntitySet;
    typedef std::map< std::string, T_Entity > T_Entities;
    typedef std::map< std::string, unsigned long > T_HlaIdentifiers;
    typedef std::set< unsigned long > T_IdentifierSet;
    //@}

private:
    //! @name Member data
    //@{
    Federate_ABC& federate_;
    LocalAgentResolver_ABC& resolver_;
    const HlaObjectNameFactory_ABC& nameFactory_;
    std::unique_ptr< HlaObjectFactory_ABC > factory_;
    std::unique_ptr< RemoteHlaObjectFactory_ABC > remoteFactory_;
    OwnershipStrategy_ABC& ownershipStrategy_;
    T_Entities localEntities_;
    T_Entities remoteEntities_;
    T_EntitySet garbageRemotes_;
    std::unique_ptr< ClassListenerComposite > pListeners_;
    std::unique_ptr< ::hla::Class< HlaObject_ABC > > hlaClass_;
    T_AttributeIdentifiers attributes_;
    T_HlaIdentifiers hlaIdentifiers_;
    T_IdentifierSet divesting_;
    T_IdentifierSet acquiring_;
    //@}
};

}
}

#endif // plugins_hla_HlaClass_h
