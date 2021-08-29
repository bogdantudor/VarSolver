#include "logging.h"
#include "Solver.h"

#include <iostream>

bool check_args( char** argv ) {

    std::string_view term1 = argv[ 1 ];
    std::string_view op = argv[ 2 ];
    std::string_view term2 = argv[ 3 ];
    std::string_view equal = argv[ 4 ];
    std::string_view result = argv[ 5 ];

    if( !term_is_valid( term1 ) ) {
        return false;
    }

    if( !term_is_valid( term2 ) ) {
        return false;
    }

    if( equal != "=" ) {
       error << "Error in equal sign: can only be '=' but instead is '" << equal << "'" << error.endl;
        return false;
    }

    if( op != "+" ) {
        error << "Error in op sign: can only be '+' but instead is '" << equal << "'" << error.endl;
        return false;
    }

    if( !term_is_valid( result ) ) {
        return false;
    }

    return true;
}

int usage() {
    info << "Usage: term1 + term2 = result. \n\r E.g. xy + y = 20" << info.endl;
    return 1;
}

int main( int argc, char** argv ) {

    std::string_view debug = "-d";
    if( argc < 6 ) {
        return usage();
    }

    if( argc == 7 ) {
        if( argv[1] == debug ) {
            setLogLevel( LogLevel::DEBUG );
            ++argv;
        } else {
            return usage();
        }
    }

    if( !check_args( argv ) ) {
        return usage();
    }

    std::string_view term1 = argv[ 1 ];
    std::string_view op = argv[ 2 ];
    std::string_view term2 = argv[ 3 ];
    std::string_view equal = argv[ 4 ];
    std::string_view result = argv[ 5 ];

    info << "Read " << term1 << ' ' << op << ' ' << term2 << ' ' << equal << ' ' << result << info.endl;

    Solver solver{term1, term2, result};

    solver.solve();

    return 0;
}