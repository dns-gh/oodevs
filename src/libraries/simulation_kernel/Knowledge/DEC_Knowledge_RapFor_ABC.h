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
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_RapFor_ABC();
    virtual ~DEC_Knowledge_RapFor_ABC();
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Accessors
    //@{
    double GetValue();
    //@}

    //! @name Tools
    //@{
    static void Initialize( xml::xistream& xis, unsigned int tickDuration );
    //@}

protected:
    //! @name Tools
    //@{
    static double ComputeRapForIncreasePerTimeStepValue( double rBaseTimeValue );
    void ApplyValue( double rTotalFightScoreFriend, double rTotalFightScoreEnemy, const double rRapForIncreasePerTimeStepValue );
    virtual void Update() = 0;
    //@}

protected:
    //! @name Member data
    //@{
    unsigned int nLastCacheUpdateTick_;
    //@}

private:
    //! @name Member data
    //@{
    double rRapForValue_;
    static const double rRapForBoundMin_;
    static const double rRapForBoundMax_;
    static double rRapForTimeStepDefaultValue_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapFor_ABC::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template < typename Archive >
void DEC_Knowledge_RapFor_ABC::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    file & nLastCacheUpdateTick_;
    file & rRapForValue_;
}

#endif // __DEC_Knowledge_RapFor_ABC_h_
