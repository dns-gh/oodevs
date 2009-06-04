// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __XmliaExportPluginFactory_h_
#define __XmliaExportPluginFactory_h_

#include "dispatcher/PluginFactory_ABC.h"

namespace plugins
{
namespace xmliaExport
{

// =============================================================================
/** @class  XmliaExportPluginFactory
    @brief  XmliaExportPluginFactory
*/
// Created: SBO 2008-02-29
// =============================================================================
class XmliaExportPluginFactory : public dispatcher::PluginFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             XmliaExportPluginFactory();
    virtual ~XmliaExportPluginFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< dispatcher::Plugin_ABC > Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& client, dispatcher::LinkResolver_ABC& resolver, dispatcher::CompositeRegistrable& registrables ) const ;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    XmliaExportPluginFactory( const XmliaExportPluginFactory& );            //!< Copy constructor
    XmliaExportPluginFactory& operator=( const XmliaExportPluginFactory& ); //!< Assignment operator
    //@}
};

}
}

#endif // __XmliaExportPluginFactory_h_
