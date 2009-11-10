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

class PHY_DotationCategory;

// =============================================================================
/** @class  StockAttribute
    @brief  StockAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class StockAttribute 
    : public ObjectAttribute_ABC
    , private UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             StockAttribute();
    explicit StockAttribute( xml::xistream& xis );
             StockAttribute( const PHY_DotationCategory& category, uint nFullNbrDotation );
    virtual ~StockAttribute();
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

    //! @name Operations
    //@{
    bool IsFull() const;
    uint Supply( const PHY_DotationCategory& category, uint quantity );
    uint Distribute( const PHY_DotationCategory& category, uint quantity );
    void SelectDotations( std::vector< std::pair< const PHY_DotationCategory*, uint >>& selection, bool select_full ) const;
    //@}

    //! @name Copy
    //@{
    StockAttribute& operator=( const StockAttribute& ); //!< Assignment operator
    //@}


    //@{
    void LoadDotation( xml::xistream& xis );
    void Send( ASN1T_StockResourceList& attribute, bool send_max ) const;
    //@}

public:
    //! @name Types
    // <Dotation, <current, max>>
    //@{
    typedef std::map< const PHY_DotationCategory*, std::pair< uint, uint > >    T_DotationProgress;
    typedef T_DotationProgress::const_iterator                                  CIT_DotationProgress;
    //@}

private:
    //! @name Member data
    //@{
    T_DotationProgress  stock_;
    //@}
};

#endif // __StockAttribute_h_
