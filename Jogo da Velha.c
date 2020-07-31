#define DIM 3
#define TamPop 160000
#define TaxMut 20
#define maxx 9
#define NumInd 10
#define partidas 5 //Numero de jogadores que serão criados
#define tentativa 9
#define avaliacoes 4 // Número de avaliações realizadas para cada indivíduo na função avalia
#define geracoes 20 // Quantidade de gerações desejadas

#include <stdio.h>  // Printf, sprintf
#include <stdlib.h> // Rand
#include <math.h>
#include <time.h>
#include <locale.h>

//Variáveis globais utilizadas
int    map[partidas+1][tentativa];
int    vitoria1 = 0;
int    vitoria2 = 0;
int    empate;
int    vez;
int    partida = 1;
int    v1,v2  = 0;
int    maxi = 0;
int    posicaofinal = 0;
int    i = 1;
int    p = 0;
int    b = 1;
int    a = 1;
int    c = 1;
int    q,w,t,y,u = 0;
int    torneiobriga;
int    teste1 = 2;
int    teste2 = 2;
int    change = 0;
int    ind1[TamPop];
int    ind2[TamPop];
int    jogador1 [partidas+1][6];
int    jogador2 [partidas+1][6];
int    fit[partidas+1];
int    maxfit = 0;
int    gen = 1;
int    pai1;
int    pai2;
int    s;
int    apocalipse = 0;
float  media = 0.0;
int    m;
int    fitord[partidas+1];
int    temp;

//Funções utilizadas
int menu(void);
void clear(void);
void zeraTabuleiro(int tabuleiro[][DIM]);
void exibeTabuleiro(int tabuleiro[][DIM]);
void jogar(int tabuleiro[][DIM]);
int checaLocal(int tabuleiro[][DIM], int linha, int coluna);
int checaLinha(int tabuleiro[][DIM]);
int checaColuna(int tabuleiro[][DIM]);
int checaDiagonal(int tabuleiro[][DIM]);
int checaEmpate(int tabuleiro[][DIM]);
int checaTermino(int tabuleiro[][DIM], int vez);
void jogada(int tabuleiro[][DIM]);
void avalia();
void initpop();
void torneio();
void genocidio();


