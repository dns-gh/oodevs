//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_FloatingBridge.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 6 $
// $Workfile: MIL_FloatingBridge.h $
//
//*****************************************************************************

#ifndef __MIL_FloatingBridge_h_
#define __MIL_FloatingBridge_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"
class TER_DynamicData;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_FloatingBridge : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_FloatingBridge );

public:
             MIL_FloatingBridge();
    virtual ~MIL_FloatingBridge();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();

    virtual bool                      Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    virtual void                      Initialize( uint nID, MIL_InputArchive& archive );
    virtual ASN1T_EnumObjectErrorCode Initialize( uint nID, const ASN1T_MagicActionCreateObject& asnCreateObject );
    //@}

    //! @name HLA
    //@{
    virtual bool Initialize( const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass );
    //@}

private:
    //! @name Tools
    //@{
    void LinkToPathFinder    ();
    void UnlinkFromPathFinder();
    //@}

    //! @name Events
    //@{
    virtual void ProcessAgentEntering( MIL_Agent_ABC& agent );
    //@}

private:
    // Path finder enhancement
    TER_DynamicData* pPathfindData_;
};

#include "MIL_FloatingBridge.inl"

#endif // __MIL_FloatingBridge_h_
