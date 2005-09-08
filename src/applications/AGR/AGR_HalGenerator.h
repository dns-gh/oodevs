// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-21 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_HalGenerator.h $
// $Author: Age $
// $Modtime: 23/09/04 16:46 $
// $Revision: 2 $
// $Workfile: AGR_HalGenerator.h $
//
// *****************************************************************************

#ifndef __AGR_HalGenerator_h_
#define __AGR_HalGenerator_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Generator_ABC.h"

// =============================================================================
/** @class  AGR_HalGenerator
    @brief  AGR_HalGenerator
*/
// Created: AGE 2004-09-21
// =============================================================================
class AGR_HalGenerator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_HalGenerator();
    virtual ~AGR_HalGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_HalGenerator( const AGR_HalGenerator& );            //!< Copy constructor
    AGR_HalGenerator& operator=( const AGR_HalGenerator& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void GenerateMissionEnumFile        ( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateMissionScriptFile      ( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateOrderConduiteScriptFile( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateRCEnumFile             ( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    //@}
};

#endif // __AGR_HalGenerator_h_
