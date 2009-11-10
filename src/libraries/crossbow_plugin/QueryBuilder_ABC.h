// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __QueryBuilder_ABC_h_
#define __QueryBuilder_ABC_h_

namespace plugins
{
namespace crossbow
{
    class Shape_ABC;

// =============================================================================
/** @class  QueryBuilder_ABC
    @brief  QueryBuilder_ABC
*/
// Created: JCR 2009-04-24
// =============================================================================
class QueryBuilder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
            QueryBuilder_ABC() {}
    virtual ~QueryBuilder_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SetId( const std::string& field ) = 0;
    virtual void SetField( const std::string& field, const std::string& value ) = 0;
    virtual void SetField( const std::string& field, int value ) = 0;
    virtual void SetField( const std::string& field, long value ) = 0;
    virtual void SetField( const std::string& field, double value ) = 0;
    virtual void SetGeometry( const Shape_ABC& shape ) = 0;

    //@}

    //! @Accessors 
    //@{
    virtual bool        IsValid() const = 0;
	virtual std::string GetSequence() const = 0;
	virtual std::string GetTableName() const = 0;
    virtual std::string Create() const = 0;
    //@}
};

} // end namespace QueryBuilder
}

#endif // __QueryBuilder_ABC_h_
