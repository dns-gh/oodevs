// *****************************************************************************
//
// $Created: NLD 2004-04-01 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentComposante.h $
// $Author: Nld $
// $Modtime: 18/04/05 16:38 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_AgentComposante.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_AgentComposante_h_
#define __DEC_Knowledge_AgentComposante_h_

#include "MIL.h"

class PHY_ComposantePion;
class PHY_ComposanteType_ABC;
class PHY_MaterialCompositionType;
class MIL_Agent_ABC;

// =============================================================================
/** @class  DEC_Knowledge_AgentComposante
    @brief  This class manages the composante knowledges for an agent
            It is owned by DEC_Knowledge_Agent
*/
// Created: NLD 2004-04-01
// =============================================================================
class DEC_Knowledge_AgentComposante
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_AgentComposante();
    explicit DEC_Knowledge_AgentComposante( const PHY_ComposantePion& composante );
    virtual ~DEC_Knowledge_AgentComposante();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void WriteKnowledges( xml::xostream& xos, unsigned int number ) const;
    //@}

    //! @name Accessors
    //@{
    const PHY_ComposanteType_ABC& GetType() const;
    unsigned int GetMajorScore() const;
    bool IsMajor() const;
    double GetMaxRange() const;
    //@}

    //! @name Operations
    //@{
    double GetDangerosity( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& compTarget, double rDistBtwSourceAndTarget, bool bUseAmmo, const PHY_MaterialCompositionType* material ) const;
    double GetMaxRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposantePion& compTarget, double rWantedPH ) const;
    //@}

private:
    //! @name Member data
    //@{
    const PHY_ComposanteType_ABC* pType_;
    bool bCanFire_;
    bool bMajor_;
    unsigned int nMajorScore_;
    double maxRange_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_AgentComposante )

#endif // __DEC_Knowledge_AgentComposante_h_

