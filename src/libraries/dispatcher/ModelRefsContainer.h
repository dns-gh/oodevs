// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelRefsContainer_h_
#define __ModelRefsContainer_h_

namespace dispatcher 
{

// =============================================================================
/** @class  ModelRefsContainer
    @brief  ModelRefsContainer
*/
// Created: NLD 2006-09-25
// =============================================================================
template< typename T >
class ModelRefsContainer
{
public:
    //! @name Types
    //@{
    typedef typename std::map< unsigned long, T* >  T_ModelRefMap;
    typedef typename T_ModelRefMap::const_iterator  CIT_ModelRefMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
     ModelRefsContainer();
    ~ModelRefsContainer();
    //@}

    //! @name Operations
    //@{
    void Clear     ();
    void Register  ( T& t );
    void Unregister( T& t );

    template< typename L, typename E > 
    void Send( L& asnList ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ModelRefsContainer( const ModelRefsContainer& );            //!< Copy constructor
    ModelRefsContainer& operator=( const ModelRefsContainer& ); //!< Assignement operator
    //@}

    //! @name Tools
//    //@{
    void ThrowError( unsigned long nID, const std::string& strMessage ) const;
    //@}

private:
    T_ModelRefMap modelRefs_;
};

}

#include "ModelRefsContainer.inl"

#endif // __ModelRefsContainer_h_
