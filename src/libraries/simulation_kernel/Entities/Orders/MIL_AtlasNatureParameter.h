// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AtlasNatureParameter_h_
#define __MIL_AtlasNatureParameter_h_

#include "MIL_BaseParameter.h"

namespace sword
{
    class Nature;
}

// =============================================================================
/** @class  MIL_AtlasNatureParameter
    @brief  MIL_AtlasNatureParameter
*/
// Created: LDC 2009-05-25
// =============================================================================
class MIL_AtlasNatureParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_AtlasNatureParameter( const sword::Nature& );
    explicit MIL_AtlasNatureParameter( int nature );
    virtual ~MIL_AtlasNatureParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    virtual bool ToNatureAtlas( int& atlas ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_AtlasNatureParameter( const MIL_AtlasNatureParameter& );            //!< Copy constructor
    MIL_AtlasNatureParameter& operator=( const MIL_AtlasNatureParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    int nature_;
    //@}
};

#endif // __MIL_AtlasNatureParameter_h_
