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

namespace Common
{
    class MsgMissionParameter;
    class MsgMissionParameters;
}

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
             MIL_OrderContext( const Common::MsgMissionParameters& asn, const MT_Vector2D& orientationReference );
             MIL_OrderContext( const MIL_OrderContext& rhs );
    virtual ~MIL_OrderContext();
    //@}

    //! @name Accessors
    //@{
          unsigned int   Length               () const;
    const MT_Vector2D&   GetDirDanger         () const;
    const MIL_Fuseau&    GetFuseau            () const;
    const T_LimaVector&  GetLimas             () const;
          MIL_LimaOrder* FindLima             ( unsigned int nID );
          MIL_LimaOrder* FindLima             ( const MIL_LimaFunction& function );
          MIL_LimaOrder* FindNextScheduledLima();
          void           AffectFuseau         ( const MIL_Fuseau& fuseau );
          void           AffectDirection      ( const MT_Vector2D& direction );
          void           Accept               ( MIL_IntelligenceOrdersVisitor_ABC& visitor ) const;
    //@}

    //! @name Network
    //@{
    void Serialize( Common::MsgMissionParameters& asn ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_OrderContext& operator=( const MIL_OrderContext& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadDirection( const Common::MsgMissionParameter& asn );
    void ReadPhaseLines( const Common::MsgMissionParameter& asn );
    void ReadLimits( const Common::MsgMissionParameter& limit1, const Common::MsgMissionParameter& limit2, const MT_Vector2D& orientationReference );
    void ReadIntelligences( const Common::MsgMissionParameter& asn );

    void WriteDirection( Common::MsgMissionParameter& asn ) const;
    void WritePhaseLines( Common::MsgMissionParameter& asn ) const;
    void WriteLimits( Common::MsgMissionParameter& limit1, Common::MsgMissionParameter& limit2 ) const;
    void WriteIntelligences( Common::MsgMissionParameter& asn ) const;
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
