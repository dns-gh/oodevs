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
#include "Entities/MIL_EntityType.h"

namespace xml
{
    class xistream;
}

namespace sword
{
    class Brain;
    class DEC_Logger;
}

class DEC_Model_ABC;
class MIL_AgentType_ABC;
class MIL_Automate;
class MIL_DictionaryExtensions;
class MIL_Entity_ABC;
class MissionController_ABC;

// =============================================================================
// @class  MIL_AutomateType
// Created: JVT 2004-08-03
// ============================================================================
class MIL_AutomateType : public MIL_EntityType
{
public:
             MIL_AutomateType( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AutomateType();

    //! @name Types
    //@{
    struct sCompositionBounds
    {
        sCompositionBounds()
            : nMin_( 0 )
            , nMax_( 0 )
        {}
        unsigned int nMin_;
        unsigned int nMax_;
    };
    typedef std::vector< std::pair< const MIL_AgentType_ABC*, sCompositionBounds > > T_CompositionMap;
    //@}

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
    virtual MIL_Automate& InstanciateAutomate( unsigned int nID, MIL_Entity_ABC& parent, MissionController_ABC& controller, xml::xistream& xis, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger  ) const;
    virtual MIL_Automate& InstanciateAutomate( unsigned int nID, MIL_Entity_ABC& parent, MissionController_ABC& controller, unsigned int knowledgeGroup, const std::string& name, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger, unsigned int context, const MIL_DictionaryExtensions& extensions ) const;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int                     GetID                            () const;
    const MIL_AgentType_ABC&                 GetTypePionPC                    () const;
    virtual const DEC_Model_ABC&             GetModel                         () const;
    virtual std::string                      GetArchetypeName                 () const;
    virtual const std::string&               GetName                          () const;
    // Pc is the first element
    const T_CompositionMap&                  GetComposition                   () const;
    //@}

    //! @name Operations
    //@{
    bool CheckComposition( const MIL_Automate& automate ) const;
    //@}

    //! @name Logistic
    //@{
    virtual bool IsLogistic() const;
    virtual bool IsLogisticBase() const;
    virtual bool IsRefugee () const;
    virtual bool IsInjuredHuman() const;
    //@}

    //! @name Logistic
    //@{
    virtual void RegisterFunctions( sword::Brain& brain, MIL_Automate& automat ) const;
    //@}

protected:
    explicit MIL_AutomateType( const DEC_Model_ABC& model );

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_AutomateType* > T_AutomateTypeMap;

    typedef const MIL_AutomateType* (*T_AutomateTypeAllocator)( const std::string& strName, xml::xistream& xis );
    typedef std::map< std::string, T_AutomateTypeAllocator > T_AutomateTypeAllocatorMap;
    //@}

private:
    //! @name Init
    //@{
    void InitializeModel( xml::xistream& xis );
    //@}

    //! @name Helpers
    //@{
    static void ReadAutomat( xml::xistream& xis );
    void ReadUnit          ( xml::xistream& xis );
    bool FindUnitType      ( const MIL_AgentType_ABC& type ) const;
    //@}

private:
          unsigned int                nID_;
    const std::string         strName_;
    const DEC_Model_ABC*      pModel_;
          T_CompositionMap    composition_;
    const MIL_AgentType_ABC*  pTypePC_;

private:
    static T_AutomateTypeAllocatorMap  automateTypeAllocators_;
    static T_AutomateTypeMap           automateTypes_;
};

#endif // __MIL_AutomateType_h_
