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
#include "dispatcher/ClientPublisher_ABC.h"
#include "game_asn/Asn.h"

namespace dispatcher
{
    class Config;
    class Model;
    class ModelVisitor_ABC;
}

namespace crossbow
{
    class ScopeEditor;
    class ReportFactory;
    class FolkManager;

// =============================================================================
/** @class  Connector
    @brief  Connector
*/
// Created: JCR 2007-04-30
// =============================================================================
class Connector : public dispatcher::ClientPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Connector( const dispatcher::Config& config, const dispatcher::Model& model );
    virtual ~Connector();
    //@}

    //! @name Operations
    //@{
    void Lock();
    void Unlock();
    ITablePtr GetTable( const std::string& name );
    //@}

    //! @name
    //@{
    void VisitModel( dispatcher::ModelVisitor_ABC& visitor );
    virtual void Send( const ASN1T_MsgsSimToClient&            msg );
    virtual void Send( const ASN1T_MsgsAuthenticationToClient& msg );
    virtual void Send( const ASN1T_MsgsReplayToClient&         msg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Connector( const Connector& );            //!< Copy constructor
    Connector& operator=( const Connector& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    bool IsLocked() const;
    void                ConnectToGeodatabase( const std::string& geodatabase );
    void                CheckOutLicences( esriLicenseProductCode eProcuct );
    void                LoadStatialReference( const std::string& feature );
    IFeatureClassPtr    LoadFeatureClass( const std::string& feature, bool clear );
    IFeatureClassPtr    GetFeatureClass( const std::string& feature );
    void                ClearFeatureClass( IFeatureClassPtr spFeatureClass );
    IFeatureClassPtr    GetObjectFeatureClass( const ASN1T_Location& location );
    //@}

    //! @name
    //@{
    template<typename ASN1T_MsgUpdate >
    void Send( IFeatureClassPtr pFeatureClass, const ASN1T_MsgUpdate& asn );
    void Send( IFeatureClassPtr pFeatureClass, const ASN1T_MsgLimaUpdate& asn );
    void Send( IFeatureClassPtr pFeatureClass, const ASN1T_MsgFolkGraphEdgeUpdate& asn );
    void Send( const ASN1T_MsgObjectUpdate& msg ); 
    void Delete( const ASN1T_MsgObjectDestruction& msg );    
    //@}

    //! @name
    //@{
    template< typename ASN1T_MsgCreation >
    void Insert( IFeatureClassPtr spFeature, const ASN1T_MsgCreation& asn );
    template< typename ASN1T_MsgCreation >
    void Insert( ITablePtr spTable, const ASN1T_MsgCreation& asn );
    template< typename ASN1T_MsgDestruction >
    void Delete( IFeatureClassPtr spFeature, const ASN1T_MsgDestruction& asn );
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
    ScopeEditor*                pScopeEditor_;
    const dispatcher::Model&    model_;
    std::auto_ptr< ReportFactory >  reportFactory_;
    std::auto_ptr< FolkManager >    folk_;
    //@}
};

}

#endif // __Connector_h_
