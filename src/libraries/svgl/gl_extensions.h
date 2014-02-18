// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gl_extensions_h_
#define __gl_extensions_h_

// =============================================================================
/** @class  gl_extensions
    @brief  gl extensions
*/
// Created: AGE 2006-09-08
// =============================================================================
namespace gl
{

    void lockArrays( GLint first, GLsizei size );
    void unlockArrays();

}

#endif // __gl_extensions_h_
