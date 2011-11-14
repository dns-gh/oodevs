// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __CompositePluginConfig_h_
#define __CompositePluginConfig_h_

#include "PluginConfig_ABC.h"
#include <vector>

class QTabWidget;

namespace xml
{
    class xistream;
}

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{

// =============================================================================
/** @class  CompositePluginConfig
    @brief  CompositePluginConfig
*/
// Created: SBO 2011-05-09
// =============================================================================
class CompositePluginConfig : public PluginConfig_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit CompositePluginConfig( QTabWidget* parent );
    virtual ~CompositePluginConfig();
    //@}

    //! @name Operations
    //@{
    virtual void OnLanguageChanged();
    virtual QString GetName() const;
    virtual bool IsAvailable() const;
    virtual void Commit( const std::string& exercise, const std::string& session );
    void Add( const tools::GeneralConfig& config, xml::xistream& xis );
    template< typename T >
    void Add( const tools::GeneralConfig& config )
    {
        PluginConfig_ABC* plugin = new T( tabs_, config );
        plugins_.push_back( plugin );
        tabs_->addTab( plugin, plugin->GetName() );
    }
    //@}

private:
    //! @name Member data
    //@{
    QTabWidget*                      parent_;
    int                              parentTabIndex_;
    QTabWidget*                      tabs_;
    std::vector< PluginConfig_ABC* > plugins_;
    //@}
};

}

#endif // __CompositePluginConfig_h_
