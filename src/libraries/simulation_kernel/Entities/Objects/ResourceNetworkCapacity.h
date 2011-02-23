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

    virtual void Register( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;

    virtual void NotifyStructuralStateChanged( float structuralState, const MIL_Object_ABC& object );
    virtual void NotifyFired() {}

    void AddConsumption( unsigned long resourceId, double consumption );
    float GetConsumptionState( unsigned long resourceId ) const;
    float GetFunctionalState() const;

    double AddToStock( const PHY_DotationCategory& dotation, double quantity );

    void RegisterNode( unsigned int id );
    void SendState( sword::UrbanAttributes& message ) const;
    void SendState( sword::ObjectAttributes& asn ) const;
    void SendFullState( sword::UrbanAttributes& message ) const;
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
    std::auto_ptr< resource::NodeProperties> nodeProperties_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ResourceNetworkCapacity )

#endif // __ResourceNetworkCapacity_h_
