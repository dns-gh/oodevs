// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_Composante_ABC.h $
// $Author: Jvt $
// $Modtime: 1/04/05 10:17 $
// $Revision: 4 $
// $Workfile: PHY_Composante_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_Composante_ABC_h_
#define __PHY_Composante_ABC_h_

class PHY_ComposanteType_ABC;

// =============================================================================
// @class  PHY_Composante_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Composante_ABC : private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef std::vector< PHY_Composante_ABC* >  T_ComposanteVector;
    typedef T_ComposanteVector::iterator        IT_ComposanteVector;
    typedef T_ComposanteVector::const_iterator  CIT_ComposanteVector;

 
    struct T_ComposanteUse
    {
        uint nNbrAvailable_;
        uint nNbrUsed_;
        uint nNbrTotal_; // nNbrTotal_ >= nNbrAvailable_ >= nNbrUsed_
        uint nNbrLent_;
    };

    typedef std::map< const PHY_ComposanteType_ABC*, T_ComposanteUse > T_ComposanteUseMap;
    typedef T_ComposanteUseMap::const_iterator                         CIT_ComposanteUseMap;
    //@}

             PHY_Composante_ABC() {}
    virtual ~PHY_Composante_ABC() {}

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint ) {}
    //@}
    
    //! @name Accessors
    //@{
    virtual const PHY_ComposanteType_ABC& GetType            () const = 0;
    virtual bool                          CanFire            () const = 0;
    virtual bool                          CanFireWhenUnloaded() const = 0;
    virtual uint                          GetMajorScore      () const = 0;
    //@}
};

#endif // __PHY_Composante_ABC_h_
