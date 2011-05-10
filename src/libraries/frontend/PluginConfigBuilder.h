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
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    std::auto_ptr< CompositePluginConfig > composite_;
    //@}
};

}

#endif // __PluginConfigBuilder_h_
