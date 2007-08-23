// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_VirtualObjectType_ControlZone.h $
// $Author: Nld $
// $Modtime: 27/10/04 17:09 $
// $Revision: 1 $
// $Workfile: MIL_VirtualObjectType_ControlZone.h $
//
// *****************************************************************************

#ifndef __MIL_VirtualObjectType_ControlZone_h_
#define __MIL_VirtualObjectType_ControlZone_h_

#include "MIL.h"

#include "MIL_VirtualObjectType.h"

namespace xml
{
    class xistream;
}

class PHY_Volume;

// =============================================================================
// @class  MIL_VirtualObjectType_ControlZone
// Created: JVT 2004-08-03
// =============================================================================
class MIL_VirtualObjectType_ControlZone : public MIL_VirtualObjectType
{
    MT_COPYNOTALLOWED( MIL_VirtualObjectType_ControlZone )

public:
             MIL_VirtualObjectType_ControlZone( const std::string& strName );
    virtual ~MIL_VirtualObjectType_ControlZone();

    //! @name Operations
    //@{
    MT_Float GetUnitDensityFirePercentage( const PHY_Volume& ) const;
    //@}

private:
    //! @name Init tools
    //@{
    virtual void Read( xml::xistream& xis );
    //@}
    //! @name Helpers
    //@{
    void ReadPercentage( xml::xistream& xis );
    //@}

private:
    //! @name types
    //@{
    typedef std::vector< MT_Float > T_UnitDensityFirePercentageVector;
    //@}

private:
    T_UnitDensityFirePercentageVector vUnitDensityFirePercentage_; // %.humain-1.px-2 ( 0..1 )
};

#include "MIL_VirtualObjectType_ControlZone.inl"

#endif // __MIL_VirtualObjectType_ControlZone_h_
