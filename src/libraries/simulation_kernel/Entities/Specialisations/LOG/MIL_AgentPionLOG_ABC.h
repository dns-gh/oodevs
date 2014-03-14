// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_AgentPionLOG_ABC.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:32 $
// $Revision: 4 $
// $Workfile: MIL_AgentPionLOG_ABC.h $
//
// *****************************************************************************

#ifndef __MIL_AgentPionLOG_ABC_h_
#define __MIL_AgentPionLOG_ABC_h_

#include "Entities/Agents/MIL_AgentPion.h"

namespace xml
{
    class xistream;
}

template < typename T > class PHY_ActionLogistic;

class MIL_AutomateLOG;

// =============================================================================
// @class  MIL_AgentPionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentPionLOG_ABC : public MIL_AgentPion
{
public:
             MIL_AgentPionLOG_ABC( const MIL_AgentTypePion& type, MissionController_ABC& controller, MIL_Automate& automate, xml::xistream& xis );
             MIL_AgentPionLOG_ABC( const MIL_AgentTypePion& type, MissionController_ABC& controller );
    virtual ~MIL_AgentPionLOG_ABC();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template < typename Archive > void load( Archive&, const unsigned int );
    template < typename Archive > void save( Archive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    MIL_AutomateLOG* FindLogisticManager() const; // Return the brain logistic whom this unit is subordinated to
    void UpdateLogistic();
    virtual void CancelAllActions();
    //@}

private:
    boost::shared_ptr< PHY_ActionLogistic< MIL_AgentPionLOG_ABC > > pLogisticAction_;
};

// BOOST_SERIALIZATION_ASSUME_ABSTRACT should be used for this
// but it seems to be buggy : inherits boost::is_abstract<T> instead of boost::true_type
namespace boost {
namespace serialization {
    template<>
    struct is_abstract<const MIL_AgentPionLOG_ABC> : boost::true_type { } ;
} // namespace serialization
}

#endif // __MIL_AgentPionLOG_ABC_h_
