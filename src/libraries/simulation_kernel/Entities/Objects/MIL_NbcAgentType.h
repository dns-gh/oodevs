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

namespace xml
{
    class xistream;
}

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
    static void Initialize( xml::xistream& xis );
    static void Terminate();

    static const MIL_NbcAgentType* Find( const std::string& strName );
    static const MIL_NbcAgentType* Find( uint nID );
    //@}

    //! @name Accessors
    //@{
    uint                   GetID                   () const;
    const std::string&     GetName                 () const;
    
    bool                   IsLiquidContaminating   () const;
    bool                   IsLiquidPoisonous       () const;
    const PHY_HumanWound&  GetLiquidRandomWound    () const; 
    
    bool                   IsGasContaminating      () const;
    bool                   IsGasPoisonous          () const;
    const PHY_HumanWound&  GetGasRandomWound       () const; 
    uint                   GetGasLifeTime          () const;
    MT_Float               GetGasPropagationAngle  () const;
    bool                   CanBeVaporized          () const;
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

    typedef std::vector< MT_Float >                T_HumanPoisonousVector;
    typedef T_HumanPoisonousVector::const_iterator CIT_HumanPoisonousVector;
    //@}

private:
     MIL_NbcAgentType( const std::string& strName, xml::xistream& xis );
    ~MIL_NbcAgentType();

    //! @name Tools
    //@{
    bool                  ReadPoisonousData( xml::xistream& xis, T_HumanPoisonousVector& data );
    const PHY_HumanWound& GetRandomWound   ( const T_HumanPoisonousVector& data ) const;
    //@}
    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadAgent( xml::xistream& xis );
    void SortEffectType  ( xml::xistream& xis );
    void ReadLiquid      ( xml::xistream& xis );
    void ReadGaz         ( xml::xistream& xis );
    void ReadEffect      ( xml::xistream& xis, T_HumanPoisonousVector& data );
    //@}

private:
    const std::string           strName_;
          uint                  nID_;
        
          T_HumanPoisonousVector liquidPoisonous_;
          bool                   bLiquidPoisonous_;
          bool                   bLiquidContaminating_;

          bool                   bCanBeVaporized_;
          T_HumanPoisonousVector gasPoisonous_;
          bool                   bGasPoisonous_;
          bool                   bGasContaminating_;
          uint                   nGasLifeTime_;
          MT_Float               rGasPropagationAngle_;

private:
    static T_NbcAgentTypeMap nbcAgentTypes_;
    static MT_Float          rCoefMaxSpeedModificator_;
    static MT_Float          rCoefReloadingTimeModificator_;
    static MT_Float          rMinPropagationSpeed_;
    static MT_Random         randomGenerator_;
};

#include "MIL_NbcAgentType.inl"


#endif // __MIL_NbcAgentType_h_