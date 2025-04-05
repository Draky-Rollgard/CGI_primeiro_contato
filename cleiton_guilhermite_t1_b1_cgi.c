/*
  Trabalho 1
  
  Aluno: Cleiton da Silva Guilhermite
  Professor: Hirosh Nakamura
    
  Desenvolva um programa que leia o arquivo texto t1_b1_cgi.cabeçalho
  e o arquivo binário t1_b1.bytes escreva seus conteúdos em um programa
  cgi nome_sobrenome_t1_b1_cgi.
  
  Obs.:
  O arquivo de valores conterá a quantidade maior que um cadastro.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct cadastro
{
    char nome[256];
    unsigned short int ano;
    unsigned char mes;
    unsigned char dia;
    float altura;
    char sexo;
    double peso; 

};


//////////////////////// Para leitura do cadastro///////////////////////////////////
void Ler_cadastro(const char* arquivo_cabecalho)
{
    FILE *arquivo = fopen (arquivo_cabecalho, "r");

    if (arquivo == NULL)
    {
        exit(1);
    }
    
    char linha[256];

    if (fgets(linha,sizeof(linha), arquivo)!=NULL)
    {
        printf("%s", linha);
    }

    else
    {
        fclose(arquivo);
        exit(1);
    }

    fclose(arquivo);
    
}
//////////////////////////////////////////////////////////////////////////////////////


////////////////////// Para ler o arquivo binário (struct)/////////////////////////////
void Ler_binario(const char *arquivo_bytes)
{
    FILE *arquivo = fopen(arquivo_bytes, "rb");
    
    if (arquivo == NULL) 
    {
        exit(1);
    }
    
    // fseek e tell pra ver se bate quantidade, disse qye seria mais bytes do que vai nos passar.
    // os dois sao de manipulação de ponteiro em arquivos binários, aí tem também o SEEK_END,SEEK_SET,SEEK_CUR.
    // cada um deles faz uma coisa diferente. end é final cur é atual e set é o começo.

    fseek(arquivo, 0, SEEK_END);
    long int tamanho_arquivo = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);
    int numero_registros = tamanho_arquivo / sizeof(struct cadastro);
    struct cadastro *registros = malloc(tamanho_arquivo);



    if (registros == NULL)
    {
        fclose(arquivo);
        exit(1);
    }

    size_t itens_lidos = fread(registros, sizeof(struct cadastro), numero_registros, arquivo);
    if (itens_lidos != numero_registros)
    {
        perror("Erro ao ler os registros");
        free(registros);
        fclose(arquivo);
        exit(1);
    }
    
    //Verifica se tá lendo certinho
    struct cadastro alunoIFMS_comp4;
    while (fread(&alunoIFMS_comp4, sizeof(struct cadastro), 1, arquivo) == 1) 
    {
        // Processar os dados lidos (neste exemplo, apenas imprimo)
        printf("Nome: %s\n", alunoIFMS_comp4.nome);
        printf("Data de Nascimento: %02u/%02u/%04hu\n", alunoIFMS_comp4.dia, alunoIFMS_comp4.mes, alunoIFMS_comp4.ano);
        printf("Altura: %.2f\n", alunoIFMS_comp4.altura);
        printf("Sexo: %c\n", alunoIFMS_comp4.sexo);
        printf("Peso: %.2f\n", alunoIFMS_comp4.peso);
        printf("\n");
    }
    

    fclose(arquivo);
    free(registros);
}
//////////////////////////////////////////////////////////////////////////////////////








/////////////////////// Para imprimir html ////////////////////////////////////////////
void gerar_html(const char *arquivo_bytes) 
{
    FILE *arquivo = fopen(arquivo_bytes, "rb");
    int aux;

    //VALIDAR ARQUIVO
    if (arquivo == NULL) 
    {
        exit(1);
    }

    // Validar html
    FILE *html = fopen("saida.html", "w");
    if (html == NULL) 
    {
        fclose(arquivo);
        exit(1);
    }

    //Abertura CABECALHO E TABELA
    //PASSAR html(IMPRIMINDO)
    fprintf(html, "<html>\n\t<head>\n\t\t<title>\n\t\t</title>\n\t</head>\n\t<body>\n\t\t<table border=\"1\">\n");

    FILE *cabecalho = fopen("t1_b1_cgi.cabecalho", "r");

    if (cabecalho == NULL)
    {
        printf("Erro ao abrir o arquivo de cabecalho.\n");
        if (html) 
            fclose(html);
        return;
    }

    char linha[1024];  // Buffer para armazenar a linha

    //Quebrar a linha em strings e escrever cada uma das strings em células
    fgets(linha, sizeof(linha), cabecalho);
    fprintf(html, "\t\t\t<tr>\n");
    char *token = strtok(linha, ",");
    while (token != NULL) 
    {
        fprintf(html, "\t\t\t\t<td>\n");
        fprintf(html, "\t\t\t%s\n", token);
        fprintf(html, "\t\t\t\t</td>\n");
        token = strtok(NULL, ",");
    }
    fprintf(html, "\t\t\t</tr>\n");

    fclose(cabecalho);

    //CORPO
    // IMPRIMIR STRUCT no html
    struct cadastro pessoa;
    while (fread(&pessoa, sizeof(struct cadastro), 1, arquivo)) {
        fprintf(html, "\t\t\t<tr>\n\t\t\t\t<td>\n");
        fprintf(html, "\t\t\t%s\n\t\t\t\t</td>", pessoa.nome);
        fprintf(html, "\n\t\t\t\t<td>\n\t\t\t%02u/%02u/%04hu\n\t\t\t\t</td>\n", pessoa.dia, pessoa.mes, pessoa.ano);
        aux = pessoa.altura;        
        fprintf(html, "\n\t\t\t\t<td>\n\t\t\t%i,%02.0f\n\t\t\t\t</td>\n", aux, (pessoa.altura - aux) * 100);
        fprintf(html, "\n\t\t\t\t<td>\n\t\t\t%c\n\t\t\t\t</td>\n", pessoa.sexo);
        aux = pessoa.peso;
        fprintf(html, "\n\t\t\t\t<td>\n\t\t\t%i,%02.0f KG\n\t\t\t\t</td>\n\t\t\t</tr>\n", aux, (pessoa.peso - aux) * 100);
    }

    //FECHANTO tudo
    fprintf(html, "    </table>\n");
    fprintf(html, "  </body>\n");
    fprintf(html, "</html>\n");

    fclose(arquivo);
    fclose(html);
}
///////////////////////////////////////////////////////////////////////////////////////




///////////////////////MAIN///////////////////////////////////////////////////////////
int main()
{
    const char *cabecalho = "t1_b1_cgi.cabecalho";
    const char *bytes = "t1_b1_cgi.bytes";

    Ler_cadastro(cabecalho);
    Ler_binario(bytes);
    gerar_html(bytes);

    return 0;
}









