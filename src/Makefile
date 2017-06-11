# variaveis dos compiladores/simuladores
MIPS=qtspim

# compilador c, linkar com o flex,
GCC=gcc
CFLAGS=-lfl -std=c99

# compilador c++, codegen
# GPP=g++
# CPPFLAGS=

# # encontre todos os arquivos .CPP e passe-os para a
# # extensao .O
# CPP_FILES := $(wildcard *.cpp)
# OBJ_CPP_FILES := $(notdir $(CPP_FILES:.cpp=.o))

# arquivos objeto de cada programa (lexico e codegen)
OBJ_SYN=syn.o
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
	$(GCC) -o $(LEX_BIN) $< $(CFLAGS)

# compile o analisador semantico/sintatico
$(SYN_BIN): $(OBJ_SYN)
	$(GCC) -o $(SYN_BIN) $< $(CFLAGS)

# compile o analisador (lexico e sintatico) e torne o
# script ./lexico executavel
$(CODEGEN_TARGET): $(GEN_BIN) $(ANALISER_TARGET)
	@chmod +rx $(CODEGEN_BIN)

# compile o gerador de codigo lexico
$(GEN_BIN): $(OBJ_GEN)
	$(GCC) -o $(GEN_BIN) $< $(CFLAGS)

#compile o analisador lexico (scanner) usando flex
lex.yy.c: lex.yy.l
	lex lex.yy.l

# limpe arquivos temporarios
clean:
	@echo -e "\n\tLimpando pasta...\n"
	@$(RM) *.syn *.lex *.asm

# limpe binarios e arquivos objeto e arquivos temporarios
full_clean: clean
	@$(RM) $(LEX_BIN) $(SYN_BIN) $(GEN_BIN) *.o *.oc lex.yy.c

# # compile de .cpp em arquivo objeto .o
# %.o: %.cpp
# 	$(GPP) -c -o $@ $< $(CPPFLAGS)

# compile de .c em arquivo objeto .o
%.o: %.c
	$(GCC) -c -o $@ $< $(CFLAGS)
