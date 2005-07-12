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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MilMissionGenerator.h $
// $Author: Age $
// $Modtime: 21/09/04 11:09 $
// $Revision: 1 $
// $Workfile: AGR_MilMissionGenerator.h $
//
// *****************************************************************************

#ifndef __AGR_MilMissionGenerator_h_
#define __AGR_MilMissionGenerator_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Generator_ABC.h"

// =============================================================================
/** @class  AGR_MilMissionGenerator
    @brief  AGR_MilMissionGenerator
*/
// Created: AGE 2004-09-21
// =============================================================================
class AGR_MilMissionGenerator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_MilMissionGenerator();
    virtual ~AGR_MilMissionGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_MilMissionGenerator( const AGR_MilMissionGenerator& );            //!< Copy constructor
    AGR_MilMissionGenerator& operator=( const AGR_MilMissionGenerator& ); //!< Assignement operator
    //@}
};

#endif // __AGR_MilMissionGenerator_h_
