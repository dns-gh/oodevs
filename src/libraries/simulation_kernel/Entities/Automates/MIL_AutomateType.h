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

#include "Tools/MIL_Config.h"
#include "MT_Tools/MT_String.h"
#include "MIL.h"

namespace xml
{
    class xistream;
}

namespace directia
{
    namespace brain
    {
        class Brain;
    }
}

class DEC_Model_ABC;
class MIL_AgentType_ABC;
class MIL_Automate;
class MIL_Entity_ABC;

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
    static const MIL_AutomateType* FindAutomateType( unsigned int nID );
    static const MIL_AutomateType* Create          ( const std::string& strName, xml::xistream& xis );
    //@}

    //! @name Instanciation
    //@{
    virtual MIL_Automate& InstanciateAutomate( unsigned int nID, MIL_Entity_ABC& parent, xml::xistream& xis, unsigned int gcPause, unsigned int gcMult ) const;
    virtual MIL_Automate& InstanciateAutomate( unsigned int nID, MIL_Entity_ABC& parent, unsigned int knowledgeGroup, const std::string& name, unsigned int gcPause, unsigned int gcMult, unsigned int context ) const;
    //@}

    //! @name Accessors
    //@{
          unsigned int                       GetID                            () const;
    const MIL_AgentType_ABC&                 GetTypePionPC                    () const;
    const DEC_Model_ABC&                     GetModel                         () const;
    const std::string&                       GetName                          () const;
          double                           GetRapForIncreasePerTimeStepValue() const;
    //@}

    //! @name Operations
    //@{
    bool CheckComposition( const MIL_Automate& automate ) const;
    //@}

    //! @name Logistic
    //@{
    virtual bool IsLogistic() const;
    virtual bool IsRefugee () const;
    virtual bool IsInjuredHuman() const;
    //@}

    //! @name Logistic
    //@{
    virtual void RegisterFunctions( directia::brain::Brain& brain, MIL_Automate& automat ) const;
    //@}

protected:
    explicit MIL_AutomateType( const DEC_Model_ABC& model );

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_AutomateType* > T_AutomateTypeMap;
    typedef T_AutomateTypeMap::const_iterator              CIT_AutomateTypeMap;

    typedef const MIL_AutomateType* (*T_AutomateTypeAllocator)( const std::string& strName, xml::xistream& xis );

    typedef std::map< std::string, T_AutomateTypeAllocator > T_AutomateTypeAllocatorMap;
    typedef T_AutomateTypeAllocatorMap::const_iterator     CIT_AutomateTypeAllocatorMap;

    struct sCompositionBounds
    {
        sCompositionBounds() : nMin_( 0 ), nMax_( 0 ) {}

        unsigned int nMin_;
        unsigned int nMax_;
    };
    typedef std::map< const MIL_AgentType_ABC*, sCompositionBounds > T_CompositionMap;
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
          unsigned int                nID_;
    const std::string         strName_;
    const DEC_Model_ABC*      pModel_;
          T_CompositionMap    composition_;
    const MIL_AgentType_ABC*  pTypePC_;
          double            rRapForIncreasePerTimeStepValue_;

private:
    static T_AutomateTypeAllocatorMap  automateTypeAllocators_;
    static T_AutomateTypeMap           automateTypes_;
};

#endif // __MIL_AutomateType_h_
