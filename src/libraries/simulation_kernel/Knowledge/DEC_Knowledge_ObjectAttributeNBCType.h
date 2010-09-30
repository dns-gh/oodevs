// *****************************************************************************
//
// $Created: RFT 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectBouchonMines.h $
// $Author: RFT $
// $Modtime: 27/04/05 15:10 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectBouchonMines.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectAttributeNBCType_h_
#define __DEC_Knowledge_ObjectAttributeNBCType_h_

#include "DEC_Knowledge_ObjectAttribute_ABC.h"
#include "MT_Tools/MT_Tools_Types.h"
#include <boost/serialization/export.hpp>
#include <boost/serialization/split_member.hpp>

class NBCTypeAttribute;
class MIL_NBCType;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
// Created: RFT 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectAttributeNBCType : public DEC_Knowledge_ObjectAttribute_ABC
{
    MT_COPYNOTALLOWED( DEC_Knowledge_ObjectAttributeNBCType )

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectAttributeNBCType();
    explicit DEC_Knowledge_ObjectAttributeNBCType( const NBCTypeAttribute& attr );
    virtual ~DEC_Knowledge_ObjectAttributeNBCType();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Register / serialization
    //@{
    void Register( DEC_Knowledge_Object& object );
    //@}

    //! @name Update
    //@{
    virtual void UpdateOnPerceptionLevel( const PHY_PerceptionLevel& currentPerceptionLevel );
    virtual void UpdateOnPerception( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateOnCollision( const DEC_Knowledge_ObjectCollision& collision );
    virtual void Send( Common::ObjectAttributes& message ) const;
    //@}

private:
    //! @name Update
    //@{
    void UpdateAttributes();
    //@}

private:
    //! @name Data Members
    //@{
    const NBCTypeAttribute* attr_;
    const MIL_NBCType* pAgent_;
    int concentration_;
    int sourceLifeDuration_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeNBCType )

#endif // __DEC_Knowledge_ObjectAttributeNBCType_h_
