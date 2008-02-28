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
    class SimulationPublisher_ABC;
    class ClientsNetworker;
    class CompositePlugin;
    class PluginFactory_ABC;

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
             PluginFactory( const Config& config, Model& model, SimulationPublisher_ABC& simulation, ClientsNetworker& clients, CompositePlugin& handler );
    virtual ~PluginFactory();
    //@}

    //! @name Operations
    //@{
    void Register( PluginFactory_ABC& factory );
    void Instanciate();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PluginFactory( const PluginFactory& );            //!< Copy constructor
    PluginFactory& operator=( const PluginFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadPlugin( const std::string& name, xml::xistream& xis ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< PluginFactory_ABC* > T_Factories;
    //@}

private:
    //! @name Member data
    //@{
    CompositePlugin& handler_;
    const Config& config_;
    Model& model_;
    SimulationPublisher_ABC& simulation_;
    ClientsNetworker& clients_;
    T_Factories factories_;
    //@}
};

}

#endif // __PluginFactory_h_
