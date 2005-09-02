// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-02 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __MOS_DynaObject_ABC_h_
#define __MOS_DynaObject_ABC_h_

#include "MOS_ASN_Types.h"
#include "MOS_IDManager.h"

class MOS_Team;

// =============================================================================
/** @class  MOS_DynaObject_ABC
    @brief  MOS_DynaObject_ABC
    @par    Using example
    @code
    MOS_DynaObject_ABC;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class MOS_DynaObject_ABC
{
    MT_COPYNOTALLOWED( MOS_DynaObject_ABC );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
             MOS_DynaObject_ABC();
             MOS_DynaObject_ABC( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~MOS_DynaObject_ABC();
    //@}

    //! @name Static Operations
    //@{
    static void           InitializeObjectIds      ( MT_XXmlInputArchive& classeIds );
    static MOS_IDManager& GetIDManagerForObjectType( ASN1T_EnumObjectType nType );
    //@}

    //! @name Accessors
    //@{
    uint                       GetID                          () const;
    ASN1T_EnumObjectType       GetType                        () const;
    const MOS_Team&            GetTeam                        () const;
    bool                       IsPrepared                     () const;
    MT_Float                   GetConstructionPercentage      () const;
    MT_Float                   GetValorizationPercentage      () const;
    MT_Float                   GetBypassConstructionPercentage() const;
    const MT_Vector2D&         GetCenter                      () const;
    const T_PointVector&       GetPointList                   () const;
    ASN1T_EnumTypeLocalisation GetLocationType                () const;
    //@}

    //! @name Modifiers
    //@{
    /*
    void SetType        ( ASN1T_EnumObjectType nType );
    */
    void SetID          ( uint nID );
    void SetTeam        ( MOS_Team& team );
    void SetLocalisation( ASN1T_EnumTypeLocalisation nType, const T_PointVector& pointVector );
    //@}

    //! @name Operations
    //@{
    virtual void Update  ( const ASN1T_MsgObjectUpdate& asnMsg  );
    virtual void ReadODB (       MT_XXmlInputArchive&   archive );
    virtual void WriteODB(       MT_XXmlOutputArchive&  archive ) const;
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_EnumObjectType       nType_;
    uint                       nID_;
    MOS_Team*                  pTeam_;

    ASN1T_EnumTypeLocalisation nTypeLocalisation_;

public:
    T_PointVector              pointVector_;
    MT_Vector2D                center_;

private:
    MT_Float                   rConstructionPercentage_;
    MT_Float                   rValorizationPercentage_;
    MT_Float                   rBypassConstructionPercentage_;

    bool                       bPrepared_;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< ASN1T_EnumObjectType, unsigned int >  T_ObjectIDs;
    typedef std::map< unsigned int, MOS_IDManager* >        T_Managers;
    //@}

private:
    //! @name Static members
    //@{
    static T_ObjectIDs objectIds_;
    static T_Managers  managers_;
    //@}
};

#ifdef MOS_USE_INLINE
#   include "MOS_DynaObject_ABC.inl"
#endif

#endif // __MOS_DynaObject_ABC_h_
