// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnAggregate_h
#define plugins_hla_NetnAggregate_h

#include "HlaObject_ABC.h"
#include "EventListener_ABC.h"
#include "UnicodeString.h"
#include "UniqueId.h"
#include <hla/HLA_Types.h>
#include <memory>

namespace plugins
{
namespace hla
{
    class Agent_ABC;
    class AttributesUpdater;
    class ObjectListener_ABC;
    class ObjectListenerComposite;
    class FOM_Serializer_ABC;


// =============================================================================
/** @class  NetnAggregate
    @brief  Netn aggregate
*/
// Created: SLI 2011-07-26
// =============================================================================
class NetnAggregate : public HlaObject_ABC
                    , private EventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnAggregate( std::auto_ptr< HlaObject_ABC > aggregate, Agent_ABC& agent, const std::string& callsign,
                     const std::string& uniqueIdentifier, const std::string& symbol, FOM_Serializer_ABC& fomSerializer );
             NetnAggregate( std::auto_ptr< HlaObject_ABC > aggregate, const std::string& identifier, FOM_Serializer_ABC& fomSerializer );
    virtual ~NetnAggregate();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer );
    virtual void Register( ObjectListener_ABC& listener );
    virtual void Unregister( ObjectListener_ABC& listener );
    virtual void Attach( Agent_ABC* agent, unsigned long simId );
    virtual void SetIdentifier( const std::string& id );
    virtual const std::string& GetIdentifier() const;
    virtual void ResetAttributes();
    //@}

private:
    //! @name Operations
    //@{
    virtual void SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction );
    virtual void FormationChanged( bool isOnRoad );
    virtual void EquipmentChanged( unsigned int type, const rpr::EntityType& entityType, unsigned int available );
    virtual void EmbarkmentChanged( bool mounted );
    virtual void PlatformAdded( const std::string& name, unsigned int id );

    void RegisterAttributes();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ObjectListenerComposite > listeners_;
    std::auto_ptr< HlaObject_ABC > aggregate_;
    Agent_ABC* agent_;
    FOM_Serializer_ABC& fomSerializer_;
    std::auto_ptr< AttributesUpdater > attributesUpdater_;
    UnicodeString callsign_;
    std::string uniqueId_;
    UnicodeString symbol_;
    double mounted_;
    int8 status_;
    std::string identifier_;

    //@}
};

}
}

#endif // plugins_hla_NetnAggregate_h
