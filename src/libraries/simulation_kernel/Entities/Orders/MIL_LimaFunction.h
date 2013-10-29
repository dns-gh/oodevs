// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_LimaFunction_h_
#define __MIL_LimaFunction_h_

#include "protocol/Protocol.h"

// =============================================================================
/** @class  MIL_LimaFunction
    @brief  MIL_LimaFunction
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_LimaFunction
{
public:
    //! @name Types
    //@{
    static const MIL_LimaFunction LD_;
    static const MIL_LimaFunction LCA_;
    static const MIL_LimaFunction LC_;
    static const MIL_LimaFunction LI_;
    static const MIL_LimaFunction LO_;
    static const MIL_LimaFunction LCAR_;
    static const MIL_LimaFunction LR_;
    static const MIL_LimaFunction LDM_;
    static const MIL_LimaFunction LFM_;
    static const MIL_LimaFunction LIA_;
    //@}

public:
    //! @name Factory
    //@{
    static       void              Initialize();
    static       void              Terminate ();
    static const MIL_LimaFunction* Find      ( const sword::PhaseLineOrder::Function& asn );
    static const MIL_LimaFunction* Find      ( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    unsigned int                    GetID   () const;
    sword::PhaseLineOrder::Function GetAsnID() const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     MIL_LimaFunction( const sword::PhaseLineOrder::Function& asn, unsigned int nID, const std::string& strName );
    ~MIL_LimaFunction();

     MIL_LimaFunction( const MIL_LimaFunction& );            //!< Copy constructor
     MIL_LimaFunction& operator=( const MIL_LimaFunction& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< sword::PhaseLineOrder::Function, const MIL_LimaFunction* > T_LimaFunctionMap;
    //@}

private:
    const sword::PhaseLineOrder::Function  nAsnID_;
    const unsigned int                  nID_;
    const std::string                   strName_;

private:
    static T_LimaFunctionMap limaFunctions_;
};

namespace boost
{
namespace archive
{
    template< class Archive >
    void save( Archive& ar, const MIL_LimaFunction* t )
    {
        unsigned int id = t ? t->GetID() : std::numeric_limits< unsigned int >::max();
        ar << id;
    }
    template< class Archive >
    void load( Archive& ar, const MIL_LimaFunction*& t )
    {
        unsigned int id;
        ar >> id;
        t = MIL_LimaFunction::Find( id );
    }
}
}

#endif // __MIL_LimaFunction_h_
