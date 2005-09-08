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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MosGenerator.h $
// $Author: Age $
// $Modtime: 21/09/04 11:36 $
// $Revision: 1 $
// $Workfile: AGR_MosGenerator.h $
//
// *****************************************************************************

#ifndef __AGR_MosGenerator_h_
#define __AGR_MosGenerator_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Generator_ABC.h"

// =============================================================================
/** @class  AGR_MosGenerator
    @brief  AGR_MosGenerator
*/
// Created: AGE 2004-09-21
// =============================================================================
class AGR_MosGenerator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_MosGenerator();
    virtual ~AGR_MosGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_MosGenerator( const AGR_MosGenerator& );            //!< Copy constructor
    AGR_MosGenerator& operator=( const AGR_MosGenerator& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void GenerateMosMissionHeaderFiles( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateMosMissionCppFiles   ( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateMosMissionConverter  ( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateMosRcs               ( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    //@}
};

#endif // __AGR_MosGenerator_h_
