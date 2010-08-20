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
    @brief  NodeElement
*/
// Created: JSR 2010-08-13
// =============================================================================
class NodeElement
{
public:
    //! @name Constructors/Destructor
    //@{
             NodeElement();
    explicit NodeElement( xml::xistream& xis, EResourceType resourceType );
             NodeElement( const NodeElement& from );
    virtual ~NodeElement();
    //@}

    //! @name Operations
    //@{
    void SetModel( const ResourceNetworkModel& model );
    void Update( xml::xistream& xis );
    void UpdateImmediateStock();
    void AddConsumptions( T_Consumptions& consumptions ); 
    bool Consume( int consumption );
    void DistributeResource();
    void Push( int quantity );
    //@}

    //! @name Network
    //@{
    void Serialize( MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork& msg ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< ResourceLink* >      T_ResourceLinks;
    typedef T_ResourceLinks::iterator        IT_ResourceLinks;
    typedef T_ResourceLinks::const_iterator CIT_ResourceLinks;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    NodeElement& operator=( const NodeElement& ); //!< Assignment operator
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
    EResourceType resourceType_;
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
