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
    explicit MIL_EquipmentTypeParameter( const Common::EquipmentType & asn );
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
    virtual bool ToElement( Common::MsgMissionParameter_Value& elem ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_EquipmentTypeParameter( const MIL_EquipmentTypeParameter& );            //!< Copy constructor
    MIL_EquipmentTypeParameter& operator=( const MIL_EquipmentTypeParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const PHY_ComposanteTypePion* pType_;
    //@}
};

#endif // __MIL_EquipmentTypeParameter_h_
