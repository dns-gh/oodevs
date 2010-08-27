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

#include "Types.h"
#include <map>
#include <vector>

namespace Common
{
    class MsgMissionParameter_Value;
}

namespace MsgsSimToClient
{
    class MsgUrbanAttributes_Infrastructures_ResourceNetwork;
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
             NodeElement( xml::xistream& xis, E_ResourceType resourceType );
             NodeElement( const NodeElement& from );
    virtual ~NodeElement();
    //@}

public:
    //! @name Types
    //@{
    struct Consumption
    {
        int amount_;
        bool critical_;
    };

    typedef std::map< E_ResourceType, Consumption >   T_Consumptions;
    typedef T_Consumptions::iterator                IT_Consumptions;
    typedef T_Consumptions::const_iterator         CIT_Consumptions;
    //@}

public:
    //! @name Operations
    //@{
    static E_ResourceType ConvertToResourceType( const std::string& type );
    void SetModel( const ResourceNetworkModel& model );
    void Update( xml::xistream& xis );
    void UpdateImmediateStock( bool isFunctional );
    void AddConsumptions( T_Consumptions& consumptions ); 
    bool Consume( int consumption );
    void DistributeResource( bool isFunctional );
    void Push( int quantity );
    //@}

    //! @name Network
    //@{
    void Serialize( MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork& msg ) const;
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
    void ReadConsumption( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const ResourceNetworkModel* model_;
    E_ResourceType resourceType_;
    T_ResourceLinks links_;
    T_Consumptions consumptions_;
    bool isActivated_;
    bool isProducer_;
    bool isStockActive_;
    int productionCapacity_;
    unsigned int stockCapacity_;
    int stockMaxCapacity_;
    int immediateStock_;
    int receivedQuantity_;
    //@}
};

}

#endif // __NodeElement_h_
