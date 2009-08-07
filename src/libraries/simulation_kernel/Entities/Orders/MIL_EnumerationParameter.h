// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_EnumerationParameter_h_
#define __MIL_EnumerationParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_EnumerationParameter
    @brief  MIL_EnumerationParameter
*/
// Created: LDC 2009-05-22
// =============================================================================
class MIL_EnumerationParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_EnumerationParameter( int );
    virtual ~MIL_EnumerationParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToId( int& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_EnumerationParameter( const MIL_EnumerationParameter& );            //!< Copy constructor
    MIL_EnumerationParameter& operator=( const MIL_EnumerationParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    int value_;
    //@}
};

#endif // __MIL_EnumerationParameter_h_
