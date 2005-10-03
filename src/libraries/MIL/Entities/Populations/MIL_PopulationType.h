// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/MIL_PopulationType.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:14 $
// $Revision: 3 $
// $Workfile: MIL_PopulationType.h $
//
// *****************************************************************************

#ifndef __MIL_PopulationType_h_
#define __MIL_PopulationType_h_

#include "MIL.h"

class MIL_Population;
class DEC_ModelPopulation;

// =============================================================================
// @class  MIL_PopulationType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_PopulationType
{
    MT_COPYNOTALLOWED( MIL_PopulationType )

public:
    //! @name Manager
    //@{
    static void                      Initialize( MIL_InputArchive& archive );
    static void                      Terminate ();
    static const MIL_PopulationType* Find      ( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string&                         GetName                () const;
          uint                                 GetID                  () const;
          MT_Float                             GetConcentrationDensity() const;
          MT_Float                             GetDefaultFluxDensity  () const;
          MT_Float                             GetMaxSpeed            () const;
    const DEC_ModelPopulation&                 GetModel               () const;
          DIA_FunctionTable< MIL_Population >& GetFunctionTable       () const;
    //@}

    //! @name Operations
    //@{
    MIL_Population& InstanciatePopulation( uint nID, MIL_InputArchive& archive ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_PopulationType*, sCaseInsensitiveLess > T_PopulationMap;
    typedef T_PopulationMap::const_iterator                                          CIT_PopulationMap;
    //@}

private:
     MIL_PopulationType( const std::string& strName, MIL_InputArchive& archive );
    ~MIL_PopulationType();

    //! @name Tools
    //@{
    void InitializeDiaFunctions();
    //@}

private:
    const std::string                          strName_;
          uint                                 nID_;
          MT_Float                             rConcentrationDensity_;
          MT_Float                             rDefaultFluxDensity_;
          MT_Float                             rMaxSpeed_;
    const DEC_ModelPopulation*                 pModel_;
          DIA_FunctionTable< MIL_Population >* pDIAFunctionTable_;

private:
    static T_PopulationMap populations_;
};

#include "MIL_PopulationType.inl"

#endif // __MIL_PopulationType_h_
