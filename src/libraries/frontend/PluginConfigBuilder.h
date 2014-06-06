// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PluginConfigBuilder_h_
#define __PluginConfigBuilder_h_

#include "CompositePluginConfig.h"
#include <boost/noncopyable.hpp>
#include <memory>

class QTabWidget;

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;
    class RealFileLoaderObserver_ABC;
}

namespace frontend
{

// =============================================================================
/** @class  PluginConfigBuilder
    @brief  PluginConfigBuilder
*/
// Created: SBO 2011-05-09
// =============================================================================
class PluginConfigBuilder : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             PluginConfigBuilder( const tools::GeneralConfig& config, QTabWidget* parent );
    virtual ~PluginConfigBuilder();
    //@}

    //! @name Operations
    //@{
    PluginConfigBuilder& BuildFromXml();
    template< typename T >
    PluginConfigBuilder& Build()
    {
        if( composite_.get() )
            composite_->Add< T >( config_ );
        return *this;
    }
    PluginConfig_ABC* Finalize();
    //@}

private:
    bool AddPlugin( const tools::Path& path );

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    std::unique_ptr< CompositePluginConfig > composite_;
    std::unique_ptr< tools::RealFileLoaderObserver_ABC > observer_;
    std::unique_ptr< tools::Loader_ABC > loader_;
    //@}
};

}

#endif // __PluginConfigBuilder_h_
