// *****************************************************************************
//
// $Created: NLD 2004-04-07 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_RapForLocal.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: DEC_Knowledge_RapForLocal.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_RapForLocal_h_
#define __DEC_Knowledge_RapForLocal_h_

#include "DEC_Knowledge_RapFor_ABC.h"
#include "DEC_Knowledge_Def.h"
#include <boost/serialization/export.hpp>

class MIL_Agent_ABC;

// =============================================================================
// Created: NLD 2004-04-07
// =============================================================================
class DEC_Knowledge_RapForLocal : public DEC_Knowledge_RapFor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_Knowledge_RapForLocal( const MIL_Agent_ABC& pion );
             DEC_Knowledge_RapForLocal();
    virtual ~DEC_Knowledge_RapForLocal();
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Update();
    void Clear();
    //@}

    //! @name Accessors
    //@{
    const T_ConstKnowledgeAgentVector& GetDangerousEnemies();
    //@}

private:
    //! @name Member data
    //@{
    const MIL_Agent_ABC* pPion_;
    T_ConstKnowledgeAgentVector dangerousEnemies_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_RapForLocal )

#endif // __DEC_Knowledge_RapForLocal_h_
