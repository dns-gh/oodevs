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
#include "Network/NetworkBufferedPercentageValue.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

class PHY_DotationCategory;

// =============================================================================
/** @class  MineAttribute
    @brief  MineAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class MineAttribute : public ObjectAttribute_ABC
                    , private UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MineAttribute();
             MineAttribute( const PHY_DotationCategory& category, unsigned int nFullNbrDotation, double density );
    explicit MineAttribute( const sword::MissionParameter_Value& attributes );
    virtual ~MineAttribute();
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

    //! @name Knowledge
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    virtual void Register( MIL_Object_ABC& object ) const;
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual bool SendUpdate( sword::ObjectAttributes& asn ) const;
    virtual void OnUpdate( const sword::MissionParameter_Value& attribute );
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void Update( double percentage );
    //@}

    //! @name Accessors
    //@{
    unsigned int GetDotationNeededForConstruction( double rDeltaPercentage ) const;
    unsigned int GetDotationRecoveredWhenDestroying( double rDeltaPercentage ) const;
    double GetState() const;
    unsigned int GetCurrentDotations() const;
    unsigned int GetDotationType() const;
    void Set( double percentage );
    void SetMaxDotations( const PHY_DotationCategory& category, unsigned int nFullNbrDotation );
    bool MustBeMined() const;
    void SetMustBeMined( bool val );
    void SetDensity( double density );
    //@}

    //! @name Copy
    //@{
    MineAttribute& operator=( const MineAttribute& ); //!< Assignment operator
    bool Update( const MineAttribute& rhs );
    //@}

private:
    //! @name Member data
    //@{
    const PHY_DotationCategory* dotation_;
    unsigned int nFullNbrDotation_;
    unsigned int nCurrentNbrDotation_;
    mutable NetworkBufferedPercentageValue< double > miningPercentage_;
    bool bMustBeMined_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MineAttribute )

#endif // __MineAttribute_h_
