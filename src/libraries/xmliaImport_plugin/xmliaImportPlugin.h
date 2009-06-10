// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BmlPlugin_h_
#define __BmlPlugin_h_

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
namespace xmliaImport
{
    //class ExtensionFactory;
    class PublisherActor;
    //class Serializer;
    class UpdateListener;
    //class ReportFactory;
    class Simulation;

// =============================================================================
/** @class  XmliaImportPlugin
    @brief  XmliaImportPlugin
*/
// Created: SBO 2008-02-29
// =============================================================================
class XmliaImportPlugin : public dispatcher::Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             XmliaImportPlugin( dispatcher::Model& model, xml::xistream& xis, dispatcher::SimulationPublisher_ABC& simulation );
    virtual ~XmliaImportPlugin();
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
    XmliaImportPlugin( const XmliaImportPlugin& );            //!< Copy constructor
    XmliaImportPlugin& operator=( const XmliaImportPlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model& model_;
    std::auto_ptr< PublisherActor > publisher_;
    std::auto_ptr< Simulation > simulation_;
    //std::auto_ptr< ReportFactory > reportFactory_;
    //std::auto_ptr< ExtensionFactory > extensionFactory_;
    std::auto_ptr< UpdateListener > listener_;
    //@}
};

}
}

#endif // __XmliaImportPlugin_h_
