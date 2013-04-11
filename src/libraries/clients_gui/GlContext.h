// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlContext_h_
#define __GlContext_h_


namespace gui
{

// =============================================================================
/** @class  GlContext
    @brief  GlContext
*/
// Created: MMC 2013-04-10
// =============================================================================
class GlContext
{
public:

    //! @name Constructors/Destructor
    //@{
    GlContext();
    ~GlContext();
    //@}

    //! @name Operations
    //@{
    void Init( HWND hWnd );
    void Purge();
    //@}

private:

    //! @name Operations
    //@{
    void Reset();
    //@}

    //! @name Member data
    //@{
    HWND hWnd_;
    HDC hDC_;
    HGLRC hRC_;
    //@}
};

}

#endif // __GlContext_h_