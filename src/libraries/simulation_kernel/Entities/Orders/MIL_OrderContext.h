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

namespace sword
{
    class MissionParameter;
    class MissionParameters;
}

class DEC_KnowledgeResolver_ABC;
class MIL_LimaFunction;

#define ORDER_BADPARAM(reason)                     \
    MASA_BADPARAM_ASN( sword::OrderAck::ErrorCode, \
        sword::OrderAck::error_invalid_parameter,  \
        static_cast< std::stringstream& >( std::stringstream() << reason ).str().c_str() )

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
             MIL_OrderContext( const sword::MissionParameters& asn, const MT_Vector2D& orientationReference,
                               const DEC_KnowledgeResolver_ABC& resolver );
             MIL_OrderContext( const MIL_OrderContext& rhs );
    virtual ~MIL_OrderContext();
    //@}

    //! @name Accessors
    //@{
          unsigned int   Length               () const;
    boost::shared_ptr< MT_Vector2D >          GetDirDanger() const;
    const MIL_Fuseau&    GetFuseau            () const;
    const T_LimaVector&  GetLimas             () const;
          MIL_LimaOrder* FindLima             ( unsigned int nID );
          MIL_LimaOrder* FindLima             ( const MIL_LimaFunction& function );
    std::vector< MIL_LimaOrder* > FindAllLimas( const MIL_LimaFunction& func );
          MIL_LimaOrder* FindNextScheduledLima();
          void           AffectFuseau         ( const MIL_Fuseau& fuseau );
          void           AffectDirection      ( const MT_Vector2D& direction );
    //@}

    //! @name Network
    //@{
    void Serialize( sword::MissionParameters& asn ) const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_OrderContext& operator=( const MIL_OrderContext& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void WriteDirection( sword::MissionParameter& asn ) const;
    void WritePhaseLines( sword::MissionParameter& asn ) const;
    void WriteLimits( sword::MissionParameter& limit1, sword::MissionParameter& limit2 ) const;
    //@}

private:
    //! @name Member Data
    //@{
    bool hasContext_;
    T_LimaVector limas_;
    MIL_Fuseau fuseau_;
    boost::shared_ptr< MT_Vector2D > dirDanger_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_OrderContext )

#endif // __MIL_OrderContext_h_
