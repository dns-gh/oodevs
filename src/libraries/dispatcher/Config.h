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

namespace dispatcher
{

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
          bool           RecorderEnabled               () const;

          std::string    GetRecorderDirectory          ( const std::string& records ) const;
          std::string    GenerateRecorderDirectory     () const;
    //@}

private:
    std::string    profiles_;
    std::string    networkSimulationParameters_;
    unsigned short networkClientsParameters_;
    bool           bRecorderEnabled_;
};

}

#endif // __Config_h_
