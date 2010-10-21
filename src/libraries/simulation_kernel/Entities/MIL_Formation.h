// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Formation_h_
#define __MIL_Formation_h_

#include "MIL.h"
#include "MIL_Entity_ABC.h"
#include "tools/Resolver.h"
#include "Entities/MIL_VisitableEntity_ABC.h"
namespace xml
{
    class xostream;
    class xistream;
}

namespace MsgsClientToSim
{
    class MsgUnitMagicAction;
}

class MIL_Army_ABC;
class PHY_NatureLevel;
class FormationFactory_ABC;
class AutomateFactory_ABC;
class MIL_Automate;
class MIL_AutomateLOG;
class MIL_AgentPion;
template < typename T > class PHY_ActionLogistic;

// =============================================================================
/** @class  MIL_Formation
    @brief  MIL_Formation
*/
// Created: NLD 2006-10-11
// =============================================================================
class MIL_Formation : public MIL_Entity_ABC
                    , public tools::Resolver< MIL_Formation >
                    , public tools::Resolver< MIL_Automate >
                    , public MIL_VisitableEntity_ABC< MIL_AgentPion >
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Formation( xml::xistream& xis, MIL_Army_ABC& army, MIL_Formation* pParent, FormationFactory_ABC& formationFactory, AutomateFactory_ABC& automateFactory );
             MIL_Formation( const std::string& name );
             MIL_Formation( int level, const std::string& name, std::string logLevelStr, MIL_Army_ABC& army, MIL_Formation* parent );
    virtual ~MIL_Formation();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void WriteODB( xml::xostream& xos ) const;
    void WriteLogisticLinksODB( xml::xostream& xos ) const;
    //@}

    //! @name Hierarchy
    //@{
    void RegisterAutomate( MIL_Automate& automate );
    void UnregisterAutomate( MIL_Automate& automate );
    void RegisterFormation( MIL_Formation& formation );
    void UnregisterFormation( MIL_Formation& formation );
    //@}

    //! @name Accessors
    //@{
    virtual MIL_Army_ABC& GetArmy() const;
    unsigned int GetID() const;
    MIL_AutomateLOG* GetBrainLogistic() const;
    MIL_AutomateLOG* FindLogisticManager() const;
    //@}

    //! @name Visitor
    //@{
    virtual void Apply( MIL_EntityVisitor_ABC< MIL_AgentPion >& visitor ) const;
    //@}

    //! @name Network
    //@{
    void SendCreation() const;
    void SendFullState() const;
    void SendLogisticLinks() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_Formation( const MIL_Formation& );            //!< Copy constructor
    MIL_Formation& operator=( const MIL_Formation& ); //!< Assignment operator
    //@}

    //! @name Tools
    //@{
    void InitializeFormation( xml::xistream& xis, FormationFactory_ABC& formationFactory );
    void InitializeAutomate( xml::xistream& xis, AutomateFactory_ABC& automateFactory );
    void ReadExtension( xml::xistream& xis );
    //@}
    
    //! @name Serialization
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_Formation* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_Formation* role, const unsigned int /*version*/ );
    //@}


private:
    //! @name Attributes
    //@{
    unsigned int nID_;
    MIL_Army_ABC* pArmy_;
    MIL_Formation* pParent_;
    const PHY_NatureLevel* pLevel_;
    std::auto_ptr<MIL_AutomateLOG> pBrainLogistic_;
    boost::shared_ptr< PHY_ActionLogistic< MIL_AutomateLOG > > pLogisticAction_;
    std::map< std::string, std::string > extensions_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_Formation )

#endif // __MIL_Formation_h_
