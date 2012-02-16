// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ResourceNetworkTypeParameter_h_
#define __MIL_ResourceNetworkTypeParameter_h_

#include "MIL_BaseParameter.h"

class PHY_ResourceNetworkType;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_ResourceNetworkTypeParameter
    @brief  MIL_ResourceNetworkTypeParameter
*/
// Created: ABR 2012-02-15
// =============================================================================
class MIL_ResourceNetworkTypeParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_ResourceNetworkTypeParameter();
    explicit MIL_ResourceNetworkTypeParameter( const sword::ResourceNetworkType& message );
    explicit MIL_ResourceNetworkTypeParameter( const PHY_ResourceNetworkType* pNetworkType );
    virtual ~MIL_ResourceNetworkTypeParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversion
    //@{
    virtual bool ToResourceNetworkType( const PHY_ResourceNetworkType*& ) const;
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
    const PHY_ResourceNetworkType* pNetworkType_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_ResourceNetworkTypeParameter )

#endif // __MIL_ResourceNetworkTypeParameter_h_
