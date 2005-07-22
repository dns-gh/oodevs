// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-19 $
// $Archive: /MVW_v10/Build/SDK/TIC_XmlExporter/src/Config.h $
// $Author: Sbo $
// $Modtime: 5/07/05 11:44 $
// $Revision: 3 $
// $Workfile: Config.h $
//
// *****************************************************************************

#ifndef __Config_h_
#define __Config_h_

#include "Types.h"

namespace TICExport
{
        
    class Workspace;

// =============================================================================
/** @class  Config
    @brief  Config
    @par    Using example
    @code
    Config;
    @endcode
*/
// Created: SBO 2005-05-19
// =============================================================================
class Config
{
public:
	//! @name Constructors/Destructor
    //@{
             Config( const std::string& strConfigFile );
    virtual ~Config();
    //@}


    //! @name Operations
    //@{
    void    LoadDisaggregationParameters( Workspace& workspace );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetServer                () const;
          uint         GetPort                  () const;
    const std::string& GetSimConfigFile         () const;
    const std::string& GetExportFilename        () const;
    const std::string& GetPlatformNatureFilename() const;
          bool         MustExportCommunications () const;
          bool         IsIncrementalExport      () const;
    //@}

private:
    //! @name Operations
    //@{
    void    LoadParameters();
	//@}

private:
	//! @name Member data
    //@{
    std::string  strConfigFile_;

    std::string  strServer_;
    uint         nPort_;
    std::string  strSimConfigFile_;
    
    std::string  strExportFilename_;
    bool         bExportCommunications_;
    bool         bExportIncremental_;
    std::string  strFilterFilename_;
    std::string  strPlatformNatureFilename_;
	//@}
};

} // end namespace TICExport

#include "Config.inl"

#endif // __Config_h_
