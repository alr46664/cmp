# variaveis dos compiladores/simuladores
MIPS=qtspim
GCC=gcc
GPP=g++
CFLAGS=

# encontre todos os arquivos .CPP e passe-os para a
# extensao .O
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(notdir $(CPP_FILES:.cpp=.o))

# arquivos objeto de cada programa (lexico e codegen)
OBJ_LEXICO=
OBJ_CODEGEN=

# rode o simulador MIPS (SPIM)
# sintaxe:
# 		make spim arg=arquivo_asm_a_ser_executado
spim: gen
	@$(MIPS) -file $(arg).asm

# rode o analisador lexico
# sintaxe:
# 		make lex arg=texto_a_ser_analisado
lex: lexico
	@./lexico $(arg).jsc $(arg).lex

# rode o analisador sintatico
# sintaxe:
# 		make sin arg=texto_a_ser_analisado
syn: lexico
	@./lexico $(arg).jsc $(arg).syn

# rode o gerador de codigo
# sintaxe:
# 		make gen arg=texto_a_ser_compilado
gen: codegen
	@./codegen $(arg).jsc $(arg).asm

# compile o analisador lexico / sintatico
lexico: $(OBJ_LEXICO)
	@$(GPP) -o lexico $< $(CFLAGS)

# compile o gerador de codigo lexico
codegen: $(OBJ_CODEGEN)
	@$(GPP) -o codegen $< $(CFLAGS)

# limpe os arquivos temporarios da pasta
clean:
	@echo -e "\tLimpando pasta...\n"
	@rm *.o *.syn *.lex *.asm

# compile de .cpp em arquivo objeto .o
%.o: %.cpp
	@$(GPP) -c -o $@ $< $(CFLAGS)
