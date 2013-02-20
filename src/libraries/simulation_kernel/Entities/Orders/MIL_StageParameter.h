// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MIL_StageParameter_h_
#define __MIL_StageParameter_h_

#include "MIL_StringParameter.h"

// =============================================================================
/** @class  MIL_StageParameter
    @brief  MIL_StageParameter
*/
// Created: ABR 2012-12-18
// =============================================================================
class MIL_StageParameter : public MIL_StringParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_StageParameter();
    explicit MIL_StageParameter( const std::string& message );
    virtual ~MIL_StageParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __MIL_StageParameter_h_
