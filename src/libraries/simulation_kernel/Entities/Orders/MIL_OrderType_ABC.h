// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_OrderType_ABC_h_
#define __MIL_OrderType_ABC_h_

namespace xml
{
    class xistream;
}

class MIL_OrderTypeParameter;
class DEC_KnowledgeResolver_ABC;
class DIA_Model;
class MIL_OrderContext;

// =============================================================================
/** @class  MIL_OrderType_ABC
    @brief  MIL_OrderType_ABC
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_OrderType_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_OrderType_ABC( uint nID, xml::xistream& xis );
    virtual ~MIL_OrderType_ABC();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName   () const;
    const DIA_TypeDef& GetDIAType() const;
          uint         GetID     () const;
    //@}

    //! @name Copy operations
    //@{
    void Copy                   ( const ASN1T_MissionParameters& from, DIA_TypedObject&         to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_OrderContext& context ) const;
    bool Copy                   ( const DIA_TypedObject&         from, DIA_TypedObject&         to, const DEC_KnowledgeResolver_ABC& knowledgeResolver ) const;
    bool Copy                   ( const DIA_TypedObject&         from, ASN1T_MissionParameters& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_OrderContext& context ) const;
    void CleanAfterSerialization( ASN1T_MissionParameters& to, const MIL_OrderContext& context ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const MIL_OrderTypeParameter* > T_MissionParameterVector;
    typedef T_MissionParameterVector::const_iterator     CIT_MissionParameterVector;
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    //@}

private:
    const uint                     nID_;
          std::string              strName_;
    const DIA_TypeDef*             pDIAType_;
          T_MissionParameterVector parameters_;
};

#endif // __MIL_OrderType_ABC_h_
