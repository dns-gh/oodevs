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

#ifndef __DEC_Knowledge_ObjectAttribute_ABC_h_
#define __DEC_Knowledge_ObjectAttribute_ABC_h_

class PHY_PerceptionLevel;
class DEC_Knowledge_Object;
class DEC_Knowledge_ObjectPerception;
class DEC_Knowledge_ObjectCollision;
class DEC_Knowledge_Object;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectAttribute_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectAttribute_ABC() {}
    virtual ~DEC_Knowledge_ObjectAttribute_ABC() {}
    //@}

    //! @name CheckPoints
    //@{
    virtual void UpdateOnPerceptionLevel( const PHY_PerceptionLevel& currentPerceptionLevel ) = 0;    
    virtual void UpdateOnPerception( const DEC_Knowledge_ObjectPerception& perception ) = 0;
    virtual void UpdateOnCollision( const DEC_Knowledge_ObjectCollision& collision  ) = 0;
    virtual void Send( ASN1T_ObjectAttributes& asn ) const = 0;
    //@}

    //! @name CheckPoint / boost deserialize
    //@{
    template< typename Archive > void serialize( Archive&, const uint ) {}
    virtual void Register( DEC_Knowledge_Object& knObject ) = 0;
    //@}
};

#endif // __DEC_Knowledge_ObjectAttribute_ABC_h_
