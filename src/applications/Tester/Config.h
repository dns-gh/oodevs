// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-24 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Config_h_
#define __Config_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"

namespace TEST {

// =============================================================================
/** @class  Config
    @brief  Config
    @par    Using example
    @code
    Config;
    @endcode
*/
// Created: SBO 2005-08-24
// =============================================================================
class Config
{

public:
    //! @name Constructors/Destructor
    //@{
             Config( const std::string& strFile );
    virtual ~Config();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetServer           () const;
    const std::string& GetConfigFile       () const;
    const std::string& GetRandomSeedFile   () const;
    uint               GetPort             () const;
    uint               GetTimeFactor       () const;
    bool               MustTestAutomat     () const;
    uint               GetPeriod           () const;
    uint               GetIterationNumber  () const;
    uint               GetIterationInterval() const;
    const std::string& GetLogFile          () const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Config( const Config& );            //!< Copy constructor
    Config& operator=( const Config& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    void LoadConfigFile( const std::string& strFile );
    //@}

private:
    //! @name Member data
    //@{
    std::string strServer_;
    std::string strConfigFile_;
    std::string strRandomSeedFile_;
    uint        nPort_;
    uint        nTimeFactor_;
    bool        bTestAutomat_;
    uint        nPeriod_;
    uint        nItNumber_;
    uint        nItInterval_;
    std::string strLogFile_;
    //@}
};

} // end namespace TEST

#include "Config.inl"

#endif // __Config_h_
