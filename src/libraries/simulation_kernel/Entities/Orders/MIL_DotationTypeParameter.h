// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_DotationTypeParameter_h_
#define __MIL_DotationTypeParameter_h_

#include "MIL_BaseParameter.h"
class PHY_DotationCategory;

// =============================================================================
/** @class  MIL_DotationTypeParameter
    @brief  MIL_DotationTypeParameter
*/
// Created: LDC 2009-06-05
// =============================================================================
class MIL_DotationTypeParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_DotationTypeParameter( const ASN1T_DotationType & asn );
    explicit MIL_DotationTypeParameter( const PHY_DotationCategory* pCategory );
    virtual ~MIL_DotationTypeParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& type ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToDotationType( ASN1T_DotationType& ) const;
    virtual bool ToDotationType( const PHY_DotationCategory*& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_DotationTypeParameter( const MIL_DotationTypeParameter& );            //!< Copy constructor
    MIL_DotationTypeParameter& operator=( const MIL_DotationTypeParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const PHY_DotationCategory* pCategory_;
    //@}
};

#endif // __MIL_DotationTypeParameter_h_
