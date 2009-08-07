// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_DirectionParameter_h_
#define __MIL_DirectionParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_DirectionParameter
    @brief  MIL_DirectionParameter
*/
// Created: LDC 2009-05-26
// =============================================================================
class MIL_DirectionParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_DirectionParameter( const ASN1T_Heading& heading );
    virtual ~MIL_DirectionParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToDirection( ASN1T_Heading& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_DirectionParameter( const MIL_DirectionParameter& );            //!< Copy constructor
    MIL_DirectionParameter& operator=( const MIL_DirectionParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    int heading_;
    //@}
};

#endif // __MIL_DirectionParameter_h_
