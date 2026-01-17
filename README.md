Passo 1 – Preparar o servidor para ACK

Registrar o handler com sigaction para pegar o si_pid (PID do cliente que enviou o sinal):

Isso permite ao servidor saber para qual PID enviar o ACK.

Exemplo: sigaction(SIGUSR1, &sa, NULL); sigaction(SIGUSR2, &sa, NULL);

Guardar o PID do cliente

No seu handler, você precisa de algo como static pid_t client_pid = 0;

Quando começar a receber sinais da “mensagem” (ou do PID enviado), armazene info->si_pid no client_pid.

Enviar ACK por caractere

Depois que ft_charhanlder retorna true (caractere completo):

kill(client_pid, SIGUSR1); // envia ACK



Passo 2 – Preparar o cliente para receber ACK

Declarar flag global para ACK:

volatile sig_atomic_t ack_received = 0;


Criar handler de ACK:

void ack_handler(int sig)
{
    if(sig == SIGUSR1)
        ack_received = 1;
}


Registrar handler:

signal(SIGUSR1, ack_handler);


Esperar ACK entre cada caractere

No seu ft_builchar (ou equivalente), depois de enviar os 8 bits de um caractere:

ack_received = 0;
while (!ack_received)
    pause(); // espera o ACK do servidor




Passo 3 – Enviar o PID do cliente (opcional, mas recomendado)

Antes de enviar a mensagem, envie getpid() do cliente bit a bit, como se fosse a “primeira mensagem”.

O servidor recebe esses bits e guarda em client_pid.

Depois disso, todos os ACKs podem ser enviados para esse PID.

Observação: se você não enviar o PID, o servidor só vai conseguir mandar ACK se você usar sigaction e info->si_pid em cada sinal, o que funciona, mas enviar explicitamente o PID é mais seguro para sincronia.




Passo 4 – Ajustar envio de mensagem

Para cada caractere enviado no cliente:

Enviar 8 bits (SIGUSR1/SIGUSR2)

Esperar ACK do servidor antes de enviar o próximo

Para o tamanho da mensagem (ft_getlen): você também pode esperar ACKs por cada 32 bits enviados (opcional, mas mantém confiabilidade).


------------------------------------------------------------------------------------

    CORRECAO DO MINITALK ANTIGO E BUGADO 

        - problema gigante com o meu putmsg !!
                - estava a usar um putstr para por a minha msg...
                    void ft_putmsg(t_struct *s);
                    {
                        ....
                        ft_putstr(s->msg);
                        ....
                    }
                    erro grave...
                    agora:
                        - uso write(1, s->msg, s->len);
                                o len e sagrado , sem ele , msg gigantes estavam a gerar bugs;
                        - verficao para saber se a msg ja esta alocada!
                        - apos o final do processo por toda a msg a 0 com  memset(s->msg, 0, s->len + 1);
        - problemas com reset de var:
            - 2 funcoes a resetar a msms string --- WTF.
                - AGORA :
                    =1 funcao responsavel por resetar as minhas vars :
                        - condicao de seguranca com if(s->msg) antes de resetar so pq sim e s->msg[s-j] = NULL;
            - forma de printar msg esta diferente !
                - antes enquanto esta condicao n fosse verdade (j == len), ia enchendo o meu buffer com o c;
                AGORA:
                    - condicao de seguranca ao encher o buffer 
                        if(s.j < s.len)
                                s.msg[s.j++] = s.c;
                            - reseto a msm o meu c;
                        - para entao quando vir que j === len.
                            - ft_putmsg(msg);
------------------------------------------------------------------------------------

PARA O BONUS:
    - o client vai ter de esperar por confirmacao!:
    - o server vai enviar um sinal de volta apos receber a msg (ou char).

        -   SERVER:
            - enviar um kill(c_pid, SIGUSR2); 
        -  CLIENT:
            - apos o envio de um char/bit. 
                - usa pause / sigwaitinfo(); - para esperar o ack.
        
        - n devo usar sleep para sincronizacao.
        - usar signaction(), e nao signal desta vez.
        - race condictions (ACK antes do pause -> FLAG GLOBAL.)

        FLUX0:

            client -> envia 8 sinais
            server -> monta byte
            server -> ACK
            client -> próximo byte


static void ft_singalsetup(void) // clareza -- no lugar de estar a por na main
{
    struct sigaction sa;

    sa.sa_sigaction = ft_serverhandler; // definir o handler.
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR1, &sa, NULL); 
    sigaction(SIGUSR2, &sa, NULL);
}

-  struct sigaction sa; == declarar a var sa. -- tipo strcut sigaction
    - contem campos - qual funcao sera chamada quando o sinal chegar - HANDLER.
    - flags etc.
