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

#ifndef __DEC_Knowledge_ObjectAttributeNBC_h_
#define __DEC_Knowledge_ObjectAttributeNBC_h_

#include "DEC_Knowledge_ObjectAttribute_ABC.h"
#include "game_asn/Simulation.h"

class NBCAttribute;
class MIL_NbcAgentType;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectAttributeNBC : public DEC_Knowledge_ObjectAttribute_ABC
{
    MT_COPYNOTALLOWED( DEC_Knowledge_ObjectAttributeNBC ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectAttributeNBC();
    explicit DEC_Knowledge_ObjectAttributeNBC( const NBCAttribute& attr );
    virtual ~DEC_Knowledge_ObjectAttributeNBC();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
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
    const NBCAttribute*     attr_;
    const MIL_NbcAgentType* pNbcAgentType_;
    //@}
};

#endif // __DEC_Knowledge_ObjectAttributeNBC_h_
