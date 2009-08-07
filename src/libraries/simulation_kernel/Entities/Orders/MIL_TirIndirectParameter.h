// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_TirIndirectParameter_h_
#define __MIL_TirIndirectParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_TirIndirectParameter
    @brief  MIL_TirIndirectParameter
*/
// Created: LDC 2009-06-05
// =============================================================================
class MIL_TirIndirectParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_TirIndirectParameter( const ASN1T_UnitFire & asn );
    virtual ~MIL_TirIndirectParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& type ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToIndirectFire( ASN1T_UnitFire& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_TirIndirectParameter( const MIL_TirIndirectParameter& );            //!< Copy constructor
    MIL_TirIndirectParameter& operator=( const MIL_TirIndirectParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    int data_;
    //@}
};

#endif // __MIL_TirIndirectParameter_h_
