# variaveis dos compiladores/simuladores
MIPS=qtspim

# compilador c, linkar com o flex,
GCC=gcc
CFLAGS=-lfl

# compilador c++, codegen
GPP=g++
CPPFLAGS=

# # encontre todos os arquivos .CPP e passe-os para a
# # extensao .O
# CPP_FILES := $(wildcard *.cpp)
# OBJ_CPP_FILES := $(notdir $(CPP_FILES:.cpp=.o))

# arquivos objeto de cada programa (lexico e codegen)
OBJ_LEXICO=lex.yy.oc
OBJ_CODEGEN=

# binarios
LEXICO_BIN=lexico.out
CODEGEN_BIN=codegen.out

# rode o simulador MIPS (SPIM)
# sintaxe:
# 		make arquivo_jsc_a_ser_executado.spim
%.spim: %.asm
	@echo -e "\n"
	@$(MIPS) -file $*.asm

# rode o analisador lexico
# sintaxe:
# 		make arquivo_jsc_a_ser_analisado.lex
%.lex: $(LEXICO_BIN)
	@echo -e "\n\tExecutando analisador lexico (scanner/tokenizer) ...\n"
	@./$(LEXICO_BIN) $*.jsc $*.lex

# rode o analisador sintatico
# sintaxe:
# 		make arquivo_jsc_a_ser_analisado.syn
%.syn: $(LEXICO_BIN)
	@echo -e "\n\tExecutando analisador simbolico (parser) ...\n"
	@./$(LEXICO_BIN) $*.jsc $*.syn

# rode o gerador de codigo
# sintaxe:
# 		make arquivo_jsc_a_ser_compilado.asm
%.asm: $(CODEGEN_BIN)
	@echo -e "\n\tExecutando gerador de código (assembly) ...\n"
	@./$(CODEGEN_BIN) $*.jsc $*.asm

# compile o analisador lexico / sintatico
$(LEXICO_BIN): $(OBJ_LEXICO)
	$(GCC) -o $(LEXICO_BIN) $< $(CFLAGS)

# compile o gerador de codigo lexico
$(CODEGEN_BIN): $(OBJ_CODEGEN)
	$(GPP) -o $(CODEGEN_BIN) $< $(CPPFLAGS)

#compile o analisador lexico (scanner) usando flex
lex.yy.c: lex.yy.l
	lex lex.yy.l

# limpe todos os arquivos (exceto arquivo fonte, incluindo binarios)
clean:
	@echo -e "\n\tLimpando pasta...\n"
	@$(RM) $(LEXICO_BIN) $(CODEGEN_BIN) *.o *.oc *.syn *.lex *.asm lex.yy.c

# compile de .cpp em arquivo objeto .o
%.o: %.cpp
	$(GPP) -c -o $@ $< $(CPPFLAGS)

# compile de .c em arquivo objeto .o
%.oc: %.c
	$(GCC) -c -o $@ $< $(CFLAGS)
