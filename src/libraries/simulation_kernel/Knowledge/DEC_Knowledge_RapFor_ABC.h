// *****************************************************************************
//
// $Created: NLD 2004-04-07 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_RapFor_ABC.h $
// $Author: Jvt $
// $Modtime: 25/03/05 13:18 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_RapFor_ABC.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_RapFor_ABC_h_
#define __DEC_Knowledge_RapFor_ABC_h_

#include "MIL.h"

#include "DEC_Knowledge_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// Created: NLD 2004-04-07
// =============================================================================
class DEC_Knowledge_RapFor_ABC : public DEC_Knowledge_ABC
{
    MT_COPYNOTALLOWED( DEC_Knowledge_RapFor_ABC );

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_RapFor_ABC();
    virtual ~DEC_Knowledge_RapFor_ABC();
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Accessors
    //@{
    MT_Float GetValue();
    //@}

    //! @name Tools
    //@{
    static void     Initialize                              ( xml::xistream& xis );
    static MT_Float ComputeRapForIncreasePerTimeStepValue   ( MT_Float rBaseTimeValue );    
    static MT_Float GetRapForIncreasePerTimeStepDefaultValue();
    //@}

protected:
    //! @name Tools
    //@{
            void ApplyValue( MT_Float rTotalFightScoreFriend, MT_Float rTotalFightScoreEnemy, const MT_Float rRapForIncreasePerTimeStepValue );
            bool NeedUpdate() const;
    virtual void Update    () = 0;
    //@}

protected:
    MT_Float rRapForValue_;
    uint     nLastCacheUpdateTick_;

private:
    static const MT_Float rRapForBoundMin_;
    static const MT_Float rRapForBoundMax_;
    static       MT_Float rRapForIncreasePerTimeStepDefaultValue_;
};

#include "DEC_Knowledge_RapFor_ABC.inl"

#endif // __DEC_Knowledge_RapFor_ABC_h_
