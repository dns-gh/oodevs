// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ActiveProtections_GUI_h_
#define __ADN_ActiveProtections_GUI_h_

#include "ADN_GUI_ABC.h"
class ADN_ActiveProtections_Data;

// =============================================================================
/** @class  ADN_ActiveProtections_GUI
    @brief  ADN_ActiveProtections_GUI
*/
// Created: LDC 2010-01-13
// =============================================================================
class ADN_ActiveProtections_GUI : public ADN_GUI_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    ADN_ActiveProtections_GUI( const ADN_ActiveProtections_Data& ) : ADN_GUI_ABC( "ADN_ActiveProtections_GUI" ) {}
    virtual ~ADN_ActiveProtections_GUI() {}
    //@}

    //! @name Operations
    //@{
    virtual void Build() {}
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_ActiveProtections_GUI( const ADN_ActiveProtections_GUI& );            //!< Copy constructor
    ADN_ActiveProtections_GUI& operator=( const ADN_ActiveProtections_GUI& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __ADN_ActiveProtections_GUI_h_
