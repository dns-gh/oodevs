// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __XmliaPlugin_h_
#define __XmliaPlugin_h_

#include "dispatcher/Plugin_ABC.h"

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Model;
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace xmlia
{
    class ExtensionFactory;
    class Publisher;
    class ReportManager;
    class Simulation;
    class LdapClient;

// =============================================================================
/** @class  XmliaPlugin
    @brief  XmliaPlugin
*/
// Created: SLG 2009-06-12
// =============================================================================
class XmliaPlugin : public dispatcher::Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             XmliaPlugin( dispatcher::Model& model, xml::xistream& xis, dispatcher::SimulationPublisher_ABC& simulationPublisher );
    virtual ~XmliaPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive                  ( const ASN1T_MsgsSimToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    XmliaPlugin( const XmliaPlugin& );            //!< Copy constructor
    XmliaPlugin& operator=( const XmliaPlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model& model_;
	dispatcher::SimulationPublisher_ABC& simulationPublisher_;
    std::auto_ptr< Publisher > publisher_;
    std::auto_ptr< Simulation > simulation_;
    std::auto_ptr< ReportManager > reportManager_;
    std::auto_ptr< ExtensionFactory > extensionFactory_;
    std::auto_ptr< LdapClient > ldap_;
    bool  bExportActivation_;
    bool  bImportActivation_;
    unsigned int nTick_;
    unsigned int nCptTick_;
    //@}
};

}
}

#endif // __XmliaPlugin_h_
