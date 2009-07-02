// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/MIL_Army.h $
// $Author: Jvt $
// $Modtime: 6/04/05 17:14 $
// $Revision: 9 $
// $Workfile: MIL_Army.h $
//
// *****************************************************************************

#ifndef __MIL_Army_ABC_h_
#define __MIL_Army_ABC_h_

#include "MIL.h" // only for E_Tristate !!!

class MIL_Object_ABC;
class DEC_Knowledge_Object;
class DEC_KnowledgeBlackBoard_Army;
class MIL_KnowledgeGroup;

// =============================================================================
// @class  MIL_Army
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Army_ABC
{  
public:
    //! @name Diplomacy Types
    //@{
    enum E_Diplomacy
    {
        eUnknown,
        eFriend,
        eEnemy,
        eNeutral
    };
    //@}

public:    
    //! @name Constructor/destructor
    //@{
    virtual ~MIL_Army_ABC() {}
    //@}

    //! @name CheckPoints / Interface
    //@{
    template< typename Archive > void serialize( Archive&, const uint ) {}
    //@}

    //! @name Hierarchy
    //@{
    virtual void RegisterObject( MIL_Object_ABC& object ) = 0;
    virtual void UnregisterObject( MIL_Object_ABC& object ) = 0;
    //@}

    //! @name Operations
    //@{    
    virtual bool       IsPerceived( const DEC_Knowledge_Object& knowledge ) const = 0;

    virtual E_Tristate IsAFriend( const MIL_Army_ABC& army ) const = 0;
    virtual E_Tristate IsAnEnemy( const MIL_Army_ABC& army ) const = 0;
    virtual E_Tristate IsNeutral( const MIL_Army_ABC& army ) const = 0;

    virtual MIL_KnowledgeGroup* FindKnowledgeGroup( uint nID ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual uint                          GetID() const = 0;
    virtual const std::string&            GetName() const = 0;
    virtual DEC_KnowledgeBlackBoard_Army& GetKnowledge() const = 0;
    //@}

    //! @name 
    //@{
    bool operator== ( const MIL_Army_ABC& rhs ) const
    {
        return GetID() == rhs.GetID();
    }

    bool operator!= ( const MIL_Army_ABC& rhs ) const
    {
        return GetID() != rhs.GetID();
    }
    //@}
};

#endif // __MIL_Army_ABC_h_
