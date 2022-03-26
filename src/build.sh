mkdir ebs-builds
gcc -Wall -DuseGCC -o ebs-builds/ebs-gcc ebs.c compile.c file.c println.c token.c code_location.c lexer.c parser.c visitor.c find_compiler.c str.c AST.c build_info.c
gcc -Wall -DuseCLANG -o ebs-builds/ebs-clang ebs.c compile.c file.c println.c token.c code_location.c lexer.c parser.c visitor.c find_compiler.c str.c AST.c build_info.c
gcc -Wall -DuseCC ebs.c -o ebs-builds/ebs-cc compile.c file.c println.c token.c code_location.c lexer.c parser.c visitor.c find_compiler.c str.c AST.c build_info.c