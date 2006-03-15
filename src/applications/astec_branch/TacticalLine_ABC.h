// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-14 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/TacticalLine_ABC.h $
// $Author: Ape $
// $Modtime: 28/07/04 14:48 $
// $Revision: 5 $
// $Workfile: TacticalLine_ABC.h $
//
// *****************************************************************************

#ifndef __TacticalLine_ABC_h_
#define __TacticalLine_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Order_Def.h"
#include "Types.h"
#include "ASN_Types.h"


// =============================================================================
/** @class  TacticalLine_ABC
    @brief  Base class of the limits and limas.
*/
// Created: APE 2004-04-14
// =============================================================================
class TacticalLine_ABC
{
    MT_COPYNOTALLOWED( TacticalLine_ABC );

public:
    enum E_State
    {   
        eStateOk            = 0x00,
        eStateCreated       = 0x01,
        eStateModified      = 0x02,
        eStateDeleted       = 0x04
    };

    enum E_NetworkState
    {   
        eNetworkStateNotRegistered     = 0,
        eNetworkStateRegistering       = 1,
        eNetworkStateRegistered        = 2
    };

    enum E_LineType
    {
        eLima,
        eLimit
    };

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_ABC();
    virtual ~TacticalLine_ABC();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgLimitCreationAck& asnMsg );
    void Update( const ASN1T_MsgLimitUpdateAck& asnMsg );
    void Update( const ASN1T_MsgLimaCreationAck& asnMsg );
    void Update( const ASN1T_MsgLimaUpdateAck& asnMsg);
    //@}

    //! @name Accessors
    //@{
    unsigned long         GetId() const;
    MIL_LineID            GetID() const;
    CT_PointVector&       GetPointList() const;
    const std::string     GetName() const;
    void                  SetName( const std::string strName );
    E_NatureLevel         GetLevel() const;

    bool                  IsUpdatingToSim() const;
    bool                  IsCreatedByMos() const;
    
    //@}

    //! @name Point operations
    //@{
    void AddPoint( const MT_Vector2D& vPos );
    void InsertPoint( uint nIndex, const MT_Vector2D& vPos );
    void ModifyPoint( uint nIndex, const MT_Vector2D& vNewPos );
    void Translate( const MT_Vector2D& vTranslation );
    void DeletePoint( uint nIndex );
    void PopPoint();
    void DeleteAllPoints();
    //@}

    virtual bool UpdateToSim() = 0;
    virtual TacticalLine_ABC::E_LineType GetLineType() const = 0;

    //! @name In/Out
    //@{
    virtual void Read( MT_InputArchive_ABC& archive );
    virtual void Write( MT_OutputArchive_ABC& archive ) const;
    //@}

protected:
    //! @name Member data
    //@{
    MIL_LineID     nID_;
    E_NatureLevel  nLevel_;

    T_PointVector  pointList_;
    std::string    strName_;

    E_State        nState_;
    E_NetworkState nNetworkState_;

    bool           bCreatedBy; 
    //@}
};

#   include "TacticalLine_ABC.inl"

#endif // __TacticalLine_ABC_h_
