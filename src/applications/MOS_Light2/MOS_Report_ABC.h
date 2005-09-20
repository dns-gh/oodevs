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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Report_ABC.h $
// $Author: Ape $
// $Modtime: 4/08/04 15:18 $
// $Revision: 2 $
// $Workfile: MOS_Report_ABC.h $
//
// *****************************************************************************

#ifndef __MOS_Report_ABC_h_
#define __MOS_Report_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  MOS_Report_ABC
    @brief  MOS_Report_ABC
    @par    Using example
    @code
    MOS_Report_ABC;
    @endcode
*/
// Created: APE 2004-08-04
// =============================================================================
class MOS_Report_ABC
{
    MT_COPYNOTALLOWED( MOS_Report_ABC );

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
             MOS_Report_ABC( const MT_Vector2D& vPos );
    virtual ~MOS_Report_ABC();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const MT_Vector2D& GetPos  () const;
    const std::string& GetTitle() const;
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


#   include "MOS_Report_ABC.inl"

#endif // __MOS_Report_ABC_h_
