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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_StuffGenerator.h $
// $Author: Age $
// $Modtime: 21/09/04 14:54 $
// $Revision: 1 $
// $Workfile: AGR_StuffGenerator.h $
//
// *****************************************************************************

#ifndef __AGR_StuffGenerator_h_
#define __AGR_StuffGenerator_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Generator_ABC.h"

// =============================================================================
/** @class  AGR_StuffGenerator
    @brief  AGR_StuffGenerator
*/
// Created: AGE 2004-09-21
// =============================================================================
class AGR_StuffGenerator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_StuffGenerator();
    virtual ~AGR_StuffGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_StuffGenerator( const AGR_StuffGenerator& );            //!< Copy constructor
    AGR_StuffGenerator& operator=( const AGR_StuffGenerator& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void GenerateMissionToolFile        ( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateMilMissionConverterFile( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    //@}
};

#endif // __AGR_StuffGenerator_h_
