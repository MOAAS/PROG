Todos os objetivos foram cumpridos. Ambos os programas realizam todas as funcionalidades sugeridas, e ainda outras.
Como indicado, os nomes e tipos das variáveis e funções foram escolhidos com cuidado.
Todas as funções terão sempre comentários no início, e por vezes no meio, explicando o funcionamento da mesma e das suas variáveis.
O programa foi pensado para que não tivesse problema algum com input incorreto do utilizador.
Todos os "Warnings" do compilador foram resolvidos.

Nas funcionalidades acrescentadas, incluem-se:
- Class Cursor:
 - Esta classe foi feita com o objetivo de auxiliar na manipulação de um tabuleiro.
 - O cursor pode ser representado por duas coordenadas (x, y) e uma direção (H, V).
 - O cursor consegue receber coordenadas do tipo "LcD" e mover-se para essa posição.
 - O cursor dá overload ao operador ++ (andar para a frente) e -- (para trás) para que a implementação da classe board seja independente da direção das palavras.
- No cwplayer, o utilizador pode voltar atrás com Ctrl-Z, caso se tenha enganado na posição que escolheu.
- Existe uma função que determina as palavras que foram acrescentadas acidentalmente, ao adicionar outras no tabuleiro, função usada tando no cwcreator como no cwplayer.
- Dá para colocar palavras em cima de outras que já estão lá, tanto no cwcreator como no cwplayer
- Na criação de um Board, o programa determina automaticamente qual o número do Board, pelos ficheiros que já existem.
- A maior parte do input é Case-insensitive, para facilitar a experiência do utilizador

- Para as pistas, foi acrescentado um sistema de cores para quando são mostradas no ecrã:
 - Vazio: Branco
 - Preenchido: Amarelo
 - Certo: Verde *
 - Errado: Vermelho *
- Para cada pista, também se mostra no ecrã o tamanho da palavra certa, para auxiliar o utilizador
- Para cada pista, também se mostra no ecrã as letras que já foram colocadas.
* - Apenas aparecem se, após encher o tabuleiro, o utilizador pedir. Conta como uma "Clue" extra, e aparecem apenas na altura em que o utilizador pede, voltando depois ao amarelo e branco.
Modo fácil (Easy): 
 - Sempre que o utilizador insere uma palavra, o programa mostra se está correto ou errado
 - Quando se guarda a informação no ficheiro, é acrescentado " | EASY MODE"