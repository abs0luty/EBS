mkdir ebs-builds
gcc -Wall -DuseGCC -o ebs-builds/ebs-gcc binaries/ebs.c compile/compile.c util/file.c util/println.c lexer/token.c util/code_location.c lexer/lexer.c parser/parser.c visitor/visitor.c util/str.c AST/AST.c build_info/build_info.c
gcc -Wall -DuseCLANG -o ebs-builds/ebs-clang binaries/ebs.c compile/compile.c util/file.c util/println.c lexer/token.c util/code_location.c lexer/lexer.c parser/parser.c visitor/visitor.c util/str.c AST/AST.c build_info/build_info.c
gcc -Wall -DuseCC binaries/ebs.c -o ebs-builds/ebs-cc compile/compile.c util/file.c util/println.c lexer/token.c util/code_location.c lexer/lexer.c parser/parser.c visitor/visitor.c util/str.c AST/AST.c build_info/build_info.c

sudo cp ebs-builds/ebs-gcc /usr/local/bin/ebs-gcc
sudo cp ebs-builds/ebs-clang /usr/local/bin/ebs-clang
sudo cp ebs-builds/ebs-cc /usr/local/bin/ebs-cc