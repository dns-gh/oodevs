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
#include <boost/serialization/export.hpp>

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
             ConstructionAttribute( const PHY_DotationCategory& category, unsigned int nFullNbrDotation );
             ConstructionAttribute( const PHY_DotationCategory& dotation, const Common::MsgObjectAttributes& asn );
    virtual ~ConstructionAttribute();
    //@}

    //! @name Initialize
    //@{
    void Load( xml::xistream& xis );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()    
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( Object& object ) const;
    //@}

    //! @name Knowledge
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;    
    //@}

    //! @name Network Update
    //@{
    virtual void SendFullState( Common::MsgObjectAttributes& asn ) const;
    virtual void SendUpdate( Common::MsgObjectAttributes& asn ) const;
    virtual void OnUpdate( const Common::MsgMissionParameter_Value& attribute );        
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
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
    void NotifyBuildByGen();
    void NotifyStopBuildByGen();
    void Set( MT_Float percentage );
    //@}

    //! @name Accessors
    //@{    
    bool HasDotation( const PHY_DotationCategory& dotation ) const;
    unsigned int GetDotationNeededForConstruction( MT_Float rDeltaPercentage ) const;
    unsigned int GetDotationRecoveredWhenDestroying( MT_Float rDeltaPercentage ) const;
    MT_Float GetState() const;
    bool NeedDestruction() const;
    unsigned int GetMaxDotation() const;
    unsigned int GetCurrentDotation() const;
    unsigned int GetDotationType() const;
    //@}

    //! @name Copy
    //@{
    ConstructionAttribute& operator=( const ConstructionAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, std::pair< unsigned int, unsigned int > >    T_DotationProgress;
    //@}
    

private:
    //! @name Member data
    //@{
    T_DotationProgress  base_;
    T_DotationProgress  improvement_;

    const PHY_DotationCategory* dotation_;
    unsigned int        nFullNbrDotation_;
    unsigned int        nCurrentNbrDotation_;
    MT_Float    rConstructionPercentage_;
    bool                bBuildByGen_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ConstructionAttribute )

#endif // __ConstructionAttribute_h_
