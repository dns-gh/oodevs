    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PluginFactory_h_
#define __PluginFactory_h_

namespace dispatcher
{
    class Config;
    class Model;
    class CompositeMessageHandler;
    class SimulationNetworker;

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
             PluginFactory( const Config& config, Model& model, SimulationNetworker& simulation );
    virtual ~PluginFactory();
    //@}

    //! @name Operations
    //@{
    void RegisterPlugins( CompositeMessageHandler& handler ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PluginFactory( const PluginFactory& );            //!< Copy constructor
    PluginFactory& operator=( const PluginFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Config& config_;
    Model& model_;
    SimulationNetworker& simulation_;
    //@}
};

}

#endif // __PluginFactory_h_
