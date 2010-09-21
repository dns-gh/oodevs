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

#include <map>
#include <vector>
#include <urban/ResourceNetworkAttribute.h>

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
class ResourceLink;
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
    //@}

    //! @name Network
    //@{
    void Serialize( Common::ResourceNetwork& msg ) const;
    void Update( const Common::MsgMissionParameter_Value& msg );
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
    //@}
};

}

#endif // __NodeElement_h_
