// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __NodeElement_h_
#define __NodeElement_h_

#include "ResourceLink.h"
#include <urban/ResourceNetworkAttribute.h>
#include <boost/serialization/split_member.hpp>
#include <vector>
#include <map>

namespace sword
{
    class MissionParameter_Value;
    class ResourceNetwork;
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
/** @class  NodeElement
    @brief  Node element
*/
// Created: JSR 2010-08-13
// =============================================================================
class NodeElement
{
public:
    //! @name Constructors/Destructor
    //@{
             NodeElement();
             NodeElement( unsigned long resourceId, const std::string& resourceName );
             NodeElement( xml::xistream& xis, unsigned long resourceId, const std::string& resourceName );
             NodeElement( const urban::ResourceNetworkAttribute::ResourceNode& node, unsigned long resourceId );
             NodeElement( const NodeElement& from );
    virtual ~NodeElement();
    //@}

public:
    //! @name Operations
    //@{
    void Update( xml::xistream& xis );
    void Finalize( const ResourceTools_ABC& tools );
    void AppendResource( std::vector< std::string >& resources );
    void UpdateImmediateStock( float functionalState );
    void AddConsumption( double consumption );
    void Consume( float& functionalState );
    void DistributeResource( float functionalState, const ResourceNetworkModel& model );
    void Push( int quantity );
    void SetActivation( bool activated );
    void SetProduction( unsigned int production );
    void AddProduction( unsigned int production );
    void DecreaseProduction( unsigned int production );
    void CreateLink( unsigned long targetId );
    void SetModifier( float modifier );
    bool RemoveLink( unsigned int nodeId );
    bool NeedUpdate() const;
    float GetFunctionalState() const;
    float GetConsumptionState() const;
    double AddToStock( double quantity );
    //@}

    //! @name Network
    //@{
    void Serialize( sword::ResourceNetwork& msg ) const;
    void Update( const sword::MissionParameter_Value& msg );
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
    NodeElement& operator=( const NodeElement& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< ResourceLink* >      T_ResourceLinks;
    typedef T_ResourceLinks::iterator        IT_ResourceLinks;
    typedef T_ResourceLinks::const_iterator CIT_ResourceLinks;
    //@}

private:
    //! @name Helpers
    //@{
    void DoDistributeResource( T_ResourceLinks& links, const ResourceNetworkModel& model );
    void ReadLink( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    unsigned long resourceId_;
    std::string resourceName_;
    T_ResourceLinks links_;
    bool isActivated_;
    unsigned int productionCapacity_;
    unsigned int stockCapacity_;
    unsigned int stockMaxCapacity_;
    unsigned int immediateStock_;
    unsigned int receivedQuantity_;
    unsigned int consumptionAmount_;
    unsigned int maxConsumption_;
    unsigned int currentConsumption_;
    double externalConsumption_;
    bool consumptionCritical_;
    float modifier_;
    mutable bool needUpdate_;
    bool magicChanged_;
    float functionalState_;
    mutable float oldFunctionalState_;
    float consumptionState_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: NodeElement::load
// Created: JSR 2010-11-17
// -----------------------------------------------------------------------------
template< typename Archive >
void NodeElement::load( Archive& file, const unsigned int )
{
    std::size_t linksSize;
    file >> resourceId_
         >> resourceName_
         >> isActivated_
         >> productionCapacity_
         >> stockCapacity_
         >> stockMaxCapacity_
         >> consumptionAmount_
         >> consumptionCritical_
         >> modifier_
         >> linksSize;
    for( std::size_t i = 0; i < linksSize; ++i )
    {
        ResourceLink* link = 0;
        file >> link;
        links_.push_back( link );
    }
}

// -----------------------------------------------------------------------------
// Name: NodeElement::save
// Created: JSR 2010-11-17
// -----------------------------------------------------------------------------
template< typename Archive >
void NodeElement::save( Archive& file, const unsigned int ) const
{
    std::size_t linksSize = links_.size();
    file << resourceId_
         << resourceName_
         << isActivated_
         << productionCapacity_
         << stockCapacity_
         << stockMaxCapacity_
         << consumptionAmount_
         << consumptionCritical_
         << modifier_
         << linksSize;
    for( std::size_t i = 0; i < linksSize; ++i )
        file << links_[ i ];
}

}

#endif // __NodeElement_h_
