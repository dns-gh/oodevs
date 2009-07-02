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

#include "MIL_Fuseau.h"
#include "MIL_LimaOrder.h"

class MIL_LimaFunction;
class MIL_IntelligenceOrder;
class MIL_IntelligenceOrdersVisitor_ABC;

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
    explicit MIL_OrderContext( bool present = false );
             MIL_OrderContext( const ASN1T_MissionParameters& asn, const MT_Vector2D& orientationReference );
             MIL_OrderContext( const MIL_OrderContext& rhs );
    virtual ~MIL_OrderContext();
    //@}

    //! @name Accessors
    //@{
          unsigned int   Length               () const;
    const MT_Vector2D&   GetDirDanger         () const;
    const MIL_Fuseau&    GetFuseau            () const;
    const T_LimaVector&  GetLimas             () const;
          MIL_LimaOrder* FindLima             ( uint nID );
          MIL_LimaOrder* FindLima             ( const MIL_LimaFunction& function );
          MIL_LimaOrder* FindNextScheduledLima();
          void           AffectFuseau         ( const MIL_Fuseau& fuseau );
          void           AffectDirection      ( const MT_Vector2D& direction );
          void           Accept               ( MIL_IntelligenceOrdersVisitor_ABC& visitor ) const;
    //@}

    //! @name Network
    //@{
    void Serialize              ( ASN1T_MissionParameters& asn ) const;
    void CleanAfterSerialization( ASN1T_MissionParameters& asn ) const;
    //@}
 
private:
    //! @name Copy/Assignement
    //@{
    MIL_OrderContext& operator=( const MIL_OrderContext& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadDirection( const ASN1T_MissionParameter& asn );
    void ReadPhaseLines( const ASN1T_MissionParameter& asn );
    void ReadLimits( const ASN1T_MissionParameter& limit1, const ASN1T_MissionParameter& limit2, const MT_Vector2D& orientationReference );
    void ReadIntelligences( const ASN1T_MissionParameter& asn );

    void WriteDirection( ASN1T_MissionParameter& asn ) const;
    void WritePhaseLines( ASN1T_MissionParameter& asn ) const;
    void WriteLimits( ASN1T_MissionParameter& limit1, ASN1T_MissionParameter& limit2 ) const;
    void WriteIntelligences( ASN1T_MissionParameter& asn ) const;
    //@}

    //! @name Helpers
    //@{
    typedef std::vector< MIL_IntelligenceOrder* >   T_IntelligenceOrders;
    typedef T_IntelligenceOrders::const_iterator  CIT_IntelligenceOrders;
    //@}

private:
    bool                 hasContext_;
    T_LimaVector         limas_;
    T_IntelligenceOrders intelligences_;
    MIL_Fuseau           fuseau_;
    MT_Vector2D          dirDanger_;
};

#endif // __MIL_OrderContext_h_
