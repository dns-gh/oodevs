// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Connector_h_
#define __Connector_h_

#include "ESRI.h"
#include "dispatcher/Publisher_ABC.h"
#include "game_asn/Asn.h"
#include "Resolver_ABC.h"

namespace kernel
{
    class AgentType;
}

namespace dispatcher
{
namespace esri
{
    class ScopeEditor;
    class World;

// =============================================================================
/** @class  Connector
    @brief  Connector    
*/
// Created: JCR 2007-04-30
// =============================================================================
class Connector : public Publisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
            Connector( const std::string& geodatabase, const kernel::Resolver_ABC< kernel::AgentType >& resolver, const World& world );
    virtual ~Connector();
    //@}
    
    //! @name 
    //@{
    void Send( const ASN1T_MsgsSimToClient& msg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Connector( const Connector& );            //!< Copy constructor
    Connector& operator=( const Connector& ); //!< Assignement operator
    //@}
    
    //! @name 
    //@{
    void ConnectToGeodatabase( const std::string& geodatabase );    
    void ClearFeatureClass( IFeatureClassPtr spFeatureClass );
    //@}

    //! @name 
    //@{
    void Send( ScopeEditor& editor, const ASN1T_MsgLimaCreation& msg );
    void Send( ScopeEditor& editor, const ASN1T_MsgLimitCreation& msg );
    void Send( ScopeEditor& editor, const ASN1T_MsgPionCreation& msg );
    void Send( ScopeEditor& editor, const ASN1T_MsgUnitAttributes& msg );
    void Send( ScopeEditor& editor, const ASN1T_MsgObjectCreation& msg );
    void Send( ScopeEditor& editor, const ASN1T_MsgObjectUpdate& msg );
    //@}
        
private:
    //! @name Member data
    //@{
    IWorkspacePtr           spWorkspace_;
    IFeatureWorkspacePtr    spFeatureWorkspace_;
    IFeatureClassPtr        spUnitFeatureClass_;  // $$$$ JCR 2007-04-30: use FeatureClassDispatcher    
    IFeatureClassPtr        spLimitFeatureClass_;  // $$$$ JCR 2007-04-30: use FeatureClassDispatcher    
    IFeatureClassPtr        spLimaFeatureClass_;  // $$$$ JCR 2007-04-30: use FeatureClassDispatcher    
    IFeatureClassPtr        spObjectFeatureClass_;  // $$$$ JCR 2007-04-30: use FeatureClassDispatcher    
    //@}
    
    //! @name 
    //@{
    const World& world_;
    const kernel::Resolver_ABC< kernel::AgentType >& resolver_;
    //@}
};

}
}

#endif // __Connector_h_
