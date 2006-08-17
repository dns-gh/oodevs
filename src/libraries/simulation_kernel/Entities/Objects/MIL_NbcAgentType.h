//*****************************************************************************
//
// $Created: JVT 03-01-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_NbcAgentType.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:29 $
// $Revision: 4 $
// $Workfile: MIL_NbcAgentType.h $
//
//*****************************************************************************

#ifndef __MIL_NbcAgentType_h_
#define __MIL_NbcAgentType_h_

#include "MIL.h"

#include "MT_Tools/MT_Random.h"

class PHY_HumanWound;

//*****************************************************************************
// Created: JVT 03-01-06
// Modified: JVT 2004-10-28
//*****************************************************************************
class MIL_NbcAgentType
{
    MT_COPYNOTALLOWED( MIL_NbcAgentType )

public:
    //! @name Factory
    //@{
    static void Initialize( MIL_InputArchive& archive );
    static void Terminate();

    static const MIL_NbcAgentType* FindNbcAgentType( const std::string& strName );
    static const MIL_NbcAgentType* FindNbcAgentType( uint nID );
    //@}

    //! @name Accessors
    //@{
    uint                   GetID              () const;
    const std::string&     GetName            () const;
    MT_Float               GetWoundFactor     ( const PHY_HumanWound& wound ) const;
    const PHY_HumanWound&  GetRandomWound     () const;
    uint                   GetLifeTime        () const;
    MT_Float               GetPropagationAngle() const;
    //@}

    //! @name Accessors
    //@{
    static MT_Float GetCoefMaxSpeedModificator     ();
    static MT_Float GetCoefReloadingTimeModificator();
    static MT_Float GetMinPropagationSpeed         ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_NbcAgentType*, sCaseInsensitiveLess > T_NbcAgentTypeMap;
    typedef T_NbcAgentTypeMap::const_iterator                                      CIT_NbcAgentTypeMap;

    typedef std::vector< MT_Float >               T_HumanCasualtyVector;
    typedef T_HumanCasualtyVector::const_iterator CIT_HumanCasualtyVector;
    //@}

private:
     MIL_NbcAgentType( const std::string& strName, MIL_InputArchive& archive );
    ~MIL_NbcAgentType();

private:
    const std::string           strName_;
          uint                  nID_;
          uint                  nLifeTime_;
          MT_Float              rPropagationAngle_;
          T_HumanCasualtyVector humanCasualties_;

private:
    static T_NbcAgentTypeMap nbcAgentTypes_;
    static MT_Float          rCoefMaxSpeedModificator_;
    static MT_Float          rCoefReloadingTimeModificator_;
    static MT_Float          rMinPropagationSpeed_;
    static MT_Random         randomGenerator_;
};

#include "MIL_NbcAgentType.inl"


#endif // __MIL_NbcAgentType_h_