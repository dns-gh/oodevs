// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-10 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __AGR_TesterProjectGenerator_h_
#define __AGR_TesterProjectGenerator_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Generator_ABC.h"

// =============================================================================
/** @class  AGR_TesterProjectGenerator
    @brief  AGR_TesterProjectGenerator
    @par    Using example
    @code
    AGR_TesterProjectGenerator;
    @endcode
*/
// Created: SBO 2005-08-10
// =============================================================================
class AGR_TesterProjectGenerator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_TesterProjectGenerator();
    virtual ~AGR_TesterProjectGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AGR_TesterProjectGenerator( const AGR_TesterProjectGenerator& );            //!< Copy constructor
    AGR_TesterProjectGenerator& operator=( const AGR_TesterProjectGenerator& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void GenerateMissionsCPPFile   ( const AGR_Workspace& workspace, const std::string& strOutputPath );
    void GenerateMissionsHeaderFile( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}
};

#endif // __AGR_TesterProjectGenerator_h_