int main(void)
{
    //Limpa o arquivo caso txt

        FILE *salvafit;
        salvafit = fopen("Fit.txt", "w");
        fprintf(salvafit,"");
        fclose(salvafit);

    //----------CRIAÇÃO DE INDIVÍDUOS----------//
    setlocale(LC_ALL,"");
    fflush(stdin);
    fflush(stdout);

    srand(time(NULL));
    maxi = 0;

    int i,j;

    do{
        initpop();

        int tabuleiro[DIM][DIM];

        vez=1;

            printf("Partida %d\n", partida);
            jogar(tabuleiro);
            posicaofinal = 0;

            partida++;

    }while(partida <= partidas);

    //Mostra a relação de vitorias das partidas
    printf("O jogador 1 venceu %d rodadas\n", vitoria1);
    printf("O jogador 2 venceu %d rodadas\n", vitoria2);
    printf("Houveram %d empates", empate);

    printf("\n");

    //Separa a partida nas jogadas de cada jogador
    for(i=1;i<=partidas;i++){
         v1 = v2 = 1;
        for(j=1;j<=tentativa;j++){
            if(j%2){
                v1++;
                if(map[i][j] == 0 || map[i][j] == 10){
                    break;
                }

                else {
                        jogador1[i][v1] = map[i][j];
                }
            }

            else{
                v2++;
                if(map[i][j] == 0 || map[i][j] == 10){
                    break;
                }

                else {
                    jogador2[i][v2] = map[i][j];
                }
            }

         }
    }

    i=1;

    printf("\n");

    //Printa a partdida e a jogada de cada jogador
    for(i=1;i<=partidas;i++){
            j = 1;
            for(j=1;j<=tentativa;j++){
                printf("%d ", map[i][j]);
            }
            printf("\t");

            j = 1;
             for(j=2;j<=6;j++){

                printf("%d ", jogador1[i][j]);

            }
            printf("\t");

            j = 1;
            for(j=2;j<=5;j++){

                printf("%d ", jogador2[i][j]);
            }

            printf("\n");
    }

    //----------FIM DA CRIAÇÃO DOS INDIVÍDUOS----------//
    printf("\n Pressione Enter para começar o algorítmo genético: ");
    getchar(); // pausa antes de começar a evoluir
    maxi = 0;

//Evolução do jogador 1
  AG:
    //Estipula um prazo para genocidio
    if(apocalipse == 15 && media<8.0){
        printf("********************GENOCÍDIO********************");
        genocidio();
        apocalipse = 0;
    }
    //

    printf("\n\n//----------Geracao %d----------\\\n\n", gen);

    avalia(); //Faz uma sequência de partidas com o jogador e atribui uma nota para cada uma

    torneio(); // Sorteia duas jogadas do jogador um, realiza uma partida entre eles e elimina o mais fraco

    //Printa o jogador1 após uma geração
    for(i=1;i<=partidas;i++){
            for(j=2;j<=6;j++){
                printf("%d ", jogador1[i][j]);
            }
            printf("\n");
    }

    //getchar();

    printf("\n");

    //Printa o fitness do jogador1 após uma geração
    for(i=1;i<=partidas;i++){
        printf("%d\t", fit[i]);
    }

    //getchar();

    printf("\n\nO melhor fit desta geracao foi %d", fit[maxi]);
    printf("\n\nO melhor fit de todos foi %d", maxfit);
    printf("\n\nA posicao é %d", maxi);
    printf("\n\nPressione Enter para próxima geração: \n\n");

    gen++;
    apocalipse++;

    printf("\n");

    //Salva os dados em um arquivo .txt para plotar gráficos
    for(s=1;s<=partidas;s++){
        salvafit = fopen("Fit.txt", "a");
        fprintf(salvafit,"%d ", fit[s]);
        fclose(salvafit);
    }
        salvafit = fopen("Fit.txt", "a");
        fprintf(salvafit,"\n");
        fclose(salvafit);


    //Calcula a média da populção
    for(m=1;m<=partidas;m++){
        media = media + fit[m];
    }
        media = media/partidas;

    //Zera o fit de toda população, exceto do melhor, antes de uma nova avaliação
    for(i=1;i<=partidas;i++){
        if(i == maxi){
            continue;
        }
       else{
        fit[i] = 0;
       }
    }

    if (gen<=geracoes){
        printf("media = %f", media);
        media = 0.0;
        fflush(salvafit);
        getchar();
        goto AG;
    }

    printf("\n");

    //Função de controle para ver a população após evoluir
    /*if(gen>partidas){
        for(i=1;i<=partidas;i++){
            for(j=2;j<=6;j++){
                printf("%d", jogador1[i][j]);
            }
            printf("\n");
        }

        getchar();
        vitoria1 = 0;
        vitoria2 = 0;
        empate = 0;
        change = 3;
        partida = 1;

        do{
        initpop();
        teste1 = 2;
        teste2 = 2;

        int tabuleiro[DIM][DIM];

        vez=1;

            printf("Partida %d\n", partida);
            jogar(tabuleiro);
            posicaofinal = 0;

            partida++;

    }while(partida <= partidas);

    printf("O jogador 1 venceu %d rodadas\n", vitoria1);
    printf("O jogador 2 venceu %d rodadas\n", vitoria2);
    printf("Houveram %d empates", empate);

    }*/


    return 0;
}

void clear(void)
{
    int count=0;

    while(count != 10)
    {
        putchar('\n');
        count++;
    }
}

void zeraTabuleiro(int tabuleiro[][DIM])
{
    int linha, coluna;
    for(linha = 0 ; linha < DIM ; linha++)
        for(coluna = 0 ; coluna < DIM ; coluna++)
            tabuleiro[linha][coluna] = 0;
}

void exibeTabuleiro(int tabuleiro[][DIM])
{
    int linha, coluna;
    putchar('\n');

    for(linha = 0 ; linha < DIM ; linha++)
    {
        for(coluna = 0 ; coluna < DIM ; coluna++)
        {
            if(tabuleiro[linha][coluna] == 0)
                printf("    ");
            else
                if(tabuleiro[linha][coluna] == 1)
                    printf("  X ");
                else
                    printf("  O ");

            if(coluna != (DIM-1))
                printf("|");
        }
        putchar('\n');
    }
    putchar('\n');
}

