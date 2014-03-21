// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CORE_MODEL_PRINTER_H
#define CORE_MODEL_PRINTER_H

#include "ModelVisitor_ABC.h"
#include <iosfwd>

namespace core
{
    class Model;

// =============================================================================
/** @class  ModelPrinter
    @brief  Debug model printer
*/
// Created: MCO 2012-04-06
// =============================================================================
class ModelPrinter : private ModelVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ModelPrinter( const Model& model, std::ostream& os, bool quotes, std::size_t indent = 0 );
    virtual ~ModelPrinter();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Visit( int64_t value );
    virtual void Visit( uint64_t value );
    virtual void Visit( double value );
    virtual void Visit( const std::string& value );

    virtual void Visit( const std::string& key, const Model& child );
    virtual void Visit( std::size_t key, const Model& child );

    virtual void Visit( const Model& element );

    virtual void Visit( const boost::shared_ptr< UserData_ABC >& data );
    //@}

    //! @name Helpers
    //@{
    std::string QuoteString( const std::string& s ) const;

    void Open( char prefix, bool open, const std::string& key );
    void Close( char postfix, bool open );

    void Indent( std::size_t indent = 0 );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    ModelPrinter( const ModelPrinter& parent, const Model& model, bool key );
    //@}

private:
    //! @name Member data
    //@{
    std::ostream& os_;
    const bool quotes_;
    const std::size_t indent_;
    const std::size_t depth_;
    bool child_;
    bool element_;
    bool key_;
    //@}
};

}

#endif // CORE_MODEL_PRINTER_H
