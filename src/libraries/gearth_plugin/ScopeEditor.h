// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gearth_ScopeEditor_h_
#define __gearth_ScopeEditor_h_

#include "dispatcher/ClientPublisher_ABC.h"
#include "game_asn/Simulation.h"
#include "game_asn/Messenger.h"
#include <map>

namespace xml
{
    class xobufferstream;
    class xostream;
}

namespace dispatcher
{
    class Config;
    class Model;
    class ModelVisitor_ABC;
}

namespace gearth
{
    class ReportFactory;
    class StyleEditor;

// =============================================================================
/** @class  ScopeEditor
    @brief  ScopeEditor
*/
// Created: JCR 2007-04-30
// =============================================================================
class ScopeEditor : public dispatcher::ClientPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ScopeEditor( const dispatcher::Config& config, const dispatcher::Model& model, const ReportFactory& reportFactory, const std::string& vpath );
    virtual ~ScopeEditor();
    //@}

    //! @name Client Publisher
    //@{
    void VisitModel( dispatcher::ModelVisitor_ABC& visitor );
    virtual void Send( const ASN1T_MsgsSimToClient& asn );
    virtual void Send( const ASN1T_MsgsAuthenticationToClient& ) {};
    virtual void Send( const ASN1T_MsgsReplayToClient&         ) {};
    virtual void Send( const ASN1T_MsgsAarToClient& ) {};
    virtual void Send( const ASN1T_MsgsMessengerToClient&); 
    virtual void Send( const ASN1T_MsgsDispatcherToClient& ) {};

    virtual std::string GetEndpoint() const { return ""; }
    //@}
        
private:
    //! @name Copy/Assignement
    //@{
    ScopeEditor( const ScopeEditor& );            //!< Copy constructor
    ScopeEditor& operator=( const ScopeEditor& ); //!< Assignement operator
    //@}
    
    //! @name Update operators
    //@{    
    void Write( const ASN1T_MsgUnitCreation& msg );
    void Write( const ASN1T_MsgUnitAttributes& msg );
    void Write( const ASN1T_MsgUnitKnowledgeCreation& msg );
    void Write( const ASN1T_MsgUnitKnowledgeUpdate& msg );
    void Write( const ASN1T_MsgLimitCreation& msg );    
    void Write( const ASN1T_MsgLimaCreation& msg );    
    void Write( const ASN1T_MsgObjectCreation& msg );
    void Write( const ASN1T_MsgObjectUpdate& msg );
    void Write( const ASN1T_MsgReport& msg );
    void Write( const ASN1T_MsgFormationCreation& asn );
    void Write( const ASN1T_MsgAutomatCreation& asn );    
    void AppendCData();
    void Flush();
    //@}

private:
    //! @name 
    //@{
    typedef std::map< unsigned, std::string > T_CDATA;
    typedef T_CDATA::iterator                       IT_CDATA;
    typedef std::map< unsigned, boost::shared_ptr< xml::xobufferstream > >   T_StubStreams;
    typedef T_StubStreams::iterator            IT_StubStreams;
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model&     model_;
    const ReportFactory&         reportFactory_;
    std::auto_ptr<StyleEditor>   styles_;
    T_StubStreams                stubs_;
    T_CDATA                      cdata_;
    std::auto_ptr<xml::xostream> xos_;
    //@}
};

}

#endif // __gearth_ScopeEditor_h_
