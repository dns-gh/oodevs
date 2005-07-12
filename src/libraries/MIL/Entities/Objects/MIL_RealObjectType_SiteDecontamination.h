// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_RealObjectType_SiteDecontamination.h $
// $Author: Nld $
// $Modtime: 2/11/04 16:46 $
// $Revision: 1 $
// $Workfile: MIL_RealObjectType_SiteDecontamination.h $
//
// *****************************************************************************

#ifndef __MIL_RealObjectType_SiteDecontamination_h_
#define __MIL_RealObjectType_SiteDecontamination_h_

#include "MIL.h"

#include "MIL_RealObjectType.h"

// =============================================================================
// @class  MIL_RealObjectType_SiteDecontamination
// Created: JVT 2004-08-03
// =============================================================================
class MIL_RealObjectType_SiteDecontamination : public MIL_RealObjectType
{
    MT_COPYNOTALLOWED( MIL_RealObjectType_SiteDecontamination )

    friend class MIL_RealObjectType;

public:
    //! @name Accessors
    //@{
    uint GetNbrMaxEnablers() const;
    //@}

private:
    MIL_RealObjectType_SiteDecontamination( const std::string strName, E_ObjectType nType, ASN1T_EnumObjectType nAsnID, MIL_MOSIDManager& idManager, MIL_RealObjectType::T_ObjectInstanciator objectInstanciator );
    virtual ~MIL_RealObjectType_SiteDecontamination();

    //! @name Init tools
    //@{
    virtual void Read( MIL_InputArchive& archive );
    //@}

private:
    uint nNbrMaxEnablers_;
};

#include "MIL_RealObjectType_SiteDecontamination.inl"

#endif // __MIL_RealObjectType_SiteDecontamination_h_
