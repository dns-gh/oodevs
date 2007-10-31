// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_OrderContext_h_
#define __MIL_OrderContext_h_

#include "MIL.h"

#include "MIL_Fuseau.h"
#include "MIL_LimaOrder.h"

class MIL_LimaOrder;
class MIL_LimaFunction;
class MIL_IntelligenceOrder;

// =============================================================================
/** @class  MIL_OrderContext
    @brief  MIL_OrderContext
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_OrderContext
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_OrderContext();
             MIL_OrderContext( const ASN1T_OrderContext& asn, const MT_Vector2D& vOrientationRefPos );
             MIL_OrderContext( const MIL_OrderContext& rhs );
    virtual ~MIL_OrderContext();
    //@}

    //! @name Accessors
    //@{
    const MT_Vector2D&   GetDirDanger         () const;
    const MIL_Fuseau&    GetFuseau            () const;
    const T_LimaVector&  GetLimas             () const;
          MIL_LimaOrder* FindLima             ( uint nID );
          MIL_LimaOrder* FindLima             ( const MIL_LimaFunction& function );
          MIL_LimaOrder* FindNextScheduledLima();
          void           AffectFuseau         ( const MIL_Fuseau& fuseau );
    //@}

    //! @name Network
    //@{
    void Serialize              ( ASN1T_OrderContext& asn ) const;
    void CleanAfterSerialization( ASN1T_OrderContext& asn ) const;
    //@}
 
private:
    //! @name Copy/Assignement
    //@{
    MIL_OrderContext& operator=( const MIL_OrderContext& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    typedef std::vector< MIL_IntelligenceOrder* >   T_IntelligenceOrders;
    typedef T_IntelligenceOrders::const_iterator  CIT_IntelligenceOrders;
    //@}

private:
    T_LimaVector         limas_;
    T_IntelligenceOrders intelligences_;
    MIL_Fuseau           fuseau_;
    MT_Vector2D          dirDanger_;
};

#include "MIL_OrderContext.inl"

#endif // __MIL_OrderContext_h_
