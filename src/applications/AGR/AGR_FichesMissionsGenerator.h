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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_FichesMissionsGenerator.h $
// $Author: Nld $
// $Modtime: 29/04/05 12:02 $
// $Revision: 2 $
// $Workfile: AGR_FichesMissionsGenerator.h $
//
// *****************************************************************************

#ifndef __AGR_FichesMissionsGenerator_h_
#define __AGR_FichesMissionsGenerator_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Generator_ABC.h"

// =============================================================================
/** @class  AGR_FichesMissionsGenerator
    @brief  AGR_FichesMissionsGenerator
*/
// Created: AGE 2004-09-21
// =============================================================================
class AGR_FichesMissionsGenerator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_FichesMissionsGenerator();
    virtual ~AGR_FichesMissionsGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_FichesMissionsGenerator( const AGR_FichesMissionsGenerator& );            //!< Copy constructor
    AGR_FichesMissionsGenerator& operator=( const AGR_FichesMissionsGenerator& ); //!< Assignement operator
    //@}

    //! @name
    //@{
    void GenerateFichesMissions( const AGR_Workspace& workspace, const std::string& strOutputPath );
    void GenerateIndex         ( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}
};

#endif // __AGR_FichesMissionsGenerator_h_
