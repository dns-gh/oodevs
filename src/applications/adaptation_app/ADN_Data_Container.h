// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Data_Container_h_
#define __ADN_Data_Container_h_

#include <boost/noncopyable.hpp>
#include "ADN_Data_ABC.h"

// =============================================================================
/** @class  ADN_Data_Container
    @brief  ADN_Data_Container
*/
// Created: ABR 2012-01-18
// =============================================================================
class ADN_Data_Container : public ADN_Data_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Data_Container();
    virtual ~ADN_Data_Container();
    //@}

    //! @name Operations
    //@{
    template< typename DataBaseType >
    void AddElement( int index );
    template< typename DataBaseType >
    DataBaseType& GetElement( int index );
    //@}

    //! @name ADN_Data_ABC Operations
    //@{
    virtual void FilesNeeded( T_StringList& vFiles ) const;
    virtual void Reset();
    virtual void Load( const tools::Loader_ABC& fileLoader );
    virtual void Save();
    virtual bool IsValidDatabase();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void ReadArchive( xml::xistream& input );
    virtual void WriteArchive( xml::xostream& output );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< int, ADN_Data_ABC* >  T_Elements;
    typedef T_Elements::iterator           IT_Elements;
    typedef T_Elements::const_iterator    CIT_Elements;
    //@}

private:
    //! @name Member data
    //@{
    T_Elements elements_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::AddElement
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
template< typename DataBaseType >
void ADN_Data_Container::AddElement( int index )
{
    assert( elements_.find( index ) == elements_.end() );
    elements_[ index ] = new DataBaseType();
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::GetElement
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
template< typename DataBaseType >
DataBaseType& ADN_Data_Container::GetElement( int index )
{
    CIT_Elements it = elements_.find( index );
    assert( it != elements_.end() );
    assert( dynamic_cast< DataBaseType* >( it->second ) != 0 );
    return *static_cast< DataBaseType* >( it->second );
}

#endif // __ADN_Data_Container_h_
