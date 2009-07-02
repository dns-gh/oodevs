// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_FragOrder_h_
#define __MIL_FragOrder_h_

class MIL_FragOrderType;
class DEC_KnowledgeResolver_ABC;
class DEC_Representations;

// =============================================================================
/** @class  MIL_FragOrder
    @brief  MIL_FragOrder
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_FragOrder : public DIA_TypedObject
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_FragOrder( const MIL_FragOrderType& type, DEC_Representations& engine, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const ASN1T_MsgFragOrder& asn );
             MIL_FragOrder( const MIL_FragOrderType& type, DEC_Representations& engine, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_FragOrder& rhs );
    virtual ~MIL_FragOrder();
    //@}

    //! @name DIA
    //@{
    static void InitializeDIA();
    //@}

    //! @name Accessors
    //@{
//    const MIL_FragOrderType& GetType() const;
    //@}

    //! @name Operations
    //@{
    void Launch();
    //@}

private:
    const MIL_FragOrderType&   type_;
          DEC_Representations& diaEngine_;
};

#endif // __MIL_FragOrder_h_
