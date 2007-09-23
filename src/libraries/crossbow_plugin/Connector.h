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
    bool IsLocked() const;
    void Unlock();
    ITablePtr GetTable( const std::string& name );
	void Initialize();
	void Finalize();
    //@}

    //! @name
    //@{
    void VisitModel( dispatcher::ModelVisitor_ABC& visitor );
    virtual void Send( const ASN1T_MsgsSimToClient&            msg );
    virtual void Send( const ASN1T_MsgsAuthenticationToClient&    ) {}
    virtual void Send( const ASN1T_MsgsReplayToClient&            ) {}
    virtual void Send( const ASN1T_MsgsAarToClient& ) {}
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Connector( const Connector& );            //!< Copy constructor
    Connector& operator=( const Connector& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void             ConnectToGeodatabase( const std::string& geodatabase );
    void             CheckOutLicences( esriLicenseProductCode eProcuct );
    void             LoadSpatialReference();
    IFeatureClassPtr LoadFeatureClass( const std::string& feature, bool clear );
    IFeatureClassPtr GetFeatureClass( const std::string& feature, bool clear = true );
    IFeatureClassPtr GetObjectFeatureClass( const ASN1T_Location& location );
    void             ClearFeatureClass( IFeatureClassPtr spFeatureClass );
    void             SetSpatialReference( IFeatureClassPtr spFeatureClass );
    void             InitializeStructure();
    //@}

    //! @name Message handlers
    //@{
    void Delete( const ASN1T_MsgObjectDestruction& msg );

    template< typename Message >
    void Create( IFeatureClassPtr spFeature, const Message& asn );
    template< typename Message >
    void Create( ITablePtr spTable, const Message& asn );
    template< typename Message >
    void Delete( IFeatureClassPtr spFeature, const Message& asn );
    template< typename Message >
    void Update( IFeatureClassPtr pFeatureClass, const Message& asn );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, IFeatureClassPtr > T_FeatureMap;
    typedef std::map< std::string, ITablePtr >        T_Tables;
    //@}

private:
    //! @name Member data
    //@{
    IAoInitializePtr        spLicenseHandler_;
    IFeatureWorkspacePtr    spWorkspace_;
    T_FeatureMap            features_;
    T_Tables                tables_;
    ISpatialReferencePtr    spSpatialReference_;
    //@}

    //! @name
    //@{
    const dispatcher::Model&       model_;
    std::auto_ptr< ReportFactory > reportFactory_;
    std::auto_ptr< FolkManager >   folk_;
    std::auto_ptr< ScopeEditor >   scopeEditor_;
    bool                           locked_;
    //@}
};

}

#endif // __Connector_h_
