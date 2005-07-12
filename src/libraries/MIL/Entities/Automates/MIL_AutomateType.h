// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/MIL_AutomateType.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:22 $
// $Revision: 8 $
// $Workfile: MIL_AutomateType.h $
//
// *****************************************************************************

#ifndef __MIL_AutomateType_h_
#define __MIL_AutomateType_h_

#include "MIL.h"

class MIL_AgentTypePion;
class MIL_Automate;
class DEC_ModelAutomate;

// =============================================================================
// @class  MIL_AutomateType
// Created: JVT 2004-08-03
// ============================================================================
class MIL_AutomateType
{
    MT_COPYNOTALLOWED( MIL_AutomateType )

public:
    MIL_AutomateType( const std::string& strName, MIL_InputArchive& archive );
    virtual ~MIL_AutomateType();

    //! @name Manager
    //@{
    static void Initialize( MIL_InputArchive& archive );
    static void Terminate ();

    static const MIL_AutomateType* FindAutomateType( const std::string& strName );
    static const MIL_AutomateType* FindAutomateType( uint nID );
    static const MIL_AutomateType* Create          ( const std::string& strName, MIL_InputArchive& archive );
    //@}

    //! @name Instanciation
    //@{
    virtual MIL_Automate& InstanciateAutomate( uint nID, MIL_InputArchive& archive ) const;
    //@}

    //! @name Accessors
    //@{
          uint                               GetID                            () const;
    const MIL_AgentTypePion&                 GetTypePionPC                    () const;
    const DEC_ModelAutomate&                 GetModel                         () const;
    const std::string&                       GetName                          () const;
          DIA_FunctionTable< MIL_Automate >& GetFunctionTable                 () const;
          MT_Float                           GetRapForIncreasePerTimeStepValue() const;
    //@}

    //! @name Operations
    //@{
    bool CheckComposition( const MIL_Automate& automate ) const;
    //@}

    //! @name Logistic
    //@{
    virtual bool IsLogistic() const;
    virtual bool IsRefugee () const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_AutomateType*, sCaseInsensitiveLess > T_AutomateTypeMap;
    typedef T_AutomateTypeMap::const_iterator                                      CIT_AutomateTypeMap;

    typedef const MIL_AutomateType* (*T_AutomateTypeAllocator)( const std::string& strName, MIL_InputArchive& archive );

    typedef std::map< std::string, T_AutomateTypeAllocator, sCaseInsensitiveLess > T_AutomateTypeAllocatorMap;
    typedef T_AutomateTypeAllocatorMap::const_iterator                             CIT_AutomateTypeAllocatorMap;

    struct sCompositionBounds
    {
        sCompositionBounds() : nMin_( 0 ), nMax_( 0 ) {}
    
        uint nMin_;
        uint nMax_;
    };
    typedef std::map< const MIL_AgentTypePion*, sCompositionBounds > T_CompositionMap;
    typedef T_CompositionMap::const_iterator                         CIT_CompositionMap;
    //@}

private:
    //! @name Init
    //@{
    void InitializeComposition ( MIL_InputArchive& archive );
    void InitializeRapFor      ( MIL_InputArchive& archive );
    void InitializeModel       ( MIL_InputArchive& archive );
    void InitializeDiaFunctions();
    //@}

private:
    //! @name Tools
    //@{
    static bool ParseComposition( std::string value, sCompositionBounds& );
    //@}

private:
          uint                nID_;
    const std::string         strName_;
    const DEC_ModelAutomate*  pModel_;
          T_CompositionMap    composition_;
    const MIL_AgentTypePion*  pTypePC_;
          MT_Float            rRapForIncreasePerTimeStepValue_;

    DIA_FunctionTable< MIL_Automate >* pDIAFunctionTable_;

private:
    static T_AutomateTypeAllocatorMap  automateTypeAllocators_;
    static T_AutomateTypeMap           automateTypes_;
};

#include "MIL_AutomateType.inl"

#endif // __MIL_AutomateType_h_