-   sa.sa_sigaction = ft_serverhandler; // definir o handler.
    - uso de sa_sigaction == info extra:
                            - PID do client.
    -  definir a minha flag = sa.sa_flags = SA_SIGINFO;
   - sigemptyset(&sa.sa_mask) - quando um sinal chega , o kernel chama o handler.
    - caso outro sinal chegar,enquanto o handler funciona, pode interromper o handler 
    e ao fazer sigemptyset(&sa.sa_mask) -- limpamos a mascara - nenhum sinal e bloqueado durante a execuao do handler.

      sigaction(SIGUSR1, &sa, NULL); 
    sigaction(SIGUSR2, &sa, NULL); == associar sinais .


void ft_serverhandler(int sig, siginfo_t *info, void *ctx)
{
    (void)ctx;
    static t_struct s; // minha struct.

    
}
/*

chave - info->si_pid. // pid do client.
    - PID - info
    - UID - id do user --- info.
    - (void) ctx -- context do processo no momento em que o sinal chegou:
        - registros?
*/
------------------------------------------------------------------------------------


FLUXO
    -- vai ser , o client gera o len , apos o envio (pause).
        - o server recebe o len - ACK.
    --  o client envia o char , apos o envio , pause();
        - o server recebe os chars (ou apos a msg) - ACK.

REGRA DE OURO DOS SINAIS

❗ Nunca usar pause() sem garantir que o sinal ainda não chegou

pause() não guarda sinais passados....
------------------------------------------------------------------------------------

bug do dia:
    -- usar char no lugar de unsigned char -...- -- char da morte kkk
    na contagem , evitar usar char para n entrar em range negativo.
    
BUG DO DIA 2:

paizao@PcDoPaizao:~/42/Arank02/miniTalk$ ./clientbonus 9838  ""
paizao@PcDoPaizao:~/42/Arank02/miniTalk$ ./clientbonus 9838  ""
paizao@PcDoPaizao:~/42/Arank02/miniTalk$ ./clientbonus 9838  ""
paizao@PcDoPaizao:~/42/Arank02/miniTalk$ ./clientbonus 9838  ""
paizao@PcDoPaizao:~/42/Arank02/miniTalk$ ./clientbonus 9838  "o"
paizao@PcDoPaizao:~/42/Arank02/miniTalk$ ./clientbonus 9838  "o"




paizao@PcDoPaizao:~/42/Arank02/miniTalk$ ./serverbonus 
PID: 9838
________________TEXT_ZONE________________
7 --- -WTFFFF

-- resposta ... n existe em si o byte 0 para "".
    
    resolvido ... confirmar se o len == 0,  resetar as vars e returrn 1.

------------------------------------------------------------------------------------
README.MD:

PROJETO FINALIZADO:
    - erro anterior:
        - passar o mini_talk antigo como se fosse uma versao base
    - hoje
        - usar somente 1 mini talk .. onde nesse ha o bonus.

    
CLIENT SIDE (BONUS):

    = mesmo conceito
            - client pega os 2 args:
                - PID + MSG 
                - pid -> precisa ser verificado 
                            -> se for, passa para o processamento de bits da msg.
                            -> se n , msg de erro , encerra o processo.
                1: processar o len da msg ate 31 bits
                        -> do mais significativo ao menor:
                                -> ft_lenprocess resolve isso:
                                    - caso o meu len for 0===  envia um SIUGSR1 (0)
                                    -> se n , processa bit a bit;
                                        -> se o meu bit for 1, envia SIGUSR2.
                                            -> se n , envia 0 (sig1);
                                -> no bonus a questao e ter um ACK do server para o clinet:
                                    -> a grande razao: -> evitar que haja perca de bits no processo.
                                    -> logo apos o meu envio , dar um pause();

                                ( PAUSE () VS USLEEP)
                                {
                                    pause -> congela o processo e so continua quando o recebe QUALQUER sinal;
                                    usleep -> a cada bit enviado , gero uma "pausa" apos cada bit enviado 
                                            -> bom para evitar perca de bits , mas depedendo do tamanho da msg;
                                                -> maior sera a margem de erro;
                                }

                -> no fim enviamos o len primeiro para o server estar preparado para entender o tamanho da msg
                                -> lembrando ... a ideia do projeto e ele "vomitar" a msg e n por char a char .
        
                2:  processar o chars para envio:
                                -> apos o envio do len ... entender quais sao os chars que estamos a enviar:
                                -> memsa logica:
                                    -> um index que acompanha o meu len:
                                        ->uma var que vai percorrer o meu bit mais significado (MSB)-> ate ao menor;
                                            -> msm logica ... os bits ativos (1) envia um sig2 , se n , sig1;
                                                -> apos cada bit... msm logica ... dar um pause... b--; 
                                        
