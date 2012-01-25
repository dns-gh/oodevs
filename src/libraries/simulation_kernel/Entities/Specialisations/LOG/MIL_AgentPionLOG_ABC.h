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

#include "MIL_AutomateLOG.h"
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
             MIL_AgentPionLOG_ABC( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis );
             MIL_AgentPionLOG_ABC( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, const std::string& name );
             MIL_AgentPionLOG_ABC( const MIL_AgentTypePion& type, const AlgorithmsFactories& algorithmFactories );
    virtual ~MIL_AgentPionLOG_ABC();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    MIL_AutomateLOG* FindLogisticManager() const; // Return the brain logistic whom this unit is subordinated to
    void UpdateLogistic(); // Called by the 'PHY_ActionLogistic' action
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

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
inline void MIL_AgentPionLOG_ABC::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< MIL_AgentPion >( *this );
}

#endif // __MIL_AgentPionLOG_ABC_h_
