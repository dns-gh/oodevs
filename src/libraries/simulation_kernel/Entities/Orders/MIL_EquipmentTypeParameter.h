// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_EquipmentTypeParameter_h_
#define __MIL_EquipmentTypeParameter_h_

#include "MIL_BaseParameter.h"

class PHY_ComposanteTypePion;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_EquipmentTypeParameter
    @brief  MIL_EquipmentTypeParameter
*/
// Created: LDC 2009-06-05
// =============================================================================
class MIL_EquipmentTypeParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_EquipmentTypeParameter();
    explicit MIL_EquipmentTypeParameter( const sword::EquipmentType& asn );
    explicit MIL_EquipmentTypeParameter( const PHY_ComposanteTypePion* pEquipmentType );
    virtual ~MIL_EquipmentTypeParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToEquipmentType( const PHY_ComposanteTypePion*& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

private:
    //! @name Member data
    //@{
    const PHY_ComposanteTypePion* pType_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_EquipmentTypeParameter )

#endif // __MIL_EquipmentTypeParameter_h_
