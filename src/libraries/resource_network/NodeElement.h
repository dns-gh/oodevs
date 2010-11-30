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
#include <map>
#include <vector>
#include <urban/ResourceNetworkAttribute.h>
#include <boost/serialization/split_member.hpp>

namespace Common
{
    class MsgMissionParameter_Value;
    class ResourceNetwork;
}

namespace xml
{
    class xistream;
}

namespace resource
{
class ResourceNetworkModel;

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
             NodeElement( xml::xistream& xis, unsigned long resourceId, const std::string& resourceName );
             NodeElement( const urban::ResourceNetworkAttribute::ResourceNode& node, unsigned long resourceId );
             NodeElement( const NodeElement& from );
    virtual ~NodeElement();
    //@}

public:
    //! @name Operations
    //@{
    void SetModel( const ResourceNetworkModel& model );
    void Update( xml::xistream& xis );
    void UpdateImmediateStock( bool isFunctional );
    void Consume( bool& isFunctional );
    void DistributeResource( bool isFunctional );
    void Push( int quantity );
    void SetModifier( unsigned int modifier );
    bool NeedUpdate() const;
    //@}

    //! @name Network
    //@{
    void Serialize( Common::ResourceNetwork& msg ) const;
    void Update( const Common::MsgMissionParameter_Value& msg );
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
    void DoDistributeResource( T_ResourceLinks& links );
    void ReadLink( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const ResourceNetworkModel* model_;
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
    bool consumptionCritical_;
    double modifier_;
    mutable bool needUpdate_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: NodeElement::load
// Created: JSR 2010-11-17
// -----------------------------------------------------------------------------
template< typename Archive >
void NodeElement::load( Archive& file, const unsigned int )
{
    unsigned int linksSize;
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
    for( unsigned int i = 0; i < linksSize; ++i )
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
    unsigned int linksSize = links_.size();
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
    for( unsigned int i = 0; i < linksSize; ++i )
        file << links_[ i ];
}

}

#endif // __NodeElement_h_
