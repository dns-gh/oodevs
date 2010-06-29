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

#ifndef __DEC_Knowledge_ObjectAttributeConstruction_h_
#define __DEC_Knowledge_ObjectAttributeConstruction_h_

#include "DEC_Knowledge_ObjectAttribute_ABC.h"
#include <boost/serialization/export.hpp>

class ConstructionAttribute;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectAttributeConstruction : public DEC_Knowledge_ObjectAttribute_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectAttributeConstruction();
    explicit DEC_Knowledge_ObjectAttributeConstruction( const ConstructionAttribute& attr );
    virtual ~DEC_Knowledge_ObjectAttributeConstruction();
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
    bool IsConstructed() const;
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
    const ConstructionAttribute* attr_;
    MT_Float    rConstructionPercentage_;
    unsigned int        nNbrDotation_;
    unsigned int        nDotationType_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeConstruction )

#endif // __DEC_Knowledge_ObjectAttributeConstruction_h_
