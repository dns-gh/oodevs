// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetworkCapacity_h_
#define __ResourceNetworkCapacity_h_

#include "MIL.h"
#include "ObjectCapacity_ABC.h"
#include "MIL_StructuralStateNotifier_ABC.h"
#include "knowledge/DEC_Knowledge_Def.h"
#include <boost/serialization/export.hpp>

class PHY_DotationCategory;

namespace google
{
namespace protobuf
{
    template< typename T > class RepeatedPtrField;
}
}

namespace sword
{
    class MissionParameter_Value;
    class ObjectAttributes;
    class UrbanAttributes;
}

namespace resource
{
    class NodeProperties;
}

namespace urban
{
    class ResourceNetworkAttribute;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ResourceNetworkCapacity
    @brief  ResourceNetworkCapacity
*/
// Created: JSR 2010-08-12
// =============================================================================
class ResourceNetworkCapacity : public ObjectCapacity_ABC
                              , public MIL_StructuralStateNotifier_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkCapacity();
    explicit ResourceNetworkCapacity( xml::xistream& xis );
    virtual ~ResourceNetworkCapacity();
    //@}

    //! @name Checkpoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Update( xml::xistream& xis, const MIL_Object_ABC& object );
    void Initialize( const urban::ResourceNetworkAttribute& urbanAttribute );
    void Update( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& list );

    boost::shared_ptr< resource::NodeProperties> GetNodeProperties() const;

    virtual void Register( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;

    virtual void NotifyStructuralStateChanged( float structuralState, const MIL_Object_ABC& object );
    virtual void NotifyFired() {}

    void ActivateAll();
    void SetActivation( unsigned long resourceId, bool activated );
    void CreateLink( unsigned long targetId, unsigned long resourceId, unsigned int production );
    bool DestroyLink( unsigned long targetId, const std::string& resource );
    void AddConsumption( unsigned long resourceId, double consumption );
    float GetConsumptionState( unsigned long resourceId ) const;
    float GetFunctionalState() const;
    void AddProduction( unsigned long resourceId, unsigned int production );
    void DecreaseProduction( unsigned long resourceId, unsigned int production );

    double AddToStock( const PHY_DotationCategory& dotation, double quantity );

    void RegisterNode( unsigned int id );
    void SendState( sword::UrbanAttributes& message ) const;
    bool SendState( sword::ObjectAttributes& asn ) const;
    void SendFullState( sword::UrbanAttributes& message ) const;
    void SendFullState( sword::ObjectAttributes& asn ) const;

    const T_ResourceNetworkVector& GetDECResourceNetworks( unsigned int objectId );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetworkCapacity( const ResourceNetworkCapacity& );            //!< Copy constructor
    ResourceNetworkCapacity& operator=( const ResourceNetworkCapacity& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< resource::NodeProperties > nodeProperties_;
    T_ResourceNetworkVector DECResourceNetworks_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ResourceNetworkCapacity )

#endif // __ResourceNetworkCapacity_h_
