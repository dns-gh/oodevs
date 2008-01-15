// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Mission_ABC_h_
#define __MIL_Mission_ABC_h_

#include "MIL.h"

#include "MIL_OrderContext.h"

class MIL_MissionType_ABC;
class MIL_FragOrderType;
class DEC_KnowledgeResolver_ABC;
class MIL_KnowledgeGroup;

// =============================================================================
/** @class  MIL_Mission_ABC
    @brief  MIL_Mission_ABC
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_Mission_ABC : public DIA_Thing
{
public:
    //! @name Constructors/Destructor
    //@{
    virtual ~MIL_Mission_ABC();
    //@}

     //! @name Operations
    //@{
    virtual void Start               () = 0;
    virtual bool IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const = 0;
    //@}

    //! @name Accessors
    //@{
    const   std::string&         GetName              () const;
    const   MIL_MissionType_ABC& GetType              () const;
    const   MT_Vector2D&         GetDirDanger         () const;
    const   MIL_Fuseau&          GetFuseau            () const;
    const   T_LimaVector&        GetLimas             () const;
            MIL_LimaOrder*       FindLima             ( uint nID );
            MIL_LimaOrder*       FindLima             ( const MIL_LimaFunction& function );
            MIL_LimaOrder*       FindNextScheduledLima();
            void                 AffectFuseau         ( const MIL_Fuseau& fuseau );
    virtual void                 AffectDirection      ( const MT_Vector2D& direction );
            void                 Accept               ( MIL_IntelligenceOrdersVisitor_ABC& visitor ) const;
    //@}

protected:
    //! @name Constructors/Destructor
    //@{
    MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const ASN1T_MissionParameters& parameters );
    MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const ASN1T_MissionParameters& parameters, const ASN1T_OrderContext& context, const MT_Vector2D& refPosition );
    MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver );
    MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_Mission_ABC& parent );
    MIL_Mission_ABC( const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_Mission_ABC& rhs );
    //@}

    //! @name Network
    //@{
    void Serialize              ( ASN1T_OrderContext&      asn ) const;
    void Serialize              ( ASN1T_MissionParameters& asn ) const;
    void CleanAfterSerialization( ASN1T_OrderContext&      asn ) const;
    void CleanAfterSerialization( ASN1T_MissionParameters& asn ) const;
    //@}

private:
    const DEC_KnowledgeResolver_ABC& knowledgeResolver_;
    const MIL_MissionType_ABC&       type_;
          MIL_OrderContext           context_;
};

#include "MIL_Mission_ABC.inl"

#endif // __MIL_Mission_ABC_h_
