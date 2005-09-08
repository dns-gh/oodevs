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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MilRCGenerator.h $
// $Author: Nld $
// $Modtime: 29/04/05 12:02 $
// $Revision: 2 $
// $Workfile: AGR_MilRCGenerator.h $
//
// *****************************************************************************

#ifndef __AGR_MilRCGenerator_h_
#define __AGR_MilRCGenerator_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Generator_ABC.h"

// =============================================================================
/** @class  AGR_MilRCGenerator
    @brief  AGR_MilRCGenerator
*/
// Created: AGE 2004-09-21
// =============================================================================
class AGR_MilRCGenerator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_MilRCGenerator();
    virtual ~AGR_MilRCGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_MilRCGenerator( const AGR_MilRCGenerator& );            //!< Copy constructor
    AGR_MilRCGenerator& operator=( const AGR_MilRCGenerator& ); //!< Assignement operator
    //@}

    //! @name Tools
    //@{
    void GenerateMILRCClassHeader( const AGR_Workspace& workspace, const std::string& strOutputPath );
    void GenerateMILRCClassBody  ( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}
};

#endif // __AGR_MilRCGenerator_h_