SERVER SIDE (BONUS):
        -> o server papel obrigatorio:
                                -> mal inicializa -> amostrar o pid:
                                        -> atraves do comando getpid();
        -> apos isso ... loop infinito:
                -> fica a espera de , atraves do seu pid (que o client contem), receber um sinal.
            -> no caso a "engine" principal do meu server esta  no serverhanlder:
                -> ou seja , processa os bits ( len , char) -> para no fim ,amostrar a msg.
                    -> primeiro:
                        ->void	ft_serverhandler(int sig, siginfo_t *info, void *ctx)
                            -> sig == sinal que estou a receber;
                            -> signfo_t info -> info extras que podemos utilizar gracas ao sigaction... no caso
                                    -> -> CLIENT PID (info->si_pid);
                                    -> ctx .. informacoes extras de como estava por ex o sistema ao receber x sinal ... n vamos utilizar void (ctx);
                ... parte principal:
                    -> processar a len ... lembrando , o meu client vai enviar os bits
                         da -> -> -> .(para a esquerda)
                    -> o server recebe <- <- <- (para a direita)
                        -> comecamos por, mais uma vez, processar os 32 bits,
                            -> len da shifts de 1 posicao para a esquerda , (len sempre sera == 0), abrindo espaco para colocarmos os bits que estao a ser enviados.
                        -> sempre que recebo bit == 1(SIG2), por 1,se n, 0;
                            -> a medida que vou recebendo a minha var global(g_bits) incrementa;
                            # RAZAO DE USO -> ja tinha muitas vars locais kkk
                            -> quando g_bits chegar aos 32 -- o meu len esta completo:
                                -> verificar se o len processado e de valor 0 
                                        -> se sim ... RESET LEN + G_BITS ... return 1;
                                        -> se n , enviar a strutura para o msghanlder.
                            ->caso o lenhandler der certo -> ACK para o client:
                                -> tudo isso gracas ao info_sipid.(literlamete o pid de quem enviou...) -> apos o envio , o client sai do pause e o cicloc continua ... processa , envia , congela , recebe, "calcula" , associa (se e valido ou n), -> ACK.
                            -> processar o CHAR !..
                    -> msm logica ... processa para <- <- <- (para a direita)
                        -> verfica quem e 1 ou 0... quando o i for 8 ( 1 byte), receta.
                        -> apos processar os chars + len o server ja esta pronto para
                                -> PROCESSAR A MSG:
                                    -> um index com valor 0 vai percorrer ate len.
                                        -> msg recebe cada char (c) processado;
                                    *apos a associacao recetar c = 0 sempre.
                                    -> e por fim , apos processar len , char , associar ...
                                        -> putmsg ... resetar as vars para voltar ao msm processo... loop infinito.

---------------------------------------------
unicode e 32 len

Por que usar 32 bits para o len?

No minitalk, os sinais transmitem apenas bits (0 ou 1).
Antes de enviar a mensagem em si, o cliente envia o tamanho da mensagem (len) codificado em 32 bits.

Ou seja:

👉 Primeiro chegam 32 sinais que representam um número inteiro.
👉 Esse número indica quantos bytes o servidor deve esperar receber.

Exemplo:

00000000 00000000 00000000 00001010


Isso em binário = 10 → a mensagem terá 10 bytes.

Isso funciona como um header, exatamente como em protocolos de rede.


unicode :
| Caractere | UTF-8 bytes |
| --------- | ----------- |
| `A`       | 1 byte      |
| `ç`       | 2 bytes     |
| `€`       | 3 bytes     |
| `你       |  3 byte    |
| `🔥`      | 4 bytes     |

    - como cada char pode ocupar diferetes tamanho de byte ... passar logo os 32 bits facilita a interpretacao da msg .... se contem "o: ou "🔥" tanto faz afinal , tudo cabe dentro do range.

1. Tamanho máximo previsível

Com 32 bits, podemos representar até:

2³² = (~4GB)

Muito maior do que qualquer mensagem real do projeto.
---------------------------------------------

configuracao do sigcaction:

static void	ft_singalsetup(void)
{
	struct sigaction	sa; -> var da configuracao do sig.

	sa.sa_sigaction = ft_serverhandler; -> onde sera interpretado
	sa.sa_flags = SA_SIGINFO; -> FLAG -> sa_siginfo-> PID DO CLIENT.
	sigemptyset(&sa.sa_mask); ->  null.
	sigaction(SIGUSR1, &sa, NULL); ->sinais
	sigaction(SIGUSR2, &sa, NULL); -> . . 
} 