// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelsContainer_h_
#define __ModelsContainer_h_

#include <map>

namespace dispatcher 
{
    class Model;

// =============================================================================
/** @class  ModelsContainer
    @brief  ModelsContainer
*/
// Created: NLD 2006-09-25
// =============================================================================
template< typename T >
class ModelsContainer
{
public:
    //! @name Types
    //@{
    typedef typename std::map< unsigned long, T* > T_ModelMap;
    typedef typename T_ModelMap::iterator          IT_ModelMap;
    typedef typename T_ModelMap::const_iterator    CIT_ModelMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
     ModelsContainer();
    ~ModelsContainer();
    //@}

    //! @name Operations
    //@{
    template< typename P >
    T&    Create ( Model& model, unsigned long nID, P& param );
    template< typename P1, typename P2 >
    T&    Create ( Model& model, unsigned long nID, P1& param1, P2& param2 );
    void  Destroy( unsigned long nID );
    T&    Get    ( unsigned long nID ) const;
    T*    Find   ( unsigned long nID ) const;    
    void  Clear  ();

    template< typename F >				void Apply( F functor ) const;
    template< typename F, typename P1 >	void Apply( F functor, P1& param1 ) const;
    template< typename L, typename E >  void Send ( L& asnList ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ModelsContainer( const ModelsContainer& );            //!< Copy constructor
    ModelsContainer& operator=( const ModelsContainer& ); //!< Assignement operator
    //@}

    //! @name Tools
    //@{
    void ThrowError( unsigned long nID, const std::string& strMessage ) const;
    //@}

private:
    T_ModelMap models_;
};

}

#include "ModelsContainer.inl"

#endif // __ModelsContainer_h_
