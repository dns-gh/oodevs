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

#ifndef __DEC_Knowledge_ObjectAttributeBypass_h_
#define __DEC_Knowledge_ObjectAttributeBypass_h_

#include "DEC_Knowledge_ObjectAttribute_ABC.h"
#include <boost/serialization/export.hpp>

class BypassAttribute;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectAttributeBypass : public DEC_Knowledge_ObjectAttribute_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectAttributeBypass();
    explicit DEC_Knowledge_ObjectAttributeBypass( const BypassAttribute& attr );
    virtual ~DEC_Knowledge_ObjectAttributeBypass();
    //@}

    //! @name Update
    //@{
    virtual void UpdateOnPerceptionLevel( const PHY_PerceptionLevel& currentPerceptionLevel );
    virtual void UpdateOnPerception( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateOnCollision( const DEC_Knowledge_ObjectCollision& collision );
    virtual void Send( Common::MsgObjectAttributes& asnMsg ) const;
    //@}

    //! @name Operations
    //@{
    bool IsBypassed() const;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    void Register( DEC_Knowledge_Object& object );
    //@}

private:
    //! @name Update
    //@{
    void UpdateAttributes();
    //@}

private:
    //! @name Data Members
    //@{
    const BypassAttribute* attr_;
    unsigned int  nBypassPercentage_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeBypass )

#endif // __DEC_Knowledge_ObjectAttributeBypass_h_
