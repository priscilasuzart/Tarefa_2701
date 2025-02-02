# Tarefa -  Unidade 04 - Capítulo 04 - Interrupções  

Este projeto foi desenvolvido para controlar uma matriz de LEDs WS2812 e um LED RGB utilizando um microcontrolador Raspberry Pi Pico. O código permite exibir números de 0 a 9 na matriz de LEDs, controlados por dois botões, enquanto um LED vermelho pisca continuamente.  

# Descrição
Foram utilizados os seguintes componentes conectados à placa BitDogLab:  
• Matriz 5x5 de LEDs (endereçáveis) WS2812, conectada à GPIO 7;    
• LED RGB, com os pinos conectados às GPIOs (11, 12 e 13);  
• Botão A conectado à GPIO 5;  
• Botão B conectado à GPIO 6.  

# Funcionalidades  
1) Piscar o LED Vermelho:  
O LED RGB (componente vermelho) pisca 5 vezes por segundo (100 ms de intervalo).
  
2) Exibição de Números na Matriz de LEDs:  
A matriz de LEDs WS2812 exibe números de 0 a 9 em um formato fixo (estilo digital).  
  
3) Controle com Botões:  
Botão A: Incrementa o número exibido (até o máximo de 9).  
Botão B: Decrementa o número exibido (até o mínimo de 0).  
O debouncing é implementado para evitar múltiplas detecções de eventos causados por ruído.

# Como utilizar
1) Piscar do LED Vermelho:  
O LED RGB conectado ao GPIO 13 piscará continuamente (5 vezes por segundo).  
  
2) Controle dos Números na Matriz de LEDs:  
Pressione o Botão A (GPIO 5) para incrementar o número exibido.  
Pressione o Botão B (GPIO 6) para decrementar o número exibido.  

3) Debouncing:  
O código ignora eventos de botão que ocorrem dentro de um intervalo de 200 ms após o último evento válido.

# Exemplo
• Ao pressionar o Botão A, o número na matriz de LEDs muda de 0 para 1.  
• Ao pressionar o Botão B, o número na matriz de LEDs muda de 1 para 0.  
• O LED vermelho pisca continuamente, independentemente da interação com os botões.

# Link do video  
https://drive.google.com/file/d/13qRcvQzw840JN8F1EMGq0wqTSkphDLvV/view?usp=sharing

# Desenvolvido por 
Priscila Pereira Suzart de Carvalho
