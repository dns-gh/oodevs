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
#include "Network/NetworkBufferedPercentageValue.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
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
             ConstructionAttribute( const PHY_DotationCategory& category, unsigned int nFullNbrDotation, double density );
             ConstructionAttribute( const PHY_DotationCategory& dotation, const sword::ObjectAttributes& asn );
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
    virtual void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name Knowledge
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    //@}

    //! @name Network Update
    //@{
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual bool SendUpdate( sword::ObjectAttributes& asn ) const;
    virtual void OnUpdate( const sword::MissionParameter_Value& attribute );
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void Build( double rDeltaPercentage );
    void NotifyBuildByGen();
    void NotifyStopBuildByGen();
    void Set( double percentage );
    void SetMaxDotations( const PHY_DotationCategory& category, unsigned int nFullNbrDotation );
    void SetDensity( double density );
    //@}

    //! @name Accessors
    //@{
    bool HasDotation( const PHY_DotationCategory& dotation ) const;
    unsigned int GetDotationNeededForConstruction( double rDeltaPercentage ) const;
    unsigned int GetDotationRecoveredWhenDestroying( double rDeltaPercentage ) const;
    double GetState() const;
    bool NeedDestruction() const;
    unsigned int GetMaxDotation() const;
    bool IsConstructed() const;
    //@}

    //! @name Copy
    //@{
    ConstructionAttribute& operator=( const ConstructionAttribute& ); //!< Assignment operator
    bool Update( const ConstructionAttribute& rhs );
    //@}

private:
    //! @name Member data
    //@{
    const PHY_DotationCategory* dotation_;
    unsigned int nFullNbrDotation_;
    unsigned int nCurrentNbrDotation_;
    mutable NetworkBufferedPercentageValue< double > constructionPercentage_;
    bool bBuildByGen_;
    double density_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ConstructionAttribute )

#endif // __ConstructionAttribute_h_
