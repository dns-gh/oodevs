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
#include "clients_kernel/XmlDescription.h"
#include <boost/shared_ptr.hpp>
#include <vector>

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
    @brief  Plugin config
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
    virtual void OnLanguageChanged();
    virtual QString GetName() const;
    virtual bool IsAvailable() const;
    virtual void Commit( const tools::Path& exercise, const tools::Path& session );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadSetting( xml::xistream& xis, QWidget* parent );
    void ReadGroup( xml::xistream& xis, QWidget* parent );
    //@}

    //! @name Type
    //@{
    typedef std::map< QGroupBox*, kernel::XmlDescription* > T_GroupBoxDescription;
    typedef T_GroupBoxDescription::iterator                IT_GroupBoxDescription;

    typedef std::vector< boost::shared_ptr< PluginSetting > > T_Settings;
    typedef T_Settings::iterator                             IT_Settings;
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const tools::Path           name_;
    const tools::Path           library_;
    const std::string           version_;
    kernel::XmlDescription      description_;
    QString                     label_;
    QGroupBox*                  box_;
    QScrollArea*                view_;
    T_Settings                  settings_;
    T_GroupBoxDescription       groupBoxs_;
    //@}
};

}

#endif // __PluginConfig_h_
