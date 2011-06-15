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

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

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
             MIL_StringParameter();
    explicit MIL_StringParameter( const std::string& message );
    virtual ~MIL_StringParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToString( std::string& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@

private:
    //! @name Member data
    //@{
    std::string value_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_StringParameter )

#endif // __MIL_StringParameter_h_
