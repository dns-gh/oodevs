// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_DateTimeParameter_h_
#define __MIL_DateTimeParameter_h_

#include "MIL_BaseParameter.h"

class PHY_DotationCategory;

// =============================================================================
/** @class  MIL_DateTimeParameter
    @brief  MIL_DateTimeParameter
*/
// Created: LDC 2009-06-05
// =============================================================================
class MIL_DateTimeParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_DateTimeParameter( const Common::MsgDateTime& asn );
    virtual ~MIL_DateTimeParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToGDH( Common::MsgDateTime& ) const;
    virtual bool ToGDH( float& value ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_DateTimeParameter( const MIL_DateTimeParameter& );            //!< Copy constructor
    MIL_DateTimeParameter& operator=( const MIL_DateTimeParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int dateTime_;
    //@}
};

#endif // __MIL_DateTimeParameter_h_
