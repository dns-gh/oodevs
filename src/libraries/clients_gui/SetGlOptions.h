// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SetGlOptions_h_
#define __SetGlOptions_h_

// =============================================================================
/** @class  SetGlOptions
    @brief  SetGlOptions
*/
// Created: AGE 2006-03-16
// =============================================================================
class SetGlOptions
{

public:
    //! @name Constructors/Destructor
    //@{
             SetGlOptions();
    virtual ~SetGlOptions();
    //@}

protected:
    //! @name Member data
    //@{
    QGLContext* context_;
    //@}
};

#endif // __SetGlOptions_h_
