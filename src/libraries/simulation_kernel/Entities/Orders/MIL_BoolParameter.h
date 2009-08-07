// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_BoolParameter_h_
#define __MIL_BoolParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_BoolParameter
    @brief  MIL_BoolParameter
*/
// Created: LDC 2009-05-22
// =============================================================================
class MIL_BoolParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_BoolParameter( bool );
    virtual ~MIL_BoolParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToBool( bool& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_BoolParameter( const MIL_BoolParameter& );            //!< Copy constructor
    MIL_BoolParameter& operator=( const MIL_BoolParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    bool value_;
    //@}
};

#endif // __MIL_BoolParameter_h_
