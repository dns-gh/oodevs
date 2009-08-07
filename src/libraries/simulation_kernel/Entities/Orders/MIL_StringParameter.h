// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_StringParameter_h_
#define __MIL_StringParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_StringParameter
    @brief  MIL_StringParameter
*/
// Created: LDC 2009-06-05
// =============================================================================
class MIL_StringParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_StringParameter( const ASN1VisibleString& asn );
    explicit MIL_StringParameter( const std::string& message );
    virtual ~MIL_StringParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& type ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToString( ASN1VisibleString& asn ) const;
    virtual bool ToString( std::string& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_StringParameter( const MIL_StringParameter& );            //!< Copy constructor
    MIL_StringParameter& operator=( const MIL_StringParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string value_;
    //@}
};

#endif // __MIL_StringParameter_h_
