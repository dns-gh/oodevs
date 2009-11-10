// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __plugins_crossbow_QueryBuilder_h_
#define __plugins_crossbow_QueryBuilder_h_

#include <vector>
#include "QueryBuilder_ABC.h"

namespace plugins
{
namespace crossbow
{

    class Shape_ABC;

// =============================================================================
/** @class  QueryBuilder
    @brief  QueryBuilder
*/
// Created: JCR 2009-04-24
// =============================================================================
class QueryBuilder : public QueryBuilder_ABC
{

public:
    //! @name Constructors/Destructor
    //@{    
    virtual ~QueryBuilder();
    //@}

    //! @name Operations
    //@{
    void SetId( const std::string& field );
    void SetField( const std::string& field, const std::string& value );
    void SetField( const std::string& field, int value );
    void SetField( const std::string& field, long value );
    void SetField( const std::string& field, double value );
    void SetGeometry( const Shape_ABC& shape );    
    //@}

    //! @name Accessor
    //@{
    virtual bool IsValid() const;
	virtual std::string GetSequence() const;
			std::string GetTableName() const;
    //@}

protected:
    //! @name 
    //@{
    explicit QueryBuilder( const std::string& table );
    //@}

    //! @name 
    //@{
    std::string BuildInsert() const;
    std::string BuildUpdate( const std::string& clause ) const;
    std::string BuildDelete( const std::string& clause ) const;
	std::string BuildSelect( const std::string& clause ) const;
	//@}

private:
    //! @name Copy/Assignment
    //@{
    QueryBuilder( const QueryBuilder& );            //!< Copy constructor
    QueryBuilder& operator=( const QueryBuilder& ); //!< Assignment operator
    //@}

private:
    typedef std::pair< std::string, std::string >   T_Parameter;

    //! @name 
    //@{
    void AddParameter( const T_Parameter& parameter );
    //@}

private:
    //! @name Member data
    //@{
    const std::string table_;
    std::vector< T_Parameter > parameters_;
	std::string idField_;
    //@}
};

class InsertQueryBuilder : public QueryBuilder {    
public:
    explicit InsertQueryBuilder( const std::string& table ) 
        : QueryBuilder( table ) {}
    virtual ~InsertQueryBuilder() {}
    
    std::string Create() const
    {
        return BuildInsert();
    }
};

class UpdateQueryBuilder : public QueryBuilder {    
public:
    explicit UpdateQueryBuilder( const std::string& table ) 
        : QueryBuilder( table ), clause_ () {}
    virtual ~UpdateQueryBuilder() {}

    void SetClause( const std::string& clause ) 
    {
        clause_ = clause;
    }

    std::string Create() const
    {
        return BuildUpdate( clause_ );
    }
private:
    std::string clause_;
};

class DeleteQueryBuilder : public QueryBuilder {    
public:
    explicit DeleteQueryBuilder( const std::string& table ) 
        : QueryBuilder( table ), clause_ () {}
	DeleteQueryBuilder( const std::string& table, std::string clause ) 
        : QueryBuilder( table ), clause_ ( clause ) {}
    virtual ~DeleteQueryBuilder() {}

    void SetClause( const std::string& clause ) 
    {
        clause_ = clause;
    }

    bool IsValid() const 
    {
        return true;
    }

    std::string Create() const
    {
        return BuildDelete( clause_ );
    }

private:
    std::string clause_;
};

}
}

#endif // __plugins_crossbow_QueryBuilder_h_
