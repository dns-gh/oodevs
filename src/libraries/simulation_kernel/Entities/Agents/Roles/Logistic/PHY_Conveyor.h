// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_Conveyor.h $
// $Author: Nld $
// $Modtime: 19/07/05 18:14 $
// $Revision: 1 $
// $Workfile: PHY_Conveyor.h $
//
// *****************************************************************************

#ifndef __PHY_Conveyor_h_
#define __PHY_Conveyor_h_

#include <boost/serialization/export.hpp>

class PHY_ComposantePion;
class MIL_AgentPion;
class PHY_DotationCategory;
class PHY_SupplyConsign_ABC;

// =============================================================================
// @class  PHY_Conveyor
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Conveyor : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
     PHY_Conveyor();
     PHY_Conveyor( PHY_ComposantePion& conveyorComp, MIL_AgentPion& conveyorPion );
    virtual ~PHY_Conveyor();
    //@}

    //! @name Dotations
    //@{
    MT_Float Convoy                 ( PHY_SupplyConsign_ABC& consign, const PHY_DotationCategory& dotationCategory, const MT_Float rNbrToConvoy ); // Return the nb convoyed
    bool     IsFull                 () const;
    void     NotifyConveyorDestroyed( PHY_SupplyConsign_ABC& consign );
    void     EmptyOut               ();
    //@}

    //! @name Lend
    //@{
    void LendTo  ( MIL_AgentPion& pion );
    void UndoLend();
    //@}

    //! @name Operations
    //@{
    unsigned int ApproximateTravelTime( const MT_Vector2D& vStartPos, const MT_Vector2D& vEndPos );
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //!/cygdrive/ @name Debug
    //@{
    void Dump() const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, MT_Float /*rNbr*/ > T_DotationMap;
    typedef T_DotationMap::iterator                                    IT_DotationMap;
    typedef T_DotationMap::const_iterator                              CIT_DotationMap;
    //@}

private:
    //! @name Member data
    //@{
    PHY_ComposantePion*  pConveyorComp_;
    MIL_AgentPion*       pConveyorPion_;
    MT_Float             rWeightCapacity_;
    MT_Float             rVolumeCapacity_;
    T_DotationMap        dotationsConvoyed_;

    MIL_AgentPion*       pLentTo_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_Conveyor )

#endif // __PHY_Conveyor_h_