void jogar(int tabuleiro[][DIM])
{

    int continua;
    zeraTabuleiro(tabuleiro);

    do{
        clear();
        exibeTabuleiro(tabuleiro);
        jogada(tabuleiro);

    }while(checaTermino(tabuleiro, vez) != 1);
}

int checaLocal(int tabuleiro[][DIM], int linha, int coluna)
{
    if(linha < 0 || linha > (DIM-1) || coluna < 0 || coluna > (DIM-1) || tabuleiro[linha][coluna] != 0)
        return 0;
    else
        return 1;
}

int checaLinha(int tabuleiro[][DIM])
{
    int linha, coluna,
        soma;

    for(linha = 0 ; linha < DIM ; linha++)
    {
        soma=0;

        for(coluna = 0 ; coluna < DIM ; coluna++)
            soma += tabuleiro[linha][coluna];

        if(soma==DIM || soma == (-1)*DIM)
            return 1;
    }

    return 0;
}

int checaColuna(int tabuleiro[][DIM])
{
    int linha, coluna,
        soma;


    for(coluna = 0 ; coluna < DIM ; coluna++)
    {
        soma=0;

        for(linha = 0 ; linha < DIM ; linha++)
            soma += tabuleiro[linha][coluna];

        if(soma==DIM || soma == (-1)*DIM)
            return 1;
    }

    return 0;
}

int checaDiagonal(int tabuleiro[][DIM])
{
    int linha,
        diagonal_principal=0,
        diagonal_secundaria=0;

    for(linha = 0 ; linha < DIM ; linha++)
    {
        diagonal_principal += tabuleiro[linha][linha];
        diagonal_secundaria += tabuleiro[linha][DIM-linha-1];
    }

    if(diagonal_principal==DIM || diagonal_principal==(-1)*DIM ||
       diagonal_secundaria==DIM || diagonal_secundaria==(-1)*DIM)
       return 1;

    return 0;
}

int checaEmpate(int tabuleiro[][DIM])
{
    int linha, coluna;

    for(linha = 0 ; linha < DIM ; linha++)
        for(coluna = 0 ; coluna < DIM ; coluna++)
            if(tabuleiro[linha][coluna] == 0)
                return 0;

    return 1;
}

int checaTermino(int tabuleiro[][DIM], int vez)
{
    if(checaLinha(tabuleiro)){

        if(change == 0 || change == 3){
            if((vez%2)+1 == 1){
                vitoria1++;
            }
             if((vez%2)+1 == 2){
                vitoria2++;
            }
            if(posicaofinal<=tentativa){
                map[partida][posicaofinal] = 10;
            }
       }

        if(change == 1){
            if((vez%2)+1 == 1){
                fit[b]= fit[b]+2;
            }
            if((vez%2)+1 == 2){
                fit[b]--;
            }
        }

        if (change == 2){
            if((vez%2)+1 == 1){
                torneiobriga = 11;
            }
            if((vez%2)+1 == 2){
                torneiobriga = 12;
            }
        }


        printf("\n\nJogo encerrado. Jogador %d venceu !\n\n", (vez%2)+1);
        exibeTabuleiro(tabuleiro);
        return 1;
    }

    if(checaColuna(tabuleiro)){
        if(change == 0 || change == 3){
            if((vez%2)+1 == 1){
                vitoria1++;
                if(posicaofinal<=tentativa){
                map[partida][posicaofinal] = 10;
            }
            }
             if((vez%2)+1 == 2){
                vitoria2++;
                if(posicaofinal<=tentativa){
                map[partida][posicaofinal] = 10;
            }
            }
        }

        if (change == 1){
            if((vez%2)+1 == 1){
                fit[b]= fit[b]+2;
            }
            if((vez%2)+1 == 2){
                fit[b]--;
            }
        }

        if (change == 2){
            if((vez%2)+1 == 1){
                torneiobriga = 11;
            }
            if((vez%2)+1 == 2){
                torneiobriga = 12;
            }
        }

        printf("\nJogo encerrado. Jogador %d venceu !\n\n", (vez%2)+1);
        exibeTabuleiro(tabuleiro);
        return 1;
    }

    if(checaDiagonal(tabuleiro)){
       if(change == 0 || change == 3){
            if((vez%2)+1 == 1){
                vitoria1++;
                if(posicaofinal<=tentativa){
                map[partida][posicaofinal] = 10;
            }
            }
             if((vez%2)+1 == 2){
                vitoria2++;
                if(posicaofinal<=tentativa){
                map[partida][posicaofinal] = 10;
                }
            }
        }

        if(change == 1){
            if((vez%2)+1 == 1){
                fit[b]= fit[b]+2;
            }
            if((vez%2)+1 == 2){
                fit[b]--;
            }
        }

        if (change == 2){
            if((vez%2)+1 == 1){
                torneiobriga = 11;
            }
            if((vez%2)+1 == 2){
                torneiobriga = 12;
            }
        }

        printf("\nJogo encerrado. Jogador %d venceu !\n\n", (vez%2)+1);
        exibeTabuleiro(tabuleiro);
        return 1;
    }

    if(checaEmpate(tabuleiro)){
        if(change == 0 || change == 3){
            empate ++;
            if(posicaofinal<=tentativa){
                map[partida][posicaofinal] = 10;
            }
        }
        if (change == 1){
            fit[b] = fit[b] + 1;
        }

        if (change == 2){
                torneiobriga = 13;
        }

        printf("\nJogo encerrado. Ocorreu um empate!\n\n");
        exibeTabuleiro(tabuleiro);
        return 1;
    }

    if(vez>1 && p == 0 || i  == 7){
            if(change == 1 || change == 3){
                fit[b] = fit[b] + 0;
            }

            if (change == 2){
                torneiobriga = 13;
            }

            printf("\n\nJogo encerrado. Falta de jogadas!\n\n");
            return 1;
        }

    return 0;
}

