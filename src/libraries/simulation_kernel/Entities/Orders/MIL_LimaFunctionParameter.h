// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __MIL_LimaFunctionParameter_h_
#define __MIL_LimaFunctionParameter_h_

#include "MIL_BaseParameter.h"

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_LimaFunctionParameter
    @brief  MIL_LimaFunctionParameter
*/
// Created: MMC 2013-10-14
// =============================================================================
class MIL_LimaFunctionParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_LimaFunctionParameter();
    explicit MIL_LimaFunctionParameter( const sword::EnumPhaseLineFunction& phaseLinefunction );
    virtual ~MIL_LimaFunctionParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToId( int& ) const;
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
    int id_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_LimaFunctionParameter )

#endif // __MIL_LimaFunctionParameter_h_
