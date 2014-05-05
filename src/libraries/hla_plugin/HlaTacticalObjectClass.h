// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_HLATACTICALOBJECTCLASS_H_
#define plugins_hla_HLATACTICALOBJECTCLASS_H_

#include "HlaClass_ABC.h"
#include "RemoteAgentSubject_ABC.h"
#include "rpr/ForceIdentifier.h"

#include <hla/ObjectRegistration_ABC.h>

#include <boost/shared_ptr.hpp>

#include <map>
#include <memory>

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
    class HlaObject_ABC;
    class HlaObjectNameFactory_ABC;
    class RemoteHlaObjectFactory_ABC;
    class HlaTacticalObjectFactory_ABC;
    class Federate_ABC;
    class ClassBuilder_ABC;
    class TacticalObject_ABC;
    class ClassListenerComposite;

// =============================================================================
/** @class  HlaClass
    @brief  Aggregate entity class
*/
// Created: AGE 2008-02-22
// =============================================================================
class HlaTacticalObjectClass : public HlaClass_ABC
                             , public RemoteAgentSubject_ABC
                             , private ::hla::ObjectRegistration_ABC< HlaObject_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    HlaTacticalObjectClass( Federate_ABC& federate, const HlaObjectNameFactory_ABC& nameFactory, std::unique_ptr< HlaTacticalObjectFactory_ABC > factory
            , std::unique_ptr< RemoteHlaObjectFactory_ABC > remoteFactory, std::unique_ptr< ClassBuilder_ABC > builder );
    virtual ~HlaTacticalObjectClass();
    //@}

    //! @name Operations
    //@{
    virtual void Register( ClassListener_ABC& listener );
    virtual void Unregister( ClassListener_ABC& listener );
    void Created( TacticalObject_ABC& object, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type );
    virtual void Divest(const std::string& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag );
    virtual void Acquire(const std::string& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag );
    virtual const T_AttributeIdentifiers& GetAttributes() const;
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< HlaObject_ABC > T_Entity;
    typedef std::map< std::string, T_Entity > T_Entities;
    typedef std::map< std::string, unsigned long > T_HlaIdentifiers;
    //@}

    //! @name Operations
    //@{
    virtual HlaObject_ABC&   Create ( const ::hla::ObjectIdentifier& objectID, const std::string& objectName );
    virtual void Destroy( HlaObject_ABC& object );

    virtual void Reflected( HlaObject_ABC& object );

    // When returned value is true, object is assumed remote
    virtual bool RequestConfirmDivestiture( const ::hla::ObjectIdentifier& objectID, const HlaObject_ABC& object, const T_AttributeIdentifiers& attributes );
    virtual void OwnershipAcquisitionNotification( const ::hla::ObjectIdentifier& objectID, const HlaObject_ABC& object, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag );
    virtual bool RequestOwnershipAssumption( const ::hla::ObjectIdentifier& objectID, const HlaObject_ABC& object, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag );
    virtual void RequestOwnershipRelease( const ::hla::ObjectIdentifier& objectID, const HlaObject_ABC& object, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag );
    //@}

    //! @name Member data
    //@{
    const HlaObjectNameFactory_ABC& nameFactory_;
    std::unique_ptr< HlaTacticalObjectFactory_ABC > factory_;
    std::unique_ptr< RemoteHlaObjectFactory_ABC > remoteFactory_;
    std::unique_ptr< ::hla::Class< HlaObject_ABC > > hlaClass_;
    T_AttributeIdentifiers attributes_;
    T_Entities localEntities_;
    T_Entities remoteEntities_;
    std::unique_ptr< ClassListenerComposite > pListeners_;
    //@}
};

}
}
#endif // plugins_hla_HLATACTICALOBJECTCLASS_H_