void jogada(int tabuleiro[][DIM])
{
    int linha, coluna;
    vez++;
    posicaofinal = vez;
    if(change == 1 && vez == 2){
        teste1 = 2;
    }
    printf("\n--> Jogador %d \n", (vez % 2) + 1);

    do{
        if((vez % 2) + 1 == 1){

           if(change == 0){ //Utilizada para a criação dos indivíduos
               p = ind1[teste1];
               teste1++;
           }

           if(change == 1){ //Utilizada para a função avalia
                p = jogador1[b][teste1];
                teste1++;
           }

           if(change == 2){ //Utilizada para a função torneio
                p = jogador1[pai1][teste1];
                teste1++;
           }

           if(change == 3){  //Utilizada para a função de controle (última em 'main')
                p = jogador1[partida][teste1];
                teste1++;
           }

           printf("%d", p);
        }

        if((vez % 2) + 1 == 2){

            if(change == 0 || change == 1 || change == 3){
                    p = ind2[teste2];
                    teste2++;
            }

            if(change == 2){
                p = jogador1[pai2][teste2];
                teste2++;
           }

           printf("%d", p);
        }

        //Se quiser jogar contra a máquina descomente abaixo a ajuste a parte de cima
        /*if((vez % 2) + 1 == 2){
                printf("Escolha uma posicao de 1 a 9 \n");
                scanf("%d", &p);
                }*/

        switch(p){
            case 1:
                linha = 0;
                coluna = 0;
                break;
            case 2:
                linha = 0;
                coluna = 1;
                break;
            case 3:
                linha = 0;
                coluna = 2;
                break;
            case 4:
                linha = 1;
                coluna = 0;
                break;
            case 5:
                linha = 1;
                coluna = 1;
                break;
            case 6:
                linha = 1;
                coluna = 2;
                break;
            case 7:
                linha = 2;
                coluna = 0;
                break;
            case 8:
                linha = 2;
                coluna = 1;
                break;
            case 9:
                linha = 2;
                coluna = 2;
                break;

            default:
                printf("Nao existe essa posicao");

            return 0;
        }


        if(checaLocal(tabuleiro, linha, coluna) == 0)
            printf("\nPosicao ocupada ou inexistente, escolha outra.\n");

    } while(checaLocal(tabuleiro, linha, coluna) == 0);


    if(vez%2){
        if(change == 0){
            map[partida][vez-1] = p; //salva as partidas em uma tabela
        }
        tabuleiro[linha][coluna] = -1;

    }

    else{
        if(change ==0){
            map[partida][vez-1] = p; //salva as partidas em uma tabela
        }
        tabuleiro[linha][coluna] = 1;

    }

}

