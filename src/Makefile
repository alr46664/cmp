
# variaveis dos compiladores/simuladores
MIPS=qtspim

# TO COMPILE THIS SOURCE CODE, YOU WILL NEED GCC / G++ >= 4.8
GPP=g++
CPPFLAGS=-lfl -lm -Wno-write-strings -std=c++11
CPPFLAGS_DEBUG=-ggdb -g3
CPPFLAGS_RELEASE=-march=native -O2

# VERIFICACAO DE LEAKS DE MEMORIA
VALGRIND=valgrind
VALGRIND_ARGS=--leak-check=full --show-leak-kinds=all -v

# # encontre todos os arquivos .CPP e passe-os para a
# # extensao .O
# CPP_FILES := $(wildcard *.cpp)
# OBJ_CPP_FILES := $(notdir $(CPP_FILES:.cpp=.o))

# arquivos objeto de cada programa (lexico e codegen)
OBJ_COMPILER=main.o classes/node.o classes/error.o classes/operate.o classes/parser.o classes/sintatical.o classes/semantical.o classes/context.o classes/defined.o lex.yy.o

# binarios
COMPILER_BIN=compilador
COMPILER_BIN_REL=$(COMPILER_BIN).release
COMPILER_BIN_DEB=$(COMPILER_BIN).debug

# default do make
default: full_clean
	@$(MAKE) $(COMPILER_BIN_REL)


# rode o simulador MIPS (SPIM)
# sintaxe:
# 		make arquivo_jsc_a_ser_executado.spim
%.spim: %.asm
	@echo -e "\n\tExecutando simulador SPIM (assembly) ...\n"
	@$(MIPS) -file $*.asm &

# rode o analisador sintatico
# sintaxe:
# 		make arquivo_jsc_a_ser_analisado.syn
%.lex %.syn %.asm: $(COMPILER_BIN_REL)
	@echo -e "\n\tExecutando compilador ...\n"
	@./$(COMPILER_BIN) $*.jsc $@
	@echo -e "Resultado:\n"
	@cat $@

# rode a compilacao em modo release
$(COMPILER_BIN_REL): CPPFLAGS += $(CPPFLAGS_RELEASE)
$(COMPILER_BIN_REL): $(COMPILER_BIN)


# rode a compilacao em modo debug
$(COMPILER_BIN_DEB): CPPFLAGS += $(CPPFLAGS_DEBUG)
$(COMPILER_BIN_DEB): $(COMPILER_BIN)


# compile o analisador semantico/sintatico e gerador
$(COMPILER_BIN): $(OBJ_COMPILER)
	$(GPP) -o $(COMPILER_BIN) $^ $(CPPFLAGS)

#compile o analisador lexico (scanner) usando flex
lex.yy.cpp: lex.yy.l
	flex -o lex.yy.cpp lex.yy.l

# limpe arquivos temporarios
clean:
	@echo -e "\n\tLimpando arquivos .syn .lex .asm .temp* ...\n"
	@find * -name '*.syn' -exec $(RM) {} \;
	@find * -name '*.lex' -exec $(RM) {} \;
	@find * -name '*.asm' -exec $(RM) {} \;
	@find * -name '*.temp*' -exec $(RM) {} \;

# limpe binarios e arquivos objeto
clean_bin:
	@echo -e "\n\tLimpando binarios e objetos ...\n"
	@$(RM) $(COMPILER_BIN) lex.yy.c lex.yy.cpp lex.yy.cc
	@find * -name '*.o' -exec $(RM) {} \;
	@find * -name '*.ocpp' -exec $(RM) {} \;

full_clean: clean_bin clean


%.v: %
	@$(VALGRIND) $(VALGRIND_ARGS) ./$*

# compile de .cpp em arquivo objeto .ocpp
%.o: %.cpp %.h
	$(GPP) -c -o $@ $< $(CPPFLAGS)

