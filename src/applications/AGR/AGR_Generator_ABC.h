// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AGR_Generator_ABC_h_
#define __AGR_Generator_ABC_h_

class AGR_Workspace;

// =============================================================================
/** @class  AGR_Generator_ABC
    @brief  AGR_Generator_ABC
*/
// Created: AGE 2004-09-21
// =============================================================================
class AGR_Generator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_Generator_ABC() {}
    virtual ~AGR_Generator_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath ) = 0;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_Generator_ABC( const AGR_Generator_ABC& );            //!< Copy constructor
    AGR_Generator_ABC& operator=( const AGR_Generator_ABC& ); //!< Assignement operator
    //@}
};

#endif // __AGR_Generator_ABC_h_
