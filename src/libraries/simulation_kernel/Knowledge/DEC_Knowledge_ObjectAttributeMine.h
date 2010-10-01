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

#ifndef __DEC_Knowledge_ObjectAttributeMine_h_
#define __DEC_Knowledge_ObjectAttributeMine_h_

#include "DEC_Knowledge_ObjectAttribute_ABC.h"
#include "MT_Tools/MT_Tools_Types.h"
#include <boost/serialization/export.hpp>

class MineAttribute;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectAttributeMine : public DEC_Knowledge_ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectAttributeMine();
    explicit DEC_Knowledge_ObjectAttributeMine( const MineAttribute& attr );
    virtual ~DEC_Knowledge_ObjectAttributeMine();
    //@}

    //! @name Update
    //@{
    virtual void UpdateOnPerceptionLevel( const PHY_PerceptionLevel& currentPerceptionLevel );
    virtual void UpdateOnPerception( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateOnCollision( const DEC_Knowledge_ObjectCollision& collision );
    virtual void Send( Common::ObjectAttributes& asnMsg ) const;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    void Register( DEC_Knowledge_Object& knObject );
    //@}

private:
    //! @name Update
    //@{
    void UpdateAttributes();
    //@}

private:
    //! @name Data members
    //@{
    const MineAttribute* attr_;
    unsigned int nNbrDotationForMining_;
    unsigned int nMinesActivityTime_;
    MT_Float rMiningPercentage_;
    float rMinesDensity_;
    unsigned int nDotationType_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeMine )

#endif // __DEC_Knowledge_ObjectAttributeMine_h_
