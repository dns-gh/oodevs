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
class NodeProperties : private tools::Resolver< NodeElement, EResourceType >
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
    void Push( int quantity, EResourceType resourceType );
    //@}

    //! @name Network
    //@{
    void Serialize( MsgsSimToClient::MsgUrbanAttributes_Infrastructures& msg ) const;
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
    void ReadNode( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const ResourceNetworkModel* model_; // useful?
    //@}
};

}

#endif // __NodeProperties_h_
