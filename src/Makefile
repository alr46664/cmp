
# variaveis dos compiladores/simuladores
MIPS=qtspim

# TO COMPILE THIS SOURCE CODE, YOU WILL NEED GCC / G++ >= 4.8
# compilador c, linkar com o flex,
GCC=gcc
CFLAGS=-ggdb -g3 -lfl -lm

GPP=g++
CPPFLAGS=-Wno-write-strings -ggdb  -g3 -lm -std=c++11

# VERIFICACAO DE LEAKS DE MEMORIA
VALGRIND=valgrind
VALGRIND_ARGS=--leak-check=full --show-leak-kinds=all -v

# compilador c++, codegen
# GPP=g++
# CPPFLAGS=

# # encontre todos os arquivos .CPP e passe-os para a
# # extensao .O
# CPP_FILES := $(wildcard *.cpp)
# OBJ_CPP_FILES := $(notdir $(CPP_FILES:.cpp=.o))

# arquivos objeto de cada programa (lexico e codegen)
OBJ_SYN=syn.ocpp classes/node.ocpp classes/error.ocpp classes/operate.ocpp classes/parser.ocpp classes/sintatical.ocpp classes/semantical.ocpp classes/context.ocpp classes/defined.ocpp
OBJ_LEX=lex.yy.o
OBJ_GEN=gen.o

# binarios
ANALISER_BIN=lexico
CODEGEN_BIN=codegen
SYN_BIN=syn
LEX_BIN=lex
GEN_BIN=gen

ANALISER_TARGET=$(ANALISER_BIN).target
CODEGEN_TARGET=$(CODEGEN_BIN).out

# rode o simulador MIPS (SPIM)
# sintaxe:
# 		make arquivo_jsc_a_ser_executado.spim
%.spim: %.asm
	@echo -e "\n\tExecutando simulador SPIM (assembly) ...\n"
	@$(MIPS) -file $*.asm

# rode o analisador lexico
# sintaxe:
# 		make arquivo_jsc_a_ser_analisado.lex
%.lex: $(ANALISER_TARGET)
	@echo -e "\n\tExecutando analisador lexico (scanner/tokenizer) ...\n"
	@./$(ANALISER_BIN) $*.jsc $*.lex
	@echo -e "Resultado:\n"
	@cat $*.lex

# rode o analisador sintatico
# sintaxe:
# 		make arquivo_jsc_a_ser_analisado.syn
%.syn: $(ANALISER_TARGET)
	@echo -e "\n\tExecutando analisador simbolico (parser) ...\n"
	@./$(ANALISER_BIN) $*.jsc $*.syn
	@echo -e "Resultado:\n"
	@cat $*.syn

# rode o gerador de codigo
# sintaxe:
# 		make arquivo_jsc_a_ser_compilado.asm
%.asm: $(CODEGEN_TARGET)
	@echo -e "\n\tExecutando gerador de código (assembly) ...\n"
	@./$(CODEGEN_BIN) $*.jsc $*.asm
	@echo -e "Resultado:\n"
	@cat $*.asm

# compile o analisador (lexico e sintatico) e torne o
# script ./lexico executavel
$(ANALISER_TARGET): $(LEX_BIN) $(SYN_BIN)
	@chmod +rx $(ANALISER_BIN)

# compile o analisador lexico
$(LEX_BIN): $(OBJ_LEX)
	$(GCC) -o $(LEX_BIN) $^ $(CFLAGS)

# compile o analisador semantico/sintatico
$(SYN_BIN): $(OBJ_SYN)
	$(GPP) -o $(SYN_BIN) $^ $(CPPFLAGS)

# compile o analisador (lexico e sintatico) e torne o
# script ./lexico executavel
$(CODEGEN_TARGET): $(GEN_BIN) $(ANALISER_TARGET)
	@chmod +rx $(CODEGEN_BIN)

# compile o gerador de codigo lexico
$(GEN_BIN): $(OBJ_GEN)
	$(GCC) -o $(GEN_BIN) $^ $(CFLAGS)

#compile o analisador lexico (scanner) usando flex
lex.yy.c: lex.yy.l
	lex lex.yy.l

# limpe arquivos temporarios
clean:
	@echo -e "\n\tLimpando arquivos .syn .lex .asm ...\n"
	@$(RM) *.syn *.lex *.asm

# limpe binarios e arquivos objeto
clean_bin:
	@echo -e "\n\tLimpando binarios e objetos ...\n"
	@$(RM) $(LEX_BIN) $(SYN_BIN) $(GEN_BIN) *.o *.ocpp classes/*.o classes/*.ocpp lex.yy.c

full_clean: clean_bin clean


%.v: %
	@$(VALGRIND) $(VALGRIND_ARGS) ./$*

# compile de .c em arquivo objeto .o
%.o: %.c
	$(GCC) -c -o $@ $< $(CFLAGS)

# compile de .cpp em arquivo objeto .ocpp
%.ocpp: %.cpp %.h
	$(GPP) -c -o $@ $< $(CPPFLAGS)

