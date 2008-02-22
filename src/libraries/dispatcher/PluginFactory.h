    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PluginFactory_h_
#define __PluginFactory_h_

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Config;
    class Model;
    class CompositePlugin;
    class SimulationNetworker;
    class ClientsNetworker;

// =============================================================================
/** @class  PluginFactory
    @brief  PluginFactory
*/
// Created: SBO 2007-07-24
// =============================================================================
class PluginFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             PluginFactory( const Config& config, Model& model, SimulationNetworker& simulation, ClientsNetworker& clients );
    virtual ~PluginFactory();
    //@}

    //! @name Operations
    //@{
    void RegisterPlugins( CompositePlugin& handler ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PluginFactory( const PluginFactory& );            //!< Copy constructor
    PluginFactory& operator=( const PluginFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadPlugin( const std::string& name, xml::xistream& xis, CompositePlugin& handler ) const;
    //@}

private:
    //! @name Member data
    //@{
    const Config& config_;
    Model& model_;
    SimulationNetworker& simulation_;
    ClientsNetworker& clients_;
    //@}
};

}

#endif // __PluginFactory_h_
