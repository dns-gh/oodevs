// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectComponentRegistry_ABC_h_
#define __ObjectComponentRegistry_ABC_h_

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ObjectComponentRegistry_ABC
    @brief  ObjectComponentRegistry_ABC
*/
// Created: JCR 2008-05-30
// =============================================================================
template< typename Component >
class ObjectComponentRegistry_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::function< void( Component&, xml::xistream& ) > T_CallBack;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ObjectComponentRegistry_ABC() {}
    virtual ~ObjectComponentRegistry_ABC() {}
    //@}

    //! @name Registrer
    //@{
    virtual void Register( const std::string& component, const T_CallBack& callback ) = 0;
    //@}

protected:
    //! @name Type
    //@{
    typedef std::map< std::string, T_CallBack >     T_CallBacks;
    typedef typename T_CallBacks::const_iterator    CIT_Callbacks;
    //@}
};

#endif
