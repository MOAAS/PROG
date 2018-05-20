Todos os objetivos foram cumpridos. Ambos os programas realizam todas as funcionalidades sugeridas, e ainda outras.
Como indicado, os nomes e tipos das vari�veis e fun��es foram escolhidos com cuidado.
Todas as fun��es ter�o sempre coment�rios no in�cio, e por vezes no meio, explicando o funcionamento da mesma e das suas vari�veis.
O programa foi pensado para que n�o tivesse problema algum com input incorreto do utilizador.
Todos os "Warnings" do compilador foram resolvidos.

Nas funcionalidades acrescentadas, incluem-se:
- Class Cursor:
 - Esta classe foi feita com o objetivo de auxiliar na manipula��o de um tabuleiro.
 - O cursor pode ser representado por duas coordenadas (x, y) e uma dire��o (H, V).
 - O cursor consegue receber coordenadas do tipo "LcD" e mover-se para essa posi��o.
 - O cursor d� overload ao operador ++ (andar para a frente) e -- (para tr�s) para que a implementa��o da classe board seja independente da dire��o das palavras.
- No cwplayer, o utilizador pode voltar atr�s com Ctrl-Z, caso se tenha enganado na posi��o que escolheu.
- Existe uma fun��o que determina as palavras que foram acrescentadas acidentalmente, ao adicionar outras no tabuleiro, fun��o usada tando no cwcreator como no cwplayer.
- D� para colocar palavras em cima de outras que j� est�o l�, tanto no cwcreator como no cwplayer
- Na cria��o de um Board, o programa determina automaticamente qual o n�mero do Board, pelos ficheiros que j� existem.
- A maior parte do input � Case-insensitive, para facilitar a experi�ncia do utilizador

- Para as pistas, foi acrescentado um sistema de cores para quando s�o mostradas no ecr�:
 - Vazio: Branco
 - Preenchido: Amarelo
 - Certo: Verde *
 - Errado: Vermelho *
- Para cada pista, tamb�m se mostra no ecr� o tamanho da palavra certa, para auxiliar o utilizador
- Para cada pista, tamb�m se mostra no ecr� as letras que j� foram colocadas.
* - Apenas aparecem se, ap�s encher o tabuleiro, o utilizador pedir. Conta como uma "Clue" extra, e aparecem apenas na altura em que o utilizador pede, voltando depois ao amarelo e branco.
Modo f�cil (Easy): 
 - Sempre que o utilizador insere uma palavra, o programa mostra se est� correto ou errado
 - Quando se guarda a informa��o no ficheiro, � acrescentado " | EASY MODE"