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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MilProjectGenerator.h $
// $Author: Age $
// $Modtime: 21/09/04 16:59 $
// $Revision: 2 $
// $Workfile: AGR_MilProjectGenerator.h $
//
// *****************************************************************************

#ifndef __AGR_MilProjectGenerator_h_
#define __AGR_MilProjectGenerator_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Generator_ABC.h"

// =============================================================================
/** @class  AGR_MilProjectGenerator
    @brief  AGR_MilProjectGenerator
*/
// Created: AGE 2004-09-21
// =============================================================================
class AGR_MilProjectGenerator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_MilProjectGenerator();
    virtual ~AGR_MilProjectGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_MilProjectGenerator( const AGR_MilProjectGenerator& );            //!< Copy constructor
    AGR_MilProjectGenerator& operator=( const AGR_MilProjectGenerator& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void GenerateMissionsProjectFile( const AGR_Workspace& workspace, const std::string& strOutputPath );
    void GenerateMissionsCPPFile    ( const AGR_Workspace& workspace, const std::string& strOutputPath );
    void GenerateMissionsHeaderFile ( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}
};

#endif // __AGR_MilProjectGenerator_h_
