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

#ifndef __AGR_TicRCGenerator_h_
#define __AGR_TicRCGenerator_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Generator_ABC.h"

// =============================================================================
/** @class  AGR_TicRCGenerator
    @brief  AGR_TicRCGenerator
    @par    Using example
    @code
    AGR_TicRCGenerator;
    @endcode
*/
// Created: SBO 2005-08-10
// =============================================================================
class AGR_TicRCGenerator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_TicRCGenerator();
    virtual ~AGR_TicRCGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AGR_TicRCGenerator( const AGR_TicRCGenerator& );            //!< Copy constructor
    AGR_TicRCGenerator& operator=( const AGR_TicRCGenerator& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void GenerateMessageRCCPPFile   ( const AGR_Workspace& workspace, const std::string& strOutputPath );
    void GenerateMessageRCHeaderFile( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}
};

#endif // __AGR_TicRCGenerator_h_
