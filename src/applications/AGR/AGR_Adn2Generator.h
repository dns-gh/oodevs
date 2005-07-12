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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Adn2Generator.h $
// $Author: Ape $
// $Modtime: 17/02/05 17:13 $
// $Revision: 1 $
// $Workfile: AGR_Adn2Generator.h $
//
// *****************************************************************************

#ifndef __AGR_Adn2Generator_h_
#define __AGR_Adn2Generator_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Generator_ABC.h"

class AGR_Mission;

// =============================================================================
/** @class  AGR_Adn2Generator
    @brief  AGR_Adn2Generator
*/
// Created: AGE 2004-09-21
// =============================================================================
class AGR_Adn2Generator : public AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    AGR_Adn2Generator();
    virtual ~AGR_Adn2Generator();
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_Adn2Generator( const AGR_Adn2Generator& );            //!< Copy constructor
    AGR_Adn2Generator& operator=( const AGR_Adn2Generator& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    bool IsOfType( const AGR_Mission& mission, const std::string& strType ) const;
    void GenerateAdn2EnumFile        ( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateAdn2MissionConverter( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    //@}
};

#endif // __AGR_Adn2Generator_h_
