// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __NodeProperties_h_
#define __NodeProperties_h_

#include "NodeElement.h"
#include "tools/Resolver.h"
#include <boost/serialization/split_member.hpp>

namespace google
{
namespace protobuf
{
    template< typename T > class RepeatedPtrField;
}
}

namespace sword
{
    class ObjectAttributeResourceNetwork;
    class UrbanAttributes_Infrastructures;
}

namespace urban
{
    class ResourceNetworkAttribute;
}

namespace xml
{
    class xistream;
}

namespace resource
{
    class ResourceNetworkModel;
    class ResourceTools_ABC;

// =============================================================================
/** @class  NodeProperties
    @brief  Node properties
*/
// Created: JSR 2010-08-13
// =============================================================================
class NodeProperties : private tools::Resolver< NodeElement >
{
public:
    //! @name Constructors/Destructor
    //@{
             NodeProperties();
    explicit NodeProperties( const ResourceTools_ABC& tools );
             NodeProperties( xml::xistream& xis, const ResourceTools_ABC& tools );
             NodeProperties( const urban::ResourceNetworkAttribute& urbanAttribute, const ResourceTools_ABC& tools );
             NodeProperties( const NodeProperties& from );
    virtual ~NodeProperties();
    //@}

    //! @name Operations
    //@{
    void SetModel( const ResourceNetworkModel& model );
    void SetTools( const ResourceTools_ABC& tools );
    void Update( xml::xistream& xis );
    void Update();
    void Finalize();
    void Push( int quantity, unsigned long resourceId );
    void SetModifier( float modifier );
    void AddConsumption( unsigned long resourceId, unsigned int consumption );
    bool NeedUpdate() const;
    float GetFunctionalState() const;
    float GetConsumptionState( unsigned long resourceId ) const;
    //@}

    //! @name Network
    //@{
    void Serialize( sword::UrbanAttributes_Infrastructures& msg ) const;
    void Serialize( sword::ObjectAttributeResourceNetwork& msg ) const;
    void Update( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& list );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< typename Archive >
    void load( Archive&, const unsigned int );
    template< typename Archive >
    void save( Archive&, const unsigned int ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    NodeProperties& operator=( const NodeProperties& ); //!< Assignment operator
    //@}

private:
    //! @name Helpers
    //@{
    void ReadNode( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    float functionalState_;
    float oldFunctionalState_;
    const ResourceTools_ABC* tools_;
    mutable bool needUpdate_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: NodeProperties::load
// Created: JSR 2010-11-17
// -----------------------------------------------------------------------------
template< typename Archive >
void NodeProperties::load( Archive& file, const unsigned int )
{
    unsigned int nNbr;
    file >> functionalState_
         >> nNbr;
    unsigned long index;
    while( nNbr-- )
    {
        file >> index;
        file >> tools::Resolver< NodeElement >::elements_[ index ];
    }
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::save
// Created: JSR 2010-11-17
// -----------------------------------------------------------------------------
template< typename Archive >
void NodeProperties::save( Archive& file, const unsigned int ) const
{
    unsigned int size = tools::Resolver< NodeElement >::elements_.size();
    file << functionalState_
         << size;
    for( std::map< unsigned long, NodeElement* >::const_iterator it = tools::Resolver< NodeElement >::elements_.begin(); it != tools::Resolver< NodeElement >::elements_.end(); ++it )
        file << it->first
             << it->second;
}

}

#endif // __NodeProperties_h_
