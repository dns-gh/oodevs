// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __StockAttribute_h_
#define __StockAttribute_h_

#include "MIL.h"
#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include <boost/serialization/export.hpp>

namespace sword
{
    class ObjectAttributes;
    class ObjectAttributeStock;
    class MissionParameter_Value;
}

namespace xml
{
    class xistream;
}

class PHY_DotationCategory;

// =============================================================================
/** @class  StockAttribute
    @brief  StockAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class StockAttribute : public ObjectAttribute_ABC
                     , private UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             StockAttribute();
    explicit StockAttribute( xml::xistream& xis );
    explicit StockAttribute( const sword::MissionParameter_Value& attributes );
    virtual ~StockAttribute();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Knowledge
    //@{
    void Instanciate( DEC_Knowledge_Object& object ) const;
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name Network Update
    //@{
    void SendFullState( sword::ObjectAttributes& asn ) const;
    bool SendUpdate( sword::ObjectAttributes& asn ) const;
    void OnUpdate( const sword::ObjectAttributes& asn );
    //@}

    //! @name ODB
    //@{
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    bool IsFull() const;
    bool IsEmpty() const;
    bool CanDistribute( const PHY_DotationCategory& category ) const;
    bool CanBeSuppliedWith( const PHY_DotationCategory& category ) const;
    double Supply( const PHY_DotationCategory& category, double quantity );
    double Distribute( const PHY_DotationCategory& category, double quantity );
    void DeprecatedSelectDotations( std::vector< std::pair< const PHY_DotationCategory*, double > >& selection, bool select_full ) const;
    //@}

    //! @name Copy
    //@{
    StockAttribute& operator=( const StockAttribute& ); //!< Assignment operator
    //@}

    //@{
    void LoadDotation( xml::xistream& xis );
    void Send( sword::ObjectAttributeStock& attribute ) const;
    //@}

private:
    //! @name Types
    // <Dotation, <current, max>>
    //@{
    struct StockDotation
    {
        StockDotation()
            : maxStock_( 0  )
            , stock_( 0 )
        {}

        double stock_;
        double maxStock_;
    };
    typedef std::map< const PHY_DotationCategory*, StockDotation > StockDotations;
    //@}

private:
    //! @name Member data
    //@{
    StockDotations stockDotations_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( StockAttribute )

#endif // __StockAttribute_h_
