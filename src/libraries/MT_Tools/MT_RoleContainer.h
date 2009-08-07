// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_RoleContainer.h $
// $Author: Jvt $
// $Modtime: 5/04/05 11:36 $
// $Revision: 3 $
// $Workfile: MT_RoleContainer.h $
//
// *****************************************************************************

#ifndef __MT_RoleContainer_h_
#define __MT_RoleContainer_h_

#include "MT_Tools_Types.h"
#include <boost/shared_ptr.hpp>

class MT_Role_ABC;

// =============================================================================
// @class  MT_RoleContainer
// Created: JVT/NLD 2004-08-03
// =============================================================================
class MT_RoleContainer
{
public:
             MT_RoleContainer();
    virtual ~MT_RoleContainer();
    
    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Role registration
    //@{
    template< typename Role >                                        typename Role::RoleInterface& RegisterRole();
    template< typename Role, typename P1 >                           typename Role::RoleInterface& RegisterRole( P1& );
    template< typename Role, typename P1, typename P2  >             typename Role::RoleInterface& RegisterRole( P1&, P2& );
    template< typename Role, typename P1, typename P2, typename P3 > typename Role::RoleInterface& RegisterRole( P1&, P2&, P3& );
    //@}

    //! @name Role accessors
    //@{
    template< typename Role >       typename Role& GetRole();
    template< typename Role > const typename Role& GetRole() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< MT_Role_ABC > > T_RoleVector;
    //@}

protected:
    //! @name Role registration
    //@{
    template< typename Role > void RegisterRole( Role* );
    //@}

private:
    //! @name Role initialization
    //@{
    template< typename Role > unsigned int EnsureSize();
    template< typename Role > unsigned int GetIdx() const;
    //@}

private:
    T_RoleVector roles_;

    static uint nCurrentIdx_;
};


#include "MT_RoleContainer.inl"

#endif // __MT_RoleContainer_h_