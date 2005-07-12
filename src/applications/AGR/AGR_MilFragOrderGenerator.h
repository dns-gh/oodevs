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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MilFragOrderGenerator.h $
// $Author: Age $
// $Modtime: 21/09/04 10:45 $
// $Revision: 1 $
// $Workfile: AGR_MilFragOrderGenerator.h $
//
// *****************************************************************************

#ifndef __AGR_MilFragOrderGenerator_h_
#define __AGR_MilFragOrderGenerator_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Generator_ABC.h"

// =============================================================================
/** @class  AGR_MilFragOrderGenerator
    @brief  AGR_MilFragOrderGenerator
*/
// Created: AGE 2004-09-21
// =============================================================================
class AGR_MilFragOrderGenerator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_MilFragOrderGenerator();
    virtual ~AGR_MilFragOrderGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_MilFragOrderGenerator( const AGR_MilFragOrderGenerator& );            //!< Copy constructor
    AGR_MilFragOrderGenerator& operator=( const AGR_MilFragOrderGenerator& ); //!< Assignement operator
    //@}
};

#endif // __AGR_MilFragOrderGenerator_h_
