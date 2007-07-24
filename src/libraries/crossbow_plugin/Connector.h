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
    class AutomatType;
}

namespace dispatcher
{
    class Config;
    class Model;
    class ModelVisitor_ABC;
}

namespace crossbow
{
    class ScopeEditor;

// =============================================================================
/** @class  Connector
    @brief  Connector    
*/
// Created: JCR 2007-04-30
// =============================================================================
class Connector : public dispatcher::Publisher_ABC
{    
public:
    //! @name Constructors/Destructor
    //@{
             Connector( const dispatcher::Config& config, const kernel::Resolver_ABC< kernel::AgentType >& agentTypes, const kernel::Resolver_ABC< kernel::AutomatType >& automatTypes, const dispatcher::Model& model );
    virtual ~Connector();
    //@}
    
    //! @name Operations
    //@{
    void Lock();
    void Unlock();
    bool IsLocked() const;
    ITablePtr GetTable( const std::string& name );
    //@}

    //! @name 
    //@{ 
    const dispatcher::Model& GetModel() const; // $$$$ SBO 2007-07-24: ???
    void VisitModel( dispatcher::ModelVisitor_ABC& visitor );
    void Send( const ASN1T_MsgsSimToClient& msg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Connector( const Connector& );            //!< Copy constructor
    Connector& operator=( const Connector& ); //!< Assignement operator
    //@}
    
    //! @name Helpers
    //@{
    void                ConnectToGeodatabase( const std::string& geodatabase );
    void                CheckOutLicences( esriLicenseProductCode eProcuct );
    void                LoadStatialReference( const std::string& feature );
    IFeatureClassPtr    LoadFeatureClass( const std::string& feature, bool clear );
    IFeatureClassPtr    GetFeatureClass( const std::string& feature );
    void                ClearFeatureClass( IFeatureClassPtr spFeatureClass );        
    //@}

    //! @name 
    //@{    
    void Send( IFeatureClassPtr pFeatureClass, const ASN1T_MsgLimaUpdate& msg );    
    void Send( IFeatureClassPtr pFeatureClass, const ASN1T_MsgUnitAttributes& msg );    
    void Send( IFeatureClassPtr pFeatureClass, const ASN1T_MsgUnitKnowledgeUpdate& msg );
    void Send( IFeatureClassPtr pFeatureClass, const ASN1T_MsgReport& msg );
    void Send( const ASN1T_MsgObjectCreation& msg );
    void Send( const ASN1T_MsgObjectDestruction& msg );
    void Send( const ASN1T_MsgObjectUpdate& msg );    
    //@}

    //! @name 
    //@{
    template< typename ASN_Type >
    void Insert( IFeatureClassPtr spFeature, const ASN_Type& asn );
    template< typename ASN_Type >
    void Insert( ITablePtr spTable, const ASN_Type& asn );
    template< typename ASN_Type >
    void Delete( IFeatureClassPtr spFeature, const ASN_Type& asn );
    //@}

private:    
    //! @name 
    //@{
    typedef std::map< std::string, IFeatureClassPtr > T_FeatureMap;
    typedef std::map< std::string, ITablePtr >        T_Tables;
    //@}

private:
    //! @name Member data
    //@{
    IAoInitializePtr        spLicenseHandler_;
    IWorkspacePtr           spWorkspace_;
    IFeatureWorkspacePtr    spFeatureWorkspace_;
    T_FeatureMap            features_;
    IFeatureClassPtr        spUnitFeatureClass_;  // $$$$ JCR 2007-04-30: use FeatureClassDispatcher    
    IFeatureClassPtr        spLimitFeatureClass_;  // $$$$ JCR 2007-04-30: use FeatureClassDispatcher    
    IFeatureClassPtr        spLimaFeatureClass_;  // $$$$ JCR 2007-04-30: use FeatureClassDispatcher    
    IFeatureClassPtr        spObjectFeatureClass_;  // $$$$ JCR 2007-04-30: use FeatureClassDispatcher    
    ISpatialReferencePtr    spSpatialReference_;
    T_Tables                tables_;
    //@}
    
    //! @name 
    //@{
    ScopeEditor*    pScopeEditor_;
    const dispatcher::Model&    model_;
    const kernel::Resolver_ABC< kernel::AgentType >& agents_;
    const kernel::Resolver_ABC< kernel::AutomatType >& automats_;
    //@}
};

}

#endif // __Connector_h_
