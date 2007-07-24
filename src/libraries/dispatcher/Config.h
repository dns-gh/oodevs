// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Config_h_
#define __Config_h_

#include "tools/GeneralConfig.h"
#include <map>

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class PluginConfig;

// =============================================================================
/** @class  Config
    @brief  Config
*/
// Created: NLD 2007-01-10
// =============================================================================
class Config : public virtual tools::GeneralConfig
{
public:
    //! @name Constructors/Destructor
    //@{
             Config();
    virtual ~Config();
    //@}

    //! @name Operations
    //@{
    virtual void Parse( int argc, char** argv );
    //@}

    //! @name Accessors
    //@{
          std::string    GetProfilesFile               () const;
    const std::string&   GetNetworkSimulationParameters() const;
          unsigned short GetNetworkClientsParameters   () const;
    const std::string&   GetPhysicalFile() const;
          std::string    BuildPhysicalChildFile( const std::string& file ) const;
    const PluginConfig&  GetPluginConfig( const std::string& name ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const PluginConfig* > T_Plugins;
    //@}

    //! @name Helpers
    //@{
    void ReadPlugin( xml::xistream& xis );
    //@}

private:
    std::string    profiles_;
    std::string    networkSimulationParameters_;
    unsigned short networkClientsParameters_;
    std::string    physical_;
    T_Plugins      plugins_;
};

}

#endif // __Config_h_
