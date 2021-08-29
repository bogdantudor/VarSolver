//
// Created by bogdan on 8/7/2021.
//

#include "Solver.h"
#include "logging.h"

Digit::Digit( char c ) : is_unknown( is_alpha( c ) ) {

    if( !is_unknown ) {
        value = c;
    }
    name = c;
}

void Digit::set( char val ) {

    assert( is_unknown );
    value = val;
}

char Digit::get() const {

    if( !value.has_value() ) {
        return name;
    }
    return value.value();
}

bool Digit::is_set() const {

    return value.has_value();
}

/**********************************************************************************************************************/

Variable::Variable( std::string_view name, Solver& context ) : _name( name ), _context( context ) {
}

void Variable::load() {

    debug << "Loading variable " << _name << debug.endl;
    for( char c : _name ) {
        auto digit = std::make_shared<Digit>( c );
        _digits.emplace_back( digit );
        if( is_alpha( c ) ) {
            _context.register_unknown( c, shared_from_this() );
            _variables.emplace( c, digit );
        }
    }
}

int Variable::evaluate() {

    int value{};
    for( auto digit : _digits ) {
        int val;
        if( !digit->is_set() ) {
            val = '0';
        } else {
            val = digit->get();
        }
        value = value * 10 + ( val - '0' );
    }
    return value;
}

void Variable::set_unknown( char x, char var ) {

    auto it = _variables.find( x );
    if( it != _variables.end() ) {
        auto digitsPtr = it->second;
        digitsPtr->set( var );
    }
}

std::string Variable::to_string() const {

    std::string result{};
    for( auto digit : _digits ) {
        result.push_back( digit->get() );
    }
    return result;
}

/**********************************************************************************************************************/

Solver::Solver( std::string_view term1, std::string_view term2, std::string_view result ) :
        _term1( std::make_shared<Variable>( term1, *this ) ),
        _term2( std::make_shared<Variable>( term2, *this ) ),
        _result( std::make_shared<Variable>( result, *this ) ) {

    _term1->load();
    _term2->load();
    _result->load();
}

void Solver::register_unknown( char unknown, std::shared_ptr<Variable> term ) {

    auto it = _unknowns.find( unknown );
    if( it == _unknowns.end() ) {
        _unknowns.emplace( unknown, std::vector<std::shared_ptr<Variable>>{ term } );
    } else {
        it->second.push_back( term );
    }
}

bool Solver::test_equality() {

    return _term1->evaluate() + _term2->evaluate() == _result->evaluate();
}

void Solver::safe_set_unknown_in_all_vars( char x, char var ) {

    debug << "Setting " << x << "=" << var << ": ";
    _unknownsVals[ x ] = var;
    _term1->set_unknown( x, var );
    _term2->set_unknown( x, var );
    _result->set_unknown( x, var );
    for( auto[k, v]: _unknownsVals ) {
        debug << k << "=" << v << " ";
    }
    debug << debug.endl;
}

void Solver::check_equality() {

    if( test_equality() ) {
        info << "Found solution: ";
        for( auto[k, v]: _unknownsVals ) {
            info << k << "=" << v << " ";
        }
        info << " ==> " << _term1->to_string() << " + " << _term2->to_string() << " = " << _result->to_string() <<
             info.endl;
    } else {
        for( auto[k, v]: _unknownsVals ) {
            debug << k << "=" << v << " ";
        }
        debug << ": failed equality " << _term1->get_name() << "(" << _term1->evaluate() << ") + "<< _term2->get_name()
        << "(" << _term2->evaluate() <<") != " <<
              _result->get_name() << "(" << _result->evaluate() << ")" << debug.endl;
    }
}

void Solver::cycle_variable_at_index( const std::vector<char>& list_of_unknowns, int index ) {

    if( list_of_unknowns.size() <= index ) {
        return;
    }
    char unknown_digit_name = list_of_unknowns[ index ];
    auto crt_unknown_val = _unknownsVals.at( unknown_digit_name );
    if( crt_unknown_val >= '9' ) {
        debug << "Increasing variable at index " << index + 1 << debug.endl;
        safe_set_unknown_in_all_vars( unknown_digit_name, '0' );
        cycle_variable_at_index( list_of_unknowns, index + 1 );
    } else {
        safe_set_unknown_in_all_vars( unknown_digit_name, ++crt_unknown_val );
    }
}

void Solver::solve() {

    std::vector<char> list_of_unknowns;
    for( auto&[unknown_digit_name, _list] : _unknowns ) {
        list_of_unknowns.push_back( unknown_digit_name );
        safe_set_unknown_in_all_vars( unknown_digit_name, '0' );
    }

    int steps = pow10( _unknowns.size() );
    int step_progress = steps / 10;
    for( long i{}; i < steps; ++i ) {
        if( steps >= 10000 ) {
            if ( i && i % step_progress == 0 ) {
                info << "Step " << i << "/" << steps << info.endl;
            }
        }
        debug << "Cycle " << i << " " ;
        cycle_variable_at_index( list_of_unknowns, 0 );
        check_equality();
        debug << info.endl;
    }
}
