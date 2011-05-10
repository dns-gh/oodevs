// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PluginConfig_h_
#define __PluginConfig_h_

#include "PluginConfig_ABC.h"
#include <boost/shared_ptr.hpp>
#include <vector>

class QGroupBox;

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
    class PluginSetting;

// =============================================================================
/** @class  PluginConfig
    @brief  PluginConfig
*/
// Created: SBO 2011-05-09
// =============================================================================
class PluginConfig : public PluginConfig_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PluginConfig( QWidget* parent, const tools::GeneralConfig& config, xml::xistream& xis );
    virtual ~PluginConfig();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual bool IsAvailable() const;
    virtual void Commit( const std::string& exercise, const std::string& session );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadSetting( xml::xistream& xis, QWidget* parent );
    void ReadGroup( xml::xistream& xis, QWidget* parent );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const std::string name_;
    const std::string library_;
    const std::string version_;
    QString label_;
    QGroupBox* box_;
    std::vector< boost::shared_ptr< PluginSetting > > settings_;
    //@}
};

}

#endif // __PluginConfig_h_
