// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectFire.h $
// $Author: RFT $
// $Modtime: 27/04/05 15:10 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectFire.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectAttributeFire_h_
#define __DEC_Knowledge_ObjectAttributeFire_h_

#include "DEC_Knowledge_ObjectAttribute_ABC.h"
#include "game_asn/Simulation.h"

class FireAttribute;
class MIL_FireClass;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectAttributeFire : public DEC_Knowledge_ObjectAttribute_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectAttributeFire();
    explicit DEC_Knowledge_ObjectAttributeFire( const FireAttribute& attr );
    virtual ~DEC_Knowledge_ObjectAttributeFire();
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
    const FireAttribute* attr_;
    const MIL_FireClass* pFireClass_;
          int            heat_;
    //@}
};

#endif // __DEC_Knowledge_ObjectAttributeFire_h_
