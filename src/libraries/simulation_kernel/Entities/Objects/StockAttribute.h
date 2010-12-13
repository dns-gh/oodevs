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
             StockAttribute( const PHY_DotationCategory& category, unsigned int nFullNbrDotation );
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
    void SendUpdate( sword::ObjectAttributes& asn ) const;
    void OnUpdate( const sword::ObjectAttributes& asn );
    //@}

    //! @name ODB
    //@{
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    bool IsFull() const;
    unsigned int Supply( const PHY_DotationCategory& category, unsigned int quantity );
    unsigned int Distribute( const PHY_DotationCategory& category, unsigned int quantity );
    void SelectDotations( std::vector< std::pair< const PHY_DotationCategory*, unsigned int > >& selection, bool select_full ) const;
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
    typedef std::pair< unsigned int, unsigned int >         T_pair;
    typedef std::map< const PHY_DotationCategory*, T_pair > T_DotationProgress;
    typedef T_DotationProgress::iterator                   IT_DotationProgress;
    typedef T_DotationProgress::const_iterator            CIT_DotationProgress;
    //@}

private:
    //! @name Member data
    //@{
    T_DotationProgress stock_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( StockAttribute )

#endif // __StockAttribute_h_
