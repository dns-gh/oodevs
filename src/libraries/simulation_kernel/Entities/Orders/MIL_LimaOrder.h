// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_LimaOrder_h_
#define __MIL_LimaOrder_h_

#include "MT_Tools/MT_Vector2DTypes.h"
#include <boost/serialization/export.hpp>

namespace sword
{
    class PhaseLineOrder;
}

class DEC_KnowledgeResolver_ABC;
class DEC_Knowledge_Object;
class MT_Line;
class MIL_LimaFunction;
class TER_Localisation;
struct ASN1T_LimaOrder;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_LimaOrder
    @brief  MIL_LimaOrder
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_LimaOrder
{
private:
    //! @name Types
    //@{
    typedef std::vector< const MIL_LimaFunction* > T_LimaFunctions;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MIL_LimaOrder();
             MIL_LimaOrder( const sword::PhaseLineOrder& asn, const DEC_KnowledgeResolver_ABC& resolver );
    virtual ~MIL_LimaOrder();
    //@}

    //! @name Operations
    //@{
    bool HasFunction      ( const MIL_LimaFunction& function ) const;
    void Flag             ( bool bFlag );
    bool IsFlagged        () const;
    void FlagSchedule     ( bool bFlag );
    bool IsScheduleFlagged() const;
    //@}

    //! @name Accessors
    //@{
          unsigned int     GetID       () const;
    const T_PointVector&   GetPoints   () const;
    const T_LimaFunctions& GetFunctions() const;
          unsigned int     GetSchedule () const;
    boost::shared_ptr< TER_Localisation > GetLocalisation() const;
    //@}

    //! @name Geometry $$$ BULLSHIT
    //@{
    bool Intersect2D( const MT_Line& line, MT_Vector2D& vPos ) const;
    bool Intersect2D( const T_PointVector& polyline, T_PointSet& intersectionSet ) const; //$$$ devrait gicler
    void ReplacePointsByNearestObjectPositions( T_PointVector& points ) const;
    //@}

    //! @name Network
    //@{
    void Serialize( sword::PhaseLineOrder& asn ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}

private:
    unsigned int nID_;
    boost::shared_ptr< TER_Localisation > localisation_;
    T_LimaFunctions functions_;
    bool bFlag_;
    bool bScheduleFlag_;
    unsigned int nSchedule_;
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > objects_;

private:
    static unsigned int nNextID_;
};

BOOST_CLASS_EXPORT_KEY( MIL_LimaOrder )

typedef std::vector< MIL_LimaOrder  > T_LimaVector;

#endif // __MIL_LimaOrder_h_
