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

#include "tools/SessionConfig.h"
class Network;

// =============================================================================
/** @class  Config
    @brief  Config
*/
// Created: NLD 2007-01-12
// =============================================================================
class Config : public tools::SessionConfig
{

public:
    //! @name Constructors/Destructor
    //@{
             Config( int argc, char** argv );
    virtual ~Config();
    //@}

    //! @name Operations
    //@{
    void Connect( Network& network ) const;
    std::string GetLogin() const;
    bool        NeedLogin() const;
    //@}

private:
    //! @name Helpers
    //@{
    void LoadSession( Network& network ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string host_;
    std::string login_;
    bool        needLogin_;
    //@}
};

#endif // __Config_h_
