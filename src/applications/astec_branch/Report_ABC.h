// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-04 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Report_ABC.h $
// $Author: Ape $
// $Modtime: 4/08/04 15:18 $
// $Revision: 2 $
// $Workfile: Report_ABC.h $
//
// *****************************************************************************

#ifndef __Report_ABC_h_
#define __Report_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  Report_ABC
    @brief  Report_ABC
    @par    Using example
    @code
    Report_ABC;
    @endcode
*/
// Created: APE 2004-08-04
// =============================================================================
class Report_ABC
{
    MT_COPYNOTALLOWED( Report_ABC );

public:
    enum E_Type
    {
        eRC      = 1000,
        eTrace   = 1001,
        eEvent   = 1002,
        eMessage = 1003,
        eWarning = 1004
    };

public:
    //! @name Constructors/Destructor
    //@{
             Report_ABC( const MT_Vector2D& vPos );
    virtual ~Report_ABC();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const MT_Vector2D& GetPos  () const;
    const std::string& GetTitle() const;
    const std::string  GetStrippedTitle() const;
    bool               IsInteresting() const;
    int                GetTime () const;
    E_Type             GetType () const;
    bool               IsRCType() const;

    bool IsNew() const;
    void SetNew( bool bNew );
    //@}

protected:
    std::string strTitle_; 
    MT_Vector2D vPos_;
    int         nTime_;
    bool        bNew_;
    E_Type      eType_;
};


#   include "Report_ABC.inl"

#endif // __Report_ABC_h_
