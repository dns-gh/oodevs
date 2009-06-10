// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ConstructionAttribute_h_
#define __ConstructionAttribute_h_

#include "MIL.h"
#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"

namespace hla 
{
    class Deserializer;
}

class PHY_DotationCategory;

// =============================================================================
/** @class  ConstructionAttribute
    @brief  ConstructionAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class ConstructionAttribute : public ObjectAttribute_ABC
                            , private UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ConstructionAttribute();
             ConstructionAttribute( const PHY_DotationCategory& category, uint nFullNbrDotation );
             ConstructionAttribute( const PHY_DotationCategory& dotation, const ASN1T_ObjectAttributes& asn );
    virtual ~ConstructionAttribute();
    //@}

    //! @name Initialize
    //@{
    void Load( xml::xistream& xis );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Knowledge
    //@{
    void Instanciate( DEC_Knowledge_Object& object ) const;    
    //@}

    //! @name Network Update
    //@{
    void SendFullState( ASN1T_ObjectAttributes& asn ) const;
    void SendUpdate( ASN1T_ObjectAttributes& asn ) const;
    void OnUpdate( const ASN1T_ObjectAttributes& asn );        
    //@}

    //! @name ODB
    //@{
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name HLA
    //@{
    void Serialize( HLA_UpdateFunctor& functor ) const;
    void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    //@}

    //! @name Operations
    //@{
    void Build( MT_Float rDeltaPercentage );
    void Improve( MT_Float rDeltaPercentage );    
    //@}

    //! @name Accessors
    //@{    
    bool HasDotation( const PHY_DotationCategory& dotation ) const;
    uint GetDotationNeededForConstruction( MT_Float rDeltaPercentage ) const;
    uint GetDotationRecoveredWhenDestroying( MT_Float rDeltaPercentage ) const;
    MT_Float GetState() const;
    uint GetMaxDotation() const;
    //@}

    //! @name Copy
    //@{
    ConstructionAttribute& operator=( const ConstructionAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, std::pair< uint, uint > >    T_DotationProgress;
    //@}

private:
    //! @name 
    //@{
    void Set( MT_Float percentage );
    //@}

private:
    //! @name Member data
    //@{
    T_DotationProgress  base_;
    T_DotationProgress  improvement_;

    const PHY_DotationCategory* dotation_;
    uint        nFullNbrDotation_;
    uint        nCurrentNbrDotation_;
    MT_Float    rConstructionPercentage_;
    //@}
};

#endif // __ConstructionAttribute_h_
