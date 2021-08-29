//
// Created by bogda on 8/7/2021.
//

#ifndef VARSOLVER_SOLVER_H
#define VARSOLVER_SOLVER_H

#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <optional>
#include <cassert>
#include <string>

inline bool is_digit( char c ) {
    return ( c >= '0' && c <= '9' );
}

inline bool is_alpha( char c ) {
    return ( c >= 'a' && c <= 'z' );
}

inline bool is_alpha_numeric( char c ) {
    return is_alpha( c ) || is_digit( c );
}

inline bool term_is_valid( std::string_view term ) {
    for( auto c : term ) {
        if( !is_alpha_numeric( c ) ) {
            std::cerr << "Error in character " << c << " of " << term << std::endl;
            return false;
        }
    }
    return true;
}

inline constexpr long pow10( int exp ) {
    assert( exp <  9 );
    if( exp == 0 ) {
        return 1l;
    }
    return 10l * pow10( exp - 1);
}

class Digit {
    std::optional<char> value;
    const bool is_unknown;
    char name;

public:
    Digit( char c );
    void set( char val );
    char get() const;
    bool is_set() const;
};

class Solver;

class Variable : public std::enable_shared_from_this<Variable>{

    const std::string _name;
    Solver& _context;
    std::vector<std::shared_ptr<Digit>> _digits;
    std::map<char, std::shared_ptr<Digit>> _variables;

public:
    Variable( std::string_view name, Solver& context );
    void load();
    int evaluate();
    void set_unknown( char x, char var ) ;
    std::string to_string() const;
    const std::string& get_name() const { return _name; }
};

class Solver {
    std::shared_ptr<Variable> _term1;
    std::shared_ptr<Variable> _term2;
    std::shared_ptr<Variable> _result;
    // map of all unknown values names to the variables where the unknowns appear
    // e.g.:
    // x -> {xy}
    // y -> {xy, y}
    std::map<char, std::vector<std::shared_ptr<Variable>>> _unknowns{};
    std::map<char, char> _unknownsVals{};

    bool test_equality();

    void set_unknown_in_all_vars_and_check( char x, char var );
    void safe_set_unknown_in_all_vars( char x, char var );
public:
    Solver( std::string_view term1, std::string_view term2, std::string_view result );
    void register_unknown( char x,  std::shared_ptr<Variable> term );
    void solve();
    void cycle_variable_at_index( const std::vector<char>& list_of_unknowns, int index );

    void check_equality();
};

#endif //VARSOLVER_SOLVER_H
