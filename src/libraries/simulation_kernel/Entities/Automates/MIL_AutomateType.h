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

namespace xml
{
    class xistream;
}

namespace directia
{
    class Brain;
}

class DEC_DataBase;
class DEC_Model_ABC;
class MIL_AgentTypePion;
class MIL_Automate;
class MIL_Formation;


// =============================================================================
// @class  MIL_AutomateType
// Created: JVT 2004-08-03
// ============================================================================
class MIL_AutomateType : private boost::noncopyable
{

public:
    MIL_AutomateType( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AutomateType();

    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();

    static const MIL_AutomateType* FindAutomateType( const std::string& strName );
    static const MIL_AutomateType* FindAutomateType( uint nID );
    static const MIL_AutomateType* Create          ( const std::string& strName, xml::xistream& xis );
    //@}

    //! @name Instanciation
    //@{
    virtual MIL_Automate& InstanciateAutomate( uint nID, MIL_Formation& parent, xml::xistream& xis, DEC_DataBase& database ) const;
    virtual MIL_Automate& InstanciateAutomate( uint nID, MIL_Automate&  parent, xml::xistream& xis, DEC_DataBase& database ) const;
    //@}

    //! @name Accessors
    //@{
          uint                               GetID                            () const;
    const MIL_AgentTypePion&                 GetTypePionPC                    () const;
    const DEC_Model_ABC&                     GetModel                         () const;
    const std::string&                       GetName                          () const;
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
    
    //! @name Logistic
    //@{
    virtual void RegisterFunctions( directia::Brain& brain, MIL_Automate& automat ) const;
    //@}

protected:
    explicit MIL_AutomateType( const DEC_Model_ABC& model );

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_AutomateType*, sCaseInsensitiveLess > T_AutomateTypeMap;
    typedef T_AutomateTypeMap::const_iterator                                      CIT_AutomateTypeMap;

    typedef const MIL_AutomateType* (*T_AutomateTypeAllocator)( const std::string& strName, xml::xistream& xis );

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
    void InitializeRapFor      ( xml::xistream& xis );
    void InitializeModel       ( xml::xistream& xis );
    void InitializeDiaFunctions();
    //@}
    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadAutomat( xml::xistream& xis );
    void ReadUnit          ( xml::xistream& xis );
    //@}

private:
          uint                nID_;
    const std::string         strName_;
    const DEC_Model_ABC*      pModel_;
          T_CompositionMap    composition_;
    const MIL_AgentTypePion*  pTypePC_;
          MT_Float            rRapForIncreasePerTimeStepValue_;

private:
    static T_AutomateTypeAllocatorMap  automateTypeAllocators_;
    static T_AutomateTypeMap           automateTypes_;
};

#endif // __MIL_AutomateType_h_
