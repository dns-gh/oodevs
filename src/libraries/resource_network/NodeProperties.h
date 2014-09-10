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
#include <tools/Resolver.h>
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
             NodeProperties( const NodeProperties& from );
    virtual ~NodeProperties();
    //@}

    //! @name Operations
    //@{
    void SetTools( const ResourceTools_ABC& tools );
    void GetAvailableResources( std::vector< std::string >& resources ) const;
    void Update( xml::xistream& xis );
    void UpdateState( const ResourceNetworkModel& model );
    void Finalize();
    void Push( int quantity, unsigned long resourceId );
    void ActivateAll();
    void SetActivation( unsigned long resourceId, bool activated );
    NodeElement& CreateNode( unsigned long resourceId );
    void CreateLink( unsigned long targetId, unsigned long resourceId, unsigned int production );
    void SetModifier( float modifier );
    void AddConsumption( unsigned long resourceId, double consumption );
    void RemoveLink( unsigned int nodeId );
    bool DestroyLink( unsigned long targetId, const std::string& resource );
    bool NeedUpdate() const;
    float GetFunctionalState() const;
    float GetConsumptionState( unsigned long resourceId ) const;
    double GetStock( unsigned long resourceId ) const;
    double AddToStock( unsigned long resourceId, double quantity );
    void DecreaseProduction( unsigned long resourceId, unsigned int production );
    void AddProduction( unsigned long resourceId, unsigned int production );
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
    void WriteODB( xml::xostream& xos ) const;
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
    std::size_t nNbr;
    file >> functionalState_;
    file >> nNbr;
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
    std::size_t size = tools::Resolver< NodeElement >::elements_.size();
    file << functionalState_;
    file << size;
    for( auto it = tools::Resolver< NodeElement >::elements_.begin(); it != tools::Resolver< NodeElement >::elements_.end(); ++it )
    {
        file << it->first;
        file << it->second;
    }
}

}

#endif // __NodeProperties_h_
