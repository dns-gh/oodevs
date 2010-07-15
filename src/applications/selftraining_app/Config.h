// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __Config_h_
#define __Config_h_

#include "frontend/Config.h"

// =============================================================================
/** @class  Config
    @brief  Config
*/
// Created: LDC 2008-10-27
// =============================================================================
class Config : public frontend::Config
{
public:
    //! @name Profiles
    //@{
    enum EProfile
    {
        eTerrain,
        eUser,
        eAdvancedUser,
        eAdministrator
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Config();
    virtual ~Config();
    //@}

    //! @name Operations
    //@{
    unsigned short GetListServerPort() const;
    unsigned short GetListClientPort() const;
    void SetProfile( EProfile profile ) const;
    EProfile GetProfile() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Config( const Config& );            //!< Copy constructor
    Config& operator=( const Config& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned short listServerPort_;
    unsigned short listClientPort_;
    mutable EProfile profile_;
    //@}
};

#endif // __Config_h_
