// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectBouchonMines.h $
// $Author: Nld $
// $Modtime: 27/04/05 15:10 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectBouchonMines.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectAttributeInputToxicCloud_h_
#define __DEC_Knowledge_ObjectAttributeInputToxicCloud_h_

#include "DEC_Knowledge_ObjectAttribute_ABC.h"
#include "game_asn/Simulation.h"
#include "simulation_terrain/TER_Polygon.h"

class InputToxicCloudAttribute;
class MIL_NbcAgentType;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectAttributeInputToxicCloud : public DEC_Knowledge_ObjectAttribute_ABC
{
    MT_COPYNOTALLOWED( DEC_Knowledge_ObjectAttributeInputToxicCloud ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectAttributeInputToxicCloud();
    explicit DEC_Knowledge_ObjectAttributeInputToxicCloud( const InputToxicCloudAttribute& attr );
    virtual ~DEC_Knowledge_ObjectAttributeInputToxicCloud();
    //@}

    //! @name CheckPoints
    //@{    
    template< typename Archive > void serialize( Archive&, const uint );
    void Register( DEC_Knowledge_Object& knObject );
    //@}

    //! @name Update
    //@{
    virtual void UpdateOnPerceptionLevel( const PHY_PerceptionLevel& currentPerceptionLevel );    
    virtual void UpdateOnPerception( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateOnCollision( const DEC_Knowledge_ObjectCollision& collision );
    virtual void Send( ASN1T_ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Update
    //@{
    void UpdateAttributes();
    //@}

private:
    //! @name Data Members
    //@{
    const InputToxicCloudAttribute*     attr_;
	TER_Polygon polygon_;
    //const MIL_NbcAgentType* pNbcAgentType_;
    //@}
};

#endif // __DEC_Knowledge_ObjectAttributeInputToxicCloud_h_
