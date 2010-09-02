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

namespace Common
{
    class MsgObjectAttributeResourceNetwork;
}

namespace MsgsSimToClient
{
    class MsgUrbanAttributes_Infrastructures;
}

namespace xml
{
    class xistream;
}

namespace resource
{
    class ResourceNetworkModel;

// =============================================================================
/** @class  NodeProperties
    @brief  Node properties
*/
// Created: JSR 2010-08-13
// =============================================================================
class NodeProperties : private tools::Resolver< NodeElement, E_ResourceType >
{
public:
    //! @name Constructors/Destructor
    //@{
             NodeProperties();
    explicit NodeProperties( xml::xistream& xis );
             NodeProperties( const NodeProperties& from );
    virtual ~NodeProperties();
    //@}

    //! @name Operations
    //@{
    void SetModel( const ResourceNetworkModel& model );
    void Update( xml::xistream& xis );
    void Update();
    void Push( int quantity, E_ResourceType resourceType );
    void SetModifier( unsigned int modifier );
    //@}

    //! @name Network
    //@{
    void Serialize( MsgsSimToClient::MsgUrbanAttributes_Infrastructures& msg ) const;
    void Serialize( Common::MsgObjectAttributeResourceNetwork& msg ) const;
    void Update( const Common::MsgMissionParameter_Value& msg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    NodeProperties& operator=( const NodeProperties& ); //!< Assignment operator
    //@}

private:
    //! @name Helpers
    //@{
    static E_ResourceType ConvertToResourceType( const std::string& type );
    void ReadNode( xml::xistream& xis );
    void ReadConsumption( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    bool isFunctional_;
    //@}
};

}

#endif // __NodeProperties_h_
