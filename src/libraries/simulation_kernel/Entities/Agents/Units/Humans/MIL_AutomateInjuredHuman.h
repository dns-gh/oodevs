// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_AutomateInjuredHuman.h $
// $Author: Rft $
// $Modtime: 14/04/05 11:33 $
// $Revision: 12 $
// $Workfile: MIL_AutomateInjuredHuman.h $
//
// *****************************************************************************

#ifndef __MIL_AutomateInjuredHuman_h_
#define __MIL_AutomateInjuredHuman_h_

#include "MIL.h"
#include "Entities/Automates/MIL_Automate.h"
#include <list>

namespace xml
{
    class xostream;
    class xistream;
}

class MIL_AutomateTypeInjuredHuman;
class PHY_InjuredHuman;
class MIL_Injury_ABC;
class PHY_ComposantePion;

// =============================================================================
// @class  MIL_AutomateInjuredHuman
// @brief  Créer un pion de type InjuredHuman (est ce que ca doit deriver de MIL_AgentPionType? )
//         L'enregistrer quelque part
//         Les mettre à jour
// Created: RFT 2004-08-03
// =============================================================================
class MIL_AutomateInjuredHuman : public MIL_Automate
{
public:
             MIL_AutomateInjuredHuman(  const MIL_AutomateTypeInjuredHuman& type
                                        , unsigned int nID, MIL_Entity_ABC& parent
                                        , xml::xistream& xis
                                        , unsigned int gcPause
                                        , unsigned int gcMult );
             MIL_AutomateInjuredHuman( const MIL_AutomateType& type, unsigned int nID, MIL_Entity_ABC& parent, unsigned int knowledgeGroup, const std::string& name, unsigned int gcPause, unsigned int gcMult, unsigned int context );

    virtual ~MIL_AutomateInjuredHuman();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void UpdateNetwork() const;
    virtual void UpdateState  ();
    virtual void Clean        ();
    //@}

    //! @name Network
    //@{
    virtual void SendFullState() const;
    //@}

    //! @name Accessors
    //@{
    void HandleInjuredHuman( PHY_InjuredHuman* injuredHuman );
    //@}

private:
    //! @name Types
    //@{
    typedef std::list< PHY_InjuredHuman* >      T_InjuredHumanList;
    typedef T_InjuredHumanList::iterator        IT_InjuredHumanList;
    typedef T_InjuredHumanList::const_iterator  CIT_InjuredHumanList;
    //@}

private:
    //! @name Constructor
    //@{
    MIL_AutomateInjuredHuman( const MIL_AutomateTypeInjuredHuman& type, unsigned int nID = 0 );
    //@}

    //! @name Tools
    //@{
    MIL_AutomateInjuredHuman* GetAutomateInjuredHuman( unsigned int nID ) const;
    const MIL_AutomateType* RetrieveType() const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_AutomateInjuredHuman* automat, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_AutomateInjuredHuman* automat, const unsigned int /*version*/ );
    //@}

private:
    //! @name private members
    //@{
    T_InjuredHumanList pInjuredHumans_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_AutomateInjuredHuman )

#endif // __MIL_AutomateInjuredHuman_h_