//ALGORITMO EVOLUTIVO
void initpop(){

    for (i=1;i<=TamPop;i++){
        ind1[i] = (1+(rand()%maxx));
        //printf("%d", ind1[i]);
    }

    for (i=1;i<=TamPop;i++){
        ind2[i] = (1+(rand()%maxx));
       //printf("%d", ind2[i]);
    }

    printf("\n");
}

 void avalia(){
    printf("//----------INICIO DA AVALIAÇÃO----------\\ %d",maxi);
    change = 1;

    for(b=1;b<=partidas;b++){
        printf("\n\n||maxi=%d b=%d||\n\n", maxi, b);
        if(b == maxi){
            continue;//Não avalia o melhor de todos até que vire outro
        }
        if(b != maxi){
            int tabuleiro[DIM][DIM];
            vez = 1;
            for(a=1;a<=avaliacoes;a++){
                initpop();
                vez = 1;
                teste1 = 2;
                teste2 = 2;
                jogar(tabuleiro);
            }
        }
    }

    printf("\n");

}

void torneio(){
    printf("//----------INICIO DO TORNEIO----------\\");

    int j;
    maxfit = fit[1];
    maxi   = 1;
    for (i=2;i<=partidas;i++) // Busca pelo melhor individuo para protege-lo
    {
        if (fit[i]>maxfit)
        {
            maxfit = fit[i];
            maxi = i;
        }
    }

    //Escolhe uma posição para ocupar o vencedor do torneio protegendo o melhor individuo
    t=0;
    do{
        t = (rand() %partidas) + 1;
    }while(t == maxi);

        // Sorteia dois individuos para 1º torneio
        pai1 = 0;
        q = (rand() %partidas) + 1;
        w = (rand() %partidas) + 1;
        if (fit[q] > fit[w])
            pai1 = q;
        else
            pai1 = w;

            printf("\n O pai 1 é %d", pai1);

        // Sorteia mais dois individuos para 2º torneio
        pai2 = 0;
        y = (rand() %partidas) + 1;
        u = (rand() %partidas) + 1;
        if (fit[q] > fit[w])
            pai2 = y;
        else
            pai2 = u;

            printf("\n O pai 2 é %d", pai2);

            fflush(stdin);

        // Realiza uma partida para determinar o melhor
        int tabuleiro[DIM][DIM];
        torneiobriga = 0;
        change = 2;
        vez = 1;
        teste1 = 2;
        teste2 = 2;
        jogar(tabuleiro);

        if (torneiobriga == 11){
            for(j=2;j<=6;j++){
                jogador1[t][j] = jogador1[pai1][j];
            }
        }

        if (torneiobriga == 12){
            for(j=2;j<=6;j++){
                jogador1[t][j] = jogador1[pai2][j];
            }
        }

        if (torneiobriga == 13){
            for(j=2;j<=6;j++){
                jogador1[t][j] = jogador1[t][j];
            }
        }

}

void genocidio(){
    int g,h;
    int fitord[partidas+1];
    int i, j,temp;
    int maxi2,maxi3;

    //Para separar os 3 melhores e não perde-los
   /* for(i=1;i<=partidas;i++){
        fitord[i] = fit[i];
    }

    for ( i = 0; i < partidas ; i++)
    {
        for ( j = i; j < partidas; j++)
        {
            if (fitord[i] < fitord[j])
            {
                 temp = fitord[i];
                fitord[i] = fitord[j];
                fitord[j] = temp;
            }
        }
    }

    for(i=1;i<=partidas;i++){
        if(fit[i] == fitord[2]){
            maxi2 = i;
        }
        if(fit[i] == fitord[3]){
            maxi3 = i;
        }
    }*/

    //Substitui os indivíduos com exceção dos 3 melhores
    for(g=1;g<=partidas;g++){
        if(g == maxi /*|| g == maxi2 || g == maxi3*/){
            continue;
        }
        for(h=2;h<=6;h++){
        jogador1[g][h] = (1+(rand()%maxx));
        }
    }
}
