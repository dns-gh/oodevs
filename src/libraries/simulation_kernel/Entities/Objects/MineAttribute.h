// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MineAttribute_h_
#define __MineAttribute_h_

#include "MIL.h"
#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"

namespace hla 
{
    class Deserializer;
}

class PHY_DotationCategory;

// =============================================================================
/** @class  MineAttribute
    @brief  MineAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class MineAttribute 
    : public ObjectAttribute_ABC
    , private UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MineAttribute();    
    explicit MineAttribute( const PHY_DotationCategory& category, uint nFullNbrDotation );
    virtual ~MineAttribute();
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
    void SendFullState( ASN1T_ObjectAttributes& asn ) const;
    void SendUpdate( ASN1T_ObjectAttributes& asn ) const;
    void OnUpdate( const ASN1T_ObjectAttributes& asn );
    void Serialize( HLA_UpdateFunctor& functor ) const;
    void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );    
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void Update( MT_Float percentage );
    //@}

    //! @name Accessors
    //@{    
    uint        GetDotationNeededForConstruction( MT_Float rDeltaPercentage ) const;
    uint        GetDotationRecoveredWhenDestroying( MT_Float rDeltaPercentage ) const;
    MT_Float    GetState() const;
    uint        GetCurrentDotations() const;
    //@}

    //! @name Copy
    //@{
    MineAttribute& operator=( const MineAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name 
    //@{
    void Set( MT_Float percentage );
    //@}

private:
    //! @name Member data
    //@{
    const PHY_DotationCategory* dotation_;
    uint     nFullNbrDotation_;
    uint     nCurrentNbrDotation_;
    MT_Float rMiningPercentage_;
    uint     nMinesActivityTime_;
    uint     nDeathTimeStep_;
    //@}
};

#endif // __MineAttribute_h_
